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

class AlgorithmBW:public SimpleAlgorithm
{
public:
  virtual void OperationPerPixel(Magick::PixelPacket* pixel);
};

class AlgorithmShiftNoise:public BaseAlgorithm
{
public:
  int Percent;

  AlgorithmShiftNoise(int percent=10):
    Percent(percent)
  {}

  virtual int DoYourJob(Magick::Image& image);
};

class AlgorithmGaussNoise:public SimpleAlgorithm
{
  virtual void OperationPerPixel(Magick::PixelPacket* pixel);

public:
  AlgorithmGaussNoise(double s=30000);

  double Sigma() const;
  double Sigma(double s);
protected:
  double sigma;
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

class HoughResult
{
public:
  HoughResult(int fi=0, int r=0, int value=0):
    Fi(fi),
    R(r),
    Value(value)
  {}
  
  int Fi;
  int R;
  int Value;
};

class HoughAccumulator:public DetailedAlgorithm
{
public:
  Magick::Image Accumulator;
  Magick::PixelPacket* AccumulatorPixels;
  int R;

  virtual int DoYourJob(Magick::Image& image);
  virtual void OperationPerPixel(Magick::PixelPacket* pixel, unsigned int x, unsigned int y);

  HoughResult Maximum(); 
};

class AlgorithmHough:public BaseAlgorithm
{
public:
  AlgorithmBW BW;
  HoughAccumulator Accu;
  AlgorithmBlur Blur;

  virtual int DoYourJob(Magick::Image& image);
};

class HoughTester:public BaseAlgorithm
{
public:
  int NumberOfNoiseLevels;
  int NumberOfTriesPerLevel;

  HoughTester(int numberOfNoiseLevels=10, int numberOfTriesPerLevel=10):
    NumberOfNoiseLevels(numberOfNoiseLevels),
    NumberOfTriesPerLevel(numberOfTriesPerLevel)
  {}

  virtual int DoYourJob(Magick::Image& image);
protected:
  AlgorithmShiftNoise noise;
  AlgorithmBW bw;
  HoughAccumulator accumulator;
  AlgorithmBlur blur;
};

#endif
