#ifndef MYSTL_UNORDERED_MAP
#define MYSTL_UNORDERED_MAP

#include "memory.h"
#include "functional.h"
#include "hashtable.h"
#include "mystl_hash_fun.h"

// 这个文件包含了两个容器unordered_map和unordered_multimap

namespace mystl {

template<class Key, class T, class HashFcn = mystl::hash<Key>, class EqualKey = mystl::equal_to<Key>, class Alloc = alloc>
class unordered_map;

template<class Key, class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const unordered_map<Key, Value, HashFcn, EqualKey, Alloc>&,
					   const unordered_map<Key, Value, HashFcn, EqualKey, Alloc>&);

// unordered_map
template<class Key, class T, class HashFcn, class EqualKey, class Alloc>
class unordered_map {
private:
	typedef hashtable<mystl::pair<const Key, T>, Key, HashFcn, mystl::Select1st<mystl::pair<const Key, T>>, EqualKey, Alloc>	Ht;
	Ht ht;

public:
	typedef typename Ht::allocator_type												allocator_type;
	typedef typename Ht::key_type													key_type;
	typedef T																		data_type;
	typedef T																		mapped_type;
	typedef typename Ht::value_type													value_type;
	typedef typename Ht::hasher														hasher;
	typedef typename Ht::key_equal													key_equal;

	typedef typename Ht::size_type													size_type;
	typedef typename Ht::difference_type											difference_type;
	typedef typename Ht::pointer													pointer;
	typedef typename Ht::const_pointer												const_pointer;
	typedef typename Ht::reference													reference;
	typedef typename Ht::const_reference											const_reference;
	typedef typename Ht::iterator													iterator;
	typedef typename Ht::const_iterator												const_iterator;
	
	allocator_type get_allocator() const {
		return ht.get_allocator();
	}

	hasher hash_funct() const {
		return ht.hash_funct();
	}

	key_equal key_eql() const {
		return ht.key_eq();
	}

	template<class Key, class Value, class HashFcn, class EqualKey, class Alloc>
	friend bool operator==(const unordered_map<Key, Value, HashFcn, EqualKey, Alloc>&,
		const unordered_map<Key, Value, HashFcn, EqualKey, Alloc>&);

// 构造和复制函数
	unordered_map() : ht(100, hasher(), key_equal()) {};

	unordered_map(size_type n) : ht(n, hasher(), key_equal()) {};

	unordered_map(size_type n, hasher hf) : ht(n, hf, key_equal()) {};

	unordered_map(size_type n, hasher hf, key_equal eql) : ht(n, hf, eql) {};

	template<class InputIterator>
	unordered_map(InputIterator first, InputIterator last) : ht(100, hasher(), key_equal()) {
		ht.insert_unique(first, last);
	}

	template<class InputIterator>
	unordered_map(InputIterator first, InputIterator last, size_type n) : ht(n, hasher(), key_equal()) {
		ht.insert_unique(first, last);
	}

	template<class InputIterator>
	unordered_map(InputIterator first, InputIterator last, size_type n, hasher hf) : ht(n, hf, key_equal()) {
		ht.insert_unique(first, last);
	}

	template<class InputIterator>
	unordered_map(InputIterator first, InputIterator last, size_type n, hasher hf, key_equal eql) : ht(n, hf, eql) {
		ht.insert_unique(first, last);
	}

	unordered_map(const std::initializer_list<value_type>& ilist) : ht(100, hasher(), key_equal()) {
		ht.insert_unique(ilist.begin(), ilist.end());
	}

	unordered_map(const std::initializer_list<value_type>& ilist, size_type n) : ht(n, hasher(), key_equal()) {
		ht.insert_unique(ilist.begin(), ilist.end());
	}

	unordered_map(const std::initializer_list<value_type>& ilist, size_type n, hasher hf) : ht(n, hf) {
		ht.insert_unique(ilist.begin(), ilist.end());
	}

	unordered_map(const std::initializer_list<value_type>& ilist, size_type n, hasher hf, key_equal eql) : ht(n, hf, eql) {
		ht.insert_unique(ilist.begin(), ilist.end());
	}

	unordered_map(const unordered_map& rhs) : ht(rhs.ht) {};

	unordered_map& operator=(unordered_map& rhs) {
		if (*this != rhs) {
			ht = rhs.ht;
		}
		return *this;
	}

