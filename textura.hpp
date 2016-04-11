// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// **
// ** Estructura de datos para guardar una textura
// **
// *********************************************************************


#ifndef IG_TEXTURA
#define IG_TEXTURA

#include <iostream>
#include "file_ply_stl.hpp"
#include <GL/glut.h>
#include "jpg_imagen.hpp"

using namespace std;

class Textura                // Imagen de textura
{
private:
	GLuint        id_text;    // identificador OpenGL de la textura
	unsigned      modo;       // modo de gen. cc.tt. (0 = desactivado, 1 = objeto, 2 = camara)
	float         cs[4],ct[4]; // coeficientes para generación de coords. de textura

public:

	Textura(string imagen, int m);
	void activar();          // activa textura en el cauce fijo
	int getModo();
};


#endif