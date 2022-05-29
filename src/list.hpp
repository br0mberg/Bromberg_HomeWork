#pragma once // NOLINT
#include <iterator>
#include <iostream>

namespace task {

struct _List_node_base {
      _List_node_base* _M_next;
      _List_node_base* _M_prev;

      static void
      swap(_List_node_base& __x, _List_node_base& __y) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_transfer(_List_node_base* const __first,
		  _List_node_base* const __last) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_reverse() _GLIBCXX_USE_NOEXCEPT;

      void
      _M_hook(_List_node_base* const __position) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_unhook() _GLIBCXX_USE_NOEXCEPT;
    };

struct _List_node_header : public _List_node_base {
      std::size_t _M_size;
      _List_node_header() _GLIBCXX_NOEXCEPT { 
         _M_init();
       }

      _List_node_header(_List_node_header&& __x): _List_node_base { __x._M_next, __x._M_prev } , _M_size(__x._M_size) {
	   if (__x._M_base()->_M_next == __x._M_base()) {
         this->_M_next = this->_M_prev = this;
      } else {
	      this->_M_next->_M_prev = this->_M_prev->_M_next = this->_M_base();
	      __x._M_init();
	   }
   }

   void _M_move_nodes(_List_node_header&& __x) {
	_List_node_base* const __xnode = __x._M_base();
	if (__xnode->_M_next == __xnode) {
      _M_init();
   } else {
	    _List_node_base* const __node = this->_M_base();
	    __node->_M_next = __xnode->_M_next;
	    __node->_M_prev = __xnode->_M_prev;
	    __node->_M_next->_M_prev = __node->_M_prev->_M_next = __node;
	    _M_size = __x._M_size;
	    __x._M_init();
	  }
   }

   void _M_init() {
	   this->_M_next = this->_M_prev = this;
	   this->_M_size = 0;
   }

    private:
      _List_node_base* _M_base() { return this; }
    };

    struct _Scratch_list : _List_node_base {
      _Scratch_list() { _M_next = _M_prev = this; }

      bool empty() const { return _M_next == this; }

      void swap(_List_node_base& __l) { _List_node_base::swap(*this, __l); }

      template<typename _Iter, typename _Cmp>
	   struct _Ptr_cmp {
	      _Cmp _M_cmp;

	      bool operator()(_List_node_base* __lhs, _List_node_base* __rhs) {
	         return _M_cmp(*_Iter(__lhs), *_Iter(__rhs)); 
          }
	   };

      template<typename _Iter>
	   struct _Ptr_cmp<_Iter, void> {
	      bool operator()(_List_node_base* __lhs, _List_node_base* __rhs) const { 
            return *_Iter(__lhs) < *_Iter(__rhs); 
         }
	   };

      template<typename _Cmp>
	   void merge(_List_node_base& __x, _Cmp __comp) {
	      _List_node_base* __first1 = _M_next;
	      _List_node_base* const __last1 = this;
	      _List_node_base* __first2 = __x._M_next;
	      _List_node_base* const __last2 = std::__addressof(__x);

	      while (__first1 != __last1 && __first2 != __last2) {
	          if (__comp(__first2, __first1)) {
		         _List_node_base* __next = __first2->_M_next;
		         __first1->_M_transfer(__first2, __next);
		         __first2 = __next;
		      } else {
               __first1 = __first1->_M_next;
            }
	      }
	  if (__first2 != __last2)
	    this->_M_transfer(__first2, __last2);
	}

      void _M_take_one(_List_node_base* __i) { 
         this->_M_transfer(__i, __i->_M_next); 
      }

      void _M_put_all(_List_node_base* __i) {
	      if (!empty()) {
            __i->_M_transfer(_M_next, this);
         }
      }
   };

template<typename T>
struct Node : public _List_node_base {
  T _M_data;
  T*       _M_valptr()       { return std::__addressof(_M_data); }
  T const* _M_valptr() const { return std::__addressof(_M_data); }
};

template<typename T, typename _Alloc>
    class _List_base {
    protected:
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<T>::other	_Tp_alloc_type;
      typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type>	_Tp_alloc_traits;
      typedef typename _Tp_alloc_traits::template rebind<Node<T> >::other _Node_alloc_type;
      typedef __gnu_cxx::__alloc_traits<_Node_alloc_type> _Node_alloc_traits;

      static size_t
      _S_distance(const _List_node_base* __first,
		  const _List_node_base* __last) {
	      size_t __n = 0;
	      while (__first != __last) {
	         __first = __first->_M_next;
	         ++__n;
	      }
	      return __n;
      }

