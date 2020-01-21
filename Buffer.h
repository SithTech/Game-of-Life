/*
Buffer class -- This is a 3D matrix class I made to help with image manipulations
Developed by: Travis Stewart
*/

#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>


//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

enum BUFFER_FORMAT{GL=0, CV=1};

template <typename T>
class Buffer {
private:
	T* buffer_data;
	int buffer_width, buffer_height, buffer_depth, buffer_format;
	bool buffer_deleted;	//Used to handle Buffers on the stack
	
public:
	//-------------------------------------------------------------------------------------------------------------------------

	//Destructor
	~Buffer() { 
		deallocate();
	}

	//-------------------------------------------------------------------------------------------------------------------------

	//Constructor(s)
	Buffer(): buffer_data(nullptr), buffer_width(0), buffer_height(0), buffer_depth(0), buffer_format(BUFFER_FORMAT::GL), buffer_deleted(false) {}
	
	//Default Format (BUFFER_FORMAT::GL)
	Buffer(int w, int h, int d): buffer_width(w), buffer_height(h), buffer_depth(d), buffer_format(BUFFER_FORMAT::GL) { 
		check_diminsions();
		allocate(buffer_width * buffer_height * buffer_depth);
	}

	Buffer(int w, int h, int d, const T t): buffer_width(w), buffer_height(h), buffer_depth(d), buffer_format(BUFFER_FORMAT::GL) { 
		check_diminsions();
		allocate(buffer_width * buffer_height * buffer_depth);
	}

	Buffer(int w, int h, int d, const T* t): buffer_width(w), buffer_height(h), buffer_depth(d), buffer_format(BUFFER_FORMAT::GL) { 
		check_diminsions();
		allocate(buffer_width * buffer_height * buffer_depth);
	}

	//With Format
	Buffer(int w, int h, int d, int f): buffer_width(w), buffer_height(h), buffer_depth(d), buffer_format(f) { 
		check_diminsions();
		allocate(buffer_width * buffer_height * buffer_depth);
	}

	Buffer(int w, int h, int d, int f, const T t): buffer_width(w), buffer_height(h), buffer_depth(d), buffer_format(f) { 
		check_diminsions();
		allocate(buffer_width * buffer_height * buffer_depth);
	}

	Buffer(int w, int h, int d, int f, const T* t): buffer_width(w), buffer_height(h), buffer_depth(d), buffer_format(f) { 
		check_diminsions();
		allocate(buffer_width * buffer_height * buffer_depth);
	}

/*	//Copy Constructor
	Buffer(const Buffer<T>& buf) {
		buffer_format = buf.format();
		buffer_width = buf.width();
		buffer_height = buf.height();
		buffer_depth = buf.depth();
		//buffer_data = buf.data();		//Shallow copy

		//Allocate new memory
		buffer_data = new T(buffer_width * buffer_height * buffer_depth);

		//Deep copy
		for(int i = 0; i < buf.size(); i++) 
			buffer_data[i] = buf(i);
	}
//*/

	//-------------------------------------------------------------------------------------------------------------------------

	inline bool allocate(int _size) {
		try {
			buffer_data = new T[_size]();
			for(int i = 0; i < _size; i++) 
				buffer_data[i] = NULL;
			return true;
		}
		catch(...) {}
		return false;
	}

	//-------------------------------------------------------------------------------------------------------------------------

	inline bool deallocate() {
		try {
			if(buffer_data != nullptr && !buffer_deleted) {
				delete[] buffer_data; 
				buffer_data = nullptr;
				buffer_deleted = true;
				return true;
			}
		}
		catch(...) {}
		return false;
	}

	//-------------------------------------------------------------------------------------------------------------------------

	//Private variable access
	T* data() { return buffer_data; }
	inline int width() const { return buffer_width; }
	inline int height() const { return buffer_height; }
	inline int depth() const { return buffer_depth; }
	inline int format() const { return buffer_format; }
	inline int size() const { return buffer_width * buffer_height * buffer_depth; }
	
