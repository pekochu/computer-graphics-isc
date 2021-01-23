#include "ParserObj.hpp"

int VIEW_FLAG = XY_VIEW;

Mesh::Mesh(){ /* CONSTRUCTOR */ }; 

Mesh::Mesh(std::vector<Vertex>& _Vertices) {
    Vertices = _Vertices;
}

Mesh::Mesh(const Mesh &obj) {

    for (int i = 0; i < int(obj.Vertices.size()); i++){
        Vertices.push_back(obj.Vertices[i]);
    }

    for(int i = 0; i < int(obj.Faces.size()); i++){
        Faces.push_back(obj.Faces[i]);
    }

    MeshName = obj.MeshName;
}


void readObj(ifstream &file, Raster *mRaster, Mesh& model){
    vector<Facesx> Faces;
    vector<Vector3> Positions;
    vector<Vertex> vertices;

    string line;
    while(getline(file, line)){
        // get vertix
        if (firstToken(line) == "v"){
            vector<string> spos;
            Vector3 modelLines;
            split(tail(line), spos, " ");

            if(VIEW_FLAG == XY_VIEW){
                modelLines.X = stof(spos[0]);
                modelLines.Y = (- stof(spos[1]));
                modelLines.Z = stof(spos[2]);
                modelLines.W = 1.0f;
            }else if(VIEW_FLAG == XZ_VIEW){
                modelLines.X = stof(spos[0]);
                modelLines.Y = stof(spos[2]);
                modelLines.Z = (- stof(spos[1]));
                modelLines.W = 1.0f;
            }else if(VIEW_FLAG == YZ_VIEW){
                modelLines.X = stof(spos[2]);
                modelLines.Y = (- stof(spos[1]));
                modelLines.Z = stof(spos[0]);
                modelLines.W = 1.0f;
            }
            
            Positions.push_back(modelLines);
        }

        if(firstToken(line) == "f"){
            vector<Vertex> vVerts;            
            getVertices(Faces, vVerts, Positions, line);

            for (int i = 0; i < int(vVerts.size()); i++){
                vertices.push_back(vVerts[i]);
            }
        }

        if(firstToken(line) == "o"){
            vector<string> splitted;
            split(line, splitted, " ");
            model.MeshName = splitted[1];
        }
    }

    model.Faces = Faces;
    model.Vertices = vertices;
    return;
}

void split(const string &in, vector<string> &out, string token) {
    // here
    out.clear();

    string temp;

    for (int i = 0; i < int(in.size()); i++){
        string test = in.substr(i, token.size());

        if (test == token) {
            if (!temp.empty()) {
                out.push_back(temp);
                temp.clear();
                i += (int)token.size() - 1;
            } else {
                out.push_back("");
            }
        } else if (i + token.size() >= in.size()) {
            temp += in.substr(i, token.size());
            out.push_back(temp);
            break;
        } else {
            temp += in[i];
        }
    }
}

void getVertices(vector<Facesx> &mFaces, vector<Vertex> &mVert, const vector<Vector3>& mPositions, string line){
    vector<string> sface, svert;
    Facesx mFace;
    Vertex vVert;

    split(tail(line), sface, " ");
    

    for (int i = 0; i < int(sface.size()); i++) {
        split(sface[i], svert, "/");
        // indice
        int idx = 0;

        // Calculate and store the vertex        
        idx = stoi(svert[0]);
        idx = (idx < 0) ? int(mPositions.size()) + idx : idx - 1;
        vVert.Position = mPositions[idx];
        mVert.push_back(vVert);

        mFace.Vertices.push_back(vVert);
    }
    mFaces.push_back(mFace);
}

string tail(const string &in) {
    size_t token_start = in.find_first_not_of(" \t");
    size_t space_start = in.find_first_of(" \t", token_start);
    size_t tail_start = in.find_first_not_of(" \t", space_start);
    size_t tail_end = in.find_last_not_of(" \t");
    if (tail_start != string::npos && tail_end != string::npos) {
        return in.substr(tail_start, tail_end - tail_start + 1);
    } else if (tail_start != string::npos){
        return in.substr(tail_start);
    }
    return "";
}

