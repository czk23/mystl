#ifndef MYSTL_NUMERIC_H
#define MYSTL_NUMERIC_H

#include "iterator_base.h"
#include "functional.h"

// 这个文件包含了数值运算的多个函数

namespace mystl {

/*****************************************************************************************/
// accumulate
// 版本1：以初值 init 对每个元素进行累加
// 版本2：以初值 init 对每个元素进行二元操作
/*****************************************************************************************/
// 版本1
template<class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T init) {
	for (; first != last; ++first)
		init += *first;
	return init;
};

// 版本2
template<class InputIterator, class T, class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op) {
	for (; first != last; ++first)
		init = binary_op(init, *first);
	return init;
};


/*****************************************************************************************/
// inner_product
// 版本1：以 init 为初值，计算两个区间的内积   
// 版本2：自定义 operator+ 和 operator*
/*****************************************************************************************/
// 版本1
template<class InputIterator1, class InputIterator2, class T>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) {
	for (; first1 != last1; ++first1, ++first2)
		init += *first1 * *first2;
	return init;
}

// 版本2
template<class InputIterator1, class InputIterator2, class T, class BinaryOperation1, class BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, 
	BinaryOperation1 binary_op1, BinaryOperation2 binary_op2) {
	for (; first1 != last1; ++first1, ++first2)
		init = binary_op1(init, binary_op2(*first1, *first2));
	return init;
}

/*****************************************************************************************/
// partial_sum
// 版本1：计算局部累计求和，结果保存到以 result 为起始的区间上
// 版本2：进行局部进行自定义二元操作
/*****************************************************************************************/
// 版本1
template<class InputIterator, class OutputIterator, class T>
OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, T*) {
	T value = *first;
	while (++first != last) {
		value += *first;
		*++result = value;
	}
	return ++result;
}

template<class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last)
		return result;
	*result = *first;
	return __partial_sum(first, last, result, mystl::value_type(first));
}

// 版本2
template<class InputIterator, class OutputIterator, class T, class BinaryOperation>
OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op, T*) {
	T value = *first;
	while (++first != last) {
		value = binary_op(value, *first);
		*++result = value;
	}
	return ++result;
}

template<class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
	if (first == last)
		return result;
	*result = *first;
	return __partial_sum(first, last, result, binary_op, mystl::value_type(first));
}

/*****************************************************************************************/
// adjacent_difference
// 版本1：计算相邻元素的差值，结果保存到以 result 为起始的区间上
// 版本2：自定义相邻元素的二元操作
/*****************************************************************************************/
// 版本1
template<class InputIterator, class OutputIterator, class T>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T*) {
	T value = *first;
	while (++first != last) {
		T temp = *first;
		*++result = temp - value;
		value = temp;
	}
	return ++result;
}

template<class InputIterator, class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last)
		return result;
	*result = *first;
	return __adjacent_difference(first, last, result, mystl::value_type(first));
}

template<class InputIterator, class OutputIterator, class BinaryOperation, class T>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op, T*) {
	T value = *first;
	while (++first != last) {
		T temp = *first;
		*++result = binary_op(temp, value);
		value = temp;
	}
	return ++result;
}

// 版本2
template<class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
	if (first == last)
		return result;
	*result = *first;
	return __adjacent_difference(first, last, result, binary_op, mystl::value_type(first));
}

// 模幂运算
template<class T, class Integer, class MonoidOperation>
T __power(T x, Integer n, MonoidOperation op) {
	T res;
	if (0 == n)
		return mystl::identity_element(op);
	else {
		while (0 == (n & 1)) {
			x = op(x, x);
			n >>= 1;
		}
		res = x;
		n >>= 1;
		while (n) {
			x = op(x, x);
			if (n & 1) {
				res = op(res, x);
			}
			n >>= 1;
		}
	}
	return res;
}

template<class T, class Integer>
inline T power(T x, Integer n) {
	return __power(x, n, mystl::multiplies<T>());
}

/*****************************************************************************************/
// iota
// 填充[first, last)，以 value 为初值开始递增
/*****************************************************************************************/
template<class ForwardIterator, class T>
void iota(ForwardIterator first, ForwardIterator last, T value) {
	while (first != last)
		*first++ = value++;
}



}	// mystl


#endif