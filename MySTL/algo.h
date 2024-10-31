#ifndef MYSTL_ALGO_H
#define MYSTL_ALGO_H

#include <type_traits>

#include "iterator_base.h"
#include "iterator.h"
#include "mystl_pair.h"
#include "functional.h"
#include "heap_algo.h"

// ����ļ�������mystl��һϵ���㷨

namespace mystl {

/*****************************************************************************************/
// all_of
// ���[first, last)���Ƿ�ȫ��Ԫ�ض�����һԪ���� unary_pred Ϊ true ������������򷵻� true
/*****************************************************************************************/
template<class InputIterator, class UnaryPredicate>
bool all_of(InputIterator first, InputIterator last, UnaryPredicate unary_pred) {
	for (; first != last; ++first)
		if (!unary_pred(*first))
			return false;
	return true;
}

/*****************************************************************************************/
// any_of
// ���[first, last)���Ƿ����ĳ��Ԫ������һԪ���� unary_pred Ϊ true ������������򷵻� true
/*****************************************************************************************/
template<class InputIterator, class UnaryPredicate>
bool any_of(InputIterator first, InputIterator last, UnaryPredicate unary_pred) {
	for (; first != last; ++first)
		if (unary_pred(*first))
			return true;
	return false;
}

/*****************************************************************************************/
// none_of
// ���[first, last)���Ƿ�ȫ��Ԫ�ض�������һԪ���� unary_pred Ϊ true ������������򷵻� true
/*****************************************************************************************/
template<class InputIterator, class UnaryPredicate>
bool none_of(InputIterator first, InputIterator last, UnaryPredicate unary_pred) {
	for (; first != last; ++first)
		if (unary_pred(*first))
			return false;
	return true;
}

/*****************************************************************************************/
// count
// ��[first, last)�����ڵ�Ԫ�������ֵ���бȽϣ�ȱʡʹ�� operator==������Ԫ����ȵĸ���
/*****************************************************************************************/
template<class InputIterator, class T>
size_t count(InputIterator first, InputIterator last, const T& value) {
	size_t n = 0;
	for (; first != last; ++first)
		if (*first == value)
			++n;
	return n;
}

/*****************************************************************************************/
// count_if
// ��[first, last)�����ڵ�ÿ��Ԫ�ض�����һԪ unary_pred ���������ؽ��Ϊ true �ĸ���
/*****************************************************************************************/
template<class InputIterator, class UnaryPredicate>
size_t count_if(InputIterator first, InputIterator last, UnaryPredicate unary_pred) {
	size_t n = 0;
	for (; first != last; ++first)
		if (unary_pred(*first))
			++n;
	return n;
}

/*****************************************************************************************/
// find
// ��[first, last)�������ҵ����� value ��Ԫ�أ�����ָ���Ԫ�صĵ�����
/*****************************************************************************************/
template<class InputIterator, class T>
InputIterator __find(InputIterator first, InputIterator last, const T& value, mystl::input_iterator_tag) {
	while (first != last && *first != value)
		++first;
	return first;
}

template<class RandomAccessIterator, class T>
RandomAccessIterator __find(RandomAccessIterator first, RandomAccessIterator last, const T& value, mystl::random_iterator_tag) {
	// typename iterator_traits<RandomAccessIterator>::difference_type n = (last - first) >> 2;
	auto n = (last - first) >> 2;
	for (; n > 0; --n) {
		if (*first == value)
			return first;
		++first;
		if (*first == value)
			return first;
		++first;
		if (*first == value)
			return first;
		++first;
		if (*first == value)
			return first;
		++first;
	}

	switch (last - first) {
	case 3:
		if (*first == value)
			return first;
		++first;
	case 2:
		if (*first == value)
			return first;
		++first;
	case 1:
		if (*first == value)
			return first;
		++first;
	case 0:
	default:
		return last;
	}

}

template<class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
	return mystl::__find(first, last, value, mystl::iterator_category(first));
}

/*****************************************************************************************/
// find_if
// ��[first, last)�������ҵ���һ����һԪ���� unary_pred Ϊ true ��Ԫ�ز�����ָ���Ԫ�صĵ�����
/*****************************************************************************************/
template<class InputIterator, class UnaryPredicate>
InputIterator __find_if(InputIterator first, InputIterator last, UnaryPredicate unary_pred, input_iterator_tag) {
	while (first != last && !unary_pred(*first))
		++first;
	return first;
}

template<class RandomAccessIterator, class UnaryPredicate>
RandomAccessIterator __find_if(RandomAccessIterator first, RandomAccessIterator last, UnaryPredicate unary_pred, random_iterator_tag) {
	auto n = (last - first) >> 2;
	for (; n > 0; --n) {
		if (unary_pred(*first))
			return first;
		++first;
		if (unary_pred(*first))
			return first;
		++first;
		if (unary_pred(*first))
			return first;
		++first;
		if (unary_pred(*first))
			return first;
		++first;
	}
	switch (last - first) {
	case 3:
		if (unary_pred(*first))
			return first;
		++first;
	case 2:
		if (unary_pred(*first))
			return first;
		++first;
	case 1:
		if (unary_pred(*first))
			return first;
		++first;
	case 0:
	default:
		return last;
	}
}

template<class InputIterator, class UnaryPredicate>
InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate unary_pred) {
	return __find_if(first, last, unary_pred, mystl::iterator_category(first));
}

/*****************************************************************************************/
// find_if_not
// ��[first, last)�������ҵ���һ����һԪ���� unary_pred Ϊ false ��Ԫ�ز�����ָ���Ԫ�صĵ�����
/*****************************************************************************************/
template<class InputIterator, class UnaryPredicate>
InputIterator __find_if_not(InputIterator first, InputIterator last, UnaryPredicate unary_pred, input_iterator_tag) {
	while (first != last && unary_pred(*first))
		++first;
	return first;
}

template<class RandomAccessIterator, class UnaryPredicate>
RandomAccessIterator __find_if_not(RandomAccessIterator first, RandomAccessIterator last, UnaryPredicate unary_pred, random_iterator_tag) {
	auto n = (last - first) >> 2;
	for (; n > 0; --n) {
		if (!unary_pred(*first))
			return first;
		++first;
		if (!unary_pred(*first))
			return first;
		++first;
		if (!unary_pred(*first))
			return first;
		++first;
		if (!unary_pred(*first))
			return first;
		++first;
	}
	switch (last - first) {
	case 3:
		if (!unary_pred(*first))
			return first;
		++first;
	case 2:
		if (!unary_pred(*first))
			return first;
		++first;
	case 1:
		if (!unary_pred(*first))
			return first;
		++first;
	case 0:
	default:
		return last;
	}
}

template<class InputIterator, class UnaryPredicate>
InputIterator find_if_not(InputIterator first, InputIterator last, UnaryPredicate unary_pred) {
	return __find_if_not(first, last, unary_pred, iterator_category(first));
}

/*****************************************************************************************/
// search
// ��[first1, last1)�в���[first2, last2)���״γ��ֵ�
/*****************************************************************************************/
template<class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
	if (first1 == last1 || first2 == last2)
		return first1;
	ForwardIterator2 tmp = first2;
	++tmp;
	if (tmp == last2)
		return find(first1, last1, *first2);
	ForwardIterator2 p1, p;
	p1 = first2;
	++p1;
	ForwardIterator1 current = first1;
	
	while (first1 != last1) {
		first1 = find(first1, last1, *first2);
		if (first1 == last1)
			return last1;

		p = p1;
		current = first1;
		if (++current == last1)
			return last1;

		while (*current == *p) {
			if (++p == last2)
				return first1;
			if (++current == last1)
				return last1;
		}

		++first1;
	}
	return first1;
}

template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, 
	ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate binary_pred) {
	if (first1 == last1 || first2 == last2)
		return first1;
	ForwardIterator2 tmp = first2;
	++tmp;
	if (tmp == last2) {
		while (first1 != last1 && !binary_pred(*first1, *first2))
			++first1;
		return first1;
	}

	ForwardIterator2 p, p1;
	p1 = first2;
	++p1;
	ForwardIterator1 current = first1;

	while (first1 != last1) {
		while (first1 != last1) {
			if (binary_pred(*first1, *first2))
				break;
			++first1;
		}
		if (first1 == last1)
			return last1;
		
		p = p1;
		current = first1;
		if (++current == last1)
			return last1;

		while (binary_pred(*current, *p)) {
			if (++p == last2)
				return first1;
			if (++current == last1)
				return last1;
		}
		++first1;
	}
	return first1;
}

/*****************************************************************************************/
// search_n
// ��[first, last)�в������� n �� value ���γɵ������У�����һ��������ָ��������е���ʼ��
/*****************************************************************************************/
template<class ForwardIterator, class Integer, class T>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Integer count, const T& value) {
	if (count <= 0)
		return first;
	first = find(first, last, value);
	while (first != last) {
		ForwardIterator i = first;
		++i;
		Integer n = count - 1;
		while (n != 0 && *i == value && i != last) {
			++i;
			--n;
		}
		if (0 == n)
			return first;
		first = find(first, last, value);
	}
	return last;
}

template<class ForwardIterator, class Integer, class T, class BinaryPredicate>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Integer count, const T& value, BinaryPredicate binary_pred) {
	if (count <= 0)
		return first;
	while (first != last && !binary_pred(*first, value)) {
		++first;
	}
	while (first != last) {
		ForwardIterator i = first;
		++i;
		Integer n = count - 1;
		while (i != last && n != 0 && binary_pred(*i, value)) {
			++i;
			--n;
		}
		if (0 == n)
			return first;
		while (i != last && !binary_pred(*i, value)) {
			++i;
		}
		first = i;
	}
	return last;
}

