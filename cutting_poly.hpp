#pragma once
#include <vector>
#include "polygon.hpp"
#include "tgaimage.hpp"
#include "polygon.hpp"

Polygon<int> sutherlandHodgmanClip(Polygon<int> &poly_points, Polygon<int> &clip_points);

const int INFTY = 1e4;
const int MAX = 100;
template <typename T>
class Vertex
{
  public:
    Point<T> p;
    Vertex(Point<T> tmp) : p(tmp)
    {
        next = equal = nullptr;
        isIntersection = visited = false;
    };
    Vertex() : p(Point<T>(0, 0))
    {
        next = equal = nullptr;
        isIntersection = visited = false;
    };
    double t1, t2;
    bool isIntersection, visited;
    Vertex<T> *next, *equal;
};

std::vector<Polygon<int>> weilerAtherton(Polygon<int> main, Polygon<int> clip);