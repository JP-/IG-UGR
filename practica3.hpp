// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// **
// ** Práctica 2  (declaraciones públicas)
// **
// *********************************************************************

#ifndef IG_PRACTICA3_HPP
#define IG_PRACTICA3s_HPP

#include "model.hpp"

namespace P3
{
	void DibujarObjetos(int modo, bool normCaras, bool normVertices) ;
	void idle();
	void fin();
	void Inicializar( int argc, char *argv[] ) ;
	bool FGE_TeclaNormal(unsigned char tecla, int x_raton, int y_raton);
	bool FGE_TeclaEspecial(int tecla, int x_raton, int y_raton);
}

#endif
