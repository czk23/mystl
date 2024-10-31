#ifndef MYSTL_PAIR_H
#define MYSTL_PAIR_H

namespace mystl {


// Ä£°åÀàpair
template<class T1, class T2>
struct pair{
	typedef	T1						first_type;
	typedef T2						second_type;

	first_type first;
	second_type second;

	pair() : first(T1()), second(T2()) {};

	pair(const T1& a, const T2& b) : first(a), second(b) {};

	template<class U1, class U2>
	pair(const pair<U1, U2>& rhs) : first(rhs.first), second(rhs.second) {};
};

template<class T1, class T2>
inline bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
	return lhs.first == rhs.first && lhs.second == rhs.second;
}

template<class T1, class T2>
inline bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
	return lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second == rhs.second);
}

template<class T1, class T2>
inline bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
	return !(lhs == rhs);
}

template<class T1, class T2>
inline bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
	return rhs < lhs;
}

template<class T1, class T2>
inline bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
	return !(rhs < lhs);
}

template<class T1, class T2>
inline bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
	return !(lhs < rhs);
}

template<class T1, class T2>
inline pair<T1, T2> make_pair(const T1& a, const T2& b) {
	return pair<T1, T2>(a, b);
}



} // mystl


#endif