#ifndef SRC_LIST_HPP_
#define SRC_LIST_HPP_

#include <cassert>
#include <iterator>
#include <limits>
#include <algorithm>

namespace task {

template<class T>
class list {
	struct node {
		T value = T();
		node *prev = nullptr;
		node *next = nullptr;

		node() = default;
		node(const T &value, node *prev = nullptr, node *next = nullptr)
			: value(value), prev(prev), next(next) {}


		static void wire(node *prev, node *next) {
			if (prev) {
				prev->next = next;
			}
			if (next) {
				next->prev = prev;
			}
		}

		static void wire(node *prev, node *curr, node *next) {
			wire(prev, curr);
			wire(curr, next);
		}
	};

	public:
    class iterator {
		friend class list;

        node *_node = nullptr;

		enum class state {
			valid,
			past_end,
			before_begin
		};

		state _state = state::past_end;

		iterator(node *node, state state = state::valid)
		 : _node(node), _state(node ? state : state::past_end) {}

    	public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator() = default;
        iterator(const iterator &other) : _node(other._node), _state(other._state) {}
        iterator& operator=(const iterator& other) = default;

        iterator& operator++() {
			switch (_state) {
			case state::past_end: {
				return *this;
			}
			case state::before_begin: {
				_state = _node ? state::valid : state::past_end;
				return *this;
			}
			case state::valid: {
				assert(_node && "No node in valid state");
				if (_node->next) {
					_node = _node->next;
				} else {
					_state = state::past_end;
				}
				return *this;
			}
			}
			// Unreachable
			abort();
			return *this;
		}
        iterator operator++(int) {
			iterator tmp(*this);
			++(*this);
			return tmp;
		}

        reference operator*() const {
			assert(_state == state::valid && "Dereferencing iterator in invalid state");
			return _node->value;
		}
        pointer operator->() const { return &this->operator*(); }

        iterator& operator--() {
			switch (_state) {
				case state::before_begin: {
					return *this;
				}
				case state::past_end: {
					_state = _node ? state::valid : state::before_begin;
					return *this;
				}
				case state::valid: {
					assert(_node && "No node in valid state");
					if (_node->prev) {
						_node = _node->prev;
					} else {
						_state = state::before_begin;
					}
					return *this;
				}
			}
			// Unreachable
			abort();
			return *this;
		}
        iterator operator--(int) {
			iterator tmp(*this);
			--(*this);
			return tmp;
		}

        bool operator==(iterator other) const {
			if (other._state != _state) {
				return _node == nullptr && other._node == nullptr;
			}

			if (_state == state::valid) {
				return _node == other._node;
			} else if (_state == state::past_end
			|| _state == state::before_begin) {
				return true;
			}

			abort();
			return false;
		}
        bool operator!=(iterator other) const { return !(*this == other); }
    };

    class const_iterator {
       	iterator _it;

	   	node *node_it() const { return _it._node; }

		struct node *node_or_null() const {
			if (_it._state == iterator::state::valid) { return _it._node; }
			return nullptr;
		}

		struct node *prev_node() const {
			if (is_past_end()) { return _it._node; }
			return node_it() ? _it._node->prev : nullptr;
		}

		friend list;

     public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::bidirectional_iterator_tag;

        const_iterator() = default;
		const_iterator(iterator it) : _it(it) {}
        const_iterator(const const_iterator &other) : _it(other._it) {}
        const_iterator& operator=(const const_iterator& other) {
			_it = other._it;
			return *this;
		}

        const_iterator& operator++() {
			++_it;
			return *this;
		}
        const_iterator operator++(int) {
			const_iterator tmp(*this);
			++(*this);
			return tmp;
		}

        reference operator*() const { return *_it; }
        pointer operator->() const { return &this->operator*(); }

        const_iterator& operator--() {
			--_it;
			return *this;
		}
        const_iterator operator--(int) {
			const_iterator tmp(*this);
			--(*this);
			return tmp;
		}

        bool operator==(const_iterator other) const {
			return _it == other._it;
		}

        bool operator!=(const_iterator other) const {
			return !(*this == other);
		}

		bool is_past_end() const noexcept {
			return _it._state == iterator::state::past_end;
		}
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    list() = default;

    list(size_t count, const T& value) {
		for (size_t i = 0; i < count; ++i) {
			push_back(value);
		}
	}

    explicit list(size_t count) : list(count, T()) {}

    ~list() { clear(); }

    list(const list& other) : list() {
		for (const auto &value : other) {
			push_back(value);
		}
	}

    list& operator=(const list& other) {
		list tmp(other);
		swap(tmp);
		return *this;
	}

    T& front() {
		assert(_head && "Accessing front of empty list");
		return _head->value;
	}

    const T& front() const {
		assert(_head && "Accessing front of empty list");
		return _head->value;
	}

    T& back() {
		assert(_tail && "Accessing back of empty list");
		return _tail->value;
	}

    const T& back() const {
		assert(_tail && "Accessing back of empty list");
		return _tail->value;
	}


    iterator begin() const { return iterator(_head); }
    iterator end() const { return iterator(_tail, iterator::state::past_end); }

    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }

    reverse_iterator rbegin() const { return reverse_iterator(end()); }
    reverse_iterator rend() const { return reverse_iterator(begin()); }

    const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

    bool empty() const { return _size == 0; }

    size_t size() const { return _size; }

    static size_t max_size() {
		return std::numeric_limits<size_t>::max();
	}

    void clear() {
		for (size_t i = 0, n = size(); i < n; ++i) {
			pop_front();
		}
		_head = _tail = nullptr;
	}