	inline void check_diminsions() {
		buffer_width  = (buffer_width  <= 0)? 1 : buffer_width;
		buffer_height = (buffer_height <= 0)? 1 : buffer_height;
		buffer_depth  = (buffer_depth  <= 0)? 1 : buffer_depth;
		buffer_format = (buffer_format < 0 )? 0 : buffer_format;
	}

	

	//-------------------------------------------------------------------------------------------------------------------------

	//Accessing elements
	//inline T& operator[] (int i) { return buffer_data[i]; }		//Only works with indexing into the 1D array
	
	//3D Get
	inline T operator() (int x, int y, int z) const {	/* Get */
		if(x < 0 || x >= buffer_width || y < 0 || y >= buffer_height || z < 0 || z >= buffer_depth) 
			return buffer_data[0]; 
		return (buffer_format)? buffer_data[x + (y * buffer_width) + (z * buffer_width * buffer_height)] : buffer_data[ (y * buffer_width * buffer_depth) + (x * buffer_depth) + z ]; 
	}

	//3D Set	
	inline T& operator() (int x, int y, int z) {	/* Set */
		if(x < 0 || x >= buffer_width || y < 0 || y >= buffer_height || z < 0 || z >= buffer_depth) 
			return buffer_data[0]; 
		return (buffer_format)? buffer_data[x + (y * buffer_width) + (z * buffer_width * buffer_height)] : buffer_data[ (y * buffer_width * buffer_depth) + (x * buffer_depth) + z ]; 
	}
	
	//-------------------------------------------------------------

	//2D Get
	inline T operator() (int x, int y) const {	/* Get */
		if(x < 0 || x >= buffer_width || y < 0 || y >= buffer_height) 
			return buffer_data[0]; 
		return (buffer_format)? buffer_data[x + (y * buffer_width) + (0 * buffer_width * buffer_height)] : buffer_data[ (y * buffer_width * buffer_depth) + (x * buffer_depth) + 0 ]; 
	}

	//2D Set
	inline T& operator() (int x, int y) {	/* Set */
		if(x < 0 || x >= buffer_width || y < 0 || y >= buffer_height) 
			return buffer_data[0]; 
		return (buffer_format)? buffer_data[x + (y * buffer_width) + (0 * buffer_width * buffer_height)] : buffer_data[ (y * buffer_width * buffer_depth) + (x * buffer_depth) + 0 ]; 
	}

	//-------------------------------------------------------------

	//1D Get
	inline T operator() (int i) const {	  /* Get */
		return buffer_data[i]; 
	}

	//1D Set	
	inline T& operator() (int i) {	/* Set */
		return buffer_data[i]; 
	}			

	//-------------------------------------------------------------------------------------------------------------------------

	//Compute Index
	inline int idx(int x, int y, int z) { return (buffer_format)? x + (y * buffer_width) + (z * buffer_width * buffer_height) : (y * buffer_width * buffer_depth) + (x * buffer_depth) + z; }
	inline int idx(int x, int y) { return (y * buffer_width) + x; }

	//Enables index wrapping in the buffer w.r.t the width, height, or depth.
	int wrapX(const int& idx) { return (buffer_width + idx) % buffer_width; }
	int wrapY(const int& idx) { return (buffer_height + idx) % buffer_height; }
	int wrapZ(const int& idx) { return (buffer_depth + idx) % buffer_depth; }

	//Enables index wrapping in the buffer w.r.t the given limit value.
	int wrapIdx(const int& limit, const int& arg) { return (limit + arg) % limit; }

	//-------------------------------------------------------------------------------------------------------------------------

	//Simple Arithmetic Operations
	void operator += (const Buffer<T>& buf) { for(int i = 0; i < size(); i++) buffer_data[i] = buffer_data[i] + buf(i); }
	void operator -= (const Buffer<T>& buf) { for(int i = 0; i < size(); i++) buffer_data[i] = buffer_data[i] - buf(i); }
	void operator *= (const Buffer<T>& buf) { for(int i = 0; i < size(); i++) buffer_data[i] = buffer_data[i] * buf(i); }
	void operator /= (const Buffer<T>& buf) { for(int i = 0; i < size(); i++) buffer_data[i] = buffer_data[i] / buf(i); }

