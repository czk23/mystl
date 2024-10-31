#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

#include "memory.h"
#include <cassert>
#include "iterator.h"
#include "type_traits.h"
#include "algobase.h"

namespace mystl {

// vector定义
template<class T, class Alloc = alloc>
class vector {
public:
	typedef simple_alloc<T, Alloc>						data_allocator;
	typedef simple_alloc<T, Alloc>						allocator_type;

	// 基本数据类型
	typedef T											value_type;
	typedef T*											pointer;
	typedef const T*									const_pointer;
	typedef T*											iterator;
	typedef const T*									const_iterator;
	typedef T&											reference;
	typedef const T&									const_reference;
	typedef size_t										size_type;
	typedef ptrdiff_t									difference_type;
	typedef mystl::reverse_iterator<iterator>			reverse_iterator;
	typedef mystl::reverse_iterator<const_iterator>		const_reverse_iterator;

	// 得到一个迭代器
	allocator_type get_allocator() {
		return data_allocator();
	}

private:
	iterator start;				// 当前空间的头部
	iterator finish;			// 当前空间的尾部
	iterator end_of_storage;	// 当前空间存储区域尾部

public:
//-----------------------------------------------------构造、复制和析构函数---------------------------------------------------

	vector();

	explicit vector(size_type n);

	vector(size_type n, const T& value);

	vector(const vector<T, Alloc>& rhs);

	template<class InputIterator>
	vector(InputIterator first, InputIterator last) {
		typedef typename Is_integer<InputIterator>::value	Integer;
		initialize_aux(first, last, Integer());
	}

	vector(std::initializer_list<value_type> ilist) {
		range_initialize(ilist.begin(), ilist.end());
	}

	vector<T, Alloc>& operator=(const vector<T, Alloc>& rhs);

	vector<T, Alloc>& operator=(std::initializer_list<value_type>& ilist);

	~vector();

	//-----------------------------------------------------迭代器相关函数---------------------------------------------------------
	iterator begin() {
		return start;
	}

	const_iterator begin() const {
		return start;
	}

	iterator end() {
		return finish;
	}

	const_iterator end() const {
		return finish;
	}

	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(end());
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

	//-----------------------------------------------------容量相关函数---------------------------------------------------------
	size_type size() const {
		return static_cast<size_type>(finish - start);
	}

	size_type max_size() const {
		return static_cast<size_type>(-1) / sizeof(T);
	}

	size_type capacity() const {
		return static_cast<size_type>(end_of_storage - start);
	}

	bool empty() const {
		return start == finish;
	}

	void reserve(size_type n);

	//-----------------------------------------------------访问元素函数---------------------------------------------------------
	reference operator[](int n) {
		assert(0 <= n && static_cast<size_type>(n) < size());
		return *(start + n);
	}

	const_reference operator[](int n) const {
		assert(0 <= n && static_cast<size_type>(n) < size());
		return *(start + n);
	}

	reference at(int n) {
		if (!(n < size()))
			throw std::out_of_range("vector<T>::at() subscript out of range");
		return (*this)[n];
	}

	const_reference at(int n) const {
		if (!(n < size()))
			throw std::out_of_range("vector<T>::at() subscript out of range");
		return (*this)[n];
	}

	reference front() {
		assert(!empty());
		return *start;
	}

	const_reference front() const {
		assert(!empty());
		return *start;
	}

	reference back() {
		assert(!empty());
		return *(finish - 1);
	}

	const_reference back() const {
		assert(!empty());
		return *(finish - 1);
	}

	pointer data() {
		assert(!empty());
		return start;
	}

	const_pointer data() const {
		assert(!empty());
		return start;
	}

	//-------------------------------------------------修改容器相关操作----------------------------------------------------------
	// 1. 给容器赋值
	void assign(size_type n, const value_type& value);

	// 2. 给容器赋值(函数模板形式)
	template<class InputIterator>
	void assign(InputIterator first, InputIterator last);

	// 3. 尾端插入元素
	void push_back(const value_type& value);

	// 4. 弹出最后一个元素
	void pop_back();

	// 5. 插入单个元素
	iterator insert(iterator position, const value_type& value);

	// 6. 插入多个元素
	void insert(iterator position, size_type n, const value_type& value);