/*****************************************************************************************/
// find_end
// ��[first1, last1)�����в���[first2, last2)���һ�γ��ֵĵط����������ڷ��� last1
/*****************************************************************************************/
template<class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2,
	forward_iterator_tag, forward_iterator_tag) {
	if (first2 == last2)
		return last1;
	ForwardIterator1 result = last1;
	while (true) {
		ForwardIterator1 new_result = search(first1, last1, first2, last2);
		if (new_result == last1)
			return result;
		else {
			result = new_result;
			first1 = new_result;
			++first1;
		}
	}
	return last1;
}

template<class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
	BidirectionalIterator2 first2, BidirectionalIterator2 last2, bidirectional_iterator_tag, bidirectional_iterator_tag) {
	typedef reverse_iterator<BidirectionalIterator1>	reverse_iterator1;
	typedef reverse_iterator<BidirectionalIterator2>	reverse_iterator2;
	reverse_iterator1 rlast1(first1);
	reverse_iterator2 rlast2(first2);
	reverse_iterator1 rresult = search(reverse_iterator1(last1), rlast1, reverse_iterator2(last2), rlast2);
	if (rresult == rlast1)
		return last1;
	BidirectionalIterator1 result = rresult.base();
	advance(result, -distance(first2, last2));
	return result;
}

template<class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
	return __find_end(first1, last1, first2, last2, iterator_category(first1), iterator_category(first2));
}

template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2,
	forward_iterator_tag, forward_iterator_tag, BinaryPredicate binary_pred) {
	if (first2 == last2)
		return last1;
	ForwardIterator1 result = last1;
	while (true) {
		ForwardIterator1 new_result = search(first1, last1, first2, last2, binary_pred);
		if (new_result == last1)
			return result;
		else {
			result = new_result;
			first1 = new_result;
			++first1;
		}
	}
	return last1;
}

template<class BidirectionalIterator1, class BidirectionalIterator2, class BinaryPredicate>
BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
	BidirectionalIterator2 first2, BidirectionalIterator2 last2, 
	bidirectional_iterator_tag, bidirectional_iterator_tag, BinaryPredicate binary_pred) {
	typedef reverse_iterator<BidirectionalIterator1>	reverse_iterator1;
	typedef reverse_iterator<BidirectionalIterator2>	reverse_iterator2;
	reverse_iterator1 rlast1(first1);
	reverse_iterator2 rlast2(first2);
	reverse_iterator1 rresult = mystl::search(reverse_iterator1(last1), rlast1, reverse_iterator2(last2), rlast2, binary_pred);
	if (rresult == rlast1)
		return last1;
	BidirectionalIterator1 result = rresult.base();
	advance(result, -distance(first2, last2));
	return result;

}

template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
	ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate binary_pred) {
	return __find_end(first1, last1, first2, last2, iterator_category(first1), iterator_category(first2), binary_pred);
}

/*****************************************************************************************/
// find_first_of
// ��[first1, last1)�в���[first2, last2)�е�ĳЩԪ�أ�����ָ���һ�γ��ֵ�Ԫ�صĵ�����
/*****************************************************************************************/
template<class InputIterator, class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2) {
	for (; first1 != last1; ++first1) {
		for (ForwardIterator it = first2; it != last2; ++it) {
			if (*first1 == *it)
				return first1;
		}
	}
	return last1;
}

template<class InputIterator, class ForwardIterator, class BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1, 
	ForwardIterator first2, ForwardIterator last2, BinaryPredicate binary_pred) {
	for (; first1 != last1; ++first1) {
		for (ForwardIterator it = first2; it != last2; ++it) {
			if (binary_pred(*first1, *it))
				return first1;
		}
	}
	return last1;
}

/*****************************************************************************************/
// for_each
// ʹ��һ���������� f ��[first, last)�����ڵ�ÿ��Ԫ��ִ��һ�� operator() �����������ܸı�Ԫ������
// f() �ɷ���һ��ֵ������ֵ�ᱻ����
/*****************************************************************************************/
template<class InputIterator, class Function>
Function for_each(InputIterator first, InputIterator last, Function f) {
	for (; first != last; ++first)
		f(*first);
	return f;
}

/*****************************************************************************************/
// adjacent_find
// �ҳ���һ��ƥ�������Ԫ�أ�ȱʡʹ�� operator== �Ƚϣ�����ҵ�����һ����������ָ�����Ԫ�صĵ�һ��Ԫ��
/*****************************************************************************************/
template<class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
	if (first == last)
		return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (*first == *next) {
			return first;
		}
		first = next;
	}
	return last;
}

template<class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred) {
	if (first == last)
		return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (binary_pred(*first, *next)) {
			return first;
		}
		first = next;
	}
	return last;
}

/*****************************************************************************************/
// lower_bound
// ��[first, last)�в��ҵ�һ����С�� value ��Ԫ�أ�������ָ�����ĵ���������û���򷵻� last
/*****************************************************************************************/
template<class ForwardIterator, class T>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& value, forward_iterator_tag) {
	auto len = distance(first, last);
	ForwardIterator middle = first;
	auto half = len;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		advance(middle, half);
		if (*middle < value) {
			first = middle;
			++first;
			len = len - half - 1;
		}
		else
			len = half;
	}
	return first;
}

template<class RandomAccessIterator, class T>
RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, random_iterator_tag) {
	auto len = last - first;
	RandomAccessIterator middle = first;
	auto half = len;
	while (len > 0) {;
		half = len >> 1;
		middle = first + half;
		if (*middle < value) {
			first = middle + 1;
			len = len - half - 1;
		}
		else
			len = half;
	}
	return first;
}

template<class ForwardIterator, class T>
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value) {
	return __lower_bound(first, last, value, iterator_category(first));
}

template<class ForwardIterator, class T, class BinaryPredicate>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& value, 
	forward_iterator_tag, BinaryPredicate binary_pred) {
	auto len = distance(first, last);
	ForwardIterator middle = first;
	auto half = len;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		advance(middle, half);
		if (binary_pred(*middle, value)) {
			first = middle;
			++first;
			len = len - half - 1;
		}
		else
			len = half;
	}
	return first;
}

template<class RandomAccessIterator, class T, class BinaryPredicate>
RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, 
	random_iterator_tag, BinaryPredicate binary_pred) {
	auto len = last - first;
	RandomAccessIterator middle = first;
	auto half = len;
	while (len > 0) {
		;
		half = len >> 1;
		middle = first + half;
		if (binary_pred(*middle, value)) {
			first = middle + 1;
			len = len - half - 1;
		}
		else
			len = half;
	}
	return first;
}

template<class ForwardIterator, class T, class BinaryPredicate>
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value, BinaryPredicate binary_pred) {
	return __lower_bound(first, last, value, iterator_category(first), binary_pred);
}

/*****************************************************************************************/
// upper_bound
// ��[first, last)�в��ҵ�һ������value ��Ԫ�أ�������ָ�����ĵ���������û���򷵻� last
/*****************************************************************************************/
template<class ForwardIterator, class T>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T& value, forward_iterator_tag) {
	auto len = distance(first, last);
	ForwardIterator middle = first;
	auto half = len;
	while (len > 0) {
		middle = first;
		half = len >> 1;
		advance(middle, half);
		if (value < *middle)
			len = half;
		else {
			first = middle;
			++first;
			len = len - half - 1;
		}
	}
	return first;
}

template<class RandomAccessIterator, class T>
RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value,
	random_iterator_tag, random_iterator_tag) {
	auto len = last - first;
	RandomAccessIterator middle = first;
	auto half = len;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (value < *middle)
			len = half;
		else {
			len = len - half - 1;
			first = middle + 1;
		}
	}
	return first;
}

template<class ForwardIterator, class T>
ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value) {
	return __upper_bound(first, last, value, iterator_category(first));
}

template<class ForwardIterator, class T, class BinaryPredicate>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T& value, 
	forward_iterator_tag, BinaryPredicate binary_pred) {
	auto len = distance(first, last);
	ForwardIterator middle = first;
	auto half = len;
	while (len > 0) {
		middle = first;
		half = len >> 1;
		advance(middle, half);
		if (binary_pred(value, *middle))
			len = half;
		else {
			first = middle;
			++first;
			len = len - half - 1;
		}
	}
	return first;
}

template<class RandomAccessIterator, class T, class BinaryPredicate>
RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value,
	random_iterator_tag, BinaryPredicate binary_pred) {
	auto len = last - first;
	RandomAccessIterator middle = first;
	auto half = len;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (binary_pred(value, *middle))
			len = half;
		else {
			len = len - half - 1;
			first = middle + 1;
		}
	}
	return first;
}

template<class ForwardIterator, class T, class BinaryPredicate>
ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value, BinaryPredicate binary_pred) {
	return __upper_bound(first, last, value, iterator_category(first), binary_pred);
}

/*****************************************************************************************/
// binary_search
// ���ֲ��ң������[first, last)���е�ͬ�� value ��Ԫ�أ����� true�����򷵻� false
/*****************************************************************************************/
template<class ForwardIterator, class T>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value) {
	ForwardIterator i = lower_bound(first, last, value);
	return i != last && *i == value;
}

template<class ForwardIterator, class T, class BinaryPredicate>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value, BinaryPredicate binary_pred) {
	ForwardIterator i = lower_bound(first, last, value, binary_pred);
	return i != last && *i == value;
}

