// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** Juan Pablo Porcel Porcel (C)
// **
// ** Definición de las funciones para leer un archivo .ge
// **
// *********************************************************************


#ifndef IG_GESTORGE
#define IG_GESTORGE

#include <iostream>
#include <vector>
#include "NodoGE.hpp"

using namespace std;

namespace ge
{

void read(const char * archivo, vector<EntradaNGE> & entradas);
NodoGE* leerNodo(ifstream & in);
void leerSource(string s, NodoGE * nodo);
void leerObjetoSimple(ifstream & in, NodoGE * nodo);
void leerTransformacion(ifstream & in, NodoGE * nodo);
void leerGradoLibertad(ifstream & in, NodoGE * nodo);
void leerDefinicion(ifstream & in, NodoGE * nodo);
void leerMatriz(ifstream & in, NodoGE * nodo);

}


#endif