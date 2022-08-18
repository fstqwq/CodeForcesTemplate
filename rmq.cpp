const int N = 5e5 + 5;
constexpr int LOG = __lg(N) + 1;
int n, a[N], st[LOG][N];
void init() {
	for (int i = 1; i <= n; i++) st[0][i] = a[i];
	for (int j = 1; j < LOG; j++) {
		for (int i = 1; i + (1 << j) - 1 <= n; i++) {
			st[j][i] = max(st[j - 1][i], st[j - 1][i + (1 << j - 1)]);
		}
	}
}
int query(int l, int r) {
	int k = __lg(r - l + 1);
	return max(st[k][l], st[k][r - (1 << k) + 1]);
}
