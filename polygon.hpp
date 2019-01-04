#pragma once
#include <vector>
#include <cmath>

template <typename T>
class Point
{
  public:
    T x;
    T y;
    T z;
    Point(T x, T y, T z = 0) : x(x), y(y), z(z){};
};

template <typename T>

class Line
{
  private:
    std::vector<Point<T>> p;

  public:
    T minY;
    T maxY;
    T minX;
    T maxX;
    double xVal;
    double slope;
    bool sw;
    void toCorrect()
    {
        sw = abs(p[0].x - p[1].x) < abs(p[0].y - p[1].y);
        if (abs(p[0].x - p[1].x) < abs(p[0].y - p[1].y))
        {
            std::swap(p[0].x, p[0].y);
            std::swap(p[1].x, p[1].y);
        }
        if (p[0].x > p[1].x)
        {
            std::swap(p[0].x, p[1].x);
            std::swap(p[0].y, p[1].y);
        }
    }
    Line(Point<T> p1, Point<T> p2)
    {
        sw = false;
        this->minY = std::min(p1.y, p2.y);
        this->maxY = std::max(p1.y, p2.y);
        this->minX = std::min(p1.x, p2.x);
        this->maxX = std::max(p1.x, p2.x);

        std::min(p1.y, p2.y) == p1.y ? this->xVal = p1.x : this->xVal = p2.x;

        this->slope =
            static_cast<double>(static_cast<double>(p1.y - p2.y)) / static_cast<double>((p1.x - p2.x));
            
        p = {p1, p2};
    };
    Point<T> operator[](int i) { return p[i]; };
    struct less_than_key
    {
        inline bool operator()(const Line &line1, const Line &line2)
        {
            if (line1.minY != line2.minY)
                return (line1.minY < line2.minY);
            return (line1.minX < line2.minX);
        }
    };

    struct by_x_val_key
    {
        inline bool operator()(const Line &line1, const Line &line2)
        {
            return (line1.xVal < line2.xVal);
        }
    };
};
template <typename T>

class Polygon
{
  protected:
    std::vector<Point<T>> p;

  public:
    Polygon(const std::vector<Point<T>> &p) : p(std::move(p)){};
    Point<T> operator[](int i) { return p[i]; }
    size_t size() { return p.size(); }
};

template <typename T>

class Triangle : public Polygon<T>
{
  private:
    std::vector<Line<T>> l;

  public:
    Triangle(Point<T> p1, Point<T> p2, Point<T> p3) : Polygon<T>({p1, p2, p3}){};
    Triangle(const std::vector<Point<T>> &p) : Polygon<T>(p){};
};

template <typename T>

Point<T> vec(Point<T> u, Point<T> v)
{
    return {u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x};
}

template <typename T>

Point<T> minus(Point<T> u, Point<T> v)
{
    return {u.x - v.x, u.y - v.y, u.z - v.z};
}

template <typename T>

T dotProduct(Point<T> p1, Point<T> p2)
{
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

template <typename T>

Point<double> normalise(Point<T> n)
{
    double nlen = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    Point<double> ans = Point<double>(n.x / nlen, n.y / nlen, n.z / nlen);
    return ans;
}