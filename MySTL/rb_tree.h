#ifndef MYSTL_RB_TREE
#define MYSTL_RB_TREE

#include "iterator.h"
#include "memory.h"
#include "mystl_pair.h"

namespace mystl {

	// 定义红黑树结点颜色
	typedef bool rb_tree_color_type;
	static constexpr rb_tree_color_type rb_tree_red = false;
	static constexpr rb_tree_color_type rb_tree_black = true;

	template <class T> struct rb_tree_node_base;
	template <class T> struct rb_tree_node;

	template <class T> struct rb_tree_iterator;
	template <class T> struct rb_tree_const_iterator;

//--------------------------------------------------红黑树结点设计-------------------------------------------------
	template<class T>
	struct rb_tree_node;

	template<class T>
	struct rb_tree_node_base {
		typedef rb_tree_color_type				color_type;
		typedef rb_tree_node_base<T>*			base_ptr;
		typedef rb_tree_node<T>*				node_ptr;

		base_ptr parent;
		base_ptr lchild;
		base_ptr rchild;
		color_type color;

		base_ptr get_base_ptr() {
			return &*this;
		}

		node_ptr get_node_ptr() {
			return reinterpret_cast<node_ptr>(&*this);
		}

		node_ptr& get_node_ref() {
			return reinterpret_cast<node_ptr&>(*this);
		}
	};

	template<class T>
	struct rb_tree_node : public rb_tree_node_base<T> {
		typedef rb_tree_node_base<T>*			base_ptr;
		typedef rb_tree_node<T>*				node_ptr;

		T value;

		base_ptr get_base_ptr() {
			return static_cast<node_ptr>(&*this);
		}

		node_ptr get_node_ptr() {
			return &*this;
		}
	};

//--------------------------------------------------红黑树迭代器设计-------------------------------------------------
	template<class T>
	struct rb_tree_iterator_base : public iterator<bidirectional_iterator_tag, T> {
		typedef rb_tree_node_base<T>*			base_ptr;

		base_ptr node;

		void increment() {
			// 右孩子不为空
			if (node->rchild) {
				node = rb_tree_min(node->rchild);
			}
			// 右孩子为空，下一结点为最左上结点的父结点
			// 例: node的下一结点为p结点
			//			 p
			//			/
			//		   q
			//          \
			//          ...
			//			 \
			//			node
			//			/  \	
			//		  ... nullptr
			else {
				base_ptr y = node->parent;
				while (y->rchild == node) {
					node = y;
					y = y->parent;
				}
				// 应对只有根结点的特殊情况
				if (node->rchild != y)
					node = y;
			}
		}

		void decrement() {
			// 结点为头结点，前一结点为最大结点
			if (node->parent->parent == node && rb_tree_is_red(node))
				node = node->rchild;
			// 左孩子不为空
			else if (node->lchild != nullptr) {
				node = rb_tree_max(node->lchild);
			}
			// 左孩子为空，前一结点为最右上结点的父结点
			// 例: node的下一结点为p结点
			//			   p
			//			    \
			//		         q
			//              /
			//            ...
			//			  /
			//			node
			//			/  \	
			//	   nullptr ...
			else {
				base_ptr y = node->parent;
				while (y->lchild == node) {
					node = y;
					y = y->parent;
				}
				node = y;
			}
		}

		bool operator==(const rb_tree_iterator_base& rhs) const {
			return node == rhs.node;
		}

		bool operator!=(const rb_tree_iterator_base& rhs) const {
			return node != rhs.node;
		}
	};

	template<class T>
	struct rb_tree_iterator : public rb_tree_iterator_base<T> {
		typedef T										value_type;
		typedef T*										pointer;
		typedef T&										reference;
		typedef rb_tree_node_base<T>*					base_ptr;
		typedef rb_tree_node<T>*						node_ptr;
		typedef rb_tree_iterator<T>						iterator;
		typedef rb_tree_const_iterator<T>				const_iterator;
		typedef iterator								self;

		using rb_tree_iterator_base<T>::node;

		rb_tree_iterator() = default;

		rb_tree_iterator(base_ptr x) {
			node = x;
		};

		rb_tree_iterator(node_ptr x) {
			node = x;
		};

		rb_tree_iterator(const iterator& x) {
			node = x.node;
		};

		rb_tree_iterator(const const_iterator& x) {
			node = x.node;
		};


		reference operator*() const{
			return node->get_node_ptr()->value;
		}

		pointer operator->() const {
			return &(operator*());
		}

		self& operator++() {
			this->increment();
			return *this;
		}

		self operator++(int) {
			self temp = *this;
			this->increment();
			return temp;
		}

		self& operator--() {
			this->decrement();
			return *this;
		}

