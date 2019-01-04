#include "tgaimage.hpp"
#include "tgaimage.hpp"
#include "polygon.hpp"

#pragma once
Polygon<int> resizePoly(Polygon<int> point, double k);

Polygon<int> transferPoly(Polygon<int> point, Point<int> vec);

Polygon<int> rotatePoly(Polygon<int> point, double angle);

Polygon<int> shiftXPoly(Polygon<int> point, double k);

Polygon<int> shiftYPoly(Polygon<int> point, double k);