	// 7. 函数模板, 插入多个元素
	template<class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last);

	// 8. 删除单个元素
	iterator erase(iterator position);

	// 9. 删除多个元素
	iterator erase(iterator first, iterator last);

	// 10. 删除所有元素
	void clear();

	// 11. 调整容器大小, 并给出初值
	void resize(size_type n, const value_type& value);

	// 12. 调用容器大小
	void resize(size_type n);

	// 13. 交换两个容器 
	void swap(vector<T, Alloc>& rhs);

private:
	//-------------------------------------------------辅助函数------------------------------------------------------------------
	//----------------------------------------------初始化辅助函数---------------------------------------------------------------
		// 1. 提供n和value时的初始化函数
	template<class Integer>
	void initialize_aux(Integer first, Integer last, true_type);

	// 2. 提供first和last时的初始化函数
	template<class InputIterator>
	void initialize_aux(InputIterator first, InputIterator last, false_type);

	// 3. 2在具体实现上调用的函数
	template<class InputIterator>
	void range_initialize(InputIterator first, InputIterator last);

	// 4. 初始化一片空间并用copy赋予初值
	template<class InputIterator>
	void allocate_and_copy(size_type n, InputIterator first, InputIterator last);

	// 5. 初始化一片空间并用fill_n赋予初值
	void allocate_and_fill_n(size_type n, const T& value);


	//---------------------------------------------------插入操作辅助函数----------------------------------------------------------
		// 1. 提供n和value时的插入操作辅助函数
	void fill_insert(iterator position, size_type n, const T& value);

	// 2. 提供first和last时插入操作辅助函数(input iterator版本)
	template<class InputIterator>
	void range_insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag);

	// 3. 提供first和last时插入操作辅助函数(forward iterator版本)
	template<class ForwardIterator>
	void range_insert(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag);

	// 4. 插入单个元素时的插入操作辅助函数
	void insert_aux(iterator position, const value_type& value);

	// 5. 函数模板，当模板参数为整数时的插入多个元素时的插入操作辅助函数
	template<class Integer>
	void insert_dispatch(iterator position, Integer n, Integer value, true_type);

	// 6. 函数模板，当模板参数为迭代器时的插入多个元素时的插入操作辅助函数
	template<class InputIterator>
	void insert_dispatch(iterator position, InputIterator first, InputIterator last, false_type);


	//---------------------------------------------------赋值操作辅助函数----------------------------------------------------------
		// 1. 提供n和value时的赋值操作辅助函数
	void fill_assign(size_type n, const value_type& value);

	// 2. 模板函数, 当模板参数为整数时的赋值操作辅助函数
	template<class Integer>
	void assign_dispatch(Integer n, Integer value, true_type);

	// 3. 模板函数, 当模板参数为迭代器时的赋值操作辅助函数
	template<class InputIterator>
	void assign_dispatch(InputIterator first, InputIterator last, false_type);

	// 4. 3在实现上调用的函数(input iterator版本)
	template<class InputIterator>
	void assign_aux(InputIterator first, InputIterator last, input_iterator_tag);

	// 5. 3在实现上调用的函数(forward iterator版本)
	template<class ForwardIterator>
	void assign_aux(ForwardIterator first, ForwardIterator last, forward_iterator_tag);


	//---------------------------------------------------其他辅助函数----------------------------------------------------------
	void clean() {
		start = nullptr;
		finish = nullptr;
		end_of_storage = nullptr;
	};

};

//--------------------------------------------------修改容器相关操作--------------------------------------------------------------
// 重新赋值函数, 参数为整数
template<class T, class Alloc>
void vector<T, Alloc>::assign(size_type n, const value_type& value) {
	fill_assign(n, value);
}

// 重新赋值函数, 模板函数
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::assign(InputIterator first, InputIterator last) {
	typedef typename Is_integer<InputIterator>::value Integer;
	assign_dispatch(first, last, Integer());
}

// 尾端插入函数
template<class T, class Alloc>
void vector<T, Alloc>::push_back(const value_type& value) {
	// 还有空间
	if (finish != end_of_storage) {
		mystl::construct(finish, value);
		++finish;

	}
	// 没有空间
	else {
		insert_aux(finish, value);
	}
}

