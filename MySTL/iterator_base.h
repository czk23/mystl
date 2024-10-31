#ifndef MYSTL_ITERATOR_BASE_H
#define MYSTL_ITERATOR_BASE_H

#include <cstdlib>

namespace mystl {

	// 迭代器的五种类型
	struct output_iterator_tag {};
	struct input_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_iterator_tag : public bidirectional_iterator_tag {};

	// iterator模板基础类
	template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct iterator {
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	// iterator_traits
	template<class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};

	// 原生指针偏特化
	template<class T>
	struct iterator_traits<T*> {
		typedef random_iterator_tag		iterator_category;
		typedef T						value_type;
		typedef ptrdiff_t				difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	// const原生指针偏特化
	template<class T>
	struct iterator_traits<const T*> {
		typedef random_iterator_tag		iterator_category;
		typedef T						value_type;
		typedef ptrdiff_t				difference_type;
		typedef const T* pointer;
		typedef const T& reference;
	};

	// 返回iterator的类型
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
	iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	// 返回iterator的difference_type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
	distance_type(const Iterator&) {
		typedef typename iterator_traits<Iterator>::difference_type difference_type;
		return static_cast<difference_type*>(0);
	}

	// 返回iterator的value_type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&) {
		typedef typename iterator_traits<Iterator>::value_type value_type;
		return static_cast<value_type*>(0);
	}

	// input_iterator版本的distance函数
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	__distance(InputIterator first, InputIterator last, input_iterator_tag) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first)
			++n;
		return n;
	}

	// random_iterator版本的distance函数
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	__distance(InputIterator first, InputIterator last, random_iterator_tag) {
		return last - first;
	}

	// 计算两个迭代器之间的距离
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	distance(InputIterator first, InputIterator last) {
		return __distance(first, last, iterator_category(first));
	}

	// 让迭代器移动n个距离
	template<class InputIterator, class Distance>
	inline void advance(InputIterator& first, Distance n) {
		__advance(first, n, iterator_category(first));
	}

	// input_iterator版本的advance函数
	template<class InputIterator, class Distance>
	inline void __advance(InputIterator& first, Distance n, input_iterator_tag) {
		while (n--) {
			++first;
		}
	}

	// bidirectional_iterator版本的advance函数
	template<class InputIterator, class Distance>
	inline void __advance(InputIterator& first, Distance n, bidirectional_iterator_tag) {
		if (n > 0)
			while (n--)
				++first;
		if (n < 0)
			while (n++)
				--first;
	}

	// random_iterator版本的advance函数
	template<class InputIterator, class Distance>
	inline void __advance(InputIterator& first, Distance n, random_iterator_tag) {
		first += n;
	}

}	// mystl

#endif