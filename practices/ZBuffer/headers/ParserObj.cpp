#include "ParserObj.hpp"

int VIEW_FLAG = XY_VIEW;

/* .........................................
 * Mesh Struct Methods Implementations
 * ......................................... */

Mesh::Mesh(){ /* CONSTRUCTOR */ }

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

/* .........................................
 * VLF Struct Methods Implementations
 * ......................................... */

VLF::VLF(){ /* CONSTRUCTOR */ }

VLF::VLF(const VLF &obj) {

    for (int i = 0; i < int(obj.Vertices.size()); i++){
        Vertices.push_back(obj.Vertices[i]);
    }

    for(int i = 0; i < int(obj.Edges.size()); i++){
        Edges.push_back(obj.Edges[i]);
    }

    for(int i = 0; i < int(obj.Faces.size()); i++){
        Faces.push_back(obj.Faces[i]);
    }

    ModelName = obj.ModelName;
};

void readObj(ifstream &file, Mesh& model){
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

void readVlf(ifstream& file, VLF& model){
    vector<Vertex> Vertices;
    vector<Edge> Edges;
    vector<Face> Faces;

    int sizeVerts, sizeEdges, sizeFaces;
    file.read((char *) &sizeVerts, sizeof(int));
    file.read((char *) &sizeEdges, sizeof(int));
    file.read((char *) &sizeFaces, sizeof(int));

    
    for (int i = 0; i < sizeVerts; i++){
        Vertex tVertex;
        tVertex.index = i;
        file.read((char *) &tVertex.Position.X, sizeof(float));
        file.read((char *) &tVertex.Position.Y, sizeof(float));
        file.read((char *) &tVertex.Position.Z, sizeof(float));

        Vertices.push_back(tVertex);
    }

    for (int i = 0; i < sizeEdges; i++){
        Edge tEdge;
        
        int idx0 = 0, idx1 = 0;
        file.read((char *) &idx0, sizeof(int));
        file.read((char *) &idx1, sizeof(int));

        tEdge.index = i;
        tEdge.P0 = Vertices[idx0];
        tEdge.P1 = Vertices[idx1];

        Edges.push_back(tEdge);
    }

    for (int i = 0; i < sizeEdges; i++){
        Face tFace;
        
        int idx0 = 0, idx1 = 0, idx2 = 0;
        file.read((char *) &idx0, sizeof(int));
        file.read((char *) &idx1, sizeof(int));
        file.read((char *) &idx2, sizeof(int));

        tFace.E0 = Edges[idx0];
        tFace.E1 = Edges[idx1];
        tFace.E2 = Edges[idx2];

        Faces.push_back(tFace);
    }

    model.Vertices = Vertices;
    model.Edges = Edges;
    model.Faces = Faces;
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

    // cout << "Translating model to (" << x << ", " << y << ", " << z << ")" << endl;
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
        matrix[1][2] = (sr)*(-1);
        matrix[2][1] = sr;
        matrix[2][2] = cr;
    }else if(ROTATE_FLAG == ROTATE_ON_Y){
        matrix[0][0] = cr;
        matrix[0][2] = sr;
        matrix[2][0] = (sr)*(-1);
        matrix[2][2] = cr;
    }else if(ROTATE_FLAG == ROTATE_ON_Z){
        matrix[0][1] = cr;
        matrix[0][1] = (sr)*(-1);
        matrix[1][0] = sr;
        matrix[1][1] = cr;
    }else{
        matrix[1][1] = cr;
        matrix[1][2] = (sr)*(-1);
        matrix[2][1] = sr;
        matrix[2][2] = cr;
    }

	applyMatrix(model.Vertices, matrix);
    for(int i = 0; i < int(model.Faces.size()); i++){
        applyMatrix(model.Faces[i].Vertices, matrix);
    }
} 

// Vector3 Multiplication Opertor Overload
Vector3 operator*(const float& left, const Vector3& right){
    return Vector3(right.X * left, right.Y * left, right.Z * left);
}

// A test to see if P1 is on the same side as P2 of a line segment ab
bool SameSide(Vector3 p1, Vector3 p2, Vector3 a, Vector3 b){
    Vector3 cp1 = CrossV3(b - a, p1 - a);
    Vector3 cp2 = CrossV3(b - a, p2 - a);

    if (DotV3(cp1, cp2) >= 0)
        return true;
    else
        return false;
}

// Generate a cross produect normal for a triangle
Vector3 GenTriNormal(Vector3 t1, Vector3 t2, Vector3 t3){
    Vector3 u = t2 - t1;
    Vector3 v = t3 - t1;

    Vector3 normal = CrossV3(u,v);

    return normal;
}

