// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// **
// ** Estructura de datos para guardar un material
// **
// *********************************************************************


#ifndef IG_MATERIAL
#define IG_MATERIAL

#include <iostream>
#include "tuplas.hpp"
#include "matrizR3D.hpp"
#include "file_ply_stl.hpp"
#include <vector>
#include <GL/glut.h>
#include <cmath>
#include "jpg_imagen.hpp"

#include "textura.hpp"



class Material       
{
private:
	Textura * text ;        // puntero a la textura (NULL si no hay)
	vector<Tupla3f>   colores ;    // colores: 0=emisión, 1=ambiental, 2=difuso, 3=especular
	float     exponente ;   // exponente para la componente pseudo-especular

public:

	Material();
	Material(string imagen, int modo = 0);
	Material(vector<Tupla3f> c, float e, string imagen = "", int modo = 0);
	void definir(vector<Tupla3f> c, float e, string imagen = "", int modo = 0);
	void activar();          // activa textura en el cauce fijo
	bool textura();
	int getModo();
	void pintarColores();

	Material & operator = (const Material & otro);
	Material operator * (float cte);
};

#endif