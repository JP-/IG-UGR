#include "camara.hpp"


const float Camara::frustum_dis_del         = 0.1,
			Camara::frustum_dis_tra         = 100.0,
			Camara::frustum_ancho           = 0.05;


//Compat method: gluLookAt deprecated
void util_compat_gluLookAt(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat lookAtX,
						   GLfloat lookAtY, GLfloat lookAtZ, GLfloat upX, GLfloat upY, GLfloat upZ)
{
	Tupla3f x, y, z;
	z = normalized(Tupla3f(eyeX-lookAtX, eyeY-lookAtY, eyeZ-lookAtZ));
	y = Tupla3f(upX, upY, upZ);
	x = y * z;
	y = z * x;
	x = normalized(x);
	y = normalized(y);
	// mat is given transposed so OpenGL can handle it.
	GLfloat mat[16] =	{x.coo[0], y.coo[0], z.coo[0], 0,
						x.coo[1], y.coo[1], z.coo[1], 0,
						x.coo[2], y.coo[2], z.coo[2], 0,
						-eyeX, -eyeY, -eyeZ, 1};
	glMultMatrixf(mat);
}


Camara::Camara()
{
	perspectiva = true;
	examinar = true;
	posicionObservador = Tupla3f(0.0,0.0,10.0);
	posicionObservador_Inicial = Tupla3f(0.0,0.0,10.0);
	posicionPuntoAtencion = Tupla3f();
	posicionPuntoAtencion_Inicial = Tupla3f();
	rotacionX = 0;
	rotacionY = 0;
	zoom = 1.0;
}

Camara::Camara(bool pers, bool exam)
{
	perspectiva = pers;
	examinar = exam;
	posicionObservador = Tupla3f(0.0,0.0,10.0);
	posicionObservador_Inicial = Tupla3f(0.0,0.0,10.0);
	posicionPuntoAtencion = Tupla3f(0.0,0.0,0.0);
	posicionPuntoAtencion_Inicial = Tupla3f(0.0,0.0,0.0);
	rotacionX = 0;
	rotacionY = 0;
	zoom = 1.0;
}


Camara::Camara(bool pers, bool exam, Tupla3f posOb, Tupla3f posPunAt, float rotX, float rotY)
{
	perspectiva = pers;
	examinar = exam;
	posicionObservador = posOb;
	posicionObservador_Inicial = posOb;
	posicionPuntoAtencion = posPunAt;
	posicionPuntoAtencion_Inicial = posPunAt;
	rotacionX = rotX;
	rotacionY = rotY;
	zoom = 1.0;
}

