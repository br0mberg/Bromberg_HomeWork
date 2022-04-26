#include "../include/emlparse.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    L_FROM,
    L_TO,
    L_DATE,
    L_CONTENT,
    L_BOUNDARY,
    L_EMPTYSTR,
    L_NULL,
    L_COUNT,
    L_ERR
} lexem_t;

typedef enum {
    S_BEGIN,
    S_FROM,
    S_TO,
    S_DATE,
    S_CONTENT,
    S_BOUNDARY,
    S_BODY,
    S_NULL,
    S_COUNT,
    S_ERR,
    S_END
} state_t;

typedef bool (*action_t)(const char *s, const char **end, cjson_callback_t callback);

typedef struct {
    state_t state;
    action_t action;
} rule_t;

static lexem_t get_lexem(const char *s, const char **end);
static lexem_t get_string(const char *s, const char **end);
static lexem_t get_value(const char *s, const char **end);

static bool extract_json(const char *s, const char **end, cjson_callback_t callback);

static bool on_obegin(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_OBEGIN); }
static bool on_oend(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_OEND); }
static bool on_abegin(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_ABEGIN); }
static bool on_aend(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_AEND); }
static bool on_str(const char *s, const char **end, cjson_callback_t callback) { return callback(s + 1, *end - 1, CJSON_EVENT_STR); }
static bool on_bool(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_BOOL); }
static bool on_null(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_NULL); }
static bool on_num(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_NUM); }

static rule_t transitions[S_COUNT][L_COUNT] = {
//                L_FROM                           L_TO                         L_DATE                         L_CONTENT                      L_BOUNDARY              L_EMPTYSTR                           L_NULL
/* S_BEGIN */     {{ S_FROM, extract_json },       { S_TO, extract_json },      { S_DATE, extract_json },      { S_CONTENT, extract_json },   { S_ERR, NULL },        { S_END, NULL },                     { S_END, NULL }},
/* S_FROM */      {{ S_ERR, NULL },                { S_TO, NULL },              { S_DATE, NULL },              { S_CONTENT, NULL },           { S_ERR, NULL },        { S_END, NULL },                     { S_END, NULL }},
/* S_TO */        {{ S_FROM, NULL },               { S_ERR, NULL },             { S_DATE, NULL },              { S_CONTENT, NULL },           { S_ERR, NULL },        { S_END, NULL },                     { S_END, NULL }},
/* S_DATE */      {{ S_FROM, NULL },               { S_TO, NULL },              { S_ERR, NULL },               { S_CONTENT, NULL },           { S_ERR, NULL },        { S_END, NULL },                     { S_END, NULL }},
/* S_CONTENT */   {{ S_FROM, NULL },               { S_TO, NULL },              { S_DATE, NULL },              { S_ERR, NULL },               { S_BOUNDARY, NULL },   { S_BODY, extract_json },            { S_END, NULL }},
/* S_BOUNDARY */  {{ S_FROM, NULL },               { S_TO, NULL },              { S_DATE, NULL },              { S_ERR, NULL },               { S_ERR, NULL },        { S_ERR, NULL },                     { S_END, NULL }},
/* S_BODY */      {{ S_ERR, NULL },                { S_ERR, NULL },             { S_ERR, NULL },               { S_ERR, NULL },               { S_ERR, NULL },        { S_ERR, NULL },                     { S_END, NULL }},
/* S_END */       {{ S_ERR, NULL },                { S_ERR, NULL },             { S_ERR, NULL },               { S_ERR, NULL },               { S_ERR, NULL },        { S_ERR, NULL },                     { S_ERR, NULL }}
};

static bool is_hex(char sym) {
    return (sym >= '0' && sym <= '9') || (sym >= 'A' && sym <= 'F') || (sym >= 'a' && sym <= 'f');
}

static lexem_t get_string(const char *s, const char **end) {
    ++s;
    while (*s != '\0' && *s != '"') {
        if (*s == '\\') {
            char next = *(s + 1);
            if (next == '"' || next == '\\' || next == '/' || next == 'b' || next == 'f' || next == 'n' || next == 'r' || next == 't') {
                ++s;
            } else if (next == 'u' && is_hex(*(s + 2)) && is_hex(*(s + 3)) && is_hex(*(s + 4)) && is_hex(*(s + 5))) {
                 s += 4;
            } else {
                 return L_ERR;
            }
        }
        ++s;
    }
    if (*s == '\0') {
        return L_ERR;
    }
    *end = s + 1;
    return L_STR;
}

static lexem_t get_value(const char *s, const char **end) {
    if (strncmp(s, "true", sizeof("true") - 1) == 0) {
        *end = s + sizeof("true") - 1;
        return L_BOOL;
    }

    if (strncmp(s, "false", sizeof("false") - 1) == 0) {
        *end = s + sizeof("false") - 1;
        return L_BOOL;
    }

    if (strncmp(s, "null", sizeof("null") - 1) == 0) {
        *end = s + sizeof("null") - 1;
        return L_NULL;
    }

    char *tmp;
    strtod(s, &tmp);
    if (tmp == s) {
        return L_ERR;
    }
    *end = tmp;
    return L_NUM;
}

// Р›РµРєСЃРµРјС‹: { } [ ] : , STR NUM BOOL NULL
static lexem_t get_lexem(const char *s, const char **end) {
    if (!s || !end) { return L_ERR; }

    *end = s + 1;
    switch (*s) {
        case '{': return L_OBEGIN;
        case '}': return L_OEND;
        case '[': return L_ABEGIN;
        case ']': return L_AEND;
        case ':': return L_COLON;
        case ',': return L_COMMA;
        case '"': return get_string(s, end);
        default: return get_value(s, end);
    }

    return L_ERR;
}

static bool extract_json(const char *s, const char **end, cjson_callback_t callback) {
    state_t state = S_BEGIN;
    while (*s) {
        while (isspace(*s)) {
            ++s;
        }
        lexem_t lexem = get_lexem(s, end);
        if (lexem == L_ERR) {
            return false;
        }
        rule_t rule = transitions[state][lexem];
        if (rule.state == S_ERR) {
            return false;
        }
        if (rule.action != NULL) {
            if (!rule.action(s, end, callback)) {
                return false;
            }
        }
        state = rule.state;
        if (rule.state == S_END) {
            break;
        }
        s = *end;
    }
    return state == S_END;
}
*/
bool cjson_parse(const char *s, cjson_callback_t callback) {
    if (!s) {
        return false;
    }
    const char *end;
    if (!extract_json(s, &end, callback)) {
        return false;
    }
    while (isspace(*end)) {
        ++end;
    }
    return *end == '\0';
}

