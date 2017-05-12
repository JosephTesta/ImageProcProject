//g++ -o main main.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 

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

void edgeDetection(CImg<unsigned char>& anImage) {
	int width = anImage.width();
  	int height = anImage.height();
	CImg<unsigned char> secondImage = anImage;
	int maskSize = 2;
	int mask1[maskSize][maskSize] = {{1,0},{0,-1}};
	int mask2[maskSize][maskSize] = {{0,1},{-1,0}};
	int edgeSize = 1;
	for (int color = 0; color < 3; color++)
		for (int h=0; h<height; h++)
			for (int w = 0; w < width; w++)
				secondImage(w,h,0,color)=anImage(w,h,0,color);
	for (int color = 0; color < 3; color++)
      for (int h = 0; h < height-1; h++)
      	for (int w = 0; w < width-1; w++) {
      		int sum1=0;
			int sum2=0;
      		for (int l = 0; l < maskSize; l++)
				for (int m = 0; m < maskSize; m++){
					sum1 += anImage(w+l,h+m,0,color)*mask1[l][m];
					sum2 += anImage(w+l,h+m,0,color)*mask2[l][m];
				}
      		int value = abs(sum1) + abs(sum2);
            if ((value <= 255) && (value >= 0))
            	secondImage(w,h,0,color)= value;
            else if(value > 255)
            	secondImage(w,h,0,color) = 255;
            else
                secondImage(w,h,0,color) = 0;
        }
	anImage = secondImage;
}

int sharpen(CImg<unsigned char>& anImage,CImgDisplay& disp,bool recursive = false,int choice = 3) {
	int width = anImage.width();
  	int height = anImage.height();
	int maskSize = 5;
	int magnitude = 0;
	CImg<unsigned char> secondImage = anImage;
	int mask[maskSize][maskSize] = {{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,25,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1}};
	//int mask[maskSize][maskSize] = {{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1}};
	//int mask[maskSize][maskSize] = {{0,-1,0},{-1,5,-1},{0,-1,0}};
	int edgeSize = ((maskSize-1)/2);
	int color = 0;
	int magChange = choice;
	float mag = 0;
	while(1) {
	  	disp = secondImage;
	    switch(magChange) {
	      case 0 :
	        anImage = secondImage;
	        return 5;     
	      case 1 : 
	        if(magnitude > 0)
	          magnitude -= 1;
          else if(recursive == true){
            anImage = secondImage;
            return 1;
          }
	        break;
	      case 2 : 
	      	if(magnitude < 5)
	        	magnitude += 1;
          else if (sharpen(secondImage,disp,true,2) == 5) {
            anImage = secondImage;
            return 5;
          }
	        break;
        case 3:
        break;
	      default: 
	        std::cout << "Enter another value" << std::endl;
	        break;
		  }
  		secondImage = anImage;
  		mag = float(magnitude)/5.0;
  		int maskSum = 0;
  		for (int i = 0; i < maskSize; i++)
  			for (int j = 0; j < maskSize; j++)
  				maskSum += mask[i][j];
  		for (int color = 0; color < 3; color++) {
  			for (int h=0; h<height; h++)
  				for (int w = 0; w < width; w++){
  					secondImage(w,h,0,color)=anImage(w,h,0,color);
  				}
  			for (int h = edgeSize; h < height-edgeSize; h++){
  				for (int w = ((maskSize-1)/2); w < width - ((maskSize-1)/2); w++){
  					int sum1=0;
  					for (int l = 0; l < maskSize; l++)
  						for (int m = 0; m < maskSize; m++){
  							sum1 += anImage(w+l-edgeSize,h+m-edgeSize,0,color)*mask[l][m];
  						}	
  					int value = ((1.0-mag)*anImage(w,h,0,color)) + (mag*int(float(sum1)/maskSum));
  					//int value = int(float(sum1)/maskSum);
  					if ((value <= 255) && (value >= 0))
  						secondImage(w,h,0,color)= value;
  					else if(value > 255)
  						secondImage(w,h,0,color) = 255;
  					else
  						secondImage(w,h,0,color) = 0;
  				}
  			}
		  }
      disp = secondImage;
      std::cout << "\nPress 0 to go back"
      "\nPress 1 to decrease sharpen"
      "\nPress 2 to increase sharpen" << std::endl;
      std::cin >> magChange;
	}
}