// Check to see if a Vector3 Point is within a 3 Vector3 Triangle
bool inTriangle(Vector3 point, Vector3 tri1, Vector3 tri2, Vector3 tri3){
    // Test to see if it is within an infinite prism that the triangle outlines.
    bool within_tri_prisim = SameSide(point, tri1, tri2, tri3) && SameSide(point, tri2, tri1, tri3)
        && SameSide(point, tri3, tri1, tri2);

    // If it isn't it will never be on the triangle
    if (!within_tri_prisim)
        return false;

    // Calulate Triangle's Normal
    Vector3 n = GenTriNormal(tri1, tri2, tri3);

    // Project the point onto this normal
    Vector3 proj = ProjV3(point, n);

    // If the distance from the triangle to the point is 0
    //	it lies on the triangle
    if (MagnitudeV3(proj) == 0)
        return true;
    else
        return false;
}

// Vector3 Cross Product
Vector3 CrossV3(const Vector3 a, const Vector3 b){
    return Vector3(a.Y * b.Z - a.Z * b.Y,
        a.Z * b.X - a.X * b.Z,
        a.X * b.Y - a.Y * b.X);
}

// Vector3 Magnitude Calculation
float MagnitudeV3(const Vector3 in){
    return (sqrtf(powf(in.X, 2) + powf(in.Y, 2) + powf(in.Z, 2)));
}

// Vector3 DotProduct
float DotV3(const Vector3 a, const Vector3 b){
    return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
}

// Angle between 2 Vector3 Objects
float AngleBetweenV3(const Vector3 a, const Vector3 b){
    float angle = DotV3(a, b);
    angle /= (MagnitudeV3(a) * MagnitudeV3(b));
    return angle = acosf(angle);
}

// Projection Calculation of a onto b
Vector3 ProjV3(const Vector3 a, const Vector3 b){
    Vector3 bn = b / MagnitudeV3(b);
    return bn * DotV3(a, bn);
}

