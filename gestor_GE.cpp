// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** Juan Pablo Porcel Porcel (C)
// **
// ** Funciones para leer un archivo .ge
// **
// *********************************************************************

#include "gestor_GE.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>

namespace ge
{


/*********************************************************************/

// Método para leer un grafo desde un archivo 
void read(const char * archivo, vector<EntradaNGE> & entradas)
{
	ifstream in;

	string s = "./grafos/"; // los grafos se encuentran en la carpeta "grafos"
	s += archivo;

	in.open(s.c_str());

	if ( !in.is_open() )
	{
		string msg = string("no puedo abrir el archivo '") + s + "' para lectura." ;
		cerr << (msg.c_str());
		exit(-1);
	}

	char c;

	in >> c;

	while(!in.eof())
	{
		NodoGE* no = leerNodo(in); // leerNodo lee un nodo, devuelve NULL si no hay nodo
		if(no != NULL)
		{
			EntradaNGE e(0, no);
			entradas.push_back(e);
		}
		in >> c;
	}

	in.close();
}


/*********************************************************************/

// lee un nodo completo del grafo
NodoGE* leerNodo(ifstream & in)
{
	NodoGE * nodo;
	char c;
	in >> c;
	switch(c)
	{
		case 's': // source (fuente)
		{
			string s;
			in >> s;
			nodo = new NodoGE();
			leerSource(s, nodo); // lee un nuevo archivo .ge
			break;
		}
		case 'd': // definición
		{
			nodo = new NodoGE();
			leerDefinicion(in, nodo); // lee la definición de un nuevo nodo
			break;
		}
		case 'e': // end (fin)
		{
			nodo = NULL;
		}
	}
	
	return nodo;
}


/*********************************************************************/

// abre el archivo enlazado en el grafo y lo lee
void leerSource(string s, NodoGE * nodo)
{
	ifstream in;

	s = "./grafos/" + s;
	
	in.open(s.c_str());
	
	if ( !in.is_open() )
	{
		string msg = string("no puedo abrir el archivo '") + s + "' para lectura." ;
		cerr << (msg.c_str());
		exit(-1);
	}

	char c;
	in >> c;
	
	NodoGE* no = leerNodo(in);
	if(no != NULL)
	{
		EntradaNGE e(0, no);
		nodo->incluirEntrada(e);
	}	
}


/*********************************************************************/

// lee la definición completa de un nodo
void leerDefinicion(ifstream & in, NodoGE * nodo)
{
	char t;
	bool sigo = true;

	in >> t;

	while(sigo)
	{
		switch(t)
		{
			case 'n': // nodo (nueva definición de un nodo)
			{
				NodoGE* no = leerNodo(in);
				if(no == NULL)
				{
					sigo = false;
				}
				else{
					EntradaNGE e(0, no);
					nodo->incluirEntrada(e);
					in >> t;
				}
				break;
			}
			case 't': // tranformacion
				leerTransformacion(in, nodo);
				in >> t;
				break;
			case 'g': // grado de libertad
				leerGradoLibertad(in, nodo);
				in >> t;
				break;
			case 'o': // objeto simple
			{
				int o;
				in >> o;
				EntradaNGE e(3, o);
				nodo->incluirEntrada(e);
				in >> t;
				break;
			}
			case '{': // comentario
			{
				in >> t;
				while(t!='}')
					in >> t;
				in >> t;
				break;
			}
			case 'm': // parámetros de una Matriz4f
			{
				leerMatriz(in, nodo);
				in >> t;
				break;
			}
		}
	}
}


/*********************************************************************/

// lee los elementos de una Matriz4f
void leerMatriz(ifstream & in, NodoGE * nodo)
{
	vector<double> param;

	for(int i=0; i<16; i++)
	{
		double d;
		in >> d;
		param.push_back(d);
	}

	Matriz4f m(param);
	
	EntradaNGE e(1, m);
	nodo->incluirEntrada(e);
}


/*********************************************************************/

// lee las distintas transformaciones (rotación, escalado, trasladado y color)
void leerTransformacion(ifstream & in, NodoGE * nodo)
{
	char t;
	in >> t;
	ParMatriz4f par;
	double d1, d2, d3;
	in >> d1; in >> d2; in >> d3;
	Tupla3f tu(d1,d2,d3);
	par.tupla = tu;
	switch(t)
	{
		case 'r': // rotación
			par.tipo = 0;
			double r;
			in >> r; // se lee el ángulo
			par.angulo = r;
			break;
		case 's': // escalado
			par.tipo = 1; 
			break;
		case 't': // trasladado
			par.tipo = 2;
			break;
		case 'c': // color
			par.tipo = 3;
			break;
	};
	EntradaNGE e(2, par);
	nodo->incluirEntrada(e);
}


/*********************************************************************/

// lee un grado de libertad
void leerGradoLibertad(ifstream & in, NodoGE * nodo)
{
	char t;
	in >> t;
	ParMatriz4f par;
	double d1, d2, d3;
	in >> d1; in >> d2; in >> d3;
	Tupla3f tu(d1,d2,d3);
	par.tupla = tu;
	switch(t)
	{
		case 'r': // rotación
			par.tipo = 0;
			double r;
			in >> r;
			par.angulo = r;
			break;
		case 's': // escalado
			par.tipo = 1;
			break;
		case 't': // trasladado
			par.tipo = 2;
			break;
		case 'c': // color (el color también puede ser considerado un grado de libertad
					//       para poder así cambiar el color de una parte del grafo sin tener
					// 		 que cambiar el archivo .ge)
			par.tipo = 3;
			break;
	};
	EntradaNGE e(4, nodo->nGrados(), 0);
	nodo->incluirGrado(par);
	nodo->incluirEntrada(e);
}


/*********************************************************************/

}