	inline void operator += (const T& t) { for(int i = 0; i < size(); i++) buffer_data[i] = buffer_data[i] + t; }
	inline void operator -= (const T& t) { for(int i = 0; i < size(); i++) buffer_data[i] = buffer_data[i] - t; }
	inline void operator *= (const T& t) { for(int i = 0; i < size(); i++) buffer_data[i] = buffer_data[i] * t; }
	inline void operator /= (const T& t) { for(int i = 0; i < size(); i++) buffer_data[i] = buffer_data[i] / t; }

	Buffer<T> operator + (const Buffer<T>& buf) { Buffer<T> t_buf = like(); for(int i = 0; i < size(); i++) t_buf(i) = buffer_data[i] + buf(i); return t_buf;}
	Buffer<T> operator - (const Buffer<T>& buf) { Buffer<T> t_buf = like(); for(int i = 0; i < size(); i++) t_buf(i) = buffer_data[i] - buf(i); return t_buf;}
	Buffer<T> operator * (const Buffer<T>& buf) { Buffer<T> t_buf = like(); for(int i = 0; i < size(); i++) t_buf(i) = buffer_data[i] * buf(i); return t_buf;}
	Buffer<T> operator / (const Buffer<T>& buf) { Buffer<T> t_buf = like(); for(int i = 0; i < size(); i++) t_buf(i) = buffer_data[i] / buf(i); return t_buf;}

	Buffer<T> operator + (const T& t) { Buffer<T> t_buf = like(); for(int i = 0; i < size(); i++) t_buf(i) = buffer_data[i] + t; return t_buf;}
	Buffer<T> operator - (const T& t) { Buffer<T> t_buf = like(); for(int i = 0; i < size(); i++) t_buf(i) = buffer_data[i] - t; return t_buf;}
	Buffer<T> operator * (const T& t) { Buffer<T> t_buf = like(); for(int i = 0; i < size(); i++) t_buf(i) = buffer_data[i] * t; return t_buf;}
	Buffer<T> operator / (const T& t) { Buffer<T> t_buf = like(); for(int i = 0; i < size(); i++) t_buf(i) = buffer_data[i] / t; return t_buf;}

	friend Buffer<T> operator + (const T& t, const Buffer<T>& buf) { Buffer<T> t_buf = buf.like(); for(int i = 0; i < buf.size(); i++) t_buf(i) = t + buf(i); return t_buf;}
	friend Buffer<T> operator - (const T& t, const Buffer<T>& buf) { Buffer<T> t_buf = buf.like(); for(int i = 0; i < buf.size(); i++) t_buf(i) = t - buf(i); return t_buf;}
	friend Buffer<T> operator * (const T& t, const Buffer<T>& buf) { Buffer<T> t_buf = buf.like(); for(int i = 0; i < buf.size(); i++) t_buf(i) = t * buf(i); return t_buf;}
	friend Buffer<T> operator / (const T& t, const Buffer<T>& buf) { Buffer<T> t_buf = buf.like(); for(int i = 0; i < buf.size(); i++) t_buf(i) = t / buf(i); return t_buf;}

	//-------------------------------------------------------------------------------------------------------------------------

