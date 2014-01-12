/* Wirtualne klasy bazowe dla algorytmów */

#ifndef BASEALGORITHM_HPP
#define BASEALGORITHM_HPP

#include "mask.hpp"
#include <Magick++.h>

class BaseAlgorithm
{
public:
  virtual ~BaseAlgorithm()=0; //Może kiedyś ktoś będzie alokował jakąś pamięć?
  virtual int DoYourJob(Magick::Image& image)=0;
  void operator()(Magick::Image& image);
};

class SimpleAlgorithm:public BaseAlgorithm
{
public:
  virtual int DoYourJob(Magick::Image& image);
  virtual void OperationPerPixel(Magick::PixelPacket* pixel)=0;
};

class DetailedAlgorithm:public BaseAlgorithm
{
public:
  virtual int DoYourJob(Magick::Image& image);
  virtual void OperationPerPixel(Magick::PixelPacket* pixel, unsigned int x, unsigned int y)=0;
};

class SingleMaskConvolutionAlgorithm:public BaseAlgorithm
{
public:
  Mask ConvolutionMask;

  virtual int DoYourJob(Magick::Image& image);
  virtual void OperationPerPixel(Magick::PixelPacket* pixels, unsigned int x, unsigned int y, Magick::PixelPacket* maskResult)=0;
};

#endif
