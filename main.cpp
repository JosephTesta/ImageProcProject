#if 0
#include "CImg.h"
using namespace cimg_library;
int main() {
  CImg<unsigned char> image("hongman.jpg"), visu(500,400,1,3,0);
  const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };
  //image.blur(2.5);
  CImgDisplay main_disp(image,"Click a point"), draw_disp(visu,"Intensity profile");
  while (!main_disp.is_closed() && !draw_disp.is_closed()) {
    main_disp.wait();
    if (main_disp.button() && main_disp.mouse_y()>=0) {
      const int y = main_disp.mouse_y();
      visu.fill(0).draw_graph(image.get_crop(0,y,0,0,image.width()-1,y,0,0),red,1,1,0,255,0);
      visu.draw_graph(image.get_crop(0,y,0,1,image.width()-1,y,0,1),green,1,1,0,255,0);
      visu.draw_graph(image.get_crop(0,y,0,2,image.width()-1,y,0,2),blue,1,1,0,255,0).display(draw_disp);
      }
    }
  return 0;
}

g++ -o main main.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 
#endif



#include "CImg.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
using namespace cimg_library;

void histogramEqualization(CImg<unsigned char>& anImage) {
  	CImg<unsigned char> secondImage = anImage;
  	int width = anImage.width();
  	int height = anImage.height();
  	int pixelArray[256][3] = {0};
  	for (int color = 0;color < 3; color++) {
		for (int h=0; h<height; h++) {
			for (int w=0; w<width; w++) {
				pixelArray[anImage(w,h,0,color)][color]++;
			}
		}
	}

	//create mapping
	int mapping[256][3] = {0};
	int currentSum = 0;
	for (int color = 0; color < 3; color++) {
		currentSum = 0;
		for (int i=0; i<256; i++) {
	    	currentSum += pixelArray[i][color];
			mapping[i][color] = round((256-1)*float(currentSum)/(height*width));
		}
	}

	//change output
	for (int color = 0; color < 3; color++) {
		for (int h=0; h<height; h++) {
			for (int w=0; w<width; w++) {
		    	secondImage(w,h,0,color) = mapping[anImage(w,h,0,color)][color];
		    	//secondImage(w,h,0,color) = anImage(w,h,0,color) +100;

			}
		}
	}

	anImage = secondImage;
}

void blurImage(CImg<unsigned char>& anImage,CImgDisplay& disp) {
  double currentBlur = 1;
  CImg<unsigned char> secondImage = anImage;
  secondImage.blur(currentBlur);
  disp = secondImage;
  int blurChange = 0;
  while(1) {
  	disp = secondImage;
    std::cout << "\nPress 0 to go back"
    "\nPress 1 to decrease blur"
    "\nPress 2 to increase blur" << std::endl;
    std::cin >> blurChange;
    switch(blurChange) {
      case 0 :
        anImage = secondImage;
        return;     
      case 1 : 
        if(currentBlur >= 0.5)
          currentBlur -= 0.5;
        secondImage = anImage;
        secondImage.blur(currentBlur);
        break;
      case 2 : 
        currentBlur += 0.5;
        secondImage = anImage;
        secondImage.blur(currentBlur);
        break;

      default: 
        std::cout << "Enter another value" << std::endl;
        break;
    }
  }
}

void crop(CImg<unsigned char>& anImage,CImgDisplay& disp) {
  double currentCrop = 1.0;
  CImg<unsigned char> secondImage = anImage;
  disp = secondImage;
  int cropChoice = 0;
  int width = anImage.width();
  int height = anImage.height();
  while(1) {
  	disp = secondImage;
    std::cout << "\nPress 0 to go back"
    "\nPress 1 to crop less"
    "\nPress 2 to crop more" << std::endl;
    std::cin >> cropChoice;
    switch(cropChoice) {
      case 0 :
        anImage = secondImage;
        return;     
      case 1 : 
      	if(currentCrop < 1) {
        	currentCrop += .1;
        	secondImage = anImage.get_crop(0,0,currentCrop*width,currentCrop*height);
    	}
        break;
      case 2 : 
      	if(currentCrop > .11) {
      		currentCrop -= .1;
        	secondImage = anImage.get_crop(0,0,currentCrop*width,currentCrop*height);
        }
        break;

      default: 
        std::cout << "Enter another value" << std::endl;
        break;
    }
  }
}

