#include <iostream>
#include <vector>

class Model
{
  public:
    std::vector<std::vector<double>> vertex;
    std::vector<std::vector<int>> edges;
    std::vector<std::vector<double>> textures;
    std::vector<std::vector<int>> textureEdges;
    std::vector<std::vector<double>> n;
    Model();
};
