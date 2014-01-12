#ifndef ALGORITHMPROXY_HPP
#define ALGORITHMPROXY_HPP

#include "algorithms.hpp"

class AlgorithmProxy
{
public:
  AlgorithmI I;
  AlgorithmGaussNoise GaussNoise;
  AlgorithmTest Test;
  AlgorithmRead Read;
  AlgorithmBlur Blur;
  AlgorithmHough Hough;
};

#endif