      struct _List_impl: public _Node_alloc_type {
	      _List_node_header _M_node;
         _List_impl() _GLIBCXX_NOEXCEPT_IF(std::is_nothrow_default_constructible<_Node_alloc_type>::value)
	         : _Node_alloc_type() { }

	      _List_impl(const _Node_alloc_type& __a): _Node_alloc_type(__a) { }

	      _List_impl(_List_impl&&) = default;

	      _List_impl(_Node_alloc_type&& __a, _List_impl&& __x)
            : _Node_alloc_type(std::move(__a)), _M_node(std::move(__x._M_node)) { }

	      _List_impl(_Node_alloc_type&& __a): _Node_alloc_type(std::move(__a)) { }
      };

      _List_impl _M_impl;

      size_t _M_get_size() const { return _M_impl._M_node._M_size; }

      void _M_set_size(size_t __n) { _M_impl._M_node._M_size = __n; }

      void _M_inc_size(size_t __n) { _M_impl._M_node._M_size += __n; }

      void _M_dec_size(size_t __n) { _M_impl._M_node._M_size -= __n; }

      size_t _M_distance(const _List_node_base* __first,
		  const _List_node_base* __last) const { 
           return _S_distance(__first, __last); 
         }

      size_t _M_node_count() const { return _M_get_size(); }

      typename _Node_alloc_traits::pointer _M_get_node()
      { return _Node_alloc_traits::allocate(_M_impl, 1); }

      void _M_put_node(typename _Node_alloc_traits::pointer __p)
      { _Node_alloc_traits::deallocate(_M_impl, __p, 1); }

  public:
      typedef _Alloc allocator_type;

      _Node_alloc_type& _M_get_Node_allocator() { return _M_impl; }

      const _Node_alloc_type& _M_get_Node_allocator() const { return _M_impl; }

      _List_base() = default;

      _List_base(const _Node_alloc_type& __a): _M_impl(__a) { }

      _List_base(_List_base&&) = default;

      _List_base(_List_base&& __x, _Node_alloc_type&& __a)
      : _M_impl(std::move(__a)) {
	      if (__x._M_get_Node_allocator() == _M_get_Node_allocator()) {
            _M_move_nodes(std::move(__x));
         }
      }

      _List_base(_Node_alloc_type&& __a, _List_base&& __x): _M_impl(std::move(__a), std::move(__x._M_impl)) { }

      _List_base(_Node_alloc_type&& __a) : _M_impl(std::move(__a)) { }

      void _M_move_nodes(_List_base&& __x){ _M_impl._M_node._M_move_nodes(std::move(__x._M_impl._M_node)); }

      ~_List_base(){ _M_clear(); }

      void _M_clear();

      void _M_init() { this->_M_impl._M_node._M_init(); }
    };

template<typename T>
    struct _List_iterator {
      typedef _List_iterator<T>		_Self;
      typedef Node<T>			_Node;

      typedef ptrdiff_t	difference_type;
      typedef std::bidirectional_iterator_tag	iterator_category;
      typedef T	value_type;
      typedef T* pointer;
      typedef T& reference;

      _List_iterator(): _M_node() { }

      explicit _List_iterator(_List_node_base* __x)
      : _M_node(__x) { }

      _Self _M_const_cast() const { return *this; }

      reference operator*() const { return *static_cast<_Node*>(_M_node)->_M_valptr(); }

      pointer operator->() const { return static_cast<_Node*>(_M_node)->_M_valptr(); }

      _Self& operator++() {
	      _M_node = _M_node->_M_next;
	      return *this;
      }

      _Self operator++(int) {
	      _Self __tmp = *this;
	      _M_node = _M_node->_M_next;
	      return __tmp;
      }

      _Self& operator--() {
	      _M_node = _M_node->_M_prev;
	      return *this;
      }

      _Self operator--(int) {
	      _Self __tmp = *this;
	      _M_node = _M_node->_M_prev;
	      return __tmp;
      }

      friend bool operator==(const _Self& __x, const _Self& __y)
      { return __x._M_node == __y._M_node; }

      friend bool operator!=(const _Self& __x, const _Self& __y) _GLIBCXX_NOEXCEPT
      { return __x._M_node != __y._M_node; }

      _List_node_base* _M_node;
    };

template<typename _Tp>
struct _List_const_iterator {
      typedef _List_const_iterator<_Tp>		_Self;
      typedef const Node<_Tp>		_Node;
      typedef _List_iterator<_Tp>		iterator;

