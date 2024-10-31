#ifndef MYSTL_HASHTABLE_H
#define MYSTL_HASHTABLE_H

#include "iterator_base.h"
#include "memory.h"
#include "algobase.h"
#include "algo.h"
#include "mystl_pair.h"
#include "vector.h"

// 这里使用开链法实现哈希表
namespace mystl {

// hashtable结点
template<class T>
struct hashtable_node {
	T value;
	hashtable_node<T>* next;

	hashtable_node() = default;

	hashtable_node(T& n) : value(n), next(nullptr) {};

	hashtable_node(const hashtable_node<T>& rhs) : value(rhs.value), next(rhs.next) {};
};

// 向前声明
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = alloc>
struct hashtable_iterator;

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = alloc>
struct hashtable_const_iterator;

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = alloc>
struct hashtable;


// hashtable迭代器(base版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = alloc>
struct hashtable_iterator_base : public mystl::iterator<forward_iterator_tag, Value> {
	typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>							hashtable;
	typedef hashtable_iterator_base<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>			base_iterator;
	typedef hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>				iterator;
	typedef hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>			const_iterator;
	typedef hashtable_node<Value>*																node_ptr;
	typedef const hashtable_node<Value>*														const_node_ptr;
	typedef hashtable*																			container_ptr;
	typedef const hashtable*																	const_container_ptr;
	
	typedef size_t																				size_type;
	typedef ptrdiff_t																			difference_type;

	node_ptr node;
	container_ptr ht;

	hashtable_iterator_base() = default;

	bool operator==(const base_iterator& rhs) const {
		return node == rhs.node;
	}

	bool operator!=(const base_iterator& rhs) const {
		return !(node == rhs.node);
	}
};


// hashtable迭代器
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
struct hashtable_iterator : public hashtable_iterator_base<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> {
	typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>							hashtable;
	typedef hashtable_iterator_base<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>			base_iterator;
	typedef hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>				iterator;
	typedef hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>			const_iterator;
	typedef hashtable_node<Value>*																node_ptr;
	typedef const hashtable_node<Value>*														const_node_ptr;
	typedef hashtable*																			container_ptr;
	typedef const hashtable*																	const_container_ptr;

	typedef size_t																				size_type;
	typedef ptrdiff_t																			difference_type;
	typedef Value&																				reference;
	typedef const Value&																		const_reference;
	typedef	Value*																				pointer;
	typedef const Value*																		const_pointer;

	using base_iterator::node;
	using base_iterator::ht;

	// 复制和构造函数
	hashtable_iterator() = default;

	hashtable_iterator(node_ptr n, container_ptr tab) {
		node = n;
		ht = tab;
	};

	hashtable_iterator(const iterator& rhs) {
		node = rhs.node;
		ht = rhs.ht;
	};

	hashtable_iterator(const const_iterator& rhs) {
		node = rhs.node;
		ht = rhs.ht;
	};

	iterator& operator=(const iterator& rhs) {
		if (rhs != *this) {
			node = rhs.node;
			ht = rhs.ht;
		}
		return *this;
	}

	iterator& operator=(const const_iterator& rhs) {
		if (rhs != *this) {
			node = rhs.node;
			ht = rhs.ht;
		}
		return *this;
	}

	// 操作符重载
	reference operator*() const {
		return node->value;
	}

	pointer operator->() const {
		return &(operator*());
	}

	iterator& operator++() {
		const_node_ptr old = node;
		node = node->next;
		if (!node) {
			size_type bucket = ht->bkt_num(old->value);
			while (!node && ++bucket < ht->buckets.size())
				node = ht->buckets[bucket];
		}
		return *this;
	}

	iterator operator++(int) {
		iterator temp = *this;
		++*this;
		return temp;
	}

	bool operator==(const iterator& rhs) const {
		return node == rhs.node;
	}

