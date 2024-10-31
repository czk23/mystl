#ifndef MYSTL_ALLOC_H
#define	MYSTL_ALLOC_H

#include <cstdlib>								// malloc
#include <exception>							// bad_alloc

namespace mystl {

	enum { ALIGN = 8 };								// ������ڴ���8�ֽڶ���
	enum { MAX_TYTES = 128 };							// ������Ŀռ�Ϊ128�ֽ�
	enum { NFREELISTS = MAX_TYTES / ALIGN };			// freelistsһ����16��ָ��

	// free_list�ṹ��
	union freelist {
		union freelist* next;
		char user_data[1];
	};

	// alloc���ʵ��
	class alloc {
	private:
		static char* start_free;					// �ڴ�ؿ�ʼλ��
		static char* end_free;						// �ڴ�ؽ���λ��
		static size_t heap_size;					// ��ѿռ�����Ŀռ�
		static freelist* free_list[NFREELISTS];			// free_list����

		static size_t round_up(size_t size);		// ����С������8�ı���
		static size_t freelist_index(size_t size);		// Ѱ�Ҷ�Ӧ��С��freelist�����±�
		static void* refill(size_t size);				// �������freelist
		static char* chunk_alloc(size_t size, int& n_objs);		// �ڴ�ط���


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


	// ��ԭ���Ĵ�С +7 �ٽ������λ����(ʵ����ʹ��λ����ʵ��)
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
		// �ڴ��ʣ��Ĵ�С��20���������
		if (bytes_left >= bytes_need) {
			char* res = start_free;
			start_free += bytes_need;
			return res;
		}
		// �ڴ��ʣ��Ĵ�С��1�������������
		if (bytes_left >= size) {
			n_objs = bytes_left / size;
			char* res = start_free;
			start_free += n_objs * size;
			return res;
		}
		// �ڴ��ʣ��Ĵ�С��һ�����鶼��������
		// �Ƚ��ڴ��ʣ��Ŀռ����ӵ���Ӧ��freelist��(ʣ��Ŀռ�һ����8�ı���)
		if (bytes_left > 0) {
			freelist** my_freelist = free_list + freelist_index(bytes_left);
			reinterpret_cast<freelist*>(start_free)->next = *my_freelist;
			*my_freelist = reinterpret_cast<freelist*>(start_free);
		}
		// ������Ҫ�������Ĵ�С
		size_t bytes_alloc = size * (20 + 20) + round_up(heap_size >> 4);
		start_free = static_cast<char*>(std::malloc(bytes_alloc));
		// �������ռ�ʧ��
		// ������û��ʣ���������˴η����freelist���оͽ��з��䣬û�о��׳��쳣
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
		// ����ѿռ�ɹ�
		end_free = start_free + bytes_alloc;
		heap_size += bytes_alloc;
		return chunk_alloc(size, n_objs);
	}

	// ����stl�淶�ķ�����
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