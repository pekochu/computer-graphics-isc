#include "Raster.hpp"

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

void Raster::write(const char *arg){
    int x = 0;
    int y = 0;

    FILE *fp;
    char *filename = (char*)calloc(strlen(arg)+4, sizeof(char));
    sprintf(filename, "%s.ppm", arg);

    fp = fopen(filename, "wb");
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

void Raster::bresenhamLine(
    unsigned int x1, unsigned int y1,
    unsigned int x2, unsigned int y2,
    unsigned char r,
    unsigned char g,
    unsigned char b) {

    double m; //slope
    double be;
    double aux;
    unsigned int x = 0;
    unsigned int y = 0;
    int dx, dy, p, updateUp, updateRight;

    if (x2 > x1 && y2 > y1) { // signs check-in because x1,x2,y1,y2 are unsigned so can't be negative
        m = (double)(y2 - y1) / (double)(x2 - x1);
    }

    if (x1 > x2 && y2 > y1) {
        m = (double)(y2 - y1) / (double)(x1 - x2);
    }

    if (x2 > x1 && y1 > y2) {
        m = (double)(y1 - y2) / (double)(x2 - x1);
    }

    if (x1 > x2 && y1 > y2) {
        m = (double)(y1 - y2) / (double)(x1 - x2);
    }

    aux = m;

    if (x2 > x1 && y2 > y1 && aux <= 1 && aux >= 0) { //1st Octant 
        m = (double)(y2 - y1) / (double)(x2 - x1);
        be = (double) y1 - (m * x1);
        dx = x2 - x1;
        dy = y2 - y1;
        p = 2 * dy - dx;
        updateUp = 2 * dy;
        updateRight = 2 * dy - 2 * dx;
    }

    if (x2 > x1 && y2 > y1 && aux > 1) { //2nd Octant
        m = (double)(y2 - y1) / (double)(x2 - x1);
        be = (double) y1 - (m * x1);
        dx = x2 - x1;
        dy = y2 - y1;
        p = 2 * dy - dx;
        updateUp = 2 * dx;
        updateRight = 2 * dx - 2 * dy;
    }

    if (x1 > x2 && y2 > y1 && aux > 1) { //3th octant
        m = (double)(x1 - x2) / (double)(y2 - y1);
        be = (double) x1 - (m * y1);
        dx = x1 - x2;
        dy = y2 - y1;
        p = 2 * dy - dx;
        updateUp = 2 * dx;
        updateRight = 2 * dx - 2 * dy;
    }

    if (x1 > x2 && y2 > y1 && aux <= 1 && aux >= 0) { //4rd octant
        m = (double)(y2 - y1) / (double)(x1 - x2);
        be = (double) y1 - (m * x1);
        dx = x1 - x2;
        dy = y2 - y1;
        p = 2 * dy - dx;
        updateUp = 2 * dy;
        updateRight = 2 * dy - 2 * dx;
    }

    if (x2 > x1 && y1 > y2 && aux <= 1 && aux >= 0) { //5th octant
        m = (double)(y1 - y2) / (double)(x2 - x1);
        be = (double) y1 - (m * x1);
        dx = x2 - x1;
        dy = y1 - y2;
        p = 2 * dy - dx;
        updateUp = 2 * dy;
        updateRight = 2 * dy - 2 * dx;
    }
    if (x2 > x1 && y1 > y2 && aux > 1) { //6th octant
        m = (double)(x2 - x1) / (double)(y1 - y2);
        be = (double) x1 - (m * y1);
        dx = x2 - x1;
        dy = y1 - y2;
        p = 2 * dy - dx;
        updateUp = 2 * dx;
        updateRight = 2 * dx - 2 * dy;
    }

    if (x1 > x2 && y1 > y2 && aux > 1) { //7th Octant
        m = (double)(x1 - x2) / (double)(y1 - y2);
        be = (double) x1 - (m * y1);
        dx = x1 - x2;
        dy = y1 - y2;
        p = 2 * dy - dx;
        updateUp = 2 * dx;
        updateRight = 2 * dx - 2 * dy;
    }

    if (x1 > x2 && y1 > y2 && aux <= 1 && aux >= 0) { //8th Octant
        m = (double)(y1 - y2) / (double)(x1 - x2);
        be = (double) y1 - (m * x1);
        dx = x1 - x2;
        dy = y1 - y2;
        p = 2 * dy - dx;
        updateUp = 2 * dy;
        updateRight = 2 * dy - 2 * dx;
    }

    setPixel(x1, y1, r, g, b);
    setPixel(x2, y2, r, g, b);

    if (x2 > x1 && y2 > y1 && aux >= 0 && aux <= 1) { //1st Octant Drawing
        y = m * x1 + be;
        for (x = x1 + 1; x < x2; x++) {
            if (p < 0) {
                y++;
                p += updateUp;
            } else {
                p += updateRight;
            }
            setPixel(x, y, r, g, b);
        }
    }

    if (x2 > x1 && y2 > y1 && aux > 1) { //2nd Octant drawing
        x = m * y1 + be;
        for (y = y1 + 1; y < y2; y++) {
            if (p < 0) {

                p += updateUp;
            } else {
                x++;
                p += updateRight;
            }
            setPixel(x, y, r, g, b);
        }
    }

    if (x1 > x2 && y2 > y1 && aux > 1) { //3rd Octant drawing
        x = m * y1 + be;
        for (y = y1 + 1; y < y2; y++) {
            if (p < 0) {

                p += updateUp;
            } else {
                x--;
                p += updateRight;
            }
            setPixel(x, y, r, g, b);
        }
    }

    if (x1 > x2 && y2 > y1 && aux >= 0 && aux <= 1) { //4th Octant drawing
        y = m * x1 + be;
        for (x = x1 - 1; x > x2; x--) {
            if (p < 0) {
                y++;
                p += updateUp;
            } else {
                p += updateRight;
            }
            setPixel(x, y, r, g, b);
        }
    }

    if (x2 > x1 && y1 > y2 && aux >= 0 && aux <= 1) { //5th octant drawing
        y = m * x1 + be;
        for (x = x1 + 1; x < x2; x++) {
            if (p < 0) {
                y--;
                p += updateUp;
            } else {
                p += updateRight;
            }
            setPixel(x, y, r, g, b);
        }
    }

    if (x2 > x1 && y1 > y2 && aux > 1) { //6th octant drawing
        x = m * y1 + be;
        for (y = y1 - 1; y > y2; y--) {
            if (p < 0) {
                p += updateUp;
            } else {
                x++;
                p += updateRight;
            }
            setPixel(x, y, r, g, b);
        }
    }

    if (x1 > x2 && y1 > y2 && aux > 1) { //7th Octant Drawing
        x = m * y1 + be;
        for (y = y1 - 1; y > y2; y--) {
            if (p < 0) {
                p += updateUp;
            } else {
                x--;
                p += updateRight;
            }
            setPixel(x, y, r, g, b);
        }
    }

    if (x1 > x2 && y1 > y2 && aux >= 0 && aux <= 1) { //8th Octant drawing
        y = m * x1 - be;
        for (x = x1 - 1; x > x2; x--) {
            if (p < 0) {
                y--;
                p += updateUp;
            } else {
                p += updateRight;
            }
            setPixel(x, y, r, g, b);
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