/*****************************************************************************************/
// equal_range
// ����[first,last)�������� value ��ȵ�Ԫ�����γɵ����䣬����һ�Ե�����ָ��������β
// ��һ��������ָ���һ����С�� value ��Ԫ�أ��ڶ���������ָ���һ������ value ��Ԫ��
/*****************************************************************************************/
template<class ForwardIterator, class T>
pair<ForwardIterator, ForwardIterator>
__equal_range(ForwardIterator first, ForwardIterator last, const T& value, forward_iterator_tag) {
	auto len = distance(first, last);
	ForwardIterator middle = first;
	auto half = len;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		advance(middle, half);
		if (*middle < value) {
			first = middle;
			++middle;
			len = len - half - 1;
		}
		else if (value < *middle)
			len = half;
		else {
			ForwardIterator left = lower_bound(first, middle, value);
			advance(first, len);
			ForwardIterator right = upper_bound(++middle, first, value);
			return pair<ForwardIterator, ForwardIterator>(left, right);
		}
	}
	return pair<ForwardIterator, ForwardIterator>(last, last);
}

template<class RandomAccessIterator, class T>
pair<RandomAccessIterator, RandomAccessIterator>
__equal_range(RandomAccessIterator first, RandomAccessIterator last, const T& value, random_iterator_tag) {
	auto len = last - first;
	RandomAccessIterator middle = first;
	auto half = len;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (*middle < value) {
			len = len - half - 1;
			first = middle + 1;
		}
		else if (value < *middle)
			len = half;
		else {
			RandomAccessIterator left = lower_bound(first, middle, value);
			first += len;
			RandomAccessIterator right = upper_bound(++middle, first, value);
			return pair<RandomAccessIterator, RandomAccessIterator>(left, right);
		}
	}
	return pair<RandomAccessIterator, RandomAccessIterator>(last, last);
}

template<class ForwardIterator, class T>
pair<ForwardIterator, ForwardIterator>
equal_range(ForwardIterator first, ForwardIterator last, const T& value) {
	return __equal_range(first, last, value, iterator_category(first));
}

template<class ForwardIterator, class T, class BinaryPredicate>
pair<ForwardIterator, ForwardIterator>
__equal_range(ForwardIterator first, ForwardIterator last, const T& value, forward_iterator_tag, BinaryPredicate binary_pred) {
	auto len = distance(first, last);
	ForwardIterator middle = first;
	auto half = len;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		advance(middle, half);
		if (binary_pred(*middle, value)) {
			first = middle;
			++middle;
			len = len - half - 1;
		}
		else if (binary_pred(value, *middle))
			len = half;
		else {
			ForwardIterator left = lower_bound(first, middle, value, binary_pred);
			advance(first, len);
			ForwardIterator right = upper_bound(++middle, first, value, binary_pred);
			return pair<ForwardIterator, ForwardIterator>(left, right);
		}
	}
	return pair<ForwardIterator, ForwardIterator>(last, last);
}

template<class RandomAccessIterator, class T, class BinaryPredicate>
pair<RandomAccessIterator, RandomAccessIterator>
__equal_range(RandomAccessIterator first, RandomAccessIterator last, const T& value, 
	random_iterator_tag, BinaryPredicate binary_pred) {
	auto len = last - first;
	RandomAccessIterator middle = first;
	auto half = len;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (binary_pred(*middle, value)) {
			len = len - half - 1;
			first = middle + 1;
		}
		else if (binary_pred(value, *middle))
			len = half;
		else {
			RandomAccessIterator left = mystl::lower_bound(first, middle, value, binary_pred);
			first += len;
			RandomAccessIterator right = mystl::upper_bound(++middle, first, value, binary_pred);
			return pair<RandomAccessIterator, RandomAccessIterator>(left, right);
		}
	}
	return pair<RandomAccessIterator, RandomAccessIterator>(last, last);
}

template<class ForwardIterator, class T, class BinaryPredicate>
pair<ForwardIterator, ForwardIterator>
equal_range(ForwardIterator first, ForwardIterator last, const T& value, BinaryPredicate binary_pred) {
	return __equal_range(first, last, value, iterator_category(first), binary_pred);
}

/*****************************************************************************************/
// generate
// ���������� gen ����������[first, last)�ڵ�ÿ��Ԫ�ظ�ֵ
/*****************************************************************************************/
template<class ForwardIterator, class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
	for (; first != last; ++first)
		*first = gen();
}

/*****************************************************************************************/
// generate_n
// �ú������� gen ������ n ��Ԫ�ظ�ֵ
/*****************************************************************************************/
template<class ForwardIterator, class Size, class Generator>
void generate_n(ForwardIterator first, Size n, Generator gen) {
	for (; n > 0; --n, ++first)
		*first = gen();
}

/*****************************************************************************************/
// includes
// �ж�����һS1 �Ƿ�������ж�S2
/*****************************************************************************************/
template<class InputIterator1, class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1)
			return false;
		else if (*first1 < *first2)
			++first1;
		else {
			++first1;
			++first2;
		}
	}
	return first2 == last2;
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
bool includes(InputIterator1 first1, InputIterator1 last1, 
	InputIterator2 first2, InputIterator2 last2, BinaryPredicate binary_pred) {
	while (first1 != last1 && first2 != last2) {
		if (binary_pred(*first2, *first1))
			return false;
		else if (binary_pred(*first1, *first2))
			++first1;
		else {
			++first1;
			++first2;
		}
	}
	return first2 == last2;
}

/*****************************************************************************************/
// is_heap
// ���[first, last)�ڵ�Ԫ���Ƿ�Ϊһ���ѣ�����ǣ��򷵻� true
/*****************************************************************************************/
template<class RandomAccessIterator>
bool is_heap(RandomAccessIterator first, RandomAccessIterator last) {
	auto n = distance(first, last);
	auto parent = 0;
	for (auto child = 1; child < n; ++child) {
		if (first[parent] < first[child])
			return false;
		if ((child & 1) == 0)
			++parent;
	}
	return true;
}

template<class RandomAccessIterator, class BinaryPredicate>
bool is_heap(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate binary_pred) {
	auto n = distance(first, last);
	auto parent = 0;
	for (auto child = 1; child < n; ++child) {
		if (binary_pred(first[parent], first[child]))
			return false;
		if ((child & 1) == 0)
			++parent;
	}
	return true;
}

/*****************************************************************************************/
// is_sorted
// ���[first, last)�ڵ�Ԫ���Ƿ���������������򷵻� true
/*****************************************************************************************/
template<class ForwardIterator>
bool is_sorted(ForwardIterator first, ForwardIterator last) {
	if (first == last)
		return true;
	ForwardIterator next = first;
	for (++next; next != last; ++next, ++first)
		if (*next < *first)
			return false;
	return true;
}

template<class ForwardIterator, class BinaryPredicate>
bool is_sorted(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred) {
	if (first == last)
		return true;
	ForwardIterator next = first;
	for (++next; next != last; ++next, ++first)
		if (binary_pred(*next, *first))
			return false;
	return true;
}

/*****************************************************************************************/
// median
// �ҳ�����ֵ���м�ֵ
/*****************************************************************************************/
template<class T>
const T& median(const T& left, const T& mid, const T& right) {
	if (left < mid) {
		if (mid < right)
			return mid;							// left < mid < right
		else if (left < right)
			return right;						// left < right <= mid
		else
			return left;						// right <= left < mid
	}
	else {
		if (left < right)
			return left;						// mid <= left < right;
		else if (mid < right)
			return right;						// mid < right <= left
		else
			return mid;							// right <= mid <= left
	}
}

template<class T, class BinaryPredicate>
const T& median(const T& left, const T& mid, const T& right, BinaryPredicate binary_pred) {
	if (binary_pred(left, mid)) {
		if (binary_pred(mid, right))
			return mid;							// left < mid < right
		else if (binary_pred(left, right))
			return right;						// left < right <= mid
		else
			return left;						// right <= left < mid
	}
	else {
		if (binary_pred(left, right))
			return left;						// mid <= left < right;
		else if (binary_pred(mid, right))
			return right;						// mid < right <= left
		else
			return mid;							// right <= mid <= left
	}
}

/*****************************************************************************************/
// max_element
// ����һ����������ָ������������Ԫ��
/*****************************************************************************************/
template<class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
	if (first == last)
		return first;
	ForwardIterator result = first;
	for (++first; first != last; ++first) {
		if (*result < *first)
			result = first;
	}
	return result;
}

template<class ForwardIterator, class BinaryPredicate>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred) {
	if (first == last)
		return first;
	ForwardIterator result = first;
	for (++first; first != last; ++first) {
		if (binary_pred(*result, *first))
			result = first;
	}
	return result;
}

/*****************************************************************************************/
// min_element
// ����һ����������ָ����������С��Ԫ��
/*****************************************************************************************/
template<class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
	if (first == last)
		return first;
	ForwardIterator result = first;
	for (++first; first != last; ++first)
		if (*first < *result)
			result = first;
	return result;
}

template<class ForwardIterator, class BinaryPredicate>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred) {
	if (first == last)
		return first;
	ForwardIterator result = first;
	for (++first; first != last; ++first)
		if (binary_pred(*first, *result))
			result = first;
	return result;
}

/*****************************************************************************************/
// swap_ranges
// ��[first1, last1)�� first2 ��ʼ��������ͬ����Ԫ��
// ���������䳤�ȱ�����ͬ���������в��ܻ����ص�������һ��������ָ�����ж����һ��������Ԫ�ص���һλ��
/*****************************************************************************************/
template<class ForwardIterator1, class ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2) {
		iter_swap(first1, first2);
	}
	return first2;
}

/*****************************************************************************************/
// transform
// ��һ���汾�Ժ������� unary_op ������[first, last)�е�ÿ��Ԫ�ز������������ result ��
// �ڶ����汾�Ժ������� binary_op ��������������[first1, last1)��[first2, last2)����ͬλ��
/*****************************************************************************************/
template<class InputIterator, class OutputIterator, class UnaryOperation>
OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation unary_op) {
	for (; first != last; ++first, ++result)
		*result = unary_op(*first);
	return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class BinaryPredicate>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
	OutputIterator result, BinaryPredicate binary_pred) {
	for (; first1 != last1; ++first1, ++first2, ++result)
		*result = binary_pred(*first1, *first2);
	return result;
}

