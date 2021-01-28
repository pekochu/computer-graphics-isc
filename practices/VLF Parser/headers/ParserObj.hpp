#include "Raster.hpp"

using namespace std;

// Rotations
#define PI 3.14159265359
#define SIN(x) sin(x * PI/180) 
#define COS(x) cos(x * PI/180) 


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

    // Variable Set Constructor
    Vector3(float X_, float Y_, float Z_){
        X = X_;
        Y = Y_;
        Z = Z_;
        W = 0;
    }

    // Bool Equals Operator Overload
    bool operator==(const Vector3& other) const
    {
        return (this->X == other.X && this->Y == other.Y && this->Z == other.Z);
    }
    // Bool Not Equals Operator Overload
    bool operator!=(const Vector3& other) const
    {
        return !(this->X == other.X && this->Y == other.Y && this->Z == other.Z);
    }
    // Addition Operator Overload
    Vector3 operator+(const Vector3& right) const
    {
        return Vector3(this->X + right.X, this->Y + right.Y, this->Z + right.Z);
    }
    // Subtraction Operator Overload
    Vector3 operator-(const Vector3& right) const
    {
        return Vector3(this->X - right.X, this->Y - right.Y, this->Z - right.Z);
    }
    // Float Multiplication Operator Overload
    Vector3 operator*(const float& other) const
    {
        return Vector3(this->X * other, this->Y * other, this->Z * other);
    }
    // Float Division Operator Overload
    Vector3 operator/(const float& other) const
    {
        return Vector3(this->X / other, this->Y / other, this->Z / other);
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
    int index;
    Vector3 Position;
};

// Structure: Facesx
//
// Description: Model Facesx for OBJ model
struct Facesx{
    vector<Vertex> Vertices;
};

// Structure: Edge
//
// Description: Model Edge object for VLF model
struct Edge{
    int index;
    Vertex P0;
    Vertex P1;
};

// Structure: Face
//
// Description: Model Face object for VLF model
struct Face{
    Edge E0;
    Edge E1;
    Edge E2;
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

// Structure: VLF
//
// Description: transformation of the
// mesh struct
struct VLF {

    // Default Constructor
    VLF();

    // Copy constructor
    VLF(const VLF &obj);

    // Model Name
    std::string ModelName;
    // Vertex List
    std::vector<Vertex> Vertices;
    // Vertex List
    std::vector<Edge> Edges;
    // Faces list
    std::vector<Face> Faces;
};

// parse .OBJ file
void readObj(ifstream&, Mesh&);
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

// Vector3 Multiplication Opertor Overload
Vector3 operator*(const float& left, const Vector3& right);

// A test to see if P1 is on the same side as P2 of a line segment ab
bool SameSide(Vector3 p1, Vector3 p2, Vector3 a, Vector3 b);

// Generate a cross produect normal for a triangle
Vector3 GenTriNormal(Vector3 t1, Vector3 t2, Vector3 t3);

// Check to see if a Vector3 Point is within a 3 Vector3 Triangle
bool inTriangle(Vector3 point, Vector3 tri1, Vector3 tri2, Vector3 tri3);

// Vector3 Cross Product
Vector3 CrossV3(const Vector3 a, const Vector3 b);

// Vector3 Magnitude Calculation
float MagnitudeV3(const Vector3 in);

// Vector3 DotProduct
float DotV3(const Vector3 a, const Vector3 b);

// Angle between 2 Vector3 Objects
float AngleBetweenV3(const Vector3 a, const Vector3 b);

// Projection Calculation of a onto b
Vector3 ProjV3(const Vector3 a, const Vector3 b);

// Triangulate a list of vertices into a face by printing
//	inducies corresponding with triangles within it
void VertexTriangluation(vector<Vertex> &, vector<Facesx> &, int &idx,
    const vector<Vertex>&);

// parse to vlf
void parseToVlf(Mesh &, Mesh &);