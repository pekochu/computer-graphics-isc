#include "headers/ParserObj.hpp"

using namespace std;

int main(int argc, char const *argv[]){
    /* code */
    if(argc < 2){
        fprintf(stderr, "USAGE: %s [.ojb filename]\n", argv[0]);
        return 1;
    }

    // Variables
    VLF model;
    Mesh triangulatedModel;
    char* dynamic_filename;
    Raster *mRaster = new Raster();

    // get name
    vector<string> splittedFilename;
    split(argv[1], splittedFilename, ".");
    string formatExt = splittedFilename[splittedFilename.size() - 1];

    // Read OBJ or VLF
    if(formatExt == "obj"){
        Mesh meshModel, triangulatedModel;
        ifstream file(argv[1]);
        if(!file.is_open()){
            fprintf(stderr, "File (%s) not found!\n", argv[1]);
            return 1;
        }else{
            readObj(file, meshModel);
        } 

        // convert to VLF
        cout << "Parsing to VLF..." << endl;
        parseToVlf(triangulatedModel, meshModel);
        createVLF(triangulatedModel, model);

        file.close();
    }else if(formatExt == "vlf"){
        ifstream file(argv[1]);
        if(!file.is_open()){
            fprintf(stderr, "File (%s) not found!\n", argv[1]);
            return 1;
        }else{
            readVlf(file, model);
        } 
        file.close();
    }else{
        cout << "Format not admitted." << endl;
    }

    // begin parse
    if(model.Vertices.size() != 0){
        Mesh test;

        for(int i = 0; i < model.Vertices.size(); i++){
            test.Vertices.push_back(model.Vertices[i]);
        }

        for(int i = 0; i < model.Faces.size(); i++){
            Facesx tFacesx;

            tFacesx.Vertices.push_back(model.Faces[i].E0.P0);
            tFacesx.Vertices.push_back(model.Faces[i].E1.P0);
            tFacesx.Vertices.push_back(model.Faces[i].E2.P0);

            test.Faces.push_back(tFacesx);
        }

        float helperMatrix[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        getMaxMins(test.Vertices, helperMatrix);
        scale(test, helperMatrix[6]-50);

        getMaxMins(test.Vertices, helperMatrix);
        translate(test, helperMatrix[0]*-1, helperMatrix[2]*-1, 1);
        getMaxMins(test.Vertices, helperMatrix);
        
        unsigned char r = 0x00;
        unsigned char g = 0x00;
        unsigned char b = 0x00;

        cout << "Drawing...\n";
        mRaster->clear();
        for(int i = 0; i < int(test.Faces.size()); i++){
            for(int j = 0; j < int(test.Faces[i].Vertices.size())-1; j+=1){
                unsigned int x1 = test.Faces[i].Vertices[j].Position.X + (960 - helperMatrix[7]);
                unsigned int y1 = test.Faces[i].Vertices[j].Position.Y + (540 - helperMatrix[8]);
                unsigned int x2 = test.Faces[i].Vertices[j+1].Position.X + (960 - helperMatrix[7]);
                unsigned int y2 = test.Faces[i].Vertices[j+1].Position.Y + (540 - helperMatrix[8]);
                mRaster->drawLine(x1, y1, x2, y2, r, g, b);
            }
        }

        dynamic_filename = (char *)calloc(splittedFilename[0].size() + 5, sizeof(char));
        sprintf(dynamic_filename, "%s.ppm", splittedFilename[0].c_str());        
        mRaster->write(dynamic_filename);
        free(dynamic_filename);  
    }else{
        cout << "Couldn't export PPM file." << endl;
    }

    return 0;
}