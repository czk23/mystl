#ifndef MYSTL_DEQUE_H
#define MYSTL_DEQUE_H

#include "iterator.h"
#include "memory.h"
#include "type_traits.h"
#include "algorithm.h"
#include <cassert>

namespace mystl {

// 定义buffer的大小
template<class T>
struct deque_buf_size {
	static constexpr size_t value = sizeof(T) < 512 ? 512 / sizeof(T) : 1;
};

// deque迭代器
template<class T, class Ref, class Ptr>
struct deque_iterator : public iterator<random_iterator_tag, T> {
	// typedef
	typedef deque_iterator<T, T&, T*>							iterator;
	typedef deque_iterator<T, const T&, const T*>				const_iterator;

	typedef T													value_type;
	typedef T*													pointer;
	typedef T&													reference;
	typedef size_t												size_type;
	typedef ptrdiff_t											difference_type;

	typedef deque_iterator										self;
	typedef T**													map_pointer;

	static const size_type buf_size = deque_buf_size<T>::value;

	// 维护的四个指针
	T* cur;
	T* first;	// 最后一个元素的下一个位置
	T* last;
	map_pointer node;

	// 构造函数
	deque_iterator() : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {};

	deque_iterator(T* v, map_pointer n) : cur(v), first(*n), last(*n + buf_size), node(n) {};

	deque_iterator(const iterator& rhs) : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node) {};

	deque_iterator(const const_iterator& rhs) : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node) {};

	// 重新设置node的值(不会设置cur的值)
	void set_node(map_pointer new_node) {
		first = *new_node;
		last = *new_node + buf_size;
		node = new_node;
	}

	// 重载运算符
	self& operator=(const iterator& rhs) {
		if (rhs != *this) {
			cur = rhs.cur;
			first = rhs.first;
			last = rhs.last;
			node = rhs.node;
		}
		return *this;
	}

	reference operator*() const {
		return *cur;
	}

	pointer operator->() const {
		return cur;
	}

	// *this-rhs
	// 画图分析
	difference_type operator-(const self& rhs) const{
		difference_type n = 0;
		n = static_cast<difference_type>(buf_size) * (node - rhs.node - 1) + (cur - first) + (rhs.last - rhs.cur);
		return n;
	}

	self& operator++() {
		++cur;
		if (cur == last) {
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}

	self operator++(int) {
		self temp = *this;
		++*this;
		return temp;
	}

	self& operator--() {
		if (cur == first) {
			set_node(node - 1);
			cur = last;
		}
		cur--;
		return *this;
	}

	self operator--(int) {
		self temp = *this;
		--*this;
		return temp;
	}

	// 注意offset_node计算的细节和cur更新的细节
	self& operator+=(difference_type n) {
		difference_type offset = cur - first + n;
		if (offset >= 0 && offset < static_cast<difference_type>(buf_size))
			cur += n;
		else {
			difference_type offset_node = offset > 0 ? (offset / static_cast<difference_type>(buf_size)) 
				: -((-offset - 1) / static_cast<difference_type>(buf_size)) - 1;
			set_node(node + offset_node);
			cur = first + (offset - offset_node * static_cast<difference_type>(buf_size));
		}
		return *this;
	}

	self operator+(difference_type n) const {
		self temp = *this;
		temp += n;
		return temp;
	}

	self& operator-=(difference_type n) {
		return *this += -n;
	}

	self operator-(difference_type n) const {
		self temp = *this;
		temp -= n;
		return temp;
	}

	reference operator[](difference_type n) const {
		return *(*this + n);
	}

	bool operator==(const self& rhs) const {
		return cur == rhs.cur;
	}

	bool operator!=(const self& rhs) const {
		return cur != rhs.cur;
	}

	bool operator<(const self& rhs) const {
		return node == rhs.node ? cur < rhs.cur : node < rhs.node;
	}

	bool operator>(const self& rhs) const {
		return rhs < *this;
	}

	bool operator<=(const self& rhs) const {
		return !(rhs < *this);
	}

	bool operator>=(const self& rhs) const {
		return !(*this < rhs);
	}

};

// deque
template<class T, class Alloc = alloc>
class deque {
public:
	// 配置器
	typedef simple_alloc<T, Alloc>						allocator_type;
	typedef simple_alloc<T, Alloc>						data_allocator;
	typedef simple_alloc<T*, Alloc>						map_allocator;

	// 基本属性
	typedef T											value_type;
	typedef value_type*									pointer;
	typedef value_type&									reference;
	typedef const value_type*							const_pointer;
	typedef const value_type&							const_reference;
	typedef size_t										size_type;
	typedef ptrdiff_t									difference_type;
	typedef pointer*									map_pointer;
	typedef const pointer*								const_map_pointer;

	// 迭代器
	typedef deque_iterator<T, T&, T*>					iterator;
	typedef deque_iterator<T, const T&, const T*>		const_iterator;
	typedef reverse_iterator<const_iterator>			const_reverse_iterator;
	typedef reverse_iterator<iterator>					reverse_iterator;

	allocator_type get_allocator() {
		return data_allocator();
	}

protected:
	// 缓冲区的大小
	static const size_type buffer_size = deque_buf_size<T>::value;
	// 初始化结点的默认个数
	const size_type initial_map_size = 8;

private:
	// 维护四个变量代表一个deque
	map_pointer map;
	size_type map_size;
	iterator start;
	iterator finish;

public:
//------------------------------------------------构造、复制和析构函数------------------------------------------------
	deque();

	deque(size_type n);

	deque(size_type n, const value_type& value);

	deque(const deque& rhs);

	deque(const std::initializer_list<T>& ilist);

	template<class InputIterator>
	deque(InputIterator first, InputIterator last);

	deque(value_type* first, value_type* last);

	deque(const_iterator first, const_iterator last);

	~deque();

	deque& operator=(const deque& rhs);

	deque& operator=(const std::initializer_list<T>& ilist);


//------------------------------------------------迭代器相关函数------------------------------------------------
	iterator begin();

	const_iterator begin() const;

	iterator end();
	
	const_iterator end() const;

	reverse_iterator rbegin();

	const_reverse_iterator rbegin() const;

	reverse_iterator rend();

	const_reverse_iterator rend() const;

	const_iterator cbegin() const;

	const_iterator cend() const;

	const_reverse_iterator crbegin() const;

	const_reverse_iterator crend() const;


//------------------------------------------------容量相关函数------------------------------------------------
	bool empty() const;

