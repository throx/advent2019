#pragma once

template <typename _Ty, int n>
class Point
{
public:
    Point() {
        memset(m_coord, 0, sizeof(m_coord));
    }

    Point( std::initializer_list<_Ty> l ) {
        int i = 0;
        for (auto x : l) {
            m_coord[i] = x;
            ++i;
        }
    }

    Point(const Point& p) {
        memcpy(m_coord, p.m_coord, sizeof(m_coord));
    }

    bool Load(std::istream& str) {
        for (int i = 0; i < n; ++i) {
            int c = str.peek();
            while (!(isdigit(c) || (c == '-'))) {
                str.get();
                if (!str.good()) return false;
                c = str.peek();
            };

            str >> m_coord[i];
        }

        return str.good();
    }

    bool operator==(const Point& other) const {
        return memcmp(m_coord, other.m_coord, sizeof(m_coord)) == 0;
    }

    bool operator<(const Point& other) const {
        return memcmp(m_coord, other.m_coord, sizeof(m_coord)) < 0;
    }

    Point& operator=(const Point& other) {
        if (this != &other) {
            memset(m_coord, other.m_coord, sizeof(m_coord));
        }
        return *this;
    }

    Point& operator+=(const Point& other) {
        for (int i = 0; i < n; ++i) m_coord[i] += other.m_coord[i];
        return *this;
    }

    Point& operator-=(const Point& other) {
        for (int i = 0; i < n; ++i) m_coord[i] -= other.m_coord[i];
        return *this;
    }

    Point Clamp(_Ty x = 1) const {
        Point p;
        for (int i = 0; i < n; ++i) {
            if (m_coord[i] <= -1) p.m_coord[i] = -1;
            else if (m_coord[i] >= 1) p.m_coord[i] = 1;
            else p.m_coord[i] = 0;
        }
        return p;
    }

    void ClampSub(const Point& other, const Point& me) {
        for (int i = 0; i < n; ++i) {
            if (other.m_coord[i] > me.m_coord[i]) ++m_coord[i];
            else if (other.m_coord[i] < me.m_coord[i]) --m_coord[i];
        }
    }

    bool IsZero() const {
        for (int i = 0; i < n; ++i) {
            if (m_coord[i] != 0) return false;
        }
        return true;
    }

    _Ty Energy() {
        _Ty e = 0;
        for (int i = 0; i < n; ++i) e = e + abs(m_coord[i]);
        return e;
    }

    const _Ty* Coord() const { return m_coord; }

private:
    _Ty m_coord[n];
};

template <typename _Ty, int n>
Point<_Ty, n> operator+(const Point<_Ty, n>& lhs, const Point<_Ty, n>& rhs) {
    Point<_Ty, n> r(lhs);
    r += rhs;
    return r;
}

template <typename _Ty, int n>
Point<_Ty, n> operator-(const Point<_Ty, n>& lhs, const Point<_Ty, n>& rhs) {
    Point<_Ty, n> r(lhs);
    r -= rhs;
    return r;
}

template <typename _Ty, int n>
inline std::ostream& operator<<(std::ostream& os, const Point<_Ty, n>& obj)
{
    os << "<" << obj.Coord()[0];
    for (int i = 1; i < n; ++i) {
        os << ", " << obj.Coord()[i];
    }
    os << ">";
    return os;
}

template <typename _Ty, int n>
inline std::istream& operator>>(std::istream& is, Point<_Ty, n>& obj)
{
    if (!obj.Load(is)) {
        is.setstate(std::ios::failbit);
    }
    return is;
}

namespace std
{
    template<typename _Ty, int n> struct hash<Point<_Ty, n> >
    {
        std::size_t operator()(Point<_Ty, n> const& p) const noexcept
        {
            if (sizeof(_Ty) * n <= sizeof(std::size_t))
            {
                std::size_t h = 0;
                memcpy(&h, p.Coord(), sizeof(_Ty) * n);
                return h;
            }

            std::size_t h = std::hash<_Ty>{}(0);
            for (int i = 0; i < n; ++i) {
                std::size_t h2 = std::hash<_Ty>{}(p.Coord()[n]);
                boost::hash_combine(h, h2);
            }
            return h;
        }
    };
}