/*****************************************************************************************/
// remove_copy
// �Ƴ���������ָ�� value ��ȵ�Ԫ�أ�����������Ƶ��� result ��ʾ��ʼλ�õ�������
/*****************************************************************************************/
template<class InputIterator, class OutputIterator, class T>
OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& value) {
	for (; first != last; ++first)
		if (*first != value)
			*result++ = *first;
	return result;
}

/*****************************************************************************************/
// remove
// �Ƴ�������ָ�� value ��ȵ�Ԫ��
// ������������ɾ����ЩԪ�أ����� remove �� remove_if �������� array
/*****************************************************************************************/
template<class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value) {
	first = mystl::find(first, last, value);
	ForwardIterator next = first;
	return first == last ? last : mystl::remove_copy(++next, last, first, value);
}

/*****************************************************************************************/
// remove_copy_if
// �Ƴ�������������һԪ���� unary_pred Ϊ true ��Ԫ�أ�����������Ƶ��� result Ϊ��ʼλ�õ�������
/*****************************************************************************************/
template<class InputIterator, class OutputIterator, class UnaryPredicate>
OutputIterator remove_copy_if(InputIterator first, InputIterator last, 
	OutputIterator result, UnaryPredicate unary_pred) {
	for (; first != last; ++first)
		if (!unary_pred(*first))
			*result++ = *first;
	return result;
}

/*****************************************************************************************/
// remove_if
// �Ƴ�������������һԪ���� unary_pred Ϊ true ��Ԫ��
/*****************************************************************************************/
template<class ForwardIterator, class UnaryPredicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, const UnaryPredicate unary_pred) {
	first = mystl::find_if(first, last, unary_pred);
	ForwardIterator next = first;
	return first == last ? first : mystl::remove_copy_if(++next, last, first, unary_pred);
}

/*****************************************************************************************/
// replace
// �����������е� old_value ���� new_value ���
/*****************************************************************************************/
template<class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value) {
	for (; first != last; ++first)
		if (*first == old_value)
			*first = new_value;
}

/*****************************************************************************************/
// replace_copy
// ��Ϊ�� replace ���ƣ���ͬ���ǽ�������Ƶ� result ��ָ�������У�ԭ����û�иı�
/*****************************************************************************************/
template<class InputIterator, class OutputIterator, class T>
OutputIterator replace_copy(InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value) {
	for (; first != last; ++first, ++result) {
		*result = *first == old_value ? new_value : *first;
	}
	return result;
}

/*****************************************************************************************/
// replace_if
// ��������������һԪ���� unary_pred Ϊ true ��Ԫ�ض��� new_value ���
/*****************************************************************************************/
template<class ForwardIterator, class UnaryPredicate, class T>
void replace_if(ForwardIterator first, ForwardIterator last, UnaryPredicate unary_pred, const T& new_value) {
	for (; first != last; ++first) {
		if (unary_pred(*first))
			*first = new_value;
	}
}

/*****************************************************************************************/
// replace_copy_if
// ��Ϊ�� replace_if ���ƣ���ͬ���ǽ�������Ƶ� result ��ָ�������У�ԭ����û�иı�
/*****************************************************************************************/
template<class InputIterator, class OutputIterator, class UnaryPredicate, class T>
OutputIterator replace_copy_if(InputIterator first, InputIterator last, OutputIterator result, 
	UnaryPredicate unary_pred, const T& new_value) {
	for (; first != last; ++first, ++result)
		*result = unary_pred(*first) ? new_value : *first;
	return result;
}

/*****************************************************************************************/
// reverse
// ��[first, last)�����ڵ�Ԫ�ط�ת
/*****************************************************************************************/
template<class BidirectionalIterator>
void __reverse(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag) {
	while (true) {
		if (first == last || first == --last)
			return;
		iter_swap(first, last);
		++first;
	}
}

template<class RandomAccessIterator>
void __reverse(RandomAccessIterator first, RandomAccessIterator last, random_iterator_tag) {
	while (first < last) {
		iter_swap(first++, --last);
	}
}

template<class BidirectionalIterator>
void reverse(BidirectionalIterator first, BidirectionalIterator last) {
	return __reverse(first, last, iterator_category(first));
}

/*****************************************************************************************/
// reverse_copy
// ��Ϊ�� reverse ���ƣ���ͬ���ǽ�������Ƶ� result ��ָ������
/*****************************************************************************************/
template<class BidirectionalIterator, class OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result) {
	while (first != last) {
		--last;
		*result = *last;
		++result;
	}
	return result;
}

/*****************************************************************************************/
// random_shuffle
// ��[first, last)�ڵ�Ԫ�ش����������
// ���ذ汾ʹ��һ������������ĺ������� rand
/*****************************************************************************************/
template<class RandomAccessIterator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last) {
	if (first == last)
		return;
	srand(static_cast<unsigned>(time(0)));
	for (RandomAccessIterator i = first + 1; i < last; ++i) {
		iter_swap(i, first + (rand() % (i - first + 1)));
	}
}

template<class RandomAccessIterator, class RandomNumberGenerator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last, RandomNumberGenerator rand) {
	if (first == last)
		return;
	auto len = distance(first, last);
	for (RandomAccessIterator i = first + 1; i < last; ++i) {
		iter_swap(i, first + (rand(i - first + 1) % len));
	}
}

/*****************************************************************************************/
// rotate
// ��[first, middle)�ڵ�Ԫ�غ� [middle, last)�ڵ�Ԫ�ػ��������Խ����������Ȳ�ͬ������
// ���ؽ����� middle ��λ��
/*****************************************************************************************/
template<class ForwardIterator>
ForwardIterator __rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, forward_iterator_tag) {
	ForwardIterator first2 = middle;
	do {
		swap(*first++, *first2++);
		if (first == middle)
			middle = first2;
	} while (first2 != last);
	ForwardIterator new_middle = first2;
	first2 = middle;
	while (first2 != last) {
		swap(*first++, *first2++);
		if (first == middle)
			middle = first2;
		if (first2 == last) 
			first2 = middle;
	}
	return new_middle;
}

template<class BidirectionalIterator>
BidirectionalIterator __rotate(BidirectionalIterator first, BidirectionalIterator middle,
	BidirectionalIterator last, bidirectional_iterator_tag) {
	__reverse(first, middle, bidirectional_iterator_tag());
	__reverse(middle, last, bidirectional_iterator_tag());
	while (first != middle && middle != last) {
		swap(*first++, *--last);
	}
	if (first == middle) {
		__reverse(middle, last, bidirectional_iterator_tag());
		return last;
	}
	else {
		__reverse(first, middle, bidirectional_iterator_tag());
		return first;
	}
}

template<class EuclideanRingElement>
EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n) {
	while (n != 0) {
		EuclideanRingElement t = m % n;
		m = n;
		n = t;
	}
	return m;
}

template<class RandomAccessIterator>
RandomAccessIterator __rotate(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last,
	random_iterator_tag) {
	auto n = last - first;
	auto l = middle - first;
	auto r = n - l;
	RandomAccessIterator result = first + (last - middle);
	if (l == r) {
		swap_ranges(first, middle, middle);
		return result;
	}
	auto cycle_times = __gcd(l, n);
	for (auto i = 0; i < cycle_times; ++i) {
		auto value = *first;
		RandomAccessIterator p = first;
		if (l < r) {
			for (auto j = 0; j < r / cycle_times; ++j) {
				if (p > first + r) {
					*p = *(p - r);
					p -= r;
				}
				*p = *(p + l);
				p += l;
			}
		}
		else {
			for (auto j = 0; j < l / cycle_times - 1; ++j) {
				if (p < last - l) {
					*p = *(p + l);
					p += l;
				}
				*p = *(p - r);
				p -= r;
			}
		}
		*p = value;
		++first;
	}
	return result;
}

template<class ForwardIterator>
ForwardIterator rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
	if (first == middle)
		return last;
	if (last == middle)
		return first;
	return __rotate(first, middle, last, iterator_category(first));
}

/*****************************************************************************************/
// rotate_copy
// ��Ϊ�� rotate ���ƣ���ͬ���ǽ�������Ƶ� result ��ָ��������
/*****************************************************************************************/
template<class ForwardIterator, class OutputIterator>
OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator result) {
	return copy(first, middle, copy(middle, last, result));
}

/*****************************************************************************************/
// is_permutation
// �ж�[first1,last1)�Ƿ�Ϊ[first2, last2)���������
/*****************************************************************************************/
template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
bool __is_permutation(ForwardIterator1 first1, ForwardIterator1 last1,
	ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate binary_pred) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		if (!binary_pred(*first1, *first2))
			break;
	auto len1 = distance(first1, last1);
	auto len2 = distance(first2, last2);
	if (len1 != len2)
		return false;
	if (0 == len1 && 0 == len2)
		return true;
	for (ForwardIterator1 i = first1; i != last1; ++i) {
		bool is_repeated = false;
		for (ForwardIterator1 j = first1; j != i; ++j) {
			if (binary_pred(*i, *j)) {
				is_repeated = true;
				break;
			}
		}
		if (!is_repeated) {
			auto c2 = 0;
			for (ForwardIterator2 j = first2; j != last2; ++j) {
				if (binary_pred(*j, *i))
					++c2;
			}
			if (0 == c2)
				return false;
			auto c1 = 1;
			ForwardIterator1 j = i;
			for (++j; j != last1; ++j) {
				if (binary_pred(*i, *j))
					++c1;
			}
			if (c2 != c1)
				return false;
		}
	}
	return true;
}

