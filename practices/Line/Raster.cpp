#include "Raster.h"

Raster::Raster(void){
    memset(data, 0x0, 1920*1080*3);
}

void Raster::setPixel(unsigned int x, unsigned int y, 
        unsigned char r, unsigned char g, unsigned char b) {
    // here
    if( x >= 1920 || y >= 1080) {
        return;
    }
    data[x][y][0] = r;
    data[x][y][1] = g;
    data[x][y][2] = b;
}

void Raster::write(void){
    int x = 0;
    int y = 0;

    FILE *fp;

    fp = fopen("result.ppm", "wb");
    fprintf(fp, "P6\n1920 1080 255\n");
    for(y = 0; y < 1080; y++) {
        for(x = 0;x < 1920; x++) {
            fwrite(data[x][y], sizeof(unsigned char), 3, fp);
        } 
    }
    fclose(fp);
}

void Raster::randomLines(unsigned int n){
    unsigned int x1, x2, y1, y2;
    unsigned char r, g, b;
    std::default_random_engine engine;
    std::uniform_int_distribution<unsigned int> distX(0,1919);
    std::uniform_int_distribution<unsigned int> distY(0,1079);
    std::uniform_int_distribution<unsigned char> distC(0x00,0xff);

    for(int i = 0; i < n; i++) {
        x1 = distX(engine);
        x2 = distX(engine);
        y1 = distY(engine);
        y2 = distY(engine);
        r = distC(engine);
        g = distC(engine);
        b = distC(engine);
        naiveLine(x1, y1, x2, y2, r, g, b);
    }
}

void Raster::ddaLine(unsigned int x1, unsigned int y1, 
        unsigned int x2, unsigned int y2, 
        unsigned char r,
        unsigned char g,
        unsigned char b){
    // here
    int dx = x2 - x1;
    int dy = y2 - y1;
    // cursor
    float x = 0, y = 0;        

    // calculate steps required for generating pixels 
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy); 

    // calculate increment in x & y for each steps 
    float xi = dx / (float) steps; 
    float yi = dy / (float) steps; 

    // Put pixel for each step 
    x = x1;
    y = y1;
    for (int i = 0; i <= steps; i++) { 
        setPixel(x, y, r, g, b); 
        x += xi; 
        y += yi;
    } 
}

void Raster::bresenhamLine(unsigned int x1, unsigned int y1, 
        unsigned int x2, unsigned int y2, 
        unsigned char r,
        unsigned char g,
        unsigned char b){
    // here
    int dx = x2 - x1;
    int dy = y2 - y1;
    // cursor
    unsigned int x = 0, y = 0;        

    // Vertical and horizontal lines
    if(dx == 0){
        // draw vertical
        dy = abs(dy);
        y = (y2 > y1) ? y2 : y1;
        while(dy > 0) {
            setPixel(x1, y--, r, g, b);
            --dy;
        }
    }else if(dy == 0){
        // draw horizontal
        dx = abs(dx);
        x = (x2 > x1) ? x2 : x1;
        while(dx > 0) {
            setPixel(x--, y1, r, g, b);
            --dx;
        }
    }else{
        // bresenham line algorithm
        // middle point
        // for diagonal lines
        int adx = abs(dx);
        int ady = abs(dy);
        // updates
        int px = 2 * ady - adx;
        int py = 2 * adx - ady;
        // for end limit
        int xend = 0, yend = 0;
        // slopes
        if(ady <= adx){
            if(dx >= 0){
                x = x1;
                y = y1;
                xend = x2;
            }else{
                x = x2;
                y = y2;
                xend = x1;
            }

            setPixel(x, y, r, g, b);
            for(int i = 0; i < xend; i++){
                x++;
                if (px < 0) {
                    px = px + 2 * ady;
                } else {
                    y += ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) ? 1 : -1;
                    px = px + 2 * (ady - adx);
                }
                setPixel(x, y, r, g, b);
            }
        }else{
            if(dy >= 0){
                x = x1;
                y = y1;
                yend = y2;
            }else{
                x = x2;
                y = y2;
                yend = y1;
            }

            setPixel(x, y, r, g, b);
            for(int i = 0; i < yend; i++){
                y++;
                if (py <= 0) {
                    py = py + 2 * adx;
                } else {
                    x += ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) ? 1 : -1;
                    py = py + 2 * (adx - ady);
                }
                setPixel(x, y, r, g, b);
            }
        }
    }
}

void Raster::naiveLine(unsigned int x1, unsigned int y1, 
        unsigned int x2, unsigned int y2, 
        unsigned char r,
        unsigned char g,
        unsigned char b){
    // here
    int dx = x2 - x1;
    int dy = y2 - y1;
    double m = (double)(dy)/(double)(dx); //slope
    double be = (double)y1 - (m*x1) ;
    // cursor
    unsigned int x = 0, y = 0;

    if(dx == 0){
        // draw on Y
        dy = abs(dy);
        y = (y2 > y1) ? y2 : y1;
        while(dy > 0) {
            setPixel(x1, y--, r, g, b);
            --dy;
        }
    }else if(dy == 0){
        // draw on X
        dx = abs(dx);
        x = (x2 > x1) ? x2 : x1;
        while(dx > 0) {
            setPixel(x--, y1, r, g, b);
            --dx;
        }
    }else{
        // increment X and Y 
        if(m > 1){
            dy = abs(dy);
            y = (y2 > y1) ? y2 : y1;
            while(dy > 0) {
                x = round((y - be) / m);
                setPixel(x, y--, r, g, b);
                --dy;
            }
        }else{
            dx = abs(dx);
            x = (x2 > x1) ? x2 : x1;
            while(dx > 0) {
                y = (m * x) + be;
                setPixel(x--, y, r, g, b);
                --dx;
            }
        }
    }
}