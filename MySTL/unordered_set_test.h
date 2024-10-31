#ifndef MYSTL_UNORDERED_SET_TEST_H
#define MYSTL_UNORDERED_SET_TEST_H

#include <iostream>
#include "unordered_set.h"

namespace mystl {

namespace unordered_set_test {

void unordered_set_test() {
	std::cout << "[===============================================================]" << std::endl;
	std::cout << "[-------------- Run container test : unordered_set -------------]" << std::endl;
	std::cout << "[-------------------------- API test ---------------------------]" << std::endl;

	int test[] = { 5,4,3,2,1 };
	mystl::unordered_set<int> us1;
	mystl::unordered_set<int> us2(520);
	mystl::unordered_set<int> us3(520, mystl::hash<int>());
	mystl::unordered_set<int> us4(520, mystl::hash<int>(), mystl::equal_to<int>());
	mystl::unordered_set<int> us5(test, test + 5);
	mystl::unordered_set<int> us6(test, test + 5, 100);
	mystl::unordered_set<int> us7(test, test + 5, 100, mystl::hash<int>());
	mystl::unordered_set<int> us8(test, test + 5, 100, mystl::hash<int>(), mystl::equal_to<int>());
	mystl::unordered_set<int> us9(us5);
//	mystl::unordered_set<int> us10(std::move(us5));
	mystl::unordered_set<int> us11;
	us11 = us6;
//	mystl::unordered_set<int> us12;
//	us12 = std::move(us6);
	mystl::unordered_set<int> us13{ 1,2,3,4,5 };
	mystl::unordered_set<int> us14;
	us13 = { 1,2,3,4,5 };

//	FUN_AFTER(us1, us1.emplace(1));
	FUN_AFTER(us1, us1.insert(1));
//	FUN_AFTER(us1, us1.emplace_hint(us1.end(), 2));
	FUN_AFTER(us1, us1.insert(5));
	FUN_AFTER(us1, us1.insert(test, test + 5));

	FUN_AFTER(us1, us1.erase(us1.begin()));
	FUN_AFTER(us1, us1.erase(us1.begin(), us1.find(4)));
	FUN_AFTER(us1, us1.erase(4));

	std::cout << std::boolalpha;
	FUN_VALUE(us1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(us1.size());
	FUN_VALUE(us1.bucket_count());
	FUN_VALUE(us1.max_bucket_count());

	FUN_AFTER(us1, us1.clear());

	FUN_AFTER(us1, us1.swap(us7));

	FUN_VALUE(*us1.begin());

	std::cout << std::boolalpha;
	FUN_VALUE(us1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(us1.size());
	FUN_VALUE(us1.max_size());
	FUN_VALUE(us1.bucket_count());

	FUN_VALUE(us1.elems_in_bucket(1));
	FUN_VALUE(us1.elems_in_bucket(2));
	FUN_VALUE(us1.elems_in_bucket(3));

	FUN_VALUE(*us1.find(3));

	auto first = *us1.equal_range(3).first;
	auto second = *us1.equal_range(3).second;
	std::cout << " us1.equal_range(3) : from " << first << " to " << second << std::endl;

	PASSED;

#if PERFORMANCE_TEST_ON
	std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|       insert        |";
#if LARGER_TEST_DATA_ON
	CON_TEST_P1(unordered_set<int>, insert, rand(), XL(LEN1), XL(LEN2), XL(LEN3));
#else
	CON_TEST_P1(unordered_set<int>, insert, rand(), L(LEN1), L(LEN2), L(LEN3));
#endif
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	PASSED;
#endif
	std::cout << "[-------------- End container test : unordered_set -------------]" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

void unordered_multiset_test() {
	std::cout << "[===============================================================]" << std::endl;
	std::cout << "[------------ Run container test : unordered_multiset ----------]" << std::endl;
	std::cout << "[-------------------------- API test ---------------------------]" << std::endl;

	int test[] = { 5,4,3,2,1 };
	mystl::unordered_multiset<int> us1;
	mystl::unordered_multiset<int> us2(520);
	mystl::unordered_multiset<int> us3(520, mystl::hash<int>());
	mystl::unordered_multiset<int> us4(520, mystl::hash<int>(), mystl::equal_to<int>());
	mystl::unordered_multiset<int> us5(test, test + 5);
	mystl::unordered_multiset<int> us6(test, test + 5, 100);
	mystl::unordered_multiset<int> us7(test, test + 5, 100, mystl::hash<int>());
	mystl::unordered_multiset<int> us8(test, test + 5, 100, mystl::hash<int>(), mystl::equal_to<int>());
	mystl::unordered_multiset<int> us9(us5);
//	mystl::unordered_multiset<int> us10(std::move(us5));
	mystl::unordered_multiset<int> us11;
	us11 = us6;
//	mystl::unordered_multiset<int> us12;
//	us12 = std::move(us6);
	mystl::unordered_multiset<int> us13{ 1,2,3,4,5 };
	mystl::unordered_multiset<int> us14;
	us14 = { 1,2,3,4,5 };

//	FUN_AFTER(us1, us1.emplace(1));
	FUN_AFTER(us1, us1.insert(1));
	FUN_AFTER(us1, us1.insert(5));
	FUN_AFTER(us1, us1.insert(test, test + 5));

	FUN_AFTER(us1, us1.erase(us1.begin()));
	FUN_AFTER(us1, us1.erase(us1.begin(), us1.find(4)));
	FUN_AFTER(us1, us1.erase(4));

	std::cout << std::boolalpha;
	FUN_VALUE(us1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(us1.size());
	FUN_VALUE(us1.bucket_count());
	FUN_VALUE(us1.max_bucket_count());

	FUN_AFTER(us1, us1.clear());

	FUN_AFTER(us1, us1.swap(us7));

	FUN_VALUE(*us1.begin());

	std::cout << std::boolalpha;
	FUN_VALUE(us1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(us1.size());
	FUN_VALUE(us1.max_size());
	FUN_VALUE(us1.bucket_count());

	FUN_AFTER(us1, us1.insert(1));
	FUN_AFTER(us1, us1.insert(2));
	FUN_AFTER(us1, us1.insert(1));
	FUN_AFTER(us1, us1.insert(2));
	FUN_AFTER(us1, us1.insert(1));
	FUN_AFTER(us1, us1.insert(3));

	FUN_VALUE(us1.elems_in_bucket(1));
	FUN_VALUE(us1.elems_in_bucket(2));
	FUN_VALUE(us1.elems_in_bucket(3));

	FUN_VALUE(us1.bucket_count());
	FUN_VALUE(us1.count(1));

	FUN_VALUE(*us1.find(3));

	auto first = *us1.equal_range(3).first;
	auto second = *us1.equal_range(3).second;
	std::cout << " us1.equal_range(3) : from " << first << " to " << second << std::endl;

	PASSED;

#if PERFORMANCE_TEST_ON
	std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|       insert        |";
#if LARGER_TEST_DATA_ON
	CON_TEST_P1(unordered_multiset<int>, insert, rand(), M(LEN1), M(LEN2), M(LEN3));
#else
	CON_TEST_P1(unordered_multiset<int>, insert, rand(), M(LEN1), M(LEN2), M(LEN3));
#endif
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	PASSED;
#endif
	std::cout << "[------------ End container test : unordered_multiset ----------]" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}


}


}	// mystl



#endif