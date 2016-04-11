// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// **
// ** Práctica 2  (declaraciones públicas)
// **
// *********************************************************************

#ifndef IG_PRACTICA4_HPP
#define IG_PRACTICA4_HPP

#include "model.hpp"

namespace P4
{
	void DibujarObjetos(int modo, bool normCaras, bool normVertices) ;
	void Inicializar( int argc, char *argv[] ) ;
	bool FGE_TeclaNormal(unsigned char tecla, int x_raton, int y_raton);
}

#endif
