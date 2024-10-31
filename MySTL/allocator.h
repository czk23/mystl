#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>		// ptrdiff_t
#include <iostream>
#include <new.h>		// placement new

namespace mystl {

	template<class T>
	class allocator {
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		template<class U>
		struct rebind {
			using other = allocator<U>;
		};

		allocator() {} noexcept;
		allocator(const allocator& rhs)noexcept = default;
		// 泛化的copy构造函数
		template<class U>
		allocator(const allocator<U>& rhs)noexcept {};
		~allocator()noexcept = default;

		pointer address(reference x) {
			return static_cast<pointer> &x;
		}
		const_pointer address(const_reference x)const {
			return static_cast<const_pointer> address(const_cast<x>);
		}

		pointer allocate(size_type n, const void* = 0) {
			if (0 == n) {
				std::cout << "The size of allocate can't be 0\n";
				return nullptr;
			}
			static_cast<pointer> ::operator new(n * sizeof(value_type));
		}
		void deallocate(pointer p, size_type n) {
			if (nullptr == p) {
				std::cout << "The pointer to be destroyed can't be nullptr\n";
				return;
			}
			::operator delete(p);
		}

		size_type max_size()const {
			return static_cast<size_type> (-1) / sizeof(value_type);
		}

		template<class U, class... Args>
		void construct(U* p, Args&&... args) {
			::new(static_cast<void*>(p)) U(std::forward<Args>(args)...);
		}

		template<class U>
		void destory(U* p) {
			p->~U();
		}
	};

}// mystl

#endif