		self operator--(int) {
			self temp = *this;
			this->decrement();
			return temp;
		}
	};

	template<class T>
	struct rb_tree_const_iterator : public rb_tree_iterator_base<T> {
		typedef T											value_type;
		typedef T&											reference;
		typedef T*											pointer;
		typedef rb_tree_node_base<T>*						base_ptr;
		typedef rb_tree_node<T>*							node_ptr;
		typedef rb_tree_iterator<T>							iterator;
		typedef rb_tree_const_iterator<T>					const_iterator;
		typedef const_iterator								self;

		using rb_tree_iterator_base<T>::node;


		rb_tree_const_iterator() = default;

		rb_tree_const_iterator(base_ptr x) { 
			node = x; 
		};

		rb_tree_const_iterator(node_ptr x) { 
			node = x; 
		};

		rb_tree_const_iterator(const iterator& x) { 
			node = x.node; 
		};

		rb_tree_const_iterator(const const_iterator& x) { 
			node = x.node; 
		};

		reference operator*() const {
			return node.get_node_ptr()->value;
		}

		pointer operator->() const {
			return &(operator*());
		}

		self& operator++() {
			this->increment();
			return *this;
		}

		self operator++(int) {
			self temp = *this;
			this->increment();
			return temp;
		}

		self& operator--() {
			this->decrement();
			return *this;
		}

		self operator--(int) {
			self temp = *this;
			this->decrement();
			return temp;
		}
	};


//--------------------------------------------------红黑树算法设计-------------------------------------------------
	template<class Node_ptr>
	Node_ptr rb_tree_min(Node_ptr node) {
		while (node->lchild)
			node = node->lchild;
		return node;
	}

	template<class Node_ptr>
	Node_ptr rb_tree_max(Node_ptr node) {
		while (node->rchild)
			node = node->rchild;
		return node;
	}

	template<class Node_ptr>
	bool rb_tree_is_lchild(Node_ptr node) {
		return node == node->parent->lchild;
	}

	template<class Node_ptr>
	bool rb_tree_is_rchild(Node_ptr node) {
		return node == node->parent->rchild;
	}

	template<class Node_ptr>
	bool rb_tree_is_red(Node_ptr node) {
		return node->color == rb_tree_red;
	}

	template<class Node_ptr>
	bool rb_tree_is_black(Node_ptr node) {
		return node->color == rb_tree_black;
	}

	template<class Node_ptr>
	void rb_tree_set_red(Node_ptr& node) {
		node->color = rb_tree_red;
	}

	template<class Node_ptr>
	void rb_tree_set_black(Node_ptr& node) {
		node->color = rb_tree_black;
	}