	bool operator!=(const iterator& rhs) const {
		return node != rhs.node;
	}
};


// hashtable迭代器(const版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
struct hashtable_const_iterator : public hashtable_iterator_base<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> {
	typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>							hashtable;
	typedef hashtable_iterator_base<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>			base_iterator;
	typedef hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>				iterator;
	typedef hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>			const_iterator;
	typedef hashtable_node<Value>*																node_ptr;
//	typedef const node_ptr																		const_node_ptr;
	typedef const hashtable_node<Value>*														const_node_ptr;
	typedef hashtable*																			container_ptr;
//	typedef const container_ptr																	const_container_ptr;
	typedef const hashtable*	 																const_container_ptr;

	typedef size_t																				size_type;
	typedef ptrdiff_t																			difference_type;
	typedef Value&																				reference;
	typedef const Value&																		const_reference;
	typedef	Value*																				pointer;
	typedef const Value*																		const_pointer;

	using base_iterator::node;
	using base_iterator::ht;

	// 复制和构造函数
	hashtable_const_iterator() = default;

	hashtable_const_iterator(const_node_ptr n, const_container_ptr tab) {
		node = n;
		ht = tab;
	};

	hashtable_const_iterator(const iterator& rhs) {
		node = rhs.node;
		ht = rhs.ht;
	};

	hashtable_const_iterator(const const_iterator& rhs) {
		node = rhs.node;
		ht = rhs.ht;
	};

	const_iterator& operator=(const iterator& rhs) {
		if (rhs != *this) {
			node = rhs.node;
			ht = rhs.ht;
		}
		return *this;
	}

	const_iterator& operator=(const const_iterator& rhs) {
		if (rhs != *this) {
			node = rhs.node;
			ht = rhs.ht;
		}
		return *this;
	}

	// 操作符重载
	reference operator*() const {
		return node->value;
	}

	pointer operator->() const {
		return &(operator*());
	}

	iterator& operator++() {
		const_node_ptr old = node;
		node = node->next;
		if (!node) {
			size_type bucket = bkt_num(old->value);
			while (!node && ++bucket < ht->buckets.size())
				node = ht->buckets[bucket];
		}
		return *this;
	}

	iterator operator++(int) {
		iterator temp = *this;
		++*this;
		return temp;
	}

	bool operator==(const const_iterator& rhs) const {
		return node == rhs.node;
	}

	bool operator!=(const const_iterator& rhs) const {
		return node != rhs.node;
	}

};


// hashtable桶数量
enum { stl_num_primes = 28 };

static const unsigned long stl_prime_list[stl_num_primes] = {
	53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 	
  1610612741ul, 3221225473ul, 4294967291ul
};

inline unsigned long stl_next_prime(unsigned long n) {
	const unsigned long* first = stl_prime_list;
	const unsigned long* last = stl_prime_list + stl_num_primes;
	const unsigned long* pos = mystl::lower_bound(first, last, n);
	return pos == last ? *(last - 1) : *pos;
}


// hashtable类
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
class hashtable {
public:
	// 将iterator和const_iterator声明为友类
	friend struct hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
	friend struct hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;

	typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>						Hashtable;
	typedef HashFcn																			hasher;
	typedef EqualKey																		key_equal;
	typedef Key																				key_type;
	typedef Value																			value_type;

	typedef hashtable_node<Value>*															node_ptr;
	typedef const hashtable_node<Value>*													const_node_ptr;
	typedef hashtable_node<Value>															node;
	typedef hashtable*																		container_ptr;
	typedef const hashtable*																const_container_ptr;

	typedef simple_alloc<Value, Alloc>														allocator_type;
	typedef simple_alloc<Value, Alloc>														data_allocator;
	typedef simple_alloc<node, Alloc>														node_allocator;

	typedef Value*																			pointer;
	typedef const Value*																	const_pointer;
	typedef Value&																			reference;
	typedef const Value&																	const_reference;
	typedef size_t																			size_type;
	typedef ptrdiff_t																		difference_type;

	typedef hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>			iterator;
	typedef hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>		const_iterator;

	allocator_type get_allocator() const {
		return data_allocator();
	}

