/*
Vector class -- 3D Vector class developed to handle linear algebra
Developed by: Travis Stewart
*/

#include "Util.h"

#ifndef VECTOR_H
#define VECTOR_H

using namespace std;

class Vector {
public:
	float x, y, z;
	int idx;

	~Vector() {}
	Vector() { x = y = z = 0.0f; }
	Vector(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
	Vector(float X, float Y) : x(X), y(Y), z(0.0f) {}
	Vector(float X) : x(X), y(0.0f), z(0.0f) {}

	//Overloaded Operators
	inline void operator = (const float& f) { x = f; y = f; z = f; }

	inline void operator += (const Vector& v) { x += v.x; y += v.y; z += v.z; }
	inline void operator -= (const Vector& v) { x -= v.x; y -= v.y; z -= v.z; }
	inline void operator *= (const Vector& v) { x *= v.x; y *= v.y; z *= v.z; }
	inline void operator /= (const Vector& v) { x /= v.x; y /= v.y; z /= v.z; }

	inline Vector operator + (const Vector& v) { return Vector(x + v.x, y + v.y, z + v.z); }
	inline Vector operator - (const Vector& v) { return Vector(x - v.x, y - v.y, z - v.z); }
	inline Vector operator * (const Vector& v) { return Vector(x * v.x, y * v.y, z * v.z); }
	inline Vector operator / (const Vector& v) { return Vector(x / v.x, y / v.y, z / v.z); }

	inline void operator += (const float& f) { x += f; y += f; z += f; }
	inline void operator -= (const float& f) { x -= f; y -= f; z -= f; }
	inline void operator *= (const float& f) { x *= f; y *= f; z *= f; }
	inline void operator /= (const float& f) { x /= f; y /= f; z /= f; }


	inline Vector operator + (const float& f) { return Vector(x + f, y + f, z + f); }
	inline Vector operator - (const float& f) { return Vector(x - f, y - f, z - f); }
	inline Vector operator * (const float& f) { return Vector(x * f, y * f, z * f); }
	inline Vector operator / (const float& f) { return Vector(x / f, y / f, z / f); }

	friend Vector operator + (const float& f, const Vector& v)	{ return Vector(f + v.x, f + v.y, f + v.z); }
	friend Vector operator - (const float& f, const Vector& v)	{ return Vector(f - v.x, f - v.y, f - v.z); }
	friend Vector operator * (const float& f, const Vector& v)	{ return Vector(f * v.x, f * v.y, f * v.z); }
	friend Vector operator / (const float& f, const Vector& v)	{ return Vector(f / v.x, f / v.y, f / v.z); }


	inline bool operator == (const Vector& v) { return x == v.x && y == v.y && z == v.z; }
	inline bool operator <  (const Vector& v) { return x < v.x && y < v.y && z < v.z; }
	inline bool operator >  (const Vector& v) { return x > v.x && y > v.y && z > v.z; }
	inline bool operator <= (const Vector& v) { return x <= v.x && y <= v.y && z <= v.z; }
	inline bool operator >= (const Vector& v) { return x >= v.x && y >= v.y && z >= v.z; }
	inline bool operator != (const Vector& v) { return x != v.x && y != v.y && z != v.z; }

	inline bool operator == (const Vector& v) const { return x == v.x && y == v.y && z == v.z; }
	inline bool operator <  (const Vector& v) const { return x < v.x && y < v.y && z < v.z; }
	inline bool operator >  (const Vector& v) const { return x > v.x && y > v.y && z > v.z; }
	inline bool operator <= (const Vector& v) const { return x <= v.x && y <= v.y && z <= v.z; }
	inline bool operator >= (const Vector& v) const { return x >= v.x && y >= v.y && z >= v.z; }
	inline bool operator != (const Vector& v) const { return x != v.x && y != v.y && z != v.z; }


	inline bool operator == (const float& v) { return x == v && y == v && z == v; }
	inline bool operator <  (const float& v) { return x < v && y < v && z < v; }
	inline bool operator >  (const float& v) { return x > v && y > v && z > v; }
	inline bool operator <= (const float& v) { return x <= v && y <= v && z <= v; }
	inline bool operator >= (const float& v) { return x >= v && y >= v && z >= v; }
	inline bool operator != (const float& v) { return x != v && y != v && z != v; }

	inline bool operator == (const float& v) const { return x == v && y == v && z == v; }
	inline bool operator <  (const float& v) const { return x < v && y < v && z < v; }
	inline bool operator >  (const float& v) const { return x > v && y > v && z > v; }
	inline bool operator <= (const float& v) const { return x <= v && y <= v && z <= v; }
	inline bool operator >= (const float& v) const { return x >= v && y >= v && z >= v; }
	inline bool operator != (const float& v) const { return x != v && y != v && z != v; }

	inline string info() { return to_string(x) + ", " + to_string(y) + ", " + to_string(z); }
	inline bool isZero() { return x == 0.0f && y == 0.0f && z == 0.0f; }
	
	float* toArray() { float rtn[3] = {x, y, z}; return rtn; }		//TODO: FIX THIS FUNCTION. IT SEEMS TO BE BROKEN.
	
	//Returns a normalized/unit Vector of the base Vector
	inline Vector unit() {	return Vector(x / sqrt(powf(x, 2.0f) + powf(y, 2.0f) + powf(z, 2.0f)), y / sqrt(powf(x, 2.0f) + powf(y, 2.0f) + powf(z, 2.0f)), z / sqrt(powf(x, 2.0f) + powf(y, 2.0f) + powf(z, 2.0f))); }

	inline float magnitude() { return sqrt(powf(x, 2.0f) + powf(y, 2.0f) + powf(z, 2.0f)); }

	//Same as unit(), but saves the changes to the base Vector
	inline void normalize() { x /= sqrt(powf(x, 2.0f) + powf(y, 2.0f) + powf(z, 2.0f)); y /= sqrt(powf(x, 2.0f) + powf(y, 2.0f) + powf(z, 2.0f));  z /= sqrt(powf(x, 2.0f) + powf(y, 2.0f) + powf(z, 2.0f)); }	

	inline void clear() { x = y = z = 0.0f; }

	inline void limit(float limit) {
		//Limit positive values
		x = (x <= limit)? x : limit;
		y = (y <= limit)? y : limit;
		z = (z <= limit)? z : limit;

		//Limit negative values
		x = (x >= -limit)? x : -limit;
		y = (y >= -limit)? y : -limit;
		z = (z >= -limit)? z : -limit;
	}

	inline Vector reflect(Vector n) {
		n.normalize();
		float dot = x * n.x + y * n.y + z * n.z;
		n *= 2.0f * dot;
		return Vector(x - n.x, y - n.y, z - n.z);
	}

	inline float getDistance(Vector pt) { return sqrt(powf((pt.x - x), 2) + powf((pt.y - y), 2) + powf((pt.z - z), 2)); }
	inline Vector getDirectionTo(Vector v) { return Vector(v.x - x, v.y - y, v.z - z); }

	inline Vector pow(float e) { return Vector(powf(x, e), powf(y, e), powf(z, e)); }

};

//Custom Hash function for the Vector class
namespace std
{
	template<> struct hash<Vector>
	{
		uint64_t operator()(Vector const& v) const noexcept
		{
			uint64_t const X(std::hash<float>{}(v.x));
			uint64_t const Y(std::hash<float>{}(v.y));
			uint64_t const Z(std::hash<float>{}(v.z));

			return ((X ^ (Y << 1)) >> 1) ^ (Z << 1);
		}
	};
}


//=========================================================================================================================
//-----------------------------------------------General Vector Functions--------------------------------------------------
//=========================================================================================================================

inline Vector normal(const Vector& v1, const Vector& v2, const Vector& v3) {
	//Create two vectors to perform cross product with
	Vector v(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
	Vector w(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);

	//Cross product VxW and return result
	return Vector((v.y*w.z - v.z*w.y), (v.z*w.x - v.x*w.z), (v.x*w.y - v.y*w.x));
}

inline float dot(const Vector& v1, const Vector& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

inline float getAngle(Vector v1, Vector v2) { return dot(v1.unit(), v2.unit()); }

inline Vector cross(const Vector& v, const Vector& w) { return Vector((v.y*w.z - v.z*w.y), (v.z*w.x - v.x*w.z), (v.x*w.y - v.y*w.x)); }

inline Vector unitVector(float theta) { return Vector(cos(theta), sin(theta), tan(theta)); }

inline Vector abs(const Vector& v) { return Vector(abs(v.x), abs(v.y), abs(v.z)); }

inline bool isnan(const Vector& v) { return isnan(v.x) || isnan(v.y) || isnan(v.z); }

// Sort() functions for Vectors
bool sortX(const Vector &v1, const Vector &v2) { return v1.x < v2.x; }
bool sortY(const Vector &v1, const Vector &v2) { return v1.y < v2.y; }
bool sortZ(const Vector &v1, const Vector &v2) { return v1.z < v2.z; }

bool rsortX(const Vector &v1, const Vector &v2) { return v1.x > v2.x; }
bool rsortY(const Vector &v1, const Vector &v2) { return v1.y > v2.y; }
bool rsortZ(const Vector &v1, const Vector &v2) { return v1.z > v2.z; }


#endif