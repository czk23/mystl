#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

#include "memory.h"
#include <cassert>
#include "iterator.h"
#include "type_traits.h"
#include "algobase.h"

namespace mystl {

// vector����
template<class T, class Alloc = alloc>
class vector {
public:
	typedef simple_alloc<T, Alloc>						data_allocator;
	typedef simple_alloc<T, Alloc>						allocator_type;

	// ������������
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

	// �õ�һ��������
	allocator_type get_allocator() {
		return data_allocator();
	}

private:
	iterator start;				// ��ǰ�ռ��ͷ��
	iterator finish;			// ��ǰ�ռ��β��
	iterator end_of_storage;	// ��ǰ�ռ�洢����β��

public:
//-----------------------------------------------------���졢���ƺ���������---------------------------------------------------

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

	//-----------------------------------------------------��������غ���---------------------------------------------------------
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

	//-----------------------------------------------------������غ���---------------------------------------------------------
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

	//-----------------------------------------------------����Ԫ�غ���---------------------------------------------------------
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

	//-------------------------------------------------�޸�������ز���----------------------------------------------------------
	// 1. ��������ֵ
	void assign(size_type n, const value_type& value);

	// 2. ��������ֵ(����ģ����ʽ)
	template<class InputIterator>
	void assign(InputIterator first, InputIterator last);

	// 3. β�˲���Ԫ��
	void push_back(const value_type& value);

	// 4. �������һ��Ԫ��
	void pop_back();

	// 5. ���뵥��Ԫ��
	iterator insert(iterator position, const value_type& value);

	// 6. ������Ԫ��
	void insert(iterator position, size_type n, const value_type& value);

	// 7. ����ģ��, ������Ԫ��
	template<class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last);

	// 8. ɾ������Ԫ��
	iterator erase(iterator position);

	// 9. ɾ�����Ԫ��
	iterator erase(iterator first, iterator last);

	// 10. ɾ������Ԫ��
	void clear();

	// 11. ����������С, ��������ֵ
	void resize(size_type n, const value_type& value);

	// 12. ����������С
	void resize(size_type n);

	// 13. ������������ 
	void swap(vector<T, Alloc>& rhs);

private:
	//-------------------------------------------------��������------------------------------------------------------------------
	//----------------------------------------------��ʼ����������---------------------------------------------------------------
		// 1. �ṩn��valueʱ�ĳ�ʼ������
	template<class Integer>
	void initialize_aux(Integer first, Integer last, true_type);

	// 2. �ṩfirst��lastʱ�ĳ�ʼ������
	template<class InputIterator>
	void initialize_aux(InputIterator first, InputIterator last, false_type);

	// 3. 2�ھ���ʵ���ϵ��õĺ���
	template<class InputIterator>
	void range_initialize(InputIterator first, InputIterator last);

	// 4. ��ʼ��һƬ�ռ䲢��copy�����ֵ
	template<class InputIterator>
	void allocate_and_copy(size_type n, InputIterator first, InputIterator last);

	// 5. ��ʼ��һƬ�ռ䲢��fill_n�����ֵ
	void allocate_and_fill_n(size_type n, const T& value);


	//---------------------------------------------------���������������----------------------------------------------------------
		// 1. �ṩn��valueʱ�Ĳ��������������
	void fill_insert(iterator position, size_type n, const T& value);

