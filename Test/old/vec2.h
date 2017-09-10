//A 2d-vector class
#ifndef __VEC2_H
#define __VEC2_H

#include <cmath>

template<typename Val> class Vec2 {
        Val mx,my;
        public:
        Vec2(Val xx = Val(), Val yy = Val())
        : mx(xx), my(yy)
        { }

        /*     Vec2(const Vec2& v)
        *         : mx(v.mx), my(v.my)
        *     { }
        *
        *     Vec2& operator=(Vec2 v)
        *     {
        *         mx = v.mx;
        *         my = v.my;
        *         return *this;
        *     }
        */

        void setPolar(Val radians, Val distance)
        {
                using namespace std;
                mx = distance*cos(radians);
                my = distance*sin(radians);
        }

        Val x() const { return mx; }
        Val y() const { return my; }

		void setX(Val x){mx = x;}
		void setY(Val y){my = y;}

        Val radians() const
        {
                using namespace std;
                return atan2(my, mx);
        }


        Val magnitudeSquared() const
        {
                return mx*mx+my*my;
        }

        Val magnitude() const
        {
                using namespace std;
                return sqrt(magnitudeSquared());
        }

        Vec2& normalize()
        {
                Val m = magnitude();
                mx /= m;
                my /= m;
                return *this;
        }

        Vec2& operator+=(Vec2 rhs)
        {
                mx += rhs.mx;
                my += rhs.my;
                return *this;
        }

        Vec2& operator-=(Vec2 rhs)
        {
                mx -= rhs.mx;
                my -= rhs.my;
                return *this;
        }

        Vec2& operator*=(Val rhs)
        {
                mx *= rhs;
                my *= rhs;
                return *this;
        }

        Vec2& operator/=(Val rhs)
        {
                mx /= rhs;
                my /= rhs;
                return *this;
        }
};

//Vector2DAddition / Subtraction

template<typename Val>
inline Vec2<Val> operator+(Vec2<Val> lhs, Vec2<Val> rhs)
{
        return lhs += rhs;
}

template<typename Val>
inline Vec2<Val> operator-(Vec2<Val> lhs, Vec2<Val> rhs)
{
        return lhs -= rhs;
}


//Vector2DScalar product

template<typename Val>
inline Vec2<Val> operator*(Vec2<Val> lhs, Val rhs)
{
        return lhs *= rhs;
}

template<typename Val>
inline Vec2<Val> operator*(Val lhs, Vec2<Val> rhs)
{
        return rhs *= lhs;
}

template<typename Val>
inline Vec2<Val> operator/(Vec2<Val> lhs, Val rhs)
{
        return lhs /= rhs;
}


//Vector2DDot product

template<typename Val>
inline Val operator*(Vec2<Val> lhs, Vec2<Val> rhs)
{
        return lhs.x()*rhs.x() + lhs.y()*rhs.y();
}


//Vector2DSign operators

template<typename Val>
inline Vec2<Val> operator-(Vec2<Val> v)
{
        return Vec2<Val>(-v.x(), -v.y());
}

template<typename Val>
inline Vec2<Val> operator+(Vec2<Val> v)
{
        return Vec2<Val>(+v.x(), +v.y());
}


//Vector2DUnit vector

template<typename Val>
inline Vec2<Val> operator~(Vec2<Val> v)
{
        return v.normalize();
}


//Vector2DComparison

template<typename Val>
inline bool operator==(Vec2<Val> lhs, Vec2<Val> rhs)
{
        return (lhs.x() == rhs.x()) && (lhs.y() == rhs.y());
}

template<typename Val>
inline bool operator!=(Vec2<Val> lhs, Vec2<Val> rhs)
{
        return (lhs.x() != rhs.x()) || (lhs.y() != rhs.y());
}


//Vector2DAbsolute value / magnitude

template<typename Val>
inline Val abs(Vec2<Val> v)
{
        return v.magnitude();
}


//A couple of handy typedefs

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;

typedef Vec2<int> Vec2i;

#endif
