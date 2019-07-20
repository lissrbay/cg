#include <cmath>
#include "tgaimage.hpp"
#include "brezenhem.hpp"
#include <vector>
#include<iostream>
#include "filling.hpp"

TGAColor changeColor(TGAColor c, double br)
{
    TGAColor tmp = (TGAColor((int)(c.r * br), (int)(c.g * br), (int)(c.b * br), c.a));
    return tmp;
}

void drawLine(Line<int> line, TGAImage &image, TGAColor color)
{
    line.toCorrect();
    int x1 = line[0].x, x2 = line[1].x;
    int y1 = line[0].y, y2 = line[1].y;
    double dx = x2 - x1;
    double dy = y2 - y1;
    double delta = abs(dy) * 2;
    double error = 0;
    int y = y1;
    for (int x = x1; x <= x2; x++)
    {
        line.sw ? image.set(y, x, color) : image.set(x, y, color);
        error += delta;
        if (error > dx)
        {
            y += (y2 > y1 ? 1 : -1);
            error -= 2 * dx;
        }
    }
}
void CirclePixel(Point<int > center, Point<int> radiusP, TGAColor color, TGAImage &image)
{
    image.set(center.x + radiusP.x, center.y + radiusP.y, color);
    image.set(center.x + radiusP.x, center.y - radiusP.y, color);
    image.set(center.x - radiusP.x, center.y + radiusP.y, color);
    image.set(center.x - radiusP.x, center.y - radiusP.y, color);
    image.set(center.x + radiusP.y, center.y + radiusP.x, color);
    image.set(center.x + radiusP.y, center.y - radiusP.x, color);
    image.set(center.x - radiusP.y, center.y + radiusP.x, color);
    image.set(center.x - radiusP.y, center.y - radiusP.x, color);
}
void drawCircle(Point<int> center, int r, TGAColor color, TGAImage &image)
{
    int d = 3 - r * 2;
    int x = 0;
    int y = r;
    while (x <= y)
    {
        CirclePixel(center, Point<int>(x, y), color, image);
        CirclePixel(center, Point<int>(y, x), color, image);

        if (d < 0)
            d += 4 * x + 6;
        else
        {
            d += (x - y) * 4 + 10;
            y -= 1;
        }
        x++;
    }
}

void drawPolygon(Polygon<int> poly, TGAImage &image, TGAColor color)
{
    size_t pSize = poly.size();
    for (size_t i = 0; i < pSize; i++){
        drawLine(Line<int>(poly[i], poly[(i + 1) % pSize]), image, color);
    }

}