#include "Raster.hpp"

using namespace std;

// Rotations
#define SIN(x) sin(x * 3.141592653589/180) 
#define COS(x) cos(x * 3.141592653589/180) 

// Enumarations for setting XY, XZ, YZ
enum viewsFlags {
    XY_VIEW = 1,
    XZ_VIEW = 3,
    YZ_VIEW = 5
};

extern int VIEW_FLAG;

// Structure: Vector3
//
// Description: A 3D Vector that Holds Positional Data
struct Vector3{
    // Default Constructor
    Vector3() {
        X = 0.0f;
        Y = 0.0f;
        Z = 0.0f;
        W = 0.0f;
    }
    // Variable Set Constructor
    Vector3(float X_, float Y_, float Z_, float W_){
        X = X_;
        Y = Y_;
        Z = Z_;
        W = W_;
    }

    // Positional Variables
    float X;
    float Y;
    float Z;
    float W;
};

// Structure: Vertex
//
// Description: Model Vertex object that holds positions only
struct Vertex {
    // Position Vector
    Vector3 Position;
};

struct Facesx{
    vector<Vertex> Vertices;
};

// Structure: Mesh
//
// Description: A Simple Mesh Object that holds
//	a name, a vertex list, and an index list
struct Mesh {
    // Default Constructor
    Mesh(){

    }
    // Variable Set Constructor
    Mesh(std::vector<Vertex>& _Vertices) {
        Vertices = _Vertices;
    }
    // Mesh Name
    std::string MeshName;
    // Vertex List
    std::vector<Vertex> Vertices;
    // Faces list
    std::vector<Facesx> Faces;
};

// parse .OBJ file
void readObj(ifstream&, Raster*, Mesh&);
void split(const string&, vector<string>&, string);
void getVertices(vector<Facesx> &, vector<Vertex> &, const vector<Vector3>&, string);
string tail(const string&);
string firstToken(const string&);
// Transformation algorithms
void findNewCoordinate(int [][2], float [][1]);
void performScale(Vector3 &, int, int);
void performTranslate(Vector3 &, float, float);
void performRotate(vector<Vertex> &, int, int, int, int);
void applyMatrix(Vector3 &, int n, double matrix[4][4]);
// Functions to transformation
void scale(Mesh &, int);
void translate(Mesh &, float, float);
void rotate(Mesh &, int, double);