// 尾端弹出元素
template<class T, class Alloc>
void vector<T, Alloc>::pop_back() {
	if (finish != start) {
		--finish;
		mystl::destroy(finish);
	}
}

// 插入单个元素
template<class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type& value) {
	size_type n = position - start;
	if (finish != end_of_storage && position == finish) {
		mystl::construct(finish, value);
		finish++;
	}
	else {
		insert_aux(position, value);
	}
	return start + n;
}

// 插入多个元素
template<class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const value_type& value) {
	fill_insert(position, n, value);
}

// 插入多个元素, 函数模板
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
	typedef typename Is_integer<InputIterator>::value Integer;
	insert_dispatch(position, first, last, Integer());
}

// 删除单个元素
template<class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
	if (position + 1 != finish)
		mystl::copy(position + 1, finish, position);
	--finish;
	mystl::destroy(finish);
	return position;
}

// 删除多个元素
template<class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
	iterator temp = mystl::copy(last, finish, first);
	mystl::destroy(temp, finish);
	finish = finish - (last - first);
	return first;
}

// 删除所有元素
template<class T, class Alloc>
void vector<T, Alloc>::clear() {
	erase(start, finish);
}

// 重新调整容量大小
template<class T, class Alloc>
void vector<T, Alloc>::resize(size_type n, const value_type& value) {
	if (n < size()) {
		erase(start + n, finish);
	}
	else {
		insert(finish, n - size(), value);
	}
}

// 重新调整容量大小
template<class T, class Alloc>
void vector<T, Alloc>::resize(size_type n) {
	resize(n, T());
}

// 交换两个vector
// 调用函数有:
// 1. vector<T, Alloc>& operator=(std::initialize_list<value_type>& ilist) -> swap
template<class T, class Alloc>
void vector<T, Alloc>::swap(vector<T, Alloc>& rhs) {
	if (this != &rhs) {
		mystl::swap(start, rhs.start);
		mystl::swap(finish, rhs.finish);
		mystl::swap(end_of_storage, rhs.end_of_storage);
	}
}


//--------------------------------------------------构造、复制和析构函数----------------------------------------------------------
// 三种构造函数
template<class T, class Alloc>
vector<T, Alloc>::vector() {
	start = nullptr;
	finish = nullptr;
	end_of_storage = nullptr;
}

template<class T, class Alloc>
vector<T, Alloc>::vector(size_type n) {
	allocate_and_fill_n(n, T());
}

template<class T, class Alloc>
vector<T, Alloc>::vector(size_type n, const T& value) {
	allocate_and_fill_n(n, value);
}

// 复制构造函数
template<class T, class Alloc>
vector<T, Alloc>::vector(const vector<T, Alloc>& rhs) {
	allocate_and_copy(rhs.size(), rhs.start, rhs.finish);
}

// 赋值操作符重载
template<class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc>& rhs) {
	if (this != &rhs) {
		size_type len = rhs.size();
		// 比当前容量还要大
		if (len > capacity()) {
			mystl::destroy(start, finish);
			data_allocator::deallocate(start, end_of_storage - start);
			allocate_and_copy(len, rhs.start, rhs.finish);
		}
		// 比当前size小
		else if (len <= size()) {
			iterator temp = mystl::copy(rhs.start, rhs.finish, start);
			mystl::destroy(temp, finish);
			finish = temp;
		}
		// 比size大但是比容量小
		else {
			iterator temp = rhs.start + size();
			mystl::copy(rhs.start, temp, start);
			mystl::uninitialized_copy(temp, rhs.finish, finish);
			finish = start + len;
		}
	}
	return *this;
}

// 赋值操作符重载(参数是initialize_list)
// 因为initialize_list只是在初始化时有用，因此直接构造一个新的vector再swap
template<class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(std::initializer_list<value_type>& ilist) {
	vector<T, Alloc> temp(ilist);
	swap(temp);
	return *this;
}

// 析构函数
template<class T, class Alloc>
vector<T, Alloc>::~vector() {
	mystl::destroy(start, finish);
	data_allocator::deallocate(start, end_of_storage - start);
	clean();
}