      typedef ptrdiff_t				difference_type;
      typedef std::bidirectional_iterator_tag	iterator_category;
      typedef _Tp				value_type;
      typedef const _Tp*			pointer;
      typedef const _Tp&			reference;

      _List_const_iterator()
      : _M_node() { }

      explicit _List_const_iterator(const _List_node_base* __x)
      : _M_node(__x) { }

      _List_const_iterator(const iterator& __x)
      : _M_node(__x._M_node) { }

      iterator _M_const_cast() const
      { return iterator(const_cast<_List_node_base*>(_M_node)); }

      reference operator*() const
      { return *static_cast<_Node*>(_M_node)->_M_valptr(); }

      pointer operator->() const
      { return static_cast<_Node*>(_M_node)->_M_valptr(); }

      _Self& operator++() {
         _M_node = _M_node->_M_next;
	      return *this;
      }

      _Self operator++(int) {
	      _Self __tmp = *this;
	      _M_node = _M_node->_M_next;
	      return __tmp;
      }

      _Self& operator--() {
	      _M_node = _M_node->_M_prev;
	      return *this;
      }

      _Self operator--(int) {
	      _Self __tmp = *this;
	      _M_node = _M_node->_M_prev;
	      return __tmp;
      }

      friend bool operator==(const _Self& __x, const _Self& __y)
      { return __x._M_node == __y._M_node; }

      friend bool operator!=(const _Self& __x, const _Self& __y)
      { return __x._M_node != __y._M_node; }

      const _List_node_base* _M_node;
    };

template<typename T, typename _Alloc = std::allocator<T> >
   class list : protected _List_base<T, _Alloc> {
      typedef typename _Alloc::value_type		_Alloc_value_type;
      __glibcxx_class_requires(T, _SGIAssignableConcept)

      typedef _List_base<T, _Alloc>			_Base;
      typedef typename _Base::_Tp_alloc_type		_Tp_alloc_type;
      typedef typename _Base::_Tp_alloc_traits		_Tp_alloc_traits;
      typedef typename _Base::_Node_alloc_type		_Node_alloc_type;
      typedef typename _Base::_Node_alloc_traits	_Node_alloc_traits;
    public:
      typedef T value_type;
      typedef typename _Tp_alloc_traits::pointer	 pointer;
      typedef typename _Tp_alloc_traits::const_pointer	 const_pointer;
      typedef typename _Tp_alloc_traits::reference	 reference;
      typedef typename _Tp_alloc_traits::const_reference const_reference;
      typedef _List_iterator<T> iterator;
      typedef _List_const_iterator<T> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef std::reverse_iterator<iterator> reverse_iterator;
      typedef size_t	size_type;
      typedef ptrdiff_t	difference_type;
      typedef _Alloc	allocator_type;
    protected:

      typedef Node<T>	_Node;

      using _Base::_M_impl;
      using _Base::_M_put_node;
      using _Base::_M_get_node;
      using _Base::_M_get_Node_allocator;

      _Node* _M_create_node(const T& __x) {
	      _Node* __p = this->_M_get_node();
	      __try {
	         _Tp_alloc_type __alloc(_M_get_Node_allocator());
	         __alloc.construct(__p->_M_valptr(), __x);
	      }
	      __catch(...) {
	         _M_put_node(__p);
	         __throw_exception_again;
	      }
	      return __p;
      }

      static size_t _S_distance(const_iterator __first, const_iterator __last)
      { return std::distance(__first, __last); }

      // return the stored size
      size_t _M_node_count() const
      { return this->_M_get_size(); }
 public:
    
    void _M_default_initialize(size_type __n) {
	   for (; __n; --__n) {
	      emplace_back();
      }
    }
    
    iterator end();

    reference back() {
        iterator __tmp = end();
        --__tmp;
        return *__tmp;
      }

    template<typename... _Args>
    reference emplace_back(_Args&&... __args) {
	  this->_M_insert(end(), std::forward<_Args>(__args)...);
	   return back();
	 }

    list() = default;

    explicit list(const allocator_type& __a): _Base(_Node_alloc_type(__a)) { }

    list(size_type __n, const value_type& __value,
	   const allocator_type& __a = allocator_type())
      : _Base(_Node_alloc_type(__a))
      { _M_fill_initialize(__n, __value); }

    explicit list(size_type __n, const allocator_type& __a = allocator_type())
      : _Base(_Node_alloc_type(__a))
      { _M_default_initialize(__n); }

    ~list() = default;

    list(const list& __x)
      : _Base(_Node_alloc_traits::
	      _S_select_on_copy(__x._M_get_Node_allocator()))
      { _M_initialize_dispatch(__x.begin(), __x.end(), std::__false_type()); }