	unordered_map& operator=(const std::initializer_list<value_type>& ilist) {
		ht.clear();
		ht.insert_unique(ilist.begin(), ilist.end());
		return *this;
	}

	~unordered_map() = default;

// 迭代器相关函数
	iterator begin() {
		return ht.begin();
	}

	const_iterator begin() const {
		return ht.begin();
	}

	iterator end() {
		return ht.end();
	}

	const_iterator end() const {
		return ht.end();
	}

	const_iterator cbegin() const {
		return ht.cbegin();
	}

	const_iterator cend() const {
		return ht.cend();
	}

// 容量相关函数
	bool empty() const {
		return ht.empty();
	}

	size_type size() const {
		return ht.size();
	}

	size_type max_size() const {
		return ht.max_size();
	}

	void resize(size_type n) {
		ht.resize(n);
	}

	size_type bucket_count() const {
		return ht.bucket_count();
	}

	size_type max_bucket_count() const {
		return ht.max_bucket_count();
	}

	size_type elems_in_bucket() const {
		return ht.elems_in_bucket();
	}

	void swap(unordered_map& rhs) {
		ht.swap(rhs.ht);
	}

// 插入删除相关函数
	mystl::pair<iterator, bool> insert(const value_type& value) {
		return ht.insert_unique(value);
	}

	template<class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		ht.insert_unique(first, last);
	}

	void insert(const value_type* first, const value_type* last) {
		ht.insert_unique(first, last);
	}

	void insert(const_iterator first, const_iterator last) {
		ht.insert_unique(first, last);
	}

	mystl::pair<iterator, bool> insert_noresize(const value_type& value) {
		return ht.insert_unique_noresize(value);
	}

	size_type erase(const key_type& key) {
		return ht.erase(key);
	}

	void erase(iterator it) {
		ht.erase(it);
	}

	void erase(iterator first, iterator last) {
		ht.erase(first, last);
	}

	void clear() {
		ht.clear();
	}

// 查找相关函数
	iterator find(const key_type& key) {
		return ht.find(key);
	}

	const_iterator find(const key_type& key) const {
		return ht.find(key);
	}

	size_type count(const key_type& key) const {
		return ht.count(key);
	}

	mystl::pair<iterator, iterator> equal_range(const key_type& key) {
		return ht.equal_range(key);
	}

	mystl::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
		return ht.equal_range(key);
	}

	T& operator[](const key_type& key) {
		return ht.find_or_insert(value_type(key, T())).second;
	}

	T& at(const key_type& key) {
		iterator it = find(key);
		if (nullptr == it.node)
			throw std::out_of_range("unordered_map<Key, T> no such element exists");
		return it->second;
	}

	const T& at(const key_type& key) const {
		const_iterator it = find(key);
		if (nullptr == it.node)
			throw std::out_of_range("unordered_map<Key, T> no such element exists");
		return it->second;
	}
};

// 其他函数
template<class Key, class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const unordered_map<Key, Value, HashFcn, EqualKey, Alloc>& lhs,
	const unordered_map<Key, Value, HashFcn, EqualKey, Alloc>& rhs) {
	return lhs.ht == rhs.ht;
}

template<class Key, class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator!=(const unordered_map<Key, Value, HashFcn, EqualKey, Alloc>& lhs,
	const unordered_map<Key, Value, HashFcn, EqualKey, Alloc>& rhs) {
	return !(lhs == rhs);
}

template<class Key, class Value, class HashFcn, class EqualKey, class Alloc>
inline void swap(unordered_map<Key, Value, HashFcn, EqualKey, Alloc>& lhs, unordered_map<Key, Value, HashFcn, EqualKey, Alloc>& rhs) {
	return lhs.swap(rhs.ht);
}


// unordered_multimap
template<class Key, class T, class HashFcn = mystl::hash<Key>, class EqualKey = mystl::equal_to<Key>, class Alloc = alloc>
class unordered_multimap;

template<class Key, class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const unordered_multimap<Key, Value, HashFcn, EqualKey, Alloc>& lhs,
	const unordered_multimap<Key, Value, HashFcn, EqualKey, Alloc>& rhs);