void brightDark(CImg<unsigned char>& anImage,CImgDisplay& disp) {
  CImg<unsigned char> secondImage = anImage;
  disp = secondImage;
  int brightChange = 0;
  int width = anImage.width();
  int height = anImage.height();
  while(1) {
  	disp = secondImage;
    std::cout << "\nPress 0 to go back"
    "\nPress 1 to darken"
    "\nPress 2 to brighten" << std::endl;
    std::cin >> brightChange;
    switch(brightChange) {
      case 0 :
        anImage = secondImage;
        return;     
      case 1 : 
        for (int h = 0; h < height; h++)
        	for (int w = 0; w < width; w++) {
	        	 secondImage(w,h,0,0) = secondImage(w,h,0,0)/2;
                 secondImage(w,h,0,1) = secondImage(w,h,0,1)/2;
                 secondImage(w,h,0,2) = secondImage(w,h,0,2)/2;
	         }
        break;
      case 2 : 
        for (int h = 0; h < height; h++)
        	for (int w = 0; w < width; w++) {
	        	 secondImage(w,h,0,0) = (secondImage(w,h,0,0)/2)+126;
                 secondImage(w,h,0,1) = (secondImage(w,h,0,1)/2)+126;
                 secondImage(w,h,0,2) = (secondImage(w,h,0,2)/2)+126;
	         }
        break;

      default: 
        std::cout << "Enter another value" << std::endl;
        break;
    }
  }
}


void noise(CImg<unsigned char>& anImage, CImgDisplay& disp) {
  int amountSigma = 0;
  int noiseChoice = 0;
  int noiseToggle = 0;
  while(1) {
    std::cout << "\nWhat kind of noise?" 
    "\nPress 0 to go back"
    "\nPress 1 for Gaussian"
    "\nPress 2 for Uniform"
    "\nPress 3 for Salt and Pepper"
    "\nPress 4 for Poisson"
    "\nPress 5 for Rician" << std::endl;
    std::cin >> noiseChoice;

    CImg<unsigned char> secondImage = anImage;
    //secondImage.get_noise(amountSigma, noiseChoice);
    disp = secondImage;
    switch(noiseChoice) {
      case 0:
        return;
      case 4: 
        while(1) {
          disp = secondImage;

          std::cout << "\nPress 0 to go back"
          "\nPress 1 to increase noise" << std::endl;
          std::cin >> noiseToggle;

          switch(noiseToggle) {
            case 0:
              anImage = secondImage;
              return;
            case 1: 
              secondImage = anImage.get_noise(amountSigma, noiseChoice-1);
              anImage = secondImage;
              break;

            default: 
              std::cout << "Enter another value" << std::endl;
              break;
          }
        }
        break;

      default: 
        if (noiseChoice == 1 || noiseChoice == 2 || noiseChoice == 3 || noiseChoice == 5) {
          while(1) {
            disp = secondImage;

            std::cout << "\nPress 0 to go back"
            "\nPress 1 to decrease noise"
            "\nPress 2 to increase noise" << std::endl;
            std::cin >> noiseToggle;

            switch(noiseToggle) {
              case 0:
                anImage = secondImage;
                return;
              case 1: 
                if(amountSigma >= 10) {
                  amountSigma -= 10;
                }
                //secondImage = anImage;
                secondImage = anImage.get_noise(amountSigma, noiseChoice-1);
                break;
              case 2: 
                amountSigma += 10;
                //secondImage = anImage;
                secondImage = anImage.get_noise(amountSigma, noiseChoice-1);
                break;

              default: 
                std::cout << "Enter another value" << std::endl;
                break;
            }
          }
        }
        else {
          std::cout << "Enter another value" << std::endl;
        }
        break;
    }
  }

}


void colorOverlay(CImg<unsigned char>& anImage) {
  int colorChoice = 0;
    std::cout << "\nChoose a color to overlay"
    "\nPress 0 to go back"
    "\nPress 1 for red"
    "\nPress 2 for green"
    "\nPress 3 for blue"
    "\nPress 4 for yellow"
    "\nPress 5 for magenta"
    "\nPress 6 for cyan" << std::endl;
    std::cin >> colorChoice;
    int width = anImage.width();
    int height = anImage.height();
    switch(colorChoice) {
      case 0 :
        return;    
      case 1 :
    	for (int h = 0; h < height; h++)
        	for (int w = 0; w < width; w++) {
	        	 //make red by changing R values to max
	             anImage(w,h,0,0) = uint(255);
	         }
        break;
      case 2 : 
    	for (int h = 0; h < height; h++)
        	for (int w = 0; w < width; w++) {
	        	 //make green by changing G values to max
	             anImage(w,h,0,1) = uint(255);
	         }
        break;
       case 3 : 
    	for (int h = 0; h < height; h++)
        	for (int w = 0; w < width; w++) {
	        	 //make blue by changing B values to max
	             anImage(w,h,0,2) = uint(255);
	         }
        break;
        case 4 : 
    	for (int h = 0; h < height; h++)
        	for (int w = 0; w < width; w++) {
	        	 //make yellow by changing R and G values to max
	             anImage(w,h,0,0) = uint(255);
	             anImage(w,h,0,1) = uint(255);
	         }
        break;
        case 5 : 
    	for (int h = 0; h < height; h++)
        	for (int w = 0; w < width; w++) {
	        	 //make magenta by changing R and B values to max
	             anImage(w,h,0,0) = uint(255);
	             anImage(w,h,0,2) = uint(255);
	         }
        break;
        case 6 : 
    	for (int h = 0; h < height; h++)
        	for (int w = 0; w < width; w++) {
	        	 //make cyan by changing G and B values to max
	             anImage(w,h,0,1) = uint(255);
	             anImage(w,h,0,2) = uint(255);
	         }
        break;

      default: 
        std::cout << "Enter another value" << std::endl;
        break;
    }
  }