	HashFcn hash_funct() const {
		return hash;
	}

	EqualKey key_eq() const {
		return equal;
	}

private:
	HashFcn hash;
	EqualKey equal;
	ExtractKey get_key;
	size_type num_elements;
	mystl::vector<node_ptr, Alloc> buckets;


	node_ptr get_node() {
		return node_allocator::allocate(1);
	}

	void put_node(node_ptr node) {
		node_allocator::deallocate(node, 1);
	}

public:
// 友元函数
	template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	friend bool operator==(const hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& lhs,
		const hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& rhs);

	template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	friend bool operator!=(const hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& lhs,
		const hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& rhs);

// 复制和构造函数
	hashtable(size_type n, HashFcn hf, EqualKey eql, ExtractKey ext) : hash(hf), equal(eql), get_key(ext), num_elements(0) {
		initialize_buckets(n);
	}

	hashtable(size_type n, HashFcn hf, EqualKey eql) : hash(hf), equal(eql), get_key(ExtractKey()), num_elements(0) {
		initialize_buckets(n);
	}

	hashtable(const hashtable& rhs) : hash(rhs.hash), equal(rhs.equal), get_key(rhs.get_key), num_elements(rhs.num_elements) {
		copy_from(rhs);
	}

	hashtable& operator=(const hashtable& rhs) {
		if (&rhs != this) {
			hash = rhs.hash;
			equal = rhs.equal;
			get_key = rhs.get_key;
			copy_from(rhs);
		}
		return *this;
	}

	~hashtable() {
		clear();
	}


// 迭代器相关函数
	iterator begin();

	const_iterator begin() const;

	iterator end();

	const_iterator end() const;

	const_iterator cbegin() const;

	const_iterator cend() const;


// 容量相关函数
	bool empty() const {
		return num_elements == 0;
	}

	size_type size() const {
		return num_elements;
	}

	size_type max_size() const {
		return static_cast<size_type>(-1);
	}

	size_type bucket_count() const {
		return buckets.size();
	}

	size_type max_bucket_count() const {
		return stl_prime_list[static_cast<int>(stl_num_primes) - 1];
	}

	size_type elems_in_bucket(size_type bucket) const {
		size_type res = 0;
		for (node_ptr cur = buckets[bucket]; cur; cur = cur->next)
			++res;
		return res;
	}

	void swap(hashtable& rhs) {
		mystl::swap(hash, rhs.hash);
		mystl::swap(equal, rhs.equal);
		mystl::swap(get_key, rhs.get_key);
		mystl::swap(num_elements, rhs.num_elements);
		buckets.swap(rhs.buckets);
	}


// 插入删除相关函数
	mystl::pair<iterator, bool> insert_unique(const value_type& value);

	mystl::pair<iterator, bool> insert_unique_noresize(const value_type& value);

	iterator insert_equal(const value_type& value);

	iterator insert_equal_noresize(const value_type& value);

	template<class InputIterator>
	void insert_unique(InputIterator first, InputIterator last);

	template<class InputIterator>
	void insert_unique(InputIterator first, InputIterator last, input_iterator_tag);

	template<class ForwardIterator>
	void insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag);

	template<class InputIterator>
	void insert_equal(InputIterator first, InputIterator last);

	template<class InputIterator>
	void insert_equal(InputIterator first, InputIterator last, input_iterator_tag);

	template<class ForwardIterator>
	void insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag);

	void insert_unique(const value_type* first, const value_type* last);

	void insert_unique(const_iterator first, const_iterator last);

	void insert_equal(const value_type* first, const value_type* last);

	void insert_equal(const_iterator first, const_iterator last);

	size_type erase(const key_type& key);

	void erase(const iterator& it);

	void erase(const const_iterator& it);

	void erase(iterator first, iterator last);

	void erase(const_iterator first, const_iterator last);

	void clear();

	size_type count(const key_type& key) const;