template<class Key, class T, class HashFcn, class EqualKey, class Alloc>
class unordered_multimap {
private:
	typedef hashtable<mystl::pair<const Key, T>, Key, HashFcn, mystl::Select1st<mystl::pair<const Key, T>>, EqualKey, Alloc>	Ht;
	Ht ht;

public:
	typedef typename Ht::allocator_type												allocator_type;
	typedef typename Ht::key_type													key_type;
	typedef T																		data_type;
	typedef T																		mapped_type;
	typedef typename Ht::value_type													value_type;
	typedef typename Ht::hasher														hasher;
	typedef typename Ht::key_equal													key_equal;

	typedef typename Ht::size_type													size_type;
	typedef typename Ht::difference_type											difference_type;
	typedef typename Ht::pointer													pointer;
	typedef typename Ht::const_pointer												const_pointer;
	typedef typename Ht::reference													reference;
	typedef typename Ht::const_reference											const_reference;
	typedef typename Ht::iterator													iterator;
	typedef typename Ht::const_iterator												const_iterator;

	allocator_type get_allocator() const {
		return ht.get_allocator();
	}

	hasher hash_funct() const {
		return ht.hash_funct();
	}

	key_equal key_eql() const {
		return ht.key_eq();
	}

	template<class Key, class Value, class HashFcn, class EqualKey, class Alloc>
	friend bool operator==(const unordered_multimap<Key, Value, HashFcn, EqualKey, Alloc>&,
		const unordered_multimap<Key, Value, HashFcn, EqualKey, Alloc>&);

// 构造和复制函数
	unordered_multimap() : ht(100, hasher(), key_equal()) {};

	unordered_multimap(size_type n) : ht(n, hasher(), key_equal()) {};

	unordered_multimap(size_type n, hasher hf) : ht(n, hf, key_equal()) {};

	unordered_multimap(size_type n, hasher hf, key_equal eql) : ht(n, hf, eql) {};

	template<class InputIterator>
	unordered_multimap(InputIterator first, InputIterator last) : ht(100, hasher(), key_equal()) {
		ht.insert_equal(first, last);
	}

	template<class InputIterator>
	unordered_multimap(InputIterator first, InputIterator last, size_type n) : ht(n, hasher(), key_equal()) {
		ht.insert_equal(first, last);
	}

	template<class InputIterator>
	unordered_multimap(InputIterator first, InputIterator last, size_type n, hasher hf) : ht(n, hf, key_equal()) {
		ht.insert_equal(first, last);
	}

	template<class InputIterator>
	unordered_multimap(InputIterator first, InputIterator last, size_type n, hasher hf, key_equal eql) : ht(n, hf, eql) {
		ht.insert_equal(first, last);
	}

	unordered_multimap(const std::initializer_list<value_type>& ilist) : ht(100, hasher(), key_equal()) {
		ht.insert_equal(ilist.begin(), ilist.end());
	}

	unordered_multimap(const std::initializer_list<value_type>& ilist, size_type n) : ht(n, hasher(), key_equal()) {
		ht.insert_equal(ilist.begin(), ilist.end());
	}

	unordered_multimap(const std::initializer_list<value_type>& ilist, size_type n, hasher hf) : ht(n, hf) {
		ht.insert_equal(ilist.begin(), ilist.end());
	}

	unordered_multimap(const std::initializer_list<value_type>& ilist, size_type n, hasher hf, key_equal eql) : ht(n, hf, eql) {
		ht.insert_equal(ilist.begin(), ilist.end());
	}

	unordered_multimap(const unordered_multimap& rhs) : ht(rhs.ht) {};

	unordered_multimap& operator=(unordered_multimap& rhs) {
		if (*this != rhs) {
			ht = rhs.ht;
		}
		return *this;
	}

	unordered_multimap& operator=(const std::initializer_list<value_type>& ilist) {
		ht.clear();
		ht.insert_unique(ilist.begin(), ilist.end());
		return *this;
	}

	~unordered_multimap() = default;

// 迭代器相关函数
	iterator begin() {
		return ht.begin();
	}

	const_iterator begin() const {
		return ht.begin();
	}

	iterator end() {
		return ht.end();
	}

	const_iterator end() const {
		return ht.end();
	}

	const_iterator cbegin() const {
		return ht.cbegin();
	}

	const_iterator cend() const {
		return ht.cend();
	}

// 容量相关函数
	bool empty() const {
		return ht.empty();
	}

	size_type size() const {
		return ht.size();
	}

	size_type max_size() const {
		return ht.max_size();
	}

	void resize(size_type n) {
		ht.resize(n);
	}

	size_type bucket_count() const {
		return ht.bucket_count();
	}

