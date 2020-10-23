#include "Raster.h"

int main(int argc, char const *argv[]){
    /* code */
    Raster * mRaster = new Raster();
    mRaster->randomLines(500);

    mRaster->write();
    return 0;
}