// 查找相关函数
	reference find_or_insert(const value_type& value);

	iterator find(const key_type& key);

	const_iterator find(const key_type& key) const;

	mystl::pair<iterator, iterator> equal_range(const key_type& key);

	mystl::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;



// 辅助函数 
	// 获得下一个质数
	size_type next_size(size_type n) const {
		return stl_next_prime(n);
	}

	// 初始化buckets数组
	void initialize_buckets(size_type n);

	// 拷贝另一个hashtable
	void copy_from(const hashtable& ht);

	// 得到一个新的结点
	node_ptr new_node(const value_type& value);

	// 销毁一个结点
	void delete_node(node_ptr node);

	// 重新给桶数组分配大小
	void resize(size_type n);

	// 获取桶的编号(只传入value)
	size_type bkt_num(const value_type& value) const {
		return bkt_num_key(get_key(value));
	}

	// 获取桶的编号(传入value和n)
	size_type bkt_num(const value_type& value, size_type n) const {
		return bkt_num_key(get_key(value), n);
	}

	// 获取桶的编号(只传入key)
	size_type bkt_num_key(const key_type& key) const {
		return bkt_num_key(key, buckets.size());
	}

	// 获取桶的编号(传入key和n)
	size_type bkt_num_key(const key_type& key, size_type n) const {
		return hash(key) % n;
	}

	// 删除桶中结点(不是从头开始)
	void erase_buckets(size_type n, node_ptr first, node_ptr last);

	// 删除桶中结点(从头开始)
	void erase_buckets(size_type n, node_ptr last);

};


//-----------------------------------------------------------迭代器相关函数------------------------------------------------------
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin() {
	for (size_type i = 0; i < buckets.size(); ++i) {
		node_ptr cur = buckets[i];
		if (cur)
			return iterator(cur, this);
	}
	return end();
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin() const {
	for (size_type i = 0; i < buckets.size(); ++i) {
		node_ptr cur = buckets[i];
		if (cur)
			return const_iterator(cur, const_cast<hashtable*>(this));
	}
	return end();
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::end() {
	return iterator(nullptr, this);
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::end() const {
	return const_iterator(nullptr, const_cast<hashtable*>(this));
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::cbegin() const {
	return begin();
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::cend() const {
	return end();
}


//----------------------------------------------------------插入删除相关函数-----------------------------------------------------
// 不重复插入一个元素
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
mystl::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool> 
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(const value_type& value) {
	resize(num_elements + 1);
	return insert_unique_noresize(value);
}

// 不重复插入一个元素(真正实现的函数)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
mystl::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool>
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const value_type& value) {
	size_type n = bkt_num(value);
	node_ptr first = buckets[n];
	for (node_ptr cur = first; cur; cur = cur->next) {
		if (equal(get_key(cur->value), get_key(value)))
			return mystl::pair<iterator, bool>(iterator(cur, this), false);
	}
	node_ptr node = new_node(value);
	node->next = first;
	buckets[n] = node;
	++num_elements;
	return mystl::pair<iterator, bool>(iterator(node, this), true);
}

// 允许重复的插入一个元素
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(const value_type& value) {
	resize(num_elements + 1);
	return insert_equal_noresize(value);
}

// 允许重复的插入一个元素(真正实现的函数)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type& value) {
	size_type n = bkt_num(value);
	node_ptr first = buckets[n];
	for (node_ptr cur = first; cur; cur = cur->next) {
		if (equal(get_key(cur->value), get_key(value))) {
			node_ptr node = new_node(value);
			node->next = cur->next;
			cur->next = node;
			++num_elements;
			return iterator(node, this);
		}
	}
	node_ptr node = new_node(value);
	node->next = first;
	buckets[n] = node;
	++num_elements;
	return iterator(node, this);
}

// 不允许重复插入多个元素
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
template<class InputIterator>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(InputIterator first, InputIterator last) {
	insert_unique(first, last, iterator_category(first));
}

// 不允许重复插入多个元素(input_iterator版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
template<class InputIterator>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(InputIterator first, InputIterator last, input_iterator_tag) {
	for (; first != last; ++first) {
		insert_unique(*first);
	}
}

// 不允许重复插入多个元素(forward_iterator版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
template<class ForwardIterator>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
	size_type n = mystl::distance(first, last);
	resize(num_elements + n);
	for (; n > 0; --n) {
		insert_unique_noresize(*first);
		++first;
	}
}

// 允许重复插入多个元素
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
template<class InputIterator>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(InputIterator first, InputIterator last) {
	insert_equal(first, last, iterator_category(first));
}

// 允许重复插入多个元素(input_iterator版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
template<class InputIterator>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(InputIterator first, InputIterator last, input_iterator_tag) {
	for (; first != last; ++first) {
		insert_equal(*first);
	}
}

// 允许重复插入多个元素(forward_iterator版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
template<class ForwardIterator>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
	size_type n = mystl::distance(first, last);
	resize(num_elements + n);
	for (; n > 0; --n) {
		insert_equal_noresize(*first);
		++first;
	}
}

// 不允许重复插入多个元素(const value_type*版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(const value_type* first, const value_type* last) {
	size_type n = mystl::distance(first, last);
	resize(num_elements + n);
	for (; n > 0; --n) {
		insert_unique_noresize(*first);
		++first;
	}
}

