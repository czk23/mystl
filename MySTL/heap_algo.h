#ifndef MYSTL_HEAP_ALGO_H
#define MYSTL_HEAP_ALGO_H

#include "iterator.h"

// 从heap.h改名为heap_algo.h
// 包含四个heap算法：push_heap、pop_heap、sort_heap和make_heap

namespace mystl {
//-------------------------------------------------push_heap-----------------------------------------------
// 默认情况为大根堆
template<class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance holeindex, Distance topindex, T value) {
	Distance parent = (holeindex - 1) / 2;
	while (topindex < holeindex && *(first + parent) < value) {
		*(first + holeindex) = *(first + parent);
		holeindex = parent;
		parent = (holeindex - 1) / 2;
	}
	*(first + holeindex) = value;
}

template<class RandomAccessIterator, class Distance, class T>
void push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
	__push_heap(first, static_cast<Distance>(last - first - 1), static_cast<Distance>(0), static_cast<T>(*(last - 1)));
}

template<class RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
	push_heap_aux(first, last, mystl::distance_type(first), mystl::value_type(first));
}

// 可以自定义排序函数
template<class RandomAccessIterator, class Distance, class T, class Compare>
void __push_heap(RandomAccessIterator first, Distance holeindex, Distance topindex, T value, Compare comp) {
	Distance parent = (holeindex - 1) / 2;
	while (topindex < holeindex && comp(*(first + parent), value)) {
		*(first + holeindex) = *(first + parent);
		holeindex = parent;
		parent = (holeindex - 1) / 2;
	}
	*(first + holeindex) = value;
}

template<class RandomAccessIterator, class Distance, class T, class Compare>
void push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp) {
	__push_heap(first, static_cast<Distance>((last - first) - 1), static_cast<Distance>(0), static_cast<T>(*(last - 1)), comp);
}

template<class RandomAccessIterator, class Compare>
void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	push_heap_aux(first, last, mystl::distance_type(first), mystl::value_type(first), comp);
}

//-------------------------------------------------pop_heap-----------------------------------------------
// 默认情况为大根堆
template<class RandomAccessIterator, class T, class Distance>
void adjust_heap(RandomAccessIterator first, Distance holeindex, Distance len, T value) {
	Distance topindex = holeindex;
	Distance rchild = holeindex * 2 + 2;
	while (rchild < len) {
		if (*(first + rchild) < *(first + rchild - 1))
			--rchild;
		*(first + holeindex) = *(first + rchild);
		holeindex = rchild;
		rchild = holeindex * 2 + 2;
	}
	if (rchild == len) {
		*(first + holeindex) = *(first + (rchild - 1));
		holeindex = rchild - 1;
	}
	__push_heap(first, holeindex, topindex, value);
	// 因为这里弹出的位置不一定是堆中的末尾(比如partial_sort函数), 所以还是需要调用push_heap函数
	// *(first + holeindex) = value;
}

template<class RandomAccessIterator, class T, class Distance>
void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*) {
	*result = *first;
	adjust_heap(first, static_cast<Distance>(0), static_cast<Distance>(last - first), value);
}

template<class RandomAccessIterator, class T>
void pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*) {
	__pop_heap(first, last - 1, last - 1, static_cast<T>(*(last - 1)), mystl::distance_type(first));
}

template<class RandomAccessIterator>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
	pop_heap_aux(first, last, mystl::value_type(first));
}

// 可自定义排序函数
template<class RandomAccessIterator, class T, class Distance, class Compare>
void adjust_heap(RandomAccessIterator first, Distance holeindex, Distance len, T value, Compare comp) {
	Distance topindex = holeindex;
	Distance rchild = holeindex * 2 + 2;
	while (rchild < len) {
		if (comp(*(first + rchild), *(first + rchild - 1)))
			--rchild;
		*(first + holeindex) = *(first + rchild);
		holeindex = rchild;
		rchild = holeindex * 2 + 2;
	}
	if (rchild == len) {
		*(first + holeindex) = *(first + rchild - 1);
		holeindex = rchild - 1;
	}
	__push_heap(first, holeindex, topindex, value, comp);
	// 因为这里弹出的位置不一定是堆中的末尾(比如partial_sort函数), 所以还是需要调用push_heap函数
	// *(first + holeindex) = value;
}

template<class RandomAccessIterator, class T, class Distance, class Compare>
void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*, Compare comp) {
	*result = *first;
	adjust_heap(first, static_cast<Distance>(0), static_cast<Distance>(last - first), value, comp);
}

template<class RandomAccessIterator, class T, class Compare>
void pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*, Compare comp) {
	__pop_heap(first, last - 1, last - 1, static_cast<T>(*(last - 1)), mystl::distance_type(first), comp);
}

template<class RandomAccessIterator, class Compare>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	pop_heap_aux(first, last, mystl::value_type(first), comp);
}

//-------------------------------------------------sort_heap-----------------------------------------------
// 默认情况为大根堆
template<class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
	while (last - first > 1)
		pop_heap(first, last--);
}

// 可自定义排序函数
template<class RandomAccessIterator, class Compare>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	while (last - first > 1)
		mystl::pop_heap(first, last--, comp);
}

//-------------------------------------------------make_heap-----------------------------------------------
// 默认情况为大根堆
template<class RandomAccessIterator, class Distance, class T>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
	Distance len = last - first;
	if (len < 2)
		return;
	Distance holeindex = (len - 2) / 2;
	while (true) {
		adjust_heap(first, holeindex, len, static_cast<T>(*(first + holeindex)));
		if (0 == holeindex)
			return;
		holeindex--;
	}
}

template<class RandomAccessIterator>
void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
	__make_heap(first, last, mystl::distance_type(first), mystl::value_type(first));
}

// 可自定义排序函数
template<class RandomAccessIterator, class Distance, class T, class Compare>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp) {
	Distance len = last - first;
	if (len < 2)
		return;
	Distance holeindex = (len - 2) / 2;
	while (true) {
		adjust_heap(first, holeindex, len, static_cast<T>(*(first + holeindex)), comp);
		if (0 == holeindex)
			return;
		holeindex--;
	}
}
template<class RandomAccessIterator, class Compare>
void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	__make_heap(first, last, mystl::distance_type(first), mystl::value_type(first), comp);
}

}	// mystl

#endif