#include <cmath>
#include "tgaimage.hpp"
#include "brezenhem.hpp"

void parametrisedCircle(Point<int> center, int r, TGAColor color, TGAImage &image)
{
    int n = round(r / sqrt(2));
    double y = r;
    for (int x = 0; x <= n; x++)
    {
        CirclePixel(center, Point<int>(x, y), color, image);
        y = round(sqrt(pow(r, 2) - pow(x, 2)));
    }
}

void parametrisedCircleByAngle(Point<int> center, int r, TGAColor color, TGAImage &image)
{

    int x1, x2, y1, y2;
    x2 = center.x + r;
    y2 = center.y;
    for (int a = 1; a <= 360; a++)
    {
        x1 = x2;
        y1 = y2;
        x2 = round(r * cos(a)) + center.x;
        y2 = round(r * sin(a)) + center.x;
        drawLine(Line<int>(Point<int>(x2, y2), Point<int>(x1, y1)), image, color);
    }
}

void DDAline(Line<int> line, TGAColor color, TGAImage &image)
{
    int x1 = line[0].x, x2 = line[1].x;
    int y1 = line[1].y, y2 = line[1].y;
    double l = std::max(x2 - x1, y2 - y1);
    int x = x1;
    int y = y1;
    while (x <= x2)
    {
        x += (x2 - x1) / l;
        y += (y2 - y1) / l;
        line.sw ? image.set(y, x, color) : image.set(x, y, color);
    }
}