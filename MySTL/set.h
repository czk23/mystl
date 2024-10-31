#ifndef MYSTL_SET_H
#define MYSTL_SET_H

// 此文档包含set和multiset两个容器

#include <functional>
#include <initializer_list>
#include "mystl_alloc.h"
#include "rb_tree.h"
#include "functional.h"

namespace mystl {

// set，键值不允许重复
template<class Key, class Compare = mystl::less<Key>, class Alloc = alloc>
class set {
public:
	typedef Key															key_type;
	typedef Key															value_type;
	typedef Compare														key_compare;
	typedef Compare														value_compare;

private:
	typedef rb_tree<Key, Key, mystl::identity<Key>, Compare, Alloc>			rb_tree_type;
	rb_tree_type tree;

public:
	typedef typename rb_tree_type::pointer								pointer;
	typedef typename rb_tree_type::const_pointer						const_pointer;
	typedef typename rb_tree_type::reference							reference;
	typedef typename rb_tree_type::const_reference						const_reference;
	typedef typename rb_tree_type::iterator								iterator;
	typedef typename rb_tree_type::const_iterator						const_iterator;
	typedef typename rb_tree_type::reverse_iterator						reverse_iterator;
	typedef typename rb_tree_type::const_reverse_iterator				const_reverse_iterator;
	typedef typename rb_tree_type::size_type							size_type;
	typedef typename rb_tree_type::difference_type						difference_type;
	typedef typename rb_tree_type::allocator_type						allocator_type;

	// 构造和复制函数
	set() : tree(Compare()) {};

	explicit set(Compare& comp) : tree(comp) {};

	template<class InputIterator>
	set(InputIterator first, InputIterator last) : tree() {
		tree.insert_unique(first, last);
	}

	template<class InputIterator>
	set(InputIterator first, InputIterator last, Compare& comp) : tree(comp) {
		tree.insert_unique(first, last);
	}


	set(const std::initializer_list<value_type>& ilist) : tree() {
		tree.insert_unique(ilist.begin(), ilist.end());
	}

	set(const set<Key, Compare, Alloc>& rhs) : tree(rhs.tree) {};

	set<Key, Compare, Alloc>& operator=(set& rhs) {
		tree = rhs.tree;
		return *this;
	}

	set<Key, Compare, Alloc>& operator=(const std::initializer_list<value_type>& ilist) {
		tree.clear();
		tree.insert_unique(ilist.begin(), ilist.end());
		return *this;
	}


	// 接口函数
	key_compare key_comp() const {
		return key_compare();
	}

	value_compare value_comp() const {
		return value_compare();
	}

	allocator_type get_allocator() const{
		return tree.get_allocator();
	}


	// 迭代器相关函数
	iterator begin() {
		return tree.begin();
	}

	const_iterator begin() const {
		return tree.begin();
	}

	iterator end() {
		return tree.end();
	}

	const_iterator end() const {
		return tree.end();
	}

	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const {
		return reverse_iterator(end());
	}

	reverse_iterator rend() {
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend() const {
		return reverse_iterator(begin());
	}

	const_iterator cbegin() const {
		return begin();
	}

	const_iterator cend() const {
		return end();
	}

	const_reverse_iterator crbegin() const {
		return rbegin();
	}

	const_reverse_iterator crend() const {
		return rend();
	}


	// 容量相关函数
	bool empty() const {
		return tree.empty();
	}

	size_type size() const {
		return tree.size();
	}

	size_type max_size() const {
		return tree.max_size();
	}

	void swap(set& rhs) {
		tree.swap(rhs.tree);
	}


	// 插入删除相关函数
	mystl::pair<iterator, bool> insert(const value_type& value) {
		return tree.insert_unique(value);
	}

	iterator insert(iterator position, const value_type& value) {
		return tree.insert_unique(position, value);
	}

	template<class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		tree.insert_unique(first, last);
	}

	void erase(iterator position) {
		tree.erase(position);
	}

