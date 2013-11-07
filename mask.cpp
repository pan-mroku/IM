#include "mask.hpp"

Mask::~Mask(){}

Mask::Mask(int width, int height, int weight, std::initializer_list<double> list):Width(width), Height(height), Weight(weight)
{
  auto val=list.begin();
  for(int x=0;x<Width;x++)
    {
      values.push_back(std::vector<double>(Height));
      for(int y=0;y<Height;y++)
        values[x][y]=*val++;
    }
}

const std::vector<double>& Mask::operator[](int x) const
{
  return values[x];
}
