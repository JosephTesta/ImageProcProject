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
using namespace cimg_library;

int blurImage(CImg<unsigned char>& anImage,CImgDisplay& disp) {
  double currentBlur = 1;
  CImg<unsigned char> secondImage = anImage;
  secondImage.blur(currentBlur);
  disp = secondImage;
  int blurChange = 0;
  while(1) {
  	disp = secondImage;
    std::cout << "press 1 to decrease blur, 2 to increase blur and 0 to go back" << std::endl;
    std::cin >> blurChange;
    switch(blurChange) {
      case 0 :
        anImage = secondImage;
        return 0;     
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

void grayScaleAverage(CImg<unsigned char>& anImage,CImgDisplay& disp) {
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

void invert(CImg<unsigned char>& anImage,CImgDisplay& disp) {
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
  CImg<unsigned char> lastImage("hongman.jpg");
  CImg<unsigned char> currentImage("hongman.jpg");
  //const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };
  //image.blur(2.5);
  //blurImage(image);
  CImgDisplay main_disp(baseImage);
  CImgDisplay second_disp(currentImage);
  int choice = 0;
  while(1) {
    second_disp = currentImage;
    std::cout << "Press 0 to exit\nPress 1 to blur\nPress 2 to convert to grayscale\nPress 3 to invert(negative)" << std::endl;
    std::cin >> choice;
    switch(choice) {
    	case 0 : 
        	std::cout << "Goodbye";
        	return 0;       
      	case 1 : 
        	std::cout << '1';
        	blurImage(currentImage,second_disp);
        break;
        case 2 : 
        	std::cout << '2';
        	grayScaleAverage(currentImage,second_disp);
        break;
        case 3 : 
        	std::cout << '3';
        	invert(currentImage,second_disp);
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