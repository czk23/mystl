#ifndef MYSTL_LIST_H
#define MTSTL_LIST_H

#include "iterator.h"
#include "memory.h"
#include "type_traits.h"
#include <cstddef>
#include <cassert>

namespace mystl {

template<class T>
struct list_node_base;

template<class T>
struct list_node;

//-----------------------------------------------list�Ľ��---------------------------------------------------------------
// list_node_base�Ľ��ṹ
template<class T>
struct list_node_base {
	typedef list_node_base<T>*						base_ptr;
	typedef list_node<T>*							node_ptr;

	base_ptr prev;
	base_ptr next;

	list_node_base() = default;

	base_ptr self() {
		return static_cast<base_ptr> (&*this);
	}

	void unlink() {
		prev = self();
		next = self();
	}

	node_ptr as_node() {
		return static_cast<node_ptr> (&*this);
	}
};

// list_node�Ľ��ṹ
template<class T>
struct list_node : public list_node_base<T> {
	typedef list_node_base<T>* base_ptr;
	typedef list_node<T>* node_ptr;

	T data;

	list_node() = default;

	list_node(const T& value) : data(value) {};

	node_ptr self() {
		return static_cast<node_ptr> (&*this);
	}

	base_ptr as_base() {
		return static_cast<base_ptr> (&*this);
	}
};

//-----------------------------------------------list�ĵ�����---------------------------------------------------------------
// list_iterator_base�ṹ
template<class T>
struct list_iterator_base : public mystl::iterator<bidirectional_iterator_tag, T> {
	typedef list_node_base<T>*						base_ptr;
	typedef list_node<T>*							node_ptr;

	typedef	size_t									size_type;
	typedef ptrdiff_t								difference_type;
	typedef bidirectional_iterator_tag				iterator_tag;

	base_ptr node;

	list_iterator_base() = default;

	list_iterator_base(base_ptr other) : node(other) {};

	void incr() {
		node = node->next;
	}

	void decr() {
		node = node->prev;
	}

	bool operator==(const list_iterator_base<T>& rhs) {
		return rhs.node == node;
	}

	bool operator!=(const list_iterator_base<T>& rhs) {
		return rhs.node != node;
	}
};

// list_iterator�ṹ
template<class T>
struct list_iterator : public list_iterator_base<T> {
	// ����ָ��
	typedef list_node_base<T>*						base_ptr;
	typedef list_node<T>*							node_ptr;

	// ��������
	typedef T										value_type;
	typedef T*										pointer;
	typedef T&										reference;
	typedef bidirectional_iterator_tag				iterator_category;
	typedef ptrdiff_t								difference_type;
	typedef size_t									size_type;

	// ����
	typedef list_iterator<T>						self;

	using list_iterator_base<T>::node;

	// ���캯��
	list_iterator() = default;

	list_iterator(base_ptr other) : list_iterator_base<T>(other) {};

	list_iterator(node_ptr other) : list_iterator_base<T>(other) {};

	list_iterator(const self& other) : list_iterator_base<T>(other.node) {};

	// ���س�Ա���ʺ���������
	reference operator*() {
		return static_cast<node_ptr>(this->node)->data;
	}

	pointer operator->() {
		return &(operator*());
	}

	// �����������Լ������
	self& operator++() {
		this->incr();
		return *this;
	}

	self operator++(int) {
		self temp = *this;
		this->incr();
		return temp;
	}

	self& operator--() {
		this->decr();
		return *this;
	}

	self operator--(int) {
		self temp = *this;
		this->decr();
		return temp;
	}

	bool operator==(const self& rhs) const {
		return node == rhs.node;
	}

	bool operator!=(const self& rhs) const {
		return node != rhs.node;
	}
};

// list_const_iterator�Ľṹ
template<class T>
struct list_const_iterator : public list_iterator_base<T> {
	// ����ָ��
	typedef list_node_base<T>*						base_ptr;
	typedef list_node<T>*							node_ptr;

