// segtree.cpp @ fstqwq/CodeForcesTemplate
template <class V, class UPD> class Tree {
	struct Node {
		int l, r;
		V v;
		UPD tag;
	} t[N * 2] ;
	int tcnt, L, R;
	int build(int l, int r) {
		int x = ++tcnt;
		if (l < r) {
			int mid = (l + r) / 2;
			t[x].l = build(l, mid);
			t[x].r = build(mid + 1, r);
			t[x].v = t[t[x].l].v + t[t[x].r].v;
		}
		else {
			t[x].l = t[x].r = 0;
			t[x].v = V(a[l]); // init value in a
		}
		t[x].tag.clear();
		return x;
	}
	inline void pushdown(int x, int l, int mid, int r) {
		t[t[x].l].v.upd(t[x].tag, l, mid);
		t[t[x].l].tag += t[x].tag;
		t[t[x].r].v.upd(t[x].tag, mid + 1, r);
		t[t[x].r].tag += t[x].tag;
		t[x].tag.clear();
	}
	void upd(int x, int l, int r, const int& p, const UPD& v) {
		if (l == r) {
			t[x].v += v;
			t[x].tag += v;
			return; 
		}
		int mid = (l + r) / 2;
		if (t[x].tag.hastag()) pushdown(x, l, mid, r);
		if (p <= mid) upd(t[x].l, l, mid, p, v);
		else upd(t[x].r, mid + 1, r, p, v);
		t[x].v = t[t[x].l].v + t[t[x].r].v;
	}
	V qry(int x, int l, int r, int ql, int qr) {
		if (l == ql && r == qr) return t[x].v;
		int mid = (l + r) / 2;
		if (t[x].tag.hastag()) pushdown(x, l, mid, r);
		if (qr <= mid) return qry(t[x].l, l, mid, ql, qr);
		if (ql >  mid) return qry(t[x].r, mid + 1, r, ql, qr);
		return qry(t[x].l, l, mid, ql, mid) + qry(t[x].r, mid + 1, r, mid + 1, qr);
	}
	void upd_tag(int x, int l, int r, int ql, int qr, const UPD& v) {
		if (l == ql && r == qr) {
			t[x].v += v;
			t[x].tag += v;
			return; 
		}
		int mid = (l + r) / 2;
		if (t[x].tag.hastag()) pushdown(x, l, mid, r);
		if (qr <= mid)  upd_tag(t[x].l, l, mid, ql, qr, v);
		else if (ql >  mid)  upd_tag(t[x].r, mid + 1, r, ql, qr, v);
		else upd_tag(t[x].l, l, mid, ql, mid, v), upd_tag(t[x].r, mid + 1, r, mid + 1, qr, v);
		t[x].v = t[t[x].l].v + t[t[x].r].v;
	}
public:
	Tree () {}
	void init(int l, int r) {
		L = l, R = r;
		tcnt = 0;
		build(l, r);
	}
	inline void upd(int p, const UPD &v) {upd(1, L, R, p, v);}
	inline void upd(int l, int r, const UPD &v) {upd_tag(1, L, R, l, r, v);}
	inline V qry(int l, int r) {return qry(1, L, R, l, r);}
};

struct UPD {
	int x;
	UPD () {}
	UPD (int y) { x = y; }
	void operator += (const UPD &o) { x += o.x; }
	void clear() { x = 0; }
	bool hastag() { return !!x;}
};

struct V {
	int x;
	V () {}
	V (int y) {x = y;}
	friend V operator + (const V &u, const V &v) {
		return V(u.x + v.x); 
	}
	void upd(const UPD &v, int l, int r) {
		x += v.x * (r - l + 1);
	}
};
