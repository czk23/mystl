#ifndef MYSTL_ITERATOR_BASE_H
#define MYSTL_ITERATOR_BASE_H

#include <cstdlib>

namespace mystl {

	// ����������������
	struct output_iterator_tag {};
	struct input_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_iterator_tag : public bidirectional_iterator_tag {};

	// iteratorģ�������
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

	// ԭ��ָ��ƫ�ػ�
	template<class T>
	struct iterator_traits<T*> {
		typedef random_iterator_tag		iterator_category;
		typedef T						value_type;
		typedef ptrdiff_t				difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	// constԭ��ָ��ƫ�ػ�
	template<class T>
	struct iterator_traits<const T*> {
		typedef random_iterator_tag		iterator_category;
		typedef T						value_type;
		typedef ptrdiff_t				difference_type;
		typedef const T* pointer;
		typedef const T& reference;
	};

	// ����iterator������
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
	iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	// ����iterator��difference_type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
	distance_type(const Iterator&) {
		typedef typename iterator_traits<Iterator>::difference_type difference_type;
		return static_cast<difference_type*>(0);
	}

	// ����iterator��value_type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&) {
		typedef typename iterator_traits<Iterator>::value_type value_type;
		return static_cast<value_type*>(0);
	}

	// input_iterator�汾��distance����
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	__distance(InputIterator first, InputIterator last, input_iterator_tag) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first)
			++n;
		return n;
	}

	// random_iterator�汾��distance����
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	__distance(InputIterator first, InputIterator last, random_iterator_tag) {
		return last - first;
	}

	// ��������������֮��ľ���
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	distance(InputIterator first, InputIterator last) {
		return __distance(first, last, iterator_category(first));
	}

	// �õ������ƶ�n������
	template<class InputIterator, class Distance>
	inline void advance(InputIterator& first, Distance n) {
		__advance(first, n, iterator_category(first));
	}

	// input_iterator�汾��advance����
	template<class InputIterator, class Distance>
	inline void __advance(InputIterator& first, Distance n, input_iterator_tag) {
		while (n--) {
			++first;
		}
	}

	// bidirectional_iterator�汾��advance����
	template<class InputIterator, class Distance>
	inline void __advance(InputIterator& first, Distance n, bidirectional_iterator_tag) {
		if (n > 0)
			while (n--)
				++first;
		if (n < 0)
			while (n++)
				--first;
	}

	// random_iterator�汾��advance����
	template<class InputIterator, class Distance>
	inline void __advance(InputIterator& first, Distance n, random_iterator_tag) {
		first += n;
	}

}	// mystl

#endif