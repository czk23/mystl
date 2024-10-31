#ifndef MYSTL_FUNCTIONAL_H
#define MYSTL_FUNCTIONAL_H

// 这个文件包含了一些函数对象

namespace mystl {

template<class Arg, class Result>
struct unary_function {
	typedef Arg							argument_type;
	typedef Result						result_type;
};

template<class Arg1, class Arg2, class Result>
struct binary_function {
	typedef Arg1						first_argument_type;
	typedef Arg2						second_argument_type;
	typedef Result						result_type;
};

template<class T>
struct plus : public binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const {
		return x + y;
	}
};

template<class T>
struct minus : public binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const {
		return x - y;
	}
};

template<class T>
struct multiplies : public binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const {
		return x * y;
	}
};

template<class T>
struct divides : public binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const {
		return x / y;
	}
};

template<class T>
struct modulus : public binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const {
		return x % y;
	}
};

template<class T>
struct negate : public unary_function<T, T> {
	T operator()(const T& x) const {
		return -x;
	}
};

template<class T>
inline T identity_element(plus<T>) {
	return T(0);
}

template<class T>
inline T identity_element(multiplies<T>) {
	return T(1);
}

template<class T>
struct equal_to : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const {
		return x == y;
	}
};

template<class T>
struct not_equal_to : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const {
		return x != y;
	}
};

template<class T>
struct greater : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const {
		return x > y;
	}
};

template<class T>
struct less : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const {
		return x < y;
	}
};

template<class T>
struct greater_equal : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const {
		return x >= y;
	}
};

template<class T>
struct less_equal : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const {
		return x <= y;
	}
};

template<class T>
struct logical_and : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const {
		return x && y;
	}
};

template<class T>
struct logical_or : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const {
		return x || y;
	}
};

template<class T>
struct logical_not : public unary_function<T, bool> {
	bool operator()(const T& x) const {
		return !x;
	}
};

template<class T>
struct identity : public unary_function<T, T> {
	const T& operator()(const T& x) const {
		return x;
	}
};

template<class Pair>
struct Select1st : public unary_function<Pair, typename Pair::first_type> {
	typename Pair::first_type operator()(const Pair& x) const {
		return x.first;
	}
};

template<class Pair>
struct Select2nd : public unary_function<Pair, typename Pair::second_type> {
	typename Pair::first_type operator()(const Pair& x) const {
		return x.second;
	}
};

template<class Predicate>
class unary_negate : public unary_function<typename Predicate::argument_type, bool> {
protected:
	Predicate pred;

public:
	explicit unary_negate(const Predicate& rhs) : pred(rhs) {};

	bool operator()(const typename Predicate::argument& x) const {
		return !pred(x);
	}
};

template<class Predicate>
inline unary_negate<Predicate> not1(const Predicate& pred) {
	return unary_negate<Predicate>(pred);
}

template<class Predicate>
class binary_negate : public binary_function<typename Predicate::first_argument_type,
	typename Predicate::second_argument_type, bool> {
protected:
	Predicate pred;

public:
	explicit binary_negate(const Predicate& rhs) : pred(rhs) {};

	bool operator()(const typename Predicate::first_argument_type& x, 
		const typename Predicate::second_argument_type& y) const {
		return !pred(x, y);
	}
};

template<class Predicate>
inline binary_negate<Predicate> not2(const Predicate& pred) {
	return binary_negate<Predicate>(pred);
}

template<class Operation>
class binder1st : public unary_function<typename Operation::second_argument_type,
	typename Operation::result_type> {
protected:
	Operation op;
	typename Operation::first_argument_type value;

public:
	binder1st(const Operation& x, const typename Operation::first_argument_type& y) : op(x), value(y) {};

	typename Operation::result_type operator()(const typename Operation::second_argument_type& x) const {
		return op(value, x);
	}
};

template<class Operation, class T>
inline binder1st<Operation> bind1st(const Operation& op, const T& x) {
	typedef typename Operation::first_argument_type		first_argument_type;
	return binder1st<Operation>(op, static_cast<first_argument_type>(x));
}