	//Boolean Operators
	bool operator == (const Buffer<T>& buf) { for(int i = 0; i < size(); i++) if(buffer_data[i] != buf(i)) return false; return true; }
	bool operator <  (const Buffer<T>& buf) { for(int i = 0; i < size(); i++) if(buffer_data[i] >= buf(i)) return false; return true; }
	bool operator >  (const Buffer<T>& buf) { for(int i = 0; i < size(); i++) if(buffer_data[i] <= buf(i)) return false; return true; }
	bool operator <= (const Buffer<T>& buf) { for(int i = 0; i < size(); i++) if(buffer_data[i] >  buf(i)) return false; return true; }
	bool operator >= (const Buffer<T>& buf) { for(int i = 0; i < size(); i++) if(buffer_data[i] <  buf(i)) return false; return true; }
	bool operator != (const Buffer<T>& buf) { for(int i = 0; i < size(); i++) if(buffer_data[i] == buf(i)) return false; return true; }

	bool operator == (const T& t) { for(int i = 0; i < size(); i++) if(buffer_data[i] != t) return false; return true; }
	bool operator <  (const T& t) { for(int i = 0; i < size(); i++) if(buffer_data[i] >= t) return false; return true; }
	bool operator >  (const T& t) { for(int i = 0; i < size(); i++) if(buffer_data[i] <= t) return false; return true; }
	bool operator <= (const T& t) { for(int i = 0; i < size(); i++) if(buffer_data[i] >  t) return false; return true; }
	bool operator >= (const T& t) { for(int i = 0; i < size(); i++) if(buffer_data[i] <  t) return false; return true; }
	bool operator != (const T& t) { for(int i = 0; i < size(); i++) if(buffer_data[i] == t) return false; return true; }

	//-------------------------------------------------------------------------------------------------------------------------
	
	//Like function
	Buffer<T> like_1() { return Buffer<T>(buffer_width, 1, 1, buffer_format); }	//Creates a Buffer with the same 1D diminsions
	Buffer<T> like_2() { return Buffer<T>(buffer_width, buffer_height, 1, buffer_format); }	//Creates a Buffer with the same 2D diminsions
	Buffer<T> like() { return Buffer<T>(buffer_width, buffer_height, buffer_depth, buffer_format); }	//Creates a Buffer with the same 3D diminsions

	//Copy function
	Buffer<T> clone() { return Buffer<T>(buffer_width, buffer_height, buffer_depth, buffer_format, data()); }
	Buffer<T> copy() { return Buffer<T>(buffer_width, buffer_height, buffer_depth, buffer_format, data()); }	//Update to be a shallow copy

	void clear() { for(int i = 0; i < size(); i++) buffer_data[i] = NULL; }
	void clear(const T& t) { for(int i = 0; i < size(); i++) buffer_data[i] = t; }

	void clearX(const int& x) { for(int y = 0; y < buffer_height; y++) for(int z = 0; z < buffer_depth; z++) buffer_data[idx(x, y, z)] = NULL; }
	void clearY(const int& y) { for(int x = 0; x < buffer_width; x++) for(int z = 0; z < buffer_depth; z++) buffer_data[idx(x, y, z)] = NULL; }
	void clearZ(const int& z) { for(int y = 0; y < buffer_height; y++) for(int x = 0; x < buffer_width; x++) buffer_data[idx(x, y, z)] = NULL; }

	void set_size(int w, int h, int d) {
		buffer_width = w;
		buffer_height = h;
		buffer_depth = d;
	}
	void resize(int w, int h, int d) { 
		set_size(w, h, d);
		//deallocate();	//Deallocated buffer memory		//NOTE: CAUSE EXCEPTION ON MALLOC DELETE
		delete[] buffer_data; 
		buffer_deleted = true;
		allocate(buffer_width * buffer_height * buffer_depth);	//Allocate buffer memory	
	}

	void link(T* t) {
		deallocate();	//Deallocated buffer memory
		buffer_deleted = false;
		buffer_data = t;
	}

	void link(T* t, int w, int h, int d) {
		set_size(w, h, d);
		link(t);
	}
	//-------------------------------------------------------------------------------------------------------------------------
	
