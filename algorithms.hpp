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

public:
  double sigma;
  AlgorithmGaussNoise(double s=30000);
};

class AlgorithmTest:public SimpleAlgorithm
{
  virtual void OperationPerPixel(Magick::PixelPacket* pixel);
};

class AlgorithmRead:public SimpleAlgorithm
{
  virtual void OperationPerPixel(Magick::PixelPacket* pixel);
};

class AlgorithmBlur:public SingleMaskConvolutionAlgorithm
{
public:
  Magick::Image Result;
  Magick::PixelPacket* ResultPixels;

  AlgorithmBlur()
  {
    ConvolutionMask.Set(3,3,9,{1,1,1,1,1,1,1,1,1});
  }

  virtual int DoYourJob(Magick::Image& image);
  virtual void OperationPerPixel(Magick::PixelPacket* pixels, unsigned int x, unsigned int y, Magick::PixelPacket* maskResult);
};

class HoughAccumulator:public DetailedAlgorithm
{
public:
  Magick::Image Accumulator;
  Magick::PixelPacket* AccumulatorPixels;
  virtual int DoYourJob(Magick::Image& image);
  virtual void OperationPerPixel(Magick::PixelPacket* pixel, unsigned int x, unsigned int y);
};

class AlgorithmHough:public BaseAlgorithm
{
public:
  AlgorithmI I;
  HoughAccumulator Accu;
  AlgorithmBlur Blur;

  AlgorithmHough();

  virtual int DoYourJob(Magick::Image& image);
};

#endif
