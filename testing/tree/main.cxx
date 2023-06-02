#include <cstdlib>
#include <cstring>
#include <cstdio>

class Node {
	public:
		Node* letters[26];
		bool end;

		Node();
};

class NameTree {
	private:
		Node __root;
	public:
		void add(const char*);
		bool search(const char*);
		void find(const char*);
};

Node::Node()
{
	std::memset(letters, 0, sizeof(letters));
	end = false;
}

void NameTree::add(const char* s) 
{
	int idx;
	Node* root = &__root;
	while(*s && root) {
		idx = *s - 'a';
		if(!root->letters[idx]) {
			root->letters[idx] = new Node();
		}
		root = root->letters[idx];
		s++;
	}
	root->end = true;
}

bool NameTree::search(const char* s)
{
	int idx;
	Node* root = &__root;

	while(*s && root) {
		idx = *s - 'a';
		if(!root->letters[idx]) {
			return false;
		}
		root = root->letters[idx];
		s++;
	}
	if(root && root->end) {
		return true;
	}

	return false;
}

void NameTree::find(const char* s)
{
	if(search(s)) {
		std::printf("achei %s\n", s);
	}
	else {
		std::printf("NAO achei %s\n", s);
	}
}

int main()
{
	NameTree t;

	t.add("teste");
	t.add("uhu");

	t.find("teste");
	t.find("test");
	t.find("uhu");
	t.find("aha");
	return 0;
}

