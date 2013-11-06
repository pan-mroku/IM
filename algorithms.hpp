#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "basealgorithm.hpp"
#include <Magick++.h>

class AlgorithmI:public SimpleAlgorithm
{
public:
  virtual void OperationPerPixel(Magick::PixelPacket* pixel);
};

class AlgorithmGaussNoise:public SimpleAlgorithm
{
  virtual void OperationPerPixel(Magick::PixelPacket* pixel);
};

class AlgorithmTest:public SimpleAlgorithm
{
  virtual void OperationPerPixel(Magick::PixelPacket* pixel);
};

class AlgorithmRead:public SimpleAlgorithm
{
  virtual void OperationPerPixel(Magick::PixelPacket* pixel);
};
 
#endif