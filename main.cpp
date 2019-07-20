#include "tgaimage.hpp"
#include <cmath>
#include <vector>
#include "parser.hpp"
#include "algo_wu.hpp"
#include "brezenhem.hpp"
#include "filling.hpp"
#include "otherdrawalgo.hpp"
#include "polygon.hpp"
#include "cutting.hpp"
#include "transformation.hpp"
#include "cutting_poly.hpp"

using namespace std;
TGAColor white = TGAColor(255, 255, 255, 255);
TGAColor violet = TGAColor(255, 0, 255, 255);
TGAColor red = TGAColor(255, 0, 60, 255);

void testBrezenhem(Model *model)
{
    TGAImage image(1000, 1000, TGAImage::RGB);

    for (size_t i = 0; i < model->edges.size(); i++)
    {
        vector<vector<double>> vertexes;
        for (size_t j = 0; j < 3; j++)
        {
            vertexes.push_back(model->vertex[model->edges[i][j]]);
        }

        vector<Point<int>> edge = toScreenCoordinatePoints(vertexes);
        Triangle<int> drawingTriangle = Triangle<int>(edge);
        drawPolygon(drawingTriangle, image, violet);
    }

    image.save("out1.tga");
}

void testWu(Model *model)
{
    TGAImage image(1000, 1000, TGAImage::RGB);

    for (size_t i = 0; i < model->edges.size(); i++)
    {
        vector<vector<double>> vertexes;
        for (size_t j = 0; j < 3; j++)
        {
            vertexes.push_back(model->vertex[model->edges[i][j]]);
        }

        vector<Point<int>> edge = toScreenCoordinatePoints(vertexes);
        Triangle<int> drawingTriangle = Triangle<int>(edge);
        drawPolygonWu(drawingTriangle, image, violet);
    }

    image.save("out2.tga");
}

void testLineAndCircleBrezenhem()
{
    TGAImage image(1000, 1000, TGAImage::RGB);
    drawLine(Line<int>(Point<int>(100, 100), Point<int>(230, 240)), image, white);
    drawCircle(Point<int>(250, 250), 80, white, image);
    image.save("out3.tga");
}

void testCircleParametrised()
{
    TGAImage image(1000, 1000, TGAImage::RGB);
    parametrisedCircle(Point<int>(250, 250), 80, white, image);
    parametrisedCircleByAngle(Point<int>(500, 500), 80, violet, image);
    DDAline(Line<int>(Point<int>(100, 140), Point<int>(180, 120)), white, image);
    image.save("out4.tga");
}

void testTriangleFill()
{
    TGAImage image(1000, 1000, TGAImage::RGB);
    Point<int> v1 = Point<int>(100, 1000);
    Point<int> v2 = Point<int>(500, 500);
    Point<int> v3 = Point<int>(1000, 600);
    fillPolygonByListOfActiveEdges(Triangle<int>(v1, v2, v3), image, getRandomColor());
    image.save("out5.tga");
}

void testLineSegment()
{
    TGAImage image(1000, 1000, TGAImage::RGB);
    vector<Point<int>> points = {Point<int>(100, 100), Point<int>(100, 500),
                                 Point<int>(500, 500), Point<int>(500, 100)};
    Polygon<int> rect = Polygon<int>(points);
    Line<int> line = Line<int>(Point<int>(1, 1), Point<int>(1000, 300));

    cyrusBeck(line, rect, image, violet);
    drawPolygon(rect, image, white);

    image.save("out6.tga");
}

void testLineSegment2()
{
    TGAImage image(1000, 1000, TGAImage::RGB);
    vector<Point<int>> points = {Point<int>(100, 100), Point<int>(100, 500),
                                 Point<int>(500, 500), Point<int>(550, 260), Point<int>(500, 100)};
    Polygon<int> rect = Polygon<int>(points);
    Line<int> line = Line<int>(Point<int>(1, 1), Point<int>(1000, 300));

    cyrusBeck(line, rect, image, violet);
    drawPolygon(rect, image, white);

    image.save("out7.tga");
}

void testLineSegment3()
{
    TGAImage image(1000, 1000, TGAImage::RGB);
    vector<Point<int>> points = {Point<int>(100, 100), Point<int>(100, 500),
                                 Point<int>(500, 500), Point<int>(500, 100)};
    Polygon<int> rect = Polygon<int>(points);
    Line<int> line = Line<int>(Point<int>(1, 1), Point<int>(1000, 300));

    liangBarsky(line, rect, image, violet);
    drawPolygon(rect, image, white);

    image.save("out8.tga");
}
void testLineSegment4()
{
    TGAImage image(1000, 1000, TGAImage::RGB);
    vector<Point<int>> points = {Point<int>(100, 100), Point<int>(100, 500),
                                 Point<int>(500, 500), Point<int>(500, 100)};
    Polygon<int> rect = Polygon<int>(points);
    Line<int> line = Line<int>(Point<int>(1, 1), Point<int>(1000, 300));

    cohenSutherland(line, rect, image, violet);
    drawPolygon(rect, image, white);

    image.save("out9.tga");
}

