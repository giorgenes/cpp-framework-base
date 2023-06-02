class Combination {
	public:
		virtual void next() = 0;
};

class LinearCombination : public Combination {
	private:
		unsigned __n;
		unsigned __k;
	public:
		virtual void next();

		LinearCombination(int n, int k)
			: __n(n), __k(k) {
		}
};

void LinearCombination::next()
{
}



template <int N, int K> class Comb : public LinearCombination {
	private:
		unsigned int __i;
	public:
		Comb() : LinearCombination(N, K) {}
		void setn(unsigned int n) {
			__i = n;
		};
		virtual void next() {
			LinearCombination::next();
			__i++;
		};
};

int main()
{
	Comb<6, 3> c;

	c.next();
}