template<class ForwardIterator1, class ForwardIterator2>
bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
	typedef typename iterator_traits<ForwardIterator1>::value_type v1;
	typedef typename iterator_traits<ForwardIterator2>::value_type v2;
	static_assert(std::is_same<v1, v2>::value, "the type should be same in mystl::is_permutation");
	return __is_permutation(first1, last1, first2, last2, equal_to<v1>());
}

template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1,
	ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate binary_pred) {
	typedef typename iterator_traits<ForwardIterator1>::value_type v1;
	typedef typename iterator_traits<ForwardIterator2>::value_type v2;
	static_assert(std::is_same<v1, v2>::value, "the type should be same in mystl::is_permutation");
	return __is_permutation(first1, last1, first2, last2, binary_pred);
}

/*****************************************************************************************/
// next_permutation
// ȡ��[first, last)����ʾ���е���һ��������ϣ����û����һ��������ϣ����� false�����򷵻� true
/*****************************************************************************************/
template<class BidirectionalIterator>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last) {
	if (first == last)
		return false;
	BidirectionalIterator tmp = first;
	++tmp;
	if (tmp == last)
		return false;
	BidirectionalIterator i = last;
	--i;
	while (true) {
		BidirectionalIterator ii = i;
		--i;
		if (*i < *ii) {
			BidirectionalIterator j = last;
			while (!(*i < *--j)) {}
			iter_swap(i, j);
			reverse(ii, last);
			return true;
		}
		if (i == first) {
			reverse(first, last);
			return false;
		}
	}
}

template<class BidirectionalIterator, class BinaryPredicate>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate binary_pred) {
	if (first == last)
		return false;
	BidirectionalIterator tmp = first;
	++tmp;
	if (tmp == last)
		return false;
	BidirectionalIterator i = last;
	--i;
	while (true) {
		BidirectionalIterator ii = i;
		--i;
		if (binary_pred(*i, *ii)) {
			BidirectionalIterator j = last;
			while (!binary_pred(*i, *--j)) {}
			iter_swap(i, j);
			reverse(ii, last);
			return true;
		}
		if (i == first) {
			reverse(first, last);
			return false;
		}
	}
}

/*****************************************************************************************/
// prev_permutation
// ȡ��[first, last)����ʾ���е���һ��������ϣ����û����һ��������ϣ����� false�����򷵻� true
/*****************************************************************************************/
template<class BidirectionalIterator>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last) {
	if (first == last)
		return false;
	BidirectionalIterator tmp = first;
	++tmp;
	if (tmp == last)
		return false;
	BidirectionalIterator i = last;
	--i;
	while (true) {
		BidirectionalIterator ii = i;
		--i;
		if (*ii < *i) {
			BidirectionalIterator j = last;
			while (!(*--j < *i)) {}
			iter_swap(i, j);
			reverse(ii, last);
			return true;
		}
		if (i == first) {
			reverse(first, last);
			return false;
		}
	}
}

template<class BidirectionalIterator, class BinaryPredicate>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate binary_pred) {
	if (first == last)
		return false;
	BidirectionalIterator tmp = first;
	++tmp;
	if (tmp == last)
		return false;
	BidirectionalIterator i = last;
	--i;
	while (true) {
		BidirectionalIterator ii = i;
		--i;
		if (binary_pred(*ii, *i)) {
			BidirectionalIterator j = last;
			while (!binary_pred(*--j, *i)) {}
			iter_swap(i, j);
			reverse(ii, last);
			return true;
		}
		if (i == first) {
			reverse(first, last);
			return false;
		}
	}
}

/*****************************************************************************************/
// merge
// ��������������ļ��� S1 �� S2 �ϲ�����������һ�οռ䣬����һ��������ָ�����һ��Ԫ�ص���һλ��
/*****************************************************************************************/
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, 
	InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
		}
		else {
			*result = *first2;
			++first2;
		}
		++result;
	}
	return copy(first2, last2, copy(first1, last1, result));
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class BinaryPredicate>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, 
	InputIterator2 first2, InputIterator2 last2, OutputIterator result, BinaryPredicate binary_pred) {
	while (first1 != last1 && first2 != last2) {
		if (binary_pred(*first1, *first2)) {
			*result = *first1;
			++first1;
		}
		else {
			*result = *first2;
			++first2;
		}
		++result;
	}
	return copy(first2, last2, copy(first1, last1, result));
}

/*****************************************************************************************/
// inplace_merge
// ��������һ��������������н�ϳɵ�һ���в���������
/*****************************************************************************************/
template<class BidirectionalIterator1, class BidirectionalIterator2, class Distance>
BidirectionalIterator1 rotate_adaptive(BidirectionalIterator1 first, BidirectionalIterator1 middle,
	BidirectionalIterator1 last, Distance len1, Distance len2, BidirectionalIterator2 buffer, Distance buffer_size) {
	BidirectionalIterator2 buffer_end;
	if (len1 > len2 && len2 <= buffer_size) {
		buffer_end = copy(middle, last, buffer);
		copy_backward(first, middle, last);
		return copy(buffer, buffer_end, first);
	}
	else if (len1 <= buffer_size) {
		buffer_end = copy(first, middle, buffer);
		copy(middle, last, first);
		return copy_backward(buffer, buffer_end, last);
	}
	else
		return rotate(first, middle, last);
}

template<class BidirectionalIterator1, class BidirectionalIterator2, class BidirectionalIterator3>
BidirectionalIterator3 merge_backward(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
	BidirectionalIterator2 first2, BidirectionalIterator2 last2, BidirectionalIterator3 result) {
	if (first1 == last1)
		return copy_backward(first2, last2, result);
	if (first2 == last2)
		return copy_backward(first1, last1, result);
	--last1;
	--last2;
	while (true) {
		if (*last2 < *last1) {
			*--result = *last1;
			if (first1 == last1)
				return copy_backward(first2, ++last2, result);
			--last1;
		}
		else {
			*--result = *last2;
			if (first2 == last2)
				return copy_backward(first1, ++last1, result);
			--last2;
		}
	}
}

template<class BidirectionalIterator, class Distance, class Pointer>
void merge_adaptive(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last,
	Distance len1, Distance len2, Pointer buffer, Distance buffer_size) {
	if (len1 <= len2 && len1 <= buffer_size) {
		Pointer buffer_end = copy(first, middle, buffer);
		merge(buffer, buffer_end, middle, last, first);
	}
	else if (len2 <= buffer_size) {
		Pointer buffer_end = copy(middle, last, buffer);
		merge_backward(first, middle, buffer, buffer_end, last);
	}
	else {
		BidirectionalIterator first_cut = first;
		BidirectionalIterator second_cut = middle;
		Distance len11 = 0;
		Distance len22 = 0;
		if (len1 > len2) {
			len11 = len1 >> 1;
			advance(first_cut, len11);
			second_cut = lower_bound(middle, last, *first_cut);
			len22 = distance(middle, second_cut);
		}
		else {
			len22 = len2 >> 1;
			advance(second_cut, len22);
			first_cut = upper_bound(first, middle, *second_cut);
			len11 = distance(first, first_cut);
		}
		BidirectionalIterator new_middle = rotate_adaptive(first_cut, middle, second_cut, len1 - len11, len22, buffer, buffer_size);
		merge_adaptive(first, first_cut, new_middle, len11, len22, buffer, buffer_size);
		merge_adaptive(new_middle, second_cut, last, len1 - len11, len2 - len22, buffer, buffer_size);
	}
}

template<class BidirectionalIterator, class Distance>
void merge_without_buffer(BidirectionalIterator first, BidirectionalIterator middle,
	BidirectionalIterator last, Distance len1, Distance len2) {
	if (0 == len1 || 0 == len2)
		return;
	if (2 == len1 + len2) {
		if (*middle < *first)
			iter_swap(first, middle);
		return;
	}
	BidirectionalIterator first_cut = first;
	BidirectionalIterator second_cut = middle;
	Distance len11 = 0;
	Distance len22 = 0;
	if (len1 > len2) {
		len11 = len1 >> 1;
		advance(first_cut, len11);
		second_cut = lower_bound(middle, last, *first_cut);
		len22 = distance(middle, second_cut);
	}
	else {
		len22 = len2 >> 1;
		advance(second_cut, len22);
		first_cut = upper_bound(first, middle, *second_cut);
		len11 = distance(first, first_cut);
	}
	BidirectionalIterator new_middle = rotate(first_cut, middle, second_cut);
	merge_without_buffer(first, first_cut, new_middle, len11, len22);
	merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22);
}

template<class BidirectionalIterator, class T>
void inplace_merge_aux(BidirectionalIterator first, BidirectionalIterator middle,
	BidirectionalIterator last, T*) {
	auto len1 = distance(first, middle);
	auto len2 = distance(middle, last);
	temporary_buffer<BidirectionalIterator, T> buf(first, last);
	if (nullptr == buf.begin()) {
		merge_without_buffer(first, middle, last, len1, len2);
	}
	else {
		merge_adaptive(first, middle, last, len1, len2, buf.begin(), buf.size());
	}
}

template<class BidirectionalIterator>
void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last) {
	if (first == middle || middle == last)
		return;
	inplace_merge_aux(first, middle, last, value_type(first));
}

template<class BidirectionalIterator1, class BidirectionalIterator2, class BidirectionalIterator3, class BinaryPredicate>
BidirectionalIterator3 merge_backward(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
	BidirectionalIterator2 first2, BidirectionalIterator2 last2, BidirectionalIterator3 result, BinaryPredicate binary_pred) {
	if (first1 == last1)
		return copy_backward(first2, last2, result);
	if (first2 == last2)
		return copy_backward(first1, last1, result);
	--last1;
	--last2;
	while (true) {
		if (binary_pred(*last2, *last1)) {
			*--result = *last1;
			if (first1 == last1)
				return copy_backward(first2, ++last2, result);
			--last1;
		}
		else {
			*--result = *last2;
			if (first2 == last2)
				return copy_backward(first1, ++last1, result);
			--last2;
		}
	}
}

