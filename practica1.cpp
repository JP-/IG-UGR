// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 1  (implementación)
// **
// *********************************************************************


#include <GL/glut.h>
#include <iostream>

#include "error-ogl.hpp"
#include "practica1.hpp"

#include <vector>

using namespace std;



Model modelo;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e 
// incializado OpenGL. El PLY se debe cargar aquí.

void P1::Inicializar( int argc, char *argv[] )
{
	if(argc < 2)
	{
		modelo.leePLY("./plys/ant.ply");
	}
	else
	{
		modelo.leePLY(argv[1]);
	}
}


/*********************************************************************/

// ---------------------------------------------------------------------
// Función a implementar en la práctica 1  para dibujar los objetos

void P1::DibujarObjetos(int modo, bool normCaras, bool normVertices)
{
	modelo.draw(modo, normCaras, normVertices);
}
