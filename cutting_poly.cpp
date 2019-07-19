#include <iostream>
#include "cutting_poly.hpp"
#include <vector>
#include <cmath>
#include "polygon.hpp"

using namespace std;

int x_intersect(int x1, int y1, int x2, int y2,
                int x3, int y3, int x4, int y4)
{
    int num = (x1 * y2 - y1 * x2) * (x3 - x4) -
              (x1 - x2) * (x3 * y4 - y3 * x4);
    int den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    return num / den;
}

int y_intersect(int x1, int y1, int x2, int y2,
                int x3, int y3, int x4, int y4)
{
    int num = (x1 * y2 - y1 * x2) * (y3 - y4) -
              (y1 - y2) * (x3 * y4 - y3 * x4);
    int den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    return num / den;
}

Polygon<int> clip(Polygon<int> &poly_points,
          Point<int> p1, Point<int> p2)
{
    Polygon<int> new_points;
    int new_poly_size = 0, poly_size = poly_points.size();
    for (int i = 0; i < poly_size; i++)
    {
        int k = (i + 1) % poly_size;
        int ix = poly_points[i].x, iy = poly_points[i].y;
        int kx = poly_points[k].x, ky = poly_points[k].y;

        int i_pos = (p2.x - p1.x) * (iy - p1.y) - (p2.y - p1.y) * (ix - p1.x);

        int k_pos = (p2.x - p1.x) * (ky - p1.y) - (p2.y - p1.y) * (kx - p1.x);

        if (i_pos < 0 && k_pos < 0)
        {
            new_points += Point<int>(kx, ky);
            new_poly_size++;
        }
        else if (i_pos >= 0 && k_pos < 0)
        {
            int nx = x_intersect(p1.x, p1.y, p2.x, p2.y, ix, iy, kx, ky);
            int ny = y_intersect(p1.x, p1.y, p2.x, p2.y, ix, iy, kx, ky);
            new_points += Point<int>(nx, ny);
            new_points += Point<int>(kx, ky);
            new_poly_size += 2;
        }
        else if (i_pos < 0 && k_pos >= 0)
        {
            int nx = x_intersect(p1.x, p1.y, p2.x, p2.y, ix, iy, kx, ky);
            int ny = y_intersect(p1.x, p1.y, p2.x, p2.y, ix, iy, kx, ky);
            new_points += Point<int>(nx, ny);
            new_poly_size++;
        }
    }

    return new_points;
}

Polygon<int> sutherlandHodgmanClip(Polygon<int> &poly_points,
                          Polygon<int> &clip_points)
{
    size_t clip_size = clip_points.size();
    Polygon<int> ans = poly_points;
    for (size_t i = 0; i < clip_size; i++)
    {
        int k = (i + 1) % clip_size;
        ans = clip(ans, clip_points[i],
             clip_points[k]);
    }

    return ans;
}
