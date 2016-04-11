// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// **
// ** Estructura de datos para guardar una textura
// **
// *********************************************************************


#ifndef IG_COLECCIONFL
#define IG_COLECCIONFL

#include <iostream>
#include "file_ply_stl.hpp"
#include <GL/glut.h>
#include "tuplas.hpp"
#include <vector>
#include "fuenteLuz.hpp"


class ColeccionFL
{
private:
	vector<FuenteLuz*> fuentes;

public:

	void insertar(FuenteLuz* fl);
	void activar();
	void desactivar();
	FuenteLuz* get(int);
};

#endif