// Get first token of string
string firstToken(const string &in) {
    if (!in.empty()) {
        size_t token_start = in.find_first_not_of(" \t");
        size_t token_end = in.find_first_of(" \t", token_start);
        if (token_start != string::npos && token_end != string::npos){
            return in.substr(token_start, token_end - token_start);
        } else if (token_start != string::npos) {
            return in.substr(token_start);
        }
    }
    return "";
}

/*
 * Helper functions
 */
void getMaxMins(vector<Vertex> &verts, float matrix[10]){
    float sxLowest = 0.0f, sxHighest = 0.0f;
    float syLowest = 0.0f, syHighest = 0.0f;
    float szLowest = 0.0f, szHighest = 0.0f;
    int sxavg = 0, syavg = 0, sfavg = 0;

    for (int i = 0; i < int(verts.size()); i++){
        sxLowest = (verts[i].Position.X < sxLowest) ? verts[i].Position.X : sxLowest; 
        sxHighest = (verts[i].Position.X > sxHighest) ? verts[i].Position.X : sxHighest;

        syLowest = (verts[i].Position.Y < syLowest) ? verts[i].Position.Y : syLowest; 
        syHighest = (verts[i].Position.Y > syHighest) ? verts[i].Position.Y : syHighest; 

        szLowest = (verts[i].Position.Z < szLowest) ? verts[i].Position.Z : szLowest; 
        szHighest = (verts[i].Position.Z > szHighest) ? verts[i].Position.Z : szHighest; 
    }

    sxavg = 1920 / (abs(sxLowest) + abs(sxHighest));
    syavg = 1080 / (abs(syLowest) + abs(syHighest));

    sfavg = (sxavg < syavg) ? sxavg -5 : syavg -5;

    matrix[0] = sxLowest;
    matrix[1] = sxHighest;
    matrix[2] = syLowest;
    matrix[3] = syHighest;
    matrix[4] = szLowest;
    matrix[5] = szHighest;
    matrix[6] = sfavg;
    matrix[7] = (sxHighest + sxLowest)/2.0f;
    matrix[8] = (syHighest + syLowest)/2.0f;
    matrix[9] = (szHighest + szLowest)/2.0f;
}
/*
 * This functions perform rotations, translations, scaletions
*/
void applyMatrix(vector<Vertex> &vertices, float matrix[4][4]){
	int i = 0;
	int j = 0;
	int op = 1;
	Vector3 aux;

	for (i = 0; i < vertices.size(); i++) {
		aux.X = 0.0f;
		aux.Y = 0.0f;
		aux.Z = 0.0f;
        aux.W = 0.0f;

		for (j = 0; j < 4; j++) {
			switch (op) {
                case 1:
                    aux.X += (matrix[j][0] * vertices[i].Position.X);
                    aux.X += (matrix[j][1] * vertices[i].Position.Y);
                    aux.X += (matrix[j][2] * vertices[i].Position.Z);
                    aux.X += (matrix[j][3] * vertices[i].Position.W);
                    op = 2;
                break;
                case 2:
                    aux.Y += (matrix[j][0] * vertices[i].Position.X);
                    aux.Y += (matrix[j][1] * vertices[i].Position.Y);
                    aux.Y += (matrix[j][2] * vertices[i].Position.Z);
                    aux.Y += (matrix[j][3] * vertices[i].Position.W);
                    op = 3;
                    break;
                case 3:
                    aux.Z += (matrix[j][0] * vertices[i].Position.X);
                    aux.Z += (matrix[j][1] * vertices[i].Position.Y);
                    aux.Z += (matrix[j][2] * vertices[i].Position.Z);
                    aux.Z += (matrix[j][3] * vertices[i].Position.W);
                    op = 4;
                break;
                case 4:
                    aux.W += (matrix[j][0] * vertices[i].Position.X);
                    aux.W += (matrix[j][1] * vertices[i].Position.Y);
                    aux.W += (matrix[j][2] * vertices[i].Position.Z);
                    aux.W += (matrix[j][3] * vertices[i].Position.W);
                    op = 1;
                break;
			}
		}

		vertices[i].Position = aux;
	}
}

