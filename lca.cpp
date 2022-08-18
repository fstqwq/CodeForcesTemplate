#include <bits/stdc++.h>
using namespace std;

int ch = 0;
inline void read(int &a) {
	bool f = 0; a = 0;
	while (ch < '0' || ch > '9') {if (ch == '-') f = 1; ch = getchar();}
	while (ch >= '0' && ch <= '9') a = a * 10 + ch - '0', ch = getchar();
	if (f) a = -a;	
}
const int LOG = 19;
const int N = 5e5 + 5;

vector <int> E[N];
int dep[N], ecnt = 1;


int n, m, s;
int fa[N][LOG];

void dfs(int x) {
	for (int i = 1; i < LOG; i++) fa[x][i] = fa[fa[x][i - 1]][i - 1];
	for (auto v : E[x]) if (v != fa[x][0]) {
		fa[v][0] = x;
		dep[v] = dep[x] + 1;
		dfs(v);
	}
}

int lca(int u, int v) {
	if (dep[u] < dep[v]) swap(u, v);
	int x = dep[u] - dep[v]; 
	for (int i = 0; x; i++, x >>= 1) if (x & 1) u = fa[u][i];
	if (u == v) return u;
	for (int i = LOG - 1; ~i; i--) if (fa[u][i] != fa[v][i])
		u = fa[u][i], v = fa[v][i];
	return fa[u][0];
}

int main() {
	read(n); read(m); read(s);
	for (int i = 1, u, v; i < n; i++) {
		read(u), read(v);
		E[u].push_back(v);
		E[v].push_back(u);
	}
	dfs(s);
	for (int i = 1, u, v; i <= m; i++) {
		read(u); read(v);
		printf("%d\n", lca(u, v));
	}
	return 0;
}
