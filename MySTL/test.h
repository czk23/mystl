#ifndef MYSTL_TEST_H
#define MYSTL_TEST_H

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <random>
#include <ctime>
#include <sstream>

#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include "color.h"

namespace mystl {
	
namespace test {

#define green redbud::io::state::manual << redbud::io::hfg::green
#define red   redbud::io::state::manual << redbud::io::hfg::red

		// 针对Debug模式和Release模式定义不同的数量级
#if defined (DEBUG) || defined(_DEBUG)
#define LEN1 10000
#define LEN2 100000
#define LEN3 1000000
#else
#define LEN1 100000
#define LEN2 1000000
#define LEN3 10000000
#endif

// 设置输出宽度
#define WIDE 14

// 设置数据规模
#define XXL(N) (N * 20)
#define XL(N) (N * 10)
#define L(N) (N * 5)
#define M(N) (N)
#define S(N) (N / 5)
#define XS(N) (N / 10)
#define XXS(N) (N / 20)

// 打印输出框
#define PRINT_EQUAL std::cout << green << "================================================================\n"

// 打印测试成功的提示
#define PASSED std::cout << green << "[ PASSED ]\n";

class TestCase {
public:
	TestCase(const char* testname) : testcase_name(testname) {};

	virtual void Run() = 0;

public:
	const char* testcase_name;
	bool nTestResult;
	double nFailed;
	double nPassed;
};

class UnitTest {
public:
	TestCase* CurrentTestCase;
	double nPassed;
	double nFailed;

protected:
	std::vector<TestCase*> testcases;

public:
	static UnitTest* GetInstance();

	TestCase* RegisterTestCase(TestCase*);

	void Run();
};

UnitTest* UnitTest::GetInstance() {
	static UnitTest instance;
	return &instance;
}

TestCase* UnitTest::RegisterTestCase(TestCase* testcase) {
	testcases.push_back(testcase);
	return testcase;
}

void UnitTest::Run() {
	for (TestCase* it : testcases) {
		CurrentTestCase = it;
		CurrentTestCase->nTestResult = true;
		CurrentTestCase->nFailed = 0;
		CurrentTestCase->nPassed = 0;
		PRINT_EQUAL;
		std::cout << green << "Run TestCase: " << CurrentTestCase->testcase_name << "\n";
		CurrentTestCase->Run();
		if (CurrentTestCase->nTestResult) {
			std::cout << green;
		}
		else {
			std::cout << red;
		}
		std::cout << CurrentTestCase->nPassed << " / " << (CurrentTestCase->nFailed + CurrentTestCase->nPassed)
			<< " Cases passed. ( " << static_cast<int>(100 * static_cast<double>(CurrentTestCase->nPassed) /
				(CurrentTestCase->nFailed + CurrentTestCase->nPassed)) << "% )\n";
		std::cout << green << "End TestCase: " << CurrentTestCase->testcase_name << "\n";
		if (CurrentTestCase->nTestResult)
			++nPassed;
		else
			++nFailed;
	}
	PRINT_EQUAL;
	std::cout << green << "Total TestCases: " << nPassed + nFailed << "\n";
	std::cout << green << "Total Passed: " << nPassed << "\n";
	std::cout << red << "Total Failed: " << nFailed << "\n";
	std::cout << green << nPassed << " / " << (nFailed + nPassed)
		<< " Cases passed. ( " << static_cast<int>(100 * static_cast<double>(nPassed) / (nFailed + nPassed)) << "% )\n\n";
}

#define TESTCASE_NAME(testcase_name) testcase_name##_TEST

// 使用宏封装复杂的测试过程
#define TEST(testcase_name)																		\
class TESTCASE_NAME(testcase_name) : public TestCase {											\
public:																							\
	TESTCASE_NAME(testcase_name)(const char* name) : TestCase(name) {};							\
	virtual void Run();																			\
private:																						\
	static TestCase* const testcase;															\
};																								\
																								\
TestCase* const TESTCASE_NAME(testcase_name)::testcase =										\
UnitTest::GetInstance()->RegisterTestCase(new TESTCASE_NAME(testcase_name)(#testcase_name)); 	\
void TESTCASE_NAME(testcase_name)::Run()


// 注意run函数的实现，没有;也没有{}，目的是直接将单元测试加在run的后面
// 例如：
// TEST(AddTestDemo){
//    EXPECT_EQ(3, add(1, 2));
//    EXPECT_EQ(2, add(1, 1));	
// }
// 展开为
// run(){
//    EXPECT_EQ(3, add(1, 2));
// 	  EXPECT_EQ(2, add(1, 1));
// }

// 当前测试成功要做的动作
#define CURRENT_TEST_CASE_SUCCESS(testcase_name) do{						\
	std::string name = #testcase_name;										\
	UnitTest::GetInstance()->CurrentTestCase->nPassed++;					\
	std::cout << green << name << " succeed!\n";							\
}while(0)																	\

// 当前测试失败要做的动作
#define CURRENT_TEST_CASE_FAIL(testcase_name) do{							\
	std::string name = #testcase_name;										\
	UnitTest::GetInstance()->CurrentTestCase->nFailed++;					\
	UnitTest::GetInstance()->CurrentTestCase->nTestResult = false;			\
	std::cout << red << name << " failed!\n";								\
}while(0)																	\

// 打印期待数据和实际数据(单个参数)
#define PRINT_EXPECT_ACTUL(v1, v2) do{										\
	std::cout << red << "expect: " << v1 << "\n";							\
	std::cout << red << "actul:" << v2 << "\n";								\
}while(0)																	\

// 真假断言
#define EXPECT_TRUE(condition) do{											\
	if (condition){															\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_TRUE");							\
	}																		\
	else{																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_TRUE");								\
	}																		\
}while(0)																	\

