// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// **
// ** Estructura de datos para guardar una camara
// **
// *********************************************************************


#ifndef IG_CAMARA
#define IG_CAMARA

#include <iostream>
#include "file_ply_stl.hpp"
#include <GL/glut.h>
#include "tuplas.hpp"
#include <vector>
#include "error-ogl.hpp"
#include "matriz4f.hpp"

using namespace std;

class Camara                // Imagen de textura
{
private:
	bool perspectiva;  // true -> camara en perspectiva, false -> camara en ortogonal
	bool examinar; 		// true -> modo examinar, false -> modo primera persona
	Tupla3f posicionObservador;  // modo primera persona
	Tupla3f posicionObservador_Inicial;  // modo primera persona
	Tupla3f posicionExaminar;
	Tupla3f posicionPuntoAtencion; // modo examinar
	Tupla3f posicionPuntoAtencion_Inicial; // modo examinar
	double alpha, beta; // alpha -> rotacion entorno al eje X, beta -> rotacion entorno al eje Y

	float zoom;
	float rotacionX, rotacionY;

	static const float frustum_dis_del, frustum_dis_tra, frustum_ancho;

public:
	
	Camara();
	Camara(bool pers, bool exam);
	Camara(bool pers, bool exam, Tupla3f posOb, Tupla3f posPunAt, float rotX, float rotY);
	
	void fijarVista(Tupla3f pA = Tupla3f(0,0,0));
	void fijarProyeccion(int ventana_tam_x, int ventana_tam_y);

	Tupla3f getObservador();
	Tupla3f getPuntoAtencion();
	float getEjeX();
	float getEjeY();
	void modificaEjeX(float incremento);
	void modificaEjeY(float incremento);
	void modificarPosicionX(float incremento);
	void modificarPosicionZ(float incremento);
	void setPuntoAtencion(Tupla3f pa);
	void setModo(bool ex);
	bool getModo();
	void reiniciarPosicion();
};


#endif
