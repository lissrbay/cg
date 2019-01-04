#include <vector>
#include "tgaimage.hpp"
#include <iostream>
#include "filling.hpp"
#include "brezenhem.hpp"
#include <cmath>
#include "polygon.hpp"
#include <algorithm>
int *zbuffer = new int[width * height];

void drawSimpleLines(const std::vector<std::vector<int>> &edges, TGAImage &image, TGAColor color)
{
	for (size_t i = 0; i < edges.size(); i++)
	{
		for (size_t j = 0; j < edges[i].size() && j + 1 < edges[i].size(); j += 2)
		{
			for (int v = edges[i][j]; v <= edges[i][j + 1]; v++)
			{
				image.set(v, i, color);
			}
		}
	}
}

std::vector<std::vector<int>> createListOfEdgePoints(Polygon<int> poly)
{
	std::vector<std::vector<int>> edges;
	edges.resize(height + 1);
	for (size_t i = 0; i < poly.size() + 1; i++)
	{
		double x0 = poly[(i) % poly.size()].x, x1 = poly[(i + 1) % poly.size()].x;
		double y0 = poly[(i) % poly.size()].y, y1 = poly[(i + 1) % poly.size()].y;
		Line<int> edge = Line<int>(Point<int>(x0, y0), Point<int>(x1, y1));
		edge.toCorrect();
		int dx = edge[1].x - edge[0].x;
		int dy = edge[1].y - edge[0].y;
		int delta = abs(dy) * 2;
		int error = 0;
		int y = edge[0].y;
		int lx = edge[0].x;
		for (int x = edge[0].x; x <= edge[1].x; x++)
		{
			if (edge.sw)
				edges[x].push_back(y);

			error += delta;
			if (error >= dx)
			{
				if (!edge.sw)
					edges[y].push_back(x);
				y += (edge[1].y > edge[0].y ? 1 : -1);
				error -= 2 * dx;
				lx = x;
			}
		}
		if (lx != edge[1].x && !edge.sw)
			edges[y].push_back(edge[1].x);
	}
	return edges;
}

void removeEdges(int scanline, std::vector<Line<int>> &activeEdges)
{
	for (std::vector<Line<int>>::iterator it = activeEdges.begin(); it < activeEdges.end();)
	{
		if (it->maxY == scanline)
		{
			activeEdges.erase(it);
		}
		else
		{
			it++;
		}
	}
}
void updateCoordinate(std::vector<Line<int>> &activeEdges)
{
	for (std::vector<Line<int>>::iterator it = activeEdges.begin(); it < activeEdges.end(); it++)
	{
		it->xVal += (1 / it->slope);
	}
}

void updateActiveEdges(int scanline, std::vector<Line<int>> &activeEdges, std::vector<Line<int>> &allEdges)
{
	for (std::vector<Line<int>>::iterator it = allEdges.begin(); it < allEdges.end(); it++)
	{
		if (it->minY == scanline)
		{
			activeEdges.push_back(*it);
		}
		if (it->minY > scanline)
		{
			return;
		}
	}
}

int initScanline(std::vector<Line<int>> &allEdges)
{
	if (allEdges.size() != 0)
	{
		return allEdges.at(0).minY;
	}
	return 0;
}

void sortAndFilterEdges(std::vector<Line<int>> &allEdges)
{
	sort(allEdges.begin(), allEdges.end(), Line<int>::less_than_key());
	for (std::vector<Line<int>>::iterator it = allEdges.begin(); it < allEdges.end(); it++)
	{
		if (it->slope == 0)
		{
			allEdges.erase(it);
		}
	}
}

void sortEdges(std::vector<Line<int>> activeEdges)
{
	sort(activeEdges.begin(), activeEdges.end(), Line<int>::by_x_val_key());
}

