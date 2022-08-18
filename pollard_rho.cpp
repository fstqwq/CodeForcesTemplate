typedef long long LL;
typedef unsigned long long ULL;
ULL mul(ULL a, ULL b, LL M) {
	LL ret = a * b - M * ULL(1.L / M * a * b);
	return ret + M * (ret < 0) - M * (ret >= (LL)M);
}
LL power(LL x, LL y, LL m) {
	LL ret = 1;
	for (x %= m; y; y >>= 1) {
		if (y & 1) ret = mul(ret, x, m);
		x = mul(x, x, m); }
	return ret; }
mt19937 rng(123);
LL Rand() { return rng() & LLONG_MAX; }
const int BASE[] = {2,325,9375,28178,450775,9780504,1795265022};//LL
//{2, 7, 61};//int(7,3e9)
struct miller_rabin {
bool check (const LL &M, const LL &base) {
	LL a = M - 1;
	while (~a & 1) a >>= 1;
	LL w = power (base, a, M); // power should use mul
	for (; a != M - 1 && w != 1 && w != M - 1; a <<= 1)
		w = mul (w, w, M);
	return w == M - 1 || (a & 1) == 1; }
bool operator () (const LL &a) {
	if (a < 4) return a > 1;
	if (~a & 1) return false;
	for (size_t i = 0; i < sizeof(BASE)/4 && BASE[i] < a; ++i)
		if (!check (a, BASE[i])) return false;
	return true; } };
miller_rabin is_prime;
LL get_factor (LL a, LL seed) {//n ^ {1/4} * log n * mul
	LL x = rand () % (a - 1) + 1, y = x;
	for (int head = 1, tail = 2; ; ) {
		x = mul (x, x, a); x = (x + seed) % a;
		if (x == y) return a;
		LL ans = gcd (abs (x - y), a);
		if (ans > 1 && ans < a) return ans;
		if (++head == tail) { y = x; tail <<= 1; }
	}
}
void factor (LL a, vector<LL> &d) {
	if (a <= 1) return;
	if (is_prime (a)) d.push_back (a);
	else {
		LL f = a;
		for (; f >= a; f = get_factor (a, rand() % (a - 1) + 1));
		factor (a / f, d);
		factor (f, d);
	}
}
