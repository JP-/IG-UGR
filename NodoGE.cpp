// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** Juan Pablo Porcel Porcel (C)
// **
// ** Clase para guardar un nodo 
// **
// *********************************************************************

#include "NodoGE.hpp"
#include <fstream>
#include <GL/freeglut.h>
#include "gestor_GE.hpp"
#include "material.hpp"



vector<Model*> NodoGE::objetos;


NodoGE::NodoGE()
{
	
}


/*********************************************************************/

NodoGE::NodoGE(const char * archivo)
{
	ge::read(archivo, entradas);
}


/*********************************************************************/

void NodoGE::abrir(const char * archivo)
{
	ge::read(archivo, entradas);
}


/*********************************************************************/

NodoGE::NodoGE(const std::vector<EntradaNGE> ent)
{
	entradas = ent;

	if(objetos.empty())
		inicializaObjetos();
}


/*********************************************************************/

void NodoGE::incluirEntrada(EntradaNGE e)
{
	entradas.push_back(e);
}


/*********************************************************************/

int NodoGE::size()
{
	return entradas.size();
}


/*********************************************************************/

void NodoGE::incluirGrado(ParMatriz4f p)
{
	grados.push_back(p);
}


/*********************************************************************/

std::vector<EntradaNGE> NodoGE::getEntradas()
{
	return entradas;
}


/*********************************************************************/

void NodoGE::getGrados(vector<ParMatriz4f*> & vector)
{
	for(int i=0; i<grados.size(); i++)
	{
		vector.push_back(&grados[i]);
	}

	for(int k=0; k<entradas.size(); k++)
	{
		if(entradas[k].getTipo() == 0)
		{
			entradas[k].getNodo()->getGrados(vector);
		}
	}
}


/*********************************************************************/

int NodoGE::nGrados()
{
	int n = 0;
	for(int k=0; k<entradas.size(); k++)
	{
		if(entradas[k].getTipo() == 0)
		{
			n += entradas[k].getNodo()->nGrados();
		}
	}
	return grados.size() + n;
}


/*********************************************************************/

void NodoGE::pintar()
{
	glPushMatrix();
	for(unsigned i=0; i<entradas.size(); i++)
	{
		switch(entradas[i].getTipo())
		{
			case 0:	entradas[i].getNodo()->pintar();
				break;
			case 1:
				break;
			case 2: Transformacion(entradas[i].getParMatriz());
				break;
			case 3: PintarObjetoSimple(entradas[i].getObjeto(), false);
				break;
			case 4:
			{	ParMatriz4f par = grados[entradas[i].getGrado()];
				Transformacion(par);
				break; }
			case 5: entradas[i].getModelo()->draw();
				break;
		}
	}
	glPopMatrix();
}


void NodoGE::pintarSeleccion()
{
	glDisable(GL_DITHER); // deshabilita el degradado
	glDisable(GL_LIGHTING);
	glPushMatrix();
	for(unsigned i=0; i<entradas.size(); i++)
	{
		switch(entradas[i].getTipo())
		{
			case 0:	entradas[i].getNodo()->pintarSeleccion();
				break;
			case 1:
				break;
			case 2: Transformacion(entradas[i].getParMatriz());
				break;
			case 3: PintarObjetoSimple(entradas[i].getObjeto(), true);
				break;
			case 4:
			{	ParMatriz4f par = grados[entradas[i].getGrado()];
				Transformacion(par);
				break; }
			case 5: entradas[i].getModelo()->draw(2, false, false, true);
				break;				
		}
	}
	glPopMatrix();
	glEnable(GL_DITHER);
	glEnable(GL_LIGHTING);
}


/*********************************************************************/

void NodoGE::Transformacion(ParMatriz4f par)
{
	switch(par.tipo)
	{
		case 0:
			glRotatef(par.angulo, par.tupla.coo[0], par.tupla.coo[1], par.tupla.coo[2]);
			break;
		case 1:
			glScalef(par.tupla.coo[0], par.tupla.coo[1], par.tupla.coo[2]);
			break;
		case 2:
			glTranslatef(par.tupla.coo[0], par.tupla.coo[1], par.tupla.coo[2]);
			break;
		case 3:
			glColor3f(par.tupla.coo[0], par.tupla.coo[1], par.tupla.coo[2]);
			GLfloat color[4] = { par.tupla.coo[0], par.tupla.coo[1], par.tupla.coo[2] } ;
			glMaterialfv( GL_FRONT, GL_EMISSION, color ) ;
			break;
	}
}


/*********************************************************************/

void NodoGE::PintarObjetoSimple(int o, bool seleccion)
{
	if(objetos.empty())
		inicializaObjetos();
	objetos[o]->draw(2, false, false, seleccion);
}


/*********************************************************************/

void NodoGE::inicializaObjetos()
{
	objetos.push_back(new Model("plys/cubo.ply", Material("texturas/text-metal.jpg",1)));  // 0
	objetos.push_back(new Model("plys/esfera.ply", Material("texturas/text-metal.jpg",1))); // 1
	objetos.push_back(new Model("plys/cono.ply", Material("texturas/text-metal.jpg",1))); // 2
	objetos.push_back(new Model("plys/anillo.ply", Material("texturas/text-metal.jpg",1))); // 3
	objetos.push_back(new Model("plys/cilindro.ply", Material("texturas/text-metal.jpg",1))); // 4
	objetos.push_back(new Model("plys/semisphere.ply", Material("texturas/text-metal.jpg",1))); // 5
	objetos.push_back(new Model("plys/piece_1.ply", Material("texturas/text-metal.jpg",1))); // 6
	objetos.push_back(new Model("plys/piece_2.ply", Material("texturas/text-metal.jpg",1))); // 7
	objetos.push_back(new Model("plys/piece_3.ply", Material("texturas/text-metal.jpg",1))); // 8
	objetos.push_back(new Model("plys/piece_4.ply", Material("texturas/text-metal.jpg",1))); // 9

	objetos.push_back(new Model("plys/head.ply", Material("texturas/text-metal.jpg",1))); // 10
}

/*********************************************************************/

EntradaNGE::EntradaNGE(int t, NodoGE* nGE)
{
	tipo = t;
	subesc = nGE;
}


/*********************************************************************/

EntradaNGE::EntradaNGE(int t, Matriz4f m)
{
	tipo = t;
	matriz = m;
}


/*********************************************************************/

EntradaNGE::EntradaNGE(int t, ParMatriz4f p)
{
	tipo = t;
	pars_matriz = p;
}


/*********************************************************************/

EntradaNGE::EntradaNGE(int t, int n)
{
	tipo = t;
	obj_sim = n;
}


/*********************************************************************/

EntradaNGE::EntradaNGE(int t, int n, int k)
{
	tipo = t;
	grado = n;
}

EntradaNGE::EntradaNGE(int t, Model * m)
{
	tipo = t;
	modelo = m;
}


/*********************************************************************/

int EntradaNGE::getTipo()
{
	return tipo;
}


/*********************************************************************/

ParMatriz4f EntradaNGE::getParMatriz()
{
	return pars_matriz;
}


/*********************************************************************/

int EntradaNGE::getObjeto()
{
	return obj_sim;
}


/*********************************************************************/

int EntradaNGE::getGrado()
{
	return grado;
}


/*********************************************************************/

NodoGE* EntradaNGE::getNodo()
{
	return subesc;
}


/*********************************************************************/


Model* EntradaNGE::getModelo()
{
	return modelo;
}