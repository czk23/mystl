#ifndef MYSTL_UNORDERED_MAP_TEST_H
#define MYSTL_UNORDERED_MAP_TEST_H

#include "unordered_map.h"
#include "map_test.h"

namespace mystl {

namespace unordered_map_test {

void unorder_map_test() {
	std::cout << "[===============================================================]" << std::endl;
	std::cout << "[-------------- Run container test : unordered_map -------------]" << std::endl;
	std::cout << "[-------------------------- API test ---------------------------]" << std::endl;

	mystl::vector<PAIR> v;
	for (int i = 0; i < 5; ++i)
		v.push_back(PAIR(5 - i, 5 - i));
	mystl::unordered_map<int, int> um1;
	mystl::unordered_map<int, int> um2(520);
	mystl::unordered_map<int, int> um3(520, mystl::hash<int>());
	mystl::unordered_map<int, int> um4(520, mystl::hash<int>(), mystl::equal_to<int>());
	mystl::unordered_map<int, int> um5(v.begin(), v.end());
	mystl::unordered_map<int, int> um6(v.begin(), v.end(), 100);
	mystl::unordered_map<int, int> um7(v.begin(), v.end(), 100, mystl::hash<int>());
	mystl::unordered_map<int, int> um8(v.begin(), v.end(), 100, mystl::hash<int>(), mystl::equal_to<int>());
	mystl::unordered_map<int, int> um9(um5);
//	mystl::unordered_map<int, int> um10(std::move(um5));
	mystl::unordered_map<int, int> um11;
	um11 = um6;
//	mystl::unordered_map<int, int> um12;
//	um12 = std::move(um6);
	mystl::unordered_map<int, int> um13{ PAIR(1,1),PAIR(2,3),PAIR(3,3) };
	mystl::unordered_map<int, int> um14;
	um14 = { PAIR(1,1),PAIR(2,3),PAIR(3,3) };

//	MAP_FUN_AFTER(um1, um1.emplace(1, 1));
	MAP_FUN_AFTER(um1, um1.insert(PAIR(1, 1)));
	MAP_FUN_AFTER(um1, um1.insert(PAIR(2, 2)));
	MAP_FUN_AFTER(um1, um1.insert(v.begin(), v.end()));

	MAP_FUN_AFTER(um1, um1.erase(um1.begin()));
	MAP_FUN_AFTER(um1, um1.erase(um1.begin(), um1.find(5)));
	MAP_FUN_AFTER(um1, um1.erase(5));

	std::cout << std::boolalpha;
	FUN_VALUE(um1.empty());
	std::cout << std::noboolalpha;

	MAP_FUN_AFTER(um1, um1.insert(PAIR(1, 1)));
	MAP_FUN_AFTER(um1, um1.insert(PAIR(2, 2)));
	FUN_VALUE(um1.size());
	FUN_VALUE(um1.bucket_count());
	FUN_VALUE(um1.max_bucket_count());

	MAP_FUN_AFTER(um1, um1.swap(um6));

	MAP_FUN_AFTER(um1, um1.clear());

	MAP_FUN_AFTER(um1, um1.swap(um7));

	MAP_VALUE(*um1.begin());
	FUN_VALUE(um1.at(1));
	FUN_VALUE(um1[1]);

	std::cout << std::boolalpha;
	FUN_VALUE(um1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(um1.size());
	FUN_VALUE(um1.max_size());
	FUN_VALUE(um1.bucket_count());
	FUN_VALUE(um1.max_bucket_count());

	FUN_VALUE(um1.count(1));
	MAP_VALUE(*um1.find(4));

	auto first = *um1.equal_range(3).first;
	auto second = *um1.equal_range(3).second;
	std::cout << " um1.equal_range(3) : from <" << first.first << ", " << first.second
		<< "> to <" << second.first << ", " << second.second << ">" << std::endl;

	PASSED;

#if PERFORMANCE_TEST_ON
	std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|       insert        |";
#if LARGER_TEST_DATA_ON
	MAP_INSERT_TEST(unordered_map, M(LEN1), M(LEN2), M(LEN3));
#else
	MAP_INSERT_TEST(unordered_map, XL(LEN1), XL(LEN2), XL(LEN3));
#endif
	std::cout << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	PASSED;
#endif
	std::cout << "[-------------- End container test : unordered_map -------------]" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

void unordered_multimap_test() {
	std::cout << "[===============================================================]" << std::endl;
	std::cout << "[----------- Run container test : unordered_multimap -----------]" << std::endl;
	std::cout << "[-------------------------- API test ---------------------------]" << std::endl;

	mystl::vector<PAIR> v;
	for (int i = 0; i < 5; ++i)
		v.push_back(PAIR(5 - i, 5 - i));

	mystl::unordered_multimap<int, int> um1;
	mystl::unordered_multimap<int, int> um2(520);
	mystl::unordered_multimap<int, int> um3(520, mystl::hash<int>());
	mystl::unordered_multimap<int, int> um4(520, mystl::hash<int>(), mystl::equal_to<int>());
	mystl::unordered_multimap<int, int> um5(v.begin(), v.end());
	mystl::unordered_multimap<int, int> um6(v.begin(), v.end(), 100);
	mystl::unordered_multimap<int, int> um7(v.begin(), v.end(), 100, mystl::hash<int>());
	mystl::unordered_multimap<int, int> um8(v.begin(), v.end(), 100, mystl::hash<int>(), mystl::equal_to<int>());
	mystl::unordered_multimap<int, int> um9(um5);
//	mystl::unordered_multimap<int, int> um10(std::move(um5));
	mystl::unordered_multimap<int, int> um11;
	um11 = um6;
//	mystl::unordered_multimap<int, int> um12;
//	um12 = std::move(um6);
	mystl::unordered_multimap<int, int> um13{ PAIR(1,1),PAIR(2,3),PAIR(3,3) };
	mystl::unordered_multimap<int, int> um14;
	um14 = { PAIR(1,1),PAIR(2,3),PAIR(3,3) };

//	MAP_FUN_AFTER(um1, um1.emplace(1, 1));
	MAP_FUN_AFTER(um1, um1.insert(PAIR(1, 1)));
	MAP_FUN_AFTER(um1, um1.insert(PAIR(2, 2)));
	MAP_FUN_AFTER(um1, um1.insert(v.begin(), v.end()));

	MAP_FUN_AFTER(um1, um1.erase(um1.begin()));
	MAP_FUN_AFTER(um1, um1.erase(um1.begin(), um1.find(3)));
	MAP_FUN_AFTER(um1, um1.erase(3));

	std::cout << std::boolalpha;
	FUN_VALUE(um1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(um1.size());
	FUN_VALUE(um1.bucket_count());
	FUN_VALUE(um1.max_bucket_count());

	MAP_FUN_AFTER(um1, um1.clear());

	MAP_FUN_AFTER(um1, um1.swap(um7));

	MAP_VALUE(*um1.begin());

	std::cout << std::boolalpha;
	FUN_VALUE(um1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(um1.size());
	FUN_VALUE(um1.max_size());
	FUN_VALUE(um1.bucket_count());
	FUN_VALUE(um1.max_bucket_count());

	MAP_FUN_AFTER(um1, um1.insert(PAIR(1, 1)));
	MAP_FUN_AFTER(um1, um1.insert(PAIR(2, 2)));
	MAP_FUN_AFTER(um1, um1.insert(PAIR(1, 1)));
	MAP_FUN_AFTER(um1, um1.insert(PAIR(2, 2)));
	MAP_FUN_AFTER(um1, um1.insert(PAIR(1, 1)));
	FUN_VALUE(um1.elems_in_bucket(1));
	FUN_VALUE(um1.elems_in_bucket(2));
	FUN_VALUE(um1.elems_in_bucket(3));

	FUN_VALUE(um1.bucket_count());

	FUN_VALUE(um1.count(1));
	MAP_VALUE(*um1.find(3));

	auto first = *um1.equal_range(3).first;
	auto second = *um1.equal_range(3).second;
	std::cout << " um1.equal_range(3) : from <" << first.first << ", " << first.second
		<< "> to <" << second.first << ", " << second.second << ">" << std::endl;

	PASSED;

#if PERFORMANCE_TEST_ON
	std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|       insert        |";
#if LARGER_TEST_DATA_ON
	MAP_INSERT_TEST(unordered_multimap, M(LEN1), M(LEN2), M(LEN3));
#else
	MAP_INSERT_TEST(unordered_multimap, M(LEN1), M(LEN2), M(LEN3));
#endif
	std::cout << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	PASSED;
#endif
	std::cout << "[----------- End container test : unordered_multimap -----------]" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

}

}	// mystl

#endif