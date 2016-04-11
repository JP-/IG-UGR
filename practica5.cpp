// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// ** Práctica 5  (implementación)
// **
// *********************************************************************


#include <GL/glut.h>
#include <iostream>

#include "error-ogl.hpp"
#include "practica5.hpp"

#include <vector>
#include <string>
#include <sstream>

#include "NodoGE.hpp"
#include "fuenteLuz.hpp"
#include "camara.hpp"

using namespace std;


FuenteLuz *luz;
vector<Camara> camaras;
int camaraActiva = 0;
int reproduciendoP5 = 0;

int pulsaciones = 0;

bool modoObservacion = false;
int girarCamara = 0;
pair<int, int> origen;
int modeloSeleccionado = -1;

bool redondeo(float a, float b)
{
	return (abs(a-b)<0.05);
}

Tupla3f interpretarPila(Tupla3f punto, stack<ParMatriz4f> pila)
{
	Tupla3f tmp = punto;
	while(!pila.empty())
	{
		if(pila.top().tipo == 2)
		{
			tmp = tmp + pila.top().tupla;
		}
		else if(pila.top().tipo == 0)
			tmp = Matriz4f('a', pila.top().angulo, pila.top().tupla)*tmp;
		else if(pila.top().tipo == 1)
			tmp = Tupla3f(tmp.coo[0]*pila.top().tupla.coo[0],tmp.coo[1]*pila.top().tupla.coo[1],tmp.coo[2]*pila.top().tupla.coo[2]);
		pila.pop();
	}
	return tmp;
}


GrafoEscena escena;
vector<ParMatriz4f*> gradosP5;

struct info
{
	Tupla3f color;
	Model * modelo;
	string nombre;
	Tupla3f puntoMedio;
};

vector<info> modelosP5;