	// ��������
	typedef T										value_type;
	typedef const T*								pointer;
	typedef const T&								reference;
	typedef bidirectional_iterator_tag				iterator_category;
	typedef ptrdiff_t								difference_type;
	typedef size_t									size_type;

	// ����
	typedef list_const_iterator<T>					self;

	using list_iterator_base<T>::node;

	// ���캯��
	list_const_iterator() = default;

	list_const_iterator(base_ptr other) : list_iterator_base<T>(other) {};

	list_const_iterator(node_ptr other) : list_iterator_base<T>(other) {};

	list_const_iterator(const self& other) : list_iterator_base<T>(other.node) {};

	// ���س�Ա���ʺ���������
	reference operator*() {
		return static_cast<node_ptr>(this->node)->data;
	}

	pointer operator->() {
		return &(operator*());
	}

	// �����������Լ������
	self& operator++() {
		this->incr();
		return *this;
	}

	self operator++(int) {
		self temp = *this;
		this->incr();
		return temp;
	}

	self& operator--() {
		this->decr();
		return *this;
	}

	self operator--(int) {
		self temp = *this;
		this->decr();
		return temp;
	}

	bool operator==(const self& rhs) const {
		return node == rhs.node;
	}

	bool operator!=(const self& rhs) const {
		return node != rhs.node;
	}
};

//-----------------------------------------------list---------------------------------------------------------------
// list�Ľṹ
template<class T, class Alloc = alloc>
class list {
public:
// ����ָ��
	typedef list_node_base<T>*										base_ptr;
	typedef list_node<T>*											node_ptr;

// ������
	typedef simple_alloc<T, Alloc>									allocator_type;
	typedef simple_alloc<T, Alloc>									data_allocator;
	typedef simple_alloc<list_node_base<T>, Alloc>					base_allocator;
	typedef simple_alloc<list_node<T>, Alloc>						node_allocator;

// ����
	typedef T														value_type;
	typedef T*														pointer;
	typedef const T*												const_pointer;
	typedef T&														reference;
	typedef const T&												const_reference;
	typedef size_t													size_type;
	typedef ptrdiff_t												difference_type;

// ������
	typedef list_iterator<T>										iterator;
	typedef list_const_iterator<T>									const_iterator;
	typedef reverse_iterator<const_iterator>						const_reverse_iterator;
	typedef reverse_iterator<iterator>								reverse_iterator;

// ����������
	allocator_type get_allocator() {
		return data_allocator();
	}

private:
// ά��һ��ͷ���
	node_ptr node;

public:
// ���졢���ƺ���������
	list() ;

	explicit list(size_type n);
	
	list(size_type n, const value_type& value);

	template<class InputIterator>
	list(InputIterator first, InputIterator last);

	list(std::initializer_list<T> ilist);

	list(const_pointer first, const_pointer last);

	list(const_iterator first, const_iterator last);

	list(const list<T, Alloc>& rhs);

	list<T, Alloc>& operator=(list<T, Alloc>& rhs);

	list<T, Alloc>& operator=(std::initializer_list<value_type> ilist);

	~list();

// ��������غ���
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

// ������غ���
	bool empty() const;

	size_type size() const;

	size_type max_size() const;

// ����Ԫ����غ���
	reference front();

	const_reference front() const;

	reference back();

	const_reference back() const;

// ����������غ���
	void assign(size_type n, const value_type& value);

	template<class InputIterator>
	void assign(InputIterator first, InputIterator last);

	void assign(std::initializer_list<T>& ilist);

	iterator insert(iterator position, const value_type& value);

	iterator insert(iterator position);
	
	void insert(iterator position, size_type n, const value_type& value);

	template<class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last);

	void insert(iterator position, const T* first, const T* last);

	void insert(iterator position, const_iterator first, const_iterator last);

	void push_front(const value_type& value);

	void push_front();

	void push_back(const value_type& value);

	void push_back();

	void pop_front();

	void pop_back();

	iterator erase(iterator position);

	iterator erase(iterator first, iterator last);

	void clear();

	void resize(size_type new_size, const value_type& value);

	void resize(size_type new_size);

