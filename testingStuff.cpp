//g++ -o testing testingStuff.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
#include <iostream>
using namespace std;

#include <CImg.h>
using namespace cimg_library;

int main()
{
    CImg<unsigned char> src("hongman.jpg");
    CImgDisplay main_disp(src);
    CImgDisplay second_disp(src);
    int width = src.width();
    int height = src.height();
    cout << width << "x" << height << endl;
    for (int r = 0; r < height; r++)
        for (int c = 0; c < width; c++) {
            cout << "(" << r << "," << c << ") ="
                 << " R" << (int)src(c,r,0,0)
                 << " G" << (int)src(c,r,0,1)
                 << " B" << (int)src(c,r,0,2) << endl;

                 //make purple by changing R and B values to max
                 #if 0
                 src(c,r,0,0) = uint(255);
                 src(c,r,0,2) = uint(255);
                 #endif

                //brighten
                 #if 0
                int num = 75;
                if((int)src(c,r,0,0)+num < 255)
                    src(c,r,0,0) = (int)src(c,r,0,0)+num;
                else
                    src(c,r,0,0) = 255;

                if((int)src(c,r,0,1)+num < 255)
                    src(c,r,0,1) = (int)src(c,r,0,1)+num;
                else
                    src(c,r,0,1) = 255;

                if((int)src(c,r,0,2)+num < 255)
                    src(c,r,0,2) = (int)src(c,r,0,2)+num;
                else
                    src(c,r,0,2) = 255;
                #endif


                //darken
                 #if 0
                 src(c,r,0,0) = src(c,r,0,0)/2;
                 src(c,r,0,1) = src(c,r,0,1)/2;
                 src(c,r,0,2) = src(c,r,0,2)/2;
                #endif


                //invert/ negative
                 #if 0
                src(c,r,0,0) = 255-(int)src(c,r,0,0);
                src(c,r,0,1) = 255-(int)src(c,r,0,1);
                src(c,r,0,2) = 255-(int)src(c,r,0,2);
                #endif


                //grayscale
                //https://www.johndcook.com/blog/2009/08/24/algorithms-convert-color-grayscale/
                #if 1
                int average = ((int)src(c,r,0,0)+(int)src(c,r,0,1)+(int)src(c,r,0,2))/3;
                src(c,r,0,0) = average;
                src(c,r,0,1) = average;
                src(c,r,0,2) = average;
                #endif


                //crop
                //try setting all RGB values after a certain row and column equal to 0. or changing width and length if possible

                //noise?

        }
    second_disp = src;
    while (!main_disp.is_closed() && !second_disp.is_closed()) {
        main_disp.wait();
    }
    return 0;
}