#ifndef __libany_tree_h
#define __libany_tree_h

template<typename T> class Node {
	public:
	T data;
	Node<T>* next;
	Node<T>* down;
	Node<T>* up;

	Node(T& t) : data(t) {
	}

	inline operator T& () {
		return data;
	};

	inline T& get_data() {
		return data;
	};
};

template<typename T> class Tree {
	public:
	Node<T>* _root;
	Node<T>* Tree<T>::add_child(Node<T>* p_root, T& p_data)
	{
		Node<T>* n = new Node<T>(p_data);
		n->down = 0;
		if(p_root) {
			n->next = p_root->down;
			p_root->down = n;
		}
		else {
			_root = n;
			n->next = 0;
		}
		n->up = p_root;

		return n;
	};
	

	Tree() : _root(0) {
	}

	~Tree() {
	}
};

#endif