#define EXPECT_FALSE(condition) do{											\
	if (!condition) {														\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_FALSE");							\
	}																		\
	else {																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_FALSE");								\
	}																		\
}while(0)																	\

// 条件断言
// 一共6种: ==, !=, >, >=, <, <=
#define EXPECT_EQ(v1, v2) do{												\
	if (v1 == v2){															\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_EQ");								\
	}																		\
	else {																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_FALSE");								\
		PRINT_EXPECT_ACTUL(v1, v2);											\
	}																		\
}while(0)																	\

#define EXPECT_NE(v1, v2) do{												\
	if (v1 != v2){															\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_NE");								\
	}																		\
	else {																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_NE");								\
		PRINT_EXPECT_ACTUL(v1, v2);											\
	}																		\
}while(0)																	\

#define EXPECT_LE(v1, v2) do{												\
	if (v1 <= v2){															\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_LE");								\
	}																		\
	else {																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_LE");								\
		PRINT_EXPECT_ACTUL(v1, v2);											\
	}																		\
}while(0)																	\

#define EXPECT_LT(v1, v2) do{												\
	if (v1 < v2){															\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_LT");								\
	}																		\
	else {																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_LT");								\
		PRINT_EXPECT_ACTUL(v1, v2);											\
	}																		\
}while(0)																	\

#define EXPECT_GE(v1, v2) do{												\
	if (v1 >= v2){															\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_GE");								\
	}																		\
	else {																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_GE");								\
		PRINT_EXPECT_ACTUL(v1, v2);											\
	}																		\
}while(0)																	\

#define EXPECT_GT(v1, v2) do{												\
	if (v1 > v2){															\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_GT");								\
	}																		\
	else {																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_GT");								\
		PRINT_EXPECT_ACTUL(v1, v2);											\
	}																		\
}while(0)																	\

