#ifndef MYSTL_UNORDERED_SET_H
#define MYSTL_UNORDERED_SET_H

#include "functional.h"
#include "hashtable.h"
#include "memory.h"
#include "mystl_hash_fun.h"

// 这个文件包括两个容器，unordered_set和unordered_multiset

namespace mystl {

// unordered_set
template<class Value, class HashFcn = mystl::hash<Value>, class EqualKey = mystl::equal_to<Value>, class Alloc = alloc>
class unordered_set {
private:
	typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc>					Hashtable;
	Hashtable ht;

public:
	typedef typename Hashtable::allocator_type													allocator_type;
	typedef typename Hashtable::key_type														key_type;
	typedef typename Hashtable::value_type														value_type;
	typedef typename Hashtable::hasher															hasher;
	typedef typename Hashtable::key_equal														key_equal;
	typedef typename Hashtable::size_type														size_type;
	typedef typename Hashtable::difference_type													difference_type;

	typedef typename Hashtable::pointer															pointer;
	typedef typename Hashtable::const_pointer													const_pointer;
	typedef typename Hashtable::reference														reference;
	typedef typename Hashtable::const_reference													const_reference;
	typedef typename Hashtable::iterator														iterator;
	typedef typename Hashtable::const_iterator													const_iterator;

	allocator_type get_allocator() const {
		return ht.get_allocator();
	}

	hasher hash_funct()  const {
		return ht.hash_funct();
	}

	key_equal key_eq() const {
		return ht.key_eq();
	}

	friend bool operator==(unordered_set& lhs, unordered_set& rhs);

// 构造和复制函数
	unordered_set() : ht(100, hasher(), key_equal()) {};

	explicit unordered_set(size_type n) : ht(n, hasher(), key_equal()) {};

	unordered_set(size_type n, hasher hf) : ht(n, hf, key_equal()) {};

	unordered_set(size_type n, hasher hf, key_equal eql) : ht(n, hf, eql) {};

	template<class InputIterator>
	unordered_set(InputIterator first, InputIterator last) : ht(100, hasher(), key_equal()) {
		ht.insert_unique(first, last);
	}

	template<class InputIterator>
	unordered_set(InputIterator first, InputIterator last, size_type n) : ht(n, hasher(), key_equal()) {
		ht.insert_unique(first, last);
	}

	template<class InputIterator>
	unordered_set(InputIterator first, InputIterator last, size_type n, hasher hf) : ht(n, hf, key_equal()) {
		ht.insert_unique(first, last);
	}

	template<class InputIterator>
	unordered_set(InputIterator first, InputIterator last, size_type n, hasher hf, key_equal eql) : ht(n, hf, eql) {
		ht.insert_unique(first, last);
	}

	unordered_set(const std::initializer_list<Value>& ilist) : ht(100, hasher(), key_equal()) {
		ht.insert_unique(ilist.begin(), ilist.end());
	}

	unordered_set(const std::initializer_list<Value>& ilist, size_type n) : ht(n, hasher(), key_equal()) {
		ht.insert_unique(ilist.begin(), ilist.end());
	}

	unordered_set(const std::initializer_list<Value>& ilist, size_type n, hasher hf) : ht(n, hf, key_equal()) {
		ht.insert_unique(ilist.begin(), ilist.end());
	}

	unordered_set(const std::initializer_list<Value>& ilist, size_type n, hasher hf, key_equal eql) : ht(n, hf, eql) {
		ht.insert_unique(ilist.begin(), ilist.end());
	}

	unordered_set(const unordered_set& rhs) : ht(rhs.ht) {};

	unordered_set& operator=(const unordered_set& rhs) {
		if (&rhs != this) {
			ht = rhs.ht;
		}
		return *this;
	}

	unordered_set& operator=(const std::initializer_list<Value>& ilist) {
		ht.clear();
		ht.insert_unique(ilist.begin(), ilist.end());
		return *this;
	}

	~unordered_set() = default;

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

	size_type elems_in_bucket(size_type bucket) const {
		return ht.elems_in_bucket(bucket);
	}