	void format(int f) {

		//Check if Buffer is already in the format
		if( buffer_format == f ) return;
		
		//Create a temporary array
		T t[buffer_width * buffer_height * buffer_depth];
		
		int idx = 0;
		for(int z = 0; z < buffer_depth; z++)
			for(int y = 0; y < buffer_height; y++)
				for(int x = 0; x < buffer_width; x++) {
					t[x + (y * buffer_width) + (z * buffer_width * buffer_height)] = buffer_data[idx]; 
					idx++;
				}

		//Copy the formatted temporary array to the buffer
		for(int i = 0; i < size(); i++) 
			buffer_data[i] = t[i];

		//Set the format
		buffer_format = f; 
	}

	//-------------------------------------------------------------------------------------------------------------------------
	
	Buffer<T> flatten() {


	}

	//-------------------------------------------------------------------------------------------------------------------------
	
	std::vector<T> to_vector() {
		//std::vector<T> v = new std::vector<T>();	//Allocated on the heap
		std::vector<T> v;	//Allocated on the stack
		for(int i = 0; i < size(); i++)
			v.pop_back(this(i));
		return v;
	}

	//-------------------------------------------------------------------------------------------------------------------------
	
	T* to_array() {
		//T* t = new T(size());		//Allocated on the heap
		T t[size()];	//Allocated on the stack
		for(int i = 0; i < size(); i++)
			t[i] = buffer_data[i];
		return t;
	}
	

	
	
	
	
	
	
	//-------------------------------------------------------------------------------------------------------------------------

	//Min, Max, Avg Functions
	T max() { T m = buffer_data[0]; for(int i = 0; i < size(); i++) m = ( m >= buffer_data[i] )? m : buffer_data[i]; return m; }
	T min() { T m = buffer_data[0]; for(int i = 0; i < size(); i++) m = ( m <= buffer_data[i] )? m : buffer_data[i]; return m; }
	T avg() { T a = buffer_data[0]; for(int i = 0; i < size(); i++) a += buffer_data[i]; return a / size(); }
	
	//-------------------------------------------------------------------------------------------------------------------------
	
	//Global Mean value per channel
	T* mean() { 
		//T* t = new T(buffer_depth);		//Is this a memory leak? Could use a Buffer<T> or std::vector instead
		T t[buffer_depth];
		for(int i = 0; i < buffer_depth; i++)
			t[i] = NULL;

		for(int z = 0; z < buffer_depth; z++)
			for(int y = 0; y < buffer_height; y++)
				for(int x = 0; x < buffer_width; x++)
					t[z] += buffer_data[idx(x, y, z)];

		for(int i = 0; i < buffer_depth; i++)
			t[i] /= (T)buffer_width * (T)buffer_height;

		return t;
	}

	//-------------------------------------------------------------------------------------------------------------------------
	
	//Row Mean value per channel
	Buffer<T> mean_row() { 
		Buffer<T> t_buf = Buffer(1, buffer_height, buffer_depth, buffer_format, (T)0);

		for(int y = 0; y < buffer_height; y++)
			for(int x = 0; x < buffer_width; x++)
				for(int z = 0; z < buffer_depth; z++)
					t_buf(0, y, z) += buffer_data[idx(x, y, z)];
		
		t_buf /= (T)buffer_width;
		
		return t_buf;
	}

	//-------------------------------------------------------------------------------------------------------------------------
	
	//Column Mean value per channel
	Buffer<T> mean_col() { 
		Buffer<T> t_buf = Buffer(buffer_width, 1, buffer_depth, buffer_format, (T)0);
		
		for(int y = 0; y < buffer_height; y++)
			for(int x = 0; x < buffer_width; x++)
				for(int z = 0; z < buffer_depth; z++)
					t_buf(x, 0, z) += buffer_data[idx(x, y, z)];
		
		t_buf /= (T)buffer_height;
		
		return t_buf;
	}

	//-------------------------------------------------------------------------------------------------------------------------

	//Median value per channel
	T* median() {}

	//-------------------------------------------------------------------------------------------------------------------------

	//Mode value per channel
	T* mode() {}

	//-------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------Matrix Operations------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------------------

	

	

	//-------------------------------------------------------------------------------------------------------------------------