// 字符串比较断言
#define EXPECT_STR_EQ(v1, v2) do{											\
	const char* a = v1;														\
	const char* b = v2;														\
	while (v1 != NULL && v2 != NULL){										\
		if (*v1 == *v2) {													\
			v1++;															\
			v2++;															\
		}																	\
		else																\
			break;															\
	}																		\
	if (NULL == v1 && NULL == v2){											\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_STR_EQ");							\
	}																		\
	else{																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_STR_EQ");							\
		std::cout << red << "expect: ";										\
		while (a)															\
			std::cout << red << *(a++);										\
		std::cout << "\n";													\
		std::cout << red << "actul: ";										\
		while (b)															\
			std::cout << red << *(b++);										\
		std::cout << "\n";													\
	}																		\
}while(0)																	\

#define EXPECT_STR_NE(v1, v2) do{											\
	const char* a = v1;														\
	const char* b = v2;														\
	while (v1 != NULL && v2 != NULL){										\
		if (*v1 == *v2) {													\
			v1++;															\
			v2++;															\
		}																	\
		else																\
			break;															\
	}																		\
	if (!(NULL == v1 && NULL == v2)){										\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_STR_NE");							\
	}																		\
	else{																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_STR_NE");							\
		std::cout << red << "expect: ";										\
		while (a)															\
			std::cout << red << *(a++);										\
		std::cout << "\n";													\
		std::cout << red << "actul: ";										\
		while (b)															\
			std::cout << red << *(b++);										\
		std::cout << "\n";													\
	}																		\
}while(0)																	\

// 指针比较断言
// 比较的是指针所指的值是否相同
// 如果要判断指针是否相同用EXPECT_EQ
#define EXPECT_PTR_EQ(p1, p2) do{											\
	if (p1 != NULL && p2 != NULL){											\
		if (*p1 == *p2){													\
			CURRENT_TEST_CASE_SUCCESS(EXPECT_PTR_EQ);						\
		}																	\
		else {																\
			CURRENT_TEST_CASE_FAIL(EXPECT_PTR_EQ);							\
			PRINT_EXPECT_ACTUL(*p1, *p2);									\
		}																	\
	}																		\
	else{																	\
		if (NULL == p1 && NULL == p2){										\
			CURRENT_TEST_CASE_SUCCESS(EXPECT_PTR_EQ);						\
		}																	\
		else{																\
			CURRENT_TEST_CASE_FAIL(EXPECT_PTR_EQ);							\
			if (NULL == p1){												\
				std::cout << red << "expect: NULL\n";						\
				std::cout << red << "actul: " << *p2 << "\n";				\
			}																\
			else {															\
				std::cout << red << "expect: " << *p1 << "\n";				\
				std::cout << red << "actul: NULL\n";						\
			}																\
		}																	\
	}																		\
}while(0)																	\

#define EXPECT_PTR_NE(v1, v2) do{											\
	if (v1 != NULL && v2 != NULL){											\
		if (*v1 != *v2){													\
			CURRENT_TEST_CASE_SUCCESS("EXPECT_PTR_NE");						\
		}																	\
		else {																\
			CURRENT_TEST_CASE_FAIL("EXPECT_PTR_NE");						\
			PRINT_EXPECT_ACTUL(*v1, *v2);									\
		}																	\
	}																		\
	else{																	\
		if (!(NULL == v1 && NULL == v2)){									\
			CURRENT_TEST_CASE_SUCCESS("EXPECT_PTR_NE");						\
		}																	\
		else{																\
			CURRENT_TEST_CASE_FAIL("EXPECT_PTR_NE");						\
			std::cout << red << "expect: NULL\n";							\
			std::cout << red << "actul: NULL\n";							\
		}																	\
	}																		\
}while(0)																	\

