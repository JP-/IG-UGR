// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// **
// ** Estructura de datos para una matriz de reotación
// **
// *********************************************************************

#ifndef IG_MATRIZ_R3D
#define IG_MATRIZ_R3D

#include <iostream>
#include "tuplas.hpp"
#include "file_ply_stl.hpp"
#include <vector>
#include <GL/glut.h>
#include <cmath>

using namespace std;

class MatrizR3D
{
	
private:
	double ** datos;	// Los valores de la matriz
	int filas;			// Núm. de filas y columnas

public:

	// Constructor sin argumentos (matriz vacia)
	MatrizR3D(void);

	// Constructor de matriz de "d" dimensiones
	MatrizR3D(char c, double alpha);

	// Constructor de copia
	MatrizR3D(const MatrizR3D & otra);

	// Destructor
	~MatrizR3D(void);

	// Sobrecarga operador ()
	double & operator () (const int fila, const int col);

	// Sobrecarga operador *
	Tupla3f operator * (const Tupla3f & t);

	MatrizR3D & operator = (const MatrizR3D & otra);


private:

	// Pide la memoria necesaria
	void ReservaMemoria (void);

	// Libera la memoria ocupada
	void LiberaMemoria (void);

	// Inicializa la matriz
	void Inicializar(char c, double alpha);

	// Proceso de Copia
	void Copia(const MatrizR3D & otra);
};

#endif