template<class Operation>
class binder2nd : public unary_function<typename Operation::first_argument_type,
	typename Operation::result_type> {
protected:
	Operation op;
	typename Operation::second_argument_type value;

public:
	binder2nd(const Operation& x, const typename Operation::second_argument_type& y) : op(x), value(y) {};

	typename Operation::result_type operator()(const typename Operation::first_argument_type& x) const {
		return op(x, value);
	}
};

template<class Operation, class T>
inline binder2nd<Operation> bind2nd(const Operation& op, const T& x) {
	typedef typename Operation::second_argument_type	second_argument_type;
	return binder2nd<Operation>(op, static_cast<second_argument_type>(x));
}

template<class Operation1, class Operation2>
class unary_compose : public unary_function<typename Operation2::argument_type, typename Operation1::result_type> {
protected:
	Operation1 fn1;
	Operation2 fn2;

public:
	unary_compose(const Operation1& op1, const Operation2& op2) : fn1(op1), fn2(op2) {};

	typename Operation1::result_type operator()(const typename Operation2::argument_type& x) const {
		return fn1(fn2(x));
	}
};

template<class Operation1, class Operation2>
inline unary_compose<Operation1, Operation2> compose1(const Operation1& op1, const Operation2& op2) {
	return unary_compose<Operation1, Operation2>(op1, op2);
}

template<class Operation1, class Operation2, class Operation3>
class binary_compose : public binary_compose<typename Operation2::argument_type, typename Operation3::argument_type,
	typename Operation1::result_type> {
protected:
	Operation1 fn1;
	Operation2 fn2;
	Operation3 fn3;

public:
	binary_compose(const Operation1& op1, const Operation2& op2, const Operation3& op3) : fn1(op1), fn2(op2), fn3(op3) {};

	typename Operation1::result_type operator()(const typename Operation2::argument_type& x,
		const typename Operation3::argument_type& y) const {
		return fn1(fn2(x), fn(y));
	}
};

template<class Operation1, class Operation2, class Operation3>
inline binary_compose<Operation1, Operation2, Operation3> compose2(const Operation1& op1, const Operation2& op2, const Operation3& op3) {
	return binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
}

template<class Arg, class Result>
class pointer_to_unary_function : public unary_function<Arg, Result> {
protected:
	Result(*ptr) (Arg);

public:
	pointer_to_unary_function() = default;

	explicit pointer_to_unary_function(Result(*rhs)(Arg)) : ptr(rhs) {};

	Result operator()(const Arg& x) const {
		return ptr(x);
	}
};

template<class Arg, class Result>
inline pointer_to_unary_function<Arg, Result> ptr_fun(Result(*x)(Arg)) {
	return pointer_to_unary_function<Arg, Result>(x);
}

template<class Arg1, class Arg2, class Result>
class pointer_to_binary_function : public binary_function<Arg1, Arg2, Result> {
protected:
	Result(*ptr)(Arg1, Arg2);

public:
	pointer_to_binary_function() = default;

	pointer_to_binary_function(Result(*rhs)(Arg1, Arg2)) : ptr(rhs) {};

	Result operator()(const Arg1& x, const Arg2& y) const {
		return ptr(x, y);
	}
};

template<class Arg1, class Arg2, class Result>
inline pointer_to_binary_function<Arg1, Arg2, Result> ptr_fun(Result(*x)(Arg1, Arg2)) {
	return pointer_to_binary_function<Arg1, Arg2, Result>(x);
}

template<class T, class Result>
class mem_fun_t : public unary_function<T*, Result> {
private:
	Result(T::* ptr)();

public:
	explicit mem_fun_t(Result(T::* rhs)()) : ptr(rhs) {};

	Result operator()(T* p) const {
		return (p->*ptr)();
	}
};

template<class T, class Result>
class const_mem_fun_t : unary_function<T*, Result> {
private:
	Result(T::*ptr)() const;

public:
	explicit const_mem_fun_t(Result(T::* rhs)() const) : ptr(rhs) {};

	Result operator()(const T* p) const {
		return (p->*ptr)();
	}
};

template<class T, class Result>
class mem_fun_ref_t : unary_function<T, Result> {
private:
	Result(T::* ptr)();

public:
	explicit mem_fun_ref_t(Result(T::* rhs)()) : ptr(rhs) {};

	Result operator()(T& p) const {
		return (p.*ptr)();
	}
};