void fillPolygonByListOfActiveEdges(Polygon<int> poly, TGAImage &image, TGAColor color)
{
	std::vector<Line<int>> allEdges;
	std::vector<Line<int>> activeEdges;
	for (size_t i = 0; i < poly.size(); i++)
	{
		double x0 = poly[i].x, x1 = poly[(i + 1) % poly.size()].x;
		double y0 = poly[i].y, y1 = poly[(i + 1) % poly.size()].y;
		Line<int> newEdge = Line<int>(Point<int>(x0, y0), Point<int>(x1, y1));
		allEdges.push_back(newEdge);
	}
	sortAndFilterEdges(allEdges);
	int scanline = initScanline(allEdges);
	updateActiveEdges(scanline, activeEdges, allEdges);
	while (activeEdges.size() != 0)
	{
		for (std::vector<Line<int>>::iterator it = activeEdges.begin(); it < activeEdges.end(); it++)
		{
			Line<int> l = Line<int>(Point<int>(it->xVal, scanline), Point<int>((it + 1)->xVal, scanline));
			drawLine(l, image, color);
			it++;
		}
		scanline++;
		removeEdges(scanline, activeEdges);
		updateCoordinate(activeEdges);
		updateActiveEdges(scanline, activeEdges, allEdges);
		sortEdges(activeEdges);
	}
	allEdges.clear();
}

Point<double> light_dir = Point<double>(0, 0, -1);

Point<int> toScreen(std::vector<double> w)
{
	int x0 = ((w[0] + 1.0) * width / 2.0);
	int y0 = ((w[1] + 1.0) * height / 2.0);
	int z0 = w.size() > 2 ? ((w[2] + 1.0) * depth / 2.0) : 0;
	Point<int> ans = {x0, y0, z0};
	return ans;
}

std::vector<Point<int>> toScreenCoordinatePoints(std::vector<std::vector<double>> vertexes)
{
	Point<int> firstVertex = toScreen(vertexes[0]);
	Point<int> secondVertex = toScreen(vertexes[1]);
	Point<int> thirdVertex = toScreen(vertexes[2]);
	std::vector<Point<int>> newTriangle = {firstVertex, secondVertex, thirdVertex};
	return newTriangle;
}

double getIntensity(Polygon<int> poly)
{
	Point<int> v = vec(poly[2] - poly[0], poly[1] - poly[0]);
	Point<double> n = normalise(v);
	return dotProduct(n, light_dir);
}
void getColorTexture(Point<int> pixel, TGAImage &image, TGAImage &image2, Polygon<int> poly, Polygon<int> colors)
{

	double aup = poly[0].x - pixel.x;
	double bup = poly[1].x - pixel.x;
	double cup = poly[2].x - pixel.x;
	double avp = poly[0].y - pixel.y;
	double bvp = poly[1].y - pixel.y;
	double cvp = poly[2].y - pixel.y;

	double f = 1.0f / ((poly[1].x - poly[0].x) * (poly[2].y - poly[0].y) - (poly[1].y - poly[0].y) * (poly[2].x - poly[0].x));
	double u = (bup * cvp - bvp * cup) * f;
	double v = (cup * avp - cvp * aup) * f;
	double w = 1.0f - (u + v);

	int cx1 = u * colors[0].x + v * colors[1].x + w * colors[2].x;
	int cy1 = u * colors[0].y + v * colors[1].y + w * colors[2].y;
	int z1 = u * poly[0].z + v * poly[1].z + w * poly[2].z;
	int idx = pixel.x + pixel.y * width;
	if (idx < width * height && zbuffer[idx] < z1)
	{
		zbuffer[idx] = z1;
		TGAColor newColor = image2.get(cx1, cy1);
		newColor = changeColor(newColor, getIntensity(poly));
		image.set(pixel.x, pixel.y, newColor);
	}
}

void drawLinesTexturing(const std::vector<std::vector<int>> &edges, TGAImage &image, TGAImage &image2, Polygon<int> points, Polygon<int> colors)
{
	for (size_t i = 0; i < edges.size(); i++)
	{
		for (size_t j = 0; j + 1 < edges[i].size(); j += 2)
		{
			int x1 = edges[i][j], x2 = edges[i][j + 1];

			if (x1 > x2)
				std::swap(x1, x2);
			for (int v = x1; v <= x2; v++)
			{
				getColorTexture(Point<int>(v, i), image, image2, points, colors);
			}
		}
	}
}

void simpleFillByEdges(Polygon<int> poly, TGAImage &image, TGAColor color)
{
	std::vector<std::vector<int>> edges = createListOfEdgePoints(poly);
	drawSimpleLines(edges, image, color);
}

void drawTriangleWithTexturing(Polygon<int> poly, TGAImage &image, TGAImage &image2, Polygon<int> colors)
{
	double intensity = getIntensity(poly);

	if (intensity > 0)
	{
		std::vector<std::vector<int>> edges = createListOfEdgePoints(poly);
		drawLinesTexturing(edges, image, image2, poly, colors);
	}
}
