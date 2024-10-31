#ifndef MYSTL_TYPE_TRAITS_H
#define MYSTL_TYPE_TRAITS_H

#include <type_traits>

namespace mystl {

	// 两个类型用来进行参数推导
	struct true_type {};
	struct false_type {};

	// 判断传入的类型是否是整数
	template<class T>
	struct Is_integer {
		typedef false_type value;
	};

	template<>
	struct Is_integer<bool> {
		typedef true_type value;
	};

	template<>
	struct Is_integer<char> {
		typedef true_type value;
	};

	template<>
	struct Is_integer<signed char> {
		typedef true_type value;
	};

	template<>
	struct Is_integer<unsigned char> {
		typedef true_type value;
	};

	template<>
	struct Is_integer<wchar_t> {
		typedef true_type value;
	};

	template<>
	struct Is_integer<short> {
		typedef true_type value;
	};

	template<>
	struct Is_integer<unsigned short> {
		typedef true_type value;
	};

	template<>
	struct Is_integer<int> {
		typedef true_type value;
	};

	template<>
	struct Is_integer<unsigned int> {
		typedef true_type value;
	};

	template<>
	struct Is_integer<long> {
		typedef true_type value;
	};

	template<>
	struct Is_integer<unsigned long> {
		typedef true_type value;
	};

	template<>
	struct Is_integer<long long> {
		typedef true_type value;
	};

	template<>
	struct Is_integer<unsigned long long> {
		typedef true_type value;
	};

	// type_traits模板
	template<class T>
	struct type_traits {
		typedef true_type		this_dummy_member_must_be_first;
		typedef false_type		has_trivial_default_constructor;
		typedef false_type		has_trivial_copy_constructor;
		typedef false_type		has_trivial_assignment_operator;
		typedef false_type		has_trivial_destructor;
		typedef false_type		is_POD_type;
	};

	// char全特化
	template<>
	struct type_traits<char> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// signed char全特化
	template<>
	struct type_traits<signed char> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// unsigned char全特化
	template<>
	struct type_traits<unsigned char> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// wchar_t全特化
	template<>
	struct type_traits<wchar_t> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// short全特化
	template<>
	struct type_traits<short> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// unsigned short全特化
	template<>
	struct type_traits<unsigned short> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// int全特化
	template<>
	struct type_traits<int> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// unsigned int全特化
	template<>
	struct type_traits<unsigned int> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// long全特化
	template<>
	struct type_traits<long> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// unsigned long全特化
	template<>
	struct type_traits<unsigned long> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// long long全特化
	template<>
	struct type_traits<long long> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// unsigned long long全特化
	template<>
	struct type_traits<unsigned long long> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// float全特化
	template<>
	struct type_traits<float> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// double全特化
	template<>
	struct type_traits<double> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// long double全特化
	template<>
	struct type_traits<long double> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// char*全特化
	template<>
	struct type_traits<char*> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// signed char*全特化
	template<>
	struct type_traits<signed char*> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// unsigned char*全特化
	template<>
	struct type_traits<unsigned char*> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// const char*全特化
	template<>
	struct type_traits<const char*> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// const signed char*全特化
	template<>
	struct type_traits<const signed char*> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};

	// const unsigned char*全特化
	template<>
	struct type_traits<const unsigned char*> {
		typedef true_type		has_trivial_default_constructor;
		typedef true_type		has_trivial_copy_constructor;
		typedef true_type		has_trivial_assignment_operator;
		typedef true_type		has_trivial_destructor;
		typedef true_type		is_POD_type;
	};


}	// mystl

#endif