void P5::Inicializar( int argc, char *argv[] )
{
	stack<ParMatriz4f> pila;
	ParMatriz4f par;
	info in;

	NodoGE * nodo1 = new NodoGE();

	// Rey
	par.tipo = 1; par.tupla = Tupla3f(0.2,0.2,0.2);
	nodo1->incluirEntrada(EntradaNGE(2,par));
	pila.push(par);
	par.tipo = 2; par.tupla = Tupla3f(0,5.6,0);
	nodo1->incluirEntrada(EntradaNGE(2,par));
	pila.push(par);
	par.tipo = 3; par.tupla = Tupla3f(0.8,0,0);
	nodo1->incluirEntrada(EntradaNGE(2,par));

	Model* m1 = new Model("plys/rey.ply", 100, true, true, true, Material("texturas/text-madera.jpg",0));
	in.puntoMedio = interpretarPila(m1->getPuntoMedio(), pila);
	in.color = par.tupla; in.modelo = m1; in.nombre = "Rey";
	modelosP5.push_back(in);
	
	nodo1->incluirEntrada(EntradaNGE(5,m1));

	escena.incluirEntrada(EntradaNGE(0,nodo1));


	// Peón
	while(!pila.empty()) pila.pop();
	NodoGE * nodo2 = new NodoGE();

	par.tipo = 1; par.tupla = Tupla3f(0.6,0.6,0.6);
	nodo2->incluirEntrada(EntradaNGE(2,par));
	pila.push(par);
	par.tipo = 2; par.tupla = Tupla3f(5,1.4,-2);
	nodo2->incluirEntrada(EntradaNGE(2,par));
	pila.push(par);
	par.tipo = 3; par.tupla = Tupla3f(0.2,0,0);
	nodo2->incluirEntrada(EntradaNGE(2,par));
	
	Model* m2 = new Model("plys/perfil.ply", 100, true, true, true, Material("texturas/text-madera.jpg",0));
	in.puntoMedio = interpretarPila(m2->getPuntoMedio(), pila);
	in.color = par.tupla; in.modelo = m2; in.nombre = "Peón";
	modelosP5.push_back(in);

	nodo2->incluirEntrada(EntradaNGE(5,m2));
	
	escena.incluirEntrada(EntradaNGE(0,nodo2));


	// Hormiga
	while(!pila.empty()) pila.pop();
	NodoGE * nodo3 = new NodoGE();

	par.tipo = 1; par.tupla = Tupla3f(0.1,0.1,0.1);
	nodo3->incluirEntrada(EntradaNGE(2,par));
	pila.push(par);
	par.tipo = 2; par.tupla = Tupla3f(-30,8,20);
	nodo3->incluirEntrada(EntradaNGE(2,par));
	pila.push(par);
	par.tipo = 0; par.tupla = Tupla3f(0,1,0); par.angulo = 40;
	nodo3->incluirEntrada(EntradaNGE(2,par));
	pila.push(par);
	par.tipo = 3; par.tupla = Tupla3f(0.9,0,0);
	nodo3->incluirEntrada(EntradaNGE(2,par));
	
	Model* m3 = new Model("plys/ant.ply", Material("texturas/text-rosa.jpg",1));
	in.puntoMedio = interpretarPila(m3->getPuntoMedio(), pila);
	in.color = par.tupla; in.modelo = m3; in.nombre = "Hormiga";
	modelosP5.push_back(in);
	
	nodo3->incluirEntrada(EntradaNGE(5,m3));
	
	escena.incluirEntrada(EntradaNGE(0,nodo3));


	// Robot
	NodoGE * nodo4 = new NodoGE("robot.ge");

	par.tipo = 3; par.tupla = Tupla3f(0.0,0,0);
	escena.incluirEntrada(EntradaNGE(2,par));
	par.tipo = 1; par.tupla = Tupla3f(0.1,0.1,0.1);
	escena.incluirEntrada(EntradaNGE(2,par));
	par.tipo = 2; par.tupla = Tupla3f(30,0,20);
	escena.incluirEntrada(EntradaNGE(2,par));
	par.tipo = 0; par.tupla = Tupla3f(0,1,0); par.angulo = -40;
	escena.incluirEntrada(EntradaNGE(2,par));
	escena.incluirEntrada(EntradaNGE(0,nodo4));



	escena.getGrados(gradosP5);

	Tupla3f luz_ambiental(1.0,1.0,1.0);
	Tupla3f luz_difusa(1.0,1.0,1.0);
	Tupla3f luz_especular(1.0,1.0,1.0);
	
	Tupla4f pos(0,2,5,1);
	vector<Tupla3f> vlp;
	vlp.push_back(luz_ambiental);
	vlp.push_back(luz_difusa);
	vlp.push_back(luz_especular);

	luz = new FuenteLuz(pos, vlp, 0, 0);

	
	camaras.push_back(Camara(true, false, Tupla3f(0,2,10), Tupla3f(0,2,0),0,0));
	camaras.push_back(Camara(true, false, Tupla3f(0,10,0), Tupla3f(0,0,0),0,0));
	camaras.push_back(Camara(true, false, Tupla3f(10,2,0), Tupla3f(0,2,0),0,0));
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 5 para dibujar los objetos

void P5::DibujarObjetos(int modo, bool normCaras, bool normVertices)
{
	luz->activar(0);
	
	escena.pintar();

	luz->desactivar(0);
}


bool P5::FGE_TeclaNormal(unsigned char tecla, int x_raton, int y_raton)
{
	bool redisp = true ;

	switch(toupper(tecla))
	{
		case 'O':
			modoObservacion ^= 1;
			if(modoObservacion)
				cout << "Ha entrado en modo observación, mueva el ratón para saber los objetos seleccionables." << endl;
			else
				cout << "Ha salido del modo observación." << endl;
			redisp = false;
			break;
		case 'A':
			camaras[camaraActiva].modificarPosicionX(-0.2);
			break;
		case 'D':
			camaras[camaraActiva].modificarPosicionX(0.2);
			break;
		case 'W':
			camaras[camaraActiva].modificarPosicionZ(0.2);
			break;
		case 'S':
			camaras[camaraActiva].modificarPosicionZ(-0.2);
			break;
		case 'R':
			camaras[camaraActiva].reiniciarPosicion();
			break;
		case '1':
			cout << "Cámara activa: 1" << endl;
			camaraActiva = 0;
			break;
		case '2':
			cout << "Cámara activa: 2" << endl;
			camaraActiva = 1;
			break;
		case '3':
			cout << "Cámara activa: 3 (perspectica ortogonal)" << endl;
			camaraActiva = 2;
			break;
		case 'P':
// 			if(camaras[camaraActiva].getModo() && modeloSeleccionado == 1)
// 			{
// 				reproduciendoP5 ^= 1;
// 				if(!reproduciendoP5)
// 					glutIdleFunc(fin);
// 				else
// 					glutIdleFunc(idle);
// 				break;
// 			}
		default:
			redisp = false;
			break;
	}

	return redisp;
}

void P5::fijarProyeccion(int ventana_tamx, int ventana_tamy)
{
	camaras[camaraActiva].fijarProyeccion(ventana_tamx, ventana_tamy);
}

void P5::fijarVista()
{
	camaras[camaraActiva].fijarVista();
}


void P5::clickRaton(int boton, int estado, int x, int y)
{
	bool selec = false;
	if(boton == GLUT_LEFT_BUTTON)
	{
		pulsaciones++;
		if(!(pulsaciones%2))
			pick(x,y);
		selec = true;
	}

	if(boton == GLUT_RIGHT_BUTTON)
	{
		origen.first = x;
		origen.second = y;
		girarCamara ^= 1;
	}


	if(selec)
		glutPostRedisplay();
}


void P5::ratonMovido(int x, int y)
{
	if(modoObservacion)
	{
		pick(x,y);
		glutPostRedisplay();
	}
}


void P5::ratonMovidoTeclaPulsada(int x, int y)
{
	if(!modoObservacion && girarCamara)
	{
		if((x-origen.first) > 1)
		{
			camaras[camaraActiva].modificaEjeY(0.01);
			origen.first = x;
			glutPostRedisplay();
		}
		else if((x-origen.first) < -1)
		{
			camaras[camaraActiva].modificaEjeY(-0.01);
			origen.first = x;
			glutPostRedisplay();
		}
		
		if((y-origen.second) > 1)
		{
			camaras[camaraActiva].modificaEjeX(0.01);
			origen.second = y;
			glutPostRedisplay();
		}
		else if((y-origen.second) < -1)
		{
			camaras[camaraActiva].modificaEjeX(-0.01);
			origen.second = y;
			glutPostRedisplay();
		}
	}
}


bool P5::pick(int x, int y)
{
	// deseleccionamos cualquier objeto que hubiera seleccionado
	for(int i=0; i<modelosP5.size(); i++)
		modelosP5[i].modelo->deseleccionar();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	escena.pintarSeleccion();
	GLint viewport[4];
	GLfloat pixel[3];
	glGetIntegerv(GL_VIEWPORT,viewport);
	glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_FLOAT,(void*)pixel);

	// seleccionamos, si procede
	return seleccionar(pixel[0]);
}



