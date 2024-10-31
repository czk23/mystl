#ifndef MYSTL_HASH_FUN_H
#define MYSTL_HASH_FUN_H

// 这个文件包含了各种基础类型的hash函数和一个万能的hash函数

namespace mystl {

template<class T>
struct hash {};

size_t stl_hash_string(const char* s) {
	unsigned long h = 0;
	for (; *s; ++s)
		h = h * 5 + *s;
	return static_cast<size_t>(h);
}

// char*全特化
template<>
struct hash<char*> {
	size_t operator()(const char* s) const {
		return stl_hash_string(s);
	}
};

// const char*全特化
template<>
struct hash<const char*> {
	size_t operator()(const char* s) const {
		return stl_hash_string(s);
	}
};

// char全特化
template<>
struct hash<char> {
	size_t operator()(char x) const {
		return x;
	}
};

// unsigned char全特化
template<>
struct hash<unsigned char> {
	size_t operator()(unsigned char x) const {
		return x;
	}
};

// signed char全特化
template<>
struct hash<signed char> {
	size_t operator()(signed char x) const {
		return x;
	}
};

// wchar_t全特化
template<>
struct hash<wchar_t> {
	size_t operator()(wchar_t x) const {
		return x;
	}
};

// char16_t全特化
template<>
struct hash<char16_t> {
	size_t operator()(char16_t x) const {
		return x;
	}
};

// char32_t全特化
template<>
struct hash<char32_t> {
	size_t operator()(char32_t x) const {
		return x;
	}
};

// short全特化
template<>
struct hash<short> {
	size_t operator()(short x) const {
		return x;
	}
};

// unsigned short全特化
template<>
struct hash<unsigned short> {
	size_t operator()(unsigned short x) const {
		return x;
	}
};

// int全特化
template<>
struct hash<int> {
	size_t operator()(int x) const {
		return x;
	}
};

// unsigned int全特化
template<>
struct hash<unsigned int> {
	size_t operator()(unsigned int x) const {
		return x;
	}
};

// long全特化
template<>
struct hash<long> {
	size_t operator()(long x) const {
		return x;
	}
};

// unsigned long全特化
template<>
struct hash<unsigned long> {
	size_t operator()(unsigned long x) const {
		return x;
	}
};

// long long全特化
template<>
struct hash<long long> {
	size_t operator()(long long x) const {
		return x;
	}
};

// unsigned long long全特化
template<>
struct hash<unsigned long long> {
	size_t operator()(unsigned long long x) const {
		return x;
	}
};

// bool全特化
template<>
struct hash<bool> {
	size_t operator()(bool x) const {
		return x;
	}
};

// 指针偏特化
template<class T>
struct hash<T*> {
	size_t operator()(const T* p) const {
		return reinterpret_cast<size_t>(p);
	}
};

// fnv-1a算法
inline size_t fnv_hash(const unsigned char* first, size_t count) {
	const size_t fnv_offset = 14695981039346656037ull;
	const size_t fnv_prime = 1099511628211ull;
	size_t res = 0;
	for (size_t i = 0; i < count; ++i) {
		res ^= static_cast<size_t>(first[i]);
		res *= fnv_prime;
	}
	return res;
}

// float全特化
template<>
struct hash<float> {
	size_t operator()(float x) const {
		return x == 0.0f ? 0 : fnv_hash((unsigned char*)&x, sizeof(float));
	}
};

// double全特化
template<>
struct hash<double> {
	size_t operator()(double x) const {
		return x == 0.0f ? 0 : fnv_hash((unsigned char*)&x, sizeof(double));
	}
};

// long double全特化
template<>
struct hash<long double> {
	size_t operator()(long double x) const {
		return x == 0.0f ? 0 : fnv_hash((unsigned char*)&x, sizeof(long double));
	}
};


// 万能的hash函数
// 使用是需要根据具体的类定义具体的hash函数(类)，重载operator()，operator()调用hash_val函数
template<typename... Types>
inline size_t hash_val(const Types&... args) {
	size_t seed = 0;
	hash_val(seed, args...);
	return seed;
}

template<typename T, typename... Types>
inline void hash_val(size_t& seed, const T& val, const Types&... args) {
	hash_combine(seed, val);
	hash_val(args...);
}

template<typename T>
inline void hash_val(size_t& seed, const T& val) {
	hash_combine(seed, val);
}

template<typename T>
inline void hash_combine(size_t& seed, const T& val) {
	seed ^= hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

}	// mystl

#endif