#pragma once
#include "tgaimage.hpp"
#include <vector>
#include "polygon.hpp"
const int width  = 1000;
const int height = 1000;
const int depth  = 255;
TGAColor changeColor(TGAColor c, double br);

void drawLine(Line<int> line, TGAImage &image, TGAColor color);

void CirclePixel(Point<int> center, Point<int> radiusP, TGAColor color, TGAImage &image);

void drawCircle(Point<int> center, int r, TGAColor color, TGAImage &image);

void drawPolygon(Polygon<int>, TGAImage &image, TGAColor color);