bool P5::seleccionar(float color)
{
	for(int i=0; i<modelosP5.size(); i++)
	{
		if(redondeo(color, modelosP5[i].color.coo[0]))
		{
			modelosP5[i].modelo->seleccionar();
			if(!modoObservacion)
			{
				cout << "Seleccionado el modelo: " << modelosP5[i].nombre << endl;
				camaras[camaraActiva].setPuntoAtencion(modelosP5[i].puntoMedio);
				cout << "Punto de atención: (" << modelosP5[i].puntoMedio.coo[0] << "," << modelosP5[i].puntoMedio.coo[1] << "," << modelosP5[i].puntoMedio.coo[2] << ")" << endl;
				camaras[camaraActiva].setModo(true);
				modeloSeleccionado = i;
			}
			return true;
		}
	}

	if(!modoObservacion)
	{
		cout << "No se ha seleccionado nada" << endl;
		camaras[camaraActiva].setPuntoAtencion(Tupla3f(0,0,0));
		camaras[camaraActiva].setModo(false);
		modeloSeleccionado = -1;
	}
	return false;
}


void P5::idle()
{
	gradosP5[0]->angulo += 1;
	modelosP5[1].puntoMedio = Matriz4f('y',gradosP5[0]->angulo)*modelosP5[1].puntoMedio;

	camaras[camaraActiva].setPuntoAtencion(modelosP5[1].puntoMedio);


	glutPostRedisplay();
}

void P5::fin() {}