// 允许重复插入多个元素(const value_type*版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(const value_type* first, const value_type* last) {
	size_type n = mystl::distance(first, last);
	resize(num_elements + n);
	for (; n > 0; --n) {
		insert_equal_noresize(*first);
		++first;
	}
}

// 不允许重复插入多个元素(const_iterator版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(const_iterator first, const_iterator last) {
	size_type n = mystl::distance(first, last);
	resize(num_elements + n);
	for (; n > 0; --n) {
		insert_unique_noresize(*first);
		++first;
	}
}


// 允许重复插入多个元素(const_iterator版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(const_iterator first, const_iterator last) {
	size_type n = mystl::distance(first, last);
	resize(num_elements + n);
	for (; n > 0; --n) {
		insert_equal_noresize(*first);
		++first;
	}
}

// 删除key值为key的元素
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const key_type& key) {
	size_type erased = 0;
	size_type n = bkt_num_key(key);
	node_ptr first = buckets[n];
	if (first) {
		node_ptr cur = first;
		node_ptr next = first->next;
		while (next) {
			if (equal(get_key(next->value), key)) {
				cur->next = next->next;
				delete_node(next);
				next = cur->next;
				++erased;
				--num_elements;
			}
			else {
				cur = next;
				next = cur->next;
			}
		}
		if (equal(get_key(first->value), key)) {
			buckets[n] = first->next;
			delete_node(first);
			++erased;
			--num_elements;
		}
	}
	return erased;
}

// 删除指定结点
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const iterator& it) {
	node_ptr p = it.node;
	if (p) {
		size_type n = bkt_num(p->value);
		node_ptr cur = buckets[n];
		if (cur == p) {
			buckets[n] = cur->next;
			delete_node(cur);
			--num_elements;
		}
		else {
			node_ptr next = cur->next;
			while (next) {
				if (next == p) {
					cur->next = next->next;
					delete_node(next);
					--num_elements;
					break;
				}
				else {
					cur = next;
					next = cur->next;
				}
			}
		}
	}
}

// 删除指定结点(const_iterator版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const const_iterator& it) {
	erase(const_cast<iterator>(it));
}

// 删除多个结点
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(iterator first, iterator last) {
	if (first.node == last.node)
		return;
	size_type first_bucket = first.node ? bkt_num(first.node->value) : buckets.size();
	size_type last_bucket = last.node ? bkt_num(last.node->value) : buckets.size();
	if (first_bucket == last_bucket) {
		erase_buckets(first_bucket, first.node, last.node);
	}
	else {
		erase_buckets(first_bucket, first.node, 0);
		for (size_type cur_bucket = first_bucket + 1; cur_bucket != last_bucket; ++cur_bucket)
			erase_buckets(cur_bucket, 0);
		if (last_bucket != buckets.size())
			erase_buckets(last_bucket, last.node);
	}
}