int blurImage(CImg<unsigned char>& anImage,CImgDisplay& disp,bool recursive = false,int choice = 3) {
  int width = anImage.width();
  int height = anImage.height();
  int maskSize = 5;
  int magnitude = 0;
  CImg<unsigned char> secondImage = anImage;
  //int mask[maskSize][maskSize] = {{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,25,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1}};
  int mask[maskSize][maskSize] = {{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1}};
  //int mask[maskSize][maskSize] = {{0,-1,0},{-1,5,-1},{0,-1,0}};
  int edgeSize = ((maskSize-1)/2);
  int color = 0;
  int magChange = choice;
  float mag = 0;
  while(1) {
      disp = secondImage;
      switch(magChange) {
        case 0 :
          anImage = secondImage;
          return 5;     
        case 1 : 
          if(magnitude > 0)
            magnitude -= 1;
          else if(recursive == true){
            anImage = secondImage;
            return 1;
          }
          break;
        case 2 : 
          if(magnitude < 2)
            magnitude += 1;
          else if (blurImage(secondImage,disp,true,2) == 5) {
            anImage = secondImage;
            return 5;
          }
          break;
        case 3:
        break;
        default: 
          std::cout << "Enter another value" << std::endl;
          break;
      }
      secondImage = anImage;
      mag = float(magnitude)/2.0;
      int maskSum = 0;
      for (int i = 0; i < maskSize; i++)
        for (int j = 0; j < maskSize; j++)
          maskSum += mask[i][j];
      for (int color = 0; color < 3; color++) {
        for (int h=0; h<height; h++)
          for (int w = 0; w < width; w++){
            secondImage(w,h,0,color)=anImage(w,h,0,color);
          }
        for (int h = edgeSize; h < height-edgeSize; h++){
          for (int w = ((maskSize-1)/2); w < width - ((maskSize-1)/2); w++){
            int sum1=0;
            for (int l = 0; l < maskSize; l++)
              for (int m = 0; m < maskSize; m++){
                sum1 += anImage(w+l-edgeSize,h+m-edgeSize,0,color)*mask[l][m];
              } 
            int value = ((1.0-mag)*anImage(w,h,0,color)) + (mag*int(float(sum1)/maskSum));
            //int value = int(float(sum1)/maskSum);
            if ((value <= 255) && (value >= 0))
              secondImage(w,h,0,color)= value;
            else if(value > 255)
              secondImage(w,h,0,color) = 255;
            else
              secondImage(w,h,0,color) = 0;
          }
        }
      }
      disp = secondImage;
      std::cout << "\nPress 0 to go back"
      "\nPress 1 to decrease blur"
      "\nPress 2 to increase blur" << std::endl;
      std::cin >> magChange;
  }
}