	size_type max_bucket_count() const {
		return ht.max_bucket_count();
	}

	size_type elems_in_bucket(size_type n) const {
		return ht.elems_in_bucket(n);
	}

	void swap(unordered_multimap& rhs) {
		ht.swap(rhs.ht);
	}

// 插入删除相关函数
	iterator insert(const value_type& value) {
		return ht.insert_equal(value);
	}

	template<class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		ht.insert_equal(first, last);
	}

	void insert(const value_type* first, const value_type* last) {
		ht.insert_equal(first, last);
	}

	void insert(const_iterator first, const_iterator last) {
		ht.insert_equal(first, last);
	}

	iterator insert_noresize(const value_type& value) {
		return ht.insert_equal_noresize(value);
	}

	size_type erase(const key_type& key) {
		return ht.erase(key);
	}

	void erase(iterator it) {
		ht.erase(it);
	}

	void erase(iterator first, iterator last) {
		ht.erase(first, last);
	}

	void clear() {
		ht.clear();
	}

// 查找相关函数
	iterator find(const key_type& key) {
		return ht.find(key);
	}

	const_iterator find(const key_type& key) const {
		return ht.find(key);
	}

	size_type count(const key_type& key) const {
		return ht.count(key);
	}

	mystl::pair<iterator, iterator> equal_range(const key_type& key) {
		return ht.equal_range(key);
	}

	mystl::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
		return ht.equal_range(key);
	}
};

// 其他函数
template<class Key, class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const unordered_multimap<Key, Value, HashFcn, EqualKey, Alloc>& lhs,
	const unordered_multimap<Key, Value, HashFcn, EqualKey, Alloc>& rhs) {
	return lhs.ht == rhs.ht;
}

template<class Key, class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator!=(const unordered_multimap<Key, Value, HashFcn, EqualKey, Alloc>& lhs,
	const unordered_multimap<Key, Value, HashFcn, EqualKey, Alloc>& rhs) {
	return !(lhs == rhs);
}

template<class Key, class Value, class HashFcn, class EqualKey, class Alloc>
inline void swap(unordered_multimap<Key, Value, HashFcn, EqualKey, Alloc>& lhs, 
	unordered_multimap<Key, Value, HashFcn, EqualKey, Alloc>& rhs) {
	return lhs.swap(rhs.ht);
}


// insert_iterator偏特化(unordered_map)
template<class Key, class T, class HashFcn, class EqualKey, class Alloc>
class insert_iterator<unordered_map<Key, T, HashFcn, EqualKey, Alloc>> {
protected:
	typedef unordered_map<Key, HashFcn, EqualKey, Alloc>		Container;
	Container* container;

public:
	typedef Container											container_type;
	typedef output_iterator_tag									iterator_category;
	typedef void												value_type;
	typedef void												difference_type;
	typedef void												pointer;
	typedef void												reference;

	insert_iterator(Container& x) : container(x) {};

	insert_iterator(Container& x, typename Container::iterator) : container(x) {};

	insert_iterator<Container>& operator=(const typename Container::value_type& value) {
		container->insert(value);
		return *this;
	}

	insert_iterator<Container>& operator*() {
		return *this;
	}

	insert_iterator<Container>& operator++() {
		return *this;
	}

	insert_iterator<Container>& operator++(int) {
		return *this;
	}
};

// insert_iterator偏特化(unordered_multimap)
template<class Key, class T, class HashFcn, class EqualKey, class Alloc>
class insert_iterator<unordered_multimap<Key, T, HashFcn, EqualKey, Alloc>> {
protected:
	typedef unordered_multimap<Key, HashFcn, EqualKey, Alloc>		Container;
	Container* container;

public:
	typedef Container												container_type;
	typedef output_iterator_tag										iterator_category;
	typedef void													value_type;
	typedef void													difference_type;
	typedef void													pointer;
	typedef void													reference;

	insert_iterator(Container& x) : container(x) {};

	insert_iterator(Container& x, typename Container::iterator) : container(x) {};

	insert_iterator<Container>& operator=(const typename Container::value_type& value) {
		container->insert(value);
		return *this;
	}

	insert_iterator<Container>& operator*() {
		return *this;
	}

	insert_iterator<Container>& operator++() {
		return *this;
	}

	insert_iterator<Container>& operator++(int) {
		return *this;
	}
};


}


#endif