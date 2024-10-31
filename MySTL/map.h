#ifndef MYSTL_MAP_H
#define MYSTL_MAP_H

// 此文档包括map和multimap两个容器

#include <functional>
#include "mystl_alloc.h"
#include <initializer_list>
#include "rb_tree.h"
#include "functional.h"

namespace mystl{

// map，键值不允许重复
template<class Key, class T, class Compare = mystl::less<Key>, class Alloc = alloc>
class map {
	typedef Key														key_type;
	typedef T														data_type;
	typedef T														mapped_type;
	typedef Compare													key_compare;
	typedef mystl::pair<const Key, T>								value_type;


	class value_compare : public mystl::binary_function<value_type, value_type, bool> {
		friend class map<Key, T, Compare, Alloc>;

	protected:
		Compare comp;
		value_compare(Compare c) : comp(c) {};

	public:
		bool operator()(const value_type& lhs, const value_type& rhs) const {
			return comp(lhs.first, rhs.first);
		}
	};

private:
	typedef mystl::rb_tree<key_type, value_type, mystl::Select1st<value_type>, key_compare, Alloc> rb_tree_type;
	rb_tree_type tree;

public:
	typedef typename rb_tree_type::pointer									pointer;
	typedef typename rb_tree_type::const_pointer							const_pointer;
	typedef typename rb_tree_type::reference								reference;
	typedef typename rb_tree_type::const_reference							const_reference;
	typedef typename rb_tree_type::iterator									iterator;
	typedef typename rb_tree_type::const_iterator							const_iterator;
	typedef typename rb_tree_type::reverse_iterator							reverse_iterator;
	typedef typename rb_tree_type::const_reverse_iterator					const_reverse_iterator;
	typedef typename rb_tree_type::difference_type							difference_type;
	typedef typename rb_tree_type::size_type								size_type;
	typedef typename rb_tree_type::allocator_type							allocator_type;


	// 构造和复制函数
	map() : tree(Compare()) {};

	explicit map(Compare comp) : tree(comp) {};

	template<class InputIterator>
	map(InputIterator first, InputIterator last) : tree(Compare()) {
		tree.insert_unique(first, last);
	}

	template<class InputIterator>
	map(InputIterator first, InputIterator last, Compare comp) : tree(comp) {
		tree.insert_unique(first, last);
	}

	map(const std::initializer_list<value_type>& ilist) : tree(Compare()) {
		tree.insert_unique(ilist.begin(), ilist.end());
	}

	map(const map<Key, T, Compare, Alloc>& rhs) : tree(rhs.tree) {};

	map<Key, T, Compare, Alloc>& operator=(map<Key, T, Compare, Alloc>& rhs) {
		tree = rhs.tree;
		return *this;
	}

	map<Key, T, Compare, Alloc>& operator=(const std::initializer_list<value_type>& ilist) {
		tree.clear();
		tree.insert_unique(ilist.begin(), ilist.end());
		return *this;
	}


	// 接口函数
	key_compare key_comp() const {
		return key_compare();
	}

	value_compare value_comp() const {
		return value_compare(Compare());
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

	void swap(map<Key, T, Compare, Alloc>& rhs) {
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


	// map相关函数
	iterator find(const key_type& key) {
		return tree.find(key);
	}

	const_iterator find(const key_type& key) const {
		return tree.find(key);
	}

	size_type count(const key_type& key) const {
		return tree.find(key) == tree.end() ? 0 : 1;
	}

	iterator lower_bound(const key_type& key) {
		return tree.lower_bound(key);
	}

	const_iterator lower_bound(const key_type& key) const {
		return tree.lower_bound(key);
	}

	iterator upper_bound(const key_type& key) {
		return tree.upper_bound(key);
	}

	const_iterator upper_bound(const key_type& key) const {
		return tree.upper_bound(key);
	}

	mystl::pair<iterator, iterator> equal_range(const key_type& key) {
		return tree.equal_range(key);
	}

	mystl::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
		return tree.equal_range(key);
	}


	// 访问元素相关函数
	data_type& at(const key_type& key) {
		iterator it = lower_bound(key);
		if (it == end() || key_comp()(key, it->first))
			throw std::out_of_range("map<Key, T> no such element exists");
		return it->second;
	}

	const data_type& at(const key_type& key) const {
		const_iterator it = lower_bound(key);
		if (it == end() || key_comp()(key, it->first))
			throw std::out_of_range("map<Key, T> no such element exists");
		return it->second;
	}

	data_type& operator[](const key_type& key) {
		iterator it = lower_bound(key);
		if (it == end() || key_comp()(key, it->first))
			it = insert(it, value_type(key, T()));
		return it->second;
	}

// 友元函数
public:
	friend bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return lhs.tree == rhs.tree;
	}

