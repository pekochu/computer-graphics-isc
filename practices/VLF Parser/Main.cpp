#include "headers/ParserObj.hpp"

using namespace std;

int main(int argc, char const *argv[]){
    /* code */
    if(argc < 2){
        fprintf(stderr, "USAGE: %s [.ojb filename]\n", argv[0]);
        return 1;
    }

    // Variables
    Mesh meshModel;
    Mesh triangulatedModel;
    char* dynamic_filename;

    // Read model
    ifstream file(argv[1]);
    if(!file.is_open()){
        fprintf(stderr, "File (%s) not found!\n", argv[1]);
        return 1;
    }else{
        readObj(file, meshModel);
    } 
    file.close();

    // get name
    vector<string> splittedFilename;
    split(argv[1], splittedFilename, ".");

    // begin parse
    if(meshModel.Vertices.size() != 0){
        cout << "Parsing to VLF..." << endl;
        parseToVlf(triangulatedModel, meshModel);

        dynamic_filename = (char *)calloc(splittedFilename[0].size() + 5, sizeof(char));
        sprintf(dynamic_filename, "%s.vlf", splittedFilename[0].c_str());

        // Calculate edges
        cout << "Generating edges..." << endl;
        int eidx = 0;
        vector<Edge> edges;
        for(int i = 0; i < triangulatedModel.Faces.size(); i++){
            Edge mEdge;

            mEdge.P0 = triangulatedModel.Faces[i].Vertices[0];
            mEdge.P1 = triangulatedModel.Faces[i].Vertices[1];
            mEdge.index = eidx++;
            edges.push_back(mEdge);

            mEdge.P0 = triangulatedModel.Faces[i].Vertices[1];
            mEdge.P1 = triangulatedModel.Faces[i].Vertices[2];
            mEdge.index = eidx++;
            edges.push_back(mEdge);

            mEdge.P0 = triangulatedModel.Faces[i].Vertices[2];
            mEdge.P1 = triangulatedModel.Faces[i].Vertices[0];
            mEdge.index = eidx++;
            edges.push_back(mEdge);
        }

        cout << "Generating faces..." << endl;        
        vector<Face> faces;
        for(int i = 0; i < edges.size(); i++){
            Face mFace;

            mFace.E0 = edges[i];
            mFace.E1 = edges[++i];
            mFace.E2 = edges[++i];

            faces.push_back(mFace);
        }

        cout << "Writing..." << endl;
        ofstream vlfFile(dynamic_filename, ios::binary | ios::out);
        if(vlfFile.is_open()){
            int header = triangulatedModel.Vertices.size();
            vlfFile.write((char*) &header, sizeof(int));
            header = edges.size();
            vlfFile.write((char*) &header, sizeof(int));
            header = triangulatedModel.Faces.size();
            vlfFile.write((char*) &header, sizeof(int));

            for (int i = 0; i < triangulatedModel.Vertices.size(); i++){
                vlfFile.write((char *) &triangulatedModel.Vertices[i].Position.X, sizeof(float));
                vlfFile.write((char *) &triangulatedModel.Vertices[i].Position.Y, sizeof(float));
                vlfFile.write((char *) &triangulatedModel.Vertices[i].Position.Z, sizeof(float));
            }
                
            for (int i = 0; i < edges.size(); i++){
                vlfFile.write((char *) &edges[i].P0.index, sizeof(int));
                vlfFile.write((char *) &edges[i].P1.index, sizeof(int));
            }

            for (int i = 0; i < faces.size(); i++){
                vlfFile.write((char *) &faces[i].E0.index, sizeof(int));
                vlfFile.write((char *) &faces[i].E1.index, sizeof(int));
                vlfFile.write((char *) &faces[i].E2.index, sizeof(int));
            }
        }

        vlfFile.close();
        free(dynamic_filename);
            
    }else{
        cout << "Couldn't export VLF file." << endl;
    }

    return 0;
}