	size_type size() const;

	size_type max_size() const;

	void resize(size_type new_size, const value_type& value);

	void resize(size_type new_size);

	void swap(deque& rhs);


//------------------------------------------------------访问元素相关函数-------------------------------------------------------
	reference operator[](difference_type n);

	const_reference operator[](difference_type n) const;

	reference at(difference_type n);

	const_reference at(difference_type n) const;

	reference front();

	const_reference front() const;

	reference back();

	const_reference back() const;


//------------------------------------------------------修改容器相关函数-------------------------------------------------------
	void assign(size_type n, const value_type& value);

	template<class InputIterator>
	void assign(InputIterator first, InputIterator last);

	void assign(std::initializer_list<T>& ilist);

	void push_back(const value_type& value);

	void push_back();

	void push_front(const value_type& value);

	void push_front();

	void pop_back();

	void pop_front();

	iterator insert(iterator position, const value_type& value);

	iterator insert(iterator position);

	void insert(iterator position, size_type n, const value_type& value);

	template<class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last);

	void insert(iterator position, const value_type* first, const value_type* last);

	void insert(iterator position, const_iterator first, const_iterator last);

	iterator erase(iterator position);

	iterator erase(iterator first, iterator last);

	void clear();


//------------------------------------------------------辅助函数-------------------------------------------------------
protected:
// 初始化一个map(配置空间)
	void initialize_map(size_type num_elements);

// 初始化结点(配置空间)
	void create_nodes(map_pointer first, map_pointer last);

// 清除结点
	void destroy_nodes(map_pointer first, map_pointer last);

// 对元素进行赋值
	void fill_initialize(const value_type& value);

// 辅助模板构造函数
	template<class Integer>
	void initialize_dispatch(Integer n, Integer value, true_type);

// 辅助模板构造函数
	template<class InputIterator>
	void initialize_dispatch(InputIterator first, InputIterator last, false_type);

// input iterator版本的辅助构造函数
	template<class InputIterator>
	void range_initialize(InputIterator first, InputIterator last, input_iterator_tag);

// forward iterator版本的辅助构造函数
	template<class ForwardIterator>
	void range_initialize(ForwardIterator first, ForwardIterator last, forward_iterator_tag);

// 重新填充deque
	void fill_assign(size_type n, const value_type& value);

// 模板函数assign的辅助函数
	template<class Integer>
	void assign_dispatch(Integer n, Integer value, true_type);

// 模板函数assign的辅助函数
	template<class InputIterator>
	void assign_dispatch(InputIterator first, InputIterator last, false_type);

// 模板函数assign的实现(input iterator版本)
	template<class InputIterator>
	void assign_aux(InputIterator first, InputIterator last, input_iterator_tag);

// 模板函数assign的实现(forward iterator版本)
	template<class ForwardIterator>
	void assign_aux(ForwardIterator first, ForwardIterator last, forward_iterator_tag);

// push_back的辅助函数
	void push_back_aux(const value_type& value);

// push_back的辅助函数
	void push_back_aux();

// push_front的辅助函数
	void push_front_aux(const value_type& value);

// push_front的辅助函数
	void push_front_aux();

// 在尾端插入元素时的辅助函数
	void reserve_map_at_back(size_type nodes_to_add = 1);

// 在头部插入元素时的辅助函数
	void reserve_map_at_front(size_type nodes_to_add = 1);

// 重构整个map
	void reallocate_map(size_type nodes_to_add, bool add_at_front);

// pop_back的辅助函数
	void pop_back_aux();

// pop_front的辅助函数
	void pop_front_aux();

// 插入单个元素辅助函数
	iterator insert_aux(iterator position, const value_type& value);

// 插入多个相同元素的辅助函数
	void fill_insert(iterator position, size_type n, const value_type& value);

// 在头部插入多个元素的辅助函数
	iterator reserve_elements_at_front(size_type new_elements);

// 在尾端插入多个元素的辅助函数
	iterator reserve_elements_at_back(size_type new_elements);

// 在头部需要新增结点的辅助函数
	void new_elements_at_front(size_type new_elements);

// 在尾端需要新增结点的辅助函数
	void new_elements_at_back(size_type new_elements);

// 在中部插入多个元素的辅助函数
	void insert_aux(iterator position, size_type n, const value_type& value);

// insert模板函数的辅助函数
	template<class Integer>
	void insert_dispatch(iterator position, Integer n, Integer value, true_type);

// insert模板函数的辅助函数
	template<class InputIterator>
	void insert_dispatch(iterator position, InputIterator first, InputIterator last, false_type);

// insert模板函数辅助函数(input iterator版本)
	template<class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag);

// insert模板函数辅助函数(forward iterator版本)
	template<class ForwardIterator>
	void insert(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag);

// insert模板函数中部插入元素辅助函数(forward iterator版本)
	template<class ForwardIterator>
	void insert_aux(iterator position, ForwardIterator first, ForwardIterator last, size_type n);

// insert辅助函数(const value_type*版本)
	void insert_aux(iterator position, const value_type* first, const value_type* last, size_type n);

// insert辅助函数(const_iterator版本)
	void insert_aux(iterator position, const_iterator first, const_iterator last, size_type n);

};


//------------------------------------------------构造、复制和析构函数------------------------------------------------
template<class T, class Alloc>
deque<T, Alloc>::deque() {
	initialize_map(0);
}

template<class T, class Alloc>
deque<T, Alloc>::deque(size_type n) {
	initialize_map(n);
	fill_initialize(T());
}

template<class T, class Alloc>
deque<T, Alloc>::deque(size_type n, const value_type& value) {
	initialize_map(n);
	fill_initialize(value);
}

template<class T, class Alloc>
deque<T, Alloc>::deque(const deque& rhs) {
	initialize_map(rhs.size());
	mystl::uninitialized_copy(rhs.start, rhs.finish, start);
}

template<class T, class Alloc>
deque<T, Alloc>::deque(const std::initializer_list<T>& ilist) {
	size_type n = distance(ilist.begin(), ilist.end());
	initialize_map(n);
	mystl::uninitialized_copy(ilist.begin(), ilist.end(), start);
}

template<class T, class Alloc>
template<class InputIterator>
deque<T, Alloc>::deque(InputIterator first, InputIterator last) {
	typedef typename Is_integer<InputIterator>::value Integer;
	initialize_dispatch(first, last, Integer());
}

