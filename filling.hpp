#pragma once
#include <vector>
#include "tgaimage.hpp"
#include <queue>
#include "polygon.hpp"

//const int step[8][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
const int step[4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

void simpleFillByEdges(Polygon<int> poly, TGAImage &image, TGAColor color);

void fillPolygonByListOfActiveEdges(Polygon<int> poly, TGAImage &image, TGAColor color);

std::vector<Point<int>> toScreenCoordinatePoints(std::vector<std::vector<double>> vertexes);

void drawTriangleWithTexturing(Polygon<int> poly, TGAImage &image, TGAImage &image2, Polygon<int> colors);

std::vector<std::vector<int>> fillPolyByListOfEdge(Polygon<int> poly);