template<class T, class Result>
class const_mem_fun_ref_t : unary_function<T, Result> {
private:
	Result(T::* ptr)() const;

public:
	explicit const_mem_fun_ref_t(Result(T::* rhs)() const) : ptr(rhs) {};

	Result operator()(const T& p) const {
		return (p.*ptr)();
	}
};

template<class T, class Arg, class Result>
class mem_fun1_t : public binary_function<T*, Arg, Result> {
private:
	Result(T::* ptr)(Arg);

public:
	explicit mem_fun1_t(Result(T::* rhs)(Arg)) : ptr(rhs) {};

	Result operator() (T* p, Arg x) const {
		return (p->*ptr)(x);
	}
};


template<class T, class Arg, class Result>
class const_mem_fun1_t : public binary_function<T*, Arg, Result> {
private:
	Result(T::* ptr)(Arg) const;

public:
	explicit const_mem_fun1_t(Result(T::* rhs)(Arg) const) : ptr(rhs) {};

	Result operator() (const T* p, Arg x) const {
		return (p->*ptr)(x);
	}
};

template<class T, class Arg, class Result>
class mem_fun1_ref_t : public binary_function<T, Arg, Result> {
private:
	Result(T::* ptr)(Arg);

public:
	explicit mem_fun1_ref_t(Result(T::* rhs)(Arg)) : ptr(rhs) {};

	Result operator() (T& p, Arg x) const {
		return (p.*ptr)(x);
	}
};

template<class T, class Arg, class Result>
class const_mem_fun1_ref_t : public binary_function<T, Arg, Result> {
private:
	Result(T::* ptr)(Arg) const;

public:
	explicit const_mem_fun1_ref_t(Result(T::* rhs)(Arg) const) : ptr(rhs) {};

	Result operator() (const T& p, Arg x) const {
		return (p.*ptr)(x);
	}
};

template<class T, class Result>
inline mem_fun_t<T, Result> mem_fun(Result(T::* f)()) {
	return mem_fun_t<T, Result>(f);
}

template<class T, class Result>
inline const_mem_fun_t<T, Result> mem_fun(Result(T::* f)() const) {
	return const_mem_fun_t<T, Result>(f);
}

template<class T, class Result>
inline mem_fun_ref_t<T, Result> mem_fun_ref(Result(T::* f)()) {
	return mem_fun_ref_t<T, Result>(f);
}

template<class T, class Result>
inline const_mem_fun_ref_t<T, Result> mem_fun_ref(Result(T::* f)()) {
	return const_mem_fun_ref_t<T, Result>(f);
}

template<class T, class Arg, class Result>
inline mem_fun1_t<T, Arg, Result> mem_fun(Result(T::* f)(Arg)) {
	return mem_fun1_t<T, Arg, Result>(f);
}

template<class T, class Arg, class Result>
inline const_mem_fun1_t<T, Arg, Result> mem_fun(Result(T::* f)() const) {
	return const_mem_fun1_t<T, Arg, Result>(f);
}

template<class T, class Arg, class Result>
inline mem_fun1_ref_t<T, Arg, Result> mem_fun_ref(Result(T::* f)()) {
	return mem_fun1_ref_t<T, Result>(f);
}

template<class T, class Arg, class Result>
inline const_mem_fun1_ref_t<T, Arg, Result> mem_fun_ref(Result(T::* f)()) {
	return const_mem_fun1_ref_t<T, Arg, Result>(f);
}

template<class T, class Arg, class Result>
inline mem_fun1_t<T, Arg, Result> mem_fun1(Result(T::* f)(Arg)) {
	return mem_fun1_t<T, Arg, Result>(f);
}

template<class T, class Arg, class Result>
inline const_mem_fun1_t<T, Arg, Result> mem_fun1(Result(T::* f)() const) {
	return const_mem_fun1_t<T, Arg, Result>(f);
}

template<class T, class Arg, class Result>
inline mem_fun1_ref_t<T, Arg, Result> mem_fun1_ref(Result(T::* f)()) {
	return mem_fun1_ref_t<T, Result>(f);
}

template<class T, class Arg, class Result>
inline const_mem_fun1_ref_t<T, Arg, Result> mem_fun1_ref(Result(T::* f)()) {
	return const_mem_fun1_ref_t<T, Arg, Result>(f);
}

}	// mystl

#endif