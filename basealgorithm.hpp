/* Wirtualne klasy bazowe dla algorytmów */

#ifndef BASEALGORITHM_HPP
#define BASEALGORITHM_HPP

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

#endif
