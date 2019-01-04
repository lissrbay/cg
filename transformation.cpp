#include <cmath>
#include "tgaimage.hpp"
#include "polygon.hpp"
#include "transformation.hpp"

#define PI 3.14159265
Point<int> transfer(Point<int> point, Point<int> vec)
{
    return point + vec;
}

Polygon<int> transferPoly(Polygon<int> poly, Point<int> vec)
{
    Polygon<int> result = Polygon<int>();
    for (size_t i = 0; i < poly.size(); i++)
    {
        result += poly[i] + vec;
    }
    return result;
}

Point<int> resize(Point<int> point, double k)
{
    Point<int> result = Point<int>(point.x * k, point.y * k);
    return result;
}

Polygon<int> resizePoly(Polygon<int> poly, double k)
{
    Polygon<int> result = Polygon<int>();
    for (size_t i = 0; i < poly.size(); i++)
    {
        result += resize(poly[i], k);
    }
    return result;
}

Point<int> rotate(Point<int> point, double angle)
{
    while (angle > 360)
        angle -= 360;
    double sinus = std::sin(angle * PI / 180);
    double cosinus = std::cos(angle * PI / 180);
    double rotateMatrix[][2] = {{cosinus, sinus}, {-sinus, cosinus}};
    Point<int> result = Point<int>(0, 0);
    int tempPoint[] = {point.x, point.y};
    for (int j = 0; j < 2; j++)
    {
        result.x += tempPoint[j] * rotateMatrix[j][0];
        result.y += tempPoint[j] * rotateMatrix[j][1];
    }
    return result;
}

Polygon<int> rotatePoly(Polygon<int> poly, double angle)
{
    Polygon<int> result;
    for (size_t i = 0; i < poly.size(); i++)
    {
        result += rotate(poly[i], angle);
    }
    return result;
}

Point<int> shiftX(Point<int> point, double k)
{
    Point<int> result = Point<int>(point.x + point.y * k, point.y);
    return result;
}

Point<int> shiftY(Point<int> point, double k)
{
    Point<int> result = Point<int>(point.x, point.x * k + point.y);
    return result;
}

Polygon<int> shiftXPoly(Polygon<int> poly, double k)
{
    Polygon<int> result = Polygon<int>();
    for (size_t i = 0; i < poly.size(); i++)
    {
        result += shiftX(poly[i], k);
    }
    return result;
}

Polygon<int> shiftYPoly(Polygon<int> &poly, double k)
{
    Polygon<int> result = Polygon<int>();
    for (size_t i = 0; i < poly.size(); i++)
    {
        result += shiftY(poly[i], k);
    }
    return result;
}
