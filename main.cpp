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

g++ -o hello_world main.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 
#endif


#include "CImg.h"
#include <iostream>
#include <string>
#include <stdio.h>
using namespace cimg_library;

int blurImage(CImg<unsigned char> anImage,CImgDisplay& main_disp) {
  double currentBlur = 1;
  CImg<unsigned char> secondImage = anImage;
  secondImage.blur(currentBlur);
  main_disp = secondImage;
  int blurChange = 0;
  while(1) {
    main_disp = secondImage;
    std::cout << "press 1 to decrease blur, 2 to increase blur and 0 to go back" << std::endl;
    std::cin >> blurChange;
    switch(blurChange) {
      case 0 :
        return 0;       
      case 1 : 
        if(currentBlur >= 0.5)
          currentBlur -= 0.5;
        secondImage = anImage;
        secondImage.blur(currentBlur);
        main_disp = secondImage;
        break;
      case 2 : 
        currentBlur += 0.5;
        secondImage = anImage;
        secondImage.blur(currentBlur);
        main_disp = secondImage;
        break;

      default: 
        std::cout << "Enter another value" << std::endl;
        break;
    }
  }

}


int main() {
  CImg<unsigned char> image("hongman.jpg");
  //const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };
  //image.blur(2.5);
  //blurImage(image);
  CImgDisplay main_disp(image);
  int choice = 0;
  while(1) {
    main_disp = image;
    std::cout << "Press 0 to exit\nPress 1 to blur" << std::endl;
    std::cin >> choice;
    switch(choice) {
      case 0 : 
        std::cout << "Goodbye";
        return 0;       
      case 1 : 
        std::cout << '1';
        blurImage(image,main_disp);
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