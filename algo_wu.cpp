#include "tgaimage.hpp"
#include <cmath>
#include <vector>
#include "parser.hpp"
#include "brezenhem.hpp"
#include "polygon.hpp"
void drawLineWu(Line<int> line, TGAImage &image, TGAColor color)
{
    line.toCorrect();
    int x1 = line[0].x, x2 = line[1].x;
    int y1 = line[0].y, y2 = line[1].y;
    double dx = x2 - x1;
    double dy = y2 - y1;
    double gradient = dy / dx;

    double y = y1 + gradient;
    for (int x = x1 + 1; x <= x2 - 1; x++)
    {
        line.sw ? image.set((int)y, x, changeColor(color, 1 - (y - (int)y))): image.set(x, (int)y, changeColor(color, 1 - (y - (int)y)));
        line.sw ? image.set((int)y + 1, x, changeColor(color, y - (int)y)) : image.set(x, (int)y + 1, changeColor(color, y - (int)y));
        y += gradient;
    }
}

void drawPolygonWu(Polygon<int> poly, TGAImage &image, TGAColor color)
{
    size_t pSize = poly.size();
    for (size_t i = 0; i < pSize; i++){
        drawLineWu(Line<int>(poly[i], poly[(i + 1) % pSize]), image, color);
    }
}
