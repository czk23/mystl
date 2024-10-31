#ifndef MYSTL_LIST_TEST_H
#define MYSTL_LIST_TEST_H

#include <list>

#include "test.h"
#include "list.h"
#include "functional.h"
#include <iostream>

namespace mystl {

namespace list_test {

void list_test() {
	std::cout << "[===============================================================]\n";
	std::cout << "[------------------ Run container test : list ------------------]\n";
	std::cout << "[-------------------------- API test ---------------------------]\n";

	int test[] = { 1, 2, 3, 4, 5 };
	mystl::list<int> l1;
	mystl::list<int> l2(5);
	mystl::list<int> l3(5, 1);
	mystl::list<int> l4(test, test + 5);
	mystl::list<int> l5(l2);
//	mystl::list<int> l6(std::move(l2));
	mystl::list<int> l6(l2);
	mystl::list<int> l7{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	mystl::list<int> l8;
	l8 = l3;
	mystl::list<int> l9(l3);
//	l9 = std::move(l3);
	mystl::list<int> l10;
	l10 = { 5, 1, 3, 2, 6, 8, 7, 9, 3, 5, 1 };

	FUN_AFTER(l1, l1.assign(8, 8));

	FUN_AFTER(l1, l1.assign(test, test + 5));
	FUN_AFTER(l1, l1.insert(l1.end(), 6));
	FUN_AFTER(l1, l1.insert(l1.end(), 2, 7));
	FUN_AFTER(l1, l1.insert(l1.begin(), test, test + 5));
	FUN_AFTER(l1, l1.push_back(2));
	FUN_AFTER(l1, l1.push_front(1));
//	FUN_AFTER(l1, l1.emplace(l1.begin(), 1));
//	FUN_AFTER(l1, l1.emplace_front(0));
//	FUN_AFTER(l1, l1.emplace_back(10));
	FUN_VALUE(l1.size());

	FUN_AFTER(l1, l1.pop_front());
	FUN_AFTER(l1, l1.pop_back());
	FUN_AFTER(l1, l1.erase(l1.begin()));
	FUN_AFTER(l1, l1.erase(l1.begin(), l1.end()));
	FUN_VALUE(l1.size());

	FUN_AFTER(l1, l1.resize(5, 1));
	FUN_AFTER(l1, l1.resize(8, 2));
	FUN_AFTER(l1, l1.resize(10));
	
	FUN_VALUE(l1.size());

	FUN_AFTER(l1, l1.splice(l1.end(), l4));
	FUN_AFTER(l1, l1.splice(l1.begin(), l5, l5.begin()));
	FUN_AFTER(l1, l1.splice(l1.end(), l6, l6.begin(), ++l6.begin()));
	FUN_VALUE(l1.size());

	FUN_AFTER(l1, l1.remove(0));
	FUN_AFTER(l1, l1.remove_if([&](int a) {return a & 1;}));
	FUN_VALUE(l1.size());

	FUN_AFTER(l1, l1.assign(l10.begin(), l10.end()));
	FUN_AFTER(l1, l1.sort());
	FUN_AFTER(l1, l1.unique());
	FUN_AFTER(l1, l1.unique([&](int a, int b) {return b == a + 1; }));
	FUN_AFTER(l1, l1.merge(l7));

	FUN_AFTER(l1, l1.assign(l10.begin(), l10.end()));
	FUN_AFTER(l1, l1.sort(mystl::greater<int>()));
	FUN_AFTER(l1, l1.merge(l8, mystl::greater<int>()));

	FUN_AFTER(l1, l1.reverse());
	FUN_AFTER(l1, l1.clear());
	FUN_AFTER(l1, l1.swap(l9));

	FUN_VALUE(*l1.begin());
	FUN_VALUE(*l1.rbegin());
	FUN_VALUE(l1.front());
	FUN_VALUE(l1.back());

	std::cout << std::boolalpha;
	FUN_VALUE(l1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(l1.size());
	FUN_VALUE(l1.max_size());
	PASSED;

#if PERFORMANCE_TEST_ON
	std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|       insert        |";
#if LARGER_TEST_DATA_ON
	CON_TEST_P2(list<int>, insert, end, rand(), XL(LEN1), XXL(LEN2), XXL(LEN3));
#else
	CON_TEST_P2(list<int>, insert, end, rand(), M(LEN1), M(LEN2), M(LEN3));
#endif
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|         sort        |";
#if LARGER_TEST_DATA_ON
	LIST_SORT_TEST(M(LEN1), M(LEN2), M(LEN3));
#else
	LIST_SORT_TEST(S(LEN1), S(LEN2), S(LEN3));
#endif
	std::cout << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	PASSED;
#endif
	std::cout << "[------------------ End container test : list ------------------]" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

}

	
}	// list_test

}	// mystl

#endif