#ifndef MYSTL_UNINITIALIZED_H
#define MYSTL_UNINITIALIZED_H

#include <type_traits>
#include <string>
#include "mystl_construct.h"
#include "algobase.h"

namespace mystl {

	//-------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------------------
	// uninitialized_copy
	template<class ForwardIterator, class InputIterator>
	ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, std::true_type) {
		return mystl::copy(first, last, result);
	}

	template<class ForwardIterator, class InputIterator>
	ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, std::false_type) {
		InputIterator p = first;
		try {
			for (; first != last; first++, result++)
				mystl::construct(&*result, *first);
		}
		catch (...) {
			while (first != p) {
				mystl::destroy(&*first);
				first--;
			}
		}
		return result;
	}

	template<class ForwardIterator, class InputIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		typedef typename iterator_traits<ForwardIterator>::value_type type;
		return __uninitialized_copy(first, last, result, std::is_trivially_copy_assignable<type>());
	}

	inline char* uninitialized_copy(const char* first, const char* last, char* result) {
		std::memmove(result, first, last - first);
		return result + (last - first);
	}

	//-------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------------------
	// uninitialized_fill

	template<class ForwardIterator, class T>
	void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value, std::true_type) {
		return mystl::fill(first, last, value);
	}

	template<class ForwardIterator, class T>
	void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value, std::false_type) {
		ForwardIterator p = first;
		try {
			for (; first != last; first++)
				mystl::construct(&*first, value);
		}
		catch (...) {
			mystl::destroy(p, first);
		}
		return last;
	}

	template<class ForwardIterator, class T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
		typedef typename iterator_traits<ForwardIterator>::value_type type;
		return __uninitialized_fill(first, last, value, std::is_trivially_copy_assignable<T>());
	}

	//-------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------------------
	// uninitialized_fill_n

	template<class ForwardIterator, class Size, class T>
	ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& value, std::true_type) {
		return mystl::fill_n(first, n, value);
	}

	template<class ForwardIterator, class Size, class T>
	ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& value, std::false_type) {
		ForwardIterator p = first;
		try {
			while (n--) {
				mystl::construct(&*first, value);
				first++;
			}
		}
		catch (...) {
			mystl::destroy(p, first);
		}
		return first + n;
	}

	template<class ForwardIterator, class Size, class T>
	ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value) {
		typedef typename iterator_traits<ForwardIterator>::value_type type;
		return __uninitialized_fill_n(first, n, value, std::is_trivially_copy_assignable<type>());
	}

}	// mystl

#endif