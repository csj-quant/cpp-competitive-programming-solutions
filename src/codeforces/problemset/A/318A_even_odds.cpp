#include <bits/stdc++.h>
using namespace std;

/* =========================
   Type aliases
   ========================= */
using ll  = long long;
using ull = unsigned long long;
using ld  = long double;

using pii = pair<int,int>;
using pll = pair<ll,ll>;
using vi  = vector<int>;
using vll = vector<ll>;

/* =========================
   Fast I/O
   ========================= */
#define FAST_IO ios::sync_with_stdio(false); cin.tie(nullptr);

/* =========================
   Constants
   ========================= */
constexpr ll INFLL = (ll)1e18;
constexpr int INF  = (int)1e9;
constexpr int MOD  = 1000000007;

/* =========================
   Debug utilities
   ========================= */
// #define DEBUG
#ifdef DEBUG
#define dbg(x) cerr << #x << " = " << x << "\n"
#else
#define dbg(x)
#endif

/* =========================
   Custom Hash (anti-hack)
   ========================= */
struct custom_hash {
    static ull splitmix64(ull x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(ull x) const noexcept {
        static const ull FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

/* =========================
   Disjoint Set Union (DSU)
   ========================= */
struct DSU {
    vector<int> p, sz;
    explicit DSU(int n) : p(n), sz(n,1) {
        iota(p.begin(), p.end(), 0);
    }
    int find(int x) {
        while (p[x] != x) {
            p[x] = p[p[x]];
            x = p[x];
        }
        return x;
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a,b);
        p[b] = a;
        sz[a] += sz[b];
        return true;
    }
};

/* =========================
   Fenwick Tree (BIT)
   ========================= */
struct Fenwick {
    int n;
    vector<ll> bit;
    explicit Fenwick(int n) : n(n), bit(n+1) {}
    void add(int i, ll v) {
        for (++i; i <= n; i += i & -i)
            bit[i] += v;
    }
    ll sum(int i) const {
        ll s = 0;
        for (++i; i > 0; i -= i & -i)
            s += bit[i];
        return s;
    }
    ll range(int l, int r) const {
        return sum(r) - (l ? sum(l-1) : 0);
    }
};

/* =========================
   Segment Tree (Range Sum)
   ========================= */
struct SegTree {
    int n;
    vector<ll> t;
    explicit SegTree(int n) : n(n), t(4*n) {}

    void update(int v, int tl, int tr, int pos, ll val) {
        if (tl == tr) {
            t[v] = val;
        } else {
            int tm = (tl + tr) / 2;
            if (pos <= tm) update(v*2, tl, tm, pos, val);
            else update(v*2+1, tm+1, tr, pos, val);
            t[v] = t[v*2] + t[v*2+1];
        }
    }

    ll query(int v, int tl, int tr, int l, int r) const {
        if (l > r) return 0;
        if (l == tl && r == tr) return t[v];
        int tm = (tl + tr) / 2;
        return query(v*2, tl, tm, l, min(r, tm))
             + query(v*2+1, tm+1, tr, max(l, tm+1), r);
    }
};

/* =========================
   Sparse Table (RMQ)
   ========================= */
struct SparseTable {
    int n, LOG;
    vector<vector<ll>> st;

    explicit SparseTable(const vector<ll>& a) {
        n = (int)a.size();
        LOG = bit_width((unsigned)n);
        st.assign(LOG, vector<ll>(n));
        st[0] = a;

        for (int k = 1; k < LOG; k++)
            for (int i = 0; i + (1<<k) <= n; i++)
                st[k][i] = min(st[k-1][i],
                               st[k-1][i + (1<<(k-1))]);
    }

    ll query(int l, int r) const {
        int k = bit_width((unsigned)(r - l + 1)) - 1;
        return min(st[k][l], st[k][r - (1<<k) + 1]);
    }
};

/* =========================
   Trie (lowercase)
   ========================= */
struct Trie {
    struct Node {
        array<int,26> nxt{};
        bool end = false;
        Node() { nxt.fill(-1); }
    };
    vector<Node> t{Node()};

    void insert(const string& s) {
        int v = 0;
        for (char c : s) {
            int x = c - 'a';
            if (t[v].nxt[x] == -1) {
                t[v].nxt[x] = (int)t.size();
                t.emplace_back();
            }
            v = t[v].nxt[x];
        }
        t[v].end = true;
    }
};

/* =========================
   Graph helpers
   ========================= */
struct Edge {
    int to;
    ll w;
};
using Graph = vector<vector<Edge>>;

/* =========================
   Geometry
   ========================= */
struct Point {
    ll x, y;
};
constexpr ll cross(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

/* =========================
   Problem solver
   ========================= */
void solve() {
    // Write problem-specific logic here
}

/* =========================
   Entry Point
   ========================= */
int main() {
    FAST_IO;

    int T = 1;
    // cin >> T;

    int64_t n, k; cin >> n >> k;
    if (n%2 == 0 && k <= n/2) cout << 2*k-1;
    else if (n%2 == 0 && k > n/2) cout << 2*k - n;
    else if (n%2 != 0 && k <= (n+1)/2) cout << 2*k - 1;
    else if (n%2 != 0 && k > (n+1)/2) cout << 2*k - n - 1;

    return 0;
}

/*
 * 1 2 3 4  ... N
 *
 * if(N==even)
 * {
 *      1 3 5 .. N-1  2      4      6  ..  N
 *      1 2 3 .. N/2  N/2+1  N/2+2  ...
 *
 *      IF(K <= N/2) cout << 2k-1
 *      else if(k > N/2) cout << 2k-  N
 * }
 * else {
 *      1 3 5 7 ... N           2           4         6      ....   N-1
 *      1 2 3 4 ... (N+1)/2     (n+1)/2+1
 *      IF(k <= (N+1)/2) COUT << 2k-1
 *      else cout << (k - (n+1)/2)*2 = 2k -n-1
 * }
*/