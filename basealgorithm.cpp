#include "basealgorithm.hpp"
#include <iostream>

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
  for(int x=0;x<image.columns();x++)
    for(int y=0;y<image.rows();y++)
      OperationPerPixel(pixelCache.get(x,y,1,1));
  pixelCache.sync();
  return 0;
}

int DetailedAlgorithm::DoYourJob(Magick::Image& image)
{
  image.modifyImage();
  Magick::Pixels pixelCache(image);
  for(int x=0;x<image.columns();x++)
    for(int y=0;y<image.rows();y++)
      OperationPerPixel(pixelCache.get(x,y,1,1),x,y);
  pixelCache.sync();
  return 0;
}

int SingleMaskConvolutionAlgorithm::DoYourJob(Magick::Image& image)
{
  image.modifyImage();
  Magick::Pixels pixelCache(image);
  Magick::PixelPacket* pixels;
  for(int x=0;x<image.columns();x++)
    {
      int rectangleWidth=ConvolutionMask.Width;
      int maskX=x-ConvolutionMask.Width/2;
      if(maskX<0)
        {
          rectangleWidth+=maskX;
          maskX=0;
        }
      if(maskX+rectangleWidth>image.columns())
        rectangleWidth=image.columns()-maskX;

      for(int y=0;y<image.rows();y++)
        {
          int rectangleHeight=ConvolutionMask.Height;
          int maskY=y-ConvolutionMask.Height/2;
          if(maskY<0)
            {
              rectangleHeight+=maskY;
              maskY=0;
            }
          if(maskY+rectangleHeight>image.rows())
            rectangleHeight=image.rows()-maskY;
          
          pixels=pixelCache.get(maskX,maskY,rectangleWidth,rectangleHeight);
          Magick::PixelPacket maskResult;
          maskResult.red=maskResult.green=maskResult.blue=0;

          for(int mx=0;mx<rectangleWidth;mx++)
            for(int my=0;my<rectangleHeight;my++)
              {
                maskResult.red+=ConvolutionMask[mx][my]*pixels[mx+rectangleWidth*my].red;
                maskResult.green+=ConvolutionMask[mx][my]*pixels[mx+rectangleWidth*my].green;
                maskResult.blue+=ConvolutionMask[mx][my]*pixels[mx+rectangleWidth*my].blue;
              }

          maskResult.red/=ConvolutionMask.Weight;
          maskResult.green/=ConvolutionMask.Weight;
          maskResult.blue/=ConvolutionMask.Weight;
          
          OperationPerPixel(pixels,x,y,&maskResult);
        }
    }
  pixelCache.sync();
  return 0;
  /*for(int x=0;x<mask.Width;x++)
    {
      for(int y=0;y<mask.Height;y++)
        std::cout<<mask[x][y]<<" ";
      std::cout<<std::endl;
    }
    std::cout<<mask.Weight<<std::endl;*/
}