//--------------------------------------------------辅助函数----------------------------------------------------------
// 构造函数辅助函数
// template<class InputIterator>
// vector(InputIterator first, InputIterator last){};
// 当InputIterator是整数时调用此函数
// 调用函数有:
// vector(first, last) -> initialize_aux
template<class T, class Alloc>
template<class Integer>
void vector<T, Alloc>::initialize_aux(Integer n, Integer value, true_type) {
	allocate_and_fill_n(n, value);
}

// 当InputIterator不是整数(是两个迭代器)时调用此函数
// 因为这里的动作在其他函数中也会用到，因此把这里的动作封装为另一个辅助函数
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::initialize_aux(InputIterator first, InputIterator last, false_type) {
	range_initialize(first, last);
}

// 当有一串的数来初始化vector时调用此函数
// 调用函数有: 
// 1. vector(first, last) -> initialize_aux -> range_initialize
// 2. vector(std::initialize_list<value_type> ilist) -> range_initialize
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::range_initialize(InputIterator first, InputIterator last) {
	size_type n = mystl::distance(first, last);
	allocate_and_copy(n, first, last);
}

// 进行两个操作，首先分配n个大小空间，然后将first到last的值复制到新的空间中
// 调用函数有:
// 1. range_initialize -> allocate_and_copy
// 2. vector(const vector<T, Alloc>& rhs) -> allocate_and_copy
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::allocate_and_copy(size_type n, InputIterator first, InputIterator last) {
	try {
		start = data_allocator::allocate(n);
		finish = mystl::uninitialized_copy(first, last, start);
		end_of_storage = start + n;
	}
	catch (...) {
		mystl::destroy(start, finish);
		data_allocator::deallocate(start, n);
		clean();
	}
}

// 进行两个操作，首先分配n个大小空间，然后将新空间用value填充
// 调用函数有:
// 1. vector(size_type n, const T& value) -> allocate_and_fill_n
// 2. vector(size_type n) -> allocate_and_fill_n
template<class T, class Alloc>
void vector<T, Alloc>::allocate_and_fill_n(size_type n, const T& value) {
	try {
		start = data_allocator::allocate(n);
		finish = uninitialized_fill_n(start, n, value);
		end_of_storage = start + n;
	}
	catch (...) {
		mystl::destroy(start, finish);
		data_allocator::deallocate(start, n);
		clean();
	}
}

// 插入单个元素的辅助函数
// 调用函数有:
// 1. 
template<class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const value_type& value) {
	// 当前容量还有空间
	if (finish != end_of_storage) {
		mystl::construct(finish, *(finish - 1));
		finish++;
		mystl::copy_backward(position, finish - 2, finish - 1);
		*position = value;
	}
	// 没有容量
	else {
		size_type old_size = size();
		size_type new_size = (old_size == 0 ? 1 : old_size * 2);
		iterator new_start = nullptr;
		iterator new_finish = nullptr;
		try {
			new_start = data_allocator::allocate(new_size);
			new_finish = mystl::uninitialized_copy(start, position, new_start);
			mystl::construct(new_finish, value);
			new_finish++;
			new_finish = mystl::uninitialized_copy(position, finish, new_finish);
		}
		catch (...) {
			mystl::destroy(new_start, new_finish);
			data_allocator::deallocate(new_start, new_size);
		}
		mystl::destroy(start, finish);
		data_allocator::deallocate(start, old_size);
		start = new_start;
		finish = new_finish;
		end_of_storage = start + new_size;
	}
}

// 在指定位置插入n个元素，当模板参数为整数时调用此函数
// 调用函数有: 
// 1. insert(position, start, last) -> insert_dispatch()
template<class T, class Alloc>
template<class Integer>
void vector<T, Alloc>::insert_dispatch(iterator position, Integer n, Integer value, true_type) {
	fill_insert(position, static_cast<size_type>(n), static_cast<value_type>(value));
}

// 在指定位置插入n个元素，当模板参数为迭代器时调用此函数
// 调用函数有: 
// 1. insert(position, start, last) -> insert_dispatch()
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::insert_dispatch(iterator position, InputIterator first, InputIterator last, false_type) {
	range_insert(position, first, last, iterator_category(first));
}

