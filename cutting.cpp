#include "tgaimage.hpp"
#include "brezenhem.hpp"
#include "cutting.hpp"
//все прямоугольники задаются списком вершин в порядке обхода по часовой стрелке, начиная с левого нижнего угла
int getcode(Point<int> point, Polygon<int> rect)
{
    int code = 0;
    if (point.y > rect[2].y)
        code |= TOP;
    if (point.y < rect[0].y)
        code |= BOTTOM;
    if (point.x < rect[0].x)
        code |= LEFT;
    if (point.x > rect[2].x)
        code |= RIGHT;
    return code;
}

Point<int> changeOneOfLineEnds(Line<int> line, Polygon<int> rect, int code)
{
    Point<int> newBegin = line[0], newEnd = line[1];
    Point<int> point = newBegin;
    double m = (double)(newEnd.y - newBegin.y) / (newEnd.x - newBegin.x);

    if (code & TOP)
    {
        point.x = newBegin.x + (rect[2].y - newBegin.y) / m;
        point.y = rect[2].y;
    }
    else if (code & BOTTOM)
    {
        point.x = newBegin.x + (rect[0].y - newBegin.y) / m;
        point.y = rect[0].y;
    }
    else if (code & LEFT)
    {
        point.x = rect[0].x;
        point.y = newBegin.y + m * (rect[0].x - newBegin.x);
    }
    else if (code & RIGHT)
    {
        point.x = rect[2].x;
        point.y = newBegin.y + m * (rect[2].x - newBegin.x);
    }
    return point;
}

void cohenSutherland(Line<int> line, Polygon<int> rect, TGAImage &image, TGAColor color)
{
    Point<int> newBegin = line[0], newEnd = line[1];
    int outcode1 = getcode(line[0], rect), outcode2 = getcode(line[1], rect);
    while (1)
    {
        if (outcode1 == 0 && outcode2 == 0)
        {
            drawLine(Line<int>(newBegin, newEnd), image, color);
            break;
        }
        else if ((outcode1 & outcode2) != 0)
        {
            break;
        }
        else
        {
            int temp;
            outcode1 == 0 ? temp = outcode2 : temp = outcode1;
            Point<int> newPoint = changeOneOfLineEnds(Line<int>(newBegin, newEnd), rect, temp);
            if (temp == outcode1)
            {
                newBegin = newPoint;
                outcode1 = getcode(newBegin, rect);
            }
            else
            {
                newEnd = newPoint;
                outcode2 = getcode(newEnd, rect);
            }
        }
    }
}

void liangBarsky(Line<int> line, Polygon<int> rect, TGAImage &image, TGAColor color)
{
    double x1 = line[0].x, y1 = line[0].y, x2 = line[1].x, y2 = line[1].y;
    int xmin = rect[0].x, xmax = rect[2].x, ymin = rect[0].y, ymax = rect[2].y;
    int newx1, newx2, newy1, newy2;
    double dx, dy;
    double tEnter = 0, tLeave = 1, temp;

    dx = x2 - x1;
    dy = y2 - y1;

    double p[] = {-dx, dx, -dy, dy};
    double q[] = {x1 - xmin, xmax - x1, y1 - ymin, ymax - y1};

    for (int i = 0; i < 4; i++)
    {
        temp = (q[i]) / p[i];
        if (p[i] < 0)
        {
            tEnter = std::max(tEnter, temp);
        }
        else
        {
            tLeave = std::min(tLeave, temp);
        }
    }

    if (tEnter < tLeave)
    {
        newx1 = line[0].x + tEnter * p[1];
        newx2 = line[0].x + tLeave * p[1];
        newy1 = line[0].y + tEnter * p[3];
        newy2 = line[0].y + tLeave * p[3];

        drawLine(Line<int>(Point<int>(newx1, newy1), Point<int>(newx2, newy2)), image, color);
    }
}

template <typename T>
Point<T> getInsideNormal(Point<T> point1, Point<T> point2, Point<T> z)
{
    Point<T> delta = point2 - point1;
    Point<T> n = Point<int>(-delta.y, delta.x, 0);
    Point<T> v = z - point1;
    T dot = dotProduct(v, n);

    if (dot < 0)
    {
        n.x *= -1;
        n.y *= -1;
    }
    return n;
}

void cyrusBeck(Line<int> line, Polygon<int> rect, TGAImage &image, TGAColor color)
{
    line.toCorrect();
    Point<int> D = line[1] - line[0];
    Point<int> boundaryPoint = rect[2];
    double tEnter = 0;
    double tLeave = 1;
    size_t i = 0;
    while (i != rect.size())
    {
        Point<int> p = rect[i];
        i++;
        Point<int> q = rect[i % rect.size()];
        Point<int> n = getInsideNormal(p, q, boundaryPoint);
        Point<int> w = line[0] - p;
        double num = dotProduct(w, n);
        double den = dotProduct(D, n);
        if (den == 0)
        {
            if (num < 0)
            {
                return;
            }
            else
            {
                continue;
            }
        }

        double t = -num / den;
        if (den > 0)
        {
            tEnter = std::max(tEnter, t);
        }
        else
        {
            tLeave = std::min(tLeave, t);
        }
        boundaryPoint = p;
    }
    if (tEnter > tLeave)
    {
        return;
    }
    double x1 = line[0].x + D.x * tEnter;
    double y1 = line[0].y + D.y * tEnter;
    double x2 = line[0].x + D.x * tLeave;
    double y2 = line[0].y + D.y * tLeave;
    drawLine(Line<int>(Point<int>(x1, y1), Point<int>(x2, y2)), image, color);
}
