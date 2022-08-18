typedef double LD;
#define cp const point &
#define cl const line &
#define cc const circle &
const LD eps = 1e-8;
const LD PI = (LD)acos(-1.0L);
int sgn(LD x) {return x > 0 ? 1 : (x < 0 ? -1 : 0);}
LD sqr (LD x) { return x * x; }
struct point {
	LD x, y;
	point () {}
	point (LD xx, LD yy) {x = xx, y = yy;}
	point operator + (cp a) const {return {x + a.x, y + a.y};}
	point operator - (cp a) const {return {x - a.x, y - a.y};}
	point operator * (LD a) const {return {x * a, y * a};}
	point operator / (LD a) const {return {x / a, y / a};}
	point unit() const {
		LD d = sqrt(x * x + y * y);
		return {x / d, y / d};
	}
	LD len () { return sqrt (x * x + y * y); }
	LD len2() const { return (x * x + y * y); }
	point rot90() const { return point(y, -x); }
	bool operator < (cp a) const {
		int d = sgn(x - a.x);
		if (d) return d < 0;
		else return sgn(y - a.y) < 0;
	}
	bool operator > (cp a) const { return a < *this; }
	bool operator == (cp a) const {
		return sgn(x - a.x) == 0 && sgn(y - a.y) == 0;
	}
	friend ostream& operator << (ostream &os, const point &a) {
		return os << a.x << "," << a.y;
	}
};
LD det (cp a, cp b) { return a.x * b.y - b.x * a.y; } 
LD dot (cp a, cp b) { return a.x * b.x + a.y * b.y; } 
LD dis (cp a, cp b) { return sqrt(sqr(a.x - b.x) + sqr(a.y - b.y));} 
bool turn_left(cp a, cp b, cp c) {
	return sgn (det (b - a, c - a)) >= 0; }
vector <point> convex_hull (vector <point> a) {
	int n = (int) a.size (), cnt = 0;
	if (n < 2) return a; 
	sort (a.begin(), a.end()); // less<pair>
	vector <point> ret;
	for (int i = 0; i < n; ++i) {
		while (cnt > 1
		&& turn_left (ret[cnt - 2], a[i], ret[cnt - 1]))
			--cnt, ret.pop_back ();
		++cnt, ret.push_back (a[i]); }
	int fixed = cnt;
	for (int i = n - 2; i >= 0; --i) {
		while (cnt > fixed
		&& turn_left (ret[cnt - 2], a[i], ret[cnt - 1]))
			--cnt, ret.pop_back ();
		++cnt, ret.push_back (a[i]); }
	ret.pop_back (); return ret;
} // counter-clockwise, ret[0] = min(pair(x, y))

struct circle {
	point c; LD r;
	circle () {}
	circle (point C, LD R) {c = C, r = R;} };
bool in_circle(cp a, const circle &b) {
	return (b.c - a).len() <= b.r; }
circle make_circle(point u, point v) {
	point p = (u + v) / 2;
	return circle(p, (u - p).len()); }
circle make_circle(cp a, cp b, cp c) {
	point p = b - a, q = c - a,
		s(dot(p, p) / 2, dot(q, q) / 2);
	LD d = det(p, q);
	p = point( det(s, point(p.y, q.y)),
		det(point(p.x, q.x), s) ) / d;
	return circle(a + p, p.len());
} 
circle min_circle (vector <point> p) {
	circle ret;
	random_shuffle (p.begin (), p.end ());
	for (int i = 0; i < (int) p.size (); ++i)
		if (!in_circle (p[i], ret)) {
			ret = circle (p[i], 0);
			for (int j = 0; j < i; ++j) if (!in_circle (p[j], ret)) {
				ret = make_circle (p[j], p[i]);
				for (int k = 0; k < j; ++k)
					if (!in_circle (p[k], ret))
						ret = make_circle(p[i],p[j],p[k]);
		} } return ret; }
