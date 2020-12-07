#include "headers/ParserObj.hpp"

using namespace std;

int main(int argc, char const *argv[]){
    /* code */
    if(argc < 2){
        fprintf(stderr, "USAGE: %s [.ojb filename]\n", argv[0]);
        return 1;
    }

    Mesh meshModel;
    Raster *mRaster = new Raster();

    ifstream file(argv[1]);
    if(!file.is_open()){
        fprintf(stderr, "File (%s) not found!\n", argv[1]);
        return 1;
    }else{
        readObj(file, mRaster, meshModel);
    } 
        
    if(meshModel.Vertices.size() != 0){
        scale(meshModel, 0);
        translate(meshModel, 5, 5);
        rotate(meshModel, 0, 70);
        translate(meshModel, 5, 5);

        unsigned char r = 0x00;
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
    

    file.close();
    vector<string> splittedFilename;
    split(argv[1], splittedFilename, ".");
    mRaster->write(splittedFilename[0]);

    return 0;
}