template<class T, class Alloc>
deque<T, Alloc>::deque(value_type* first, value_type* last) {
	difference_type n = last - first;
	initialize_map(n);
	mystl::uninitialized_copy(first, last, start);
}

template<class T, class Alloc>
deque<T, Alloc>::deque(const_iterator first, const_iterator last) {
	difference_type n = distance(first, last);
	initialize_map(n);
	mystl::uninitialized_copy(first, last, start);
}

template<class T, class Alloc>
deque<T, Alloc>::~deque() {
	mystl::destroy(start, finish);
	if (map) {
		destroy_nodes(start.node, finish.node + 1);
		map_allocator::deallocate(map, map_size);
	}
}

template<class T, class Alloc>
deque<T, Alloc>& deque<T, Alloc>::operator=(const deque& rhs) {
	if (&rhs != this) {
		const auto len = size();
		if (size() >= rhs.size()) {
			erase(mystl::copy(rhs.begin(), rhs.end(), start), finish);
		}
		else {
			const_iterator mid = rhs.begin() + static_cast<difference_type>(len);
			mystl::copy(rhs.begin(), mid, start);
			insert(finish, mid, rhs.end());
		}
	}
	return *this;
}

template<class T, class Alloc>
deque<T, Alloc>& deque<T, Alloc>::operator=(const std::initializer_list<T>& ilist) {
	deque temp(ilist);
	swap(temp);
	return *this;
}


//------------------------------------------------迭代器相关函数------------------------------------------------
template<class T, class Alloc>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::begin() {
	return start;
}

template<class T, class Alloc>
typename deque<T, Alloc>::const_iterator
deque<T, Alloc>::begin() const {
	return start;
}

template<class T, class Alloc>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::end() {
	return finish;
}

template<class T, class Alloc>
typename deque<T, Alloc>::const_iterator
deque<T, Alloc>::end() const {
	return finish;
}

template<class T, class Alloc>
typename deque<T, Alloc>::reverse_iterator
deque<T, Alloc>::rbegin() {
	return reverse_iterator(end());
}

template<class T, class Alloc>
typename deque<T, Alloc>::const_reverse_iterator
deque<T, Alloc>::rbegin() const {
	return reverse_iterator(end());
}

template<class T, class Alloc>
typename deque<T, Alloc>::reverse_iterator
deque<T, Alloc>::rend() {
	return reverse_iterator(begin());
}

template<class T, class Alloc>
typename deque<T, Alloc>::const_reverse_iterator
deque<T, Alloc>::rend() const {
	return reverse_iterator(begin());
}

template<class T, class Alloc>
typename deque<T, Alloc>::const_iterator
deque<T, Alloc>::cbegin() const {
	return begin();
}

template<class T, class Alloc>
typename deque<T, Alloc>::const_iterator
deque<T, Alloc>::cend() const {
	return end();
}

template<class T, class Alloc>
typename deque<T, Alloc>::const_reverse_iterator
deque<T, Alloc>::crbegin() const {
	return rbegin();
}

template<class T, class Alloc>
typename deque<T, Alloc>::const_reverse_iterator
deque<T, Alloc>::crend() const {
	return rend();
}


//------------------------------------------------容量相关函数------------------------------------------------
template<class T, class Alloc>
bool deque<T, Alloc>::empty() const {
	return start == finish;
}

template<class T, class Alloc>
typename deque<T, Alloc>::size_type
deque<T, Alloc>::size() const {
	return finish - start;
}

template<class T, class Alloc>
typename deque<T, Alloc>::size_type
deque<T, Alloc>::max_size() const {
	return static_cast<size_type>(-1);
}

template<class T, class Alloc>
void deque<T, Alloc>::resize(size_type new_size, const value_type& value) {
	if (new_size < size())
		erase(start + static_cast<difference_type>(new_size), finish);
	else {
		insert(finish, new_size - size(), value);
	}
}

template<class T, class Alloc>
void deque<T, Alloc>::resize(size_type new_size) {
	resize(new_size, T());
}

template<class T, class Alloc>
void deque<T, Alloc>::swap(deque<T, Alloc>& rhs) {
	mystl::swap(start, rhs.start);
	mystl::swap(finish, rhs.finish);
	mystl::swap(map, rhs.map);
	mystl::swap(map_size, rhs.map_size);
}


//------------------------------------------------------访问元素相关函数-------------------------------------------------------
template<class T, class Alloc>
typename deque<T, Alloc>::reference
deque<T, Alloc>::operator[](difference_type n) {
	assert(n < size());
	return start[n];
}

template<class T, class Alloc>
typename deque<T, Alloc>::const_reference
deque<T, Alloc>::operator[](difference_type n) const {
	assert(n < size());
	return start[n];
}

template<class T, class Alloc>
typename deque<T, Alloc>::reference
deque<T, Alloc>::at(difference_type n) {
	assert(n < size());
	return (*this)[n];
}

template<class T, class Alloc>
typename deque<T, Alloc>::const_reference
deque<T, Alloc>::at(difference_type n) const {
	assert(n < size());
	return (*this)[n];
}

template<class T, class Alloc>
typename deque<T, Alloc>::reference
deque<T, Alloc>::front() {
	return *start;
}

template<class T, class Alloc>
typename deque<T, Alloc>::const_reference
deque<T, Alloc>::front() const {
	return *start;
}

template<class T, class Alloc>
typename deque<T, Alloc>::reference
deque<T, Alloc>::back() {
	iterator temp = finish;
	--temp;
	return *temp;
}

template<class T, class Alloc>
typename deque<T, Alloc>::const_reference
deque<T, Alloc>::back() const {
	iterator temp = finish;
	--temp;
	return *temp;
}


//------------------------------------------------------修改容器相关函数-------------------------------------------------------
template<class T, class Alloc>
void deque<T, Alloc>::assign(size_type n, const value_type& value) {
	fill_assign(n, value);
}

template<class T, class Alloc>
template<class InputIterator>
void deque<T, Alloc>::assign(InputIterator first, InputIterator last) {
	typedef typename Is_integer<InputIterator>::value Integer;
	assign_dispatch(first, last, Integer());
}

template<class T, class Alloc>
void deque<T, Alloc>::assign(std::initializer_list<T>& ilist) {
	assign_aux(ilist.begin(), ilist.end(), forward_iterator_tag);
}

template<class T, class Alloc>
void deque<T, Alloc>::push_back(const value_type& value) {
	if (finish.cur != finish.last - 1) {
		mystl::construct(finish.cur, value);
		++finish.cur;
	}
	else
		push_back_aux(value);
}