// 删除多个结点(const_iterator版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const_iterator first, const_iterator last) {
	erase(const_cast<iterator>(first), const_cast<iterator>(last));
}

// 清除全部结点
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear() {
	for (size_type i = 0; i < buckets.size(); ++i) {
		node_ptr node = buckets[i];
		while (node) {
			node_ptr next = node->next;
			delete_node(node);
			node = next;
		}
		buckets[i] = nullptr;
	}
	num_elements = 0;
}


//------------------------------------------------------------查找相关函数-------------------------------------------------------
// 查找值等于value的结点，如果没有则插入
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::reference
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find_or_insert(const value_type& value) {
	resize(num_elements + 1);
	size_type n = bkt_num(value);
	node_ptr first = buckets[n];
	for (node_ptr cur = first; cur; cur = cur->next) {
		if (equal(get_key(cur->value), get_key(value)))
			return cur->value;
	}
	node_ptr node = new_node(value);
	node->next = first;
	buckets[n] = node;
	++num_elements;
	return node->value;
}

// 查找值key值等于key的结点
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find(const key_type& key) {
	size_type n = bkt_num_key(key);
	node_ptr first = buckets[n];
	for (; first && !equal(get_key(first->value), key); first = first->next) {

	}
	return iterator(first, this);
}

// 查找值key值等于key的结点(const版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find(const key_type& key) const{
	size_type n = bkt_num_key(key);
	const_node_ptr first = buckets[n];
	for (; first && !equal(get_key(first->value), key); first = first->next) {
	
	}
	return const_iterator(first, const_cast<hashtable*>(this));
}

// 计算值key值等于key的结点的个数
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::count(const key_type& key) const {
	size_type n = bkt_num_key(key);
	size_type res = 0;
	node_ptr cur = buckets[n];
	for (; cur; cur = cur->next) {
		if (equal(get_key(cur->value), key))
			++res;
	}
	return res;
}

// 查找值key值等于key的区间
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
mystl::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, 
	typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator>
	hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type& key) {
	size_type n = bkt_num_key(key);
	node_ptr first = buckets[n];
	for (; first; first = first->next) {
		if (equal(get_key(first->value), key)) {
			for (node_ptr cur = first->next; cur; cur = cur->next) {
				if (!equal(get_key(cur->value), key))
					return mystl::pair<iterator, iterator>(iterator(first, this), iterator(cur, this));
			}
			for (size_type m = n + 1; m < buckets.size(); ++m) {
				if (buckets[m])
					return mystl::pair<iterator, iterator>(iterator(first, this), iterator(buckets[m], this));
			}
			return mystl::pair<iterator, iterator>(iterator(first, this), end());
		}
	}
	return mystl::pair<iterator, iterator>(end(), end());
}


// 查找值key值等于key的区间(const版本)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
mystl::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator,
	typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator>
	hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type& key) const{
	size_type n = bkt_num_key(key);
	node_ptr first = buckets[n];
	for (; first; first = first->next) {
		if (equal(get_key(first->value), key)) {
			for (node_ptr cur = first->next; cur; cur = cur->next) {
				if (!equal(get_key(cur->value), key))
					return mystl::pair<const_iterator, const_iterator>(const_iterator(first, const_cast<hashtable*>(this)), 
						const_iterator(cur, const_cast<hashtable*>(this)));
			}
			for (size_type m = n + 1; m < buckets.size(); ++m) {
				if (buckets[m])
					return mystl::pair<const_iterator, const_iterator>(const_iterator(first, const_cast<hashtable*>(this)),
						const_iterator(buckets[m], const_cast<hashtable*>(this)));
			}
			return mystl::pair<const_iterator, const_iterator>(const_iterator(first, const_cast<hashtable*>(this)), end());
		}
	}
	return mystl::pair<const_iterator, const_iterator>(end(), end());
}


