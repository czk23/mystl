#ifndef MYSTL_SET_ALGO_H
#define MYSTL_SET_ALGO_H

#include "algobase.h"

// ����ļ��������ĸ�set��ص��㷨: set_union, set_intersection, set_difference, set_symmetric_difference
// Ҫ���������set���������

namespace mystl {

/*****************************************************************************************/
// set_union
// ���� S1��S2 �Ľ�������浽 result �У�����һ��������ָ����������β��
/*****************************************************************************************/
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
		}
		else if (*first2 < *first1) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
			++first2;
		}
		++result;
	}
	return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
	while (first1 != last1 && first2 != last2) {
		if (comp(*first1, *first2)) {
			*result = *first1;
			++first1;
		}
		else if (comp(*first2, *first1)) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
			++first2;
		}
		++result;
	}
	return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
}

/*****************************************************************************************/
// set_intersection
// ���� S1��S2 �Ľ�������浽 result �У�����һ��������ָ����������β��
/*****************************************************************************************/
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2)
			++first1;
		else if (*first2 < *first1)
			++first2;
		else {
			*result = *first1;
			++first1;
			++first2;
			++result;
		}
	}
	return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
	while (first1 != last1 && first2 != last2) {
		if (comp(*first1, *last1))
			++first1;
		else if (comp(*first2, *last2))
			++first2;
		else {
			*result = *first1;
			++first1;
			++first2;
			++result;
		}
	}
	return result;
}

/*****************************************************************************************/
// set_difference
// ���� S1-S2 �Ľ�������浽 result �У�����һ��������ָ����������β��
/*****************************************************************************************/
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2 < *first1)
			++first2;
		else {
			++first1;
			++first2;
		}
	}
	return mystl::copy(first1, last1, result);
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
	while (first1 != last1 && first2 != last2) {
		if (comp(*first1, *first2)) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (comp(*first2, *first1))
			++first2;
		else {
			++first1;
			++first2;
		}
	}
	return mystl::copy(first1, last1, result);
}

/*****************************************************************************************/
// set_symmetric_difference
// ���� (S1-S2)��(S2-S1) �Ľ�������浽 result �У�����һ��������ָ����������β��
/*****************************************************************************************/
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2 < *first1) {
			*result = *first2;
			++first2;
			++result;
		}
		else {
			++first1;
			++first2;
		}
	}
	return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
	while (first1 != last1 && first2 != last2) {
		if (comp(*first1, *first2)) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (comp(*first2, *first1)) {
			*result = *first2;
			++first2;
			++result;
		}
		else {
			++first1;
			++first2;
		}
	}
	return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
}

}	// mystl


#endif