void crop(CImg<unsigned char>& anImage,CImgDisplay& disp) {
  double currentCrop = 1.0;
  CImg<unsigned char> secondImage = anImage;
  disp = secondImage;
  int cropChoice = 0;
  int width = anImage.width();
  int height = anImage.height();
  int cropControl = 0;
  int validMouse1 = 0;
  int validMouse2 = 0;
  int mouseX1 = -1;
  int mouseY1 = -1;
  int mouseX2 = -1;
  int mouseY2 = -1;
  int cWidth;
  int cHeight;
  while(1) {
  	disp = secondImage;
    std::cout << "\nPress 0 to go back"
    "\nPress 1 to crop." << std::endl;
    std::cin >> cropChoice;
    switch(cropChoice) {
      case 0 :
        anImage = secondImage;
        return;     
      case 1 : 
      	std::cout << "\nPut your mouse on the top-left corner where you'd like to crop\n"
        "the preview image, then enter 1. Enter 0 to cancel." << std::endl;
        std::cin >> cropControl;
        if (cropControl != 1){
          return;
        }
        while(validMouse1 == 0){
          mouseX1 = disp.mouse_x();
          mouseY1 = disp.mouse_y();
          std::cout << mouseX1 << "," << mouseY1 << std::endl;
          if(mouseX1 == -1 || mouseY1 == -1){
            std::cout << "\nMouse is not within the preview image."
            "\n Place the mouse inside the preview image window where"
            "\n you would like the top-left corner of the cropped image to be."
            "\n and enter 1. Enter 0 to exit."
            << std::endl;
            std::cin >> cropControl;
            if (cropControl != 1){
              return;
            }
          }else{
            validMouse1 = 1;
          }
        }
        //display crop region
        for (int h = 0; h < height; h++) {
          for (int w = 0; w < width; w++) {
            if(w < mouseX1 || h < mouseY1){
              secondImage(w,h,0,0) = 0;
              secondImage(w,h,0,1) = 0;
              secondImage(w,h,0,2) = 0;
            }
          }
        }
        disp = secondImage;
        std::cout << "\nPut your mouse on the bottom-right corner where you'd like to crop\n"
        "the preview image, then enter 1. Enter 0 to cancel." << std::endl;
        std::cin >> cropControl;
        if (cropControl != 1){
          return;
        }
        while(validMouse2 == 0){
          mouseX2 = disp.mouse_x();
          mouseY2 = disp.mouse_y();
          std::cout << mouseX2 << "," << mouseY2 << std::endl;
          if(mouseX2 == -1 || mouseY2 == -1){
            std::cout << "\nMouse is not within the preview image."
            "\n Place the mouse inside the preview image window where"
            "\n you would like the bottom-left corner of the cropped image to be."
            "\n and enter 1. Enter 0 to exit."
            << std::endl;
            std::cin >> cropControl;
            if (cropControl != 1){
              return;
            }
          }else if (mouseX2 < mouseX1 || mouseY2 < mouseY1){
            std::cout << "\nSecond point must be to the right of and below the first."
            "\n Place the mouse inside the preview image window where"
            "\n you would like the bottom-left corner of the cropped image to be."
            "\n and enter 1. Enter 0 to exit."
            << std::endl;
            std::cin >> cropControl;
            if (cropControl != 1){
              return;
            }
          }else{
            validMouse2 = 1;
          }
        }
        //display crop region
        for (int h = 0; h < height; h++) {
          for (int w = 0; w < width; w++) {
            if(w > mouseX2|| h > mouseY2){
              secondImage(w,h,0,0) = 0;
              secondImage(w,h,0,1) = 0;
              secondImage(w,h,0,2) = 0;
            }
          }
        }
        disp = secondImage;
        std::cout << "\nIs this your intended result? 1 for yes, 0 to cancel." << std::endl;
        std::cin >> cropControl;
        if (cropControl == 0){
          return;
        }
        // Crop here.
        //disp = seconImage.get_crop(mouseX1,mouseY1,mouseX2,mouseY2);
        anImage = secondImage.get_crop(mouseX1,mouseY1,mouseX2,mouseY2);
        cWidth = mouseX2 - mouseX1;
        cHeight = mouseY2 - mouseY1;
        disp.resize(cWidth,cHeight);
        return;

      default: 
        std::cout << "Enter another value" << std::endl;
        break;
    }
  }
}