	//Matrix Multiply
	Buffer<T> mult(Buffer<T>& buf) {
		Buffer<T> t_buf(buf.width(), buffer_height, buffer_depth, buffer_format, (T)0);
		
		//Check matrix diminsions and create the appropriate Buffer<T>
		if(buffer_width != buf.height()) return t_buf;

		//Multiply the buffers
		for(int z = 0; z < buffer_depth; z++)
			for(int j = 0; j < t_buf.height(); j++)
				for(int i = 0; i < t_buf.width(); i++)
					for(int k = 0; k < buf.height(); k++)
						t_buf(i, j, z) += buffer_data[idx(k, j, z)] * buf(i, k, z);

		return t_buf;
	}

	//-------------------------------------------------------------------------------------------------------------------------

	//Matrix Transpose
	Buffer<T> transpose() {
		Buffer<T> t_buf(buffer_height, buffer_width, buffer_depth, buffer_format);

		for(int y = 0; y < buffer_height; y++)
			for(int x = 0; x < buffer_width; x++)
				for(int z = 0; z < buffer_depth; z++)
					t_buf(y, x, z) = buffer_data[idx(x, y, z)];

		return t_buf;
	}

	//-------------------------------------------------------------------------------------------------------------------------

	Buffer<T> adjoint() {
		Buffer<T> t_buf(buffer_width, buffer_height, buffer_depth, buffer_format);

		if (t_buf.size() == 1) 
		{ 
			t_buf(0,0) = 1; 
			return NULL; 
		} 
	
		// temp is used to store cofactors of A[][] 
		T sign = 1;//, temp[N][N]; 
	
		for (int i=0; i<t_buf.width(); i++) 
		{ 
			for (int j=0; j<t_buf.height(); j++) 
			{ 
				// Get cofactor of A[i][j] 
				//getCofactor(A, temp, i, j, N); 
	
				// sign of adj[j][i] positive if sum of row 
				// and column indexes is even. 
				sign = ((i+j)%2==0)? 1: -1; 
	
				// Interchanging rows and columns to get the 
				// transpose of the cofactor matrix 
				t_buf(i, j) = (sign)*(determinant(cofactor(i, j, t_buf.width()), t_buf.width()-1)); 
			} 
		}

		return t_buf;
	}

	//-------------------------------------------------------------------------------------------------------------------------
	
	T determinant(int n) {
		T det = 0; // Initialize result 
  
		//  Base case : if matrix contains single element 
		if (n == 1) 
			return buffer_data[idx(0,0)]; 
	
		//int temp[N][N]; // To store cofactors 
	
		T sign = 1;  // To store sign multiplier 
	
		// Iterate for each element of first row 
		for (int f = 0; f < n; f++) 
		{ 
			// Getting Cofactor of A[0][f] 
			//getCofactor(A, temp, 0, f, n); 
			det += sign * buffer_data[idx(f, 0)] * cofactor(0, f, n).determinant(n - 1); 
	
			// terms are to be added with alternate sign 
			sign = -sign; 
		} 
	
		return det; 
	}

	//-------------------------------------------------------------------------------------------------------------------------
	
	Buffer<T> cofactor(int q, int p, int n) {
		Buffer<T> t_buf(buffer_width, buffer_height, buffer_depth, buffer_format);
		
		int i = 0, j = 0; 
  
		// Looping for each element of the matrix 
		for (int row = 0; row < n; row++) 
		{ 
			for (int col = 0; col < n; col++) 
			{ 
				//  Copying into temporary matrix only those element 
				//  which are not in given row and column 
				if (row != p && col != q) 
				{ 
					t_buf(i, j++) = buffer_data[idx(col, row)]; 
	
					// Row is filled, so increase row index and 
					// reset col index 
					if (j == n - 1) 
					{ 
						j = 0; 
						i++; 
					} 
				} 
			} 
		} 

		return t_buf;
	}

	//-------------------------------------------------------------------------------------------------------------------------
	
