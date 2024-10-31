#ifndef MYSTL_SET_TEST
#define MYSTL_SET_TEST

#include "test.h"
#include "rb_tree.h"
#include "set.h"
#include <iostream>

namespace mystl {

namespace set_test {

void set_test() {
	std::cout << "[===============================================================]" << std::endl;
	std::cout << "[------------------ Run container test : set -------------------]" << std::endl;
	std::cout << "[-------------------------- API test ---------------------------]" << std::endl;

	int test[] = { 5,4,3,2,1 };
	mystl::set<int> s1;
	mystl::set<int, mystl::greater<int>> s2;
	mystl::set<int> s3(test, test + 5);
	mystl::set<int> s4(test, test + 5);
	mystl::set<int> s5(s3);
//	mystl::set<int> s6(std::move(s3));
	mystl::set<int> s7;
	s7 = s4;
//	mystl::set<int> s8;
//	s8 = std::move(s4);
	mystl::set<int> s9{ 1,2,3,4,5 };
	mystl::set<int> s10;
	s10 = { 1,2,3,4,5 };

	for (int i = 5; i >= 0; --i)
	{
		FUN_AFTER(s1, s1.insert(i));
	}

	FUN_AFTER(s1, s1.erase(s1.begin()));
	FUN_AFTER(s1, s1.erase(1));
	FUN_AFTER(s1, s1.erase(2));
	FUN_AFTER(s1, s1.erase(s1.begin(), s1.end()));

	FUN_AFTER(s1, s1.insert(test, test + 5));
	FUN_AFTER(s1, s1.insert(5));
	FUN_AFTER(s1, s1.insert(s1.end(), 6));

	FUN_VALUE(s1.count(5));

	FUN_VALUE(*s1.find(3));
	FUN_VALUE(*s1.lower_bound(3));
	FUN_VALUE(*s1.upper_bound(3));
	auto first = *s1.equal_range(3).first;
	auto second = *s1.equal_range(3).second;
	std::cout << " s1.equal_range(3) : from " << first << " to " << second << std::endl;

	FUN_AFTER(s1, s1.erase(s1.begin()));
	FUN_AFTER(s1, s1.erase(2));
	FUN_AFTER(s1, s1.erase(s1.begin(), s1.find(4)));
	FUN_AFTER(s1, s1.clear());

	FUN_AFTER(s1, s1.swap(s5));

	FUN_VALUE(*s1.begin());
	FUN_VALUE(*s1.rbegin());

	std::cout << std::boolalpha;
	FUN_VALUE(s1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(s1.size());
	FUN_VALUE(s1.max_size());

	PASSED;
#if PERFORMANCE_TEST_ON
	std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|       insert        |";
#if LARGER_TEST_DATA_ON
	CON_TEST_P1(set<int>, insert, rand(), XL(LEN1), XL(LEN2), XL(LEN3));
#else
	CON_TEST_P1(set<int>, insert, rand(), M(LEN1), M(LEN2), M(LEN3));
#endif
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	PASSED;
#endif
	std::cout << "[------------------ End container test : set -------------------]" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

void multiset_test() {
	std::cout << "[===============================================================]" << std::endl;
	std::cout << "[---------------- Run container test : multiset ----------------]" << std::endl;
	std::cout << "[-------------------------- API test ---------------------------]" << std::endl;

	int test[] = { 5,4,3,2,1 };
	mystl::multiset<int> s1;
	mystl::multiset<int, mystl::greater<int>> s2;
	mystl::multiset<int> s3(test, test + 5);
	mystl::multiset<int> s4(test, test + 5);
	mystl::multiset<int> s5(s3);
//	mystl::multiset<int> s6(std::move(s3));
	mystl::multiset<int> s7;
	s7 = s4;
//	mystl::multiset<int> s8;
//	s8 = std::move(s4);
	mystl::multiset<int> s9{ 1,2,3,4,5 };
	mystl::multiset<int> s10;
	s10 = { 1,2,3,4,5 };

	for (int i = 5; i >= 0; --i)
	{
		FUN_AFTER(s1, s1.insert(i));
	}
//	FUN_AFTER(s1, s1.emplace_hint(s1.begin(), 0));
	FUN_AFTER(s1, s1.erase(s1.begin()));
	FUN_AFTER(s1, s1.erase(1));
	FUN_AFTER(s1, s1.erase(2));
	FUN_AFTER(s1, s1.erase(s1.begin(), s1.end()));

	FUN_AFTER(s1, s1.insert(test, test + 5));
	FUN_AFTER(s1, s1.insert(5));
	FUN_AFTER(s1, s1.insert(s1.end(), 5));

	FUN_VALUE(s1.count(5));

	FUN_VALUE(*s1.find(3));
	FUN_VALUE(*s1.lower_bound(3));
	FUN_VALUE(*s1.upper_bound(3));
	auto first = *s1.equal_range(3).first;
	auto second = *s1.equal_range(3).second;
	std::cout << " s1.equal_range(3) : from " << first << " to " << second << std::endl;

	FUN_AFTER(s1, s1.erase(s1.begin()));
	FUN_AFTER(s1, s1.erase(2));
	FUN_AFTER(s1, s1.erase(s1.begin(), s1.find(4)));
	FUN_AFTER(s1, s1.clear());

	FUN_AFTER(s1, s1.swap(s5));

	FUN_VALUE(*s1.begin());
	FUN_VALUE(*s1.rbegin());

	std::cout << std::boolalpha;
	FUN_VALUE(s1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(s1.size());
	FUN_VALUE(s1.max_size());

	PASSED;
#if PERFORMANCE_TEST_ON
	std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|       insert        |";
#if LARGER_TEST_DATA_ON
	CON_TEST_P1(multiset<int>, insert, rand(), M(LEN1), M(LEN2), M(LEN3));
#else
	CON_TEST_P1(multiset<int>, insert, rand(), M(LEN1), M(LEN2), M(LEN3));
#endif
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	PASSED;
#endif
	std::cout << "[---------------- End container test : multiset ----------------]" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

}	// set_test

}	//mystl

#endif