	template<class Node_ptr>
	Node_ptr rb_tree_next(Node_ptr node) {
		if (node->rchild) {
			return rb_tree_min(node->rchild);
		}
		while (rb_tree_is_rchild(node))
			node = node->parent;
		return node->parent;
	}

/*---------------------------------------*\
|       p                         p       |
|      / \                       / \      |
|     x   d    rotate left      y   d     |
|    / \       ===========>    / \        |
|   a   y                     x   c       |
|      / \                   / \          |
|     b   c                 a   b         |
\*---------------------------------------*/
// 左旋，参数一为左旋点，参数二为根节点
	template<class Node_ptr>
	void rb_tree_rotate_left(Node_ptr x, Node_ptr& root) {
		Node_ptr y = x->rchild;
		x->rchild = y->lchild;
		if (y->lchild) {
			y->lchild->parent = x;
		}
		y->parent = x->parent;
		if (root == x)
			root = y;
		else if (rb_tree_is_lchild(x))
			x->parent->lchild = y;
		else
			x->parent->rchild = y;
		y->lchild = x;
		x->parent = y;
	}

/*----------------------------------------*\
|     p                         p          |
|    / \                       / \         |
|   d   x      rotate right   d   y        |
|      / \     ===========>      / \       |
|     y   a                     b   x      |
|    / \                           / \     |
|   b   c                         c   a    |
\*----------------------------------------*/
// 右旋，参数一为右旋点，参数二为根节点
	template<class Node_ptr>
	void rb_tree_rotate_right(Node_ptr x, Node_ptr& root) {
		Node_ptr y = x->lchild;
		x->lchild = y->rchild;
		if (y->rchild)
			y->rchild->parent = x;
		y->parent = x->parent;
		if (root == x)
			root = y;
		else if (rb_tree_is_lchild(x))
			x->parent->lchild = y;
		else
			x->parent->rchild = y;
		y->rchild = x;
		x->parent = y;
	}

// 插入节点后使 rb tree 重新平衡，参数一为新增节点，参数二为根节点
//
// case 1: 新增节点位于根节点，令新增节点为黑
// case 2: 新增节点的父节点为黑，没有破坏平衡，直接返回
// case 3: 父节点和叔叔节点都为红，令父节点和叔叔节点为黑，祖父节点为红，
//         然后令祖父节点为当前节点，继续处理
// case 4: 父节点为红，叔叔节点为 NIL 或黑色，父节点为左（右）孩子，当前节点为右（左）孩子，
//         让父节点成为当前节点，再以当前节点为支点左（右）旋
// case 5: 父节点为红，叔叔节点为 NIL 或黑色，父节点为左（右）孩子，当前节点为左（右）孩子，
//         让父节点变为黑色，祖父节点变为红色，以祖父节点为支点右（左）旋
	template<class Node_ptr>
	void rb_tree_insert_rebalance(Node_ptr x, Node_ptr& root) {
		rb_tree_set_red(x);
		while (x != root && rb_tree_is_red(x->parent)) {
			// 父结点是左孩子
			if (rb_tree_is_lchild(x->parent)) {
				Node_ptr uncle = x->parent->parent->rchild;
				if (uncle != nullptr && rb_tree_is_red(uncle)) {
					rb_tree_set_black(x->parent);
					rb_tree_set_black(uncle);
					rb_tree_set_red(x->parent->parent);
					x = x->parent->parent;
				}
				else {
					if (rb_tree_is_rchild(x)) {
						x = x->parent;
						rb_tree_rotate_left(x, root);
					}
					rb_tree_set_black(x->parent);
					rb_tree_set_red(x->parent->parent);
					rb_tree_rotate_right(x->parent->parent, root);
					break;
				}
			}
			// 父结点是右孩子
			else {
				Node_ptr uncle = x->parent->parent->lchild;
				if (uncle != nullptr && rb_tree_is_red(uncle)) {
					rb_tree_set_black(x->parent);
					rb_tree_set_black(uncle);
					rb_tree_set_red(x->parent->parent);
					x = x->parent->parent;
				}
				else {
					if (rb_tree_is_lchild(x)) {
						x = x->parent;
						rb_tree_rotate_right(x, root);
					}
					rb_tree_set_black(x->parent);
					rb_tree_set_red(x->parent->parent);
					rb_tree_rotate_left(x->parent->parent, root);
					break;
				}
			}
		}
		rb_tree_set_black(root);
	}

// 删除节点后使 rb tree 重新平衡，参数一为要删除的节点，参数二为根节点，参数三为最小节点，参数四为最大节点
// 太难了，红黑树里面最难的部分，直接copy的这个函数
// 参考博客: http://blog.csdn.net/v_JULY_v/article/details/6105630
//          http://blog.csdn.net/v_JULY_v/article/details/6109153
	template <class NodePtr>
	NodePtr rb_tree_erase_rebalance(NodePtr z, NodePtr& root, NodePtr& leftmost, NodePtr& rightmost)
	{
		// y 是可能的替换节点，指向最终要删除的节点
		auto y = (z->lchild == nullptr || z->rchild == nullptr) ? z : rb_tree_next(z);
		// x 是 y 的一个独子节点或 NIL 节点
		auto x = (y->lchild != nullptr) ? y->lchild : y->rchild;
		// xp 为 x 的父节点
		NodePtr xp = nullptr;

		// y != z 说明 z 有两个非空子节点，此时 y 指向 z 右子树的最左节点，x 指向 y 的右子节点。
		// 用 y 顶替 z 的位置，用 x 顶替 y 的位置，最后用 y 指向 z
		if (y != z)
		{
			z->lchild->parent = y;
			y->lchild = z->lchild;

			// 如果 y 不是 z 的右子节点，那么 z 的右子节点一定有左孩子
			if (y != z->rchild)
			{ // x 替换 y 的位置
				xp = y->parent;
				if (x != nullptr)
					x->parent = y->parent;

				y->parent->lchild = x;
				y->rchild = z->rchild;
				z->rchild->parent = y;
			}
			else
			{
				xp = y;
			}

			// 连接 y 与 z 的父节点 
			if (root == z)
				root = y;
			else if (rb_tree_is_lchild(z))
				z->parent->lchild = y;
			else
				z->parent->rchild = y;
			y->parent = z->parent;
			mystl::swap(y->color, z->color);
			y = z;
		}
		// y == z 说明 z 至多只有一个孩子
		else
		{
			xp = y->parent;
			if (x)
				x->parent = y->parent;

			// 连接 x 与 z 的父节点
			if (root == z)
				root = x;
			else if (rb_tree_is_lchild(z))
				z->parent->lchild = x;
			else
				z->parent->rchild = x;

			// 此时 z 有可能是最左节点或最右节点，更新数据
			if (leftmost == z)
				leftmost = x == nullptr ? xp : rb_tree_min(x);
			if (rightmost == z)
				rightmost = x == nullptr ? xp : rb_tree_max(x);
		}

		// 此时，y 指向要删除的节点，x 为替代节点，从 x 节点开始调整。
		// 如果删除的节点为红色，树的性质没有被破坏，否则按照以下情况调整（x 为左子节点为例）：
		// case 1: 兄弟节点为红色，令父节点为红，兄弟节点为黑，进行左（右）旋，继续处理
		// case 2: 兄弟节点为黑色，且两个子节点都为黑色或 NIL，令兄弟节点为红，父节点成为当前节点，继续处理
		// case 3: 兄弟节点为黑色，左子节点为红色或 NIL，右子节点为黑色或 NIL，
		//         令兄弟节点为红，兄弟节点的左子节点为黑，以兄弟节点为支点右（左）旋，继续处理
		// case 4: 兄弟节点为黑色，右子节点为红色，令兄弟节点为父节点的颜色，父节点为黑色，兄弟节点的右子节点
		//         为黑色，以父节点为支点左（右）旋，树的性质调整完成，算法结束
		if (!rb_tree_is_red(y))
		{ // x 为黑色时，调整，否则直接将 x 变为黑色即可
			while (x != root && (x == nullptr || !rb_tree_is_red(x)))
			{
				if (x == xp->lchild)
				{ // 如果 x 为左子节点
					auto brother = xp->rchild;
					if (rb_tree_is_red(brother))
					{ // case 1
						rb_tree_set_black(brother);
						rb_tree_set_red(xp);
						rb_tree_rotate_left(xp, root);
						brother = xp->rchild;
					}
					// case 1 转为为了 case 2、3、4 中的一种
					if ((brother->lchild == nullptr || !rb_tree_is_red(brother->lchild)) &&
						(brother->rchild == nullptr || !rb_tree_is_red(brother->rchild)))
					{ // case 2
						rb_tree_set_red(brother);
						x = xp;
						xp = xp->parent;
					}
					else
					{
						if (brother->rchild == nullptr || !rb_tree_is_red(brother->rchild))
						{ // case 3
							if (brother->lchild != nullptr)
								rb_tree_set_black(brother->lchild);
							rb_tree_set_red(brother);
							rb_tree_rotate_right(brother, root);
							brother = xp->rchild;
						}
						// 转为 case 4
						brother->color = xp->color;
						rb_tree_set_black(xp);
						if (brother->rchild != nullptr)
							rb_tree_set_black(brother->rchild);
						rb_tree_rotate_left(xp, root);
						break;
					}
				}
				else  // x 为右子节点，对称处理
				{
					auto brother = xp->lchild;
					if (rb_tree_is_red(brother))
					{ // case 1
						rb_tree_set_black(brother);
						rb_tree_set_red(xp);
						rb_tree_rotate_right(xp, root);
						brother = xp->lchild;
					}
					if ((brother->lchild == nullptr || !rb_tree_is_red(brother->lchild)) &&
						(brother->rchild == nullptr || !rb_tree_is_red(brother->rchild)))
					{ // case 2
						rb_tree_set_red(brother);
						x = xp;
						xp = xp->parent;
					}
					else
					{
						if (brother->lchild == nullptr || !rb_tree_is_red(brother->lchild))
						{ // case 3
							if (brother->rchild != nullptr)
								rb_tree_set_black(brother->rchild);
							rb_tree_set_red(brother);
							rb_tree_rotate_left(brother, root);
							brother = xp->lchild;
						}
						// 转为 case 4
						brother->color = xp->color;
						rb_tree_set_black(xp);
						if (brother->lchild != nullptr)
							rb_tree_set_black(brother->lchild);
						rb_tree_rotate_right(xp, root);
						break;
					}
				}
			}
			if (x != nullptr)
				rb_tree_set_black(x);
		}
		return y;
	}


// 红黑树
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc = alloc>
class rb_tree {
public:
	// typedefs
	typedef rb_tree_node_base<Value>*							base_ptr;
	typedef rb_tree_node<Value>*								node_ptr;
	typedef rb_tree_node_base<Value>							base_type;
	typedef rb_tree_node<Value>									node_type;

