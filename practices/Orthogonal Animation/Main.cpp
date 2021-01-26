#include "headers/ParserObj.hpp"

using namespace std;

int main(int argc, char const *argv[]){
    /* code */
    if(argc < 2){
        fprintf(stderr, "USAGE: %s [.ojb filename]\n", argv[0]);
        return 1;
    }

    Mesh meshModel;
    char* dynamic_filename;
    Raster *mRaster = new Raster();

    ifstream file(argv[1]);
    if(!file.is_open()){
        fprintf(stderr, "File (%s) not found!\n", argv[1]);
        return 1;
    }else{
        readObj(file, mRaster, meshModel);
    } 
    file.close();

    vector<string> splittedFilename;
    split(argv[1], splittedFilename, ".");

    float centeredX = 0.0f, centeredY = 0.0;
        
    if(meshModel.Vertices.size() != 0){
        float helperMatrix[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        getMaxMins(meshModel.Vertices, helperMatrix);
        scale(meshModel, helperMatrix[6]-50);

        for(int x = 0; x < 360; x++){
            getMaxMins(meshModel.Vertices, helperMatrix);
            translate(meshModel, helperMatrix[0]*-1, helperMatrix[2]*-1, 1);
            getMaxMins(meshModel.Vertices, helperMatrix);
            // Mesh toDisplay = meshModel;
            // Make rotations
            cout << "Rotating and centering..." << endl;
            rotate(meshModel, 1.5f, ROTATE_ON_Y);
            getMaxMins(meshModel.Vertices, helperMatrix);
            
            unsigned char r = 0x00;
            unsigned char g = 0x00;
            unsigned char b = 0x00;

            cout << "Drawing...\n";
            mRaster->clear();
            for(int i = 0; i < int(meshModel.Faces.size()); i++){
                for(int j = 0; j < int(meshModel.Faces[i].Vertices.size())-1; j+=1){
                    unsigned int x1 = meshModel.Faces[i].Vertices[j].Position.X + (960 - helperMatrix[7]);
                    unsigned int y1 = meshModel.Faces[i].Vertices[j].Position.Y + (540 - helperMatrix[8]);
                    unsigned int x2 = meshModel.Faces[i].Vertices[j+1].Position.X + (960 - helperMatrix[7]);
                    unsigned int y2 = meshModel.Faces[i].Vertices[j+1].Position.Y + (540 - helperMatrix[8]);
                    mRaster->drawLine(x1, y1, x2, y2, r, g, b);
                }
            }

            dynamic_filename = (char *)calloc(splittedFilename[0].size() + 5, sizeof(char));
            sprintf(dynamic_filename, "%s.%03d.ppm", splittedFilename[0].c_str(), x);
            mRaster->write(dynamic_filename);
            free(dynamic_filename);
        }
    }else{
        cout << "The model couldn't be drawn" << endl;
    }

    return 0;
}