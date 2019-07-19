#pragma once
#include <vector>
#include "polygon.hpp"
#include "tgaimage.hpp"
#include "polygon.hpp"

Polygon<int> sutherlandHodgmanClip(Polygon<int> &poly_points, Polygon<int> &clip_points);