	Buffer<T> inverse() {
		Buffer<T> t_buf(buffer_width, buffer_height, buffer_depth, buffer_format);

		// Find determinant of A[][] 
		int det = determinant(width()); 
		if (det == 0) 
		{ 
			std::cout << "Singular matrix, can't find its inverse"; 
			return false; 
		} 
	
		// Find adjoint 
		Buffer<T> adj = adjoint(); 
		
		// Find Inverse using formula "inverse(A) = adj(A)/det(A)" 
		for (int i=0; i<t_buf.height(); i++) 
			for (int j=0; j<t_buf.width(); j++) 
				t_buf(j, i) = adj(j, i)/(T)det; 
	
		return t_buf; 
	}

	//-------------------------------------------------------------------------------------------------------------------------

	//Standard Deviation
	T* std_deviation() {
		T* t = variance();
		for(int i = 0; i < buffer_depth; i++)
			t[i] = sqrt(t[i]);

		return t;
	}

	//-------------------------------------------------------------------------------------------------------------------------

	//Variance		var(x) = 1/n∑(x_i - mean_x)^2, i=1 -> n
	T* variance() {
		T* m = mean();
		T* t = new T(buffer_depth);		//Is this a memory leak? Could use a Buffer<T> or std::vector instead

		//Empty the variance array
		for(int i = 0; i < buffer_depth; i++)
			t[i] = NULL;

		//Compute the variance
		for(int z = 0; z < buffer_depth; z++)
			for(int y = 0; y < buffer_height; y++)
				for(int x = 0; x < buffer_width; x++)
					t[z] += pow(buffer_data[idx(x, y, z)] - m[z], 2.0);

		for(int i = 0; i < buffer_depth; i++)
			t[i] /= (T)buffer_width * (T)buffer_height;

		delete[] m;	//Might need this if it is a memory leak. Either way can't hurt

		return t;
	}

	//-------------------------------------------------------------------------------------------------------------------------

	//Covariance	cov(x,y) = 1/n∑(x_i - mean_x)(y_i - mean_y), i=1 -> n
	T* covariance() {
		T* m = mean();
		T* t = new T(buffer_depth);		//Is this a memory leak? Could use a Buffer<T> or std::vector instead
		Buffer<T> t_buf = transpose();

		//Empty the covariance array
		for(int i = 0; i < buffer_depth; i++)
			t[i] = NULL;

		for(int z = 0; z < buffer_depth; z++)
			for(int y = 0; y < buffer_height; y++)
				for(int x = 0; x < buffer_width; x++)
					t[z] += (buffer_data[idx(x, y, z)] - m[z]) * (t_buf(x, y, z) - m[z]);

		for(int i = 0; i < buffer_depth; i++)
			t[i] /= (T)buffer_width * (T)buffer_height;

		delete[] m;	//Might need this if it is a memory leak. Either way can't hurt

		return t;
	}

	//-------------------------------------------------------------------------------------------------------------------------

	//Covariance Matrix
	Buffer<T> covariance_matrix() {
		/*
		Buffer<T> ones = Buffer<T>(buffer_height, buffer_height, buffer_depth, buffer_format, 1.0f);
		Buffer<T> c_buf = clone();
		Buffer<T> t_buf = (c_buf / (T)c_buf.height());
		
		//Standard Deviation Scores		m = M - 11`M
		Buffer<T> std_buf = c_buf - ones.mult(t_buf);

		//Return (Deviation Score Sums of Squares)/Height	(m`m)(1/N)
		Buffer<T> cov_buf = (std_buf.transpose().mult(std_buf)) / (T)c_buf.height();
		*/

		Buffer<T> c_buf(buffer_height * buffer_width, buffer_depth, 1, 1.0f);
		int idx = 0;

		if(buffer_format == GL) {
			for(int z = 0; z < buffer_depth; z++) {
				idx = 0;
				for(int i = z; i < c_buf.width(); i+=buffer_depth) {
					c_buf(idx, z) = buffer_data[i];
					idx++;
				}
			}
		}
		else {
			for(int z = 0; z < buffer_depth; z++) {
				for(int i = 0; i < c_buf.width(); i++) {
					c_buf(i, z) = buffer_data[i + z*c_buf.width()];
				}
			}
		}

		Buffer<T> mean_buf = c_buf.mean_row();
		Buffer<T> std_buf = c_buf.like();

		for(int y = 0; y < std_buf.height(); y++)
			for(int x = 0; x < std_buf.width(); x++)
				std_buf(x, y) = c_buf(x, y) - mean_buf(0, y);

		//Return (Deviation Score Sums of Squares)/Height	(m`m)(1/N)
		Buffer<T> cov_buf = (std_buf.transpose().mult(std_buf)) / (T)c_buf.height();

		return cov_buf;
	}