	void swap(unordered_set& rhs) {
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

	void erase(const iterator& it) {
		ht.erase(it);
	}

	void erase(const iterator& first, const iterator& last) {
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
template<class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(unordered_set<Value, HashFcn, EqualKey, Alloc>& lhs, unordered_set<Value, HashFcn, EqualKey, Alloc>& rhs) {
	return lhs.ht == rhs.ht;
}

template<class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator!=(unordered_set<Value, HashFcn, EqualKey, Alloc>& lhs, unordered_set<Value, HashFcn, EqualKey, Alloc>& rhs) {
	return !(lhs.ht == rhs.ht);
}

template<class Value, class HashFcn, class EqualKey, class Alloc>
inline void swap(unordered_set<Value, HashFcn, EqualKey, Alloc>& lhs, unordered_set<Value, HashFcn, EqualKey, Alloc>& rhs) {
	return lhs.swap(rhs);
}


// unordered_multiset
template<class Value, class HashFcn = mystl::hash<Value>, class EqualKey = mystl::equal_to<Value>, class Alloc = alloc>
class unordered_multiset {
private:
	typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc>					Hashtable;
	Hashtable ht;

public:
	typedef typename Hashtable::allocator_type													allocator_type;
	typedef typename Hashtable::key_type														key_type;
	typedef typename Hashtable::value_type														value_type;
	typedef typename Hashtable::hasher															hasher;
	typedef typename Hashtable::key_equal														key_equal;
	typedef typename Hashtable::size_type														size_type;
	typedef typename Hashtable::difference_type													difference_type;

	typedef typename Hashtable::pointer															pointer;
	typedef typename Hashtable::const_pointer													const_pointer;
	typedef typename Hashtable::reference														reference;
	typedef typename Hashtable::const_reference													const_reference;
	typedef typename Hashtable::iterator														iterator;
	typedef typename Hashtable::const_iterator													const_iterator;

	allocator_type get_allocator() const {
		return ht.get_allocator();
	}

	hasher hash_funct()  const {
		return ht.hash_funct();
	}

	key_equal key_eq() const {
		return ht.key_eq();
	}

	friend bool operator==(unordered_multiset& lhs, unordered_multiset& rhs);

	// 构造和复制函数
	unordered_multiset() : ht(100, hasher(), key_equal()) {};

	explicit unordered_multiset(size_type n) : ht(n, hasher(), key_equal()) {};

	unordered_multiset(size_type n, hasher hf) : ht(n, hf, key_equal()) {};

	unordered_multiset(size_type n, hasher hf, key_equal eql) : ht(n, hf, eql) {};

	template<class InputIterator>
	unordered_multiset(InputIterator first, InputIterator last) : ht(100, hasher(), key_equal()) {
		ht.insert_equal(first, last);
	}

	template<class InputIterator>
	unordered_multiset(InputIterator first, InputIterator last, size_type n) : ht(n, hasher(), key_equal()) {
		ht.insert_equal(first, last);
	}

	template<class InputIterator>
	unordered_multiset(InputIterator first, InputIterator last, size_type n, hasher hf) : ht(n, hf, key_equal()) {
		ht.insert_equal(first, last);
	}

	template<class InputIterator>
	unordered_multiset(InputIterator first, InputIterator last, size_type n, hasher hf, key_equal eql) : ht(n, hf, eql) {
		ht.insert_equal(first, last);
	}

	unordered_multiset(const std::initializer_list<Value>& ilist) : ht(100, hasher(), key_equal()) {
		ht.insert_equal(ilist.begin(), ilist.end());
	}

	unordered_multiset(const std::initializer_list<Value>& ilist, size_type n) : ht(n, hasher(), key_equal()) {
		ht.insert_equal(ilist.begin(), ilist.end());
	}

	unordered_multiset(const std::initializer_list<Value>& ilist, size_type n, hasher hf) : ht(n, hf, key_equal()) {
		ht.insert_equal(ilist.begin(), ilist.end());
	}

	unordered_multiset(const std::initializer_list<Value>& ilist, size_type n, hasher hf, key_equal eql) : ht(n, hf, eql) {
		ht.insert_equal(ilist.begin(), ilist.end());
	}

	unordered_multiset(const unordered_multiset& rhs) : ht(rhs.ht) {};

	unordered_multiset& operator=(const unordered_multiset& rhs) {
		if (&rhs != this) {
			ht = rhs.ht;
		}
		return *this;
	}

	unordered_multiset& operator=(const std::initializer_list<Value>& ilist) {
		ht.clear();
		ht.insert_equal(ilist.begin(), ilist.end());
		return *this;
	}

	~unordered_multiset() = default;

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

	size_type elems_in_bucket(size_type bucket) const {
		return ht.elems_in_bucket(bucket);
	}

	void swap(unordered_multiset rhs) {
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

	mystl::pair<iterator, bool> insert_noresize(const value_type& value) {
		return ht.insert_equal_noresize(value);
	}

	size_type erase(const key_type& key) {
		return ht.erase(key);
	}

	void erase(const iterator& it) {
		ht.erase(it);
	}

	void erase(const iterator& first, const iterator& last) {
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
template<class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(unordered_multiset<Value, HashFcn, EqualKey, Alloc>& lhs, unordered_multiset<Value, HashFcn, EqualKey, Alloc>& rhs) {
	return lhs.ht == rhs.ht;
}

template<class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator!=(unordered_multiset<Value, HashFcn, EqualKey, Alloc>& lhs, unordered_multiset<Value, HashFcn, EqualKey, Alloc>& rhs) {
	return !(lhs.ht == rhs.ht);
}

template<class Value, class HashFcn, class EqualKey, class Alloc>
inline void swap(unordered_multiset<Value, HashFcn, EqualKey, Alloc>& lhs, unordered_multiset<Value, HashFcn, EqualKey, Alloc>& rhs) {
	return lhs.swap(rhs);
}


// insert_iterator偏特化(unordered_set)
template<class Key, class HashFcn, class EqualKey, class Alloc>
class insert_iterator<unordered_set<Key, HashFcn, EqualKey, Alloc>> {
protected:
	typedef unordered_set<Key, HashFcn, EqualKey, Alloc>		Container;
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

// insert_iterator偏特化(unordered_multiset)
template<class Key, class HashFcn, class EqualKey, class Alloc>
class insert_iterator<unordered_multiset<Key, HashFcn, EqualKey, Alloc>> {
protected:
	typedef unordered_multiset<Key, HashFcn, EqualKey, Alloc>		Container;
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


}	// mystl



#endif