    list& operator=(const list& other) {
       this->assign(other.begin(), other.end());
	    return *this;
    }

    list(list&&) = default;

    list(std::initializer_list<T> l, const allocator_type& a = allocator_type()): _Base(_Node_alloc_type(a))
      { _M_initialize_dispatch(l.begin(), l.end(), std::__false_type()); }

    list(const list& __x, const std::type_identity_t<allocator_type>& __a)
      : _Base(_Node_alloc_type(__a))
      { _M_initialize_dispatch(__x.begin(), __x.end(), std::__false_type()); }

   private:
      list(list&& __x, const allocator_type& __a, std::true_type) noexcept
      : _Base(_Node_alloc_type(__a), std::move(__x))
      { }

      list(list&& __x, const allocator_type& __a, std::false_type)
      : _Base(_Node_alloc_type(__a)) {
	      if (__x._M_get_Node_allocator() == this->_M_get_Node_allocator())
	         this->_M_move_nodes(std::move(__x));
	      else
	         insert(begin(), std::__make_move_if_noexcept_iterator(__x.begin()), std::__make_move_if_noexcept_iterator(__x.end()));
      }

   public:
    list(list&& __x, const std::type_identity_t<allocator_type>& __a)
      noexcept(_Node_alloc_traits::_S_always_equal())
      : list(std::move(__x), __a,
	     typename _Node_alloc_traits::is_always_equal{})
      { }

   template<typename _InputIterator>
	 list(_InputIterator __first, _InputIterator __last, const allocator_type& __a = allocator_type())
	: _Base(_Node_alloc_type(__a)) {
	  typedef typename std::__is_integer<_InputIterator>::__type _Integral;
	  _M_initialize_dispatch(__first, __last, _Integral());
	}

    list& operator=(list&& __x)
      noexcept(_Node_alloc_traits::_S_nothrow_move()) {
	   constexpr bool __move_storage = _Node_alloc_traits::_S_propagate_on_move_assign()
	      || _Node_alloc_traits::_S_always_equal();
	      _M_move_assign(std::move(__x), __bool_constant<__move_storage>());
	      return *this;
      }

    list& operator=(initializer_list<value_type> __l) {
	      this->assign(__l.begin(), __l.end());
	      return *this;
      }

    void assign(size_type __n, const value_type& __val) { _M_fill_assign(__n, __val); }
    void assign(initializer_list<value_type> __l) { this->_M_assign_dispatch(__l.begin(), __l.end(), __false_type()); }

    void assign(size_type __n, const value_type& __val)
      { _M_fill_assign(__n, __val); }

    allocator_type get_allocator() const
      { return allocator_type(_Base::_M_get_Node_allocator()); }

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

    iterator begin() { return iterator(this->_M_impl._M_node._M_next); }


    iterator end() { return iterator(&this->_M_impl._M_node); }