template<class T, class Alloc>
void deque<T, Alloc>::push_back() {
	if (finish.cur != finish.last - 1) {
		mystl::construct(finish.cur);
		++finish.cur;
	}
	else
		push_back_aux();
}

template<class T, class Alloc>
void deque<T, Alloc>::push_front(const value_type& value) {
	if (start.cur != start.first) {
		--start.cur;
		mystl::construct(start.cur, value);
	}
	else
		push_front_aux(value);
}

template<class T, class Alloc>
void deque<T, Alloc>::push_front() {
	if (start.cur != start.first) {
		--start.cur;
		mystl::construct(start.cur);
	}
	else
		push_front_aux();
}

template<class T, class Alloc>
void deque<T, Alloc>::pop_back() {
	if (finish.cur != finish.first) {
		--finish.cur;
		mystl::destroy(finish.cur);
	}
	else
		pop_back_aux();
}

template<class T, class Alloc>
void deque<T, Alloc>::pop_front() {
	if (start.cur != start.last - 1) {
		mystl::destroy(start.cur);
		++start.cur;
	}
	else
		pop_front_aux();
}

template<class T, class Alloc>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::insert(iterator position, const value_type& value) {
	if (position.cur == start.cur) {
		push_front(value);
		return start;
	}
	else if (position.cur == finish.cur) {
		push_back(value);
		iterator temp = finish;
		--temp;
		return temp;
	}
	else
		return insert_aux(position, value);
}

template<class T, class Alloc>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::insert(iterator position) {
	return insert(position, T());
}

template<class T, class Alloc>
void deque<T, Alloc>::insert(iterator position, size_type n, const value_type& value) {
	fill_insert(position, n, value);
}

template<class T, class Alloc>
template<class InputIterator>
void deque<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
	typedef typename Is_integer<InputIterator>::value Integer;
	insert_dispatch(position, first, last, Integer());
}

template<class T, class Alloc>
void deque<T, Alloc>::insert(iterator position, const value_type* first, const value_type* last) {
	difference_type n = distance(first, last);
	if (position.cur == start.cur) {
		iterator new_start = reserve_elements_at_front(n);
		try {
			mystl::uninitialized_copy(first, last, new_start);
			start = new_start;
		}
		catch (...) {
			destroy_nodes(new_start.node, start.node);
		}
	}
	else if (position.cur == finish.cur) {
		iterator new_finish = reserve_elements_at_back(n);
		try {
			mystl::uninitialized_copy(first, last, finish);
			finish = new_finish;
		}
		catch (...) {
			destroy_nodes(finish.node + 1, new_finish.node + 1);
		}
	}
	else
		insert_aux(position, first, last, n);
}

template<class T, class Alloc>
void deque<T, Alloc>::insert(iterator position, const_iterator first, const_iterator last) {
	difference_type n = distance(first, last);
	if (position.cur == start.cur) {
		iterator new_start = reserve_elements_at_front(n);
		try {
			mystl::uninitialized_copy(first, last, new_start);
			start = new_start;
		}
		catch (...) {
			destroy_nodes(new_start.node, start.node);
		}
	}
	else if (position.cur == finish.cur) {
		iterator new_finish = reserve_elements_at_back(n);
		try {
			mystl::uninitialized_copy(first, last, finish);
			finish = new_finish;
		}
		catch (...) {
			destroy_nodes(finish.node + 1, new_finish.node + 1);
		}
	}
	else
		insert_aux(position, first, last, n);
}

template<class T, class Alloc>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::erase(iterator position) {
	iterator next = position;
	++next;
	size_type index = position - start;
	if (index < size() / 2) {
		mystl::copy_backward(start, position, next);
		pop_front();
	}
	else {
		mystl::copy(next, finish, position);
		pop_back();
	}
	return start + index;
}

template<class T, class Alloc>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::erase(iterator first, iterator last) {
	if (first.cur == start.cur && last.cur == finish.cur) {
		clear();
		return finish;
	}
	size_type n = last - first;
	size_type elems_before = first - start;
	if (elems_before < (size() - n) / 2) {
		mystl::copy_backward(start, first, last);
		iterator new_start = start + n;
		mystl::destroy(start, new_start);
		destroy_nodes(start.node, new_start.node);
		start = new_start;
	}
	else {
		mystl::copy(last, finish, first);
		iterator new_finish = finish - n;
		mystl::destroy(new_finish, finish);
		destroy_nodes(new_finish.node + 1, finish.node + 1);
		finish = new_finish;
	}
	return start + elems_before;
}

template<class T, class Alloc>
void deque<T, Alloc>::clear() {
	for (map_pointer node = start.node + 1; node < finish.node; ++node) {
		mystl::destroy(*node, *node + buffer_size);
		data_allocator::deallocate(*node, buffer_size);
	}
	if (start.node != finish.node) {
		mystl::destroy(start.cur, start.last);
		mystl::destroy(finish.first, finish.cur);
		data_allocator::deallocate(finish.first, buffer_size);
	}
	else
		mystl::destroy(start.cur, finish.cur);
	finish = start;
}


//------------------------------------------------------辅助函数-------------------------------------------------------
// 初始化deque(只是分配空间)
// 调用函数都是构造函数
template<class T, class Alloc>
void deque<T, Alloc>::initialize_map(size_type num_elements) {
	// 会额外多分配一个node
	size_type num_nodes = num_elements / buffer_size + 1;
	// 前后各预留一个结点扩充时使用
	map_size = mystl::max(num_nodes + 2, initial_map_size);
	map = map_allocator::allocate(map_size);
	map_pointer nstart = map + (map_size - num_nodes) / 2;
	map_pointer nfinish = nstart + num_nodes;
	try {
		create_nodes(nstart, nfinish);
	}
	catch (...) {
		map_allocator::deallocate(map, map_size);
		map = nullptr;
		map_size = 0;
	}
	start.set_node(nstart);
	finish.set_node(nfinish - 1);
	start.cur = start.first;
	finish.cur = finish.first + num_elements % buffer_size;
}

// 初始化结点(只是分配空间)
// 调用函数有：
// 1. initialize_map(num_elements) -> create_nodes
template<class T, class Alloc>
void deque<T, Alloc>::create_nodes(map_pointer first, map_pointer last) {
	map_pointer cur = first;
	try {
		for (; cur < last; ++cur)
			*cur = data_allocator::allocate(buffer_size);
	}
	catch (...) {
		destroy_nodes(first, cur);
	}
}