// Triangulate a list of vertices into a face by printing
//	inducies corresponding with triangles within it
void VertexTriangluation(vector<Vertex> &Vertices, vector<Facesx> &Faces, int &idx,
    const vector<Vertex> &meshVerts){
    // If there are 2 or less verts,
    // no triangle can be created,
    // so exit
    if (meshVerts.size() < 3) {
        return;
    }
    // If it is a triangle no need to calculate it
    if (meshVerts.size() == 3) {
        Facesx face;
        face.Vertices = meshVerts;

        // asign index number
        for(int k = 0; k < 3; k++){
            face.Vertices[k].index = idx;
            idx++;
        }
        // push to the vertices vector
        Faces.push_back(face);

        // push to the faces vector
        for(int k = 0; k < 3; k++) Vertices.push_back(face.Vertices[k]);
        return;
    }

    // Create a list of vertices
    vector<Vertex> tVerts = meshVerts;
    vector<Facesx> tFaces;
    while (true) {
        // For every vertex
        for (int i = 0; i < int(tVerts.size()); i++) {
            // pPrev = the previous vertex in the list
            Vertex pPrev;
            if (i == 0) {
                pPrev = tVerts[tVerts.size() - 1];
            } else {
                pPrev = tVerts[i - 1];
            }

            // pCur = the current vertex;
            Vertex pCur = tVerts[i];

            // pNext = the next vertex in the list
            Vertex pNext;
            if (i == tVerts.size() - 1) {
                pNext = tVerts[0];
            } else {
                pNext = tVerts[i + 1];
            }

            // Check to see if there are only 3 verts left
            // if so this is the last triangle
            if (tVerts.size() == 3) {
                // Create a triangle from pCur, pPrev, pNext
                Facesx face;
                for (int j = 0; j < int(tVerts.size()); j++) {
                    if (meshVerts[j].Position == pCur.Position)
                        face.Vertices.push_back(meshVerts[j]);
                    if (meshVerts[j].Position == pPrev.Position)
                        face.Vertices.push_back(meshVerts[j]);
                    if (meshVerts[j].Position == pNext.Position)
                        face.Vertices.push_back(meshVerts[j]);
                }
                tFaces.push_back(face);
                
                tVerts.clear();
                break;
            }

            if (tVerts.size() == 4) {
                // Create a triangle from pCur, pPrev, pNext
                Facesx face;
                for (int j = 0; j < int(meshVerts.size()); j++) {
                    if (meshVerts[j].Position == pCur.Position)
                        face.Vertices.push_back(meshVerts[j]);
                    if (meshVerts[j].Position == pPrev.Position)
                        face.Vertices.push_back(meshVerts[j]);
                    if (meshVerts[j].Position == pNext.Position)
                        face.Vertices.push_back(meshVerts[j]);
                }
                tFaces.push_back(face);


                Vector3 tempVec;
                for (int j = 0; j < int(tVerts.size()); j++)
                {
                    if (tVerts[j].Position != pCur.Position
                        && tVerts[j].Position != pPrev.Position
                        && tVerts[j].Position != pNext.Position)
                    {
                        tempVec = tVerts[j].Position;
                        break;
                    }
                }

                // Create a triangle from pCur, pPrev, pNext
                face.Vertices.clear();
                for (int j = 0; j < int(meshVerts.size()); j++)
                {
                    if (meshVerts[j].Position == pPrev.Position)
                        face.Vertices.push_back(meshVerts[j]);
                    if (meshVerts[j].Position == pNext.Position)
                        face.Vertices.push_back(meshVerts[j]);
                    if (meshVerts[j].Position == tempVec)
                        face.Vertices.push_back(meshVerts[j]);
                }
                tFaces.push_back(face);

                tVerts.clear();
                break;
            }

            // If Vertex is not an interior vertex
            float angle = AngleBetweenV3(pPrev.Position - pCur.Position, pNext.Position - pCur.Position) * (180 / PI);
            if (angle <= 0 && angle >= 180)
                continue;

            // If any vertices are within this triangle
            bool inTri = false;
            for (int j = 0; j < int(meshVerts.size()); j++)
            {
                if (inTriangle(meshVerts[j].Position, pPrev.Position, pCur.Position, pNext.Position)
                    && meshVerts[j].Position != pPrev.Position
                    && meshVerts[j].Position != pCur.Position
                    && meshVerts[j].Position != pNext.Position)
                {
                    inTri = true;
                    break;
                }
            }
            if (inTri)
                continue;

            // Create a triangle from pCur, pPrev, pNext
            Facesx face;
            for (int j = 0; j < int(meshVerts.size()); j++) {
                if (meshVerts[j].Position == pCur.Position)
                    face.Vertices.push_back(meshVerts[j]);
                if (meshVerts[j].Position == pPrev.Position)
                    face.Vertices.push_back(meshVerts[j]);
                if (meshVerts[j].Position == pNext.Position)
                    face.Vertices.push_back(meshVerts[j]);
            }
            tFaces.push_back(face);

            // Delete pCur from the list
            for (int j = 0; j < int(tVerts.size()); j++) {
                if (tVerts[j].Position == pCur.Position) {
                    tVerts.erase(tVerts.begin() + j);
                    break;
                }
            }

            // reset i to the start
            // -1 since loop will add 1 to it
            i = -1;
        }

        // if no triangles were created
        if (tFaces.size() == 0) break;
        else{
            for(int k = 0; k < tFaces.size(); k++){
                // asign index number
                for(int l = 0; l < 3; l++){
                    tFaces[k].Vertices[l].index = idx;
                    idx++;
                }

                Faces.push_back(tFaces[k]);

                for(int l = 0; l < 3; l++) Vertices.push_back(tFaces[k].Vertices[l]);
            }
        } 

        // if no more vertices
        if (tVerts.size() == 0)
            break;
    }
}

void parseToVlf(Mesh &mVlf, Mesh &vMesh){
    mVlf.MeshName = vMesh.MeshName;
    int idx = 0;

    for(int i = 0; i < vMesh.Faces.size(); i++){
        VertexTriangluation(mVlf.Vertices, mVlf.Faces, idx, vMesh.Faces[i].Vertices);
    }
}

void createVLF(Mesh &mesh, VLF &model){
    // Calculate edges
    cout << "Generating edges..." << endl;
    int eidx = 0;
    vector<Edge> edges;
    for(int i = 0; i < mesh.Faces.size(); i++){
        Edge mEdge;

        mEdge.P0 = mesh.Faces[i].Vertices[0];
        mEdge.P1 = mesh.Faces[i].Vertices[1];
        mEdge.index = eidx++;
        edges.push_back(mEdge);

        mEdge.P0 = mesh.Faces[i].Vertices[1];
        mEdge.P1 = mesh.Faces[i].Vertices[2];
        mEdge.index = eidx++;
        edges.push_back(mEdge);

        mEdge.P0 = mesh.Faces[i].Vertices[2];
        mEdge.P1 = mesh.Faces[i].Vertices[0];
        mEdge.index = eidx++;
        edges.push_back(mEdge);
    }

    cout << "Generating new faces..." << endl;        
    vector<Face> faces;
    for(int i = 0; i < edges.size(); i++){
        Face mFace;

        mFace.E0 = edges[i];
        mFace.E1 = edges[++i];
        mFace.E2 = edges[++i];

        faces.push_back(mFace);
    }

    model.Vertices = mesh.Vertices;
    model.Edges = edges;
    model.Faces = faces;
}