	typedef Key													key_type;
	typedef Value												value_type;
	typedef Compare												key_compare;

	typedef simple_alloc<Value, Alloc>							allocator_type;
	typedef simple_alloc<Value, Alloc>							data_allocator;
	typedef simple_alloc<base_type, Alloc>						base_allocator;
	typedef simple_alloc<node_type, Alloc>						node_allocator;

	typedef value_type*											pointer;
	typedef const value_type*									const_pointer;
	typedef value_type&											reference;
	typedef const value_type&									const_reference;

	typedef size_t												size_type;
	typedef ptrdiff_t											difference_type;

	typedef rb_tree_iterator<Value>								iterator;
	typedef rb_tree_const_iterator<Value>						const_iterator;
	typedef reverse_iterator<iterator>							reverse_iterator;
	typedef mystl::reverse_iterator<const_iterator>				const_reverse_iterator;

protected:
	// 三个成员变量
	base_ptr header;
	size_type node_count;
	key_compare key_comp;

protected:
	allocator_type get_allocator() const {
		return node_allocator();
	}

	static const key_type& get_key(base_ptr& node) {
		return Keyofvalue()(static_cast<node_ptr>(node)->value);
	}

	static const key_type& get_key(node_ptr& node) {
		return Keyofvalue()(node->value);
	}

