#include<cstring>
#include<cmath>
#include<iostream>
#include<random>

class Raster{
    unsigned char data[1920][1080][3];

    public:
    // Constructor
    Raster(void);

    // setPixel function
    void setPixel(unsigned int x, unsigned int y, 
            unsigned char r, unsigned char g, unsigned char b);

    void write(void);

    void randomLines(unsigned int n);

    void drawLine(unsigned int x1, unsigned int y1, 
            unsigned int x2, unsigned int y2, 
            unsigned char r,
            unsigned char g,
            unsigned char b){
        bresenhamLine(x1, y1, x2, y2, r, g, b);
    }

    void drawNaiveLine(unsigned int x1, unsigned int y1, 
            unsigned int x2, unsigned int y2, 
            unsigned char r,
            unsigned char g,
            unsigned char b){
        naiveLine(x1, y1, x2, y2, r, g, b);
    }

    void drawDDALine(unsigned int x1, unsigned int y1, 
            unsigned int x2, unsigned int y2, 
            unsigned char r,
            unsigned char g,
            unsigned char b){
        ddaLine(x1, y1, x2, y2, r, g, b);
    }

    private:
    void ddaLine(unsigned int x1, unsigned int y1, 
            unsigned int x2, unsigned int y2, 
            unsigned char r,
            unsigned char g,
            unsigned char b);

    void bresenhamLine(unsigned int x1, unsigned int y1, 
            unsigned int x2, unsigned int y2, 
            unsigned char r,
            unsigned char g,
            unsigned char b);

    void naiveLine(unsigned int x1, unsigned int y1, 
            unsigned int x2, unsigned int y2, 
            unsigned char r,
            unsigned char g,
            unsigned char b);
};