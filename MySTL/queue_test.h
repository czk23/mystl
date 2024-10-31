#pragma once

#include "queue.h"
#include "test.h"

namespace mystl {

namespace queue_test {

void queue_print(mystl::queue<int> q) {
	while (!q.empty()) {
		std::cout << q.front() << " ";
		q.pop();
	}
	std::cout << std::endl;
}

void priority_queue_print(mystl::priority_queue<int> q) {
	while (!q.empty()) {
		std::cout << q.top() << " ";
		q.pop();
	}
	std::cout << std::endl;
}

#define QUEUE_COUT(q) do {										\
	std::string q_name = #q;									\
	std::cout << green << q_name << ":";						\
	queue_print(q);												\
} while(0)

#define QUEUE_FUN_AFTER(con, fun) do {							\
	std::string fun_name = #fun;								\
	std::cout << green <<"After " << fun_name << std::endl;		\
	fun;														\
	QUEUE_COUT(con);											\
} while(0);

#define PRIORITY_QUEUE_COUT(q) do {								\
	std::string q_name = #q;									\
	std::cout << green << q_name << ":";						\
	priority_queue_print(q);									\
} while(0)

#define PRIORITY_QUEUE_FUN_AFTER(con, fun) do {					\
	std::string fun_name = #fun;								\
	std::cout << green <<"After " << fun_name << std::endl;		\
	fun;														\
	PRIORITY_QUEUE_COUT(con);									\
} while(0);

void queue_test() {

	std::cout << "[===============================================================]" << std::endl;
	std::cout << "[----------------- Run container test : queue ------------------]" << std::endl;
	std::cout << "[-------------------------- API test ---------------------------]" << std::endl;

	int test[] = { 1, 2, 3, 4, 5 };
	mystl::deque<int> d1(5);
	mystl::queue<int> q1;
	mystl::queue<int> q2(5);
	mystl::queue<int> q3(5, 1);
	mystl::queue<int> q4(test, test + 5);
	mystl::queue<int> q5(d1);
//	mystl::queue<int> q6(std::move(d1));
	mystl::queue<int> q7(q2);
//	mystl::queue<int> q8(std::move(q2));
	mystl::queue<int> q9;
	q9 = q3;
	mystl::queue<int> q10;
//	q10 = std::move(q3);
	mystl::queue<int> q11{ 1,2,3,4,5 };
	mystl::queue<int> q12;
	q12 = { 1,2,3,4,5 };

	QUEUE_FUN_AFTER(q1, q1.push(1));
	QUEUE_FUN_AFTER(q1, q1.push(2));
	QUEUE_FUN_AFTER(q1, q1.push(3));
	QUEUE_FUN_AFTER(q1, q1.pop());

//	QUEUE_FUN_AFTER(q1, q1.emplace(4));
//	QUEUE_FUN_AFTER(q1, q1.emplace(5));

	std::cout << std::boolalpha;
	FUN_VALUE(q1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(q1.size());
	FUN_VALUE(q1.front());
	FUN_VALUE(q1.back());
	while (!q1.empty())
	{
		QUEUE_FUN_AFTER(q1, q1.pop());
	}
	QUEUE_FUN_AFTER(q1, q1.swap(q4));
	QUEUE_FUN_AFTER(q1, q1.clear());
	PASSED;

#if PERFORMANCE_TEST_ON
	std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|         push        |";
#if LARGER_TEST_DATA_ON
	CON_TEST_P1(queue<int>, push, rand(), XLL(LEN1), XLL(LEN2), XLL(LEN3));
#else
	CON_TEST_P1(queue<int>, push, rand(), XL(LEN1), XL(LEN2), XL(LEN3));
#endif
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	PASSED;
#endif
	std::cout << "[----------------- End container test : queue ------------------]" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

void priority_queue_test() {
	std::cout << "[===============================================================]" << std::endl;
	std::cout << "[------------- Run container test : priority_queue -------------]" << std::endl;
	std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
	int test[] = { 1,2,3,4,5 };
	mystl::vector<int> v1(5);
	mystl::priority_queue<int> p1;
	mystl::priority_queue<int> p2(5);
	mystl::priority_queue<int> p3(5, 1);
	mystl::priority_queue<int> p4(test, test + 5);
	mystl::priority_queue<int> p5(v1);
//	mystl::priority_queue<int> p6(std::move(v1));
	mystl::priority_queue<int> p7(p2);
//	mystl::priority_queue<int> p8(std::move(p2));
	mystl::priority_queue<int> p9;
	p9 = p3;
	mystl::priority_queue<int> p10;
//	p10 = std::move(p3);
	mystl::priority_queue<int> p11{ 1,2,3,4,5 };
	mystl::priority_queue<int> p12;
	p12 = { 1,2,3,4,5 };

	PRIORITY_QUEUE_FUN_AFTER(p1, p1.push(1));
	PRIORITY_QUEUE_FUN_AFTER(p1, p1.push(5));
	PRIORITY_QUEUE_FUN_AFTER(p1, p1.push(3));
	PRIORITY_QUEUE_FUN_AFTER(p1, p1.pop());
	PRIORITY_QUEUE_FUN_AFTER(p1, p1.push(7));
	PRIORITY_QUEUE_FUN_AFTER(p1, p1.push(2));
	PRIORITY_QUEUE_FUN_AFTER(p1, p1.push(8));

//	PRIORITY_QUEUE_FUN_AFTER(p1, p1.emplace(7));
//	PRIORITY_QUEUE_FUN_AFTER(p1, p1.emplace(2));
//	PRIORITY_QUEUE_FUN_AFTER(p1, p1.emplace(8));

	std::cout << std::boolalpha;
	FUN_VALUE(p1.empty());
	std::cout << std::noboolalpha;

	FUN_VALUE(p1.size());
	FUN_VALUE(p1.top());

	while (!p1.empty())
	{
		PRIORITY_QUEUE_FUN_AFTER(p1, p1.pop());
	}
	PRIORITY_QUEUE_FUN_AFTER(p1, p1.swap(p4));
	PRIORITY_QUEUE_FUN_AFTER(p1, p1.clear());
	PASSED;

#if PERFORMANCE_TEST_ON
	std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	std::cout << "|         push        |";
#if LARGER_TEST_DATA_ON
	CON_TEST_P1(priority_queue<int>, push, rand(), XLL(LEN1), XLL(LEN2), XLL(LEN3));
#else
	CON_TEST_P1(priority_queue<int>, push, rand(), XL(LEN1), XL(LEN2), XL(LEN3));
#endif
	std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
	PASSED;
#endif
	std::cout << "[------------- End container test : priority_queue -------------]" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}


}	// queue_test

}	// mystl