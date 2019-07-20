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

Point<int> intersection(Point<int> p1, Point<int> p2, Point<int> ip, Point<int> kp)
{
    int nx = x_intersect(p1.x, p1.y, p2.x, p2.y, ip.x, ip.y, kp.x, kp.y);
    int ny = y_intersect(p1.x, p1.y, p2.x, p2.y, ip.x, ip.y, kp.x, kp.y);
    return Point<int>(nx, ny);
}

Polygon<int> clip(Polygon<int> &poly_points, Point<int> p1, Point<int> p2)
{
    Polygon<int> new_points;
    int poly_size = poly_points.size();
    for (int i = 0; i < poly_size; i++)
    {
        int k = (i + 1) % poly_size;
        Point<int> kp = poly_points[k];
        Point<int> ip = poly_points[i];
        int i_pos = (p2.x - p1.x) * (ip.y - p1.y) - (p2.y - p1.y) * (ip.x - p1.x);
        int k_pos = (p2.x - p1.x) * (kp.y - p1.y) - (p2.y - p1.y) * (kp.x - p1.x);

        if (i_pos < 0 && k_pos < 0)
        {
            new_points += kp;
        }
        else if (i_pos >= 0 && k_pos < 0)
        {
            new_points += intersection(p1, p2, ip, kp);
            new_points += kp;
        }
        else if (i_pos < 0 && k_pos >= 0)
        {
            new_points += intersection(p1, p2, ip, kp);
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
        ans = clip(ans, clip_points[i], clip_points[k]);
    }

    return ans;
}

int cmp(double d)
{
    if (fabs(d) < 0.0000001)
        return 0;
    return (d > 0) ? 1 : -1;
}
double det(double x1, double y1, double x2, double y2)
{
    return x1 * y2 - x2 * y1;
}
double cross(Vertex<int> a, Vertex<int> b, Vertex<int> c)
{
    return det(b.p.x - a.p.x, b.p.y - a.p.y, c.p.x - a.p.x, c.p.y - a.p.y);
}
double cos(Vertex<int> &a, Vertex<int> &b, Vertex<int> &c)
{
    double x1 = b.p.x - a.p.x, y1 = b.p.y - a.p.y, x2 = c.p.x - a.p.x, y2 = c.p.y - a.p.y;
    return (x1 * x2 + y1 * y2) / (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2));
}
bool isIntersected(Vertex<int> &a, Vertex<int> &b, Vertex<int> &c, Vertex<int> &d)
{
    int c1 = cmp(cross(a, c, d)), c2 = cmp(cross(b, c, d)), c3 = cmp(cross(c, a, b)), c4 = cmp(cross(d, a, b));
    if (((c1 ^ c2) == -2 || ((c1 & c2) == 0 && (c1 | c2) != 0)) && ((c3 ^ c4) == -2 || ((c3 & c4) == 0 && (c1 | c2) != 0)))
        return 1;
    return 0;
}
bool isOnLine(Vertex<int> &p, Vertex<int> &p1, Vertex<int> &p2)
{
    return (!cmp(cross(p, p1, p2)) && ((p.p.x - p1.p.x) * (p.p.x - p2.p.x) <= 0) && ((p.p.y - p1.p.y) * (p.p.y - p2.p.y) <= 0));
}
bool lineCross(Vertex<int> &a, Vertex<int> &b, Vertex<int> &c, Vertex<int> &d, Vertex<int> &i)
{
    if (isIntersected(a, b, c, d))
    {
        double dx1 = b.p.x - a.p.x, dx2 = d.p.x - c.p.x, dx3 = c.p.x - a.p.x, dy1 = b.p.y - a.p.y, dy2 = d.p.y - c.p.y, dy3 = c.p.y - a.p.y;
        double dx = dy1 * dx2 - dx1 * dy2;
        i.t1 = (dx2 * dy3 - dx3 * dy2) / dx;
        i.t2 = (dx1 * dy3 - dx3 * dy1) / dx;
        i.p.x = a.p.x + i.t1 * dx1;
        i.p.y = a.p.y + i.t1 * dy1;
        i.isIntersection = 1;
        i.visited = 0;
        i.next = i.equal = nullptr;
        return 1;
    }
    return 0;
}
Vertex<int> *add(Vertex<int> *p, Vertex<int> *q, Vertex<int> &i, int type)
{
    Vertex<int> *newpoint = new Vertex<int>(), *r = p;
    *(newpoint) = i;
    if (type == 1)
    {
        while (r->next != q && (r->next)->t1 <= i.t1)
            r = r->next;
    }
    else
    {
        while (r->next != q && (r->next)->t2 <= i.t2)
            r = r->next;
    }
    newpoint->next = r->next;
    r->next = newpoint;
    return newpoint;
}
int addfirst(Vertex<int> i, Vertex<int> *p, Vertex<int> *q, Vertex<int> *j, Vertex<int> *k, Vertex<int> *e1, Vertex<int> *e2, Vertex<int> *temp)
{
    i.t2 = 0;
    e1 = add(p, q, i, 1);
    if (k->equal)
        e2 = add(k->equal, temp, i, 2);
    else
        e2 = add(k, temp, i, 2);
    e1->equal = e2;
    e2->equal = e1;
    return 0;
}
int addsecond(Vertex<int> i, Vertex<int> *p, Vertex<int> *q, Vertex<int> *j, Vertex<int> *k, Vertex<int> *e1, Vertex<int> *e2)
{
    i.t2 = 1;
    e1 = add(p, q, i, 1);
    e2 = add(j, k, i, 2);
    e1->equal = e2;
    e2->equal = e1;
    return 1;
}