	//-------------------------------------------------------------------------------------------------------------------------
/*
	//Covariance Matrix
	Buffer<T> spectral_covariance_matrix() {
		Buffer<T> spectral = Buffer<T>(buffer_height * buffer_width, buffer_depth, 1, 1.0f);
		int idx;

		for(int z = 0; z < buffer_depth; z++) {
			idx = 0;
			for(int i = z; i < spectral.width(); i+=buffer_depth) {
				spectral(idx, z) = buffer_data[i];
				idx++;
			}
		}
				
		return cov_buf;
	}
*/
	//-------------------------------------------------------------------------------------------------------------------------
	


	//-------------------------------------------------------------------------------------------------------------------------
	//------------------------------------------------Distance Functions-------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------------------

	//Euclidean
	Buffer<T> euclidean(T* t) { 
		Buffer<T> t_buf = like_2();
		
		for(int y = 0; y < buffer_height; y++)
			for(int x = 0; x < buffer_width; x++) {
				t_buf(x, y) = NULL;		//Clear the new buffer before using it
				for(int z = 0; z < buffer_depth; z++)
					t_buf(x, y) += pow((buffer_data[idx(x, y, z)] - t[z]), 2.0);
				t_buf(x, y) = sqrt(t_buf(x, y));	//In-place square-root
			}

		return t_buf;
	}

	//-------------------------------------------------------------------------------------------------------------------------

	Buffer<T> manhattan(T* t) { 
		Buffer<T> t_buf = like_2();
		
		for(int y = 0; y < buffer_height; y++)
			for(int x = 0; x < buffer_width; x++) {
				t_buf(x, y) = NULL;		//Clear the new buffer before using it
				for(int z = 0; z < buffer_depth; z++)
					t_buf(x, y) += abs(buffer_data[idx(x, y, z)] - t[z]);
			}

		return t_buf;
	}

	//-------------------------------------------------------------------------------------------------------------------------

	Buffer<T> minkowski(T* t, double p) { 	//p is the exponent
		Buffer<T> t_buf = like_2();

		for(int y = 0; y < buffer_height; y++)
			for(int x = 0; x < buffer_width; x++) {
				t_buf(x, y) = NULL;		//Clear the new buffer before using it
				for(int z = 0; z < buffer_depth; z++)
					t_buf(x, y) += pow(abs(buffer_data[idx(x, y, z)] - t[z]), p);
				t_buf(x, y) = pow(t_buf(x, y), 1.0/p);
			}

		return t_buf;
	}

	//-------------------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------------------------

	std::string info() {
		std::string rtn_str = "";
		for(int z = 0; z < buffer_depth; z++) {
			rtn_str += "[_,_," + std::to_string(z) + "]\n";
			for(int y = 0; y < buffer_height; y++) {
				rtn_str += "\t";
				for(int x = 0; x < buffer_width; x++) {
					rtn_str += std::to_string(buffer_data[idx(x, y, z)]) + "\t";
				}
				rtn_str += "\n";
			}
			rtn_str += "[_,_," + std::to_string(z) + "]\n";
		}
		return rtn_str + "\n";
	}

};

//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

#endif