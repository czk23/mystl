#pragma once

#include "stack.h"
#include "test.h"

namespace mystl {

namespace stack_test{

void stack_print(stack<int> s) {
	while (!s.empty()) {
		std::cout << s.top() << " ";
		s.pop();
	}
	std::cout << std::endl;
}

#define STACK_COUT(s) do {									\
	std::string s_name = #s;								\
	std::cout << green << s_name << ": ";					\
	stack_print(s);											\
} while(0)

#define STACK_FUN_AFTER(s, fun) do {						\
	std::string fun_name = #fun;							\
	std::cout << green << "After " << fun_name << "\n";		\
	fun;													\
	STACK_COUT(s);											\
} while(0)

void stack_test() {
	std::cout << "[===============================================================]" << std::endl;
	std::cout << "[----------------- Run container test : stack ------------------]" << std::endl;
	std::cout << "[-------------------------- API test ---------------------------]" << std::endl;

	int test[] = { 1,2,3,4,5 };
	mystl::deque<int> d1(5);
	mystl::stack<int> s1;
	mystl::stack<int> s2(5);
	mystl::stack<int> s3(5, 1);
	mystl::stack<int> s4(test, test + 5);
	mystl::stack<int> s5(d1);
//	mystl::stack<int> s6(std::move(d1));
	mystl::stack<int> s7(s2);
//	mystl::stack<int> s8(std::move(s2));
	mystl::stack<int> s9;
	s9 = s3;
	mystl::stack<int> s10;
//	s10 = std::move(s3);
	mystl::stack<int> s11{ 1,2,3,4,5 };
	mystl::stack<int> s12;
	s12 = { 1,2,3,4,5 };

	STACK_FUN_AFTER(s1, s1.push(1));
	STACK_FUN_AFTER(s1, s1.push(2));
	STACK_FUN_AFTER(s1, s1.push(3));
	STACK_FUN_AFTER(s1, s1.pop());
	STACK_FUN_AFTER(s1, s1.push(4));
	STACK_FUN_AFTER(s1, s1.push(5));

//	STACK_FUN_AFTER(s1, s1.emplace(4));
//	STACK_FUN_AFTER(s1, s1.emplace(5));

	std::cout << std::boolalpha;
	FUN_VALUE(s1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(s1.size());
	FUN_VALUE(s1.top());
	while (!s1.empty())
	{
		STACK_FUN_AFTER(s1, s1.pop());
	}

	STACK_FUN_AFTER(s1, s1.swap(s4));
	STACK_FUN_AFTER(s1, s1.clear());
	PASSED;

#if PERFORMANCE_TEST_ON
	std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|         push        |";
#if LARGER_TEST_DATA_ON
	CON_TEST_P1(stack<int>, push, rand(), XLL(LEN1), XLL(LEN2), XLL(LEN3));
#else
	CON_TEST_P1(stack<int>, push, rand(), XL(LEN1), XL(LEN2), XL(LEN3));
#endif
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	PASSED;
#endif
	std::cout << "[----------------- End container test : stack ------------------]" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

}	// stack_test

}	// mystl