#define EXPECT_PTR_RANGE_EQ(v1, v2, len) do{								\
	const char* a = v1;														\
	const char* b = v2;														\
	size_t i = 0;															\
	for (; i < len; i++){													\
		if (!v1 && !v2 && *v1 == *v2){										\
			v1++;															\
			v2++;															\
		}																	\
		else																\
			break;															\
	}																		\
	if (i == len){															\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_PTR_RANGE_EQ");					\
	}																		\
	else {																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_PTR_RANGE_EQ");						\
		std::cout << red << "expect: ";										\
		if (NULL == a){														\
			std::cout << red << "NULL\n";									\
		}																	\
		else{																\
			for (size_t i = 0; i < len && a; i++){							\
				std::cout << red << *a;										\
				a++;														\
			}																\
			std::cout << "\n";												\
		}																	\
		std::cout << red << "actul: ";										\
		if (NULL == b){														\
			std::cout << red << "NULL\n";									\
		}																	\
		else{																\
			for (size_t i = 0; i < len && b; i++){							\
				std::cout << red << *b;										\
				b++;														\
			}																\
			std::cout << "\n";												\
		}																	\
	}																		\
}while(0)																	\

#define EXPECT_PTR_RANGE_NE(v1, v2, len) do{								\
	const char* a = v1;														\
	const char* b = v2;														\
	size_t i = 0;															\
	for (; i < len; i++){													\
		if (!v1 && !v2 && *v1 == *v2){										\
			v1++;															\
			v2++;															\
		}																	\
		else																\
			break;															\
	}																		\
	if (i != len){															\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_PTR_RANGE_NE");					\
	}																		\
	else {																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_PTR_RANGE_NE");						\
		std::cout << red << "expect: ";										\
		for (size_t i = 0; i < len && a; i++){								\
			std::cout << red << *a;											\
			a++;															\
		}																	\
		std::cout << "\n";													\
		std::cout << red << "actul: ";										\
		for (size_t i = 0; i < len && b; i++){								\
			std::cout << red << *b;											\
			b++;															\
		}																	\
		std::cout << "\n";													\
	}																		\
}while(0)																	\

// 容器比较断言
// 这里的容器不能是指针
#define EXPECT_CON_EQ(c1, c2) do{											\
	auto it1 = std::begin(c1);												\
	auto it2 = std::begin(c2);												\
	while (it1 != std::end(c1) && it2 != std::end(c2)){						\
		if (*it1 == *it2){													\
			it1++;															\
			it2++;															\
		}																	\
		else																\
			break;															\
	}																		\
	if (it1 == std::end(c1) && it2 == std::end(c2)){						\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_CON_EQ");							\
	}																		\
	else{																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_CON_EQ");							\
		it1 = std::begin(c1);												\
		it2 = std::begin(c2);												\
		std::cout << red << "expect: ";										\
		while (it1 != std::end(c1)){										\
			std::cout << red << *it1 << " ";								\
			it1++;															\
		}																	\
		std::cout << "\n";													\
		std::cout << red << "actul: ";										\
		while (it2 != std::end(c2)){										\
			std::cout << red << *it2 << " ";								\
			it2++;															\
		}																	\
		std::cout << "\n";													\
	}																		\
}while(0)																	\

#define EXPECT_CON_NE(c1, c2) do{											\
	auto it1 = std::begin(c1);												\
	auto it2 = std::begin(c2);												\
	while (it1 != std::end(c1) && it2 != std::end(c2)){						\
		if (*it1 == *it2){													\
			it1++;															\
			it2++;															\
		}																	\
		else																\
			break;															\
	}																		\
	if (it1 != std::end(c1) || it2 != std::end(c2)){						\
		CURRENT_TEST_CASE_SUCCESS("EXPECT_CON_NE");							\
	}																		\
	else{																	\
		CURRENT_TEST_CASE_FAIL("EXPECT_CON_NE");							\
		it1 = std::begin(c1);												\
		it2 = std::begin(c2);												\
		std::cout << red << "expect: ";										\
		while (it1 != std::end(c1)){										\
			std::cout << red << *it1 << " ";								\
			it1++;															\
		}																	\
		std::cout << "\n";													\
		std::cout << red << "actul: ";										\
		while (it2 != std::end(c2)){										\
			std::cout << red << *it2 << " ";								\
			it2++;															\
		}																	\
		std::cout << "\n";													\
	}																		\
}while(0)																	\