	// 2. �ṩfirst��lastʱ���������������(input iterator�汾)
	template<class InputIterator>
	void range_insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag);

	// 3. �ṩfirst��lastʱ���������������(forward iterator�汾)
	template<class ForwardIterator>
	void range_insert(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag);

	// 4. ���뵥��Ԫ��ʱ�Ĳ��������������
	void insert_aux(iterator position, const value_type& value);

	// 5. ����ģ�壬��ģ�����Ϊ����ʱ�Ĳ�����Ԫ��ʱ�Ĳ��������������
	template<class Integer>
	void insert_dispatch(iterator position, Integer n, Integer value, true_type);

	// 6. ����ģ�壬��ģ�����Ϊ������ʱ�Ĳ�����Ԫ��ʱ�Ĳ��������������
	template<class InputIterator>
	void insert_dispatch(iterator position, InputIterator first, InputIterator last, false_type);


	//---------------------------------------------------��ֵ������������----------------------------------------------------------
		// 1. �ṩn��valueʱ�ĸ�ֵ������������
	void fill_assign(size_type n, const value_type& value);

	// 2. ģ�庯��, ��ģ�����Ϊ����ʱ�ĸ�ֵ������������
	template<class Integer>
	void assign_dispatch(Integer n, Integer value, true_type);

	// 3. ģ�庯��, ��ģ�����Ϊ������ʱ�ĸ�ֵ������������
	template<class InputIterator>
	void assign_dispatch(InputIterator first, InputIterator last, false_type);

	// 4. 3��ʵ���ϵ��õĺ���(input iterator�汾)
	template<class InputIterator>
	void assign_aux(InputIterator first, InputIterator last, input_iterator_tag);

	// 5. 3��ʵ���ϵ��õĺ���(forward iterator�汾)
	template<class ForwardIterator>
	void assign_aux(ForwardIterator first, ForwardIterator last, forward_iterator_tag);


	//---------------------------------------------------������������----------------------------------------------------------
	void clean() {
		start = nullptr;
		finish = nullptr;
		end_of_storage = nullptr;
	};

};

//--------------------------------------------------�޸�������ز���--------------------------------------------------------------
// ���¸�ֵ����, ����Ϊ����
template<class T, class Alloc>
void vector<T, Alloc>::assign(size_type n, const value_type& value) {
	fill_assign(n, value);
}

// ���¸�ֵ����, ģ�庯��
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::assign(InputIterator first, InputIterator last) {
	typedef typename Is_integer<InputIterator>::value Integer;
	assign_dispatch(first, last, Integer());
}

// β�˲��뺯��
template<class T, class Alloc>
void vector<T, Alloc>::push_back(const value_type& value) {
	// ���пռ�
	if (finish != end_of_storage) {
		mystl::construct(finish, value);
		++finish;

	}
	// û�пռ�
	else {
		insert_aux(finish, value);
	}
}

// β�˵���Ԫ��
template<class T, class Alloc>
void vector<T, Alloc>::pop_back() {
	if (finish != start) {
		--finish;
		mystl::destroy(finish);
	}
}

// ���뵥��Ԫ��
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

// ������Ԫ��
template<class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const value_type& value) {
	fill_insert(position, n, value);
}

// ������Ԫ��, ����ģ��
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
	typedef typename Is_integer<InputIterator>::value Integer;
	insert_dispatch(position, first, last, Integer());
}

// ɾ������Ԫ��
template<class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
	if (position + 1 != finish)
		mystl::copy(position + 1, finish, position);
	--finish;
	mystl::destroy(finish);
	return position;
}

// ɾ�����Ԫ��
template<class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
	iterator temp = mystl::copy(last, finish, first);
	mystl::destroy(temp, finish);
	finish = finish - (last - first);
	return first;
}

// ɾ������Ԫ��
template<class T, class Alloc>
void vector<T, Alloc>::clear() {
	erase(start, finish);
}

// ���µ���������С
template<class T, class Alloc>
void vector<T, Alloc>::resize(size_type n, const value_type& value) {
	if (n < size()) {
		erase(start + n, finish);
	}
	else {
		insert(finish, n - size(), value);
	}
}

// ���µ���������С
template<class T, class Alloc>
void vector<T, Alloc>::resize(size_type n) {
	resize(n, T());
}

// ��������vector
// ���ú�����:
// 1. vector<T, Alloc>& operator=(std::initialize_list<value_type>& ilist) -> swap
template<class T, class Alloc>
void vector<T, Alloc>::swap(vector<T, Alloc>& rhs) {
	if (this != &rhs) {
		mystl::swap(start, rhs.start);
		mystl::swap(finish, rhs.finish);
		mystl::swap(end_of_storage, rhs.end_of_storage);
	}
}


//--------------------------------------------------���졢���ƺ���������----------------------------------------------------------
// ���ֹ��캯��
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

// ���ƹ��캯��
template<class T, class Alloc>
vector<T, Alloc>::vector(const vector<T, Alloc>& rhs) {
	allocate_and_copy(rhs.size(), rhs.start, rhs.finish);
}

