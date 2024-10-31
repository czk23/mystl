#ifndef MYSTL_ALLOC_H
#define	MYSTL_ALLOC_H

#include <cstdlib>								// malloc
#include <exception>							// bad_alloc

namespace mystl {

	enum { ALIGN = 8 };								// 分配的内存以8字节对齐
	enum { MAX_TYTES = 128 };							// 最大分配的空间为128字节
	enum { NFREELISTS = MAX_TYTES / ALIGN };			// freelists一共有16根指针

	// free_list结构体
	union freelist {
		union freelist* next;
		char user_data[1];
	};

	// alloc类的实现
	class alloc {
	private:
		static char* start_free;					// 内存池开始位置
		static char* end_free;						// 内存池结束位置
		static size_t heap_size;					// 向堆空间申请的空间
		static freelist* free_list[NFREELISTS];			// free_list数组

		static size_t round_up(size_t size);		// 将大小调整至8的倍数
		static size_t freelist_index(size_t size);		// 寻找对应大小的freelist数组下标
		static void* refill(size_t size);				// 重新填充freelist
		static char* chunk_alloc(size_t size, int& n_objs);		// 内存池分配


	public:
		static void* allocate(size_t size);
		static void deallocate(void* p, size_t size);
		static void* reallocate(void* p, size_t old_size, size_t new_size);
	};

	char* alloc::start_free = nullptr;
	char* alloc::end_free = nullptr;
	size_t alloc::heap_size = 0;
	freelist* alloc::free_list[NFREELISTS] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
							nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
							nullptr, nullptr, nullptr, nullptr, };


	// 将原来的大小 +7 再将最后三位清零(实现上使用位运算实现)
	size_t alloc::round_up(size_t size) {
		return (size + ALIGN - 1) & ~(ALIGN - 1);
	}

	size_t alloc::freelist_index(size_t size) {
		return (size + ALIGN - 1) / ALIGN - 1;
	}

	void* alloc::allocate(size_t size) {
		if (size > static_cast<size_t>(MAX_TYTES))
			return std::malloc(size);
		freelist** my_freelist = free_list + freelist_index(size);
		freelist* res = *my_freelist;
		if (nullptr == res) {
			void* r = refill(round_up(size));
			return r;
		}
		*my_freelist = res->next;
		return res;
	}

	void alloc::deallocate(void* p, size_t size) {
		if (size > static_cast<size_t>(MAX_TYTES)) {
			std::free(p);
			return;
		}
		freelist** my_freelist = free_list + freelist_index(size);
		static_cast<freelist*>(p)->next = *my_freelist;
		*my_freelist = static_cast<freelist*>(p);
	}

	void* alloc::reallocate(void* p, size_t old_size, size_t new_size) {
		deallocate(p, old_size);
		p = allocate(new_size);
		return p;
	}

	void* alloc::refill(size_t size) {
		int n_objs = 20;
		char* chunk = chunk_alloc(size, n_objs);
		if (1 == n_objs)
			return chunk;
		freelist* res = reinterpret_cast<freelist*>(chunk);
		freelist** my_freelist = free_list + freelist_index(size);
		*my_freelist = reinterpret_cast<freelist*>(chunk + size);
		freelist* p = *my_freelist;
		for (int i = 1; i < n_objs - 1; i++) {
			p->next = reinterpret_cast<freelist*>(reinterpret_cast<char*>(p) + size);
			p = p->next;
		}
		p->next = nullptr;
		return res;
	}

	char* alloc::chunk_alloc(size_t size, int& n_objs) {
		size_t bytes_need = size * n_objs;
		size_t bytes_left = end_free - start_free;
		// 内存池剩余的大小够20个区块分配
		if (bytes_left >= bytes_need) {
			char* res = start_free;
			start_free += bytes_need;
			return res;
		}
		// 内存池剩余的大小够1个以上区块分配
		if (bytes_left >= size) {
			n_objs = bytes_left / size;
			char* res = start_free;
			start_free += n_objs * size;
			return res;
		}
		// 内存池剩余的大小连一个区块都不够分配
		// 先将内存池剩余的空间连接到对应的freelist中(剩余的空间一定是8的倍数)
		if (bytes_left > 0) {
			freelist** my_freelist = free_list + freelist_index(bytes_left);
			reinterpret_cast<freelist*>(start_free)->next = *my_freelist;
			*my_freelist = reinterpret_cast<freelist*>(start_free);
		}
		// 计算需要向堆申请的大小
		size_t bytes_alloc = size * (20 + 20) + round_up(heap_size >> 4);
		start_free = static_cast<char*>(std::malloc(bytes_alloc));
		// 向堆申请空间失败
		// 先找有没有剩余可以满足此次分配的freelist，有就进行分配，没有就抛出异常
		if (nullptr == start_free) {
			size_t index = freelist_index(size);
			for (; index < NFREELISTS; index++) {
				if (free_list[index] != nullptr) {
					free_list[index] = free_list[index]->next;
					start_free = reinterpret_cast<char*>(free_list[index]);
					end_free = reinterpret_cast<char*>(free_list[index]) + (index + 1) * 8;
					return chunk_alloc(size, n_objs);
				}
			}
			end_free = nullptr;
			throw std::bad_alloc();
		}
		// 申请堆空间成功
		end_free = start_free + bytes_alloc;
		heap_size += bytes_alloc;
		return chunk_alloc(size, n_objs);
	}

	// 符合stl规范的分配器
	template<class T, class Alloc>
	class simple_alloc {
	public:
		static T* allocate() {
			return static_cast<T*>(Alloc::allocate(sizeof(T)));
		}

		static T* allocate(size_t n) {
			return 0 == n ? nullptr : static_cast<T*>(Alloc::allocate(sizeof(T) * n));
		}

		static void deallocate(void* p) {
			Alloc::deallocate(p, sizeof(T));
		}

		static void deallocate(void* p, size_t n) {
			if (n != 0)
				Alloc::deallocate(p, sizeof(T) * n);
		}
	};

}	// mystl

#endif