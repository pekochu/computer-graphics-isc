#include "headers/ParserObj.hpp"
#include <thread> 
#include <pthread.h>

using namespace std;

int printPerspectives(string, int);

int main(int argc, char const *argv[]){
    /* code */
    if(argc < 2){
        fprintf(stderr, "USAGE: %s [.ojb filename]\n", argv[0]);
        return 1;
    }

    ifstream file(argv[1]);
    if(!file.is_open()){
        fprintf(stderr, "File (%s) not found!\n", argv[1]);
        return 1;
    }else{
        file.close();
        string filename(argv[1], strlen(argv[1]));
        
        thread t1(printPerspectives, filename, XY_VIEW);
        thread t2(printPerspectives, filename, XZ_VIEW);
        thread t3(printPerspectives, filename, YZ_VIEW);

        t1.join();
        t2.join();
        t3.join();
    }

    file.close();

    return 0;
}

int printPerspectives(string filename, int view){
    Raster *mRaster = new Raster();
    Mesh meshModel;
    
    ifstream file(filename.c_str());
    readObj(file, mRaster, meshModel, view);
    file.close();

    if(meshModel.Vertices.size() != 0){
        scale(meshModel, 0);
        translate(meshModel, 5, 5);

        unsigned char r = 0xFF;
        unsigned char g = 0x00;
        unsigned char b = 0x00;

        for(int i = 0; i < int(meshModel.Faces.size()); i++){
            for(int j = 0; j < int(meshModel.Faces[i].Vertices.size())-1; j+=1){
                unsigned int x1 = meshModel.Faces[i].Vertices[j].Position.X;
                unsigned int y1 = meshModel.Faces[i].Vertices[j].Position.Y;
                unsigned int x2 = meshModel.Faces[i].Vertices[j+1].Position.X;
                unsigned int y2 = meshModel.Faces[i].Vertices[j+1].Position.Y;
                mRaster->drawLine(x1, y1, x2, y2, r, g, b);
            }
        }
    }else{
        cout << "The model couldn't be drawn" << endl;
    }

    if(view == XY_VIEW) mRaster->write("XYView");
    if(view == XZ_VIEW) mRaster->write("XZView");
    if(view == YZ_VIEW) mRaster->write("ZYView");
    return 0;
}