	friend bool operator<(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return lhs.tree < rhs.tree;
	}

};


//----------------------------------------------------其他函数-------------------------------------------------------
template<class Key, class T, class Compare, class Alloc>
bool operator==(map<Key, T, Compare, Alloc>& lhs, map<Key, T, Compare, Alloc>& rhs) {
	return lhs == rhs;
}

template<class Key, class T, class Compare, class Alloc>
bool operator<(map<Key, T, Compare, Alloc>& lhs, map<Key, T, Compare, Alloc>& rhs) {
	return lhs < rhs;
}

template<class Key, class T, class Compare, class Alloc>
bool operator!=(map<Key, T, Compare, Alloc>& lhs, map<Key, T, Compare, Alloc>& rhs) {
	return !(lhs == rhs);
}

template<class Key, class T, class Compare, class Alloc>
bool operator>(map<Key, T, Compare, Alloc>& lhs, map<Key, T, Compare, Alloc>& rhs) {
	return rhs < lhs;
}

template<class Key, class T, class Compare, class Alloc>
bool operator<=(map<Key, T, Compare, Alloc>& lhs, map<Key, T, Compare, Alloc>& rhs) {
	return !(rhs < lhs);
}

template<class Key, class T, class Compare, class Alloc>
bool operator>=(map<Key, T, Compare, Alloc>& lhs, map<Key, T, Compare, Alloc>& rhs) {
	return !(lhs < rhs);
}

template<class Key, class T, class Compare, class Alloc>
void swap(map<Key, T, Compare, Alloc>& lhs, map<Key, T, Compare, Alloc>& rhs) {
	return lhs.swap(rhs);
}


// multimap，允许键值重复
template<class Key, class T, class Compare = mystl::less<Key>, class Alloc = alloc>
class multimap {
	typedef Key																key_type;
	typedef T																data_type;
	typedef T																mapped_type;
	typedef mystl::pair<const Key, T>											value_type;
	typedef Compare															key_compare;


class value_compare : public mystl::binary_function<value_type, value_type, bool> {
	friend class multimap<Key, T, Compare, Alloc>;

protected:
	Compare comp;
	value_compare(Compare c) : comp(c) {};

public:
	bool operator()(const value_type& lhs, const value_type& rhs) const {
		return comp(lhs.first, rhs.first);
	}
};

private:
	typedef rb_tree < key_type, value_type, mystl::Select1st<value_type>, Compare, Alloc > rb_tree_type;
	rb_tree_type tree;

public:
	typedef typename rb_tree_type::pointer									pointer;
	typedef typename rb_tree_type::const_pointer							const_pointer;
	typedef typename rb_tree_type::reference								reference;
	typedef typename rb_tree_type::const_reference							const_reference;
	typedef typename rb_tree_type::iterator									iterator;
	typedef typename rb_tree_type::const_iterator							const_iterator;
	typedef typename rb_tree_type::reverse_iterator							reverse_iterator;
	typedef typename rb_tree_type::const_reverse_iterator					const_reverse_iterator;
	typedef typename rb_tree_type::difference_type							difference_type;
	typedef typename rb_tree_type::size_type								size_type;
	typedef typename rb_tree_type::allocator_type							allocator_type;


	// 构造和复制函数
	multimap() : tree(Compare()) {};

