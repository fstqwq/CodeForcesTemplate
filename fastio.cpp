const int SZ = 1 << 16;
int getc() {
	static char buf[SZ], *ptr = buf, *top = buf;
	if (ptr == top) {
		ptr = buf, top = buf + fread(buf, 1, SZ, stdin);
		if (top == buf) return -1; }
	return *ptr++;
}
#define getchar getc
using T = int; // to LL if needed
T read() {
	T ret = 0;
	int ch = getchar(), rev = 0;
	while (ch < '0') rev |= ch == '-', ch = getchar();
	while (ch >= '0') ret = ret * 10 + ch - '0', ch = getchar();
	return rev ? -ret : ret;
}
void read(T &x) { x = read(); }
void read(double &ret) {
	int ch = getchar(), rev = 0;
	for ( ; ch < '0' && ch != '.'; ch = getchar()) rev |= ch == '-';
	double lower = 1;
	for ( ; ch >= '0' || ch == '.'; ch = getchar()) {
		if (ch == '.') lower = 10;
		else ret = ret * 10 + ch - '0';
		if (lower > 1) lower *= 10;
	}
	ret = ret / lower;
	if (rev) ret = -ret;
}
