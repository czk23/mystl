#ifndef MYSTL_ALGOBASE_H
#define MYSTL_ALGOBASE_H

#include "iterator.h"
#include "mystl_pair.h"

// ����ļ�������һЩ�����㷨

namespace mystl {

/*****************************************************************************************/
// iter_swap
// ��������������ָ����Ե�
/*****************************************************************************************/
template<class ForwardIterator1, class ForwardIterator2, class T>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*) {
	T temp = *a;
	*a = *b;
	*b = temp;
}

template<class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
	__iter_swap(a, b, mystl::value_type(a));
}
/*****************************************************************************************/
// swap
// ��������������ֵ
/*****************************************************************************************/
template<class T>
inline void swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

/*****************************************************************************************/
// max
// ȡ�����еĽϴ�ֵ���������ʱ��֤���ص�һ������
/*****************************************************************************************/
template<class T>
inline const T& max(const T& lhs, const T& rhs) {
	return lhs < rhs ? rhs : lhs;
}

template<class T, class Compare>
inline const T& max(const T& lhs, const T& rhs, Compare comp) {
	return comp(lhs, rhs) ? rhs : lhs;
}

/*****************************************************************************************/
// min 
// ȡ�����еĽ�Сֵ���������ʱ��֤���ص�һ������
/*****************************************************************************************/
template<class T>
inline const T& min(const T& lhs, const T& rhs) {
	return rhs < lhs ? rhs : lhs;
}

template<class T, class Compare>
inline const T& min(const T& lhs, const T& rhs, Compare comp) {
	return comp(rhs, lhs) ? rhs : lhs;
}

/*****************************************************************************************/
// copy
// �� [first, last)�����ڵ�Ԫ�ؿ����� [result, result + (last - first))��
/*****************************************************************************************/
template<class InputIterator, class OutputIterator>
OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
	for (; first != last; ++first, ++result)
		*result = *first;
	return result;
}

template<class RandomAccessIterator, class OutputIterator>
OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_iterator_tag) {
	size_t n = static_cast<size_t>(last - first);
	for (; n > 0; --n, ++result, ++first)
		*result = *first;
	return result;
}

template<class InputIterator, class OutputIterator>
OutputIterator copy_aux(InputIterator first, InputIterator last, OutputIterator result) {
	return __copy(first, last, result, mystl::iterator_category(first));
}

template<class T, class U>
typename std::enable_if<std::is_same<typename std::remove_const<T>::value, U>::value&&
	std::is_trivially_copy_assignable<U>::value, U*>::type
	copy_aux(T* first, T* last, U* result) {
	size_t n = static_cast<size_t>(last - first);
	if (n != 0)
		std::memmove(result, first, n * sizeof(U));
	return first + n;
}

template<class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
	return copy_aux(first, last, result);
}

/*****************************************************************************************/
// copy_backward
// �� [first, last)�����ڵ�Ԫ�ؿ����� [result - (last - first), result)��
/*****************************************************************************************/
template<class T, class U>
typename std::enable_if<std::is_same<typename std::remove_const<T>::value, U>::value&&
	std::is_trivially_copy_assignable<U>::value, U*>::type
	copy_backward_aux(T* first, T* last, U* result) {
	size_t n = static_cast<size_t>(last - first);
	if (n != 0) {
		result -= n;
		std::memmove(result, first, sizeof(U) * n);
	}
	return result;
}

template<class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
	BidirectionalIterator2 result, bidirectional_iterator_tag) {
	while (first != last) {
		*--result = *--last;
	}
	return result;
}

template<class RandomAccessIterator, class BidirectionalIterator2>
BidirectionalIterator2 __copy_backward(RandomAccessIterator first, RandomAccessIterator last,
	BidirectionalIterator2 result, random_iterator_tag) {
	size_t n = static_cast<size_t>(last - first);
	for (; n > 0; --n) {
		*--result = *--last;
	}
	return result;
}
template<class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2 copy_backward_aux(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
	return __copy_backward(first, last, result, mystl::iterator_category(first));
}

template<class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
	return copy_backward_aux(first, last, result);
}

/*****************************************************************************************/
// copy_if
// ��[first, last)������һԪ���� unary_pred ��Ԫ�ؿ������� result Ϊ��ʼ��λ����
/*****************************************************************************************/
template<class InputIterator, class OutputIterator, class UnaryPredicate>
OutputIterator copy_if(InputIterator first, InputIterator last, OutputIterator result, UnaryPredicate unary_pred) {
	for (; first != last; ++first)
		if (unary_pred(*first))
			*result++ = *first;
	return result;
}

/*****************************************************************************************/
// copy_n
// �� [first, first + n)�����ϵ�Ԫ�ؿ����� [result, result + n)��
// ����һ�� pair �ֱ�ָ�򿽱�������β��
/*****************************************************************************************/
template<class InputIterator, class Size, class OutputIterator>
mystl::pair<InputIterator, OutputIterator> copy_n(InputIterator first, Size count, OutputIterator result) {
	return __copy_n(first, count, result, iterator_category(first));
}

template<class InputIterator, class Size, class OutputIterator>
mystl::pair<InputIterator, OutputIterator> __copy_n(InputIterator first, Size count, OutputIterator result, input_iterator_tag) {
	for (; count > 0; --count)
		*result++ = *first++;
	return mystl::pair<InputIterator, OutputIterator>(first, result);
}

