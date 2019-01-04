#include "tgaimage.hpp"
#include "brezenhem.hpp"

#pragma once

const int LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8;

void cohenSutherland(Line<int> line, Polygon<int> rect, TGAImage &image, TGAColor color);

void liangBarsky(Line<int> line, Polygon<int> rect, TGAImage &image, TGAColor color);

void cyrusBeck(Line<int> line, Polygon<int> rect, TGAImage &image, TGAColor color);

