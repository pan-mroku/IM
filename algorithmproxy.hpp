#ifndef ALGORITHMPROXY_HPP
#define ALGORITHMPROXY_HPP

#include "algorithms.hpp"

class AlgorithmProxy
{
public:
  AlgorithmI I;
  AlgorithmBW BW;
  AlgorithmShiftNoise ShiftNoise;
  AlgorithmGaussNoise GaussNoise;
  AlgorithmTest Test;
  AlgorithmRead Read;
  AlgorithmBlur Blur;
  AlgorithmHough Hough;
  HoughTester HoughTest;
};

#endif