// 遍历输出容器
#define COUT(container) do{													\
	std::string container_name = #container;								\
	std::cout << green << container_name << ": ";							\
	for (auto it : container)												\
		std::cout << green << it << " ";									\
	std::cout << "\n";														\
}while(0)																	\

#define STR_COUT(str) do{													\
	std::string name = #str;												\
	std::cout << green << name << ": " << str << "\n";						\
}while(0)																	\

// 输出函数作用后容器的数据
// 这里需要v1这个参数是为了方便调用COUT函数输出函数作用后的结果
// 例: FUN_AFTER(v1, v1.assign(a, a + 5))
#define FUN_AFTER(con, fun) do{												\
	std::string fun_name = #fun;											\
	std::cout << green << "After " << fun_name << ": \n";					\
	fun;																	\
	COUT(con);																\
}while(0)																	\

#define STR_FUN_AFTER(str, fun) do{											\
	std::string name = #fun;												\
	std::cout << green << "After " << fun_name << ": \n";					\
	fun;																	\
	STR_COUT(str);															\
}while(0)																	\

// 输出容器调用函数的值
// 这里的函数是指容器内部实现的函数
#define FUN_VALUE(fun) do{													\
	std::string name = #fun;												\
	auto res = fun;															\
	std::cout << green << name << ": " << res << "\n";						\
}while (0)																	\

// 输出测试的数量级
// 运用stringstream实现size_t到string的类型转换
// 例: 100000   |   1000000   |    10000000   |
#define PRINT_LEN(len1, len2, len3, width) do{								\
	std::stringstream s;													\
	s << len1 << " " << len2 << " " << len3;								\
	std::string s1, s2, s3;													\
	s >> s1;																\
	s >> s2;																\
	s >> s3;																\
	s1 += "   |";															\
	std::cout << std::setw(width) << s1;									\
	s2 += "   |";															\
	std::cout << std::setw(width) << s2;									\
	s3 += "   |";															\
	std::cout << std::setw(width) << s3 << "\n";							\
}while(0)																	

// 常用测试性能的宏
// NameSpace的值为std或mystl
// con是包含了命名空间的容器名称
// 注意clock返回的是时钟周期数，要除以CLOCKS_PER_SEC才是时间
// 测试的是容器中的函数
#define PRINT_TIME(start, end) do{											\
	int n = static_cast<int>(static_cast<double>(end - start)				\
      / CLOCKS_PER_SEC * 1000);												\
	char buf[10];															\
	std::snprintf(buf, sizeof(buf), "%d", n);								\
	std::string t = buf;													\
	t += "ms    |";															\
	std::cout << std::setw(WIDE) << t;										\
}while(0)																	\

#define FUN_TEST_FORMAT1(con, fun, arg, count) do{							\
	srand(static_cast<int>(time(0)));										\
	clock_t start, end;														\
	con container;															\
	start = clock();														\
	for (size_t i = 0; i < count; i++){										\
		container.fun(arg);													\
	}																		\
	end = clock();															\
	PRINT_TIME(start, end);													\
}while(0)																	\

#define FUN_TEST_FORMAT2(con, fun, arg1, arg2, count) do{					\
	srand(static_cast<int>(time(0)));										\
	clock_t start, end;														\
	con container;															\
	start = clock();														\
	for (size_t i = 0; i < count; ++i){										\
		container.fun(container.arg1(), arg2);								\
	}																		\
	end = clock();															\
	PRINT_TIME(start, end);													\
}while(0)																	\


#define LIST_SORT_DO_TEST(NameSpace, count) do{								\
	srand(static_cast<int>(time(0)));										\
	clock_t start, end;														\
	NameSpace::list<int> l;													\
	for (size_t i = 0; i < count; i++){										\
		l.insert(l.end(), rand());											\
	}																		\
	start = clock();														\
	l.sort();																\
	end = clock();															\
	PRINT_TIME(start, end);													\
}while(0)	