// 在指定位置插入n个元素，当insert函数的两个参数是整数时会调用此函数
// 调用函数有:
// 1. insert(position, start, last) -> insert_dispatch(position, n, value, true_type) -> fill_insert()
template<class T, class Alloc>
void vector<T, Alloc>::fill_insert(iterator position, size_type n, const T& value) {
	if (n != 0) {
		// 容量大于等于n
		if (static_cast<size_type>(end_of_storage - finish) >= n) {
			size_type elem_after = finish - position;
			// position之后的元素个数大于n
			// 转移后半部分 -> 转移前半部分 ->填充
			if (elem_after > n) {
				iterator new_finish = mystl::uninitialized_copy(finish - n, finish, finish);
				mystl::copy_backward(position, finish - n, finish);
				mystl::fill(position, position + n, value);
				finish = new_finish;
			}
			// position之后的元素个数大于等于n
			// 填充一部分 -> 转移 -> 填充剩下部分
			else {
				iterator new_finish = mystl::uninitialized_fill_n(finish, n - elem_after, value);
				new_finish = mystl::uninitialized_copy(position, finish, new_finish);
				mystl::fill(position, finish, value);
				finish = new_finish;
			}
		}
		// 容量比n小
		else {
			size_type old_size = size();
			size_type new_size = old_size + mystl::max(old_size, n);
			iterator new_start = nullptr;
			iterator new_finish = nullptr;
			try {
				new_start = data_allocator::allocate(new_size);
				new_finish = mystl::uninitialized_copy(start, position, new_start);
				new_finish = mystl::uninitialized_fill_n(new_finish, n, value);
				new_finish = mystl::uninitialized_copy(position, finish, new_finish);
			}
			catch (...) {
				mystl::destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, n);
			}
			mystl::destroy(start, finish);
			data_allocator::deallocate(start, old_size);
			start = new_start;
			finish = new_finish;
			end_of_storage = start + new_size;
		}
	}
}

// 在指定位置插入n个元素，当insert函数的参数是迭代器时调用此函数
// 针对迭代器是input iterator的版本
// 调用函数有:
// 1. insert(position, start, last) -> insert_dispatch(position, first, last, false_type) -> range_insert()
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::range_insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag) {
	for (; first != last; ++first) {
		position = insert(position, *first);
		++position;
	}
}

// 在指定位置插入n个元素，当insert函数的参数是迭代器时调用此函数
// 针对迭代器是forward iterator的版本
// 调用函数有:
// 1. insert(position, start, last) -> insert_dispatch(position, first, last, false_type) -> range_insert()
template<class T, class Alloc>
template<class ForwardIterator>
void vector<T, Alloc>::range_insert(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
	size_type n = mystl::distance(first, last);
	if (n != 0) {
		// 容量大于等于n
		if (capacity() >= n) {
			size_type elem_after = finish - position;
			// position之后的元素个数大于n
			// 转移后半部分 -> 转移前半部分 ->填充
			if (elem_after > n) {
				iterator new_finish = mystl::uninitialized_copy(finish - n, finish, finish);
				mystl::copy_backward(position, finish - n, finish);
				mystl::copy(first, last, position);
				finish = new_finish;
			}
			// position之后的元素个数大于等于n
			// 填充一部分 -> 转移 -> 填充剩下部分
			else {
				ForwardIterator mid = first;
				mystl::advance(mid, elem_after);
				iterator new_finish = mystl::uninitialized_copy(mid, last, finish);
				new_finish = mystl::uninitialized_copy(position, finish, new_finish);
				mystl::copy(first, mid, position);
				finish = new_finish;
			}
		}
		// 容量比n小
		else {
			size_type old_size = size();
			size_type new_size = old_size + mystl::max(old_size, n);
			iterator new_start = nullptr;
			iterator new_finish = nullptr;
			try {
				new_start = data_allocator::allocate(new_size);
				new_finish = mystl::uninitialized_copy(start, position, new_start);
				new_finish = mystl::uninitialized_copy(first, last, new_finish);
				new_finish = mystl::uninitialized_copy(position, finish, new_finish);
			}
			catch (...) {
				mystl::destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);
			}
			mystl::destroy(start, finish);
			data_allocator::deallocate(start, old_size);
			start = new_start;
			finish = new_finish;
			end_of_storage = start + new_size;
		}
	}
}

