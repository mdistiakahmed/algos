#include <bits/stdc++.h>
using namespace std;

/*
Notes:
   written for maximizing values
   while minimizing lines maximize the dual -y = (-mx-c)
*/

typedef long long Long;
const Long inf = 1000000000000000000LL;

struct Line {
   Long m, c;
   Line(Long _m = 0, Long _c = 0) : m(_m), c(_c) {};
   Long get(Long x) const { return m * x + c; }
   bool operator < (const Line &B) const { return m < B.m; }
};
inline bool isBad(Line &P, Line &C, Line &N) {
   return (P.c - C.c) * (N.m - P.m) >= (P.c - N.c) * (C.m - P.m);
}

// special case where lines have nondecreasing slope
struct ConvexHull {
   vector <Line> hull;

   void addLine(Long m, Long c) {
      if(hull.size() > 0 && hull.back().m == m) {
         if(hull.back().c < c) hull.pop_back();
         else return;
      }
      hull.push_back(Line(m, c));
      int sz = hull.size();
      while(sz > 2 && isBad(hull[sz-3], hull[sz-2], hull[sz-1])) {
         swap(hull[sz-2], hull[sz-1]);
         hull.pop_back();
         sz--;
      }
   }

   Long query(Long x) {
      if(hull.size() == 0) return -inf;
      int l = -1, r = hull.size() - 1;
      while(r - l > 1) {
         int m = (l + r) / 2;
         if(hull[m].get(x) <= hull[m+1].get(x)) l = m;
         else r = m;
      }
      return hull[r].get(x);
   }

   void clear() { hull.clear(); }
};

struct Line {
   Long m, b;
   mutable function<const Line*()> succ;
   bool operator<(const Line& rhs) const {
      if (rhs.b != -inf) return m < rhs.m;
      const Line* s = succ();
      if (!s) return 0;
      Long x = rhs.m;
      return b - s->b < (s->m - m) * x;
   }
};
struct HullDynamic : public multiset<Line> { // will maintain upper hull for maximum
   bool bad(iterator y) {
      auto z = next(y);
      if (y == begin()) {
      	if (z == end()) return 0;
         return y->m == z->m && y->b <= z->b;
      }
      auto x = prev(y);
      if (z == end()) return y->m == x->m && y->b <= x->b;
      return (x->b - y->b)*(z->m - y->m) >= (y->b - z->b)*(y->m - x->m);
	}
   void insert_line(Long m, Long b) {
      auto y = insert({ m, b });
      y->succ = [=] { return next(y) == end() ? 0 : &*next(y); };
      if (bad(y)) { erase(y); return; }
      while (next(y) != end() && bad(next(y))) erase(next(y));
      while (y != begin() && bad(prev(y))) erase(prev(y));
   }
   Long query(Long x) {
      auto l = *lower_bound((Line) { x, -inf });
      return l.m * x + l.b;
   }
};

