#ifndef MYSTL_ITERATOR_H
#define MYSTL_ITERATOR_H

#include "iterator_base.h"
#include <iostream>

namespace mystl {
	//====================================================================================
	// insert_iterator, 包括back_insert_iterator, front_insert_iterator, insert_iterator

	// back_insert_iterator
	template<class Container>
	class back_insert_iterator {
	protected:
		Container* container;

	public:
		typedef output_iterator_tag		iterator_category;
		typedef void					value_type;
		typedef void					difference_type;
		typedef void					pointer;
		typedef void					reference;

		explicit back_insert_iterator(Container& c) : container(c) {};

		back_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
			container->push_back(value);
			return *this;
		}

		back_insert_iterator<Container>& operator*() const {
			return *this;
		}

		back_insert_iterator<Container>& operator++() const {
			return *this;
		}

		back_insert_iterator<Container>& operator++(int) const {
			return *this;
		}
	};

	// 辅助函数，返回一个back_insert_iterator对象
	template<class Container>
	inline back_insert_iterator<Container> back_inserter(const Container& c) {
		return back_insert_iterator<Container>(c);
	}

	// front_insert_iterator
	template<class Container>
	class front_insert_iterator {
	protected:
		Container* container;

	public:
		typedef output_iterator_tag		iterator_category;
		typedef void					value_type;
		typedef void					difference_type;
		typedef void					pointer;
		typedef void					reference;

		explicit front_insert_iterator(Container& c) : container(c) {};

		front_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
			container->push_front(value);
			return *this;
		}

		front_insert_iterator<Container>& operator*() const {
			return *this;
		}

		front_insert_iterator<Container>& operator++() const {
			return *this;
		}

		front_insert_iterator<Container>& operator++(int) const {
			return *this;
		}
	};

	// 辅助函数, 返回一个front_insert_iterator对象
	template<class Container>
	front_insert_iterator<Container> front_inserter(const Container& c) {
		return front_insert_iterator<Container>(c);
	}

	// insert_iterator
	template<class Container>
	class insert_iterator {
	protected:
		Container* container;
		typename Container::iterator iter;

	public:
		typedef output_iterator_tag		iterator_category;
		typedef void					value_type;
		typedef void					difference_type;
		typedef void					pointer;
		typedef void					reference;

		explicit insert_iterator(Container& c, typename Container::iterator it) : container(c), iter(it) {};

		insert_iterator<Container>& operator=(const typename Container::value_type& value) {
			iter = container->insert(iter, value);
			++iter;
			return *this;
		}

		insert_iterator<Container>& operator*() const {
			return *this;
		}

		insert_iterator<Container>& operator++() const {
			return *this;
		}

		insert_iterator<Container>& operator++(int) const {
			return *this;
		}
	};

	// 辅助函数, 返回一个insert_iterator对象
	template<class Container>
	insert_iterator<Container> inserter(Container& c, typename Container::iterator& i) {
		return insert_iterator<Container>(c, i);
	}

	//=======================================================================================
	// reverse_iterator
	template<class Iterator>
	class reverse_iterator {
	protected:
		Iterator current;

	public:
		typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;
		typedef typename iterator_traits<Iterator>::value_type			value_type;
		typedef typename iterator_traits<Iterator>::difference_type		difference_type;
		typedef typename iterator_traits<Iterator>::pointer				pointer;
		typedef typename iterator_traits<Iterator>::reference			reference;

		reverse_iterator() = default;

		explicit reverse_iterator(Iterator iter) : current(iter) {};

		reverse_iterator(const reverse_iterator<Iterator>& rhs) : current(rhs.current) {};

		Iterator base() const {
			return current;
		}
		// 注意不能修改current的值
		reference operator*() const {
			Iterator temp = current;
			return *(--temp);
		}

		pointer operator->() const {
			return &(operator*());
		}

		reverse_iterator<Iterator>& operator++() {
			--current;
			return *this;
		}

		reverse_iterator<Iterator>& operator--() {
			++current;
			return *this;
		}

		reverse_iterator<Iterator> operator++(int) {
			reverse_iterator<Iterator> temp = *this;
			--current;
			return temp;
		}

		reverse_iterator<Iterator> operator--(int) {
			reverse_iterator<Iterator> temp = *this;
			++current;
			return temp;
		}

		reverse_iterator<Iterator> operator+(const difference_type& n) const {
			return reverse_iterator<Iterator>(current - n);
		}

		reverse_iterator<Iterator> operator-(const difference_type& n) const {
			return reverse_iterator<Iterator>(current + n);
		}

		reverse_iterator<Iterator>& operator+=(const difference_type& n) {
			current -= n;
			return *this;
		}

		reverse_iterator<Iterator>& operator-=(const difference_type& n) {
			current += n;
			return *this;
		}

		reference operator[](const difference_type& n) const {
			return *(*this + n);
		}
	};

	// 对一些比较运算符进行重载
	template<class Iterator>
	inline bool operator==(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return lhs.base() == rhs.base();
	}

	template<class Iterator>
	inline bool operator!=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return !(lhs == rhs);
	}

	template<class Iterator>
	inline bool operator<(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return lhs.base() < rhs.base();
	}

	template<class Iterator>
	inline bool operator>(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return rhs < lhs;
	}

	template<class Iterator>
	inline bool operator<=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return !(lhs < rhs);
	}

	template<class Iterator>
	inline bool operator>=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return !(rhs < lhs);
	}

	// 计算两个reverse_iterator之间的距离
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type
		operator-(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return rhs.base() - lhs.base();
	}

	// 对于number + reverse_iterator进行重载
	template<class Iterator>
	inline reverse_iterator<Iterator> operator+(const typename reverse_iterator<Iterator>::difference_type& n,
		const reverse_iterator<Iterator>& rhs) {
		return reverse_iterator<Iterator>(rhs.base() - n);
	}

	//===============================================================================================
	// iostream, 包括istream和ostream

	// istream
	template<class T, class Distance = ptrdiff_t>
	class istream_iterator {
	protected:
		std::istream* stream;
		T value;
		bool end_mark;
		void read() {
			end_mark = (*stream) ? true : false;
			if (!end_mark) {
				*stream >> value;
				end_mark = (*stream) ? true : false;
			}
		}

	public:
		typedef input_iterator_tag		iterator_category;
		typedef T						value_type;
		typedef Distance				difference_type;
		typedef const T* pointer;
		typedef const T& reference;

		istream_iterator() : stream(&std::cin), end_mark(false) {};
		istream_iterator(std::istream& s) : stream(&s) { read(); };

		reference operator*() const {
			return value;
		}

		pointer operator->() const {
			return &(operator*());
		}

		istream_iterator<T, Distance>& operator++() {
			read();
			return *this;
		}

		istream_iterator<T, Distance> operator++(int) {
			istream_iterator<T, Distance> temp = *this;
			read();
			return temp;
		}
	};

	// ostream
	template<class T>
	class ostream_iterator {
	protected:
		std::ostream* stream;
		const char* _string;

	public:
		typedef output_iterator_tag		iterator_category;
		typedef void					value_type;
		typedef void					difference_type;
		typedef void					pointer;
		typedef void					reference;

		ostream_iterator(std::ostream& s) : stream(&s), _string(nullptr) {};
		ostream_iterator(std::ostream& s, const std::string* c) : stream(&s), _string(c) {};

		ostream_iterator<T>& operator=(const T& value)const {
			*stream << value;
			if (_string)
				*stream << _string;
			return *this;
		}

		ostream_iterator<T>& operator*()const {
			return *this;
		}

		ostream_iterator<T>& operator++()const {
			return *this;
		}

		ostream_iterator<T>& operator++(int)const {
			return *this;
		}
	};

}	// mystl

#endif