	void swap(list<T, Alloc>& rhs);

// list��غ���
	void splice(iterator position, list<T, Alloc>& other);

	void splice(iterator position, list<T, Alloc>& other, iterator i);

	void splice(iterator position, list<T, Alloc>& other, iterator first, iterator last);

	void remove(const value_type& value);

	void unique();

	void merge(list<T, Alloc>& other);

	void reverse();

	void sort();

	template<class Predicate>
	void remove_if(Predicate Pred);

	template<class BinaryPredicate>
	void unique(BinaryPredicate binary_pred);

	template<class StrictWeakOrdering>
	void merge(list<T, Alloc>& other, StrictWeakOrdering comp);

	template<class StrictWeakOrdering>
	void sort(StrictWeakOrdering comp);

// ��������
private:
	void allocate_and_init();

	void fill_assign(size_type n, const value_type& value);

	template<class Integer>
	void assign_dispatch(Integer n, Integer value, true_type);

	template<class InputIterator>
	void assign_dispatch(InputIterator first, InputIterator last, false_type);

	node_ptr get_node();

	void put_node(node_ptr p);

	node_ptr create_node();

	node_ptr create_node(const value_type& value);

	void fill_insert(iterator position, size_type n, const value_type& value);

	template<class Integer>
	void insert_dispatch(iterator position, Integer n, Integer value, true_type);

	template<class InputIterator>
	void insert_dispatch(iterator position, InputIterator first, InputIterator last, false_type);

	void transfer(iterator position, iterator first, iterator last);

};

//-----------------------------------------------���졢���ƺ���������---------------------------------------------------------------
template<class T, class Alloc>
list<T, Alloc>::list() {
	allocate_and_init();
}

template<class T, class Alloc>
list<T, Alloc>::list(size_type n) {
	allocate_and_init();
	insert(begin(), n, T());
}

template<class T, class Alloc>
list<T, Alloc>::list(size_type n, const value_type& value) {
	allocate_and_init();
	insert(begin(), n, value);
}

template<class T, class Alloc>
template<class InputIterator>
list<T, Alloc>::list(InputIterator first, InputIterator last) {
	allocate_and_init();
	insert(begin(), first, last);
}

template<class T, class Alloc>
list<T, Alloc>::list(std::initializer_list<T> ilist) {
	allocate_and_init();
	insert(begin(), ilist.begin(), ilist.end());
}

template<class T, class Alloc>
list<T, Alloc>::list(const_pointer first, const_pointer last) {
	allocate_and_init();
	insert(begin(), first, last);
}

template<class T, class Alloc>
list<T, Alloc>::list(const_iterator first, const_iterator last) {
	allocate_and_init();
	insert(begin(), first, last);
}

template<class T, class Alloc>
list<T, Alloc>::list(const list<T, Alloc>& rhs) {
	allocate_and_init();
	insert(begin(), rhs.begin(), rhs.end());
}

template<class T, class Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(list<T, Alloc>& rhs) {
	if (rhs != *this) {
		iterator first1 = begin();
		iterator last1 = end();
		const_iterator first2 = rhs.cbegin();
		const_iterator last2 = rhs.cend();
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			*first1 = *first2;
		}
		if (first1 == last1) {
			insert(last1, first2, last2);
		}
		else {
			erase(first1, last1);
		}
		return *this;
	}
}

template<class T, class Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(std::initializer_list<T> ilist) {
	iterator first1 = begin();
	iterator last1 = end();
	auto first2 = ilist.begin();
	auto last2 = ilist.end();
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		*first1 = *first2;
	}
	if (first1 == last1) {
		insert(last1, first2, last2);
	}
	else {
		erase(first1, last1);
	}
	return *this;
}

template<class T, class Alloc>
list<T, Alloc>::~list() {
	if (node) {
		clear();
		node_allocator::deallocate(node);
		node = nullptr;
	}
}


