#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <cstring>
#include <cmath>
#include <random>

using namespace std;

class Raster{
    unsigned char data[1920][1080][3];
    double xIni, yIni, xEnd, yEnd; // Bresenham vars
    double slope = 0; // Slope
    int evaluatedSlope; // self-explained

    public:
    // Constructor
    Raster(void);

    // setPixel function
    void setPixel(unsigned int x, unsigned int y, 
            unsigned char r, unsigned char g, unsigned char b);

    void write(char* arg);

    void randomLines(unsigned int n);

    void clear();

    void drawLine(unsigned int x1, unsigned int y1, 
            unsigned int x2, unsigned int y2, 
            unsigned char r, unsigned char g, unsigned char b){
        ddaLine(x1, y1, x2, y2, r, g, b);
    }

    void drawNaiveLine(unsigned int x1, unsigned int y1, 
            unsigned int x2, unsigned int y2, 
            unsigned char r, unsigned char g, unsigned char b){
        naiveLine(x1, y1, x2, y2, r, g, b);
    }

    void drawDDALine(unsigned int x1, unsigned int y1, 
            unsigned int x2, unsigned int y2, 
            unsigned char r, unsigned char g, unsigned char b){
        ddaLine(x1, y1, x2, y2, r, g, b);
    }

    private:
    void ddaLine(unsigned int x1, unsigned int y1, 
            unsigned int x2, unsigned int y2, 
            unsigned char r, unsigned char g, unsigned char b);

    void naiveLine(unsigned int x1, unsigned int y1, 
            unsigned int x2, unsigned int y2, 
            unsigned char r, unsigned char g, unsigned char b);

    /* Bresenham Functions */
    void bresenhamLine(unsigned int x1, unsigned int y1, 
            unsigned int x2, unsigned int y2, 
            unsigned char r, unsigned char g, unsigned char b);

    int bresenhamSlope(double slope);

    void bresenhamDraw( int x0, int y0, int x1, int y1, 
            unsigned char r, unsigned char g, unsigned char b,
            int slope);
};