void Camara::fijarVista(Tupla3f pA)
{
	CError();

	Tupla3f vup(0.0,0.0,0.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (examinar)
	{
		Matriz4f * m1, * m2;
		vup.coo[1] = 1.0;
		Tupla3f direccion = normalized(posicionObservador - posicionPuntoAtencion);
// 		m1 = new Matriz4f('a', -rotacionX, normalized(direccion*vup+posicionPuntoAtencion));
// 		m2 = new Matriz4f('a', -rotacionY, normalized(posicionPuntoAtencion+vup));
		m1 = new Matriz4f('y', rotacionY);
		m2 = new Matriz4f('x', rotacionX);
		posicionObservador = *m1 * *m2 * posicionExaminar;
	}
	else // Modo Primera persona
	{
		Tupla3f vector_ojo_foco = posicionPuntoAtencion_Inicial - posicionObservador;
		Matriz4f * m1, * m2;
		int menor = 2;
		if(abs(vector_ojo_foco.coo[1]) > abs(vector_ojo_foco.coo[menor]))
			menor = 1;
		if(abs(vector_ojo_foco.coo[0]) > abs(vector_ojo_foco.coo[menor]))
			menor = 0;
		if(menor == 2)
		{
			m1 = new Matriz4f('y', -rotacionY);
			m2 = new Matriz4f('x', -rotacionX);
			vup.coo[1] = 1.0;
		}
		else if(menor == 0)
		{
			m1 = new Matriz4f('y', -rotacionY);
			m2 = new Matriz4f('z', rotacionX);
			vup.coo[1] = 1.0;
		}
		else if(menor == 1)
		{
			m1 = new Matriz4f('z', rotacionY);
			m2 = new Matriz4f('x', rotacionX);
			vup.coo[2] = -1.0;
		}
		vector_ojo_foco = *m1 * *m2 * vector_ojo_foco;
		posicionPuntoAtencion = posicionObservador + vector_ojo_foco;
	}

	gluLookAt(posicionObservador.coo[0], posicionObservador.coo[1], posicionObservador.coo[2],
				  posicionPuntoAtencion.coo[0], posicionPuntoAtencion.coo[1], posicionPuntoAtencion.coo[2],
				vup.coo[0],vup.coo[1],vup.coo[2]);

// 	util_compat_gluLookAt(posicionObservador.coo[0], posicionObservador.coo[1], posicionObservador.coo[2],
// 			  posicionPuntoAtencion.coo[0], posicionPuntoAtencion.coo[1], posicionPuntoAtencion.coo[2],
// 			  0.0,1.0,0.0);
	
	CError();
}


void Camara::fijarProyeccion(int ventana_tam_x, int ventana_tam_y)
{
	CError();

	const GLfloat ratioYX = GLfloat( ventana_tam_y )/GLfloat( ventana_tam_x );
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if(perspectiva)
	{
		glFrustum
		(
			-frustum_ancho,
			+frustum_ancho,
			-frustum_ancho*ratioYX,
			+frustum_ancho*ratioYX,
			+frustum_dis_del,
			+frustum_dis_tra
		);
	}
	else // proyección ortogonal
	{
		glOrtho
		(
			-frustum_ancho*100-zoom,
			+frustum_ancho*100-zoom,
			-frustum_ancho*ratioYX*100-zoom,
			+frustum_ancho*ratioYX*100-zoom,
			+frustum_dis_del,
			+frustum_dis_tra
		);
	}

	
	
	CError();
}

Tupla3f Camara::getObservador()
{
	return posicionObservador;
}

Tupla3f Camara::getPuntoAtencion()
{
	return posicionPuntoAtencion;
}

float Camara::getEjeX()
{
	return rotacionX;
}

float Camara::getEjeY()
{
	return rotacionY;
}

void Camara::modificaEjeX(float incremento)
{
	rotacionX += incremento;
	// Limitamos a [-Pi,Pi] la cámara en Y para que no dé saltos.
	// Le quitamos 0.001 para que no llegue al límite
	if(rotacionX <= -M_PI/2)
		rotacionX = -M_PI/2 + 0.001;
	else if(rotacionX >= M_PI/2)
		rotacionX = M_PI/2 - 0.001;
}

void Camara::modificaEjeY(float incremento)
{
	rotacionY += incremento;
}

void Camara::modificarPosicionX(float incremento)
{
	if (!examinar)
	{
		Tupla3f pov = posicionPuntoAtencion - posicionObservador;
		Tupla3f vup = Tupla3f(0,0,0);
		int menor = 2;
		if(pov.coo[1] < pov.coo[menor])
			menor = 1;
		if(pov.coo[0] < pov.coo[menor])
			menor = 0;
		if(menor == 2)
		{
			vup.coo[1] = 1.0;
		}
		else if(menor == 0)
		{
			vup.coo[1] = 1.0;
		}
		else if(menor == 1)
		{
			vup.coo[2] = -1.0;
		}

		// la direccion sera el producto vectorial de esta direccion por el vup
		Tupla3f direccion = normalized(pov*vup);
		
		posicionObservador = posicionObservador + (direccion * incremento);
		posicionPuntoAtencion_Inicial = posicionPuntoAtencion_Inicial + (direccion * incremento);
	}
	else
	{
		zoom += incremento;
	}
}

void Camara::modificarPosicionZ(float incremento)
{
	if (!examinar)
	{
		Tupla3f pov = posicionPuntoAtencion - posicionObservador;		
		Tupla3f direccion = normalized(pov);
		
		posicionObservador = posicionObservador + (direccion * incremento);
		posicionPuntoAtencion_Inicial = posicionPuntoAtencion_Inicial + (direccion * incremento);
	}
}

void Camara::setPuntoAtencion(Tupla3f pa)
{
	posicionPuntoAtencion = pa;
}

void Camara::setModo(bool ex)
{
	if(ex == true)
		cout << "Modo examinar" << endl;
	examinar = ex;
	posicionExaminar = posicionObservador;
	rotacionX = 0;
	rotacionY = 0;
}


bool Camara::getModo()
{
	return examinar;
}

void Camara::reiniciarPosicion()
{
	posicionObservador = posicionObservador_Inicial;
	posicionPuntoAtencion = posicionPuntoAtencion_Inicial;
	rotacionX = 0;
	rotacionY = 0;
}