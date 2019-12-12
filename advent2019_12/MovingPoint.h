#pragma once
#include "Point.h"

__int64 mx = 0;

template<typename _Ty, int n>
class MovingPoint
{
public:
    typedef Point<_Ty, n> PointT;
    MovingPoint() : MovingPoint(PointT(), PointT()) {}
    MovingPoint(const PointT& pos) : MovingPoint(pos, PointT()) {}
    MovingPoint(const PointT& pos, const PointT& vel) : m_pos(pos), m_vel(vel) {}
    MovingPoint(const MovingPoint& m) : m_pos(m.m_pos), m_vel(m.m_vel) {}

    MovingPoint& operator=(const MovingPoint& other) {
        m_pos = other.m_pos;
        m_vel = other.m_vel;
    }

    bool operator==(const MovingPoint& other) const {
        return (m_pos == other.m_pos) && (m_vel == other.m_vel);
    }

    bool operator<(const MovingPoint& other) const {
        return (m_pos == other.m_pos) ? (m_vel < other.m_vel) : (m_pos < other.m_pos);
    }

    void Gravitate(const MovingPoint& other) {
        //m_vel += (other.Pos() - m_pos).Clamp();
        m_vel.ClampSub(other.Pos(), m_pos);
    }

    _Ty Energy() {
        return m_pos.Energy() * m_vel.Energy();
    }

    void Move() {
        m_pos += m_vel;
    }

    bool Stopped() const {
        return m_vel.IsZero();
    }

    MovingPoint<_Ty, 1> Slice(int dim) const {
        Point<_Ty, 1> pos({ m_pos.Coord()[dim] });
        Point<_Ty, 1> vel({ m_vel.Coord()[dim] });
        return MovingPoint<_Ty, 1>(pos, vel);
    }

    PointT& Pos() { return m_pos; }
    const PointT& Pos() const { return m_pos; }
    PointT& Vel() { return m_vel; }
    const PointT& Vel() const { return m_vel; }

    bool Is(const MovingPoint& other) const {
        return (this == &other);
    }

private:
    PointT m_pos;
    PointT m_vel;
};

template <typename _Ty, int n>
inline std::ostream& operator<<(std::ostream& os, const MovingPoint<_Ty, n>& obj)
{
    os << "pos=" << obj.Pos() << ", vel=" << obj.Vel();
    return os;
}

namespace std
{
    template<typename _Ty, int n> struct hash<MovingPoint<_Ty, n> >
    {
        std::size_t operator()(MovingPoint<_Ty, n> const& m) const noexcept
        {
            std::size_t h1 = std::hash<MovingPoint<_Ty, n>::PointT>{}(m.Pos());
            std::size_t h2 = std::hash<MovingPoint<_Ty, n>::PointT>{}(m.Vel());
            boost::hash_combine(h1, h2);
            return h1;
        }
    };
}