//--------------------------------------------------------------辅助函数---------------------------------------------------------
// 初始化buckets数组
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::initialize_buckets(size_type n) {
	size_type n_buckets = next_size(n);
	buckets.reserve(n_buckets);
	buckets.assign(n_buckets, nullptr);
	num_elements = 0;
}

// 拷贝另一个hashtable
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable& ht) {
	buckets.clear();
	buckets.reserve(ht.buckets.size());
	buckets.assign(ht.buckets.size(), nullptr);
	try {
		for (size_type bucket = 0; bucket < ht.buckets.size(); ++bucket) {
			node_ptr cur = ht.buckets[bucket];
			if (cur) {
				node_ptr copy = new_node(cur->value);
				buckets[bucket] = copy;
				for (node_ptr next = cur->next; next; ) {
					node_ptr temp = new_node(next->value);
					copy->next = temp;
					copy = temp;
					cur = next;
					next = cur->next;
				}
			}
		}
		num_elements = ht.num_elements;
	}
	catch (...) {
		clear();
	}
}

// 得到一个新的结点
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::node_ptr
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::new_node(const value_type& value) {
	node_ptr node = get_node();
	node->next = nullptr;
	try {
		mystl::construct(&node->value, value);
		return node;
	}
	catch (...) {
		put_node(node);
	}
}

// 清除一个结点
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::delete_node(node_ptr node) {
	mystl::destroy(&node->value);
	put_node(node);
}

// 重新给桶数组分配大小
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::resize(size_type n) {
	int old_n = buckets.size();
	if (n > old_n) {
		size_type new_n = next_size(n);
		if (new_n > old_n) {
			vector<node_ptr, Alloc> temp(new_n, nullptr);
			try {
				for (size_type i = 0; i < old_n; ++i) {
					node_ptr first = buckets[i];
					while (first) {
						size_type new_bucket = bkt_num(first->value, new_n);
						node_ptr next = first->next;
						buckets[i] = first->next;
						first->next = temp[new_bucket];
						temp[new_bucket] = first;
						first = next;
					}
				}
				buckets.swap(temp);
			}
			catch (...) {
				for (size_type i = 0; i < temp.size(); ++i) {
					node_ptr cur = temp[i];
					while (cur) {
						node_ptr next = cur->next;
						delete_node(cur);
						cur = next;
					}
				}
				throw;
			}
		}
	}
}

// 删除桶中结点(不是从头开始)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase_buckets(size_type n, node_ptr first, node_ptr last) {
	node_ptr cur = buckets[n];
	if (cur == first) {
		erase_buckets(n, last);
		return;
	}
	node_ptr next = cur->next;
	for (; next != first; ) {
		cur = next;
		next = cur->next;
	}
	while (next != last) {
		cur->next = next->next;
		delete_node(next);
		next = cur->next;
		--num_elements;
	}
}

// 删除桶中结点(从头开始)
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase_buckets(size_type n, node_ptr last) {
	node_ptr cur = buckets[n];
	while (cur != last) {
		buckets[n] = cur->next;
		delete_node(cur);
		cur = buckets[n];
		--num_elements;
	}
}


//-------------------------------------------------------------其他函数-------------------------------------------------------
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
bool operator==(const hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& lhs, 
	const hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& rhs) {
	typedef typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::node_ptr node_ptr;
	typedef typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type size_type;
	if (lhs.buckets.size() != rhs.buckets.size())
		return false;
	for (size_type i = 0; i < lhs.buckets.size(); ++i) {
		node_ptr cur1 = lhs.buckets[i];
		node_ptr cur2 = rhs.buckets[i];
		for (; cur1 && cur2 && cur1->value == cur2->value; cur1 = cur1->next, cur2 = cur2->next)
			;
		if (cur1 || cur2)
			return false;
	}
	return true;
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
bool operator!=(const hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& lhs,
	const hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& rhs) {
	return !(lhs == rhs);
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void swap(hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& lhs,
	hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& rhs) {
	lhs.swap(rhs);
}



} // mystl



#endif