template<class RandomAccessIterator, class Size, class OutputIterator>
mystl::pair<RandomAccessIterator, OutputIterator> copy_n(RandomAccessIterator first, Size count, 
	OutputIterator result, random_iterator_tag) {
	return mystl::pair<RandomAccessIterator, OutputIterator>(first + count, copy(first, first + count, result));
}

/*****************************************************************************************/
// fill_n
// �� first λ�ÿ�ʼ��� n ��ֵ
/*****************************************************************************************/
template<class OutputIterator, class Size, class T>
OutputIterator __fill_n(OutputIterator first, Size n, const T& value) {
	for (; n > 0; --n, ++first)
		*first = value;
	return first;
}

template<class T, class Size, class U>
typename std::enable_if <std::is_integral<T>::value && 1 == sizeof(T) && !std::is_same<T, bool>::value
	&& std::is_integral<U>::value && 1 == sizeof(U), T* >::type
	__fill_n(T* first, Size n, U value) {
	if (n > 0)
		std::memset(first, static_cast<unsigned char>(value), static_cast<size_t>(n));
	return first + n;
}

template<class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
	return __fill_n(first, n, value);
}

/*****************************************************************************************/
// fill
// Ϊ [first, last)�����ڵ�����Ԫ�������ֵ
/*****************************************************************************************/
template<class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value) {
	__fill(first, last, value, iterator_category(first));
}

template<class ForwardIterator, class T>
void __fill(ForwardIterator first, ForwardIterator last, const T& value, forward_iterator_tag) {
	for (; first != last; ++first)
		*first = value;
}

template<class RandomAccessIterator, class T>
void __fill(RandomAccessIterator first, RandomAccessIterator last, const T& value, random_iterator_tag) {
	size_t n = static_cast<size_t>(last - first);
	fill_n(first, n, value);
}

/*****************************************************************************************/
// mismatch
// ƽ�бȽ��������У��ҵ���һ��ʧ���Ԫ�أ�����һ�Ե��������ֱ�ָ������������ʧ���Ԫ��
/*****************************************************************************************/
template<class InputIterator1, class InputIterator2>
mystl::pair<InputIterator1, InputIterator2>
mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	while (first1 != last1) {
		if (*first1 != *first2)
			break;
		++first1;
		++first2;
	}
	return mystl::pair<InputIterator1, InputIterator2>(first1, first2);
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
mystl::pair<InputIterator1, InputIterator2>
mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred) {
	while (first1 != last1 && binary_pred(*first1, *first2)) {
		++first1;
		++first2;
	}
	return mystl::pair<InputIterator1, InputIterator2>(first1, first2);
}

/*****************************************************************************************/
// equal
// �Ƚϵ�һ������ [first, last)�����ϵ�Ԫ��ֵ�Ƿ�͵ڶ��������
/*****************************************************************************************/
template<class InputIterator1, class InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2)
		if (*first1 != *first2)
			return false;
	return true;
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred) {
	for (; first1 != last1; ++first1, ++first2) {
		if (!binary_pred(*first1, *first2))
			return false;
	}
	return true;
}

/*****************************************************************************************/
// lexicographical_compare
// ���ֵ������ж��������н��бȽϣ�����ĳ��λ�÷��ֵ�һ�鲻���Ԫ��ʱ�������м��������
// (1)�����һ���е�Ԫ�ؽ�С������ true �����򷵻� false
// (2)������� last1 ����δ���� last2 ���� true
// (3)������� last2 ����δ���� last1 ���� false
// (4)���ͬʱ���� last1 �� last2 ���� false
/*****************************************************************************************/
template<class InputIterator1, class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (*first1 < *first2)
			return true;
		if (*first1 > *first2)
			return false;
	}
	return first1 == last1 && first2 != last2;
}

template<class InputIterator1, class InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (comp(*first1, *first2))
			return true;
		if (comp(*first2, *first1))
			return false;
	}
	return first1 == last1 && first2 != last2;
}

bool lexicographical_compare(unsigned char* first1, unsigned char* last1, unsigned char* first2, unsigned char* last2) {
	const size_t len1 = last1 - first1;
	const size_t len2 = last2 - first2;
	const int result = std::memcmp(first1, first2, min(len1, len2));
	return 0 == result ? len1 < len2 : result < 0;
}

/*****************************************************************************************/
// lexicographical_compare_3way
// ���ֵ������ж��������н��бȽϣ�����ĳ��λ�÷��ֵ�һ�鲻���Ԫ��ʱ�������м��������
// (1)�����һ���е�Ԫ�ؽ�С������ -1 �����򷵻� 1
// (2)������� last1 ����δ���� last2 ���� -1
// (3)������� last2 ����δ���� last1 ���� 1
// (4)���ͬʱ���� last1 �� last2 ���� 0
/*****************************************************************************************/
template<class InputIterator1, class InputIterator2>
int lexicographical_compare_3way(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (*first1 < *first2)
			return -1;
		if (*first2 < *first1)
			return 1;
	}
	return first2 == last2 ? !(first1 == last1) : -1;
}

int lexicographical_compare_3way(unsigned char* first1, unsigned char* last1, unsigned char* first2, unsigned char* last2) {
	const ptrdiff_t len1 = last1 - first1;
	const ptrdiff_t len2 = last2 - first2;
	const int result = std::memcmp(first1, first2, min(len1, len2));
	return result != 0 ? result : (len1 == len2 ? 0 : len1 < len2 ? -1 : 1);
}

}	//mystl

#endif