void Intersect(Vertex<int> *p, Vertex<int> *q, Vertex<int> *main)
{
    Vertex<int> *j = main, *k, *e1, *e2;
    Vertex<int> i = Vertex<int>(Point<int>(0, 0));
    while (j)
    {
        k = j->next;
        while (k->isIntersection)
            k = k->next;
        if (j->equal && isOnLine(*j, *p, *q))
        {
            j = k;
            continue;
        }
        if (isOnLine(*k, *p, *q))
        {
            Vertex<int> *temp = k->next;
            if (k->equal)
                temp = (k->equal)->next;
            while (temp->isIntersection)
                temp = temp->next;
            i = *k;
            if (q->p.x != p->p.x)
                i.t1 = (k->p.x - p->p.x) / (q->p.x - p->p.x);
            else
                i.t1 = (k->p.y - p->p.y) / (q->p.y - p->p.y);
            i.isIntersection = 1;
            i.next = i.equal = nullptr;
            if (cos(*k, *j, *p) < cos(*k, *temp, *p))
            {
                i.t2 = addfirst(i, p, q, j, k, e1, e2, temp);
                i.t2 = addsecond(i, p, q, j, k, e1, e2);
            }
            else
            {
                i.t2 = addsecond(i, p, q, j, k, e1, e2);
                i.t2 = addfirst(i, p, q, j, k, e1, e2, temp);
            }
            if (k->equal)
                j = k->next;
            else
            {
                j = temp;
                if (j->equal)
                    j = j->next;
            }
            continue;
        }
        if (lineCross(*p, *q, *j, *k, i))
        {
            addsecond(i, p, q, j, k, e1, e2);
        }
        j = k;
        if (j->equal)
            j = j->next;
    }
}

