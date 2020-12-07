#include "headers/ParserObj.hpp"

using namespace std;

int main(int argc, char const *argv[]){
    /* code */
    if(argc < 2){
        fprintf(stderr, "USAGE: %s [.ojb filename]\n", argv[0]);
        return 1;
    }

    Mesh meshModel;

    bool flag_ask_view = false;
    int user_flag_selected = 0;
    do{
        cout << "\nPlease select a perspective view:\n";
        cout << "\t1. XY view\n";
        cout << "\t2. XZ view\n";
        cout << "\t3. YZ view\n\n> ";
        cin >> user_flag_selected;
        if(user_flag_selected > 0 && user_flag_selected < 4){
            if(user_flag_selected == 1) VIEW_FLAG = XY_VIEW;
            if(user_flag_selected == 2) VIEW_FLAG = XZ_VIEW;
            if(user_flag_selected == 3) VIEW_FLAG = YZ_VIEW;
            // get out of loop
            flag_ask_view = true;
        }
    }while(!flag_ask_view);

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
        rotate(meshModel, 0, 0, 30);
        translate(meshModel, 0, 0);

        unsigned char r = 0xFF;
        unsigned char g = 0xFF;
        unsigned char b = 0xFF;

        /* for (int i = 0; i < int(meshModel.Vertices.size()) - 1; i+=1){
            // draw the model
            mRaster->drawLine(meshModel.Vertices[i].Position.X, meshModel.Vertices[i].Position.Y,
                    meshModel.Vertices[i+1].Position.X, meshModel.Vertices[i+1].Position.Y, r, g, b);
        } */

        /* for(int i = 0; i < int(meshModel.Faces.size()); i++){
            for(int j = 0; j < int(meshModel.Faces[i].Vertices.size()); j+=2){
                mRaster->drawLine(meshModel.Faces[i].Vertices[j].Position.X, meshModel.Faces[i].Vertices[j].Position.Y,
                        meshModel.Faces[i].Vertices[j+1].Position.X, meshModel.Faces[i].Vertices[j+1].Position.Y, r, g, b);
                cout << "Face " << i + 1 << ". X = " << meshModel.Faces[i].Vertices[j].Position.X; 
                cout << "\tY = " <<  meshModel.Faces[i].Vertices[j].Position.Y << endl;
            }            
        } */

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
    mRaster->write(argv[1]);

    return 0;
}