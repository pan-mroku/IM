#ifndef SIMPLEALGORITHM_HPP
#define SIMPLEALGORITHM_HPP

#include <Magick++.h>

class SimpleAlgorithm
{
private:

protected:

public:
  int DoYourJob(Magick::Image& image);
  virtual void OperationPerPixel(Magick::PixelPacket* pixel)=0;
  void operator ()(Magick::Image& image);
};

#endif