template<class BidirectionalIterator, class Distance, class Pointer, class BinaryPredicate>
void merge_adaptive(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last,
	Distance len1, Distance len2, Pointer buffer, Distance buffer_size, BinaryPredicate binary_pred) {
	if (len1 <= len2 && len1 <= buffer_size) {
		Pointer buffer_end = copy(first, middle, buffer);
		mystl::merge(buffer, buffer_end, middle, last, first, binary_pred);
	}
	else if (len2 <= buffer_size) {
		Pointer buffer_end = copy(middle, last, buffer);
		merge_backward(first, middle, buffer, buffer_end, last, binary_pred);
	}
	else {
		BidirectionalIterator first_cut = first;
		BidirectionalIterator second_cut = middle;
		Distance len11 = 0;
		Distance len22 = 0;
		if (len1 > len2) {
			len11 = len1 >> 1;
			advance(first_cut, len11);
			second_cut = mystl::lower_bound(middle, last, *first_cut, binary_pred);
			len22 = distance(middle, second_cut);
		}
		else {
			len22 = len2 >> 1;
			advance(second_cut, len22);
			first_cut = mystl::upper_bound(first, middle, *second_cut, binary_pred);
			len11 = distance(first, first_cut);
		}
		BidirectionalIterator new_middle = rotate_adaptive(first_cut, middle, second_cut, len1 - len11, len22, buffer, buffer_size);
		merge_adaptive(first, first_cut, new_middle, len11, len22, buffer, buffer_size, binary_pred);
		merge_adaptive(new_middle, second_cut, last, len1 - len11, len2 - len22, buffer, buffer_size, binary_pred);
	}
}

template<class BidirectionalIterator, class Distance, class BinaryPredicate>
void merge_without_buffer(BidirectionalIterator first, BidirectionalIterator middle,
	BidirectionalIterator last, Distance len1, Distance len2, BinaryPredicate binary_pred) {
	if (0 == len1 || 0 == len2)
		return;
	if (2 == len1 + len2) {
		if (binary_pred(*middle, *first))
			iter_swap(first, middle);
		return;
	}
	BidirectionalIterator first_cut = first;
	BidirectionalIterator second_cut = middle;
	Distance len11 = 0;
	Distance len22 = 0;
	if (len1 > len2) {
		len11 = len1 >> 1;
		advance(first_cut, len11);
		second_cut = mystl::lower_bound(middle, last, *first_cut, binary_pred);
		len22 = distance(middle, second_cut);
	}
	else {
		len22 = len2 >> 1;
		advance(second_cut, len22);
		first_cut = mystl::upper_bound(first, middle, *second_cut, binary_pred);
		len11 = distance(first, first_cut);
	}
	BidirectionalIterator new_middle = rotate(first_cut, middle, second_cut);
	merge_without_buffer(first, first_cut, new_middle, len11, len22, binary_pred);
	merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22, binary_pred);
}

template<class BidirectionalIterator, class T, class BinaryPredicate>
void inplace_merge_aux(BidirectionalIterator first, BidirectionalIterator middle,
	BidirectionalIterator last, T*, BinaryPredicate binary_pred) {
	auto len1 = distance(first, middle);
	auto len2 = distance(middle, last);
	temporary_buffer<BidirectionalIterator, T> buf(first, last);
	if (!buf.begin()) {
		merge_without_buffer(first, middle, last, len1, len2, binary_pred);
	}
	else {
		merge_adaptive(first, middle, last, len1, len2, buf.begin(), buf.size(), binary_pred);
	}
}

template<class BidirectionalIterator, class BinaryPredicate>
void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, 
	BidirectionalIterator last, BinaryPredicate binary_pred) {
	if (first == middle || middle == last)
		return;
	inplace_merge_aux(first, middle, last, value_type(first), binary_pred);
}

/*****************************************************************************************/
// partial_sort
// �������������������򣬱�֤��С�� N ��Ԫ���Ե���˳������[first, first + N)��
/*****************************************************************************************/
template<class RandomAccessIterator>
void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last) {
	make_heap(first, middle);
	for (RandomAccessIterator i = middle; i != last; ++i) {
		if (*i < *first)
			__pop_heap(first, middle, i, *i, distance_type(first));
	}
	mystl::sort_heap(first, middle);
}

template<class RandomAccessIterator, class BinaryPredicate>
void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, 
	RandomAccessIterator last, BinaryPredicate binary_pred) {
	mystl::make_heap(first, middle, binary_pred);
	for (RandomAccessIterator i = middle; i != last; ++i) {
		if (binary_pred(*i, *first))
			__pop_heap(first, middle, i, *i, distance_type(first), binary_pred);
	}
	mystl::sort_heap(first, middle, binary_pred);
}

/*****************************************************************************************/
// partial_sort_copy
// ��Ϊ�� partial_sort ���ƣ���ͬ���ǰ����������Ƶ� result ������
/*****************************************************************************************/
template<class InputIterator, class RandomAccessIterator, class Distance>
RandomAccessIterator __partial_sort_copy(InputIterator first, InputIterator last,
	RandomAccessIterator result_first, RandomAccessIterator result_last, Distance*) {
	if (result_first == result_last)
		return result_last;
	RandomAccessIterator result_real_last = result_first;
	while (first != last && result_real_last != result_last) {
		*result_real_last = *first;
		++first;
		++result_real_last;
	}
	make_heap(result_first, result_real_last);
	while (first != last) {
		if (*first < *result_first) 
			adjust_heap(result_first, static_cast<Distance>(0), static_cast<Distance>(result_real_last - result_first), *first);
		++first;
	}
	sort_heap(result_first, result_real_last);
	return result_real_last;
}

template<class InputIterator, class RandomAccessIterator>
RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last,
	RandomAccessIterator result_first, RandomAccessIterator result_last) {
	return __partial_sort_copy(first, last, result_first, result_last, distance_type(result_first));
}

template<class InputIterator, class RandomAccessIterator, class Distance, class BinaryPredicate>
RandomAccessIterator __partial_sort_copy(InputIterator first, InputIterator last,
	RandomAccessIterator result_first, RandomAccessIterator result_last, Distance*, BinaryPredicate binary_pred) {
	if (result_first == result_last)
		return result_last;
	RandomAccessIterator result_real_last = result_first;
	while (first != last && result_real_last != result_last) {
		*result_real_last = *first;
		++first;
		++result_real_last;
	}
	mystl::make_heap(result_first, result_real_last, binary_pred);
	while (first != last) {
		if (binary_pred(*first, *result_first))
			adjust_heap(result_first, static_cast<Distance>(0), static_cast<Distance>(result_real_last - result_first), 
				*first, binary_pred);
		++first;
	}
	mystl::sort_heap(result_first, result_real_last, binary_pred);
	return result_real_last;
}

template<class InputIterator, class RandomAccessIterator, class BinaryPredicate>
RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last,
	RandomAccessIterator result_first, RandomAccessIterator result_last, BinaryPredicate binary_pred) {
	return __partial_sort_copy(first, last, result_first, result_last, distance_type(result_first), binary_pred);
}

/*****************************************************************************************/
// partition
// �������ڵ�Ԫ�����ţ���һԪ���������ж�Ϊ true ��Ԫ�ػ�ŵ������ǰ��
// �ú�������֤Ԫ�ص�ԭʼ���λ��
/*****************************************************************************************/
template<class ForwardIterator, class UnaryPredicate>
ForwardIterator __partition(ForwardIterator first, ForwardIterator last, UnaryPredicate unary_pred, forward_iterator_tag) {
	if (first == last)
		return first;
	while (unary_pred(*first)) {
		if (++first == last)
			return first;
	}

	ForwardIterator next = first;
	while (++next != last) {
		if (unary_pred(*next)) {
			swap(*first, *next);
			++first;
		}
	}
	return first;
}

template<class BidirectionalIterator, class UnaryPredicate>
BidirectionalIterator __partition(BidirectionalIterator first, BidirectionalIterator last,
	UnaryPredicate unary_pred, bidirectional_iterator_tag) {
	while (true) {
		while (true) {
			if (first == last)
				return first;
			else if (unary_pred(*first))
				++first;
			else
				break;
		}
		--last;
		while (true) {
			if (first == last)
				return first;
			else if (!unary_pred(*last))
				--last;
			else
				break;
		}
		iter_swap(first, last);
		++first;
	}
}

template<class ForwardIterator, class UnaryPredicate>
ForwardIterator partition(ForwardIterator first, ForwardIterator last, UnaryPredicate unary_pred) {
	return __partition(first, last, unary_pred, iterator_category(first));
}

/*****************************************************************************************/
// stable_partition
// �������ڵ�Ԫ�����ţ���һԪ���������ж�Ϊ true ��Ԫ�ػ�ŵ������ǰ��
// �ú�����֤Ԫ�ص�ԭʼ���λ��
/*****************************************************************************************/
template<class ForwardIterator, class UnaryPredicate, class Distance>
ForwardIterator inplace_stable_partition(ForwardIterator first, ForwardIterator last, UnaryPredicate unary_pred, Distance len) {
	if (1 == len)
		return unary_pred(*first) ? last : first;
	ForwardIterator middle = first;
	advance(middle, len >> 1);
	return rotate(inplace_stable_partition(first, middle, unary_pred, len >> 1),
		middle,
		inplace_stable_partition(middle, last, unary_pred, len - len >> 1));
}

