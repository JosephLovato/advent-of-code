#include <cstddef>

template <class T>
class point {
   public:
    point();
    point(T r, T c) : row(r), col(c){};
    T row;
    T col;
};

template <class T>
struct point_equality {
   public:
    bool operator()(const point<T>& p1, const point<T>& p2) const {
        if (p1.col == p2.col && p1.row == p2.row)
            return true;
        else
            return false;
    }
};

template <class T>
struct point_hasher {
   public:
    size_t operator()(const point<T>& p) const {
        int v = p.row * 100 + p.col;
        return v;
    }
};
