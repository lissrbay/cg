#pragma once
#include "tgaimage.hpp"
#include <cmath>
#include <vector>
#include "polygon.hpp"
void drawLineWu(Line<int> line, TGAImage &image, TGAColor color);
void drawPolygonWu(Polygon<int> p, TGAImage &image, TGAColor color);