void grayScaleAverage(CImg<unsigned char>& anImage) {
    int width = anImage.width();
    int height = anImage.height();
    CImg<unsigned char> secondImage = anImage;
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
		    int average = ((int)secondImage(c,r,0,0)+(int)secondImage(c,r,0,1)+(int)secondImage(c,r,0,2))/3;
		    secondImage(c,r,0,0) = average;
		    secondImage(c,r,0,1) = average;
		    secondImage(c,r,0,2) = average;
		}
    }
    anImage = secondImage;
}

void invert(CImg<unsigned char>& anImage) {
    int width = anImage.width();
    int height = anImage.height();
    CImg<unsigned char> secondImage = anImage;
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
		        secondImage(c,r,0,0) = 255-(int)secondImage(c,r,0,0);
            secondImage(c,r,0,1) = 255-(int)secondImage(c,r,0,1);
            secondImage(c,r,0,2) = 255-(int)secondImage(c,r,0,2);
		}
    }
    anImage = secondImage;
}


int main() {
  CImg<unsigned char> baseImage("hongman.jpg");
  //CImg<unsigned char> lastImage("hongman.jpg");
  std::vector<CImg<unsigned char> > lastImage;
  CImg<unsigned char> currentImage("hongman.jpg");
  //const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };
  //image.blur(2.5);
  //blurImage(image);
  CImgDisplay main_disp(baseImage,"Original Image");
  CImgDisplay second_disp(currentImage,"Edited Image");
  int choice = 0;

  while(!main_disp.is_closed() && !second_disp.is_closed()) {
    second_disp = currentImage;
    std::cout << "\nPress 0 to save and exit"
    "\nPress 1 to undo"
    "\nPress 2 to convert to grayscale"
    "\nPress 3 to invert(negative)"
    "\nPress 4 to blur"
    "\nPress 5 to overlay a color"
    "\nPress 6 to change brightness"
    "\nPress 7 to add noise"
    "\nPress 8 to crop" 
    "\nPress 9 to equalize histogram"
    << std::endl;
    std::cin >> choice;
    switch(choice) {
    	case 0 : 
        	std::cout << "\nImage saved as editedImage.jpg\nGoodbye\n";
          currentImage.save("editedImage.jpg");
        	return 0;       
      	case 1 : 
      		if(lastImage.empty() == false){
	      		currentImage = lastImage.back(); 
	      		lastImage.pop_back();
	      	}
	      	else {
	      		std::cout << "Nothing to undo" << std::endl;
	      	}
        break;
        case 2 : 
        	lastImage.push_back(currentImage);
        	grayScaleAverage(currentImage);
        break;
        case 3 :
        	lastImage.push_back(currentImage);
        	invert(currentImage);
        break;
        case 4:
      		lastImage.push_back(currentImage);
        	blurImage(currentImage,second_disp);
        break;
        case 5:
      		lastImage.push_back(currentImage);
        	colorOverlay(currentImage);
        break;
        case 6:
      		lastImage.push_back(currentImage);
        	brightDark(currentImage,second_disp);
        break;
        case 7:
          lastImage.push_back(currentImage);
          noise(currentImage,second_disp);
        break;
        case 8:
      		lastImage.push_back(currentImage);
        	crop(currentImage,second_disp);
        break;
        case 9:
        	lastImage.push_back(currentImage);
        	histogramEqualization(currentImage);
        break;
      	default: 
        	std::cout << "Enter another value" << std::endl;
        break;
    }
  }

  #if 0
  while (!main_disp.is_closed()) {
    main_disp.wait();
  }
  #endif
  
  return 0;
}