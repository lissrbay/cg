#pragma once
#include "tgaimage.hpp"
#include "brezenhem.hpp"

void parametrisedCircle(Point<int> center, int r, TGAColor color, TGAImage &image);

void parametrisedCircleByAngle(Point<int> center, int r, TGAColor color, TGAImage &image);

void DDAline(Line<int> line, TGAColor color, TGAImage &image);