	size_type erase(const key_type& key) {
		return tree.erase(key);
	}

	void erase(iterator first, iterator last) {
		tree.erase(first, last);
	}

	void clear() {
		tree.clear();
	}


	// set相关操作函数
	iterator find(const key_type& key) const {
		return tree.find(key);
	}

	size_type count(const key_type& key) const {
		return find(key) == end() ? 0 : 1;
	}

	iterator lower_bound(const key_type& key) const {
		return tree.lower_bound(key);
	}

	iterator upper_bound(const key_type& key) const {
		return tree.upper_bound(key);
	}

	mystl::pair<iterator, iterator> equal_range(const key_type& key) const {
		return tree.equal_range(key);
	}

public:
	friend bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
		return lhs.tree == rhs.tree;
	}

	friend bool operator<(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
		return lhs.tree < rhs.tree;
	}
};


//-----------------------------------------------------其他相关函数-------------------------------------------------------
template<class Key, class Compare, class Alloc>
bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
	return lhs == rhs;
}

template<class Key, class Compare, class Alloc>
bool operator<(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
	return lhs < rhs;
}

template<class Key, class Compare, class Alloc>
bool operator!=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
	return !(lhs == rhs);
}

template<class Key, class Compare, class Alloc>
bool operator>(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
	return rhs < lhs;
}

template<class Key, class Compare, class Alloc>
bool operator<=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
	return !(rhs < lhs);
}

template<class Key, class Compare, class Alloc>
bool operator>=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
	return !(lhs < rhs);
}

template<class Key, class Compare, class Alloc>
void swap(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
	lhs.swap(rhs);
}


// multiset，键值允许重复
template<class Key, class Compare = mystl::less<Key>, class Alloc = alloc >
class multiset {
public:
	typedef Key														key_type;
	typedef Key														value_type;
	typedef Compare													key_compare;
	typedef Compare													value_compare;

private:
	typedef rb_tree < Key, Key, mystl::identity<Key>, Compare, Alloc > rb_tree_type;
	rb_tree_type tree;

public:
	typedef typename rb_tree_type::pointer							pointer;
	typedef typename rb_tree_type::const_pointer					const_pointer;
	typedef typename rb_tree_type::reference						reference;
	typedef typename rb_tree_type::const_reference					const_reference;
	typedef typename rb_tree_type::iterator							iterator;
	typedef typename rb_tree_type::const_iterator					const_iterator;
	typedef typename rb_tree_type::reverse_iterator					reverse_iterator;
	typedef typename rb_tree_type::const_reverse_iterator			const_reverse_iterator;
	typedef typename rb_tree_type::difference_type					difference_type;
	typedef typename rb_tree_type::size_type						size_type;
	typedef typename rb_tree_type::allocator_type					allocator_type;


// 构造和复制函数
	multiset() : tree(Compare()) {};

	explicit multiset(Compare& comp) : tree(comp) {};

	template<class InputIterator>
	multiset(InputIterator first, InputIterator last) : tree(Compare()) {
		tree.insert_equal(first, last);
	}

	template<class InputIterator>
	multiset(InputIterator first, InputIterator last, Compare comp) : tree(comp) {
		tree.insert_equal(first, last);
	}

	multiset(const std::initializer_list<Key>& ilist) : tree() {
		tree.insert_equal(ilist.begin(), ilist.end());
	}

	multiset(multiset<Key, Compare, Alloc>& rhs) : tree(rhs.tree) {}

	multiset<Key, Compare, Alloc>& operator=(multiset& rhs) {
		tree = rhs.tree;
		return *this;
	}

	multiset<Key, Compare, Alloc>& operator=(const std::initializer_list<Key>& ilist) {
		tree.clear();
		tree.insert_equal(ilist.begin(), ilist.end());
		return *this;
	}


	// 接口函数
	key_compare key_comp() const {
		return key_compare();
	}

	value_compare value_comp() const {
		return value_compare();
	}

