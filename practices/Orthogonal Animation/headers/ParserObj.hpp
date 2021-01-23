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

// Rotation enumeration on X, Y and Z
enum rotateFlags {
    ROTATE_ON_X = 1,
    ROTATE_ON_Y = 2,
    ROTATE_ON_Z = 3
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
    Mesh();

    // Variable Set Constructor
    Mesh(std::vector<Vertex>& _Vertices);

    // Copy constructor
    Mesh(const Mesh &obj);

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
// helper functions to transformations
void getMaxMins(vector<Vertex>&, float matrix[10]);
// functions to perform rotations, translations, scaletions
void applyMatrix(vector<Vertex>&, float matrix[4][4]);
void scale(Mesh &, int);
void translate(Mesh &, float, float, float);
void focalTranslation(Mesh &, float, float, float, float);
void translation(Mesh &, float, float, float);
void transform3D(Mesh &);
void rotate(Mesh &, float, int);