template<class ForwardIterator, class UnaryPredicate, class Pointer, class Distance>
ForwardIterator stable_partition_adaptive(ForwardIterator first, ForwardIterator last, UnaryPredicate unary_pred,
	Distance len, Pointer buffer, Distance buffer_size) {
	if (len <= buffer_size) {
		ForwardIterator result1 = first;
		Pointer result2 = buffer;
		for (; first != last; ++first) {
			if (unary_pred(*first)) {
				*result1 = *first;
				++result1;
			}
			else {
				*result2 = *first;
				++result2;
			}
		}
		copy(buffer, result2, result1);
		return result1;
	}
	else {
		ForwardIterator middle = first;
		advance(middle, len >> 1);
		return rotate(stable_partition_adaptive(first, middle, unary_pred, len >> 1, buffer, buffer_size),
			middle, 
			stable_partition_adaptive(middle, last, unary_pred, len - len >> 1, buffer, buffer_size));
	}
}

template<class ForwardIterator, class UnaryPredicate, class Distance, class T>
ForwardIterator stable_partition_aux(ForwardIterator first, ForwardIterator last, UnaryPredicate unary_pred, Distance*, T*) {
	temporary_buffer<ForwardIterator, T> buf(first, last);
	if (buf.size() > 0)
		return stable_partition_adaptive(first, last, unary_pred, static_cast<Distance>(buf.requested_size()),
			buf.begin(), static_cast<Distance>(buf.size()));
	else
		return inplace_stable_partition(first, last, unary_pred, static_cast<Distance>(buf.requested_size()));
}

template<class ForwardIterator, class UnaryPredicate>
ForwardIterator stable_partition(ForwardIterator first, ForwardIterator last, UnaryPredicate unary_pred) {
	if (first == last)
		return first;
	return stable_partition_aux(first, last, unary_pred, value_type(first), distance_type(first));
}

/*****************************************************************************************/
// partition_copy
// ��Ϊ�� partition ���ƣ���ͬ���ǣ�����һԪ�������ж�Ϊ true �ķŵ� result_true ���������
// ����ŵ� result_false ��������䣬������һ�� mystl::pair ָ�������������β��
/*****************************************************************************************/
template<class InputIterator, class OutputIterator1, class OutputIterator2, class UnaryPredicate>
pair<OutputIterator1, OutputIterator2> partition_copy(InputIterator first, InputIterator last,
	OutputIterator1 result_true, OutputIterator2 result_false, UnaryPredicate unary_pred) {
	for (; first != last; ++first) {
		if (unary_pred(*first)) {
			*result_true = *first;
			++result_true;
		}
		else {
			*result_false = *first;
			++result_false;
		}
	}
	return pair<OutputIterator1, OutputIterator2>(result_true, result_false);
}

/*****************************************************************************************/
// sort
// ��[first, last)�ڵ�Ԫ���Ե����ķ�ʽ����
/*****************************************************************************************/
constexpr static size_t stl_threshold = 128;

template<class RandomAccessIterator, class T>
void unguarded_linear_insert(RandomAccessIterator last, T value) {
	RandomAccessIterator next = last;
	--next;
	while (value < *next) {
		*last = *next;
		last = next;
		--next;
	}
	*last = value;
}

template<class RandomAccessIterator>
void unguarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
	for (RandomAccessIterator i = first; i != last; ++i)
		unguarded_linear_insert(i, *i);
}

template<class RandomAccessIterator>
void linear_insert(RandomAccessIterator first, RandomAccessIterator last) {
	auto value = *last;
	if (value < *first) {
		copy_backward(first, last, last + 1);
		*first = value;
	}
	else
		unguarded_linear_insert(last, value);
}

template<class RandomAccessIterator>
void insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
	if (first == last)
		return;
	for (RandomAccessIterator i = first + 1; i != last; ++i)
		linear_insert(first, i);

}

template<class RandomAccessIterator>
void final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
	if (static_cast<size_t>(last - first) > stl_threshold) {
		insertion_sort(first, first + stl_threshold);
		unguarded_insertion_sort(first + stl_threshold, last);
	}
	else
		insertion_sort(first, last);
}

template<class RandomAccessIterator, class T>
RandomAccessIterator unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, const T& pivot) {
	while (true) {
		while (*first < pivot)
			++first;
		--last;
		while (pivot < *last)
			--last;
		if (!(first < last))
			return first;
		iter_swap(first, last);
		++first;
	}
}

template<class Size>
Size lg(Size n) {
	Size k = 0;
	for (; n != 1; n >>= 1)
		++k;
	return k;
}

template<class RandomAccessIterator, class Size>
void introsort_loop(RandomAccessIterator first, RandomAccessIterator last, Size depth_limit) {
	while (static_cast<size_t>(last - first) > stl_threshold) {
		if (0 == depth_limit) {
			partial_sort(first, last, last);
			return;
		}
		--depth_limit;
		auto mid = median(*first, *(first + (last - first) / 2), *(last - 1));
		RandomAccessIterator cut = unguarded_partition(first, last, mid);
		introsort_loop(cut, last, depth_limit);
		last = cut;
	}
}

template<class RandomAccessIterator>
void sort(RandomAccessIterator first, RandomAccessIterator last) {
	if (first != last) {
		introsort_loop(first, last, lg(last - first) * 2);
		final_insertion_sort(first, last);
	}
}

template<class RandomAccessIterator, class T, class BinaryPredicate>
void unguarded_linear_insert(RandomAccessIterator last, T value, BinaryPredicate binary_pred) {
	RandomAccessIterator next = last;
	--next;
	while (binary_pred(value, *next)) {
		*last = *next;
		last = next;
		--next;
	}
	*last = value;
}

template<class RandomAccessIterator, class BinaryPredicate>
void unguarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate binary_pred) {
	for (RandomAccessIterator i = first; i != last; ++i)
		unguarded_linear_insert(i, *i, binary_pred);
}

template<class RandomAccessIterator, class BinaryPredicate>
void linear_insert(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate binary_pred) {
	auto value = *last;
	if (binary_pred(value, *first)) {
		copy_backward(first, last, last + 1);
		*first = value;
	}
	else
		unguarded_linear_insert(last, value, binary_pred);
}

template<class RandomAccessIterator, class BinaryPredicate>
void insertion_sort(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate binary_pred) {
	if (first == last)
		return;
	for (RandomAccessIterator i = first + 1; i != last; ++i)
		linear_insert(first, i, binary_pred);
}

template<class RandomAccessIterator, class BinaryPredicate>
void final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate binary_pred) {
	if (static_cast<size_t>(last - first) > stl_threshold) {
		insertion_sort(first, first + stl_threshold, binary_pred);
		unguarded_insertion_sort(first + stl_threshold, last, binary_pred);
	}
	else
		insertion_sort(first, last, binary_pred);
}

template<class RandomAccessIterator, class T, class BinaryPredicate>
RandomAccessIterator unguarded_partition(RandomAccessIterator first, 
	RandomAccessIterator last, const T& pivot, BinaryPredicate binary_pred) {
	while (true) {
		while (binary_pred(*first, pivot))
			++first;
		--last;
		while (binary_pred(*last, pivot))
			--last;
		if (!(first < last))
			return first;
		iter_swap(first, last);
		++first;
	}
}

template<class RandomAccessIterator, class Size, class BinaryPredicate>
void introsort_loop(RandomAccessIterator first, RandomAccessIterator last, Size depth_limit, BinaryPredicate binary_pred) {
	while (static_cast<size_t>(last - first) > stl_threshold) {
		if (0 == depth_limit) {
			mystl::partial_sort(first, last, last, binary_pred);
			return;
		}
		--depth_limit;
		auto mid = median(*first, *(first + (last - first) / 2), *(last - 1), binary_pred);
		RandomAccessIterator cut = unguarded_partition(first, last, mid, binary_pred);
		introsort_loop(cut, last, depth_limit, binary_pred);
		last = cut;
	}
}

template<class RandomAccessIterator, class BinaryPredicate>
void sort(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate binary_pred) {
	if (first != last) {
		introsort_loop(first, last, lg(last - first) * 2, binary_pred);
		final_insertion_sort(first, last, binary_pred);
	}
}

/*****************************************************************************************/
// stable_sort
// ��[first, last)�ڵ�Ԫ���Ե����ķ�ʽ����
// �����Ԫ�ص����λ�ú�����ǰһ��
/*****************************************************************************************/
constexpr static size_t stl_chunk_size = 7;

template<class RandomAccessIterator>
void inplace_stable_sort(RandomAccessIterator first, RandomAccessIterator last) {
	if (last - first < 15) {
		insertion_sort(first, last);
		return;
	}
	RandomAccessIterator middle = first + (last - first) / 2;
	inplace_stable_sort(first, middle);
	inplace_stable_sort(middle, last);
	merge_without_buffer(first, middle, last, middle - first, last - middle);
}

template<class RandomAccessIterator1, class RandomAccessIterator2, class Distance>
void merge_sort_loop(RandomAccessIterator1 first, RandomAccessIterator1 last, 
	RandomAccessIterator2 result, Distance step_size) {
	Distance two_step = step_size * 2;
	while (last - first >= two_step) {
		result = merge(first, first + step_size, first + step_size, first + two_step, result);
		first += two_step;
	}
	step_size = mystl::min(static_cast<Distance>(last - first), step_size);
	merge(first, first + step_size, first + step_size, last, result);
}

template<class RandomAccessIterator, class Distance>
void chunk_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Distance chunk_size) {
	while (last - first >= chunk_size) {
		insertion_sort(first, first + chunk_size);
		first += chunk_size;
	}
	insertion_sort(first, last);
}

template<class RandomAccessIterator, class Pointer, class Distance>
void merge_sort_with_buffer(RandomAccessIterator first, RandomAccessIterator last, Pointer buffer, Distance*) {
	Distance len = last - first;
	Pointer buffer_last = buffer + len;
	size_t step_size = stl_chunk_size;

	chunk_insertion_sort(first, last, step_size);
	for (auto j = first; j != last; ++j)
		std::cout << *j << " ";
	std::cout << "\n";
	while (step_size < len) {
		merge_sort_loop(first, last, buffer, step_size);
		step_size *= 2;
		merge_sort_loop(buffer, buffer_last, first, step_size);
		step_size *= 2;
	}
}