// 给vector重新分配值, 当assign函数的参数为整数时调用此函数
// 调用函数有:
// 1. assign(n, value) -> fill_assign
// 2. assign(first, last) -> assign_dispatch(first, last, true_type)(模板参数为整数) -> fill_assign
template<class T, class Alloc>
void vector<T, Alloc>::fill_assign(size_type n, const value_type& value) {
	if (capacity() < n) {
		vector temp(n, value);
		swap(temp);
	}
	else if (size() < n) {
		mystl::fill(start, finish, value);
		finish = mystl::uninitialized_fill_n(finish, n - size(), value);
	}
	else {
		mystl::fill_n(start, n, value);
		iterator new_finish = start + n;
		erase(new_finish, finish);
		finish = new_finish;
	}
}

// 给vector重新分配值, 当assign函数的模板参数为整数时调用此函数
// 调用函数有:
// 1. assign(first, assign) -> assign_dispatch
template<class T, class Alloc>
template<class Integer>
void vector<T, Alloc>::assign_dispatch(Integer n, Integer value, true_type) {
	fill_assign(static_cast<size_type>(n), static_cast<value_type>(value));
}

// 给vector重新分配值, 当assign函数的模板参数为迭代器时调用此函数, 还要进一步区分迭代器的类型
// 调用函数有:
// 1. assign(first, last) -> assign_dispatch
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::assign_dispatch(InputIterator first, InputIterator last, false_type) {
	assign_aux(first, last, iterator_category(first));
}

// 给vector重新分配值, 当assign函数的模板参数为迭代器时调用此函数(input iterator版本)
// 调用函数有:
// 1. assign(first, last) -> assign_dispatch(first, last, false_type) -> assign_aux
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::assign_aux(InputIterator first, InputIterator last, input_iterator_tag) {
	iterator cur = start;
	for (; first != last && cur != finish; ++first, ++cur) {
		*cur = *first;
	}
	if (first != last) {
		insert(finish, first, last);
	}
	else {
		erase(cur, finish);
	}
}

// 给vector重新分配值, 当assign函数的模板参数为迭代器时调用此函数(forward iterator版本)
// 调用函数有:
// 1. assign(first, last) -> assign_dispatch(first, last, false_type) -> assign_aux
template<class T, class Alloc>
template<class ForwardIterator>
void vector<T, Alloc>::assign_aux(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
	size_type len = distance(first, last);
	if (capacity() < len) {
		mystl::destroy(start, finish);
		data_allocator::deallocate(start, end_of_storage - start);
		allocate_and_copy(len, first, last);
	}
	else if (size() >= len) {
		mystl::copy(first, last, start);
		mystl::destroy(start + len, finish);
		finish = start + len;
	}
	else {
		ForwardIterator mid = first;
		mystl::advance(mid, len);
		mystl::copy(first, mid, start);
		finish = mystl::uninitialized_copy(mid, last, finish);
	}
}

// 预留空间函数, 只有当n大于当前容量时才会重新分配
template<class T, class Alloc>
void vector<T, Alloc>::reserve(size_type n) {
	if (n > capacity()) {
		size_type old_size = size();
		iterator new_start = data_allocator::allocate(n);
		iterator new_finish = mystl::uninitialized_copy(start, finish, new_start);
		mystl::destroy(start, finish);
		data_allocator::deallocate(start, n);
		start = new_start;
		finish = new_finish;
		end_of_storage = start + n;
	}
}

//--------------------------------------------------常用函数(不属于vector)----------------------------------------------------------
template<class T, class Alloc>
inline bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return (lhs.size() == rhs.size() && mystl::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template<class T, class Alloc>
inline bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return !(lhs == rhs);
}

template<class T, class Alloc>
inline bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, class Alloc>
inline bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return !(lhs < rhs);
}

template<class T, class Alloc>
inline bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return rhs < lhs;
}

template<class T, class Alloc>
inline bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return !(rhs < lhs);
}

template<class T, class Alloc>
inline void swap(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	lhs.swap(rhs);
}

}	// mystl

#endif