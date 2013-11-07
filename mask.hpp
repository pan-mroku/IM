#ifndef MASK_HPP
#define MASK_HPP

#include <vector>

class Mask
{
protected:
  std::vector<std::vector<double>> values;

public:
  const int Width,Height,Weight;

  Mask(int width=3, int height=3, int weight=1, std::initializer_list<double> list={0,0,0,0,0,0,0,0,0});
  virtual ~Mask(); //nie zaszkodzi

  const std::vector<double>& operator[](int x) const;
};

#endif