struct line {
	point s, t;
	line () {}
	line (point ss, point tt) {s = ss, t = tt;}
};
bool point_on_segment(cp a,cl b) {
	return sgn (det (a - b.s, b.t - b.s)) == 0 // point on line
		&& sgn (dot (b.s - a, b.t - a)) <= 0; }
bool two_side(cp a,cp b,cl c) {
	return sgn (det (a - c.s, c.t - c.s))
		* sgn (det (b - c.s, c.t - c.s)) < 0; }
bool intersect_judge(cl a,cl b) {
	if (point_on_segment (b.s, a)
	|| point_on_segment (b.t, a)) return true;
	if (point_on_segment (a.s, b)
	|| point_on_segment (a.t, b)) return true;
	return two_side (a.s, a.t, b)
		&& two_side (b.s, b.t, a); }
point line_intersect(cl a, cl b) {
	LD s1 = det (a.t - a.s, b.s - a.s);
	LD s2 = det (a.t - a.s, b.t - a.s);
	return (b.s * s2 - b.t * s1) / (s2 - s1); }
bool point_on_ray (cp a, cl b) {
	return sgn (det (a - b.s, b.t - b.s)) == 0
	&& sgn (dot (a - b.s, b.t - b.s)) >= 0; }
bool ray_intersect_judge(line a, line b) {
	LD s1, s2;
	s1 = det(a.t - a.s, b.s - a.s);
	s2 = det(a.t - a.s, b.t - a.s);
	if (sgn(s1) == 0 && sgn(s2) == 0) {
		return sgn(dot(a.t - a.s, b.s - a.s)) >= 0
			|| sgn(dot(b.t - b.s, a.s - b.s)) >= 0; }
	if (!sgn(s1 - s2) || sgn(s1) == sgn(s2 - s1)) return 0;
	swap(a, b);
	s1 = det(a.t - a.s, b.s - a.s);
	s2 = det(a.t - a.s, b.t - a.s);
	return sgn(s1) != sgn(s2 - s1); }
LD point_to_line (cp a, cl b) {
	return abs (det (b.t-b.s, a-b.s)) / dis (b.s, b.t); }
point project_to_line (cp a, cl b) {
	return b.s + (b.t - b.s)
	* (dot (a - b.s, b.t - b.s) / (b.t - b.s).len2 ()); }
LD point_to_segment (cp a, cl b) {
	if (sgn (dot (b.s - a, b.t - b.s))
	* sgn (dot (b.t - a, b.t - b.s)) <= 0)
		return abs(det(b.t - b.s, a - b.s)) / dis(b.s, b.t);
	return min (dis (a, b.s), dis (a, b.t)); }
bool in_polygon (cp p, const vector <point> & po) {
	int n = (int) po.size (); int counter = 0;
	for (int i = 0; i < n; ++i) {
		point a = po[i], b = po[(i + 1) % n];
		if (point_on_segment (p, line (a, b))) return true;
		int x = sgn (det (p - a, b - a)),
			y = sgn (a.y - p.y), z = sgn (b.y - p.y);
		if (x > 0 && y <= 0 && z > 0) ++counter;
		if (x < 0 && z <= 0 && y > 0) --counter; }
	return counter != 0; }
vector <point> line_circle_intersect (cl a, cc b) {
	if (sgn (point_to_line (b.c, a) - b.r) > 0)
		return vector <point> ();
	LD x = sqrt(sqr(b.r)-sqr(point_to_line (b.c, a)));
	return vector <point>
	({project_to_line (b.c, a) + (a.s - a.t).unit () * x,
	project_to_line (b.c, a) - (a.s - a.t).unit () * x});}
