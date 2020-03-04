namespace P_R {
	void mul(LL &x, LL y, LL m) {
		x %= m, y %= m;
		unsigned long long t = (long long)((long double)x * y / m);
		x = (unsigned long long) x * y - t * m;
		x = (x % m + m) % m;
	}

	LL power(LL x, LL y, LL m) {
		LL ret = 1;
		for (x %= m; y; y >>= 1) {
			if (y & 1) mul(ret, x, m);
			mul(x, x, m);
		}
		return ret;
	}

	LL Rand() {
		static LL a = 2333333;
		a ^= a >> 12;
		a ^= a << 25;
		a ^= a >> 27;
		return a * 0x2545F4914F6CDD1D;
	}

	LL gcd(LL a, LL b) {
		while (b) {
			LL c = a;
			a = b; b = c % b;
		}
		return a;
	}

	const int P[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, -1}; // 3.8e18
	// 1e9 : 2, 7, 61
	// 1e12 : 2, 13, 23, 1662803
	// ULL : ~37
	vector<LL> D;

	bool witness(int p, LL x, int t, LL n) {
		LL y = power(p, x, n);
		if (y == 1) return 0;
		for (int i = 0; i < t; i++) {
			if (y == n - 1) return 0;
			if (y == 1) return 0;
			mul(y, y, n);
		}
		return 1;
	}

	bool MR(LL n) { // O(k log ^ 3)
		if (n < 4) return 1;
		for (int i = 0; ~P[i]; i++) {
			if (n == P[i]) return 1;
			if (n % P[i] == 0) return 0;
		}
		LL x = n - 1; int t = 0;
		while (~x & 1) {
			++t;
			x >>= 1;
		}
		for (int i = 0; ~P[i]; i++) {
			if (witness(P[i], x, t, n)) return 0;
		}
		return 1;
	}

	void PR(LL n) { // O(n ^ {1 / 4} * log)
		if (MR(n)) {
			D.push_back(n);
			return;
		}
		LL a, b, c, d;
		while (1) {
			c = Rand() % n;
			a = b = Rand() % n;
			mul(b, b, n); (b += c) %= n;
			while (a != b) {
				d = a > b ? a - b : b - a;
				d = gcd(d, n);
				if (d > 1 && d < n) {
					PR(d); PR(n / d);
					return;
				}
				mul(a, a, n); (a += c) %= n;
				mul(b, b, n); (b += c) %= n;
				mul(b, b, n); (b += c) %= n;
			}
		}
	}
	void solve(LL n) {
		D.clear();
		if (n < 2) return;
		PR(n);
		sort(D.begin(), D.end());
	}
}

/* 
3.5e4 int
1e5 int (mul : x * y % m)
2e4 1e12
5e3 1e16
2e3 1e18
*/