#define LIST_SORT_TEST(len1, len2, len3) do {								\
	PRINT_LEN(len1, len2, len3, WIDE);										\
	std::cout << "|         std         |";				                    \
	LIST_SORT_DO_TEST(std, len1);									        \
	LIST_SORT_DO_TEST(std, len2);											\
	LIST_SORT_DO_TEST(std, len3);											\
	std::cout << "\n|        mystl        |";								\
	LIST_SORT_DO_TEST(mystl, len1);											\
	LIST_SORT_DO_TEST(mystl, len2);											\
	LIST_SORT_DO_TEST(mystl, len3);											\
} while(0)																	\

#define MAP_INSERT_DO_TEST(Namespace, con, count) do {						\
	srand(static_cast<int>(time(0)));										\
	clock_t start, end;														\
	Namespace::con<int, int> c;												\
	start = clock();														\
	for (size_t i = 0; i < count; i++){										\
		c.insert(Namespace::make_pair(rand(), rand()));						\
	}																		\
	end = clock();															\
	PRINT_TIME(start, end);													\
} while(0)

#define MAP_INSERT_TEST(con, len1, len2, len3) do {							\
	PRINT_LEN(len1, len2, len3, WIDE);										\
	std::cout << "|         std         |";				                    \
	MAP_INSERT_DO_TEST(std, con, len1);									    \
	MAP_INSERT_DO_TEST(std, con, len2);										\
	MAP_INSERT_DO_TEST(std, con, len3);										\
	std::cout << "\n|        mystl        |";								\
	MAP_INSERT_DO_TEST(mystl, con, len1);									\
	MAP_INSERT_DO_TEST(mystl, con, len2);									\
	MAP_INSERT_DO_TEST(mystl, con, len3);									\
} while(0)

/*
#define MAP_EMPLACE_TEST(NameSpace, count) do{								\
	srand(static_cast<int>(time(0)));										\
	clock_t start, end;														\
	NameSpace::map<int, int> m;												\
	start = clock();														\
	for (size_t i = 0; i < count; i++){										\
		map.emplace(rand(), rand());										\
	}																		\
	end = clock();															\
	PRINT_TIME(start, end);													\
}while(0)																	\
*/

// 用不同数量级参数完成性能测试
#define CON_TEST_P1(con, fun, arg, len1, len2, len3) do{					\
	PRINT_LEN(len1, len2, len3, WIDE);										\
	std::cout << green << "|         std         |";						\
	FUN_TEST_FORMAT1(std::con, fun, arg, len1);								\
	FUN_TEST_FORMAT1(std::con, fun, arg, len2);								\
	FUN_TEST_FORMAT1(std::con, fun, arg, len3);								\
	std::cout << "\n";														\
	std::cout << green << "|        mystl        |";						\
	FUN_TEST_FORMAT1(mystl::con, fun, arg, len1);							\
	FUN_TEST_FORMAT1(mystl::con, fun, arg, len2);							\
	FUN_TEST_FORMAT1(mystl::con, fun, arg, len3);							\
	std::cout << "\n";														\
}while(0)																	

#define CON_TEST_P2(con, fun, arg1, arg2, len1, len2, len3) do{				\
	PRINT_LEN(len1, len2, len3, WIDE);										\
	std::cout << green << "|         std         |";						\
	FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len1);						\
	FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len2);						\
	FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len3);						\
	std::cout << "\n";														\
	std::cout << green << "|        mystl        |";						\
	FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len1);					\
	FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len2);					\
	FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len3);					\
	std::cout << "\n";														\
}while(0)																	

// 开启所有测试
#define RUN_ALL_TEST() do{													\
	mystl::test::UnitTest::GetInstance()->Run();							\
}while(0)																	

// 是否开启性能测试模式
#ifndef PERFORMANCE_TEST_ON
#define PERFORMANCE_TEST_ON 1
#endif

// 是否开启大数据量测试模式
#ifndef LARGE_DATA_TEST_ON
#define LARGE_DATA_TEST_ON 0
#endif

	} // namespace test
} // namespace mystl

#endif // MYSTL_TEST_H