// ��ֵ����������
template<class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc>& rhs) {
	if (this != &rhs) {
		size_type len = rhs.size();
		// �ȵ�ǰ������Ҫ��
		if (len > capacity()) {
			mystl::destroy(start, finish);
			data_allocator::deallocate(start, end_of_storage - start);
			allocate_and_copy(len, rhs.start, rhs.finish);
		}
		// �ȵ�ǰsizeС
		else if (len <= size()) {
			iterator temp = mystl::copy(rhs.start, rhs.finish, start);
			mystl::destroy(temp, finish);
			finish = temp;
		}
		// ��size���Ǳ�����С
		else {
			iterator temp = rhs.start + size();
			mystl::copy(rhs.start, temp, start);
			mystl::uninitialized_copy(temp, rhs.finish, finish);
			finish = start + len;
		}
	}
	return *this;
}

// ��ֵ����������(������initialize_list)
// ��Ϊinitialize_listֻ���ڳ�ʼ��ʱ���ã����ֱ�ӹ���һ���µ�vector��swap
template<class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(std::initializer_list<value_type>& ilist) {
	vector<T, Alloc> temp(ilist);
	swap(temp);
	return *this;
}

// ��������
template<class T, class Alloc>
vector<T, Alloc>::~vector() {
	mystl::destroy(start, finish);
	data_allocator::deallocate(start, end_of_storage - start);
	clean();
}

//--------------------------------------------------��������----------------------------------------------------------
// ���캯����������
// template<class InputIterator>
// vector(InputIterator first, InputIterator last){};
// ��InputIterator������ʱ���ô˺���
// ���ú�����:
// vector(first, last) -> initialize_aux
template<class T, class Alloc>
template<class Integer>
void vector<T, Alloc>::initialize_aux(Integer n, Integer value, true_type) {
	allocate_and_fill_n(n, value);
}

// ��InputIterator��������(������������)ʱ���ô˺���
// ��Ϊ����Ķ���������������Ҳ���õ�����˰�����Ķ�����װΪ��һ����������
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::initialize_aux(InputIterator first, InputIterator last, false_type) {
	range_initialize(first, last);
}

// ����һ����������ʼ��vectorʱ���ô˺���
// ���ú�����: 
// 1. vector(first, last) -> initialize_aux -> range_initialize
// 2. vector(std::initialize_list<value_type> ilist) -> range_initialize
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::range_initialize(InputIterator first, InputIterator last) {
	size_type n = mystl::distance(first, last);
	allocate_and_copy(n, first, last);
}

// �����������������ȷ���n����С�ռ䣬Ȼ��first��last��ֵ���Ƶ��µĿռ���
// ���ú�����:
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

// �����������������ȷ���n����С�ռ䣬Ȼ���¿ռ���value���
// ���ú�����:
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