	allocator_type get_allocator() const {
		return tree.get_allocator();
	}

	
	// 迭代器相关函数
	iterator begin() {
		return tree.begin();
	}

	const_iterator begin() const {
		return tree.begin();
	}

	iterator end() {
		return tree.end();
	}

	const_iterator end() const {
		return tree.end();
	}

	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const {
		return reverse_iterator(end());
	}

	reverse_iterator rend() {
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend() const {
		return reverse_iterator(begin());
	}

	const_iterator cbegin() const {
		return begin();
	}

	const_iterator cend() const {
		return end();
	}

	const_reverse_iterator crbegin() const {
		return rbegin();
	}

	const_reverse_iterator crend() const {
		return rend();
	}


	// 容量相关函数
	bool empty() const {
		return tree.empty();
	}

	size_type size() const {
		return tree.size();
	}

	size_type max_size() const {
		return tree.max_size();
	}

	void swap(multiset& rhs) {
		tree.swap(rhs.tree);
	}

	
	// 插入删除相关函数
	iterator insert(const value_type& value) {
		return tree.insert_equal(value);
	}

	iterator insert(iterator position, const value_type& value) {
		return tree.insert_equal(position, value);
	}

	template<class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		tree.insert_equal(first, last);
	}

	void erase(iterator position) {
		tree.erase(position);
	}

	size_type erase(const key_type& key) {
		return tree.erase(key);
	}

	void erase(iterator first, iterator last) {
		tree.erase(first, last);
	}

	void clear() {
		tree.clear();
	}


	// multiset相关操作函数
	iterator find(const key_type& key) const {
		return tree.find(key);
	}

	size_type count(const key_type& key) const {
		return tree.count(key);
	}

	iterator lower_bound(const key_type& key) const {
		return tree.lower_bound(key);
	}

	iterator upper_bound(const key_type& key) const {
		return tree.upper_bound(key);
	}

	mystl::pair<iterator, iterator> equal_range(const key_type& key) const {
		return tree.equal_range(key);
	}

	friend bool operator==(multiset<Key, Compare, Alloc>& lhs, multiset<Key, Compare, Alloc>& rhs) {
		return lhs.tree == rhs.tree;
	}

	friend bool operator<(multiset<Key, Compare, Alloc>& lhs, multiset<Key, Compare, Alloc>& rhs) {
		return lhs.tree < rhs.tree;
	}
};


//--------------------------------------------------其他相关函数-------------------------------------------------------
template<class Key, class Compare, class Alloc>
bool operator<(multiset<Key, Compare, Alloc>& lhs, multiset<Key, Compare, Alloc>& rhs) {
	return lhs < rhs;
}

template<class Key, class Compare, class Alloc>
bool operator==(multiset<Key, Compare, Alloc>& lhs, multiset<Key, Compare, Alloc>& rhs) {
	return lhs == rhs;
}

template<class Key, class Compare, class Alloc>
bool operator!=(multiset<Key, Compare, Alloc>& lhs, multiset<Key, Compare, Alloc>& rhs) {
	return !(lhs == rhs);
}

template<class Key, class Compare, class Alloc>
bool operator>(multiset<Key, Compare, Alloc>& lhs, multiset<Key, Compare, Alloc>& rhs) {
	return rhs < lhs;
}

template<class Key, class Compare, class Alloc>
bool operator<=(multiset<Key, Compare, Alloc>& lhs, multiset<Key, Compare, Alloc>& rhs) {
	return !(rhs < lhs);
}

template<class Key, class Compare, class Alloc>
bool operator>=(multiset<Key, Compare, Alloc>& lhs, multiset<Key, Compare, Alloc>& rhs) {
	return !(lhs < rhs);
}

template<class Key, class Compare, class Alloc>
void swap(multiset<Key, Compare, Alloc>& lhs, multiset<Key, Compare, Alloc>& rhs) {
	return lhs.swap(rhs);
}

} // mystl

#endif