// 清除结点
// 调用函数有：
// 1. initialize_map(num_elements) -> create_nodes(first, last) -> destroy_nodes
template<class T, class Alloc>
void deque<T, Alloc>::destroy_nodes(map_pointer first, map_pointer last) {
	for (map_pointer cur = first; cur != last; ++cur)
		data_allocator::deallocate(*cur, buffer_size);
}

// 对元素进行赋值
// 调用函数有：
// 1. 构造函数
template<class T, class Alloc>
void deque<T, Alloc>::fill_initialize(const value_type& value) {
	map_pointer cur = start.node;
	try {
		for (; cur < finish.node; ++cur) {
			mystl::uninitialized_fill(*cur, *cur + buffer_size, value);
		}
		mystl::uninitialized_fill(finish.first, finish.cur, value);
	}
	catch (...) {
		mystl::destroy(start, iterator(*cur, cur));
	}
}

// 辅助模板构造函数，当模板参数为整数时调用此函数
// 调用函数有：
// 1. deque(n, value) -> initialize_dispatch
template<class T, class Alloc>
template<class Integer>
void deque<T, Alloc>::initialize_dispatch(Integer n, Integer value, true_type) {
	initialize_map(n);
	fill_initialize(value);
}

// input iterator版本的辅助构造函数
// 调用函数有：
// 1. deque(first, last) -> initialize_dispatch(first, last, false_type) -> range_initialize
template<class T, class Alloc>
template<class InputIterator>
void deque<T, Alloc>::range_initialize(InputIterator first, InputIterator last, input_iterator_tag) {
	initialize_map(0);
	try {
		for (; first != last; ++first)
			push_back(*first);
	}
	catch (...) {
		clear();
	}
}

// forward iterator版本的辅助构造函数
// 调用函数有：
// 1. deque(first, last) -> initialize_dispatch(first, last, false_type) -> range_initialize
template<class T, class Alloc>
template<class ForwardIterator>
void deque<T, Alloc>::range_initialize(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
	size_type n = distance(first, last);
	initialize_map(n);
	map_pointer cur = start.node;
	try {
		iterator mid = start;
		for (; cur < finish.node; ++cur) {
			advance(mid, buffer_size);
			mystl::uninitialized_copy(first, mid, *cur);
			first = mid;
		}
		mystl::uninitialized_copy(first, last, finish.first);
	}
	catch (...) {
		destroy(start, iterator(*cur, cur));
	}
}

// 辅助模板构造函数，当模板参数不是整数时调用此函数
// 调用函数有：
// 1. deque(first, last) -> initialize_dispatch 
template<class T, class Alloc>
template<class InputIterator>
void deque<T, Alloc>::initialize_dispatch(InputIterator first, InputIterator last, false_type) {
	range_initialize(first, last, iterator_category(first));
}

