// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// **
// ** Estructura de datos para guardar el modelo
// **
// *********************************************************************

#ifndef IG_MODEL
#define IG_MODEL

#include <iostream>
#include "tuplas.hpp"
#include "matrizR3D.hpp"
#include "file_ply_stl.hpp"
#include <vector>
#include <GL/glut.h>
#include <math.h>
#include "material.hpp"

using namespace std;

typedef pair<GLfloat, GLfloat> tex;


class Model
{

private:

	vector<Tupla3f> vertices;
	vector<Tupla3i> caras;
	vector<Tupla3f> normalesVertices;
	vector<Tupla3f> normalesCaras;
	vector<Tupla3f> colores;
	vector<Tupla3f> baricentros;
	vector<tex> coordTex;
	Material material;
	Material materialSeleccionado;
	bool seleccionado;
	Tupla3f puntoCentro;
	
	void revolucion(const char * archivo, int N, bool tapaSUP, bool tapaINF, bool textura);
	void creaPLY(const char * archivo);
	void barrido(const char * archivo, bool cerrado, double N);
	void calcularPuntoMedio();

public:

	Model(){};
	Model(const char * archivo, Material m = Material());
	Model(const char * archivo, int N, bool tapaSUP = true, bool tapaINF = true, bool textura = false, Material m = Material());
	Model(const char * archivo, bool cerrado, double N);
	void leePLY(const char * archivo, Material m = Material());
	void crearRevolucion(const char * archivo, int N, bool tapaSUP = true, bool tapaINF = true, bool textura = false, Material m = Material());
	void crearBarrido(const char * archivo, bool cerrado, double N);
	void calcularNormalesVertices();
	void calcularNormalesCaras();
	void calcularNormales();
	void calcularBaricentros();
	vector<Tupla3f> getVertices();
	vector<Tupla3i> getCaras();
	Material getMaterial();
	void setMaterial(Material m);
	void draw(int modo = 2, bool = false, bool = false, bool s = false);
	void drawNormalesCaras();
	void drawNormalesVertices();
	void seleccionar();
	void deseleccionar();
	Tupla3f getPuntoMedio();
	
};

#endif