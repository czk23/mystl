#ifndef MYSTL_QUEUE_H
#define MYSTL_QUEUE_H

// 这个文件包含两个容器queue和priority_queue
// queue作为deque的adpter，priority_queue调用heap算法实现

#include "deque.h"
#include "heap_algo.h"
#include "vector.h"
#include "functional.h"

namespace mystl {

// -------------------------------------------------------queue-------------------------------------------------------
template<class T, class Sequence = deque<T>>
class queue {
	// typedef
	typedef Sequence								container_type;
	typedef typename Sequence::value_type			value_type;
	typedef typename Sequence::size_type			size_type;
	typedef typename Sequence::reference			reference;
	typedef typename Sequence::const_reference		const_reference;

	static_assert(std::is_same<T, value_type>::value,
		"the value_type of Container should be same with T");

private:
	// 维护一个底层容器
	container_type c;

public:
	// 构造、移动和析构函数
	queue() = default;

	explicit queue(size_type n) : c(n){

	}

	queue(size_type n, const value_type& value) : c(n, value) {
		
	}

	queue(const std::initializer_list<T>& ilist) : c(ilist) {
		
	}

	template<class InputIterator>
	queue(InputIterator first, InputIterator last) : c(first, last) {
		
	}

	queue(container_type& container) : c(container) {
		
	}

	queue(const queue& rhs) : c(rhs.c) {
		
	}

	queue& operator=(const queue& rhs) {
		c = rhs.c;
		return *this;
	}

	queue& operator=(const std::initializer_list<T>& ilist) {
		c = ilist;
		return *this;
	}

	~queue() = default;

	// 访问元素相关函数
	reference front() {
		return c.front();
	}

	const_reference front() const {
		return c.front();
	}

	reference back() {
		return c.back();
	}

	const_reference back() const {
		return c.back();
	}

	// 容量相关函数
	bool empty() const {
		return c.empty();
	}

	size_type size() const {
		return c.size();
	}

	// 修改容器相关函数
	void push(const value_type& value) {
		c.push_back(value);
	}

	void pop() {
		c.pop_front();
	}

	void clear() {
		while (!empty())
			pop();
	}

	void swap(queue rhs) {
		c.swap(rhs.c);
	}

	friend bool operator==(const queue& lhs, const queue& rhs) {
		return lhs.c == rhs.c;
	}

	friend bool operator<(const queue& lhs, const queue& rhs) {
		return lhs.c < rhs.c;
	}
};

// 常用函数
template<class T, class Sequence>
bool operator==(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
	return lhs == rhs;
}

template<class T, class Sequence>
bool operator<(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
	return lhs < rhs;
}

template<class T, class Sequence>
bool operator!=(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
	return !(lhs == rhs);
}

template<class T, class Sequence>
bool operator>(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
	return rhs < lhs;
}

template<class T, class Sequence>
bool operator<=(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
	return !(rhs < lhs);
}

template<class T, class Sequence>
bool operator>=(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
	return !(lhs < rhs);
}

template<class T, class Sequence>
void swap(queue<T, Sequence> lhs, queue<T, Sequence> rhs) {
	return lhs.swap(rhs);
}


//-------------------------------------------------------priority_queue-------------------------------------------------------
template<class T, class Sequence = vector<T>, class Compare = mystl::less<typename Sequence::value_type>>
class priority_queue {
public:
	// typedef
	typedef Sequence											container_type;
	typedef typename Sequence::value_type						value_type;
	typedef typename Sequence::size_type						size_type;
	typedef typename Sequence::reference						reference;
	typedef typename Sequence::const_reference					const_reference;

	// 维护一个底层容器和一个比较函数
protected:
	container_type c;
	Compare comp;

public:
	// 构造、复制和析构函数
	priority_queue() = default;

	priority_queue(const Compare& x) : comp(x) {};

	priority_queue(const container_type& con) : c(con) {
		make_heap(c.begin(), c.end(), comp);
	}

	priority_queue(const Compare& x, const container_type& con) : c(con), comp(x) {
		make_heap(c.begin(), c.end(), comp);
	};

	priority_queue(size_type n) : c(n) {
		make_heap(c.begin(), c.end(), comp);
	}

	priority_queue(size_type n, const value_type& value) : c(n, value) {
		make_heap(c.begin(), c.end(), comp);
	}

	priority_queue(const std::initializer_list<T>& ilist) : c(ilist) {
		make_heap(c.begin(), c.end(), comp);
	}

	template<class InputIterator>
	priority_queue(InputIterator first, InputIterator last) : c(first, last) {
		make_heap(c.begin(), c.end(), comp);
	}

	template<class InputIterator>
	priority_queue(InputIterator first, InputIterator last, const Compare& x) : c(first, last), comp(x) {
		make_heap(c.begin(), c.end(), comp);
	}

	template<class InputIterator>
	priority_queue(InputIterator first, InputIterator last, const Compare& x, const container_type& con) : c(con), comp(x) {
		c.insert(c.end(), first, last);
		make_heap(c.begin(), c.end(), comp);
	}

	priority_queue(value_type* first, value_type* last) : c(first, last) {
		make_heap(c.begin(), c.end(), comp);
	}

	priority_queue(value_type* first, value_type* last, const Compare& x) : c(first, last), comp(x) {
		make_heap(c.begin(), c.end(), comp);
	}

	priority_queue(value_type* first, value_type* last, const Compare& x, const container_type& con) : c(con), comp(x) {
		c.insert(c.end(), first, last);
		make_heap(c.begin(), c.end(), comp);
	}

	priority_queue(priority_queue& rhs) : c(rhs.c), comp(rhs.comp) {
		make_heap(c.begin(), c.end(), comp);
	}

	~priority_queue() = default;

	priority_queue& operator=(const priority_queue& rhs) {
		c = rhs.c;
		comp = rhs.comp;
		make_heap(c.begin(), c.end(), comp);
		return *this;
	}

	priority_queue& operator=(const std::initializer_list<T>& ilist) {
		c = ilist;
		make_heap(c.begin(), c.end(), comp);
		return *this;
	}

	// 访问元素相关函数
	const_reference top() const {
		return c.front();
	}

	// 容量相关函数
	bool empty() const {
		return c.empty();
	}

	size_type size() const {
		return c.size();
	}

	void swap(priority_queue& rhs) {
		mystl::swap(c, rhs.c);
		mystl::swap(comp, rhs.comp);
	}

	// 修改容器相关函数
	void push(const value_type& value) {
		c.push_back(value);
		push_heap(c.begin(), c.end(), comp);
	}

	void pop() {
		pop_heap(c.begin(), c.end(), comp);
		c.pop_back();
	}

	void clear() {
		while (!empty())
			pop();
	}
};


}	// mystl


#endif