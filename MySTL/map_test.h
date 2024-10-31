#ifndef MYSTL_MAP_TEST_H
#define MYSTL_MAP_TEST_H

#include "test.h"
#include "rb_tree.h"
#include "mystl_pair.h"
#include "map.h"
#include <iostream>

namespace mystl {

namespace map_test {

#define PAIR	mystl::pair<int, int>

#define MAP_COUT(m) do {																	\
	std::string m_name = #m;																\
	std::cout << m_name << ": ";															\
	for (auto it : m)																		\
		std::cout << green << "<" << it.first << ", " << it.second << "> ";					\
	std::cout << std::endl;																	\
} while(0)

#define MAP_FUN_AFTER(m, fun) do {															\
	std::string fun_name = #fun;															\
	std::cout << green << "After " << fun_name << ": \n";									\
	fun;																					\
	MAP_COUT(m);																			\
} while(0)

#define MAP_VALUE(fun) do {																	\
	std::string fun_name = #fun;															\
	auto it = fun;																			\
	std::cout << green << fun_name << ": <" << it.first << ", " << it.second << ">\n";		\
} while(0)

void map_test() {
	std::cout << "[===============================================================]" << std::endl;
	std::cout << "[------------------ Run container test : map -------------------]" << std::endl;
	std::cout << "[-------------------------- API test ---------------------------]" << std::endl;

	mystl::vector<PAIR> v;
	for (int i = 0; i < 5; ++i)
		v.push_back(PAIR(i, i));

	mystl::map<int, int> m1;
	mystl::map<int, int, mystl::greater<int>> m2;
	mystl::map<int, int> m3(v.begin(), v.end());
	mystl::map<int, int> m4(v.begin(), v.end());
	mystl::map<int, int> m5(m3);


//	mystl::map<int, int> m6(std::move(m3));
	mystl::map<int, int> m7;
	m7 = m4;
	mystl::map<int, int> m8;
//	m8 = std::move(m4);

	mystl::map<int, int> m9{ PAIR(1,1), PAIR(3,2), PAIR(2,3) };


	mystl::map<int, int> m10;
	m10 = { PAIR(1,1), PAIR(3,2), PAIR(2,3) };

//	for (int i = 5; i > 0; --i)
//	{
//		MAP_FUN_AFTER(m1, m1.emplace(i, i));
//	}


	for (int i = 5; i >= 0; --i)
		MAP_FUN_AFTER(m1, m1.insert(PAIR(i, i)));

//	MAP_FUN_AFTER(m1, m1.emplace_hint(m1.begin(), 0, 0));
	MAP_FUN_AFTER(m1, m1.erase(m1.begin()));
	MAP_FUN_AFTER(m1, m1.erase(1));
	MAP_FUN_AFTER(m1, m1.erase(2));
	MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.end()));

	MAP_FUN_AFTER(m1, m1.insert(v.begin(), v.end()));
	MAP_FUN_AFTER(m1, m1.insert(m1.end(), PAIR(5, 5)));

	FUN_VALUE(m1.count(1));

	MAP_VALUE(*m1.find(3));
	MAP_VALUE(*m1.lower_bound(3));
	MAP_VALUE(*m1.upper_bound(2));
	auto first = *m1.equal_range(2).first;
	auto second = *m1.equal_range(2).second;
	std::cout << " m1.equal_range(2) : from <" << first.first << ", " << first.second
		<< "> to <" << second.first << ", " << second.second << ">" << std::endl;

	MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.find(3)));
	MAP_FUN_AFTER(m1, m1.clear());

	MAP_FUN_AFTER(m1, m1.swap(m9));

	MAP_VALUE(*m1.begin());
	MAP_VALUE(*m1.rbegin());

	FUN_VALUE(m1[1]);
	MAP_FUN_AFTER(m1, m1[1] = 3);
	FUN_VALUE(m1.at(1));

	std::cout << std::boolalpha;
	FUN_VALUE(m1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(m1.size());
	FUN_VALUE(m1.max_size());

	PASSED;
#if PERFORMANCE_TEST_ON
	std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|       insert        |";
#if LARGER_TEST_DATA_ON
	MAP_INSERT_TEST(map, XL(LEN1), XL(LEN2), XL(LEN3));
#else
	MAP_INSERT_TEST(map, M(LEN1), M(LEN2), M(LEN3));
#endif
	std::cout << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	PASSED;
#endif
	std::cout << "[------------------ End container test : map -------------------]" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

void multimap_test() {
	std::cout << "[===============================================================]" << std::endl;
	std::cout << "[---------------- Run container test : multimap ----------------]" << std::endl;
	std::cout << "[-------------------------- API test ---------------------------]" << std::endl;

	mystl::vector<PAIR> v;
	for (int i = 0; i < 5; ++i)
		v.push_back(PAIR(i, i));
	mystl::multimap<int, int> m1;
	mystl::multimap<int, int, mystl::greater<int>> m2;
	mystl::multimap<int, int> m3(v.begin(), v.end());
	mystl::multimap<int, int> m4(v.begin(), v.end());
	mystl::multimap<int, int> m5(m3);
//	mystl::multimap<int, int> m6(std::move(m3));
	mystl::multimap<int, int> m7;
	m7 = m4;
//	mystl::multimap<int, int> m8;
//	m8 = std::move(m4);
	mystl::multimap<int, int> m9{ PAIR(1,1),PAIR(3,2),PAIR(2,3) };
	mystl::multimap<int, int> m10;
	m10 = { PAIR(1,1),PAIR(3,2),PAIR(2,3) };

	for (int i = 5; i >= 0; --i)
	{
		MAP_FUN_AFTER(m1, m1.insert(PAIR(i, i)));
	}
	MAP_FUN_AFTER(m1, m1.erase(m1.begin()));
	MAP_FUN_AFTER(m1, m1.erase(1));
	MAP_FUN_AFTER(m1, m1.erase(2));
	MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.end()));

	MAP_FUN_AFTER(m1, m1.insert(v.begin(), v.end()));
	MAP_FUN_AFTER(m1, m1.insert(PAIR(5, 5)));
	MAP_FUN_AFTER(m1, m1.insert(m1.end(), PAIR(5, 5)));

	FUN_VALUE(m1.count(5));
	MAP_VALUE(*m1.find(5));

	MAP_VALUE(*m1.lower_bound(3));
	MAP_VALUE(*m1.upper_bound(2));
	auto first = *m1.equal_range(2).first;
	auto second = *m1.equal_range(2).second;
	std::cout << " m1.equal_range(2) : from <" << first.first << ", " << first.second
		<< "> to <" << second.first << ", " << second.second << ">" << std::endl;

	MAP_FUN_AFTER(m1, m1.erase(m1.begin()));
	MAP_FUN_AFTER(m1, m1.erase(1));
	MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.find(4)));
	MAP_FUN_AFTER(m1, m1.clear());

	MAP_FUN_AFTER(m1, m1.swap(m9));

	MAP_FUN_AFTER(m1, m1.insert(PAIR(3, 3)));

	MAP_VALUE(*m1.begin());
	MAP_VALUE(*m1.rbegin());

	std::cout << std::boolalpha;
	FUN_VALUE(m1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(m1.size());
	FUN_VALUE(m1.max_size());

	PASSED;

#if PERFORMANCE_TEST_ON
	std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|       insert        |";
#if LARGER_TEST_DATA_ON
	MAP_INSERT_TEST(multimap, M(LEN1), M(LEN2), M(LEN3));
#else
	MAP_INSERT_TEST(multimap, M(LEN1), M(LEN2), M(LEN3));
#endif
	std::cout << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	PASSED;
#endif
	std::cout << "[---------------- End container test : multimap ----------------]" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

}

}		// map_test

}		// mystl


#endif