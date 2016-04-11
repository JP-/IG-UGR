// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// **
// ** Estructura de datos para guardar una textura
// **
// *********************************************************************


#ifndef IG_FUENTELUZ
#define IG_FUENTELUZ

#include <iostream>
#include "file_ply_stl.hpp"
#include <GL/glut.h>
#include "tuplas.hpp"
#include <vector>

using namespace std;

class FuenteLuz                // Imagen de textura
{
private:
	Tupla4f  posvec;       // posición (w=1) o vector (w=0)
	vector<Tupla3f>  colores;   // colores: 0=ambiental, 1=difuso, 2=especular.
	double    alpha, beta;      // ángulos de rotación (fuente direccional modificable interactivamente)

public:

	FuenteLuz(){};
	FuenteLuz(Tupla4f p, vector<Tupla3f> c, float lo, float la);
	void definir(Tupla4f p, vector<Tupla3f> c, float lo, float la);
	void activar( int i ) ; // activa la fuente de luz (con número GL_LIGHT0+i)
	void desactivar( int i ) ; // activa la fuente de luz (con número GL_LIGHT0+i)
	float getAlpha();
	void setAlpha(float);
	void setBeta(float);
	float getBeta();
	Tupla4f * getPos();
};


#endif