// 重新填充deque
// 调用函数有：
// 1. assign(n, value) -> fill_assign
// 2. assign(first, last) -> assign_dispatch(n, value, true_type) -> fill_assign
template<class T, class Alloc>
void deque<T, Alloc>::fill_assign(size_type n, const value_type& value) {
	if (n > size()) {
		mystl::fill(begin(), end(), value);
		insert(end(), n - size(), value);
		std::cout << "After insert: ";
		for (auto it = begin(); it != end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
	else {
		mystl::fill(begin(), begin() + n, value);
		erase(begin() + n, end());
	}
}

// 当模板参数是整数时，模板函数assign调用的函数
// 调用函数有：
// 1. assign(first, last) -> assign_dispatch
template<class T, class Alloc>
template<class Integer>
void deque<T, Alloc>::assign_dispatch(Integer n, Integer value, true_type) {
	fill_assign(n, value);
}

// 当模板参数不是整数时，模板函数assign调用的函数
// 调用函数有：
// 1. assign(first, last) -> assign_dispatch
template<class T, class Alloc>
template<class InputIterator>
void deque<T, Alloc>::assign_dispatch(InputIterator first, InputIterator last, false_type) {
	assign_aux(first, last, iterator_category(first));
}

// 当模板参数不是整数时调用的函数(input iterator版本)
// 调用函数有：
// 1. assign(first, last) -> assign_dispatch(first, last, false_type) -> assign_aux
template<class T, class Alloc>
template<class InputIterator>
void deque<T, Alloc>::assign_aux(InputIterator first, InputIterator last, input_iterator_tag) {
	iterator cur = begin();
	for (; first != last && cur != end(); ++first, ++cur)
		*cur = *first;
	if (first == last) {
		erase(cur, end());
	}
	else {
		insert(end(), first, last);
	}
}

// 当模板参数不是整数时调用的函数(forward iterator版本)
// 调用函数有：
// 1. assign(first, last) -> assign_dispatch(first, last, false_type) -> assign_aux
template<class T, class Alloc>
template<class ForwardIterator>
void deque<T, Alloc>::assign_aux(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
	size_type n = distance(first, last);
	if (n > size()) {
		ForwardIterator mid = first;
		advance(mid, size());
		mystl::copy(first, mid, begin());
		insert(end(), mid, last);
	}
	else {
		erase(mystl::copy(first, last, begin()), end());
	}
}

// push_back的辅助函数(只有当finish.cur == finish.last - 1时调用)
// 调用函数有：
// 1. push_back(value) -> push_back_aux
template<class T, class Alloc>
void deque<T, Alloc>::push_back_aux(const value_type& value) {
	reserve_map_at_back();
	*(finish.node + 1) = data_allocator::allocate(buffer_size);
	try {
		mystl::construct(finish.cur, value);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}
	catch (...) {
		mystl::destroy(*(finish.node + 1));
	}
}

// push_back的辅助函数(只有当finish.cur == finish.last - 1时调用)
// 调用函数有：
// 1. push_back() -> push_back_aux
template<class T, class Alloc>
void deque<T, Alloc>::push_back_aux() {
	reserve_map_at_back();
	*(finish.node + 1) = data_allocator::allocate(buffer_size);
	try {
		mystl::construct(finish.cur);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}
	catch (...) {
		mystl::destroy(*(finish.node + 1));
	}
}

// push_front的辅助函数(只有当start.cur == start.first时调用)
// 调用函数有：
// 1. push_front(value) -> push_front_aux
template<class T, class Alloc>
void deque<T, Alloc>::push_front_aux(const value_type& value) {
	reserve_map_at_front();
	*(start.node - 1) = data_allocator::allocate(buffer_size);
	try {
		start.set_node(start.node - 1);
		start.cur = start.last - 1;
		mystl::construct(start.cur, value);
	}
	catch (...) {
		++start;
		mystl::destroy(*(start.node - 1));
	}
}

// push_front的辅助函数(只有当start.cur == start.first时调用)
// 调用函数有：
// 1. push_front() -> push_front_aux
template<class T, class Alloc>
void deque<T, Alloc>::push_front_aux() {
	reserve_map_at_front();
	*(start.node - 1) = data_allocator::allocate(buffer_size);
	try {
		start.set_node(start.node - 1);
		start.cur = start.last - 1;
		mystl::construct(start.cur);
	}
	catch (...) {
		++start;
		mystl::destroy(*(start.node - 1));
	}
}

// 在尾端插入元素时调整map
// 调用函数有：
// 1. push_back(value) -> push_back_aux(value) -> reserve_map_at_back
// 2. push_back() -> push_back_aux() -> reserve_map_at_back
// 3. insert(n, value) -> fill_insert(n, value) -> reverse_elements_at_back(new_elements) -> new_elements_at_back(new_elements) -> reserve_map_at_back
template<class T, class Alloc>
void deque<T, Alloc>::reserve_map_at_back(size_type nodes_to_add) {
	if (nodes_to_add + 1 > map_size - (finish.node - map))
		reallocate_map(nodes_to_add, false);
}

// 在头部插入元素时调整map
// 调用函数有：
// 1. push_front(value) -> push_front_aux(value) -> reserve_map_at_front
// 2. push_front() -> push_front_aux() -> reserve_map_at_front
// 3. insert(n, value) -> fill_insert(n, value) -> reverse_elements_at_front(new_elements) -> new_elements_at_front(new_elements) -> reserve_map_at_front
template<class T, class Alloc>
void deque<T, Alloc>::reserve_map_at_front(size_type nodes_to_add) {
	if (nodes_to_add > static_cast<size_type>(start.node - map))
		reallocate_map(nodes_to_add, true);
}

// 重构整个map
// 调用函数有：
// 1. push_back(value) -> push_back_aux(value) -> reserve_map_at_back(nodes_to_add, false) -> reallocate_map
// 2. push_back() -> push_back_aux() -> reserve_map_at_back(nodes_to_add, false) -> reallocate_map
// 3. push_front(value) -> push_front_aux(value) -> reserve_map_at_front(nodes_to_add, true) -> reallocate_map
// 4. push_front() -> push_front_aux() -> reserve_map_at_front(nodes_to_add, true) -> reallocate_map
// 5. insert(n, value) -> fill_insert(n, value) -> reverse_elements_at_back(new_elements) -> new_elements_at_back(new_elements) -> reserve_map_at_back(nodes_to_add) -> reallocate_map
// 3. insert(n, value) -> fill_insert(n, value) -> reverse_elements_at_front(new_elements) -> new_elements_at_front(new_elements) -> reserve_map_at_front(nodes_to_add) -> reallocate_map
template<class T, class Alloc>
void deque<T, Alloc>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
	size_type old_num_nodes = finish.node - start.node + 1;
	size_type new_num_nodes = old_num_nodes + nodes_to_add;
	map_pointer new_start;
	if (map_size > 2 * new_num_nodes) {
		new_start = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
		if (new_start < start.node) {
			mystl::copy(start.node, finish.node + 1, new_start);
		}
		else {
			mystl::copy_backward(start.node, finish.node + 1, new_start + old_num_nodes);
		}
	}
	else {
		size_type new_map_size = map_size + mystl::max(map_size, new_num_nodes ) + 2;
		map_pointer new_map = map_allocator::allocate(new_map_size);
		new_start = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
		mystl::copy(start.node, finish.node + 1, new_start);
		map_allocator::deallocate(map, map_size);
		map = new_map;
		map_size = new_map_size;
	}
	start.set_node(new_start);
	finish.set_node(new_start + old_num_nodes - 1);
}

// pop_back的辅助函数(只有在finish.cur == finish.first时调用)
// 调用函数有：
// 1. pop_back() -> pop_back_aux()
template<class T, class Alloc>
void deque<T, Alloc>::pop_back_aux() {
	data_allocator::deallocate(finish.first, buffer_size);
	finish.set_node(finish.node - 1);
	finish.cur = finish.last - 1;
	mystl::destroy(finish.cur);
}

// pop_front的辅助函数(只有在start.cur == start.first时调用)
// 调用函数有：
// 1. pop_front() -> pop_front_aux()
template<class T, class Alloc>
void deque<T, Alloc>::pop_front_aux() {
	mystl::destroy(start.cur);
	data_allocator::deallocate(start.cur, buffer_size);
	start.set_node(start.node + 1);
	start.cur = start.first;
}

// 插入一个元素的辅助函数
// 调用函数有：
// 1. insert(position, value) -> insert_aux
template<class T, class Alloc>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::insert_aux(iterator position, const value_type& value) {
	size_type index = position - start;
	if (index < size() / 2) {
		push_front(front());
		position = start + index;
		iterator front1 = start;
		++front1;
		iterator front2 = front1;
		++front2;
		iterator position1 = position;
		++position1;
		mystl::copy(front2, position1, front1);
	}
	else {
		push_back(back());
		position = start + index;
		iterator back1 = finish;
		--back1;
		iterator back2 = back1;
		--back2;
		mystl::copy_backward(position, back1, back2);
	}
	*position = value;
	return position;
}

// 插入多个相同元素的辅助函数
// 调用函数有：
// 1. insert(position, n, value) -> fill_insert
template<class T, class Alloc>
void deque<T, Alloc>::fill_insert(iterator position, size_type n, const value_type& value) {
	if (position.cur == start.cur) {
		iterator new_start = reserve_elements_at_front(n);
		try {
			mystl::uninitialized_fill(new_start, start, value);
			start = new_start;
		}
		catch (...) {
			destroy_nodes(new_start.node, start.node);
		}
	}
	else if (position.cur == finish.cur) {
		iterator new_finish = reserve_elements_at_back(n);
		try {
			mystl::uninitialized_fill(finish, new_finish, value);
			finish = new_finish;
		}
		catch (...) {
			destroy_nodes(finish.node + 1, new_finish.node + 1);
		}
	}
	else
		insert_aux(position, n, value);
}

// 在头部插入多个元素的辅助函数
// 调用函数有：
// 1. insert(n, value) -> fill_insert(n, value) -> reverse_elements_at_front
template<class T, class Alloc>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::reserve_elements_at_front(size_type new_elements) {
	size_type vacancies = static_cast<size_type>(start.cur - start.first);
	if (vacancies < new_elements)
		new_elements_at_front(new_elements);
	return start - static_cast<difference_type>(new_elements);
}

// 在尾端插入多个元素的辅助函数
// 调用函数有：
// 1. insert(n, value) -> fill_insert(n, value) -> reverse_elements_at_back
template<class T, class Alloc>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::reserve_elements_at_back(size_type new_elements) {
	size_type vacancies = static_cast<size_type>(finish.last - finish.cur - 1);
	if (vacancies < new_elements)
		new_elements_at_back(new_elements);
	return finish + static_cast<difference_type>(new_elements);
}

// 在头部新增结点的辅助函数
// 调用函数有：
// 1. insert(n, value) -> fill_insert(n, value) -> reverse_elements_at_front(new_elements) -> new_elements_at_front
template<class T, class Alloc>
void deque<T, Alloc>::new_elements_at_front(size_type new_elements) {
	size_type new_nodes = (new_elements - 1) / buffer_size + 1;
	reserve_map_at_front(new_nodes);
	size_type i = 1;
	try {
		for (; i <= new_nodes; ++i)
			*(start.node - i) = data_allocator::allocate(buffer_size);
	}
	catch (...) {
		for (size_type j = 1; j < i; ++j)
			data_allocator::deallocate(*(start.node - j), buffer_size);
	}
}

// 在尾端新增结点的辅助函数
// 调用函数有：
// 1. insert(n, value) -> fill_insert(n, value) -> reverse_elements_at_back(new_elements) -> new_elements_at_back
template<class T, class Alloc>
void deque<T, Alloc>::new_elements_at_back(size_type new_elements) {
	size_type new_nodes = (new_elements - 1) / buffer_size + 1;
	reserve_map_at_back(new_nodes);
	size_type i = 1;
	try {
		for (; i <= new_nodes; ++i)
			*(finish.node + i) = data_allocator::allocate(buffer_size);
	}
	catch (...) {
		for (size_type j = i; j < i; ++j)
			data_allocator::deallocate(*(finish.node + j), buffer_size);
	}
}

// 在中部插入多个元素的辅助函数
// 调用函数有：
// 1. insert(n, value) -> fill_insert(n, value) -> insert_aux
template<class T, class Alloc>
void deque<T, Alloc>::insert_aux(iterator position, size_type n, const value_type& value) {
	size_type elems_before = position - start;
	if (elems_before < static_cast<difference_type>(size() / 2)) {
		iterator new_start = reserve_elements_at_front(n);
		iterator old_start = start;
		position = start + elems_before;
		try {
			if (elems_before >= static_cast<difference_type>(n)) {
				iterator mid = start + static_cast<difference_type>(n);
				mystl::uninitialized_copy(start, mid, new_start);
				start = new_start;
				mystl::copy(mid, position, old_start);
				mystl::fill(position - static_cast<difference_type>(n), position, value);
			}
			else {
				mystl::uninitialized_copy(start, position, new_start);
				mystl::uninitialized_fill(new_start + elems_before, start, value);
				start = new_start;
				mystl::fill(old_start, position, value);
			}
		}
		catch (...) {
			destroy_nodes(new_start.node, start.node);
		}
	}
	else {
		iterator new_finish = reserve_elements_at_back(n);
		iterator old_finish = finish;
		size_type elems_after = static_cast<difference_type>(size()) - elems_before;
		position = finish - elems_after;
		try {
			if (elems_after > static_cast<difference_type>(n)) {
				iterator mid = finish - static_cast<difference_type>(n);
				mystl::uninitialized_copy(mid, finish, finish);
				finish = new_finish;
				mystl::copy_backward(position, mid, old_finish);
				mystl::fill(position, position + static_cast<difference_type>(n), value);
			}
			else {
				mystl::uninitialized_fill(finish, position + static_cast<difference_type>(n), value);
				mystl::uninitialized_copy(position, finish, position + static_cast<difference_type>(n));
				finish = new_finish;
				mystl::fill(position, old_finish, value);
			}
		}
		catch (...) {
			destroy_nodes((finish.node + 1), (new_finish.node + 1));
		}
	}
}

// 当insert模板函数的模板参数是整数时调用
// 调用函数有：
// 1. insert(position, first, last) -> insert_dispatch
template<class T, class Alloc>
template<class Integer>
void deque<T, Alloc>::insert_dispatch(iterator position, Integer n, Integer value, true_type) {
	fill_insert(position, n, value);
}

// 当insert模板函数的模板参数不是整数时调用
// 调用函数有：
// 1. insert(position, first, last) -> insert_dispatch
template<class T, class Alloc>
template<class InputIterator>
void deque<T, Alloc>::insert_dispatch(iterator position, InputIterator first, InputIterator last, false_type) {
	insert(position, first, last, iterator_category(first));
}

// 插入多个不同元素辅助函数(input iterator版本)
// 调用函数有：
// 1. insert(position, first, last) -> insert_dispatch(position, first, last, false_type) -> insert
template<class T, class Alloc>
template<class InputIterator>
void deque<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag) {
	mystl::copy(first, last, inserter(*this, position));
}

