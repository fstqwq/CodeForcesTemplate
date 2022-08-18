struct Sieve {
	static const int N = 2e7 + 5; // N must >= 1e4
	static constexpr int PN = N < 5e5 ? N / 8 : N / 12;
	bool not_prime[N];
	int pcnt, prime[PN];
	int phi[N];
	Sieve() {
		not_prime[1] = 1;	
		for (int i = 2; i < N; i++) {
			if (!not_prime[i]) prime[++pcnt] = i;//, phi[i] = i - 1;
			for (int j = 1, k; (k = i * prime[j]) < N; j++) {
				not_prime[k] = 1;
				if (i % prime[j]) {
					// phi[k] = phi[i] * (prime[j] - 1);
				} else {
					// phi[k] = phi[i] * prime[j];	
					break;
				} } } }
	bool operator [] (const int x) const {
		return x >= 2 && x < N && !not_prime[x];
	}
} is_prime;