	static const value_type& get_value(base_ptr& node) {
		return static_cast<node_ptr>(node)->value;
	}

	static const value_type& get_value(node_ptr& node) {
		return node->value;
	}

	base_ptr& root() const {
		return header->parent;
	}

	node_ptr& left(node_ptr& rhs) const {
		return reinterpret_cast<node_ptr&>(rhs->lchild);
	}

	node_ptr& left(base_ptr& rhs) const {
		return reinterpret_cast<node_ptr&>(rhs->lchild);
	}

	node_ptr& right(const node_ptr& rhs) const {
		return reinterpret_cast<node_ptr&>(rhs->rchild);
	}

	node_ptr& right(const base_ptr& rhs) const {
		return reinterpret_cast<node_ptr&>(rhs->rchild);
	}

	base_ptr& leftmost() {
		return header->lchild;
	}

	node_ptr& leftmost() const {
		return reinterpret_cast<node_ptr&>(header->lchild);
	}

	base_ptr& rightmost() {
		return header->rchild;
	}

	node_ptr& rightmost() const {
		return reinterpret_cast<node_ptr&>(header->rchild);
	}


public:
	// 构造、复制和析构函数
	rb_tree() : header(nullptr), node_count(0) {
		header = get_node();
		empty_initialize();
	}

	rb_tree(key_compare comp) : header(nullptr), node_count(0), key_comp(comp) {
		header = get_node();
		empty_initialize();
	}

	rb_tree(const rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& rhs) : header(nullptr), node_count(0), 
		key_comp(rhs.key_comp) {
		header = get_node();
		if (nullptr == rhs.root())
			empty_initialize();
		else {
			rb_tree_set_red(header);
			root() = copy_from(rhs.root(), header);
			leftmost() = rb_tree_min(root());
			rightmost() = rb_tree_max(root());
		}
		node_count = rhs.node_count;
	}

	rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& operator=(rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& rhs) {
		if (*this == rhs)
			return *this;
		clear();
		key_comp = rhs.key_comp;
		node_count = 0;
		if (0 == rhs.node_count) {
			root() = nullptr;
			leftmost() = header;
			rightmost() = header;
		}
		else {
			root() = copy_from(rhs.root(), header);
			leftmost() = rb_tree_min(root());
			rightmost() = rb_tree_max(root());
			node_count = rhs.node_count;
		}
		return *this;
	}

	~rb_tree() {
		clear();
	}


	// 迭代器相关函数
	iterator begin() {
		return leftmost();
	}
	
	const_iterator begin() const {
		return leftmost();
	}

	iterator end(){
		return header;
	}

	const_iterator end() const {
		return header;
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
		return node_count == 0;
	}

	size_type size() const {
		return node_count;
	}

	size_type max_size() const {
		return static_cast<size_type>(-1);
	}

	void swap(rb_tree& rhs);


	// 结点相关函数
	node_ptr create_node(value_type value);

	node_ptr clone_node(base_ptr x);

	void destroy_node(node_ptr x);


	// 插入删除函数
	iterator insert_equal(const value_type& value);

	iterator insert_equal(iterator position, const value_type& value);

	template<class InputIterator>
	void insert_equal(InputIterator first, InputIterator last);

	mystl::pair<iterator, bool> insert_unique(const value_type& value);

	iterator insert_unique(iterator position, const value_type& value);

	template<class InputIterator>
	void insert_unique(InputIterator first, InputIterator last);

	iterator insert(base_ptr x, base_ptr y, const value_type& value);

	void erase(iterator position);

	size_type erase(const key_type& k);

	void erase(iterator first, iterator last);

	void erase(node_ptr x);

	void clear();


	// 红黑树相关函数
	iterator find(const key_type& k);

	const_iterator find(const key_type& k) const;

	size_type count(const key_type& k) const;

