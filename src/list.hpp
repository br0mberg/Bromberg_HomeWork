#pragma once // NOLINT

#include <cassert>
#include <iterator>
#include <limits>

namespace task {


template<class T>
class list {
	/// Узел двунаправленного списка
	struct node {
		T value = T();
		node *prev = nullptr;
		node *next = nullptr;

		node() = default;
		node(const T &value, node *prev = nullptr, node *next = nullptr)
			: value(value), prev(prev), next(next) {}

		/// Соединить два узла
		static void wire(node *prev, node *next) {
			if (prev) {
				prev->next = next;
			}
			if (next) {
				next->prev = prev;
			}
		}

		/// Соединить три узла
		static void wire(node *prev, node *curr, node *next) {
			wire(prev, curr);
			wire(curr, next);
		}
	};

	public:
    class iterator {
		// Даём списку доступ к приватному конструктору
		friend class list;

		/// Указатель на узел в списке
        node *_node = nullptr;

		/// Состояние текущего итератора
		enum class state {
			valid,        /// Обычный итератор на валидный элемент
			past_end,     /// Вышли за конец списка
			before_begin  /// Вышли за начало списка
		};
		/// Состояние текущего итератора
		state _state = state::past_end;

		/// Приватный конструктор от узла для списка
		iterator(node *node, state state = state::valid)
		 : _node(node), _state(state) {
			if (nullptr == node) {
				_state = state::past_end;
			}
		}

    	public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator() = default;
        iterator(const iterator &other) : _node(other._node), _state(other._state) {}
        iterator& operator=(const iterator& other) {
			_node = other._node;
			_state = other._state;
			return *this;
		}

        iterator& operator++() {
			switch (_state) {
			// Не делаем ничего, если мы за последним элементом
			case state::past_end: return *this;
			// Только меняем состояние, если перед первым элементом
			case state::before_begin:
				_state = _node ? state::valid : state::past_end;
				return *this;
			case state::valid:
				assert(_node && "No node in valid state");
				// Перемещаемся к следующему, если есть
				if (_node->next) {
					_node = _node->next;
				} else {
					_state = state::past_end;
				}
				return *this;
			default:
				assert(false && "unreachable");
				return *this;
			}
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
			// Не делаем ничего, если мы перед началом
			case state::before_begin: return *this;
			// Только меняем состояние, если за последним элементом
			case state::past_end:
				_state = _node ? state::valid : state::before_begin;
				return *this;
			case state::valid:
				assert(_node && "No node in valid state");
				// Перемещаемся к предыдущему, если есть
				if (_node->prev) {
					_node = _node->prev;
				} else {
					_state = state::before_begin;
				}
				return *this;
			default:
				assert(false && "unreachable");
				return *this;
			}
		}
        iterator operator--(int) {
			iterator tmp(*this);
			--(*this);
			return tmp;
		}

        bool operator==(iterator other) const {
			if (other._state != _state) {
				// При разных состояниях равны только итераторы по-умолчанию
				return _node == nullptr && other._node == nullptr;
			}

			switch (_state) {
			case state::valid: return _node == other._node;
			// В остальных состояниях итераторы равны всегда
			default: return true;
			}
		}
        bool operator!=(iterator other) const { return !(*this == other); }
    };

    class const_iterator {
       	iterator _it;

	   	node *node_it() const { return _it._node; }

		// Даём списку доступ к указателю на узел
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
        bool operator!=(const_iterator other) const { return !(*this == other); }
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


	/// Создать пустой список
    list() = default;

	/// Cоздать список из повторяющихся элементов
    list(size_t count, const T& value) {
		for (size_t i = 0; i < count; ++i) {
			push_back(value);
		}
	}

	/// Cоздать список из повторяющихся элементов по-умолчанию
    explicit list(size_t count) : list(count, T()) {}

	/// Очищаем память
    ~list() { clear(); }


	/// Копируем другой список. Сложность: O(n)
    list(const list& other) : list() {
		// for (auto it = other.cbegin(); it != other.cend(); ++it) {
		// 	push_back(*it);
		// }
		for (const auto &value : other) {
			push_back(value);
		}
	}
    list& operator=(const list& other) {
		list tmp(other);
		swap(tmp);
		return *this;
	}

	/// Получить первый элемент в списке. Сложность: O(1)
    T& front() {
		assert(_head && "Accessing front of empty list");
		return _head->value;
	}
	/// Получить первый элемент в списке. Сложность: O(1)
    const T& front() const { return const_cast<list *>(this)->front(); }

	/// Получить последний элемент в списке. Сложность: O(1)
    T& back() {
		assert(_tail && "Accessing back of empty list");
		return _tail->value;
	}
	/// Получить последний элемент в списке. Сложность: O(1)
    const T& back() const { return const_cast<list *>(this)->back(); }


    iterator begin() const { return iterator(_head); }
    iterator end() const { return iterator(_tail, iterator::state::past_end); }

    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }

    reverse_iterator rbegin() const { return reverse_iterator(end()); }
    reverse_iterator rend() const { return reverse_iterator(begin()); }

