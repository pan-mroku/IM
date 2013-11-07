#include "basealgorithm.hpp"

BaseAlgorithm::~BaseAlgorithm(){}

void BaseAlgorithm::operator ()(Magick::Image& image)
{
  DoYourJob(image);
}

int SimpleAlgorithm::DoYourJob(Magick::Image& image)
{
  //"Note: The main benefit of the 'Pixels' class is that it provides efficient access to raw image pixels"

  image.modifyImage();
  Magick::Pixels pixelCache(image);
  for(int x=0;x<image.rows();x++)
    for(int y=0;y<image.columns();y++)
      OperationPerPixel(pixelCache.get(x,y,1,1));
  pixelCache.sync();
  return 0;
}
