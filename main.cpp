#include "algorithmproxy.hpp"
#include <Magick++.h> 
#include <iostream> 
#include <string>

enum ProgramFunction
  {
    UNKNOWN=0,
    ANALYSE,
    SHIFT,
    GAUSS,
    ILLUMINANCE,
    BLACKANDWHITE,
    HOUGH,
    TEST,
    BLUR,
  };

void info()
{
  std::cout<<"Odporne metody analizy obrazów"<<std::endl<<"Maciej Szewczyk, Paweł Szymański 2013"<<std::endl;
  std::cout<<"Użycie: IM opcja plik_wejściowy plik_wyjściowy"<<std::endl;
  std::cout<<"Opcje:\n\
\t-a, --analiza [ilość poziomów szumu] [ilość prób na poziom szumu]\t Analiza odporności algorytmu Hougha\n\
\t-b, --blur\t\t\t\t\t\t\t\t Rozmycie\n\
\t-g, --gauss [s:0-100]\t\t\t\t\t\t\t Szum Gaussa\n\
\t-h, --hough\t\t\t\t\t\t\t\t Detektor Hougha\n\
\t-j, --jasność\t\t\t\t\t\t\t\t Jasność obrazu\n\
\t-m, --monochrome\t\t\t\t\t\t\t Czerń i biel\n\
\t-s, --shift [s:0-100]\t\t\t\t\t\t\t Szum przesunięć\n\
\t-t, --test\t\t\t\t\t\t\t\t algorytm testowy\n\
\t-q\t\t\t\t\t\t\t\t\t Tryb cichy\n\
\t - \t\t\t\t\t\t\t\t\t Nie zapisuje efektów na dysk"<<std::endl;
}

int main(int argc,char **argv) 
{ 

  std::string input="",output="out.png";
  char whatShallIDo=UNKNOWN;
  bool silenceMyDear=false, writeToFile=true;
  AlgorithmProxy algorithms;
  if(argc<3)
    {
      info();
      return 1;
    }

  for(int i=1;i<argc;i++)
    {
      std::string arg=argv[i];//bez rzutowania if nie dzia, więc niech już będzie ładnie.

      if(argv[i][0]!='-')
        {
          if(input=="")
            input=argv[i];
          else
            output=argv[i];
          continue;
        }

      else if(arg=="-a" || arg=="--analiza")
        {
          if(argv[i+1][0]>'0' && argv[i+1][0]<='9')
            algorithms.HoughTest.NumberOfNoiseLevels=std::stoi(argv[++i]);

          if(argv[i+1][0]>'0' && argv[i+1][0]<='9')
            algorithms.HoughTest.NumberOfTriesPerLevel=std::stoi(argv[++i]);

          whatShallIDo=ANALYSE;
          silenceMyDear=true;
          writeToFile=false;
        }

      else if(arg=="-b" || arg=="--blur")
          whatShallIDo=BLUR;

      else if(arg=="-s" || arg=="--shift")
        {
          if(argv[i+1][0]>='0' && argv[i+1][0]<='9')
            algorithms.ShiftNoise.Percent=std::stod(argv[++i]);

          whatShallIDo=SHIFT;
          
        }

      else if(arg=="-g" || arg=="--gauss")
        {
          if(argv[i+1][0]>='0' && argv[i+1][0]<='9')
            algorithms.GaussNoise.Sigma(std::stod(argv[++i]));

          whatShallIDo=GAUSS;
          
        }

      else if(arg=="-h" || arg=="--hough")
        whatShallIDo=HOUGH;

      else if(arg=="-j" || arg=="--jasność")
        whatShallIDo=ILLUMINANCE;

      else if(arg=="-m" || arg=="--monochrome")
        whatShallIDo=BLACKANDWHITE;

      else if(arg=="-t" || arg=="--test")
        whatShallIDo=TEST;

      else if(arg=="-q")
        silenceMyDear=true;

      else if(arg=="-")
        writeToFile=false;
    }

  if(whatShallIDo==UNKNOWN || input=="")
    {
      info();
      return 1;
    }

  Magick::InitializeMagick(*argv);

  // Construct the image object. Seperating image construction from the 
  // the read operation ensures that a failure to read the image file 
  // doesn't render the image object useless. 
  Magick::Image image;


  try { 

    image.read( input );

    switch(whatShallIDo)
      {

      case ANALYSE:
        {
          algorithms.HoughTest(image);
          break;
        }

      case BLUR:
        {
          algorithms.Blur(image);
          break;
        }

      case SHIFT:
        {
          algorithms.ShiftNoise(image);
          break;
        }

      case GAUSS:
        {
          algorithms.GaussNoise(image);
          break;
        }

      case HOUGH:
        {
          algorithms.Hough(image);
          break;
        }

      case ILLUMINANCE:
        {
          algorithms.I(image);
          break;
        }

      case BLACKANDWHITE:
        {
          algorithms.BW(image);
          break;
        }

      case TEST:
        {
          /*algorithms.Test(image);
            algorithms.Read(image);*/
          algorithms.Read(image);
          break;
        }

      }

    if(!silenceMyDear)
      {
        //IMPORTANT: the image may be altered when it is displayed via 'Image::display()'; in order to prevent such situations always use a copy of the image intended for visualization!
        Magick::Image copy(image);
        copy.display();
      }
    
    // Write the image to a file 
    if(writeToFile)
      image.write( output ); 
  } 
  catch( Magick::Exception &error_ ) 
    { 
      std::cout << "Caught exception: " << error_.what() << std::endl; 
      return 1; 
    } 
  return 0; 
}