//-----------------------------------------------��������غ���---------------------------------------------------------------
template<class T, class Alloc>
typename list<T, Alloc>::iterator 
list<T, Alloc>::begin() {
	return node->next;
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator 
list<T, Alloc>::begin() const{
	return node->next;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator 
list<T, Alloc>::end() {
	return node;
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator 
list<T, Alloc>::end() const {
	return node;
}

template<class T, class Alloc>
typename list<T, Alloc>::reverse_iterator 
list<T, Alloc>::rbegin() {
	return reverse_iterator(end());
}

template<class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator 
list<T, Alloc>::rbegin() const {
	return reverse_iterator(end());
}

template<class T, class Alloc>
typename list<T, Alloc>::reverse_iterator 
list<T, Alloc>::rend() {
	return reverse_iterator(begin());
}

template<class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator 
list<T, Alloc>::rend() const {
	return const_reverse_iterator(begin());
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator 
list<T, Alloc>::cbegin() const {
	return begin();
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator 
list<T, Alloc>::cend() const {
	return end();
}

template<class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator 
list<T, Alloc>::crbegin() const {
	return rbegin();
}

template<class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator 
list<T, Alloc>::crend() const {
	return rend();
}


//-----------------------------------------------������غ���---------------------------------------------------------------
template<class T, class Alloc>
bool list<T, Alloc>::empty() const {
	return node == static_cast<node_ptr>(node->next);
}

template<class T, class Alloc>
typename list<T, Alloc>::size_type 
list<T, Alloc>::size() const {
	size_type n = distance(begin(), end());
	return n;
}

template<class T, class Alloc>
typename list<T, Alloc>::size_type 
list<T, Alloc>::max_size() const {
	return static_cast<size_type>(-1);
}


//-----------------------------------------------����Ԫ����غ���---------------------------------------------------------------
template<class T, class Alloc>
typename list<T, Alloc>::reference 
list<T, Alloc>::front() {
	assert(!empty());
	return *begin();
}

template<class T, class Alloc>
typename list<T, Alloc>::const_reference 
list<T, Alloc>::front() const {
	assert(!empty());
	return *begin();
}

template<class T, class Alloc>
typename list<T, Alloc>::reference 
list<T, Alloc>::back() {
	assert(!empty());
	return *(--end());
}

template<class T, class Alloc>
typename list<T, Alloc>::const_reference 
list<T, Alloc>::back() const {
	assert(!empty());
	return *(--end());
}

//-----------------------------------------------����������غ���---------------------------------------------------------------
// Ϊlist��ֵ�����ܲ���n��value
template<class T, class Alloc>
void list<T, Alloc>::assign(size_type n, const value_type& value) {
	fill_assign(n, value);
}

// ģ�庯����Ϊlist��ֵ������������first��last��Ҳ������n��value
template<class T, class Alloc>
template<class InputIterator>
void list<T, Alloc>::assign(InputIterator first, InputIterator last) {
	typedef typename Is_integer<InputIterator>::value Integer;
	assign_dispatch(first, last, Integer());
}

// Ϊlist���������ܲ���std::initializer_list
template<class T, class Alloc>
void list<T, Alloc>::assign(std::initializer_list<T>& ilist) {
	iterator first1 = begin();
	iterator last1 = end();
	auto first2 = ilist.begin();
	auto last2 = ilist.end();
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		*first1 = *first2;
	}
	if (first1 == last1) {
		insert(last1, first2, last2);
	}
	else {
		erase(first2, last2);
	}
}

// ָ��λ��ǰ����һ��Ԫ�أ����ܲ���Ϊ����λ�úͲ���ֵ
template<class T, class Alloc>
typename list<T, Alloc>::iterator 
list<T, Alloc>::insert(iterator position, const value_type& value) {
//	assert(size() + 1 <= max_size());
	node_ptr temp = create_node(value);
	temp->next = position.node;
	temp->prev = position.node->prev;
	position.node->prev->next = temp;
	position.node->prev = temp;
	return temp;
}

// ָ��λ��ǰ����һ��Ԫ�أ����ܲ���Ϊ����λ��
template<class T, class Alloc>
typename list<T, Alloc>::iterator 
list<T, Alloc>::insert(iterator position) {
	insert(position, T());
}

// ָ��λ��ǰ������ֵ�����ܲ���Ϊn��value
template<class T, class Alloc>
void list<T, Alloc>::insert(iterator position, size_type n, const value_type& value) {
	fill_insert(position, n, value);
}

// ģ�庯����ָ��λ��ǰ������Ԫ�أ�����������first��last��Ҳ������n��value
template<class T, class Alloc>
template<class InputIterator>
void list<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
	typedef typename Is_integer<InputIterator>::value Integer;
	insert_dispatch(position, first, last, Integer());
}

// ָ��λ��ǰ������ֵ�����ܲ���Ϊfirst��last
template<class T, class Alloc>
void list<T, Alloc>::insert(iterator position, const T* first, const T* last) {
	for (; first != last; ++first) {
		insert(position, *first);
	}
}

// ָ��λ��ǰ������ֵ�����ܲ���Ϊfirst��last
template<class T, class Alloc>
void list<T, Alloc>::insert(iterator position, const_iterator first, const_iterator last) {
	for (; first != last; ++first) {
		insert(position, *first);
	}
}

// ��ͷ������һ��Ԫ�أ�����һ������value
template<class T, class Alloc>
void list<T, Alloc>::push_front(const value_type& value) {
	insert(begin(), value);
}

// ��ͷ������һ��Ԫ��
template<class T, class Alloc>
void list<T, Alloc>::push_front() {
	insert(begin(), T());
}

// ��β������һ��Ԫ�أ�����һ������value
template<class T, class Alloc>
void list<T, Alloc>::push_back(const value_type& value) {
	insert(end(), value);
}

// ��β������һ��Ԫ��
template<class T, class Alloc>
void list<T, Alloc>::push_back() {
	insert(end(), T());
}

// ����ͷ��һ��Ԫ��
template<class T, class Alloc>
void list<T, Alloc>::pop_front() {
	erase(begin());
}

// ����β��һ��Ԫ��
template<class T, class Alloc>
void list<T, Alloc>::pop_back() {
	erase(--end());
}

// ���ָ��λ���ϵ�Ԫ��
template<class T, class Alloc>
typename list<T, Alloc>::iterator 
list<T, Alloc>::erase(iterator position) {	
	list_node_base<T>* next_node = position.node->next;
	position.node->prev->next = position.node->next;
	position.node->next->prev = position.node->prev;
	node_ptr n = static_cast<node_ptr>(position.node);
	mystl::destroy(&n->data);
	put_node(n);
	return iterator(next_node);
}

// ���ָ�������ϵ�Ԫ��
template<class T, class Alloc>
typename list<T, Alloc>::iterator 
list<T, Alloc>::erase(iterator first, iterator last) {
	while (first != last) {
		erase(first++);
	}
	return last;
}

// ������н��
template<class T, class Alloc>
void list<T, Alloc>::clear() {
	node_ptr cur = static_cast<node_ptr>(node->next);
	while (cur != node) {
		node_ptr temp = cur;
		cur = static_cast<node_ptr>(cur->next);
		mystl::destroy(&temp->data);
		put_node(temp);
	}
	node->prev = node;
	node->next = node;
}

// ����ָ��list�Ĵ�С��������������new_size��value(�²���Ԫ�ص�ֵ��ԭ��Ԫ�ص�ֵ����)
template<class T, class Alloc>
void list<T, Alloc>::resize(size_type new_size, const value_type& value) {
	size_type len = 0;
	iterator i = begin();
	for (; i != end() && len < new_size; ++len, ++i)
		;
	if (len == new_size) {
		erase(i, end());
	}
	else {
		insert(i, new_size - len, value);
	}
}

// ����ָ��list�Ĵ�С������һ������new_size
template<class T, class Alloc>
void list<T, Alloc>::resize(size_type new_size) {
	resize(new_size, T());
}

// ��������list
template<class T, class Alloc>
void list<T, Alloc>::swap(list<T, Alloc>& rhs) {
	mystl::swap(node, rhs.node);
}


//-----------------------------------------------list��غ���---------------------------------------------------------------
// ����һ��list���뵽position֮ǰ
template<class T, class Alloc>
void list<T, Alloc>::splice(iterator position, list<T, Alloc>& other) {
	if (other.node->next != other.node) {
		transfer(position, other.begin(), other.end());
	}
}

// ��һ�������뵽position֮ǰ(������ͬһ��listҲ���ܲ���ͬһ��list)
template<class T, class Alloc>
void list<T, Alloc>::splice(iterator position, list<T, Alloc>& other, iterator i) {
	iterator j = i.node->next;
	if (i == position || j == position)
		return;
	transfer(position, i, j);
}

// ��һ��������в��뵽position֮ǰ(������ͬһ��listҲ���ܲ���ͬһ��list)
template<class T, class Alloc>
void list<T, Alloc>::splice(iterator position, list<T, Alloc>& other, iterator first, iterator last) {
	if (first != last)
		transfer(position, first, last);
}

// ��list�н���ֵ����value�Ľ�����
template<class T, class Alloc>
void list<T, Alloc>::remove(const value_type& value) {
	iterator first = begin();
	iterator last = end();
	while (first != last) {
		iterator next = first;
		++next;
		if (*first == value)
			erase(first);
		first = next;
	}
}

// ����������ڵ�ֵ��ͬ�Ľ��
template<class T, class Alloc>
void list<T, Alloc>::unique() {
	if (node->next == node || node->next->next == node)
		return;
	iterator first = begin();
	iterator last = end();
	iterator next = first;
	while (++next != last) {
		if (*first == *next)
			erase(next);
		else
			first = next;
		next = first;
	}
}

// �����������list�ϲ���һ�������list
template<class T, class Alloc>
void list<T, Alloc>::merge(list<T, Alloc>& other) {
	iterator first1 = begin();
	iterator last1 = end();
	iterator first2 = other.begin();
	iterator last2 = other.end();
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1) {
			iterator next = first2;
			transfer(first1, first2, ++next);
			first2 = next;
		}
		else
			++first1;
	}
	if (first2 != last2) {
		transfer(last1, first2, last2);
	}
}

// ��list����
template<class T, class Alloc>
void list<T, Alloc>::reverse() {
	if (node->next == node || node->next->next == node)
		return;
	iterator first = begin();
	++first;
	while (first != end()) {
		iterator old = first;
		++first;
		transfer(begin(), old, first);
	}
}

// ��list��������(stl��sort��list�����ã���ΪҪ�������Ϊrandom access����)
// �㷨˼�����ƹ鲢����Ͷ����Ƽӷ�
template<class T, class Alloc>
void list<T, Alloc>::sort() {
	if (node->next == node || node->next->next == node)
		return;
	list<T, Alloc> carry;
	list<T, Alloc> counter[64];
	int fill = 0;
	while (!empty()) {
		carry.splice(carry.begin(), *this, begin());
		int i = 0;
		while (i < fill && !counter[i].empty()) {
			counter[i].merge(carry);
			carry.swap(counter[i++]);
		}
		carry.swap(counter[i]);
		if (i == fill)
			++fill;
	}
	for (int i = 1; i < fill; ++i)
		counter[i].merge(counter[i - 1]);
	swap(counter[fill - 1]);
}

// ģ�庯����remove�����ĸĽ���ԭ��ֵ��ȵ�������Ϊģ�����
template<class T, class Alloc>
template<class Predicate>
void list<T, Alloc>::remove_if(Predicate pred) {
	iterator first = begin();
	iterator last = end();
	while (first != last) {
		iterator next = first;
		++next;
		if (pred(*first))
			erase(first);
		first = next;
	}
}

// ģ�庯����unique�����ĸĽ���ԭ��ֵ��ȵ�������Ϊģ�����
template<class T, class Alloc>
template<class BinaryPredicate>
void list<T, Alloc>::unique(BinaryPredicate binary_pred) {
	if (node->next == node || node->next->next == node)
		return;
	iterator first = begin();
	iterator last = end();
	if (first == last)
		return;
	iterator next = first;
	while (++next != last) {
		if (binary_pred(*first, *next))
			erase(next);
		else
			first = next;
		next = first;
	}
}

// ģ�庯����merge�����ĸĽ��棬ԭ�������С�ڹ�ϵ��Ϊģ�����
template<class T, class Alloc>
template<class StrictWeakOrdering>
void list<T, Alloc>::merge(list<T, Alloc>& other, StrictWeakOrdering comp) {
	iterator first1 = begin();
	iterator last1 = end();
	iterator first2 = other.begin();
	iterator last2 = other.end();
	while (first1 != last1 && first2 != last2) {
		if (comp(*first2, *first1)) {
			iterator next = first2;
			transfer(first1, first2, ++next);
			first2 = next;
		}
		else
			++first1;
	}
	if (first2 != last2)
		transfer(last1, first2, last2);
}

// ģ�庯����sort�����ĸĽ��棬ԭ�������С�ڹ�ϵ��Ϊģ�����
template<class T, class Alloc>
template<class StrictWeakOrdering>
void list<T, Alloc>::sort(StrictWeakOrdering comp) {
	if (node->next == node || node->next->next == node)
		return;
	list<T, Alloc> carry;
	list<T, Alloc> counter[64];
	int fill = 0;
	while (!empty()) {
		splice(carry.begin(), *this, begin());
		int i = 0;
		while (i < fill && !counter[i].empty()) {
			counter[i].merge(carry, comp);
			carry.swap(counter[i++]);
		}
		carry.swap(counter[i]);
		if (i == fill)
			fill++;
	}
	for (int i = 1; i < fill; ++i)
		counter[i].merge(counter[i - 1], comp);
	swap(counter[fill - 1]);
}


//-----------------------------------------------��������---------------------------------------------------------------
// ��ʼ��list
// ���õĺ������ǹ��캯��
template<class T, class Alloc>
void list<T, Alloc>::allocate_and_init() {
	node = node_allocator::allocate(1);
	node->prev = node;
	node->next = node;
}

// ���list��ֵ
// ���ú�����:
// 1. assign(n, value) -> assign_dispatch(n, value, true_type) -> fill_assign
// 2. assign(n, value) -> fill_assign
template<class T, class Alloc>
void list<T, Alloc>::fill_assign(size_type n, const value_type& value) {
	iterator i = begin();
	for (; i != end() && n > 0; ++i, --n) {
		*i = value;
	}
	if (i == end()) {
		insert(end(), n, value);
	}
	else {
		erase(i, end());
	}
}

// ģ�庯������ģ�����Ϊ����ʱassign��������
// ���ú�����:
// 1. assign(n, value) -> assign_dispatch
template<class T, class Alloc>
template<class Integer>
void list<T, Alloc>::assign_dispatch(Integer n, Integer value, true_type) {
	fill_assign(n, value);
}

// ģ�庯������ģ�����Ϊ������ʱassign��������
// ���ú�����:
// 1. assign(first, last) -> assign_dispatch
template<class T, class Alloc>
template<class InputIterator>
void list<T, Alloc>::assign_dispatch(InputIterator first, InputIterator last, false_type) {
	iterator first1 = begin();
	iterator last1 = end();
	for (; first1 != last1 && first != last; ++first1, ++first) {
		*first1 = *first;
	}
	if (first1 == last1) {
		insert(last1, first, last);
	}
	else {
		erase(first1, last1);
	}
}

// ����һ�����
// ���ú�����:
// 1. insert(iterator position) -> create_node() -> get_node
// 2. insert(iterator position, value_type& value) -> create_node(value) -> get_node
template<class T, class Alloc>
typename list<T, Alloc>::node_ptr 
list<T, Alloc>::get_node() {
	return node_allocator::allocate(1);
}

// ���һ�����
// ���ú�����:
// 1. erase(iterator position) -> put_node
// 2. clear() -> put_node
template<class T, class Alloc>
void list<T, Alloc>::put_node(node_ptr p) {
	node_allocator::deallocate(p, 1);
}

// ����һ����㲢��c��ֵ
// ���ú�����:
// 1. insert(iterator position, const value_type& value) -> create_node
template<class T, class Alloc>
typename list<T, Alloc>::node_ptr 
list<T, Alloc>::create_node() {
	node_ptr p = get_node();
	try {
		mystl::construct(p, T());
	}
	catch (...) {
		mystl::destroy(p);
		put_node(p);
	}
	return p;
}

// ����һ����㲢����ֵ
// ���ú�����:
// 1. insert(iterator position) -> create_node
template<class T, class Alloc>
typename list<T, Alloc>::node_ptr 
list<T, Alloc>::create_node(const value_type& value) {
	node_ptr p = get_node();
	try {
		mystl::construct(p, value);
	}
	catch (...) {
		mystl::destroy(p);
		put_node(p);
	}
	return p;
}

// ָ��λ�ò���ֵ
// ���ú�����:
// 1. insert(n, value) -> insert_dispatch(n, value, true_type) -> fill_insert
// 2. insert(n, value) -> fill_insert
template<class T, class Alloc>
void list<T, Alloc>::fill_insert(iterator position, size_type n, const value_type& value) {
	for (; n > 0; --n)
		insert(position, value);
}

// ģ�庯����ģ�����Ϊ����ʱinsert��������
// ���ú�����:
// 1. insert(n, value) -> insert_dispatch
template<class T, class Alloc>
template<class Integer>
void list<T, Alloc>::insert_dispatch(iterator position, Integer n, Integer value, true_type) {
	fill_insert(position, n, static_cast<value_type>(value));
}

// ģ�庯����ģ�����Ϊ������ʱinsert��������
// ���ú�����:
// 1. insert(first, last) -> insert_dispatch
template<class T, class Alloc>
template<class InputIterator>
void list<T, Alloc>::insert_dispatch(iterator position, InputIterator first, InputIterator last, false_type) {
	for (; first != last; ++first)
		insert(position, *first);
}

// ��first��last���뵽positionλ�ô�
// ���ú�����:
// 1. splice(iterator position, list<T, Alloc> other) -> transfer
// 2. splice(iterator position, list<T, Alloc> other, iterator i) -> transfer
// 3. splice(iterator position, list<T, Alloc> other, iterator first, iterator last) -> transfer
// 4. merge() -> transfer
// 5. reverse() -> transfer
// 6. merge(comp) -> transfer
template<class T, class Alloc>
void list<T, Alloc>::transfer(iterator position, iterator first, iterator last) {
	if (position != last) {
		last.node->prev->next = position.node;
		first.node->prev->next = last.node;
		position.node->prev->next = first.node;
		list_node_base<T>* temp = position.node->prev;
		position.node->prev = last.node->prev;
		last.node->prev = first.node->prev;
		first.node->prev = temp;
	}
}


//-----------------------------------------------���ú���(������list��Ա����)----------------------------------------------------
// ����opeartor==
template<class T, class Alloc>
bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
	typedef typename list<T, Alloc>::const_iterator const_iterator;
	const_iterator first1 = lhs.begin();
	const_iterator last1 = lhs.end();
	const_iterator first2 = rhs.begin();
	const_iterator last2 = rhs.end();
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (*first1 != *first2)
			return false;
	}
	return (first1 == last1) && (first2 == last2);
}

// ����opeartor!=
template<class T, class Alloc>
bool operator!=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
	return !(lhs == rhs);
}

// ����opeartor<
template<class T, class Alloc>
bool operator<(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
	return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

// ����operator>
template<class T, class Alloc>
bool operator>(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
	return rhs < lhs;
}

// ����operator<=
template<class T, class Alloc>
bool operator<=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
	return !(rhs < lhs);
}

// ����operator>=
template<class T, class Alloc>
bool operator>=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
	return !(lhs < rhs);
}

// ��������list
template<class T, class Alloc>
void swap(list<T, Alloc> lhs, list<T, Alloc> rhs) {
	lhs.swap(rhs);
}


}	// mystl

#endif