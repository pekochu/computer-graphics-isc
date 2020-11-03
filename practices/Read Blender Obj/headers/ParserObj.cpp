#include "ParserObj.hpp"

void readObj(ifstream &file, Raster *mRaster){
    vector<Vector3> Positions;
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    string line;
    while(getline(file, line)){
        // get vertix
        if (firstToken(line) == "v"){
            vector<string> spos;
            Vector3 vertices;
            split(tail(line), spos, " ");

            vertices.X = stof(spos[0]);
            vertices.Y = stof(spos[1]);
            vertices.Z = stof(spos[2]);

            // cout << "X: " << stof(spos[0]) << "\t| Y: " << stof(spos[1]) << "\t| Z: " << stof(spos[2]) << endl;
            Positions.push_back(vertices);
        }

        if(firstToken(line) == "f"){
            vector<Vertex> vVerts;            
            getVertices(vVerts, Positions, line);

            for (int i = 0; i < int(vVerts.size()); i++){
                vertices.push_back(vVerts[i]);
            }
        }
    }

    float sxLowest = 0.0f, sxHighest = 0.0f;
    float syLowest = 0.0f, syHighest = 0.0f;
    int sxavg = 0, syavg = 0, sfavg = 0;

    for (int i = 0; i < int(vertices.size()); i++){
        sxLowest = (vertices[i].Position.X < sxLowest) ? vertices[i].Position.X : sxLowest; 
        sxHighest = (vertices[i].Position.X > sxHighest) ? vertices[i].Position.X : sxHighest;

        syLowest = (vertices[i].Position.Y < syLowest) ? vertices[i].Position.Y : syLowest; 
        syHighest = (vertices[i].Position.Y > syHighest) ? vertices[i].Position.Y : syHighest; 
    }

    sxavg = 1920 / (abs(sxLowest) + abs(sxHighest));
    syavg = 1080 / (abs(syLowest) + abs(syHighest));

    sfavg = (sxavg < syavg) ? sxavg -3 : syavg -3;
    
    cout << "Scale model with " << sfavg << " factor..." << endl;
    for (int i = 0; i < int(vertices.size()); i++){
        // translate the vertices
        scale(vertices[i].Position, sfavg, sfavg);
    }

    for (int i = 0; i < int(vertices.size()); i++){
        sxLowest = (vertices[i].Position.X < sxLowest) ? vertices[i].Position.X : sxLowest; 
        sxHighest = (vertices[i].Position.X > sxHighest) ? vertices[i].Position.X : sxHighest;

        syLowest = (vertices[i].Position.Y < syLowest) ? vertices[i].Position.Y : syLowest; 
        syHighest = (vertices[i].Position.Y > syHighest) ? vertices[i].Position.Y : syHighest; 
    }

    cout << "Translate model..." << endl;
    for (int i = 0; i < int(vertices.size()); i++){
        // Translate the vertices
        translate(vertices[i].Position, abs(sxLowest) + 5, abs(syLowest) + 5);
    }

    // print 
    unsigned char r = 0x00;
    unsigned char g = 0x79;
    unsigned char b = 0x6b;
    for (int i = 0; i < int(vertices.size()) - 1; i += 2){
        // Scale the vertices
        // cout << "X1 = " << vertices[i].Position.X << "\tY1 = " << vertices[i].Position.Y;
        // cout << " to X2 = " << vertices[i+1].Position.X << "\tY2 = " << vertices[i+1].Position.Y << endl;
        mRaster->drawLine(nearbyint(vertices[i].Position.X), nearbyint(vertices[i].Position.Y),
                nearbyint(vertices[i+1].Position.X), nearbyint(vertices[i+1].Position.Y), r, g, b);
    }
}

void split(const std::string &in,
			std::vector<std::string> &out,
			std::string token) {
    // here
    out.clear();

    std::string temp;

    for (int i = 0; i < int(in.size()); i++){
        std::string test = in.substr(i, token.size());

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

void getVertices(vector<Vertex> &mVert, const vector<Vector3>& mPositions, string line){
    vector<string> sface, svert;
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
    }    
}

std::string tail(const std::string &in) {
    size_t token_start = in.find_first_not_of(" \t");
    size_t space_start = in.find_first_of(" \t", token_start);
    size_t tail_start = in.find_first_not_of(" \t", space_start);
    size_t tail_end = in.find_last_not_of(" \t");
    if (tail_start != std::string::npos && tail_end != std::string::npos) {
        return in.substr(tail_start, tail_end - tail_start + 1);
    } else if (tail_start != std::string::npos){
        return in.substr(tail_start);
    }
    return "";
}

// Get first token of string
std::string firstToken(const std::string &in) {
    if (!in.empty()) {
        size_t token_start = in.find_first_not_of(" \t");
        size_t token_end = in.find_first_of(" \t", token_start);
        if (token_start != std::string::npos && token_end != std::string::npos){
            return in.substr(token_start, token_end - token_start);
        } else if (token_start != std::string::npos) {
            return in.substr(token_start);
        }
    }
    return "";
}

// Homegeneus space algorithms
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
void scale(Vector3 &position, int sx, int sy) {   
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

void translate (Vector3 &position, float xf, float yf) {   
    // calculating translated coordinates 
    position.X += xf; 
    position.Y += yf; 
} 

void rotate(vector<Vertex> &vertices, int n, int x_pivot, int y_pivot, int angle) { 
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