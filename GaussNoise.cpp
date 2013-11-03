#include <stdlib.h>
#include <math.h>

#include "GaussNoise.h"

double DblRan(void) {
  double out = (float)rand() / RAND_MAX;
  return out;

} // DblRan()



double GenerateGaussNoiseNorm(void) {
	static int iset=0;
	static double gset;
	double fac,r,v1,v2;

	if  (iset == 0) {
		do {
			v1=2.0*DblRan()-1.0;
			v2=2.0*DblRan()-1.0;
			r=v1*v1+v2*v2;
		} while (r >= 1.0);
		fac=sqrt(-2.0*log(r)/r);
		gset=v1*fac;
		iset=1;
		return v2*fac;
	} else {
		iset=0;
		return gset;
	}
} // GenerateGaussNoiseNorm



double GenerateGaussNoise(double sigma,double m) {
  return GenerateGaussNoiseNorm() * sigma + m;
} // GenerateGaussNoise



/*void GaussNoiseImage(Tpic *Ppic,double sigma) {
  int x,y;

  for (x = 0; x < _Zx(Ppic); x++)
  for (y = 0; y < _Zy(Ppic); y++) {

    double noi = GenerateGaussNoise(sigma);
    long int gre = (long int)GRE(Ppic,x,y);
    gre += round(noi);
    if (gre < 0)
      gre = 0;
    if (gre > Xbrh)
      gre = Xbrh;
    SetGRE(Ppic,x,y,(Tbrh)gre);

  } // for (x,y)
}
*/
