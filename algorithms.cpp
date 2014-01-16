#include "algorithms.hpp"
#include "GaussNoise.h"
#include <iostream>

AlgorithmGaussNoise::AlgorithmGaussNoise(double s):SimpleAlgorithm()
{
  Sigma(s);
}

double AlgorithmGaussNoise::Sigma() const
{
  return sigma/1000;
}

double AlgorithmGaussNoise::Sigma(double s)
{
  if(s>100)
    s=100;
  if(s<0)
    s=0;

  sigma=s*1000; //wartości s z zakresu 1-100.   
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

void AlgorithmBW::OperationPerPixel(Magick::PixelPacket* pixel)
{
  float i=pixel->red+pixel->green+pixel->blue;
  i/=3;
  if(i<(1<<QuantumDepth)/2)
    *pixel=Magick::Color("black");
  else
    *pixel=Magick::Color("white");
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
  R=floor(sqrt(size.width()*size.width()+size.height()*size.height()));
  Accumulator=Magick::Image(Magick::Geometry(270,R), Magick::ColorRGB(0,0,0));

  Magick::Pixels accuCache(Accumulator);
  AccumulatorPixels=accuCache.get(0,0,270,R);
  int max=DetailedAlgorithm::DoYourJob(image);

  accuCache.sync();
  
  return max;
}

void HoughAccumulator::OperationPerPixel(Magick::PixelPacket* pixel, unsigned int x, unsigned int y)
{
  //Zakładam, że obraz jest już czarno-biały i mogę go modyfikować. Jak ktoś chce, to nie sobie sam kopiuje obraz.
  //skoro obraz jest czarno-biały, to wystarczy sprawdzić jeden kanał

  if(pixel->red>0)
    return;

  for(int fi=-90;fi<180;fi++)
    {
      double fi_f=fi;
      double r=double(x)*cos(fi_f*3.14/180.0)+double(y)*sin(fi_f*3.14/180.0);

      if(r<0)
        continue;

      //wystaczy nam jeden kanał
      AccumulatorPixels[fi+90+270*(unsigned int)r].red+=1;
    }
}

HoughResult HoughAccumulator::Maximum()
{
  Magick::Pixels accuCache(Accumulator);
  AccumulatorPixels=accuCache.get(0,0,270,R);

  HoughResult max;

  for(int fi=0;fi<Accumulator.columns();fi++)
    for(int r=0;r<Accumulator.rows();r++)
      if(AccumulatorPixels[fi+r*270].red>max.Value)
        {
          max.Value=AccumulatorPixels[fi+r*270].red;
          max.R=r;
          max.Fi=fi;
        }
  accuCache.sync();
  
  return max;
}

int AlgorithmHough::DoYourJob(Magick::Image& image)
{
  BW(image);
  std::cout<<"BW"<<std::endl;
  Accu(image);
  std::cout<<"Ac"<<std::endl;
  Blur(Accu.Accumulator);
  std::cout<<"Bl"<<std::endl;

  HoughResult max=Accu.Maximum();
  std::cout<<"Max"<<std::endl;

  Magick::Image result(image.size(), Magick::Color("white"));
  
  result.modifyImage();
  Magick::Pixels pixelCache(result);
  Magick::PixelPacket* pixels=pixelCache.set(0,0,image.columns(),image.rows());
  for(int x=0;x<image.columns();x++)
    for(int y=0;y<image.rows();y++)
      {
        double fi_f=max.Fi-90;
        double r_=double(x)*cos(fi_f*3.14/180.0)+double(y)*sin(fi_f*3.14/180.0);

        int pixelIndex=x+y*image.columns();

        if(r_<0) //jak wyżej
          continue;
        
        if(round(r_)==max.R)
          pixels[pixelIndex].red=pixels[pixelIndex].blue=pixels[pixelIndex].green=0;       
        else
          pixels[pixelIndex].red=pixels[pixelIndex].blue=pixels[pixelIndex].green=(1<<QuantumDepth)-1;
      }

  pixelCache.sync();

  image=result;

  std::cout<<"R: "<<max.R<<" FI: "<<max.Fi<<" Value: "<<max.Value<<std::endl;
  return 0;
}

int HoughTester::DoYourJob(Magick::Image& image)
{
  std::cout<<"\"noise\";\"R\";\"FI\";\"value\""<<std::endl;

  for(int level=0;level<NumberOfNoiseLevels+2;level++)
    {
      noise.Sigma(double(level)*100/double(NumberOfNoiseLevels+1));

      for(int pass=0;pass<NumberOfTriesPerLevel;pass++)
        {
          Magick::Image copy=image;
          noise(copy);
          bw(copy);
          accumulator(copy);
          blur(accumulator.Accumulator);
          
          HoughResult max=accumulator.Maximum();

          std::cout<<noise.Sigma()<<";"<<max.R<<";"<<max.Fi<<";"<<max.Value<<std::endl;

          if(level==0)
            break;
        }
    }
}