    const_iterator cbegin() const { return const_iterator(this->_M_impl._M_node._M_next); }
    const_iterator cend() const { return const_iterator(&this->_M_impl._M_node); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() const {
       return reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const {
       return const_reverse_iterator(end());
    }

    const_reverse_iterator crend() const {
       return const_reverse_iterator(begin());
    }


    bool empty() const { return this->_M_impl._M_node._M_next == &this->_M_impl._M_node; }

    size_t size() const { return _M_node_count(); }

    size_type max_size() const { return _Node_alloc_traits::max_size(_M_get_Node_allocator()); }

    void clear() {
      Node* cur = first;
      while (cur != last) {
          Node* tmp = static_cast<Node*>(cur);
          cur = tmp->next;
          T* val = tmp->_M_valptr();
          _M_put_node(tmp);
        }
    }

    iterator insert(const_iterator pos, const T& value) {
      Node* tmp = Node(value);
      tmp->_M_hook(pos._M_const_cast()._M_node);
      this->_M_inc_size(1);
      return iterator(tmp);
    }

    iterator insert(const_iterator pos, size_t count, const T& value) {
       if (count) {
          list tmp(count, value);
          iterator it = tmp.begin();
          splice(pos, tmp);
          return it;
        }
      return pos._M_const_cast();
    }

    iterator erase(const_iterator pos) {
      iterator ret = iterator(pos.node->next);
      _M_erase(pos._M_const_cast());
      return ret;
    }

    iterator erase(const_iterator first, const_iterator last) {
       iterator ret = iterator(first.node);
       for ( ; first < last ; ++first) {
         pos = iterator(first.node);
         ret = iterator(first.node->next);
         _M_erase(pos._M_const_cast());
       }
       return ret;
    }


    void push_back(const T& value) {
       // TODO
    }
    void pop_back() {
       // TODO
    }

    void push_front(const T& value) {
       // TODO
    }
    void pop_front() {
       // TODO
    }

    const_iterator resize_pos(size_t &count) const {
       const_iterator i;
      const size_t len = size();
      if (count < len) {
         if (count <= len / 2) {
              i = begin();
              std::advance(i, count);
         } else {
              i = end();
              ptrdiff_t num_erase = len - count;
              std::advance(i, -num_erase);
         }
         count = 0;
         return i;
      } else {
         i = end();
      }
      count -= len;
      return i;
    }

    void resize(size_t count) {
      const_iterator i = resize_pos(count);
      if (count) {
         insert(end(), count, nullpptr);
      } else {
         erase(i._M_const_cast(), end());
      }
    }

    void base_swap(list& x, list& y) {
       if ( x.next != &x ) {
         if ( y.next != &y ) {
            std::swap(x.next, y.next);
            std::swap(x.prev, y.prev);
            x.next->prev = x.prev->next = &x;
            y.next->prev = y.prev->next = &y;
         } else {
              y.next = x.next;
              y.prev = x.prev;
              y.next->prev = y.prev->next = &y;
              x.next = x.prev = &x;
            }
        } else if ( y.next != &y ) {
          x.next = y.next;
          x.prev = y.prev;
          x.next->prev = x.prev->next = &x;
          y.next = y.prev = &y;
        }
    }

    void swap(list& other) {
      base_swap(this->_M_impl._M_node, other._M_impl._M_node);
      size_t other_size = other._M_get_size();
      other._M_set_size(this->_M_get_size());
      this->_M_set_size(other_size);
    }

    void merge(list& other) {
       if (this != std::__addressof(other)) {
          _M_check_equal_allocators(other);
          iterator first1 = begin();
          iterator last1 = end();
          iterator first2 = other.begin();
          iterator last2 = other.end();
          const size_t orig_size = other.size();
          __try {
            while (first1 != last1 && first2 != last2)
              if (*first2 < *first1) {
                  iterator next = first2;
                  _M_transfer(first1, first2, ++next);
                  first2 = next;
                } else {
                   ++first1;
                }
            if (first2 != last2) {
               _M_transfer(last1, first2, last2);
            }
            this->_M_inc_size(other._M_get_size());
            other._M_set_size(0);
          }
          __catch(...) {
              const size_t dist = std::distance(first2, last2);
              this->_M_inc_size(orig_size - dist);
              other._M_set_size(dist);
              __throw_exception_again;
          }
       }
    }

    void splice(const_iterator pos, list& other) {
       if (!other.empty()) {
            _M_check_equal_allocators(other);
            this->_M_transfer(pos._M_const_cast(), other.begin(), other.end());
            this->_M_inc_size(other._M_get_size());
            other._M_set_size(0);
       }
    }

    void remove(const T& value) {
      iterator first = begin();
      iterator last = end();
      iterator extra = last;
      while (first != last) {
          iterator next = first;
          ++next;
          if (*first == value) {
              if (std::__addressof(*first) != std::__addressof(value))
                _M_erase(first);
              else
                extra = first;
            }
          first = next;
        }
      if (extra != last) {
         _M_erase(extra);
      }
    }

    void reverse() {
       this->node._M_reverse();
    }

    void unique() {
      iterator first = begin();
      iterator last = end();
      if (first == last)
        return;
      iterator next = first;
      while (++next != last) {
          if (*first == *next) {
            _M_erase(next);
          } else {
            first = next;
          }
          next = first;
        }
    }

    void sort() {
      if (first != last
          && first->next != last) {
        list carry;
        list tmp[64];
        list * fill = tmp;
        list * counter = NULL;
        try {
            do {
                carry.splice(carry.begin(), *this, begin());
                for (counter = tmp; counter != fill && !counter->empty(); ++counter) {
                    counter->merge(carry);
                    carry.swap(*counter);
                  }
                carry.swap(*counter);
                if (counter == fill) {
                  ++fill;
                }
            } while ( !empty() );

            for (counter = tmp + 1; counter != fill; ++counter)
              counter->merge(*(counter - 1));
            swap(*(fill - 1) );
          }
        __catch(...) {
            this->splice(this->end(), carry);
            for (int i = 0; i < sizeof(tmp)/sizeof(tmp[0]); ++i) {
               this->splice(this->end(), tmp[i]);
            }
            __throw_exception_again;
          }
      }
    }
};

}  // namespace task