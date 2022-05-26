#pragma once // NOLINT
#include <iterator>

namespace task {

template<class T>
class list {
     struct Node;
 protected:
     Node* node;
 public:
    class iterator {
        friend class list;
        friend class const_list;
     public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator(): node() {}
        explicit iterator(const Node* other): node(other) {}
        iterator(const iterator& it): node(it.node) {}
        iterator& operator=(const iterator& it) {
           node = it.node;
        }

        iterator& operator++() {
           node = node->next;
           return *this;
        }

        iterator operator++(int) {
            self tmp = *this;
	         node = node->next;
	         return tmp;
        }

        reference operator*() const {
           return *static_cast<Node*>(node)->item();
        }

        pointer operator->() const {
           return static_cast<Node*>(node)->item();
        }

        iterator& operator--() {
            node = node->prev;
	         return *this;
        }

        iterator operator--(int) {
            self tmp = *this;
	         node = node->prev;
	         return tmp;
        }

        bool operator==(const iterator& other) const {
            return (*this).node == other.node;
        }

        bool operator!=(const iterator& other) const {
            return (*this).node != other.node;
        }

     private:
         typedef iterator self;
         Node* node;
    };

    class const_iterator {
     public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        const_iterator(): node() {}
        explicit const_iterator(const Node* other): node(other) {}
        const_iterator(const iterator& it): node(it.node) {}
        const_iterator& operator++() {
            node = node->next;
	         return *this;
        }
        const_iterator operator++(int) {
            self tmp = *this;
	         node = node->next;
	         return tmp;
        }
        reference operator*() const {
           return *static_cast<Node*>(node)->item();
        }
        pointer operator->() const {
           return static_cast<Node*>(node)->item();
        }
        const_iterator& operator--() {
            node = node->prev;
	         return *this;
        }
        const_iterator operator--(int) {
            self tmp = *this;
	         node = node->prev;
	         return tmp;
        }

        bool operator==(const const_iterator& other) const {
            return (*this).node == other.node;
        }
        bool operator!=(const const_iterator& other) const {
           return (*this).node != other.node;
        }

     private:
         typedef const_iterator self;
         Node*	node;
         typedef iterator iterator;
    };

    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;

    list() = default;
    list(size_t count, const T& value) {
       // TODO
    }
    explicit list(size_t count) {
       // TODO
    }
    ~list() = default;

    list(const list& other);
    list& operator=(const list& other) {
         this->assign(other.begin(), other.end());
	      return *this;
    }

    void fill_assign(size_t count, const T& value) {
      iterator i = begin();
      for (; i != end() && count > 0; ++i, --count)
        *i = value;
      if (count > 0)
        insert(end(), count, value);
      else
        // TODO
    }

    void assign(size_t count, const T& value) {
      fill_assign(count, value);
    }


    T& front() { return *begin();}
    const T& front() const { { return *begin();}}

    T& back() {
       iterator tmp = end();
	    --tmp;
	    return *tmp;
    }

    const T& back() const {
       const_iterator tmp = end();
	    --tmp;
	    return *tmp;
    }


    iterator begin() const;
    iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin() const;
    reverse_iterator rend() const;

    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;


    bool empty() const;
    size_t size() const;
    size_t max_size() const;
    void clear();

    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, size_t count, const T& value);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);


    void push_back(const T& value);
    void pop_back();

    void push_front(const T& value);
    void pop_front();

    void resize(size_t count);
    void swap(list& other);


    void merge(list& other);
    void splice(const_iterator pos, list& other);
    void remove(const T& value);
    void reverse();
    void unique();
    void sort();

    // Your code goes here?..

 private:
    struct Node {
		T item;	 // Содержимое узла
		Node* prev;	 // Указатель на предыдущий узел
		Node* next;	 // Указатель на следующий узел
	};

	Node* first;   // Указатель на первый узел списка
	Node* last;	 // Указатель на последний узел списка
	size_t length;  // Размер списка
    // Your code goes here...
};

template<class T>
list<T>::list() {}
// Your template function definitions may go here...

}  // namespace task