template<class RandomAccessIterator, class Pointer, class Distance>
void stable_sort_adaptive(RandomAccessIterator first, RandomAccessIterator last, Pointer buffer, Distance buffer_size) {
	Distance len = (last - first + 1) / 2;
	RandomAccessIterator middle = first + len;
	if (len > buffer_size) {
		stable_sort_adaptive(first, middle, buffer, buffer_size);
		stable_sort_adaptive(middle, last, buffer, buffer_size);
	}
	else {
		merge_sort_with_buffer(first, middle, buffer, static_cast<Distance*>(0));
		merge_sort_with_buffer(middle, last, buffer, static_cast<Distance*>(0));
	}
	merge_adaptive(first, middle, last, static_cast<Distance>(middle - first), 
		static_cast<Distance>(last - middle), buffer, buffer_size);
}

template<class RandomAccessIterator, class T, class Distance>
void stable_sort_aux(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*) {
	temporary_buffer<RandomAccessIterator, T> buf(first, last);
	if (buf.size() > 0)
		stable_sort_adaptive(first, last, buf.begin(), static_cast<Distance>(buf.size()));
	else
		inplace_stable_sort(first, last);
}

template<class RandomAccessIterator>
void stable_sort(RandomAccessIterator first, RandomAccessIterator last) {
	stable_sort_aux(first, last, value_type(first), distance_type(first));
}

template<class RandomAccessIterator, class BinaryPredicate>
void inplace_stable_sort(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate binary_pred) {
	if (last - first < 15) {
		insertion_sort(first, last, binary_pred);
		return;
	}
	RandomAccessIterator middle = first + ((last - first) >> 1);
	inplace_stable_sort(first, middle, binary_pred);
	inplace_stable_sort(middle, last, binary_pred);
	merge_without_buffer(first, middle, last, middle - first, last - middle, binary_pred);
}

template<class RandomAccessIterator1, class RandomAccessIterator2, class Distance, class BinaryPredicate>
void merge_sort_loop(RandomAccessIterator1 first, RandomAccessIterator1 last,
	RandomAccessIterator2 result, Distance step_size, BinaryPredicate binary_pred) {
	Distance two_step = step_size << 1;
	while (last - first >= two_step) {
		result = mystl::merge(first, first + step_size, first + step_size, first + two_step, result, binary_pred);
		first += two_step;
	}
	step_size = mystl::min(static_cast<Distance>(last - first), step_size);
	mystl::merge(first, first + step_size, first + step_size, last, result, binary_pred);
}

template<class RandomAccessIterator, class Distance, class BinaryPredicate>
void chunk_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Distance chunk_size, BinaryPredicate binary_pred) {
	while (last - first >= chunk_size) {
		insertion_sort(first, first + chunk_size, binary_pred);
		first += chunk_size;
	}
	insertion_sort(first, last, binary_pred);
}

template<class RandomAccessIterator, class Pointer, class Distance, class BinaryPredicate>
void merge_sort_with_buffer(RandomAccessIterator first, RandomAccessIterator last, 
	Pointer buffer, Distance*, BinaryPredicate binary_pred) {
	Distance len = last - first;
	Pointer buffer_last = buffer + len;
	size_t step_size = stl_chunk_size;

	chunk_insertion_sort(first, last, step_size, binary_pred);
	while (step_size < len) {
		merge_sort_loop(first, last, buffer, step_size, binary_pred);
		step_size <<= 1;
		merge_sort_loop(buffer, buffer_last, first, step_size, binary_pred);
		step_size <<= 1;
	}
}

template<class RandomAccessIterator, class Pointer, class Distance, class BinaryPredicate>
void stable_sort_adaptive(RandomAccessIterator first, RandomAccessIterator last, 
	Pointer buffer, Distance buffer_size, BinaryPredicate binary_pred) {
	Distance len = (last - first + 1) >> 1;
	RandomAccessIterator middle = first + len;
	if (len > buffer_size) {
		stable_sort_adaptive(first, middle, buffer, buffer_size, binary_pred);
		stable_sort_adaptive(middle, last, buffer, buffer_size, binary_pred);
	}
	else {
		merge_sort_with_buffer(first, middle, buffer, static_cast<Distance*>(0), binary_pred);
		merge_sort_with_buffer(middle, last, buffer, static_cast<Distance*>(0), binary_pred);
	}
	merge_adaptive(first, middle, last, static_cast<Distance>(middle - first),
		static_cast<Distance>(last - middle), buffer, buffer_size, binary_pred);
}

template<class RandomAccessIterator, class T, class Distance, class BinaryPredicate>
void stable_sort_aux(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*, BinaryPredicate binary_pred) {
	temporary_buffer<RandomAccessIterator, T> buf(first, last);
	if (buf.size() > 0)
		stable_sort_adaptive(first, last, buf.begin(), static_cast<Distance>(buf.size()), binary_pred);
	else
		inplace_stable_sort(first, last, binary_pred);
}

template<class RandomAccessIterator, class BinaryPredicate>
void stable_sort(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate binary_pred) {
	stable_sort_aux(first, last, value_type(first), distance_type(first), binary_pred);
}

/*****************************************************************************************/
// merge sort
// ��[first, last)�ڵ�Ԫ���Ե����ķ�ʽ����
/*****************************************************************************************/
template<class BidirectionalIterator>
void mergesort(BidirectionalIterator first, BidirectionalIterator last) {
	auto n = distance(first, last);
	if (0 == n || 1 == n)
		return;
	BidirectionalIterator middle = first + (n >> 1);
	mergesort(first, middle);
	mergesort(middle, last);
	inplace_merge(first, middle, last);
}

template<class BidirectionalIterator, class BinaryPredicate>
void mergesort(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate binary_pred) {
	auto n = distance(first, last);
	if (0 == n || 1 == n)
		return;
	BidirectionalIterator middle = first + (n >> 1);
	mergesort(first, middle, binary_pred);
	mergesort(middle, last, binary_pred);
	mystl::inplace_merge(first, middle, last, binary_pred);
}

/*****************************************************************************************/
// nth_element
// ���������ţ�ʹ������С�ڵ� n ��Ԫ�ص�Ԫ�س���������ǰ�棬�������ĳ��������ĺ���
/*****************************************************************************************/
template<class RandomAccessIterator>
void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last) {
	while (last - first > 3) {
		RandomAccessIterator cut = unguarded_partition(first, last, mystl::median(*first, *(first + ((last - first) >> 1)), *(last - 1)));
		if (cut <= nth)
			first = cut;
		else
			last = cut;
	}
	insertion_sort(first, last);
}

template<class RandomAccessIterator, class BinaryPredicate>
void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, BinaryPredicate binary_pred) {
	while (last - first > 3) {
		RandomAccessIterator cut = unguarded_partition(first, last, 
			mystl::median(*first, *(first + ((last - first) >> 1)), *(last - 1), binary_pred));
		if (cut <= nth)
			first = cut;
		else
			last = cut;
	}
	insertion_sort(first, last, binary_pred);
}

/*****************************************************************************************/
// unique_copy
// ��[first, last)�н�Ԫ�ظ��Ƶ� result �ϣ����б�������������ظ���Ԫ�أ�ֻ�Ḵ��һ��
/*****************************************************************************************/
template<class InputIterator, class OutputIterator>
OutputIterator __unique_copy(InputIterator first, InputIterator last, OutputIterator result, forward_iterator_tag) {
	*result = *first;
	while (++first != last) {
		if (*first != *result)
			*++result = *first;
	}
	return ++result;
}

template<class InputIterator, class OutputIterator>
OutputIterator __unique_copy(InputIterator first, InputIterator last, OutputIterator result, output_iterator_tag) {
	auto value = *first;
	*result = value;
	while (++first != last) {
		if (value != *first) {
			value = *first;
			*++result = value;
		}
	}
	return ++result;
}

template<class InputIterator, class OutputIterator>
OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last)
		return result;
	return __unique_copy(first, last, result, iterator_category(first));
}

template<class InputIterator, class OutputIterator, class BinaryPredicate>
OutputIterator __unique_copy(InputIterator first, InputIterator last, 
	OutputIterator result, forward_iterator_tag, BinaryPredicate binary_pred) {
	*result = *first;
	while (++first != last) {
		if (!(binary_pred(*first, *result)))
			*++result = *first;
	}
	return ++result;
}

template<class InputIterator, class OutputIterator, class BinaryPredicate>
OutputIterator __unique_copy(InputIterator first, InputIterator last, 
	OutputIterator result, output_iterator_tag, BinaryPredicate binary_pred) {
	auto value = *first;
	*result = value;
	while (++first != last) {
		if (!(binary_pred(*first, value))) {
			value = *first;
			*++result = value;
		}
	}
	return ++result;
}

template<class InputIterator, class OutputIterator, class BinaryPredicate>
OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result, BinaryPredicate binary_pred) {
	if (first == last)
		return result;
	return __unique_copy(first, last, result, iterator_category(first), binary_pred);
}

/*****************************************************************************************/
// unique
// �Ƴ�[first, last)���ظ���Ԫ�أ����б������򣬺� remove ���ƣ���Ҳ����������ɾ���ظ�Ԫ��
/*****************************************************************************************/
template<class ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
	first = mystl::adjacent_find(first, last);
	return unique_copy(first, last, first);
}

template<class ForwardIterator, class BinaryPredicate>
ForwardIterator unique(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred) {
	first = mystl::adjacent_find(first, last, binary_pred);
	return mystl::unique_copy(first, last, first, binary_pred);
}

}	// mystl

#endif