LD circle_intersect_area (cc a, cc b) {
	LD d = dis (a.c, b.c);
	if (sgn (d - (a.r + b.r)) >= 0) return 0;
	if (sgn (d - abs(a.r - b.r)) <= 0) {
		LD r = min (a.r, b.r);
		return r * r * PI; }
	LD x = (d * d + a.r * a.r - b.r * b.r) / (2 * d),
		   t1 = acos (min (1., max (-1., x / a.r))),
		   t2 = acos (min (1., max (-1., (d - x) / b.r)));
	return sqr(a.r)*t1 + sqr(b.r)*t2 - d*a.r*sin(t1);}
vector <point> circle_intersect (cc a, cc b) {
	if (a.c == b.c
		|| sgn (dis (a.c, b.c) - a.r - b.r) > 0
		|| sgn (dis (a.c, b.c) - abs (a.r - b.r)) < 0)
		return {};
	point r = (b.c - a.c).unit ();
	LD d = dis (a.c, b.c);
	LD x = ((sqr (a.r) - sqr (b.r)) / d + d) / 2;
	LD h = sqrt (sqr (a.r) - sqr (x));
	if (sgn (h) == 0) return {a.c + r * x};
	return {a.c + r * x + r.rot90 () * h,
			  a.c + r * x - r.rot90 () * h}; }
vector <point> tangent (cp a, cc b) {
	circle p = make_circle (a, b.c);
	return circle_intersect (p, b); }
vector <line> extangent (cc a, cc b) {
	vector <line> ret;
	if (sgn(dis (a.c, b.c)-abs (a.r - b.r))<=0) return ret;
	if (sgn (a.r - b.r) == 0) {
		point dir = b.c - a.c;
		dir = (dir * a.r / dir.len ()).rot90 ();
		ret.push_back (line (a.c + dir, b.c + dir));
		ret.push_back (line (a.c - dir, b.c - dir));
	} else {
		point p = (b.c * a.r - a.c * b.r) / (a.r - b.r); 
		vector pp = tangent (p, a), qq = tangent (p, b);
		if (pp.size () == 2 && qq.size () == 2) {
			if (sgn (a.r-b.r) < 0)
				swap (pp[0], pp[1]), swap (qq[0], qq[1]);
			ret.push_back(line (pp[0], qq[0]));
			ret.push_back(line (pp[1], qq[1])); } }
	return ret; }
vector <line> intangent (cc a, cc b) {
	vector <line> ret;
	point p = (b.c * a.r + a.c * b.r) / (a.r + b.r); 
	vector pp = tangent (p, a), qq = tangent (p, b);
	if (pp.size () == 2 && qq.size () == 2) {
		ret.push_back (line (pp[0], qq[0]));
		ret.push_back (line (pp[1], qq[1])); }
	return ret; }
vector <point> cut (const vector<point> &c, line p) {
	vector <point> ret;
	if (c.empty ()) return ret;
	for (int i = 0; i < (int) c.size (); ++i) {
		int j = (i + 1) % (int) c.size ();
		if (turn_left (p.s, p.t, c[i])) ret.push_back (c[i]);
		if (two_side (c[i], c[j], p))
			ret.push_back (line_intersect (p, line (c[i], c[j]))); }
	return ret; }
