struct BIT {
	using T = int;
	int n;
	vector <T> a;
	void init(size_t _n) {n = (int)_n; a.assign(n + 1, 0);}
	void init(const vector <T>& b) { // b 0-based to a 1-based
		init(b.size());
		for (int i = 1; i <= n; i++) {
			a[i] += b[i - 1];
			int j = i + (i & -i);
			if (j <= n) a[j] += a[i];
		}
	}
	void clear() {fill(a.begin(), a.end(), 0);}
	T qry(int x) { // prefix sum
		T w = 0;
		for (assert (x >= 0); x; x -= x & -x) w += a[x];
		return w;
	}
	T qry(int l, int r) {
		return qry(r) - qry(max(l - 1, 0));
	}
	void upd(int x, T y = 1) {
		for (assert (x); x <= n; x += x & -x) a[x] += y;
	}
	// below requires: value 0 / 1
	int kth(int k) { // -1, 1st, ..., sum-th, n+1
		if (!k) return -1;
		int w = 0;
		for (int i = __lg(n); ~i; --i) {
			int x = w + (1 << i);
			if (x > n || a[x] >= k);
			else k -= a[w = x];
		}
		return w + 1;
	}
	int upper_bound(int x) { // first >
		return kth(qry(x) + 1);
	}
	int lower_bound(int x) { // first >=
		return kth((x ? qry(x - 1) : 0) + 1);
	}
	int lower(int x) { // first <
		return x ? kth(qry(x - 1)) : -1;
	}
	int leq(int x) { // first <=
		return kth(qry(x));
	}
};

