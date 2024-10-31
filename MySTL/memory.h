#ifndef MYSTL_MEMORY_H
#define MYSTL_MEMORY_H

#include "mystl_alloc.h"
#include "mystl_construct.h"
#include "mystl_uninitialized.h"

namespace mystl {

// 临时缓冲区的申请和释放
template<class ForwardIterator, class T>
class temporary_buffer {
private:
	ptrdiff_t	original_len;
	ptrdiff_t	len;
	T*			buffer;

public:
	temporary_buffer(ForwardIterator first, ForwardIterator last);

	~temporary_buffer() {
		mystl::destroy(buffer, buffer + len);
		free(buffer);
	}

	T* begin() const {
		return buffer;
	}

	T* end() const {
		return buffer + len;
	}

	ptrdiff_t size() const {
		return len;
	}

	ptrdiff_t requested_size() const {
		return original_len;
	}

private:
	temporary_buffer(const temporary_buffer&);

	void operator=(const temporary_buffer&);

private:
	void allocate_buffer();

	void initialize_buffer(const T&, std::true_type) {};

	void initialize_buffer(const T& value, std::false_type) {
		mystl::uninitialized_fill_n(buffer, len, value);
	}
};

template<class ForwardIterator, class T>
temporary_buffer<ForwardIterator, T>::temporary_buffer(ForwardIterator first, ForwardIterator last) {
	try {
		len = mystl::distance(first, last);
		allocate_buffer();
		if (len > 0) {
			initialize_buffer(*first, std::is_trivially_default_constructible<T>());
		}
	}
	catch (...) {
		free(buffer);
		buffer = nullptr;
		len = 0;
	}
}

template<class ForwardIterator, class T>
void temporary_buffer<ForwardIterator, T>::allocate_buffer() {
	original_len = len;
	if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
		len = INT_MAX / sizeof(T);
	while (len > 0) {
		buffer = static_cast<T*>(malloc(len * sizeof(T)));
		if (buffer)
			break;
		len >>= 1;
	}
}


// 智能指针auto_ptr模板
template<class T>
class auto_ptr {
private:
	T* pointer;

public:
	explicit auto_ptr(T* p = nullptr) : pointer(p) {};

	template<class U>
	auto_ptr(auto_ptr<U>& rhs) : pointer(rhs.release()) {};

	template<class U>
	auto_ptr& operator=(auto_ptr<U>& rhs) {
		if (this->get() != rhs.get()) {
			delete pointer;
			pointer = rhs.release();
		}
	}

	~auto_ptr() {
		delete pointer;
	}

	T& operator*() const {
		return *pointer;
	}

	T* operator->() const {
		return pointer;
	}

	T* get() const {
		return pointer;
	}

	T* release() {
		T* temp = pointer;
		pointer = nullptr;
		return temp;
	}

	void reset(T* p) {
		if (p != pointer) {
			delete pointer;
			pointer = p;
		}
	}
};

}	// mystl

#endif
