// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// ** Práctica 1  (implementación)
// **
// *********************************************************************


#include <GL/glut.h>
#include <iostream>

#include "error-ogl.hpp"
#include "practica4.hpp"
#include "jpg_imagen.hpp"

#include <vector>
#include <string>
#include <sstream>

#include "fuenteLuz.hpp"
#include "tuplas.hpp"
#include "material.hpp"
#include "coleccionFL.hpp"

using namespace std;



Model lataINF, lataSUP;

vector<Model> modelos;
int nlatas = 8;

Model peon0, peon1, peon2, lata, esfera;

FuenteLuz *luzPosicional, *luzDireccional;

ColeccionFL fuentes;


void P4::Inicializar( int argc, char *argv[] )
{
	// tapas
	Tupla3f tce(0,0,0); // Color gris
	Tupla3f tca(0.6,0.6,0.6);
	Tupla3f tcd(0.9,0.9,0.9);
	Tupla3f tces(0.1,0.1,0.1);
	vector<Tupla3f> vt;
	vt.push_back(tce);
	vt.push_back(tca);
	vt.push_back(tcd);
	vt.push_back(tces);
	float tee = 3;
	Material mTapa(vt, tee);
	lataINF.crearRevolucion("plys/lata-pinf.ply", 30, false, true, false, mTapa);
	lataSUP.crearRevolucion("plys/lata-psup.ply", 30, false, true, false, mTapa);

	// lata
	Tupla3f clce(0,0,0); // Color blanco puro
	Tupla3f clca(0.5,0.5,0.5);
	Tupla3f clcd(1,1,1);
	Tupla3f clces(0.1,0.1,0.1);
	vector<Tupla3f> vcl;
	vcl.push_back(clce);
	vcl.push_back(clca);
	vcl.push_back(clcd);
	vcl.push_back(clces);
	float clee = 50.0;
	Material mLata(vcl, clee, "texturas/text-lata-1.jpg");
	lata.crearRevolucion("plys/lata-pcue.ply", 30, false, false, true, mLata);

	// peón madera
	Tupla3f pmce(0,0,0); // Color blanco puro
	Tupla3f pmca(0.5,0.5,0.5);
	Tupla3f pmcd(1,1,1);
	Tupla3f pmces(0.7,0.7,0.7);
	vector<Tupla3f> vpm;
	vpm.push_back(pmce);
	vpm.push_back(pmca);
	vpm.push_back(pmcd);
	vpm.push_back(pmces);
	float pmee = 5.0;
	Material material_peon_madera(vpm, pmee, "texturas/text-madera.jpg", 1);
	peon0.crearRevolucion("plys/perfil.ply", 60, true, true, false, material_peon_madera);
	
	// peón blanco
	Tupla3f pbce(0,0,0); // Color blanco
	Tupla3f pbca(0.5, 0.5, 0.5);
	Tupla3f pbcd(0.9,0.9,0.9);
	Tupla3f pbces(0,0,0);
	vector<Tupla3f> vpb;
	vpb.push_back(pbce);
	vpb.push_back(pbca);
	vpb.push_back(pbcd);
	vpb.push_back(pbces);
	float pbee = 0.0;
	Material material_peon_blanco(vpb, pbee);
	peon1.crearRevolucion("plys/perfil.ply", 60, true, true, false, material_peon_blanco);
	
	// peón negro
	Tupla3f pnce(0.0,0.0,0.0); // Color negro
	Tupla3f pnca(0,0,0);
	Tupla3f pncd(0.2,0.2,0.2);
	Tupla3f pnces(0.1,0.1,0.1);
	vector<Tupla3f> vpn;
	vpn.push_back(pnce);
	vpn.push_back(pnca);
	vpn.push_back(pncd);
	vpn.push_back(pnces);
	float pnee = 50.0;
	Material material_peon_negro(vpn, pnee);	
	peon2.crearRevolucion("plys/perfil.ply", 60, true, true, false, material_peon_negro);
	
	
	Material mEsfera;
	Tupla3f color[4] = {Tupla3f(1, 1, 0), Tupla3f(1, 1, 0), Tupla3f(1, 1, 0), Tupla3f(0, 0, 0)};
	vector<Tupla3f> col(color, color+sizeof(color)/sizeof(Tupla3f));
	float e = 0.0;
	mEsfera.definir(col, e);
	esfera.leePLY("plys/sphere.ply", mEsfera);
	
	
	Tupla3f luz_posicional_componente_ambiental(1.0,1.0,1.0);
	Tupla3f luz_posicional_componente_difusa(1.0,1.0,1.0);
	Tupla3f luz_posicional_componente_especular(1.0,1.0,1.0);
	Tupla3f luz_direccional_componente_ambiental(0.0,0.0,0.0);
	Tupla3f luz_direccional_componente_difusa(1.0,1.0,1.0);
	Tupla3f luz_direccional_componente_especular(1.0,1.0,1.0);
	
	Tupla4f pos(0,2,5,1);
	Tupla4f pos1(3,1,1.5,0);
	Tupla3f a(0.2,0.2,0.2), b(1,1,1), c(1,1,1);
	vector<Tupla3f> vlp;
	vlp.push_back(luz_posicional_componente_ambiental);
	vlp.push_back(luz_posicional_componente_difusa);
	vlp.push_back(luz_posicional_componente_especular);
	vector<Tupla3f> vld;
	vld.push_back(luz_direccional_componente_ambiental);
	vld.push_back(luz_direccional_componente_difusa);
	vld.push_back(luz_direccional_componente_especular);
	
	float alpha = 0;
	float beta = 0;
	
	luzPosicional = new FuenteLuz(pos, vlp, alpha, beta);
	luzDireccional = new FuenteLuz(pos1, vld, 0, 0);
	fuentes.insertar(luzPosicional);
	fuentes.insertar(luzDireccional);
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 4 para dibujar los objetos

void P4::DibujarObjetos(int modo, bool normCaras, bool normVertices)
{
	fuentes.activar();
// 	glEnable(GL_LIGHT0);

	glPushMatrix();
	glRotatef( fuentes.get(0)->getAlpha(), 1.0, 0.0, 0.0 ) ;
	glRotatef( fuentes.get(0)->getBeta(), 0.0, 1.0, 0.0 ) ;
	glTranslatef(fuentes.get(0)->getPos()->coo[0],fuentes.get(0)->getPos()->coo[1],fuentes.get(0)->getPos()->coo[2]);
	glScalef(0.1,0.1,0.1);
	esfera.draw(modo, normCaras, normVertices);
	glPopMatrix();

	glPushMatrix();
	glRotatef( fuentes.get(1)->getAlpha(), 1.0, 0.0, 0.0 ) ;
	glRotatef( fuentes.get(1)->getBeta(), 0.0, 1.0, 0.0 ) ;
	glTranslatef(fuentes.get(1)->getPos()->coo[0],fuentes.get(1)->getPos()->coo[1],fuentes.get(1)->getPos()->coo[2]);
	glScalef(0.1,0.1,0.1);
	esfera.draw(modo, normCaras, normVertices);
	glPopMatrix();

	// lata
	lataINF.draw(modo, normCaras, normVertices);
	lataSUP.draw(modo, normCaras, normVertices);
	lata.draw(modo, normCaras, normVertices);

	// peón madera
	glPushMatrix();
	glTranslatef(0,0,1);
	glScalef(0.3,0.3,0.3);
	glTranslatef(0,1.4,0);
	peon0.draw(modo, normCaras, normVertices);
	glPopMatrix();

	// peón blanco
	glPushMatrix();
	glTranslatef(1,0,1);
	glScalef(0.3,0.3,0.3);
	glTranslatef(0,1.4,0);
	peon1.draw(modo, normCaras, normVertices);
	glPopMatrix();

	// peón negro
	glPushMatrix();
	glTranslatef(2,0,1);
	glScalef(0.3,0.3,0.3);
	glTranslatef(0,1.4,0);
	peon2.draw(modo, normCaras, normVertices);
	glPopMatrix();

	fuentes.desactivar();
}


bool P4::FGE_TeclaNormal(unsigned char tecla, int x_raton, int y_raton)
{
	bool redisp = true ;

	switch (toupper(tecla))
	{
		case 'L':
		{
			float p = fuentes.get(0)->getBeta();
			fuentes.get(0)->setBeta(p + 1);
			fuentes.activar();
			break;
		}
		case 'J':
		{
			float p = fuentes.get(0)->getBeta();
			fuentes.get(0)->setBeta(p - 1);
			fuentes.activar();
			break;
		}
		case 'K':
		{
			float p = fuentes.get(0)->getAlpha();
			fuentes.get(0)->setAlpha(p + 1);
			fuentes.activar();
			break;
		}
		case 'I':
		{
			float p = fuentes.get(0)->getAlpha();
			fuentes.get(0)->setAlpha(p - 1);
			fuentes.activar();
			break;
		}
		default:
			redisp = false;
			break;
	}

	return redisp;
}
