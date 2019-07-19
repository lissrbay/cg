#include <iostream>
#include <vector>
#include <string>
#include "parser.hpp"
#include <string.h>

using namespace std;

int getColorCoordinateFromString(string s)
{
    int p1 = s.find('/', 0);
    int p2 = s.find('/', p1 + 1);
    int num = stoi(s.substr(p1 + 1, p2 - 1));
    return num;
}

Model::Model()
{
    freopen("african_head.obj", "r", stdin);
    string c = " ";
    string s1, s2, s3;
    double x, y, z;
    int i = 0;
    while (i < 4)
    {
        if (c == "v" || c == "vt" || c == "vn")
        {
            cin >> x >> y >> z;

            if (c == "v")
                vertex.push_back({x, y, z});
            if (c == "vt")
                textures.push_back({x, y, z});
            if (c == "vn")
                n.push_back({x, y, z});
        }
        if (c == "f")
        {
            cin >> s1 >> s2 >> s3;
            edges.push_back({stoi(s1) - 1, stoi(s2) - 1, stoi(s3) - 1});
            int x1 = getColorCoordinateFromString(s1);
            int y1 = getColorCoordinateFromString(s2);
            int z1 = getColorCoordinateFromString(s3);
            textureEdges.push_back({x1, y1, z1});
        }
        cin >> c;
        if (c == "#")  i++;
    }

    fclose(stdin);
}
