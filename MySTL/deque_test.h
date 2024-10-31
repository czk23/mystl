#ifndef MYSTL_DEQUE_TEST_H
#define MTSTL_DEQUE_TEST_H

#include "deque.h"
#include "test.h"
#include <iostream>

namespace mystl {

namespace deque_test {

void deque_test() {
	std::cout << "[===============================================================]\n";
	std::cout << "[----------------- Run container test : deque ------------------]\n";
	std::cout << "[-------------------------- API test ---------------------------]\n";

	int test[] = { 1,2,3,4,5 };
	mystl::deque<int> d1;
	mystl::deque<int> d2(5);
	mystl::deque<int> d3(5, 1);
	mystl::deque<int> d4(test, test + 5);
	mystl::deque<int> d5(d2);
//	mystl::deque<int> d6(std::move(d2));
	mystl::deque<int> d7;
	d7 = d3;
//	mystl::deque<int> d8;
//	d8 = std::move(d3);
	mystl::deque<int> d9{ 1,2,3,4,5,6,7,8,9 };
	mystl::deque<int> d10;
	d10 = { 1,2,3,4,5,6,7,8,9 };

	FUN_AFTER(d1, d1.assign(5, 1));
	FUN_AFTER(d1, d1.assign(8, 8));
	FUN_AFTER(d1, d1.assign(test, test + 5));

	FUN_AFTER(d1, d1.insert(d1.end(), 6));
	FUN_AFTER(d1, d1.insert(d1.begin()));
	FUN_AFTER(d1, d1.insert(d1.end() - 1, 2, 7));
	FUN_AFTER(d1, d1.insert(d1.begin(), test, test + 5));

	FUN_AFTER(d1, d1.erase(d1.begin()));
	FUN_AFTER(d1, d1.erase(d1.begin(), d1.begin() + 4));

//	FUN_AFTER(d1, d1.emplace_back(8));
//	FUN_AFTER(d1, d1.emplace_front(8));
//	FUN_AFTER(d1, d1.emplace(d1.begin() + 1, 9));

	FUN_AFTER(d1, d1.push_front(1));
	FUN_AFTER(d1, d1.push_back(2));
	FUN_AFTER(d1, d1.pop_back());
	FUN_AFTER(d1, d1.pop_front());

	FUN_AFTER(d1, d1.resize(5));
	FUN_AFTER(d1, d1.resize(9, 9));
	FUN_AFTER(d1, d1.clear());

	FUN_AFTER(d1, d1.swap(d4));

	FUN_VALUE(*(d1.begin()));
	FUN_VALUE(*(d1.end() - 1));
	FUN_VALUE(*(d1.rbegin()));
	FUN_VALUE(*(d1.rend() - 1));

	FUN_VALUE(d1.front());
	FUN_VALUE(d1.back());

	FUN_VALUE(d1.at(1));
	FUN_VALUE(d1[2]);

	std::cout << std::boolalpha;
	FUN_VALUE(d1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(d1.size());
	FUN_VALUE(d1.max_size());
	PASSED;

#if PERFORMANCE_TEST_ON
	std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|     push_front      |";
#if LARGER_TEST_DATA_ON
	CON_TEST_P1(deque<int>, push_front, rand(), XLL(LEN1), XLL(LEN2), XLL(LEN3));
#else
	CON_TEST_P1(deque<int>, push_front, rand(), XL(LEN1), XL(LEN2), XL(LEN3));
#endif
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|     push_back       |";
#if LARGER_TEST_DATA_ON
	CON_TEST_P1(deque<int>, push_back, rand(), XLL(LEN1), XLL(LEN2), XLL(LEN3));
#else
	CON_TEST_P1(deque<int>, push_back, rand(), XL(LEN1), XL(LEN2), XL(LEN3));
#endif
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	PASSED;
#endif
	std::cout << "[----------------- End container test : deque ------------------]" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}






}	// vector_test

}	// mystl

#endif