	iterator lower_bound(const key_type& k);

	const_iterator lower_bound(const key_type& k) const;

	iterator upper_bound(const key_type& k);

	const_iterator upper_bound(const key_type& k) const;

	mystl::pair<iterator, iterator> equal_range(const key_type& k);

	mystl::pair<const_iterator, const_iterator> equal_range(const key_type& k) const;

private:
	// 辅助函数
	node_ptr get_node();

	void put_node(node_ptr node);

	void empty_initialize();

	node_ptr copy_from(base_ptr x, base_ptr p);

	void erase_since(base_ptr x);

	bool rb_tree_verify() const;

};


//------------------------------------------------结点相关函数------------------------------------------------
// 创建一个结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::node_ptr
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::create_node(value_type value) {
	node_ptr temp = get_node();
	try {
		mystl::construct(&temp->value, value);
	}
	catch (...) {
		put_node(temp);
	}
	return temp;
}

// 克隆一个结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::node_ptr
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::clone_node(base_ptr x) {
	node_ptr temp = create_node(x->get_node_ptr()->value);
	temp->color = x->color;
	temp->lchild = nullptr;
	temp->rchild = nullptr;
	return temp;
}

// 清除一个结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
void rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::destroy_node(node_ptr x) {
	mystl::destroy(&x->value);
	put_node(x);
}


//--------------------------------------------------插入删除函数------------------------------------------------
// 给定值，插入一个结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::iterator
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::insert_equal(const value_type& value) {
	node_ptr x = static_cast<node_ptr>(root());
	node_ptr y = static_cast<node_ptr>(header);
	while (x) {
		y = x;
		x = key_comp(Keyofvalue()(value), Keyofvalue()(x->value)) ? left(x) : right(x);
	}
	return insert(x, y, value);
}

// 给定插入位置和值，插入一个结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::iterator
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::insert_equal(iterator position, const value_type& value) {
	if (position.node == leftmost()) {
		if (size() > 0 && !key_comp((get_key(position.node)), Keyofvalue()(value)))
			return insert(position.node, position.node, value);
		else
			return insert_equal(value);
	}
	else if (position.node == header) {
		if (!key_comp(Keyofvalue()(value), get_key(rightmost())))
			return insert(0, rightmost(), value);
		else
			return insert_equal(value);
	}
	else {
		iterator before = position;
		--before;
		if (!key_comp(Keyofvalue()(value), get_key(before.node)) && !key_comp(get_key(position.node), Keyofvalue()(value))) {
			if (nullptr == before.node->rchild)
				return insert(0, before.node, value);
			else
				return insert(position.node, position.node, value);
		}
		else
			return insert_equal(value);
	}
}

// 模板函数，给定一个区间进行插入
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
template<class InputIterator>
void rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::insert_equal(InputIterator first, InputIterator last) {
	for (; first != last; ++first)
		insert_equal(*first);
}

// 给定值，插入一个结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
mystl::pair<typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::insert_unique(const value_type& value) {
	node_ptr x = static_cast<node_ptr>(root());
	node_ptr y = static_cast<node_ptr>(header);
	bool comp = true;
	while (x) {
		y = x;
		comp = key_comp(Keyofvalue()(value), get_key(x));
		x = comp ? left(x) : right(x);
	}
	iterator j = iterator(y);
	if (comp) {
		if (j == begin())
			return mystl::pair<iterator, bool>(insert(x, y, value), true);
		else
			--j;
	}
	if (key_comp(get_key(j.node), Keyofvalue()(value)))
		return mystl::pair<iterator, bool>(insert(x, y, value), true);
	return mystl::pair<iterator, bool>(j, false);
}

// 给定插入位置和值，插入一个结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::iterator
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::insert_unique(iterator position, const value_type& value) {
	if (position.node == leftmost()) {
		if (size() > 0 && key_comp(Keyofvalue()(value), get_key(position.node)))
			return insert(position.node, position.node, value);
		else
			return insert_unique(value).first;
	}
	else if (position.node == header) {
		if (key_comp(get_key(rightmost()), Keyofvalue()(value)))
			return insert(0, rightmost(), value);
		else
			return insert_unique(value).first;
	}
	else {
		iterator before = position;
		--before;
		if (key_comp(get_key(before.node), Keyofvalue()(value)) && key_comp(Keyofvalue()(value), get_key(position.node))) {
			if (nullptr == before.node->rchild)
				return insert(0, before.node, value);
			else
				return insert(position.node, position.node, value);
		}
		else
			return insert_unique(value).first;
	}
}

// 模板函数，给定一个区间进行插入
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
template<class InputIterator>
void rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::insert_unique(InputIterator first, InputIterator last) {
	for (; first != last; ++first)
		insert_unique(*first);
}

// 给定插入位置、父结点和值，插入一个结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::iterator
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::insert(base_ptr old_x, base_ptr old_y, const value_type& value) {
	node_ptr x = static_cast<node_ptr>(old_x);
	node_ptr y = static_cast<node_ptr>(old_y);
	node_ptr z;
	if (y == header || x != nullptr || key_comp(Keyofvalue()(value), get_key(y))) {
		z = create_node(value);
		left(y) = z;
		if (y == header) {
			root() = z;
			rightmost() = z;
		}
		else if (leftmost() == y)
			leftmost() = z;
	}
	else {
		z = create_node(value);
		right(y) = z;
		if (rightmost() == y)
			rightmost() = z;
	}
	z->parent = y;
	z->lchild = nullptr;
	z->rchild = nullptr;
	rb_tree_insert_rebalance(z->get_base_ptr(), header->parent);
	++node_count;
	return iterator(z);
}

// 删除一个结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
void rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::erase(iterator position) {
	base_ptr y = rb_tree_erase_rebalance(position.node, header->parent, header->lchild, header->rchild);
	destroy_node(static_cast<node_ptr>(y));
	--node_count;
}

// 给定一个值，删除结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::size_type
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::erase(const key_type& key) {
	mystl::pair<iterator, iterator> p = equal_range(key);
	size_type n = mystl::distance(p.first, p.second);
	erase(p.first, p.second);
	return n;
}

// 给定一个区间，删除结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
void rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::erase(iterator first, iterator last) {
	if (first == begin() && last == end())
		clear();
	else {
		while (first != last)
			erase(first++);
	}
}

// 删除以x为根的子树
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
void rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::erase(node_ptr x) {
	while (x) {
		erase(static_cast<node_ptr>(x->rchild));
		node_ptr y = static_cast<node_ptr>(x->lchild);
		destroy_node(x);
		x = y;
	}
}

// 删除整棵树
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
void rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::clear() {
	if (node_count != 0) {
		erase(static_cast<node_ptr>(root()));
		leftmost() = header;
		rightmost() = header;
		root() = nullptr;
		node_count = 0;
	}
}


//--------------------------------------------------红黑树相关函数------------------------------------------------
// 查找红黑树中是否有key值为k的结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::iterator
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::find(const key_type& k) {
	node_ptr y = static_cast<node_ptr>(header);
	node_ptr x = static_cast<node_ptr>(root());
	while (x) {
		if (!key_comp(get_key(x), k)) {
			y = x;
			x = left(x);
		}
		else
			x = right(x);
	}
	iterator j = iterator(y);
	if (j == end() || key_comp(k, get_key(y)))
		return end();
	return j;
}

// 查找红黑树中是否有key值为k的结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::const_iterator
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::find(const key_type& k) const {
	node_ptr y = static_cast<node_ptr>(header);
	node_ptr x = static_cast<node_ptr>(root());
	while (x) {
		if (!key_comp(get_key(x), k)) {
			y = x;
			x = left(x);
		}
		else
			x = right(x);
	}
	const_iterator j = const_iterator(y);
	if (j == end() || key_comp(k, get_key(y)))
		return end();
	return j;
}

// 统计红黑树中有多少结点的key为k
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::size_type
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::count(const key_type& k) const {
	mystl::pair<iterator, iterator> p = equal_range(k);
	size_type n = mystl::distance(p.first, p.second);
	return n;
}

// 查找红黑树中第一个大于等于k的结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::iterator
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::lower_bound(const key_type& k) {
	node_ptr y = static_cast<node_ptr>(header);
	node_ptr x = static_cast<node_ptr>(root());
	while (x) {
		if (!key_comp(get_key(x), k)) {
			y = x;
			x = left(x);
		}
		else
			x = right(x);
	}
	return iterator(y);
}

// 查找红黑树中第一个大于等于k的结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::const_iterator
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::lower_bound(const key_type& k) const {
	node_ptr y = static_cast<node_ptr>(header);
	node_ptr x = static_cast<node_ptr>(root());
	while (x) {
		if (!key_comp(get_key(x), k)) {
			y = x;
			x = left(x);
		}
		else
			x = right(x);
	}
	return const_iterator(y);
}

// 查找红黑树中第一个大于k的结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::iterator
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::upper_bound(const key_type& k) {
	node_ptr y = static_cast<node_ptr>(header);
	node_ptr x = static_cast<node_ptr>(root());
	while (x) {
		if (key_comp(k, get_key(x))) {
			y = x;
			x = left(x);
		}
		else
			x = right(x);
	}
	return iterator(y);
}

// 查找红黑树中第一个大于k的结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::const_iterator
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::upper_bound(const key_type& k) const {
	node_ptr y = static_cast<node_ptr>(header);
	node_ptr x = static_cast<node_ptr>(root());
	while (x) {
		if (key_comp(k, get_key(x))) {
			y = x;
			x = left(x);
		}
		else
			x = right(x);
	}
	return const_iterator(y);
}

// 查找红黑树中key等于k的结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
mystl::pair<typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::iterator,
	typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::iterator>
	rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::equal_range(const key_type& k) {
	return mystl::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}

// 查找红黑树中key等于k的结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
mystl::pair<typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::const_iterator,
	typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::const_iterator>
	rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::equal_range(const key_type& k) const {
	return mystl::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
}


//---------------------------------------------------辅助函数------------------------------------------------
// 获得一个新的结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::node_ptr
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::get_node() {
	return node_allocator::allocate(1);
}

// 销毁一个结点
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
void rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::put_node(node_ptr node) {
	node_allocator::deallocate(node);
}

// 对红黑树进行初始化
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
void rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::empty_initialize() {
	header->color = rb_tree_red;
	root() = nullptr;
	leftmost() = header;
	rightmost() = header;
	node_count = 0;
}

// 递归复制一棵树
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
typename rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::node_ptr
rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::copy_from(base_ptr x, base_ptr p) {
	base_ptr top = clone_node(x);
	top->parent = p;
	try {
		if (x->rchild)
			top->rchild = copy_from(x->rchild, top);
		p = top;
		x = x->lchild;
		while (x) {
			base_ptr y = clone_node(x);
			p->lchild = y;
			y->parent = p;
			if (x->rchild)
				y->rchild = copy_from(x->rchild, y);
			p = y;
			x = x->lchild;
		}
	}
	catch (...) {
		erase_since(top);
	}
	return static_cast<node_ptr>(top);
}

// 递归销毁一棵树
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
void rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::erase_since(base_ptr x) {
	while (x) {
		erase_since(x->rchild);
		base_ptr y = x->lchild;
		destroy_node(x->get_node_ptr());
		x = y;
	}
}

// 计算从参数结点到根结点路径上黑色结点个数
template<class T>
inline int black_count(rb_tree_node_base<T>* x, rb_tree_node_base<T>* root) {
	if (nullptr == x)
		return 0;
	int bc = x->color == rb_tree_black ? 1 : 0;
	if (x == root)
		return bc;
	return bc + black_count(x->parent, root);
}

// 判断一棵树是否是红黑树
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
bool rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::rb_tree_verify() const {
	if (0 == node_count || begin() == end())
		return (0 == node_count) && (begin() == end()) && (header->lchild = header && header->rchild == header);
	int len = black_count(leftmost(), root());
	for (iterator it = begin(); it != end(); ++it) {
		node_ptr x = static_cast<node_ptr>(it.node);
		node_ptr L = static_cast<node_ptr>(it.node->lchild);
		node_ptr R = static_cast<node_ptr>(it.node->rchild);

		if (x->color == rb_tree_red)
			if ((L && L->color == rb_tree_red) && (R && R->color == rb_tree_red))
				return false;

		if (L && key_comp(get_key(x), get_key(L)))
			return false;
		if (R && key_comp(get_key(R), get_key(x)))
			return false;

		if (!L && !R && black_count(x, root()) != len)
			return false;

		if (leftmost() != rb_tree_min(root()))
			return false;
		if (rightmost() != rb_tree_max(root))
			return false;

		return true;
	}
}

// 交换两棵树
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
void rb_tree<Key, Value, Keyofvalue, Compare, Alloc>::swap(rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& rhs) {
	mystl::swap(header, rhs.header);
	mystl::swap(node_count, rhs.node_count);
	mystl::swap(key_comp, rhs.key_comp);
}


//------------------------------------------------------------其他函数------------------------------------------------------------
template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
bool operator==(rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& lhs, rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& rhs) {
	return lhs.size() == rhs.size() && mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
bool operator!=(rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& lhs, rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& rhs) {
	return !(lhs == rhs);
}

template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
bool operator<(rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& lhs, rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& rhs) {
	return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), lhs.end());
}

template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
bool operator>(rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& lhs, rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& rhs) {
	return rhs < lhs;
}

template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
bool operator<=(rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& lhs, rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& rhs) {
	return !(rhs < lhs);
}

template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
bool operator>=(rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& lhs, rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& rhs) {
	return !(lhs < rhs);
}

template<class Key, class Value, class Keyofvalue, class Compare, class Alloc>
void swap(rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& lhs, rb_tree<Key, Value, Keyofvalue, Compare, Alloc>& rhs) {
	lhs.swap(rhs);
}





}	// mystl


#endif