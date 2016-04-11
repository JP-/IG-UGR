// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** Juan Pablo Porcel Porcel (C)
// **
// ** Práctica 3
// **
// *********************************************************************

#include <GL/glut.h>
#include <iostream>

#include "error-ogl.hpp"
#include "practica3.hpp"
#include "NodoGE.hpp"

#include <vector>

using namespace std;



Model modelo3;
GrafoEscena grafo;
vector<ParMatriz4f*> grados;
int sentidoRI = 1;
int sentidoRD = 1;
int sentidoAI = 1;
int sentidoAD = 1;

int sentidoRpla = 1;
int sentidoApla = 1;

int reproduciendo = 0;


int n = 0;



void P3::Inicializar( int argc, char *argv[] )
{
	grafo.abrir("grafo.ge");
	grafo.getGrados(grados);
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 3  para dibujar los objetos

void P3::DibujarObjetos(int modo, bool normCaras, bool normVertices)
{
	grafo.pintar();
}


void P3::idle()
{
	if(sentidoRpla)
	{
		grados[0]->tupla.coo[0] += 0.5;
		grados[8]->tupla.coo[2] -= 0.125;
		if(grados[0]->tupla.coo[0] == 3)
			sentidoRpla ^= 1;
	}
	else
	{
		grados[0]->tupla.coo[0] -= 0.5;
		grados[8]->tupla.coo[2] += 0.125;
		if(grados[0]->tupla.coo[0] == -3)
			sentidoRpla ^= 1;
	}
	if(sentidoRD)
	{
		grados[1]->angulo += 5;
		grados[10]->tupla.coo[1] += -0.025;
	}
	else
	{
		grados[1]->angulo -= 5;
		grados[10]->tupla.coo[1] += 0.025;
	}
	if(grados[1]->angulo == 60 || grados[1]->angulo == 0)
		sentidoRD ^= 1;
	if(sentidoRI)
	{
		grados[2]->angulo += 5;
		grados[9]->tupla.coo[1] += -0.025;
	}
	else
	{
		grados[2]->angulo -= 5;
		grados[9]->tupla.coo[1] += 0.025;
	}
	if(grados[2]->angulo == 60 || grados[2]->angulo == 0)
		sentidoRI ^= 1;

	if(sentidoApla)
	{
		grados[4]->tupla.coo[0] += 0.5;
		grados[11]->tupla.coo[2] -= 0.125;
		if(grados[4]->tupla.coo[0] == 3)
			sentidoApla ^= 1;
	}
	else
	{
		grados[4]->tupla.coo[0] -= 0.5;
		grados[11]->tupla.coo[2] += 0.125;
		if(grados[4]->tupla.coo[0] == -3)
			sentidoApla ^= 1;
	}
	if(sentidoApla)
	{
		grados[4]->tupla.coo[0] += 0.5;
		grados[11]->tupla.coo[2] -= 0.125;
		if(grados[4]->tupla.coo[0] == 3)
			sentidoApla ^= 1;
	}
	else
	{
		grados[4]->tupla.coo[0] -= 0.5;
		grados[11]->tupla.coo[2] += 0.125;
		if(grados[4]->tupla.coo[0] == -3)
			sentidoApla ^= 1;
	}
	if(sentidoAD)
	{
		grados[6]->angulo += 5;
		grados[12]->tupla.coo[1] += -0.025;
	}
	else
	{
		grados[6]->angulo -= 5;
		grados[12]->tupla.coo[1] += 0.025;
	}
	if(grados[6]->angulo == 60 || grados[6]->angulo == 0)
		sentidoAD ^= 1;
	if(sentidoAI)
	{
		grados[5]->angulo += 5;
		grados[13]->tupla.coo[1] += -0.025;
	}
	else
	{
		grados[5]->angulo -= 5;
		grados[13]->tupla.coo[1] += 0.025;
	}
	if(grados[5]->angulo == 60 || grados[5]->angulo == 0)
		sentidoAI ^= 1;


	if(n++ == 40)
		n = 0;

	if(n == 5)
		grados[3]->tupla.coo[1] = 2;
	else if(n == 15)
		grados[3]->tupla.coo[1] = 0;
	else if(n == 21)
		grados[7]->tupla.coo[1] = 2;
	else if(n == 33)
		grados[7]->tupla.coo[1] = 0;

	glutPostRedisplay();
}


/*********************************************************************/

void P3::fin() {}


/*********************************************************************/

bool P3::FGE_TeclaNormal(unsigned char tecla, int x_raton, int y_raton)
{
	bool redisp = true ;
	
	switch (toupper(tecla))
	{
		case 'A':
		{
			ParMatriz4f* par = grados[0];
			ParMatriz4f* par1 = grados[8];
			if(par->tupla.coo[0] != 3)
			{
				par->tupla.coo[0] += 1;
				par1->tupla.coo[2] -= 0.25;
			}
			break;
		}
		case 'D':
		{
			ParMatriz4f* par = grados[0];
			ParMatriz4f* par1 = grados[8];
			if(par->tupla.coo[0] != -3)
			{
				par->tupla.coo[0] -= 1;
				par1->tupla.coo[2] += 0.25;
			}
			break;
		}
		case 'S':
		{
			ParMatriz4f* par = grados[0];
			ParMatriz4f* par1 = grados[8];
			if(par->tupla.coo[2] != 1)
			{
				par->tupla.coo[2] = 1;
				par1->tupla.coo[0] = +0.25;
			}
			else
			{
				par->tupla.coo[2] = 0;
				par1->tupla.coo[0] = 0;
			}
			break;
		}
		case 'C':
		{
			ParMatriz4f* par = grados[1];
			ParMatriz4f* par1 = grados[10];
			if(sentidoRD)
			{
				par->angulo += 10;
				par1->tupla.coo[1] += -0.05;
			}
			else
			{
				par->angulo -= 10;
				par1->tupla.coo[1] += 0.05;
			}
			if(par->angulo == 60 || par->angulo == 0)
				sentidoRD ^= 1;
			break;
		}
		case 'X':
		{
			ParMatriz4f* par = grados[2];
			ParMatriz4f* par1 = grados[9];
			if(sentidoRI)
			{
				par->angulo += 10;
				par1->tupla.coo[1] += -0.05;
			}
			else
			{
				par->angulo -= 10;
				par1->tupla.coo[1] += 0.05;
			}
			if(par->angulo == 60 || par->angulo == 0)
				sentidoRI ^= 1;
			break;
		}
		case 'F':
		{
			ParMatriz4f* par = grados[3];
			if(par->tupla.coo[1] != 2)
				par->tupla.coo[1] = 2;
			else
				par->tupla.coo[1] = 0;
			break;
		}
		case 'J':
		{
			ParMatriz4f* par = grados[4];
			ParMatriz4f* par1 = grados[11];
			if(par->tupla.coo[0] != -3)
			{
				par->tupla.coo[0] -= 1;
				par1->tupla.coo[2] += 0.25;
			}
			break;
		}
		case 'L':
		{
			ParMatriz4f* par = grados[4];
			ParMatriz4f* par1 = grados[11];
			if(par->tupla.coo[0] != 3)
			{
				par->tupla.coo[0] += 1;
				par1->tupla.coo[2] -= 0.25;
			}
			break;
		}
		case 'K':
		{
			ParMatriz4f* par = grados[4];
			ParMatriz4f* par1 = grados[11];
			if(par->tupla.coo[2] != 1)
			{
				par->tupla.coo[2] = 1;
				par1->tupla.coo[0] = +0.25;
			}
			else
			{
				par->tupla.coo[2] = 0;
				par1->tupla.coo[0] = 0;
			}
			break;
		}
		case 'N':
		{
			ParMatriz4f* par = grados[6];
			ParMatriz4f* par1 = grados[12];
			if(sentidoAI)
			{
				par->angulo += 10;
				par1->tupla.coo[1] += -0.05;
			}
			else
			{
				par->angulo -= 10;
				par1->tupla.coo[1] += 0.05;
			}
			if(par->angulo == 60 || par->angulo == 0)
				sentidoAI ^= 1;
			break;
		}
		case 'M':
		{
			ParMatriz4f* par = grados[5];
			ParMatriz4f* par1 = grados[13];
			if(sentidoAD)
			{
				par->angulo += 10;
				par1->tupla.coo[1] += -0.05;
			}
			else
			{
				par->angulo -= 10;
				par1->tupla.coo[1] += 0.05;
			}
			if(par->angulo == 60 || par->angulo == 0)
				sentidoAD ^= 1;
			break;
		}
		case 'H':
		{
			ParMatriz4f* par = grados[7];
			if(par->tupla.coo[1] != 2)
				par->tupla.coo[1] = 2;
			else
				par->tupla.coo[1] = 0;
			break;
		}
		case 'P':  // animación
		{
			reproduciendo ^= 1;
			if(!reproduciendo)
				glutIdleFunc(fin);
			else
				glutIdleFunc(idle);
			break;
		}
		default:
			redisp = false;
			break;
	}

	return redisp;
}


/*********************************************************************/

bool P3::FGE_TeclaEspecial(int tecla, int x_raton, int y_raton)
{
	bool redisp = true ;
	
	switch (toupper(tecla))
	{

		default:
			redisp = true;
			break;
	}

	return redisp;
}


/*********************************************************************/