#ifndef MYSTL_HASH_FUN_H
#define MYSTL_HASH_FUN_H

// ����ļ������˸��ֻ������͵�hash������һ�����ܵ�hash����

namespace mystl {

template<class T>
struct hash {};

size_t stl_hash_string(const char* s) {
	unsigned long h = 0;
	for (; *s; ++s)
		h = h * 5 + *s;
	return static_cast<size_t>(h);
}

// char*ȫ�ػ�
template<>
struct hash<char*> {
	size_t operator()(const char* s) const {
		return stl_hash_string(s);
	}
};

// const char*ȫ�ػ�
template<>
struct hash<const char*> {
	size_t operator()(const char* s) const {
		return stl_hash_string(s);
	}
};

// charȫ�ػ�
template<>
struct hash<char> {
	size_t operator()(char x) const {
		return x;
	}
};

// unsigned charȫ�ػ�
template<>
struct hash<unsigned char> {
	size_t operator()(unsigned char x) const {
		return x;
	}
};

// signed charȫ�ػ�
template<>
struct hash<signed char> {
	size_t operator()(signed char x) const {
		return x;
	}
};

// wchar_tȫ�ػ�
template<>
struct hash<wchar_t> {
	size_t operator()(wchar_t x) const {
		return x;
	}
};

// char16_tȫ�ػ�
template<>
struct hash<char16_t> {
	size_t operator()(char16_t x) const {
		return x;
	}
};

// char32_tȫ�ػ�
template<>
struct hash<char32_t> {
	size_t operator()(char32_t x) const {
		return x;
	}
};

// shortȫ�ػ�
template<>
struct hash<short> {
	size_t operator()(short x) const {
		return x;
	}
};

// unsigned shortȫ�ػ�
template<>
struct hash<unsigned short> {
	size_t operator()(unsigned short x) const {
		return x;
	}
};

// intȫ�ػ�
template<>
struct hash<int> {
	size_t operator()(int x) const {
		return x;
	}
};

// unsigned intȫ�ػ�
template<>
struct hash<unsigned int> {
	size_t operator()(unsigned int x) const {
		return x;
	}
};

// longȫ�ػ�
template<>
struct hash<long> {
	size_t operator()(long x) const {
		return x;
	}
};

// unsigned longȫ�ػ�
template<>
struct hash<unsigned long> {
	size_t operator()(unsigned long x) const {
		return x;
	}
};

// long longȫ�ػ�
template<>
struct hash<long long> {
	size_t operator()(long long x) const {
		return x;
	}
};

// unsigned long longȫ�ػ�
template<>
struct hash<unsigned long long> {
	size_t operator()(unsigned long long x) const {
		return x;
	}
};

// boolȫ�ػ�
template<>
struct hash<bool> {
	size_t operator()(bool x) const {
		return x;
	}
};

// ָ��ƫ�ػ�
template<class T>
struct hash<T*> {
	size_t operator()(const T* p) const {
		return reinterpret_cast<size_t>(p);
	}
};

// fnv-1a�㷨
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

// floatȫ�ػ�
template<>
struct hash<float> {
	size_t operator()(float x) const {
		return x == 0.0f ? 0 : fnv_hash((unsigned char*)&x, sizeof(float));
	}
};

// doubleȫ�ػ�
template<>
struct hash<double> {
	size_t operator()(double x) const {
		return x == 0.0f ? 0 : fnv_hash((unsigned char*)&x, sizeof(double));
	}
};

// long doubleȫ�ػ�
template<>
struct hash<long double> {
	size_t operator()(long double x) const {
		return x == 0.0f ? 0 : fnv_hash((unsigned char*)&x, sizeof(long double));
	}
};


// ���ܵ�hash����
// ʹ������Ҫ���ݾ�����ඨ������hash����(��)������operator()��operator()����hash_val����
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