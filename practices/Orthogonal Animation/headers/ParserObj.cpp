#include "ParserObj.hpp"

int VIEW_FLAG = XY_VIEW;

void readObj(ifstream &file, Raster *mRaster, Mesh& model){
    vector<Facesx> Faces;
    vector<Vector3> Positions;
    vector<Vertex> vertices;
    vector<unsigned int> indices;

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

// Transformation functions
// find new pixel
void findNewCoordinate(int s[][2], float p[][1]) { 
    float temp[2][1] = { 0 }; 
  
    for (int i = 0; i < 2; i++) 
        for (int j = 0; j < 1; j++) 
            for (int k = 0; k < 2; k++) 
                temp[i][j] += (s[i][k] * p[k][j]); 
  
    p[0][0] = temp[0][0]; 
    p[1][0] = temp[1][0]; 
} 
  
// Scaling the point
void performScale(Vector3 &position, int sx, int sy) {   
    // Initializing the Scaling Matrix. 
    int s[2][2] = { sx, 0, 0, sy }; 
    float p[2][1]; 
  
    // Scaling the point
    p[0][0] = position.X;
    p[1][0] = position.Y;
    findNewCoordinate(s, p);
    position.X = p[0][0];
    position.Y = p[1][0];
} 

// translate the vertices
void performTranslate (Vector3 &position, float xf, float yf) {   
    // calculating translated coordinates 
    position.X += xf; 
    position.Y += yf; 
} 

// perform rotation
void performRotate(vector<Vertex> &vertices, int n, int x_pivot, int y_pivot, int angle) { 
    int i = 0; 
    while (i < n) { 
        int x_shifted = vertices[i].Position.X - x_pivot; 
        int y_shifted = vertices[i].Position.Y - y_pivot; 
  
        vertices[i].Position.X = x_pivot + (x_shifted*COS(angle)  
                          - y_shifted*SIN(angle)); 
        vertices[i].Position.Y = y_pivot + (x_shifted*SIN(angle)  
                          + y_shifted*COS(angle)); 
        i++; 
    } 
} 

/*
  This function apply the matrix to perform rotations, translations, scaletions
*/
void applyMatrix(vector<Vertex> &vertices, int n, double matrix[4][4]){
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
    float sxLowest = 0.0f, sxHighest = 0.0f;
    float syLowest = 0.0f, syHighest = 0.0f;
    int sxavg = 0, syavg = 0, sfavg = 0;

    for (int i = 0; i < int(model.Vertices.size()); i++){
        sxLowest = (model.Vertices[i].Position.X < sxLowest) ? model.Vertices[i].Position.X : sxLowest; 
        sxHighest = (model.Vertices[i].Position.X > sxHighest) ? model.Vertices[i].Position.X : sxHighest;

        syLowest = (model.Vertices[i].Position.Y < syLowest) ? model.Vertices[i].Position.Y : syLowest; 
        syHighest = (model.Vertices[i].Position.Y > syHighest) ? model.Vertices[i].Position.Y : syHighest; 
    }

    sxavg = 1920 / (abs(sxLowest) + abs(sxHighest));
    syavg = 1080 / (abs(syLowest) + abs(syHighest));

    sfavg = (sxavg < syavg) ? sxavg -5 : syavg -5;
    
    cout << "Scaling model with " << sfavg << " factor..." << endl;
    for (int i = 0; i < int(model.Vertices.size()); i++){
        performScale(model.Vertices[i].Position, sfavg, sfavg);
    }

    for(int i = 0; i < int(model.Faces.size()); i++){
        for(int j = 0; j < int(model.Faces[i].Vertices.size()); j++){
            performScale(model.Faces[i].Vertices[j].Position, sfavg, sfavg);
        }
    }
}

void translate(Mesh &model, float x, float y){
    float sxLowest = 0.0f, sxHighest = 0.0f;
    float syLowest = 0.0f, syHighest = 0.0f;

    for (int i = 0; i < int(model.Vertices.size()); i++){
        sxLowest = (model.Vertices[i].Position.X < sxLowest) ? model.Vertices[i].Position.X : sxLowest; 
        sxHighest = (model.Vertices[i].Position.X > sxHighest) ? model.Vertices[i].Position.X : sxHighest;

        syLowest = (model.Vertices[i].Position.Y < syLowest) ? model.Vertices[i].Position.Y : syLowest; 
        syHighest = (model.Vertices[i].Position.Y > syHighest) ? model.Vertices[i].Position.Y : syHighest; 
    }

    cout << "Translating model to (" << x << ", " << y << ")" << endl;
    for (int i = 0; i < int(model.Vertices.size()); i++){
        // Translate the vertices
        performTranslate(model.Vertices[i].Position, abs(sxLowest) + x, abs(syLowest) + y);
    }

    for(int i = 0; i < int(model.Faces.size()); i++){
        for(int j = 0; j < int(model.Faces[i].Vertices.size()); j++){
            performTranslate(model.Faces[i].Vertices[j].Position, abs(sxLowest) + x, abs(syLowest) + y);
        }
    }
}

void rotate(Mesh &model, int n, double beta) { 
    double cr = COS(beta);
	double sr = SIN(beta);

	double matrix[4][4] = {
		{1, 0, 0, 0},
		{0, cr, -sr, 0},
		{0, sr, cr, 0},
		{0, 0, 0, 1}
	};

	applyMatrix(model.Vertices, n, matrix);
    for(int i = 0; i < int(model.Faces.size()); i++){
        applyMatrix(model.Faces[i].Vertices, n, matrix);
    }
} 