void getVertex(Polygon<int> main, Polygon<int> clip, Vertex<int> *mainhead, Vertex<int> *cliphead) 
{
    Vertex<int> *n, *r, *start;
    bool flag = 0;

    start = r = mainhead;
    for (size_t i = 1; i < main.size(); i++)
    {
        Vertex<int> *n = new Vertex<int>(main[i]);
        if (flag)
        {
            start = n;
            flag = 0;
        }
        if (n->p.x == start->p.x && n->p.y == start->p.y)
        {
            n->equal = start;
            start->equal = n;
            flag = 1;
        }
        r->next = n;
        r = n;
    }
    r = cliphead;
    for (size_t i = 1; i < clip.size(); i++)
    {
        Vertex<int> *n = new Vertex<int>(clip[i]);
        r->next = n;
        r = n;
    }
    start = r = cliphead;
    while (r)
    {
        n = r->next;
        Intersect(r, n, mainhead);
        r = n;
        if (r->p.x == start->p.x && r->p.y == start->p.y)
        {
            r->equal = start;
            start->equal = r;
            r = r->next;
            start = r;
        }
    }
}

bool isInClip(Vertex<int> *p, Polygon<int> clip)
{
    int count = 0, start = 0;
    Vertex<int> p1, p2, l;
    l.p.y = p->p.y;
    l.p.x = INFTY;
    for (size_t i = 0; i < clip.size() - 1; i++)
    {
        p1.p = clip[i];
        p2.p = clip[i + 1];
        if (isOnLine(*p, p1, p2))
            return 1;
        if (clip[i + 1].x == clip[start].x && clip[i + 1].y == clip[start].y)
        {
            i++;
            start = i + 1;
        }
        if (!cmp(p1.p.y - p2.p.y))
            continue;
        if (isOnLine(p1, *p, l))
        {
            if (p1.p.y > p2.p.y)
                count++;
        }
        else if (isOnLine(p2, *p, l))
        {
            if (p2.p.y > p1.p.y)
                count++;
        }
        else if (isIntersected(*p, l, p1, p2))
            count++;
    }
    if (count % 2 == 1)
        return 1;
    return 0;
}
Polygon<int> addPoints(Polygon<int> pointarray)
{
    Polygon<int> ans;
    for (size_t i = 0; i < pointarray.size(); i++)
    {
        Point<int> n = pointarray[i];
        ans += n;
    }
    return ans;
}

std::vector<Polygon<int>> output(Vertex<int> *main, Vertex<int> *clip, Polygon<int> clipArray)
{
    Vertex<int> *r = main, *n = main;
    Polygon<int> outArray;
    int outlength = 0;
    bool type = 0;
    std::vector<Polygon<int>> ans;

    while (r)
    {
        while (r && (!r->isIntersection || r->visited))
        {
            n = r;
            r = r->next;
        }
        if (!r)
            break;
        if (!n->isIntersection && isInClip(n, clipArray) && ((r->p.x != n->p.x) || (r->p.y != n->p.y)))
        {
            r = r->equal;
            type = 1;
        }
        outArray += r->p;
        r->visited = 1;
        r = r->next;
        while (r)
        {
            while (!r->isIntersection && !r->visited)
            {
                outArray += r->p;
                r->visited = 1;
                if (r->equal)
                    r = r->equal;
                r = r->next;
            }
            if (r->visited)
                break;
            if (r->p.x != outArray[outlength - 1].x || r->p.y != outArray[outlength - 1].y)
            {
                outArray += r->p;
            }
            r->visited = 1;
            (r->equal)->visited = 1;
            r = (r->equal)->next;
        }
        ans.push_back(addPoints(outArray));
        if (type)
        {
            r = r->equal;
            type = 0;
        }
        n = r;
        r = r->next;
    }
    r = main;
    while (r)
    {
        n = r;
        r = r->next;
        delete n;
    }
    r = clip;
    while (r)
    {
        n = r;
        r = r->next;
        delete n;
    }
    return ans;
}

std::vector<Polygon<int>> weilerAtherton(Polygon<int> main, Polygon<int> clip)
{
    Vertex<int> *mainhead = new Vertex<int>(main[0]), *cliphead = new Vertex<int>(clip[0]);
    getVertex(main, clip, mainhead, cliphead);
    return output(mainhead, cliphead, clip);
}
