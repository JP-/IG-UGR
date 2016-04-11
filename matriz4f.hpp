// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// **
// ** Clase Matriz4f para almacenar los 16 valores de una matriz de transformación
// **
// *********************************************************************

#ifndef IG_MATRIZ4F
#define IG_MATRIZ4F

#include <iostream>
#include "file_ply_stl.hpp"
#include "tuplas.hpp"
#include <vector>
#include <cmath>

using namespace std;

class Matriz4f
{
	
private:
	double ** datos;	// Los valores de la matriz
	int filas;			// Núm. de filas y columnas

public:

	// Constructor sin argumentos (matriz vacia)
	Matriz4f(void);

	// Constructor de matriz de "d" dimensiones
	Matriz4f(char c, double alpha, Tupla3f vector = Tupla3f(0,0,0));

	Matriz4f(char c, vector<double> param);

	Matriz4f(vector<double> param);

	// Constructor de copia
	Matriz4f(const Matriz4f & otra);

	// Destructor
	~Matriz4f(void);

	// Sobrecarga operador ()
	double & operator () (const int fila, const int col);

	Matriz4f operator * (const Matriz4f &m);
	Tupla3f operator * (Tupla3f &t);
	
	Matriz4f & operator = (const Matriz4f & otra);


private:

	// Pide la memoria necesaria
	void ReservaMemoria (void);

	// Libera la memoria ocupada
	void LiberaMemoria (void);

	// Inicializa la matriz
	void Inicializar(char c, double alpha, Tupla3f vector = Tupla3f(0,0,0));

	void Inicializar(char c, vector<double> param);

	void Inicializar(vector<double> param);
	
	// Proceso de Copia
	void Copia(const Matriz4f & otra);
};

#endif