    iterator insert(const_iterator pos, const T& value) {
		if (pos == cbegin()) {
			push_front(value);
			return begin();
		}
		if (pos == cend()) {
			push_back(value);
			return --end();
		}

		auto node = new struct node(value);

		node::wire(pos.node_it(), node, pos.node_it()->next);

		++_size;

		return iterator(node);
	}

    iterator insert(const_iterator pos, size_t count, const T& value) {
		auto it = pos._it;
		for (size_t i = 0; i < count; ++i) {
			auto cur = insert(pos, value);
			if (i == 0) {
				it = cur;
			}
		}
		return it;
	}

    iterator erase(const_iterator pos) {
		if (pos == cend()) { return end(); }

		if (pos == cbegin()) {
			_head = pos.node_it()->next;
		}

		if (std::next(pos) == cend()) {
			_tail = pos.node_it()->prev;
		}

		auto next = std::next(pos);

		node::wire(pos.node_it()->prev, pos.node_it()->next);
		delete pos.node_it();

		--_size;

		if (_size == 0) {
			nullify();
		}

		return next._it;
	}

    iterator erase(const_iterator first, const_iterator last) {
		iterator next;
		while (first != last) {
			next = erase(first++);
		}
		return next;
	}

    void push_back(const T& value) {
		node *node = new struct node(value, _tail);

		if (_tail) {
			_tail->next = node;
		} else {
			_head = node;
		}

		_tail = node;
		++_size;
	}

    void pop_back() {
		switch (size()) {
			case 0: {
				return;
			}
			case 1: {
				_head = nullptr;
			}
			default: {
				break;
			}
		}

		node *tmp = _tail;
		_tail = _tail->prev;
		node::wire(_tail, nullptr);
		delete tmp;

		--_size;
	}

    void push_front(const T& value) {
		node *node = new struct node(value, nullptr, _head);

		if (_head) {
			_head->prev = node;
		} else {
			_tail = node;
		}

		_head = node;
		++_size;
	}

    void pop_front() {
		switch (size()) {
		case 0: return;
		case 1: _tail = nullptr;
		default: break;
		}

		node *tmp = _head;
		_head = _head->next;
		node::wire(nullptr, _head);
		delete tmp;

		--_size;
	}

    void resize(size_t count) {
		if (count >= size()) {
			for (size_t i = size(); i < count; ++i) {
				push_back(T());
			}
		} else {
			erase(std::next(cbegin(), count), cend());
		}
	}

    void swap(list& other) {
		std::swap(_size, other._size);
		std::swap(_head, other._head);
		std::swap(_tail, other._tail);
	}

    void merge(list& other) {
		if (other.empty()) { return; }
		if (empty()) {
			*this = std::move(other);
			return;
		}

		if (front() > other.front()) {
			swap(other);
		}

		auto it1 = cbegin();
		auto it2 = other.cbegin();
		while (it1 != cend() && it2 != other.cend()) {
			if (*it2 < *it1) {
				do {
					auto node = it2.node_it();
					++it2;

					node::wire(it1.prev_node(), node, it1.node_it());
				} while (it2 != other.cend() && *it2 < *it1);
			} else {
				++it1;
			}
		}

		if (it2 != other.cend()) {
			node::wire(_tail, it2.node_it());
			_tail = other._tail;
		}

		_size += other._size;

		other.nullify();
	}

    void splice(const_iterator pos, list& other) {
		if (other.empty()) { return; }

		node::wire(pos.prev_node(), other._head);
	 	node::wire(other._tail, pos.node_or_null());

		if (pos == cbegin()) {
			_head = other._head;
		}

		if (pos == cend()) {
			_tail = other._tail;
		}

		_size += other._size;

		other.nullify();
	}

    void remove(const T& value) {
		T tmp = value;

		for (auto it = cbegin(); it != cend();) {
			if (*it == tmp) {
				erase(it++);
			} else {
				++it;
			}
		}
	}

    void reverse() {
		node *cur = _head;
		while (cur) {
			std::swap(cur->next, cur->prev);
			cur = cur->prev;
		}
		std::swap(_head, _tail);
	}

    void unique() {
		for (auto prev = cbegin(), cur = std::next(prev); cur != cend();) {
			if (*cur == *prev) {
				erase(cur++);
			} else {
				++cur;
				++prev;
			}
		}
	}

    void sort() {
		if (size() <= 1) { return; }

		size_t n = size();

		for (size_t block_size = 1; block_size < n; block_size *= 2) {
			size_t element_index = 0;
			for (
				auto block_pair_start = cbegin();
				block_pair_start != cend();
			) {
				auto second_block_start = std::next(block_pair_start, block_size);
				auto l = extract(
					block_pair_start, second_block_start,
					std::min(block_size, n - element_index));
				element_index += block_size;

				auto second_block_end = std::next(second_block_start, block_size);
				auto r = extract(
					second_block_start, second_block_end,
					element_index >= n ? 0 : std::min(block_size, n - element_index));
				element_index += block_size;

				l.merge(r);

				splice(second_block_end.is_past_end() ? cend() : second_block_end, l);

				block_pair_start = second_block_end;
			}
		}
	}

	private:
	size_t _size = 0;

	node *_head = nullptr;
	node *_tail = nullptr;

	void nullify() {
		_size = 0;
		_head = nullptr;
		_tail = nullptr;
	}

	list extract(const_iterator first, const_iterator last, size_t len) {
		if (len == 0 || first == last) { return list(); }

		auto l = list();
		l._size = len;
		l._head = first.node_it();
		l._tail = last.prev_node();

		if (first == cbegin()) {
			_head = last.node_or_null();
		}
		if (last == cend()) {
			_tail = first.prev_node();
		}

		node::wire(l._head->prev, l._tail->next);
		l._head->prev = nullptr;
		l._tail->next = nullptr;

		_size -= len;

		return l;
	}
};

}  // namespace task

#endif  // SRC_LIST_HPP_
