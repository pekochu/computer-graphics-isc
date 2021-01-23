#include "Raster.hpp"

Raster::Raster(void){
    memset(data, 0xFF, 1920*1080*3);
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

void Raster::write(char *arg){
    int x = 0;
    int y = 0;

    FILE *fp;    

    fp = fopen(arg, "wb");
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

void Raster::clear(){
    for(int y = 0; y < 1080; y++) {
        for(int x = 0;x < 1920; x++) {
            data[x][y][0] = 0xFF;
            data[x][y][1] = 0xFF;
            data[x][y][2] = 0xFF;
        } 
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

void Raster::bresenhamLine(unsigned int x1, unsigned int y1, 
        unsigned int x2, unsigned int y2, 
        unsigned char r,
        unsigned char g,
        unsigned char b){
    // here
    
    if (x2 < x1) {
        xIni = (double)x2;
        yIni = (double)y2;
        xEnd = (double)x1;
        yEnd = (double)y1;
    } else {
        xIni = (double)x1;
        yIni = (double)y1;
        xEnd = (double)x2;
        yEnd = (double)y2;
    }

    if (yEnd - yIni == 0) {
        for (int i = xIni; i <= xEnd; i++) {
            if (i >= 0 && i < 1920 && yIni >= 0 && yIni < 1080) {
                setPixel(i, yIni, r, g, b);
            }                
        }
    } else if (xEnd - xIni == 0) {
        int aux = 0;
        if (yEnd < yIni) {
            aux = yIni;
            yIni = yEnd;
            yEnd = aux;
        }
        for (int i = yIni; i <= yEnd; i++) {
            if (xIni >= 0 && xIni < 1920 && i >= 0 && i < 1080) {
                setPixel(xIni, i, r, g, b);
            }
        }
    } else {
        slope = (yEnd - yIni) / (xEnd - xIni);
        evaluatedSlope = bresenhamSlope(slope);
        if (evaluatedSlope == 1) {
            int yd = (int)yIni;
            for (int i = xIni; i <= xEnd; i++) {
                if (i >= 0 && i < 1920 && yd >= 0 && yd < 1080) {
                    setPixel(i, yd, r, g, b);
                }
                yd++;
            }
        } else if (evaluatedSlope == 2) {
            bresenhamDraw((int)xIni, (int)yIni, (int)xEnd, (int)yEnd,
                        r, g, b, evaluatedSlope);
        } else if (evaluatedSlope == 3) {
            bresenhamDraw((int)xIni, (int)yIni, (int)xEnd, (int)yEnd,
                        r, g, b, evaluatedSlope);
        } else if (evaluatedSlope == 4) {
            bresenhamDraw((int)xIni, (int)yIni, (int)xEnd, (int)yEnd,
                        r, g, b, evaluatedSlope);
        } else if (evaluatedSlope == 5) {
            int yd = (int)yIni;
            for (int i = xIni; i <= xEnd; i++) {
                if (i >= 0 && i < 1920 && yd >= 0 && yd < 1080) {
                    setPixel(i, yd, r, g, b);
                }
                yd--;
            }
        } else {
            bresenhamDraw((int)xIni, (int)yIni, (int)xEnd, (int)yEnd,
                        r, g, b, evaluatedSlope);
        }
    }
}

// evaluate slope
int Raster::bresenhamSlope(double slope) {
    if (slope > 1)		
        return 2;
    else if (slope < 0 && slope > -1)	
        return 3;
    else if (slope < -1)	
        return 4;
    else if (slope == 1)	
        return 1;
    else if (slope == -1)	
        return 5;
    else			
        return 0;
}

void Raster::bresenhamDraw(int x0, int y0, int x1, int y1, 
        unsigned char r, unsigned char g, unsigned char b, int slope) {
    // here
    int dx;
    int dy;
    int stepX, stepY;
    
    dx = x1 - x0;
    dy = y1 - y0;

    if (dy < 0) {
        dy = -dy;
        stepY = -1;
    } else stepY = 1;

    if (dx < 0) {
        dx = -dx;
        stepX = -1;
    } else stepX = 1;
    
    dx <<= 1;
    dy <<= 1;
    
    if (x0 >= 0 && x0 < 1920 && y0 >= 0 && y0 < 1080) {
        setPixel(x0, y0, r, g, b);
    }
    if (dx > dy) {
        int fraction = dy - (dx >> 1);
        
        while (x0 != x1) {
            x0 += stepX;
            if (fraction >= 0) {
                y0 += stepY;
                fraction -= dx;
            }
            fraction += dy;
            
            setPixel(x0, y0, r, g, b);
        }
    } else {
        int fraction = dx - (dy >> 1);
        
        while (y0 != y1) {
            
            if (fraction >= 0) {
                x0 += stepX;
                fraction -= dy;
            }
            y0 += stepY;
            fraction += dx;
            
            if (x0 >= 0 && x0 < 1920 && y0 >= 0 && y0 < 1080) {
                setPixel(x0, y0, r, g, b);
            }
        }
    }
}