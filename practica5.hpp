// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// **
// ** Práctica 5  (declaraciones públicas)
// **
// *********************************************************************

#ifndef IG_PRACTICA5_HPP
#define IG_PRACTICA5_HPP

#include "model.hpp"

namespace P5
{
	void DibujarObjetos(int modo, bool normCaras, bool normVertices) ;
	void Inicializar( int argc, char *argv[] ) ;
	bool FGE_TeclaNormal(unsigned char tecla, int x_raton, int y_raton);
	void fijarProyeccion(int ventana_tamx, int ventana_tamy);
	void fijarVista();

	void clickRaton(int boton, int estado, int x, int y);
	void ratonMovido(int x, int y);
	void ratonMovidoTeclaPulsada(int x, int y);
	bool pick(int x, int y);
	bool seleccionar(float color);
	void idle();
	void fin();
}

#endif
