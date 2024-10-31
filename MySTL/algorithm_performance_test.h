#pragma once

#include <algorithm>
#include "algorithm.h"
#include "test.h"

namespace mystl {

namespace test {

namespace algorithm_performance_test {

// 函数性能测试宏定义
#define FUN_TEST1(NameSpace, fun, count) do {                  \
    std::string fun_name = #fun;                               \
    srand(static_cast<int>(time(0)));                          \
    char buf[10];                                              \
    clock_t start, end;                                        \
    int *arr = new int[count];                                 \
    for(size_t i = 0; i < count; ++i)                          \
        *(arr + i) = rand();                                   \
    start = clock();                                           \
    NameSpace::fun(arr, arr + count);                          \
    end = clock();                                             \
    int n = static_cast<int>(static_cast<double>(end - start)  \
        / CLOCKS_PER_SEC * 1000);                              \
    std::snprintf(buf, sizeof(buf), "%d", n);                  \
    std::string t = buf;                                       \
    t += "ms   |";                                             \
    std::cout << std::setw(WIDE) << t;                         \
    delete []arr;                                              \
} while(0)                                                     \

#define FUN_TEST2(NameSpace, fun, count) do {                  \
    std::string fun_name = #fun;                               \
    srand(static_cast<int>(time(0)));                          \
    char buf[10];                                              \
    clock_t start, end;                                        \
    int *arr = new int[count];                                 \
    for(size_t i = 0; i < count; ++i)                          \
        *(arr + i) = rand();                                   \
    start = clock();                                           \
    for(size_t i = 0; i < count; ++i)                          \
        NameSpace::fun(arr, arr + count, rand());              \
    end = clock();                                             \
    int n = static_cast<int>(static_cast<double>(end - start)  \
        / CLOCKS_PER_SEC * 1000);                              \
    std::snprintf(buf, sizeof(buf), "%d", n);                  \
    std::string t = buf;                                       \
    t += "ms   |";                                             \
    std::cout << std::setw(WIDE) << t;                         \
    delete []arr;                                              \
} while(0)                                                     \


void binary_search_test() {
	std::cout << "[------------------- function : binary_search ------------------]" << std::endl;
	std::cout << "| orders of magnitude |";
	PRINT_LEN(XL(LEN1), XL(LEN2), XL(LEN3), WIDE);
	std::cout << "|         std         |";
	FUN_TEST2(std, binary_search, XL(LEN1));
	FUN_TEST2(std, binary_search, XL(LEN2));
	FUN_TEST2(std, binary_search, XL(LEN3));
	std::cout << std::endl << "|        mystl        |";
	FUN_TEST2(mystl, binary_search, XL(LEN1));
	FUN_TEST2(mystl, binary_search, XL(LEN2));
	FUN_TEST2(mystl, binary_search, XL(LEN3));
	std::cout << std::endl;
}

void sort_test() {
	std::cout << "[----------------------- function : sort -----------------------]" << std::endl;
	std::cout << "| orders of magnitude |";
	PRINT_LEN(XL(LEN1), XL(LEN2), XL(LEN3), WIDE);
	std::cout << "|         std         |";
	FUN_TEST1(std, sort, XL(LEN1));
	FUN_TEST1(std, sort, XL(LEN2));
	FUN_TEST1(std, sort, XL(LEN3));
	std::cout << std::endl << "|        mystl        |";
	FUN_TEST1(mystl, sort, XL(LEN1));
	FUN_TEST1(mystl, sort, XL(LEN2));
	FUN_TEST1(mystl, sort, XL(LEN3));
	std::cout << std::endl;
}

void algorithm_performance_test() {

#if PERFORMANCE_TEST_ON
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[--------------- Run algorithm performance test ----------------]" << std::endl;
    sort_test();
    binary_search_test();
    std::cout << "[--------------- End algorithm performance test ----------------]" << std::endl;
    std::cout << "[===============================================================]" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
#endif // PERFORMANCE_TEST_ON
}

}		// algorithm_performance_test

}		// test

}		// mystl