void blurOrSharpen(CImg<unsigned char>& anImage,CImgDisplay& disp) {
    std::cout << "\nPress 0 to go back"
    "\nPress 1 to blur"
    "\nPress 2 to sharpen" << std::endl;
    int choice;
    std::cin >> choice;
    switch(choice) {
      case 0:
        return;
        break;
      case 1:
        blurImage(anImage,disp);
        break;
      case 2:
        sharpen(anImage,disp);
        break;
      default:
        std::cout << "Enter another value" << std::endl;
        break;
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
    int value = 0;
    switch(brightChange) {
      case 0 :
        anImage = secondImage;
        return;     
      case 1 : 
        for (int color = 0; color < 3; color++) {
          for (int h = 0; h < height; h++)
          	for (int w = 0; w < width; w++) {
  	        	    value = secondImage(w,h,0,color)/1.1;
                  if ((value <= 255) && (value >= 0))
                    secondImage(w,h,0,color)= value;
                  else if(value > 255)
                    secondImage(w,h,0,color) = 255;
                  else
                    secondImage(w,h,0,color) = 0;
            }
  	     }
        break;
      case 2 : 
        for (int color = 0; color < 3; color++) {
          for (int h = 0; h < height; h++)
          	for (int w = 0; w < width; w++) {
  	        	 value = (secondImage(w,h,0,color)/1.1)+23;
               if ((value <= 255) && (value >= 0))
                secondImage(w,h,0,color)= value;
              else if(value > 255)
                secondImage(w,h,0,color) = 255;
              else
                secondImage(w,h,0,color) = 0;
            }
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
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
		    int average = ((int)secondImage(w,h,0,0)+(int)secondImage(w,h,0,1)+(int)secondImage(w,h,0,2))/3;
		    secondImage(w,h,0,0) = average;
		    secondImage(w,h,0,1) = average;
		    secondImage(w,h,0,2) = average;
		}
    }
    anImage = secondImage;
}

void invert(CImg<unsigned char>& anImage) {
    int width = anImage.width();
    int height = anImage.height();
    CImg<unsigned char> secondImage = anImage;
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
		        secondImage(w,h,0,0) = 255-(int)secondImage(w,h,0,0);
            secondImage(w,h,0,1) = 255-(int)secondImage(w,h,0,1);
            secondImage(w,h,0,2) = 255-(int)secondImage(w,h,0,2);
		}
    }
    anImage = secondImage;
}

void backup(std::vector<CImg<unsigned char> >& lastImage, std::vector<int>& windowSizes,
  CImg<unsigned char>& currentImage,CImgDisplay& second_disp){
  lastImage.push_back(currentImage);
  windowSizes.push_back(second_disp.window_height());
  windowSizes.push_back(second_disp.window_width());
}


int main() {
  CImg<unsigned char> baseImage("lenna.png");
  //CImg<unsigned char> lastImage("hongman.jpg");
  std::vector<CImg<unsigned char> > lastImage;
  std::vector<int> windowSizes;
  int uWidth = 0;
  int uHeight = 0;
  CImg<unsigned char> currentImage("lenna.png");
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
    "\nPress 4 to blur or sharpen"
    "\nPress 5 to overlay a color"
    "\nPress 6 to change brightness"
    "\nPress 7 to add noise"
    "\nPress 8 to crop" 
    "\nPress 9 to equalize the histogram"
    "\nPress 10 to detect edges"
    << std::endl;
    std::cin >> choice;
    switch(choice) {
    	case 0 : 
        	std::cout << "\nImage saved as editedImage.jpg\nGoodbye\n";
          currentImage.save("editedImage.jpg");
        	return 0;       
      	case 1 : 
      		if(lastImage.empty() == false){
            uWidth = windowSizes.back();
            windowSizes.pop_back();
            uHeight = windowSizes.back();
            windowSizes.pop_back();
            second_disp.resize(uWidth,uHeight);
	      		currentImage = lastImage.back(); 
	      		lastImage.pop_back();
	      	}
	      	else {
	      		std::cout << "Nothing to undo" << std::endl;
	      	}
        break;
        case 2 : 
        	backup(lastImage,windowSizes,currentImage,second_disp);
        	grayScaleAverage(currentImage);
        break;
        case 3 :
        	backup(lastImage,windowSizes,currentImage,second_disp);
        	invert(currentImage);
        break;
        case 4:
      		backup(lastImage,windowSizes,currentImage,second_disp);
        	blurOrSharpen(currentImage,second_disp);
        break;
        case 5:
      		backup(lastImage,windowSizes,currentImage,second_disp);
        	colorOverlay(currentImage);
        break;
        case 6:
      		backup(lastImage,windowSizes,currentImage,second_disp);
        	brightDark(currentImage,second_disp);
        break;
        case 7:
          backup(lastImage,windowSizes,currentImage,second_disp);
          noise(currentImage,second_disp);
        break;
        case 8:
          backup(lastImage,windowSizes,currentImage,second_disp);
        	crop(currentImage,second_disp);
        break;
        case 9:
        	backup(lastImage,windowSizes,currentImage,second_disp);
        	histogramEqualization(currentImage);
        break;
        case 10:
        	backup(lastImage,windowSizes,currentImage,second_disp);
        	edgeDetection(currentImage);
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