// 插入多个不同元素辅助函数(forward iterator版本)
// 调用函数有：
// 1. insert(position, first, last) -> insert_dispatch(position, first, last, false_type) -> insert
template<class T, class Alloc>
template<class ForwardIterator>
void deque<T, Alloc>::insert(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
	size_type n = distance(first, last);
	if (position.cur == start.cur) {
		iterator new_start = reserve_elements_at_front(n);
		try {
			mystl::uninitialized_copy(first, last, new_start);
			start = new_start;
		}
		catch (...) {
			destroy_nodes(new_start.node, start.node);
		}
	}
	else if (position.cur == finish.cur) {
		iterator new_finish = reserve_elements_at_back(n);
		try {
			mystl::uninitialized_copy(first, last, finish);
			finish = new_finish;
		}
		catch (...) {
			destroy_nodes(finish.node + 1, new_finish.node + 1);
		}
	}
	else
		insert_aux(position, first, last, n);
}

// insert函数在中部插入多个不同元素辅助函数(只对forward iterator有效)
// 调用函数有：
// 1. insert(position, first, last) -> insert_dispatch(position, first, last, false_type) -> insert(position, first, last, forward_iterator_tag) -> insert_aux(position, first, last, n)
template<class T, class Alloc>
template<class ForwardIterator>
void deque<T, Alloc>::insert_aux(iterator position, ForwardIterator first, ForwardIterator last, size_type n) {
	size_type elems_before = position - start;
	if (elems_before < size() / 2) {
		iterator new_start = reserve_elements_at_front(n);
		iterator old_start = start;
		position = start + elems_before;
		try {
			if (elems_before >= n) {
				iterator mid = start + n;
				mystl::uninitialized_copy(start, mid, new_start);
				mystl::copy(mid, position, start);
				mystl::copy(first, last, position - n);
				start = new_start;
			}
			else {
				mystl::uninitialized_copy(start, position, new_start);
				ForwardIterator mid = first;
				advance(mid, n - elems_before);
				mystl::uninitialized_copy(first, mid, new_start + elems_before);
				mystl::copy(mid, last, start);
				start = new_start;
			}
		}
		catch (...) {
			destroy_nodes(start.node, new_start.node);
		}
	}
	else {
		iterator new_finish = reserve_elements_at_back(n);
		iterator old_finish = finish;
		size_type elems_after = size() - elems_before;
		position = finish - elems_after;
		try {
			if (elems_after > n) {
				iterator mid = finish - n;
				mystl::uninitialized_copy(mid, finish, finish);
				mystl::copy_backward(position, mid, finish);
				copy(first, last, position);
				finish = new_finish;
			}
			else {
				mystl::uninitialized_copy(position, finish, new_finish - elems_after);
				ForwardIterator mid = first;
				advance(mid, elems_after);
				mystl::uninitialized_copy(mid, last, finish);
				mystl::copy(first, mid, position);
				finish = new_finish;
			}
		}
		catch (...) {
			destroy_nodes(finish.node + 1, new_finish.node + 1);
		}
	}
}

