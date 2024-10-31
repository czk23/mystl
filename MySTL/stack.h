#ifndef MTSTL_STACK_H
#define MTSTL_STACK_H

#include "deque.h"

namespace mystl {

// stack
template<class T, class Sequence = deque<T>>
class stack {
	// typedef
	typedef Sequence								container_type;
	typedef typename Sequence::value_type			value_type;
	typedef typename Sequence::size_type			size_type;
	typedef typename Sequence::reference			reference;
	typedef typename Sequence::const_reference		const_reference;

private:
	// ά��һ���ײ�����
	container_type c;

public:
	// ���졢�ƶ�����������
	stack() = default;

	explicit stack(size_type n) : c(n) {
		
	}

	stack(size_type n, const value_type& value) : c(n, value) {
		
	}

	stack(const std::initializer_list<T>& ilist) : c(ilist) {
		
	}

	template<class InputIterator>
	stack(InputIterator first, InputIterator last) : c(first, last) {
		
	}

	stack(container_type& container) : c(container) {
		
	}

	stack(const stack& rhs) : c(rhs.c) {
		
	}

	stack& operator=(const stack& rhs) {
		c = rhs.c;
		return *this;
	}

	stack& operator=(const std::initializer_list<T>& ilist) {
		c = ilist;
		return *this;
	}

	~stack() = default;

	// ����Ԫ����غ���
	reference top() {
		return c.back();
	}

	const_reference top() const {
		return c.back();
	}

	// ������غ���
	bool empty() const {
		return c.empty();
	}

	size_type size() const {
		return c.size();
	}

	// �޸�������غ���
	void push(const value_type& value) {
		c.push_back(value);
	}

	void pop() {
		c.pop_back();
	}

	void clear() {
		while (!empty())
			pop();
	}

	void swap(stack rhs) {
		c.swap(rhs.c);
	}

	friend bool operator==(const stack& lhs, const stack& rhs) {
		return lhs.c == rhs.c;
	}

	friend bool operator<(const stack& lhs, const stack& rhs) {
		return lhs.c < rhs.c;
	}
};

// ���ú���
template<class T, class Sequence>
bool operator==(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
	return lhs == rhs;
}

template<class T, class Sequence>
bool operator<(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
	return lhs < rhs;
}

template<class T, class Sequence>
bool operator!=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
	return !(lhs == rhs);
}

template<class T, class Sequence>
bool operator>(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
	return rhs < lhs;
}

template<class T, class Sequence>
bool operator<=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
	return !(rhs < lhs);
}

template<class T, class Sequence>
bool operator>=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
	return !(lhs < rhs);
}

template<class T, class Sequence>
void swap(stack<T, Sequence> lhs, stack<T, Sequence> rhs) {
	return lhs.swap(rhs);
}

}	// mystl

#endif