// ���뵥��Ԫ�صĸ�������
// ���ú�����:
// 1. 
template<class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const value_type& value) {
	// ��ǰ�������пռ�
	if (finish != end_of_storage) {
		mystl::construct(finish, *(finish - 1));
		finish++;
		mystl::copy_backward(position, finish - 2, finish - 1);
		*position = value;
	}
	// û������
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

// ��ָ��λ�ò���n��Ԫ�أ���ģ�����Ϊ����ʱ���ô˺���
// ���ú�����: 
// 1. insert(position, start, last) -> insert_dispatch()
template<class T, class Alloc>
template<class Integer>
void vector<T, Alloc>::insert_dispatch(iterator position, Integer n, Integer value, true_type) {
	fill_insert(position, static_cast<size_type>(n), static_cast<value_type>(value));
}

// ��ָ��λ�ò���n��Ԫ�أ���ģ�����Ϊ������ʱ���ô˺���
// ���ú�����: 
// 1. insert(position, start, last) -> insert_dispatch()
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::insert_dispatch(iterator position, InputIterator first, InputIterator last, false_type) {
	range_insert(position, first, last, iterator_category(first));
}

// ��ָ��λ�ò���n��Ԫ�أ���insert��������������������ʱ����ô˺���
// ���ú�����:
// 1. insert(position, start, last) -> insert_dispatch(position, n, value, true_type) -> fill_insert()
template<class T, class Alloc>
void vector<T, Alloc>::fill_insert(iterator position, size_type n, const T& value) {
	if (n != 0) {
		// �������ڵ���n
		if (static_cast<size_type>(end_of_storage - finish) >= n) {
			size_type elem_after = finish - position;
			// position֮���Ԫ�ظ�������n
			// ת�ƺ�벿�� -> ת��ǰ�벿�� ->���
			if (elem_after > n) {
				iterator new_finish = mystl::uninitialized_copy(finish - n, finish, finish);
				mystl::copy_backward(position, finish - n, finish);
				mystl::fill(position, position + n, value);
				finish = new_finish;
			}
			// position֮���Ԫ�ظ������ڵ���n
			// ���һ���� -> ת�� -> ���ʣ�²���
			else {
				iterator new_finish = mystl::uninitialized_fill_n(finish, n - elem_after, value);
				new_finish = mystl::uninitialized_copy(position, finish, new_finish);
				mystl::fill(position, finish, value);
				finish = new_finish;
			}
		}
		// ������nС
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

// ��ָ��λ�ò���n��Ԫ�أ���insert�����Ĳ����ǵ�����ʱ���ô˺���
// ��Ե�������input iterator�İ汾
// ���ú�����:
// 1. insert(position, start, last) -> insert_dispatch(position, first, last, false_type) -> range_insert()
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::range_insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag) {
	for (; first != last; ++first) {
		position = insert(position, *first);
		++position;
	}
}

// ��ָ��λ�ò���n��Ԫ�أ���insert�����Ĳ����ǵ�����ʱ���ô˺���
// ��Ե�������forward iterator�İ汾
// ���ú�����:
// 1. insert(position, start, last) -> insert_dispatch(position, first, last, false_type) -> range_insert()
template<class T, class Alloc>
template<class ForwardIterator>
void vector<T, Alloc>::range_insert(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
	size_type n = mystl::distance(first, last);
	if (n != 0) {
		// �������ڵ���n
		if (capacity() >= n) {
			size_type elem_after = finish - position;
			// position֮���Ԫ�ظ�������n
			// ת�ƺ�벿�� -> ת��ǰ�벿�� ->���
			if (elem_after > n) {
				iterator new_finish = mystl::uninitialized_copy(finish - n, finish, finish);
				mystl::copy_backward(position, finish - n, finish);
				mystl::copy(first, last, position);
				finish = new_finish;
			}
			// position֮���Ԫ�ظ������ڵ���n
			// ���һ���� -> ת�� -> ���ʣ�²���
			else {
				ForwardIterator mid = first;
				mystl::advance(mid, elem_after);
				iterator new_finish = mystl::uninitialized_copy(mid, last, finish);
				new_finish = mystl::uninitialized_copy(position, finish, new_finish);
				mystl::copy(first, mid, position);
				finish = new_finish;
			}
		}
		// ������nС
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

// ��vector���·���ֵ, ��assign�����Ĳ���Ϊ����ʱ���ô˺���
// ���ú�����:
// 1. assign(n, value) -> fill_assign
// 2. assign(first, last) -> assign_dispatch(first, last, true_type)(ģ�����Ϊ����) -> fill_assign
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

// ��vector���·���ֵ, ��assign������ģ�����Ϊ����ʱ���ô˺���
// ���ú�����:
// 1. assign(first, assign) -> assign_dispatch
template<class T, class Alloc>
template<class Integer>
void vector<T, Alloc>::assign_dispatch(Integer n, Integer value, true_type) {
	fill_assign(static_cast<size_type>(n), static_cast<value_type>(value));
}

// ��vector���·���ֵ, ��assign������ģ�����Ϊ������ʱ���ô˺���, ��Ҫ��һ�����ֵ�����������
// ���ú�����:
// 1. assign(first, last) -> assign_dispatch
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::assign_dispatch(InputIterator first, InputIterator last, false_type) {
	assign_aux(first, last, iterator_category(first));
}

// ��vector���·���ֵ, ��assign������ģ�����Ϊ������ʱ���ô˺���(input iterator�汾)
// ���ú�����:
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

// ��vector���·���ֵ, ��assign������ģ�����Ϊ������ʱ���ô˺���(forward iterator�汾)
// ���ú�����:
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

// Ԥ���ռ亯��, ֻ�е�n���ڵ�ǰ����ʱ�Ż����·���
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

//--------------------------------------------------���ú���(������vector)----------------------------------------------------------
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