    const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }


	/// Пуст ли список? Сложность: O(1)
    bool empty() const { return _size == 0; }

	/// Число элементов в списке. Сложность: O(1)
    size_t size() const { return _size; }

	/// Теоретическое максимальное число элементов в списке. Сложность: O(1)
    static size_t max_size() {
		return std::numeric_limits<size_t>::max();
	}

	/// Очистить список. Сложность: O(n)
    void clear() {
		for (size_t i = 0, n = size(); i < n; ++i) {
			pop_front();
		}
		_head = _tail = nullptr;
	}

	/// Вставить элемент перед позицией. Сложность: O(1)
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

	/// Вставить несколько элементов перед позицией. Сложность: O(count)
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

	/// Удалить элемент. Сложность: O(1)
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

	/// Удалить диапазон элементов. Сложность: O(distance(first, last))
    iterator erase(const_iterator first, const_iterator last) {
		iterator next;
		while (first != last) {
			next = erase(first++);
		}
		return next;
	}

	/// Добавить значение в конец. Сложность: O(1)
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

	/// Удалить значение из конца. Сложность: O(1)
    void pop_back() {
		switch (size()) {
		// Ничего не делаем в пустом списке
		case 0: return;
		// Нужно обнулить голову, если 1 элемент
		case 1: _head = nullptr;
		default: break;
		}

		node *tmp = _tail;
		_tail = _tail->prev;
		node::wire(_tail, nullptr);
		delete tmp;

		--_size;
	}

	/// Добавить значение в начало. Сложность: O(1)
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

	/// Удалить значение из начала. Сложность: O(1)
    void pop_front() {
		switch (size()) {
		// Ничего не делаем в пустом списке
		case 0: return;
		// Нужно обнулить хвост, если 1 элемент
		case 1: _tail = nullptr;
		default: break;
		}

		node *tmp = _head;
		_head = _head->next;
		node::wire(nullptr, _head);
		delete tmp;

		--_size;
	}

	/// Изменить размер списка. Сложность: O(|count - n|)
    void resize(size_t count) {
		if (count >= size()) {  // Увеличиваем список
			for (size_t i = size(); i < count; ++i) {
				push_back(T());
			}
		} else {  // Уменьшаем список
			erase(std::next(cbegin(), count), cend());
		}
	}

	/// Поменять содержимое списков местами. Сложность: O(1)
    void swap(list& other) {
		std::swap(_size, other._size);
		std::swap(_head, other._head);
		std::swap(_tail, other._tail);
	}


	/// Совместить 2 отсортированных списка в 1. Сложность: O(n)
    void merge(list& other) {
		if (other.empty()) { return; }
		if (empty()) {
			*this = std::move(other);
			return;
		}

		/// Меняем местами, чтобы начиналось всегда с меньшего
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

					node::wire(it1.node_it()->prev, node, it1.node_it());
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

	/// Переместить элементы из другого списка. Сложность: O(1)
    void splice(const_iterator pos, list& other) {
		if (other.empty()) { return; }

		node::wire(pos.node_it()->prev, other._head);
	 	node::wire(other._tail, pos.node_it());

		if (pos == cend()) {
			_tail = other._tail;
		}

		_size += other._size;

		other.nullify();
	}

	/// Удалить все копии значения из списка. Сложность: O(n)
    void remove(const T& value) {
		// value может быть ссылкой на элемент внутри контейнера
		// поэтому нужна копия
		T tmp = value;

		for (auto it = cbegin(); it != cend();) {
			if (*it == tmp) {
				erase(it++);
			} else {
				++it;
			}
		}
	}

	/// Развернуть список. Сложность: O(n)
    void reverse() {
		node *cur = _head;
		while (cur) {
			std::swap(cur->next, cur->prev);
			// Тут нужно идти именно по prev,
			// так как мы поменяли местами
			cur = cur->prev;
		}
		std::swap(_head, _tail);
	}

	/// Удалить последовательные дубликаты элементов. Сложность: O(n)
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

	/// Сортируем список по возрастанию. Сложность: O(n*log(n))
    void sort() {
		if (size() <= 1) { return; }

		auto right = split_in_half();

		sort();
		right.sort();

		merge(right);
	}

	private:
	// Нужно хранить размер для гарантии в O(1)
	size_t _size = 0;
	// Указатель на начало списка
	node *_head = nullptr;
	// Указатель на конец списка для быстрой вставки
	node *_tail = nullptr;

	/// Очистить все поля без удаления памяти
	void nullify() {
		_size = 0;
		_head = nullptr;
		_tail = nullptr;
	}

	/// Разбить список пополам. Возвращает правую половину. Сложность: O(n)
	list split_in_half() {
		if (size() <= 1) { return list(); }

		auto median = std::prev(cend(), size() / 2);

		_tail = median.node_it()->prev;
		_tail->next = nullptr;

		median.node_it()->prev = nullptr;

		list right;
		right._head = median.node_it();
		right._size = size() / 2;
		right._tail = cend().node_it();

		_size -= right.size();

		return right;
	}
};

}  // namespace task
