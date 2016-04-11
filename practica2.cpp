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
#include "practica2.hpp"

#include <vector>
#include <string.h>

using namespace std;



Model modelo2;

double gradoA, gradoB, gradoC;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 2 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL. El PLY se debe cargar aquí.

void P2::Inicializar( int argc, char *argv[] )
{
	switch(argc)
	{
		case 3:
			if(!strcmp(argv[2], "B"))
			{
				modelo2.crearBarrido("perfil.ply", false, 0.5);
			}
			else
				modelo2.crearRevolucion("perfil.ply", 10);
			break;
		case 4:
			if(!strcmp(argv[2], "B"))
				modelo2.crearBarrido(argv[3], false, 0.5);
			else
				modelo2.crearRevolucion(argv[3], 10);
			break;
		case 5:
			if(!strcmp(argv[2], "B"))
				modelo2.crearBarrido(argv[3], false, atof(argv[4]));
			else
				modelo2.crearRevolucion(argv[3], atoi(argv[4]));
			break;
		case 6:
			if(!strcmp(argv[2], "B"))
				modelo2.crearBarrido(argv[3], atoi(argv[5]), atof(argv[4]));
			else
				modelo2.crearRevolucion(argv[3], atoi(argv[4]));
			break;
		default: // por defecto se carga el ply rey.ply en revolución
			modelo2.crearRevolucion("./plys/rey.ply", 10);
			break;
	}
}


/*********************************************************************/

// ---------------------------------------------------------------------
// Función a implementar en la práctica 2  para dibujar los objetos

void P2::DibujarObjetos(int modo, bool normCaras, bool normVertices)
{
	modelo2.draw(modo, normCaras, normVertices);
}
