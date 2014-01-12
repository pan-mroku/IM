#include "algorithmproxy.hpp"
#include <Magick++.h> 
#include <iostream> 

enum ProgramFunction
  {
    UNKNOWN=0,
    GAUSS,
    ILLUMINANCE,
    HOUGH,
    TEST,
    BLUR,
  };

void info()
{
  std::cout<<"Odporne metody analizy obrazów"<<std::endl<<"Maciej Szewczyk, Paweł Szymański 2013"<<std::endl;
  std::cout<<"Użycie: IM opcja plik_wejściowy plik_wyjściowy"<<std::endl;
  std::cout<<"Opcje:\n\
\t-b, --blur\t\t Rozmycie\n\
\t-g, --gauss\t\t Szum Gaussa\n\
\t-h, --hough\t\t Detektor Hougha\n\
\t-j, --jasność\t\t Jasność obrazu\n\
\t-t, --test\t\t algorytm testowy\n\
\t-q\t\t\t Tryb cichy\n\
\t - \t\t\t Nie zapisuje efektów na dysk"<<std::endl;
}

int main(int argc,char **argv) 
{ 

  std::string input="",output="out.png";
  char whatShallIDo=UNKNOWN;
  bool silenceMyDear=false, writeToFile=true;

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

      else if(arg=="-b" || arg=="--blur")
          whatShallIDo=BLUR;

      else if(arg=="-g" || arg=="--gauss")
          whatShallIDo=GAUSS;

      else if(arg=="-h" || arg=="--hough")
        whatShallIDo=HOUGH;

      else if(arg=="-j" || arg=="--jasność")
        whatShallIDo=ILLUMINANCE;

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
  AlgorithmProxy algorithms;

  try { 

    image.read( input );

    switch(whatShallIDo)
      {

      case BLUR:
        {
          algorithms.Blur(image);
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

      case TEST:
        {
          /*algorithms.Test(image);
            algorithms.Read(image);*/
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