// insert_aux(const value_type*版本)
// 调用函数有：
// 1. insert(position, first, last) -> insert_aux
template<class T, class Alloc>
void deque<T, Alloc>::insert_aux(iterator position, const value_type* first, const value_type* last, size_type n) {
	size_type elems_before = position - start;
	if (elems_before < size() / 2) {
		iterator new_start = reserve_elements_at_front(n);
		iterator old_start = start;
		position = start + elems_before;
		try {
			if (elems_before >= n) {
				iterator mid = start + n;
				mystl::uninitialized_copy(start, mid, new_start);
				mystl::copy(mid, position, start);
				mystl::copy(first, last, position - n);
				start = new_start;
			}
			else {
				mystl::uninitialized_copy(start, position, new_start);
				const value_type* mid = first + (n - elems_before);
				mystl::uninitialized_copy(first, mid, new_start + elems_before);
				mystl::copy(mid, last, start);
				start = new_start;
			}
		}
		catch (...) {
			destroy_nodes(new_start.node, start.node);
		}
	}
	else {
		iterator new_finish = reserve_elements_at_back(n);
		iterator old_finish = finish;
		size_type elems_after = size() - elems_before;
		position = finish - elems_after;
		try {
			if (elems_after > n) {
				iterator mid = finish - n;
				mystl::uninitialized_copy(mid, finish, finish);
				mystl::copy_backward(position, mid, finish);
				mystl::copy(first, last, position);
				finish = new_finish;
			}
			else {
				const value_type* mid = first + elems_after;
				mystl::uninitialized_copy(position, finish, new_finish - elems_after);
				mystl::uninitialized_copy(mid, last, finish);
				mystl::copy(first, mid, position);
				finish = new_finish;
			}
		}
		catch (...) {
			destroy_nodes(finish.node + 1, new_finish.node + 1);
		}
	}
}

// insert_aux(const_iterator版本)
// 调用函数有：
// 1. insert(position, first, last) -> insert_aux
template<class T, class Alloc>
void deque<T, Alloc>::insert_aux(iterator position, const_iterator first, const_iterator last, size_type n) {
	size_type elems_before = position - start;
	if (elems_before < size() / 2) {
		iterator new_start = reserve_elements_at_front(n);
		iterator old_start = start;
		position = start + elems_before;
		try {
			if (elems_before >= n) {
				iterator mid = start + n;
				mystl::uninitialized_copy(start, mid, new_start);
				mystl::copy(mid, position, start);
				mystl::copy(first, last, position - n);
				start = new_start;
			}
			else {
				mystl::uninitialized_copy(start, position, new_start);
				const_iterator mid = first + (n - elems_before);
				mystl::uninitialized_copy(first, mid, new_start + elems_before);
				mystl::copy(mid, last, start);
				start = new_start;
			}
		}
		catch (...) {
			destroy_nodes(new_start.node, start.node);
		}
	}
	else {
		iterator new_finish = reserve_elements_at_back(n);
		iterator old_finish = finish;
		size_type elems_after = size() - elems_before;
		position = finish - elems_after;
		try {
			if (elems_after > n) {
				iterator mid = finish - n;
				mystl::uninitialized_copy(mid, finish, finish);
				mystl::copy_backward(position, mid, finish);
				mystl::copy(first, last, position);
				finish = new_finish;
			}
			else {
				const_iterator mid = first + elems_after;
				mystl::uninitialized_copy(position, finish, new_finish - elems_after);
				mystl::uninitialized_copy(mid, last, finish);
				mystl::copy(first, mid, position);
				finish = new_finish;
			}
		}
		catch (...) {
			destroy_nodes(finish.node + 1, new_finish.node + 1);
		}
	}
}



//------------------------------------------------------常用函数-------------------------------------------------------
template<class T, class Alloc>
bool operator==(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) {
	return lhs.size() == rhs.size() && mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class T, class Alloc>
bool operator!=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) {
	return !(lhs == rhs);
}

template<class T, class Alloc>
bool operator<(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) {
	return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, class Alloc>
bool operator>(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) {
	return rhs < lhs;
}

template<class T, class Alloc>
bool operator<=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) {
	return !(rhs < lhs);
}

template<class T, class Alloc>
bool operator>=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) {
	return !(lhs < rhs);
}

template<class T, class Alloc>
void swap(deque<T, Alloc>& lhs, deque<T, Alloc>& rhs) {
	lhs.swap(rhs);	
}



}		// mystl


#endif