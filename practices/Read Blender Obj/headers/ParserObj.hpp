#include "Raster.hpp"

using namespace std;

#define SIN(x) sin(x * 3.141592653589/180) 
#define COS(x) cos(x * 3.141592653589/180) 

// Structure: Vector3
//
// Description: A 3D Vector that Holds Positional Data
struct Vector3{
    // Default Constructor
    Vector3() {
        X = 0.0f;
        Y = 0.0f;
        Z = 0.0f;
    }
    // Variable Set Constructor
    Vector3(float X_, float Y_, float Z_){
        X = X_;
        Y = Y_;
        Z = Z_;
    }
    // Bool Equals Operator Overload
    bool operator==(const Vector3& other) const {
        return (this->X == other.X && this->Y == other.Y && this->Z == other.Z);
    }
    // Bool Not Equals Operator Overload
    bool operator!=(const Vector3& other) const {
        return !(this->X == other.X && this->Y == other.Y && this->Z == other.Z);
    }
    // Addition Operator Overload
    Vector3 operator+(const Vector3& right) const {
        return Vector3(this->X + right.X, this->Y + right.Y, this->Z + right.Z);
    }
    // Subtraction Operator Overload
    Vector3 operator-(const Vector3& right) const {
        return Vector3(this->X - right.X, this->Y - right.Y, this->Z - right.Z);
    }
    // Float Multiplication Operator Overload
    Vector3 operator*(const float& other) const {
        return Vector3(this->X * other, this->Y * other, this->Z * other);
    }
    // Float Division Operator Overload
    Vector3 operator/(const float& other) const {
        return Vector3(this->X / other, this->Y / other, this->Z / other);
    }

    // Positional Variables
    float X;
    float Y;
    float Z;
};

// Structure: Vertex
//
// Description: Model Vertex object that holds positions only
struct Vertex {
    // Position Vector
    Vector3 Position;
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
};

// parse .OBJ file
void readObj(ifstream&, Raster*);
void split(const string&, vector<string>&, string);
void getVertices(vector<Vertex> &, const vector<Vector3>&, string);
string tail(const string&);
string firstToken(const string&);
// Homogeneus space algorithms
void findNewCoordinate(int [][2], float [][1]);
void scale(Vector3 &, int, int);
void translate(Vector3 &, float, float);
void rotate(vector<Vertex> &, int, int, int, int);