const LD CINF = 2.1e9;
struct Convex {
 int n;
 vector<point> a, upper, lower;
 Convex(vector<point> _a) : a(_a) {
 	n = (int)a.size(); int k = 0;
 	for(int i = 1; i < n; ++ i) if (a[k] < a[i]) k = i;
 	for(int i = 0; i <= k; ++ i) lower.push_back(a[i]);
 	for(int i = k; i < n; ++ i) upper.push_back(a[i]);
 	upper.push_back(a[0]);
 }
 pair <LL, int> get_tan(vector <point> & con, point vec) {
 	int l = 0, r = (int) con.size() - 2;
 	for ( ; l + 1 < r; ) {
 		int mid = (l + r) / 2;
 		if (sgn(det(con[mid + 1] - con[mid], vec)) > 0) r = mid;
 		else l = mid;
 	}
 	return max(make_pair (det(vec, con[r]), r), make_pair(det(vec, con[0]), 0));
 }
 void upd_tan(cp p, int id, int &i0, int &i1) {
 	if (det(a[i0] - p, a[id] - p) > 0) i0 = id;
 	if (det(a[i1] - p, a[id] - p) < 0) i1 = id;
 }
 void search(int l, int r, point p, int &i0, int &i1) {
 	if (l == r) return;
 	upd_tan(p, l % n, i0, i1);
 	int sl = sgn(det(a[l % n] - p, a[(l + 1) % n] - p));
 	for ( ; l + 1 < r; ) {
 		int mid = (l + r) / 2;
 		int smid = sgn(det(a[mid % n] - p, a[(mid + 1) % n] - p));
 		if (smid == sl) l = mid;
 		else r = mid;
 	}
 	upd_tan(p, r % n, i0, i1);
 }
 int search(point u, point v, int l, int r) {
 	int sl = sgn(det(v - u, a[l % n] - u));
 	for ( ; l + 1 < r; ) {
 		int mid = (l + r) / 2;
 		int smid = sgn(det(v - u, a[mid % n] - u));
 		if (smid == sl) l = mid;
 		else r = mid;
 	}
 	return l % n;
 }
 // 判定点是否在凸包内, 在边界返回 true
 bool contain(point p) {
 	if (p.x < lower[0].x || p.x > lower.back().x) return false;
 	auto id = lower_bound(lower.begin(), lower.end(), point(p.x, -CINF)) - lower.begin();
 	if (lower[id].x == p.x) {
 		if (lower[id].y > p.y) return false;
 	} else if (det(lower[id - 1] - p, lower[id] - p) < 0) return false;
 	id = lower_bound(upper.begin(), upper.end(), point(p.x, CINF), greater<point>()) - upper.begin();
 	if (upper[id].x == p.x) {
 		if (upper[id].y < p.y) return false;
 	} else if (det(upper[id - 1] - p, upper[id] - p) < 0) return false;
 	return true;
 }
 // 求点 p 关于凸包的两个切点, 如果在凸包外则有序返回编号, 共线的多个切点返回任意一个, 否则返回 false
 bool get_tan(point p, int &i0, int &i1) {
 	i0 = i1 = 0;
 	int id = int(lower_bound(lower.begin(), lower.end(), p) - lower.begin());
 	search(0, id, p, i0, i1);
 	search(id, (int)lower.size(), p, i0, i1);
 	id = int(lower_bound(upper.begin(), upper.end(), p, greater <point> ()) - upper.begin());
 	search((int)lower.size() - 1, (int) lower.size() - 1 + id, p, i0, i1);
 	search((int)lower.size() - 1 + id, (int) lower.size() - 1 + (int)upper.size(), p, i0, i1);
 	return true;
 }
 // 求凸包上和向量 vec 叉积最大的点, 返回编号, 共线的多个切点返回任意一个
 int get_tan(point vec) {
 	pair<LL, int> ret = get_tan(upper, vec);
 	ret.second = (ret.second + (int)lower.size() - 1) % n;
 	ret = max(ret, get_tan(lower, vec));
 	return ret.second;
 }
 // 求凸包和直线 u,v 的交点, 如果无严格相交返回 false. 如果有则是和 (i,next(i)) 的交点, 两个点无序, 交在点上不确定返回前后两条线段其中之一
 bool get_inter(point u, point v, int &i0, int &i1) {
 	int p0 = get_tan(u - v), p1 = get_tan(v - u);
 	if (sgn(det(v - u, a[p0] - u)) * sgn(det(v - u, a[p1] - u)) < 0) {
 		if (p0 > p1) swap(p0, p1);
 		i0 = search(u, v, p0, p1);
 		i1 = search(u, v, p1, p0 + n);
 		return true;
 	} else return false;
}};