	explicit multimap(Compare comp) : tree(comp) {};

	template<class InputIterator>
	multimap(InputIterator first, InputIterator last) : tree(Compare()) {
		tree.insert_equal(first, last);
	}

	template<class InputIterator>
	multimap(InputIterator first, InputIterator last, Compare comp) : tree(comp) {
		tree.insert_equal(first, last);
	}

	multimap(const std::initializer_list<value_type>& ilist) : tree(Compare()) {
		tree.insert_equal(ilist.begin(), ilist.end());
	}

	multimap(multimap<Key, T, Compare, Alloc>& rhs) : tree(rhs.tree) {};

	multimap& operator=(multimap<Key, T, Compare, Alloc>& rhs) {
		tree = rhs.tree;
		return *this;
	}

	multimap& operator=(const std::initializer_list<value_type>& ilist) {
		tree.clear();
		tree.insert_equal(ilist.begin(), ilist.end());
		return *this;
	}


	// 接口函数
	key_compare key_comp() const {
		return key_compare();
	}

	value_compare value_comp() const {
		return value_compare(tree.key_comp());
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

	void swap(multimap<Key, T, Compare, Alloc>& rhs) {
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


	// map相关函数
	iterator find(const key_type& key) {
		return tree.find(key);
	}

	const_iterator find(const key_type& key) const {
		return tree.find(key);
	}

	size_type count(const key_type& key) const {
		return tree.count(key);
	}

	iterator lower_bound(const key_type& key) {
		return tree.lower_bound(key);
	}

	const_iterator lower_bound(const key_type& key) const {
		return tree.lower_bound(key);
	}

	iterator upper_bound(const key_type& key) {
		return tree.upper_bound(key);
	}

	const_iterator upper_bound(const key_type& key) const {
		return tree.upper_bound(key);
	}

	mystl::pair<iterator, iterator> equal_range(const key_type& key) {
		return tree.equal_range(key);
	}

	mystl::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
		return tree.equal_range(key);
	}


	// 友元函数
	friend bool operator==(multimap<Key, T, Compare, Alloc>& lhs, multimap<Key, T, Compare, Alloc>& rhs) {
		return lhs.tree == rhs.tree;
	}

	friend bool operator<(multimap<Key, T, Compare, Alloc>& lhs, multimap<Key, T, Compare, Alloc>& rhs) {
		return lhs.tree < rhs.tree;
	}
};


//----------------------------------------------------其他函数-------------------------------------------------------
template<class Key, class T, class Compare, class Alloc>
bool operator==(multimap<Key, T, Compare, Alloc>& lhs, multimap<Key, T, Compare, Alloc>& rhs) {
	return lhs == rhs;
}

template<class Key, class T, class Compare, class Alloc>
bool operator<(multimap<Key, T, Compare, Alloc>& lhs, multimap<Key, T, Compare, Alloc>& rhs) {
	return lhs < rhs;
}

template<class Key, class T, class Compare, class Alloc>
bool operator!=(multimap<Key, T, Compare, Alloc>& lhs, multimap<Key, T, Compare, Alloc>& rhs) {
	return !(lhs == rhs);
}

template<class Key, class T, class Compare, class Alloc>
bool operator>(multimap<Key, T, Compare, Alloc>& lhs, multimap<Key, T, Compare, Alloc>& rhs) {
	return rhs < lhs;
}

template<class Key, class T, class Compare, class Alloc>
bool operator<=(multimap<Key, T, Compare, Alloc>& lhs, multimap<Key, T, Compare, Alloc>& rhs) {
	return !(rhs < lhs);
}

template<class Key, class T, class Compare, class Alloc>
bool operator>=(multimap<Key, T, Compare, Alloc>& lhs, multimap<Key, T, Compare, Alloc>& rhs) {
	return !(lhs < rhs);
}

template<class Key, class T, class Compare, class Alloc>
void swap(multimap<Key, T, Compare, Alloc>& lhs, multimap<Key, T, Compare, Alloc>& rhs) {
	return lhs.swap(rhs);
}

} // mystl

#endif