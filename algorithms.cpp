#include "algorithms.hpp"
#include "GaussNoise.h"
#include <iostream>

void AlgorithmGaussNoise::OperationPerPixel(Magick::PixelPacket* pixel)
{
  for(auto pixelChannel : {&pixel->red, &pixel->green, &pixel->blue})
    {
      double noi=GenerateGaussNoise(30000);//1000-100000
      *pixelChannel+=round(noi);
    }
}

void AlgorithmI::OperationPerPixel(Magick::PixelPacket* pixel)
{
  float i=3*pixel->red+5*pixel->green+2*pixel->blue;
  i/=10;
  pixel->red=pixel->green=pixel->blue=i;
}

void AlgorithmTest::OperationPerPixel(Magick::PixelPacket* pixel)
{
  pixel->red=pixel->green=pixel->blue=1000;
}

void AlgorithmRead::OperationPerPixel(Magick::PixelPacket* pixel)
{
  for(auto channel : {pixel->red, pixel->green, pixel->blue})
    std::cout<<channel<<" ";
  std::cout<<std::endl;
}

AlgorithmSingleMaskConvolution::AlgorithmSingleMaskConvolution(Mask& mask):mask(mask){}

int AlgorithmSingleMaskConvolution::DoYourJob(Magick::Image& image)
{
  for(int x=0;x<mask.Width;x++)
    {
      for(int y=0;y<mask.Height;y++)
        std::cout<<mask[x][y]<<" ";
      std::cout<<std::endl;
    }
  std::cout<<mask.Weight<<std::endl;
}