void scale(Mesh &model, int s){
        float matrix[4][4] = {
		{(float)s, 0, 0, 0},
		{0, (float)s, 0, 0},
		{0, 0, (float)s, 0},
		{0, 0, 0, 1}
	};
    
    cout << "Scaling model with " << s << " factor..." << endl;
    applyMatrix(model.Vertices, matrix);
    for(int i = 0; i < int(model.Faces.size()); i++){
        applyMatrix(model.Faces[i].Vertices, matrix);
    }
}

void translate(Mesh &model, float x, float y, float z){
    
    float matrix[4][4] = {
		{1, 0, 0, x},
		{0, 1, 0, y},
		{0, 0, 1, z},
		{0, 0, 0, 1}
	};

    cout << "Translating model to (" << x << ", " << y << ", " << z << ")" << endl;
    applyMatrix(model.Vertices, matrix);
    for(int i = 0; i < int(model.Faces.size()); i++){
        applyMatrix(model.Faces[i].Vertices, matrix);
    }
}

void focalTranslation(Mesh &model, float x, float y, float z, float f){
    float matrix[4][4] = {
		{1, 0, 0, -x},
		{0, 1, 0, -y},
		{0, 0, 1, (3 * f) - z},
		{0, 0, 1 / f, (((3 * f) - z)) / f}
	};

    cout << "Translating model to (" << x << ", " << y << ", " << z << ") and Focal " << f << endl;
    applyMatrix(model.Vertices, matrix);
    for(int i = 0; i < int(model.Faces.size()); i++){
        applyMatrix(model.Faces[i].Vertices, matrix);
    }
}

void translation(Mesh &model, float x, float y, float z){
    
    float matrix[4][4] = {
		{1, 0, 0, -x},
		{0, 1, 0, -y},
		{0, 0, 1, -z},
		{0, 0, 0, 1}
	};

    cout << "Translating model to (" << x << ", " << y << ", " << z << ")" << endl;
    applyMatrix(model.Vertices, matrix);
    for(int i = 0; i < int(model.Faces.size()); i++){
        applyMatrix(model.Faces[i].Vertices, matrix);
    }
}

void transform3D(Mesh &model){
    for (int i = 0; i < model.Vertices.size(); i++) {
		model.Vertices[i].Position.X /= model.Vertices[i].Position.W;
		model.Vertices[i].Position.Y /= model.Vertices[i].Position.W;
		model.Vertices[i].Position.Z /= model.Vertices[i].Position.W;
	}

    for(int i = 0; i < int(model.Faces.size()); i++){
        for (int j = 0; j < model.Faces[i].Vertices.size(); j++) {
            model.Faces[i].Vertices[j].Position.X /= model.Faces[i].Vertices[j].Position.W;
            model.Faces[i].Vertices[j].Position.Y /= model.Faces[i].Vertices[j].Position.W;
            model.Faces[i].Vertices[j].Position.Z /= model.Faces[i].Vertices[j].Position.W;
        }
    }
}

void rotate(Mesh &model, float beta, int ROTATE_FLAG) { 
    float cr = COS(beta);
	float sr = SIN(beta);

    float matrix[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    if(ROTATE_FLAG == ROTATE_ON_X){
        matrix[1][1] = cr;
        matrix[1][2] = -sr;
        matrix[2][1] = sr;
        matrix[2][2] = cr;
    }else if(ROTATE_FLAG == ROTATE_ON_Y){
        matrix[0][0] = cr;
        matrix[0][2] = sr;
        matrix[2][0] = -sr;
        matrix[2][2] = cr;
    }else if(ROTATE_FLAG == ROTATE_ON_Z){
        matrix[0][1] = cr;
        matrix[0][1] = -sr;
        matrix[1][0] = sr;
        matrix[1][1] = cr;
    }else{
        matrix[1][1] = cr;
        matrix[1][2] = -sr;
        matrix[2][1] = sr;
        matrix[2][2] = cr;
    }

	applyMatrix(model.Vertices, matrix);
    for(int i = 0; i < int(model.Faces.size()); i++){
        applyMatrix(model.Faces[i].Vertices, matrix);
    }
} 