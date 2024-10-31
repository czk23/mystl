#ifndef MYSTL_CONSTRUCT_H
#define MYSTL_CONSTRUCT_H

#include <type_traits>
#include "iterator.h"

namespace mystl {

	template<class T1, class T2>
	inline void construct(T1* p, const T2& value) {
		::new (static_cast<void*>(p)) T1(value);
	}

	template<class T1>
	inline void construct(T1* p) {
		::new (static_cast<void*>(p)) T1();
	}

	template<class T1>
	inline void destroy(T1* p) {
		p->~T1();
	}

	template<class ForwardIterator>
	inline void __destroy(ForwardIterator first, ForwardIterator last, std::true_type) {}

	template<class ForwardIterator>
	inline void __destroy(ForwardIterator first, ForwardIterator last, std::false_type) {
		for (; first != last; first++)
			destroy(&*first);
	}

	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
	typedef typename iterator_traits<ForwardIterator>::value_type type;
		__destroy(first, last, std::is_trivially_destructible<type>());
	}

	void destroy(char*, char*) {};
	void destroy(int*, int*) {};
	void destroy(double*, double*) {};
	void destroy(long*, long*) {};
	void destroy(float*, float*) {};

} // mystl


#endif