void testTexturingModel()
{
    TGAImage image(width, height, TGAImage::RGB);
    TGAImage image2;
    image2.read_tga_file("african_head_diffuse.tga");
    image2.flip_vertically();
    Model *model = new Model();
    for (size_t i = 0; i < model->edges.size(); i++)
    {
        vector<vector<double>> vertexes;
        vector<Point<int>> colors;

        for (size_t j = 0; j < 3; j++)
        {
            vertexes.push_back(model->vertex[model->edges[i][j]]);
            int x = model->textures[model->textureEdges[i][j] - 1][0] * image2.get_width();
            int y = model->textures[model->textureEdges[i][j] - 1][1] * image2.get_height();
            colors.push_back(Point<int>(x, y, 0));
        }

        vector<Point<int>> edge = toScreenCoordinatePoints(vertexes);
        Triangle<int> coordinateTriangle = Triangle<int>(edge);
        Triangle<int> colorsTriangle = Triangle<int>(colors);
        drawTriangleWithTexturing(coordinateTriangle, image, image2, colorsTriangle);
    }

    image.save("out10.tga");
}

void testPolyClip()
{
    TGAImage image(width, height, TGAImage::RGB);
    vector<Point<int>> points = {Point<int>(45, 326), Point<int>(321, 220), Point<int>(0, 0)};
    Polygon<int> main = Polygon<int>(points);
    points = {Point<int>(100, 100), Point<int>(150, 520), Point<int>(500, 520), Point<int>(500, 100)};
    Polygon<int> clip = Polygon<int>(points);
    Polygon<int> ans = sutherlandHodgmanClip(clip, main);

    drawPolygon(main, image, violet);
    drawPolygon(clip, image, white);
    drawPolygon(ans, image, red);

    image.save("out11.tga");
}

void testTransformPoly()
{
    TGAImage image(width, height, TGAImage::RGB);
    vector<Point<int>> points = {Point<int>(100, 100), Point<int>(100, 500),
                                 Point<int>(500, 500), Point<int>(500, 100)};
    Polygon<int> rect = Polygon<int>(points);
    Polygon<int> newRect;
    newRect = transferPoly(rect, {180, 100});
    newRect = rotatePoly(newRect, 30);
    newRect = resizePoly(newRect, 0.5);
    newRect = shiftXPoly(newRect, 1.0 / 3);
    for (size_t i = 0; i < rect.size(); i++)
    {
        drawLine(Line<int>(rect[i], rect[(i + 1) % rect.size()]), image, white);
        drawLine(Line<int>(newRect[i], newRect[(i + 1) % newRect.size()]), image, violet);
        drawLine(Line<int>(rect[i], newRect[i]), image, red);
    }

    image.save("out12.tga");
}

void testPolyClip2()
{
    TGAImage image(width, height, TGAImage::RGB);
    vector<Point<int>> points = {Point<int>(45, 326), Point<int>(321, 220), Point<int>(0, 0), Point<int>(45, 326)};
    Polygon<int> main2 = Polygon<int>(points);
    points = {Point<int>(100, 100), Point<int>(150, 520), Point<int>(500, 520), Point<int>(500, 100), Point<int>(100, 100)};
    Polygon<int> clip2 = Polygon<int>(points);
    points = {Point<int>(45, 326), Point<int>(321, 220), Point<int>(0, 0), Point<int>(45, 326)};
    Polygon<int> main = Polygon<int>(points);
    points = {Point<int>(100, 100), Point<int>(150, 520), Point<int>(500, 520), Point<int>(500, 100), Point<int>(100, 100)};
    Polygon<int> clip = Polygon<int>(points);
    drawPolygon(main, image, violet);
    drawPolygon(clip, image, white);

    std::vector<Polygon<int>> ans2 = weilerAtherton(main2, clip2);
    for (size_t i = 0; i < ans2.size(); i++)
    {
        Polygon<int> ans = ans2[i];
        drawPolygon(ans, image, red);
    }
    image.save("out13.tga");
}

int main(int argc, char **argv)
{
    Model *model = new Model();
    testBrezenhem(model); //голова
    testWu(model);        //тоже, но с Ву
    testLineAndCircleBrezenhem();
    testCircleParametrised();
    testLineSegment();
    testLineSegment2();
    testLineSegment3();
    testLineSegment4();
    testTriangleFill();
    testTransformPoly();
    testTexturingModel();
    testPolyClip();
    testPolyClip2();

    return 0;
}
