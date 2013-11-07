/* Klasy algorytmów, których można używać.
   Powinny być dostępne poprzez AlgorithmProxy. */

#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "basealgorithm.hpp"
#include "mask.hpp"
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

class AlgorithmSingleMaskConvolution:public BaseAlgorithm
{
public:
  Mask mask;

  AlgorithmSingleMaskConvolution(Mask& mask);  
  virtual int DoYourJob(Magick::Image& image);
};
 
#endif
