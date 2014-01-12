#include "algorithms.hpp"
#include "GaussNoise.h"
#include <iostream>

AlgorithmGaussNoise::AlgorithmGaussNoise(double s):SimpleAlgorithm(),sigma(s)
{
}

void AlgorithmGaussNoise::OperationPerPixel(Magick::PixelPacket* pixel)
{
  for(auto pixelChannel : {&pixel->red, &pixel->green, &pixel->blue})
    {
      double noi=GenerateGaussNoise(sigma);//1000-100000
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

int AlgorithmBlur::DoYourJob(Magick::Image& image)
{
  Result=Magick::Image(image.size(), Magick::ColorRGB(0,0,0));
  Magick::Pixels cache(Result);
  ResultPixels=cache.get(0,0,image.size().width(),image.size().height());

  int result=SingleMaskConvolutionAlgorithm::DoYourJob(image);

  cache.sync();

  image=Result;

  return result;
}

void AlgorithmBlur::OperationPerPixel(Magick::PixelPacket* pixels, unsigned int x, unsigned int y, Magick::PixelPacket* maskResult)
{
  ResultPixels[x+Result.size().width()*y]=*maskResult;
}

int HoughAccumulator::DoYourJob(Magick::Image& image)
{
  Magick::Geometry size=image.size();
  int R=floor(sqrt(size.width()*size.width()+size.height()*size.height()));
  Accumulator=Magick::Image(Magick::Geometry(270,R), Magick::ColorRGB(0,0,0));

  Magick::Pixels accuCache(Accumulator);
  AccumulatorPixels=accuCache.get(0,0,270,R);

  int result=DetailedAlgorithm::DoYourJob(image);

  accuCache.sync();
  
  return result;
}

void HoughAccumulator::OperationPerPixel(Magick::PixelPacket* pixel, unsigned int x, unsigned int y)
{
  //Zakładam, że obraz jest już czarno-biały i mogę go modyfikować. Jak ktoś chce, to nie sobie sam kopiuje obraz.
  //skoro obraz jest czarno-biały, to wystarczy sprawdzić jeden kanał

  if(pixel->red<65535)
    return;

  for(int fi=-90;fi<180;fi++)
    {
      double fi_f=fi;
      double r=double(x)*cos(fi_f*3.14/180.0)+double(y)*sin(fi_f*3.14/180.0);

      if(r<0)
        continue;

      //      std::cout<<AccumulatorPixels[fi+90+270*(unsigned int)r].red<<std::endl;
      //wystaczy nam jeden kanał
      AccumulatorPixels[fi+90+270*(unsigned int)r].red+=1;
    }
}

AlgorithmHough::AlgorithmHough():BaseAlgorithm()
{
  //Blur(Mask(3,3,9,{1,1,1,1,1,1,1,1,1}));
}

int AlgorithmHough::DoYourJob(Magick::Image& image)
{
  I(image);
  Accu(image);
  Blur(Accu.Accumulator);

  image=Accu.Accumulator;
}

