#include "model.hpp"

#define S 2  // constante multiplicativa color en seleccion


/*********************************************************************/

// Constructor para cargar un modelo definido en un archivo ply
Model::Model(const char * archivo, Material m)
{
	// El método creaPLY() nos abre el archivo PLY y lee el modelo
	creaPLY(archivo);

	material = m;
	materialSeleccionado = m*S;

	seleccionado = false;

	calcularNormalesCaras();
	calcularNormalesVertices();
	calcularBaricentros();
	calcularPuntoMedio();
}


/*********************************************************************/

// Constructor para crear un modelo en revolución dado un perfil
Model::Model(const char * archivo, int N, bool tapaSUP, bool tapaINF, bool textura, Material m)
{
	// El método revolucion() nos crea el modelo en revolución
	revolucion(archivo, N, tapaSUP, tapaINF, textura);

	material = m;
	materialSeleccionado = m*S;

	seleccionado = false;

	// Calculamos las normales y los baricentros para pintar las normales
	calcularNormalesCaras();
	calcularNormalesVertices();
	calcularBaricentros();
	calcularPuntoMedio();
}


/*********************************************************************/

// Constructor para crear un modelo en barrido dado un perfil
Model::Model(const char * archivo, bool cerrado, double N)
{
	// El método barrido() nos crea el modelo en barrido
	barrido(archivo, cerrado, N);

	// Calculamos las normales y los baricentros para pintar las normales
	calcularNormalesCaras();
	calcularNormalesVertices();
	calcularBaricentros();
	calcularPuntoMedio();
}


/*********************************************************************/

void Model::leePLY(const char * archivo, Material m)
{
	// Limpiamos el modelo anterior
	vertices.clear();
	caras.clear();
	normalesVertices.clear();
	normalesCaras.clear();
	colores.clear();
	baricentros.clear();
	//------------------------------//

	material = m;
	materialSeleccionado = m*S;

	seleccionado = false;

	// El método creaPLY() nos abre el archivo PLY y lee el modelo
	creaPLY(archivo);


	calcularNormalesCaras();
	calcularNormalesVertices();
	calcularBaricentros();
	calcularPuntoMedio();
}


/*********************************************************************/

void Model::crearRevolucion(const char * archivo, int N, bool tapaSUP, bool tapaINF, bool textura, Material m)
{
	// Limpiamos el modelo anterior
	vertices.clear();
	caras.clear();
	normalesVertices.clear();
	normalesCaras.clear();
	colores.clear();
	baricentros.clear();
	//------------------------------//

	material = m;
	materialSeleccionado = m*S;

	seleccionado = false;

	// El método revolucion() nos crea el modelo en revolución
	revolucion(archivo, N, tapaSUP, tapaINF, textura);

	// Calculamos las normales y los baricentros para pintar las normales
	calcularNormalesCaras();
	calcularNormalesVertices();
	calcularBaricentros();
	calcularPuntoMedio();
}


/*********************************************************************/

void Model::crearBarrido(const char * archivo, bool cerrado, double N)
{
	// Limpiamos el modelo anterior
	vertices.clear();
	caras.clear();
	normalesVertices.clear();
	normalesCaras.clear();
	colores.clear();
	baricentros.clear();
	//------------------------------//

	// El método barrido() nos crea el modelo en barrido
	barrido(archivo, cerrado, N);

	// Calculamos las normales y los baricentros para pintar las normales
	calcularNormalesCaras();
	calcularNormalesVertices();
	calcularBaricentros();
	calcularPuntoMedio();
}


/*********************************************************************/

// Método para calcular las normales de las caras
void Model::calcularNormalesCaras()
{
	for(int i=0; i<caras.size(); i++)
	{
		Tupla3f A = vertices[caras[i].idx[0]];
		Tupla3f B = vertices[caras[i].idx[1]];
		Tupla3f C = vertices[caras[i].idx[2]];
		Tupla3f BA, AC;
		BA.coo[0] = B.coo[0] - A.coo[0];
		BA.coo[1] = B.coo[1] - A.coo[1];
		BA.coo[2] = B.coo[2] - A.coo[2];
		AC.coo[0] = A.coo[0] - C.coo[0];
		AC.coo[1] = A.coo[1] - C.coo[1];
		AC.coo[2] = A.coo[2] - C.coo[2];

		Tupla3f normal;
		normal.coo[0] = ((BA.coo[2]*AC.coo[1]) - (BA.coo[1]*AC.coo[2]));
		normal.coo[1] = ((BA.coo[0]*AC.coo[2]) - (BA.coo[2]*AC.coo[0]));
		normal.coo[2] = ((BA.coo[1]*AC.coo[0]) - (BA.coo[0]*AC.coo[1]));

		float mod = sqrt((normal.coo[0]*normal.coo[0])+(normal.coo[1]*normal.coo[1])+(normal.coo[2]*normal.coo[2]));

		// normalizamos, dividimos por el módulo
		mod = 1.0/mod;

		normal.coo[0] *= mod;
		normal.coo[1] *= mod;
		normal.coo[2] *= mod;
		//------------------------------//

		normalesCaras.push_back(normal);
	}
}


/*********************************************************************/

// Método para calcular las normales a los vértices
void Model::calcularNormalesVertices()
{
	for(int i=0; i<vertices.size(); i++)
	{
		Tupla3f aux(0,0,0);
		// recorremos todas las caras para ver qué caras contienen el punto
		for(int j=0; j<caras.size(); j++)
		{
			// Si la cara contiene ese punto sumamos a aux su normal
			if(caras[j].idx[0]==i ||caras[j].idx[1]==i || caras[j].idx[2]==i)
			{
				aux.coo[0] += normalesCaras[j].coo[0];
				aux.coo[1] += normalesCaras[j].coo[1];
				aux.coo[2] += normalesCaras[j].coo[2];
			}
		}

		double modulo = sqrt((aux.coo[0]*aux.coo[0])+(aux.coo[1]*aux.coo[1])+(aux.coo[2]*aux.coo[2]));

		normalesVertices.push_back(Tupla3f(aux.coo[0]/modulo,aux.coo[1]/modulo,aux.coo[2]/modulo));
	}
}


/*********************************************************************/

// Calculamos los centros de las caras, baricentros
void Model::calcularBaricentros()
{
	for(int i=0; i<caras.size(); i++)
	{
		Tupla3f aux;
		aux.coo[0] = (vertices[caras[i].idx[0]].coo[0] + vertices[caras[i].idx[1]].coo[0] + vertices[caras[i].idx[2]].coo[0])/3.0;
		aux.coo[1] = (vertices[caras[i].idx[0]].coo[1] + vertices[caras[i].idx[1]].coo[1] + vertices[caras[i].idx[2]].coo[1])/3.0;
		aux.coo[2] = (vertices[caras[i].idx[0]].coo[2] + vertices[caras[i].idx[1]].coo[2] + vertices[caras[i].idx[2]].coo[2])/3.0;

		baricentros.push_back(aux);
	}
}


/*********************************************************************/

vector<Tupla3f> Model::getVertices()
{
	return vertices;
}


/*********************************************************************/

vector<Tupla3i> Model::getCaras()
{
	return caras;
}


/*********************************************************************/

Material Model::getMaterial()
{
	return material;
}


/*********************************************************************/

void Model::setMaterial(Material m)
{
	material = m;
}


/*********************************************************************/

void Model::draw(int modo, bool normCaras, bool normVertices, bool seleccion)
{
	if(normCaras)
		drawNormalesCaras();
	if(normVertices)
		drawNormalesVertices();

		
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	glNormalPointer(GL_FLOAT, 0, normalesVertices.data());

	if(!seleccion)
	{
		if(seleccionado)
			materialSeleccionado.activar();
		else
			material.activar();
		if(material.getModo() == 0)
		{
			glTexCoordPointer(2, GL_FLOAT, 0, coordTex.data());
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}

	switch(modo)
	{
		case 0: // Modo puntos
				glDrawArrays(GL_POINTS, 0, vertices.size());
			break;
		case 1: // Modo alambre
				glPolygonMode(GL_FRONT, GL_LINE);
				glDrawElements(GL_TRIANGLES, caras.size()*3, GL_UNSIGNED_INT, caras.data());
			break;
		case 2: // Modo sólido
				glPolygonMode(GL_FRONT, GL_FILL);
				glDrawElements(GL_TRIANGLES, caras.size()*3, GL_UNSIGNED_INT, caras.data());
			break;
		case 3: // Modo ajedrez
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			GLfloat color[4] = { 1, 0, 0, 1.0 } ;
			glMaterialfv( GL_FRONT, GL_EMISSION, color ) ;

			// Las caras pares están en la primera mitad del array de caras
			glDrawElements(GL_TRIANGLES, (caras.size()*3)/2, GL_UNSIGNED_INT, &(caras[0].idx[0]));

			GLfloat color1[4] = { 0, 0, 1, 1.0 } ;
			glMaterialfv( GL_FRONT, GL_EMISSION, color1 ) ;
			glColor3f(0, 0, 0);
			// Las caras impares están en la segunda mitad
			glDrawElements(GL_TRIANGLES, (caras.size()*3)/2, GL_UNSIGNED_INT, &(caras[caras.size()/2].idx[0]));
			break;
		}
		case 4:
				glBegin(GL_TRIANGLES);
					for(int i=0; i<caras.size(); i++)
					{
						GLfloat color[4] = { abs(normalesCaras[i].coo[0]),abs(normalesCaras[i].coo[1]),abs(normalesCaras[i].coo[2]) } ;
						glMaterialfv( GL_FRONT, GL_EMISSION, color ) ;
						glColor3f(abs(normalesCaras[i].coo[0]),abs(normalesCaras[i].coo[1]),abs(normalesCaras[i].coo[2]));
						glVertex3f(vertices[caras[i].idx[0]].coo[0],vertices[caras[i].idx[0]].coo[1],vertices[caras[i].idx[0]].coo[2]);
						glVertex3f(vertices[caras[i].idx[1]].coo[0],vertices[caras[i].idx[1]].coo[1],vertices[caras[i].idx[1]].coo[2]);
						glVertex3f(vertices[caras[i].idx[2]].coo[0],vertices[caras[i].idx[2]].coo[1],vertices[caras[i].idx[2]].coo[2]);
					}
				glEnd();
			break;
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable( GL_TEXTURE_GEN_S );
	glDisable( GL_TEXTURE_GEN_T );
}


/*********************************************************************/

void Model::drawNormalesCaras()
{
	glColor3f(1, 0, 1);

	for(int i=0; i<normalesCaras.size(); i++)
	{
		// Pintamos la normal desde el baricentro de la cara a la normal+baricentro
		glBegin(GL_LINES);
  			glVertex3fv(&baricentros[i].coo[0]);
			glVertex3f((normalesCaras[i].coo[0]*0.4+baricentros[i].coo[0]),
					   (normalesCaras[i].coo[1]*0.4+baricentros[i].coo[1]),
					   (normalesCaras[i].coo[2]*0.4+baricentros[i].coo[2]));
		glEnd();
	}
}


/*********************************************************************/

void Model::drawNormalesVertices()
{
	glColor3f(0.5, 1, 0);

	for(int i=0; i<normalesVertices.size(); i++)
	{
		// Pintamos la normal desde el vértice a la normal+vértice
		glBegin(GL_LINES);
		glVertex3fv(&vertices[i].coo[0]);
		glVertex3f((normalesVertices[i].coo[0]*0.4+vertices[i].coo[0]),
				   (normalesVertices[i].coo[1]*0.4+vertices[i].coo[1]),
				   (normalesVertices[i].coo[2]*0.4+vertices[i].coo[2]));
		glEnd();
	}
}



void Model::seleccionar()
{
	seleccionado = true;
}


void Model::deseleccionar()
{
	seleccionado = false;
}


Tupla3f Model::getPuntoMedio()
{
	return puntoCentro;
}


/*********************************************************************/
// Métodos privados
/*********************************************************************/


void Model::creaPLY(const char * archivo)
{
	// Abrimos el archivo y leemos los puntos y las caras
	vector<float> v;
	vector<int> c;

	ply::read(archivo, v, c);

	// Guardamos los vértices conforme nuestra estructura de datos Tupla3f
	for(int i=0; i<v.size(); i+=3)
	{
		Tupla3f aux(v[i], v[i+1], v[i+2]);

		vertices.push_back(aux);
	}

	// Guardamos las caras conforme nuestra estructura de datos Tupla3i
	for(int i=0; i<c.size(); i+=3)
	{
		Tupla3i aux;
		aux.idx[0] = c[i];
		aux.idx[1] = c[i+1];
		aux.idx[2] = c[i+2];

		caras.push_back(aux);
	}

	// Guardamos las caras pares en la primera mitad y las impares en la segunda
	vector<Tupla3i> par, impar;
	for(int i=0; i<caras.size(); i++)
	{
		if(i%2) par.push_back(caras[i]);
		if(!(i%2)) impar.push_back(caras[i]);
	}
	caras.clear();
	caras = par;

	for(int i=0; i<impar.size(); i++)
	{
		caras.push_back(impar[i]);
	}
	//------------------------------//
}


/*********************************************************************/

void Model::revolucion(const char * archivo, int N, bool tapaSUP, bool tapaINF, bool textura)
{
	vector<float> v;

	ply::read_vertices(archivo, v);


	for(int i=0; i<v.size(); i+=3)
	{
		Tupla3f aux1(v[i], v[i+1], v[i+2]);

		vertices.push_back(aux1);
	}


	// si el perfil está definido de abajo a arriba invertimos el
	// orden de los vértices, para calcular bien las normales siempre
	if(vertices[0].coo[1] < vertices[vertices.size()-1].coo[1])
	{
		for(int i=0, j=vertices.size()-1; i<j; i++, j--)
		{
			Tupla3f aux = vertices[i];
			vertices[i] = vertices[j];
			vertices[j] = aux;
		}
	}
	
	vector<GLfloat> coordenadasT;
	for(int i=0; i<vertices.size()-1; i++)
	{
		coordenadasT.push_back(1-vertices[i].coo[1]/vertices[0].coo[1]);
	}
	coordenadasT.push_back(1);
	for(int i=0; i<vertices.size(); i++)
	{
		coordTex.push_back(tex(0,coordenadasT[i]));
	}


	// Comprobamos que no están definidos los puntos para crear las tapas en el perfil
	bool definidaTapaArriba = false;
	bool definidaTapaAbajo = false;
	Tupla3f arriba, abajo;

	if(vertices[0].coo[0] == 0 && vertices[0].coo[2] == 0)
	{
		definidaTapaArriba = true;
		abajo.coo[0] = vertices[0].coo[0];
		abajo.coo[1] = vertices[0].coo[1];
		abajo.coo[2] = vertices[0].coo[2];
		vertices.erase(vertices.begin());
	}
	if(vertices[vertices.size()-1].coo[0] == 0 && vertices[vertices.size()-1].coo[2] == 0)
	{
		definidaTapaAbajo = true;
		arriba.coo[0] = vertices[vertices.size()-1].coo[0];
		arriba.coo[1] = vertices[vertices.size()-1].coo[1];
		arriba.coo[2] = vertices[vertices.size()-1].coo[2];
		vertices.pop_back();
	}
	//------------------------------//

	unsigned nvertices = vertices.size(); // número de vértices del perfil

	int tam = N;
	if(textura)
		tam++;	

	// creamos la matriz de revolución en función de N
	for(int k=1; k<tam; k++)
	{
		double alpha = (((360*1.0)/N) * M_PI) / 180.0;
		alpha = alpha*k;

		// Matriz de revolución para el eje y con un ángulo alpha
		MatrizR3D matriz('y', alpha);

		tex t;
		t.first = (k*1.0)/(N);

		for(int i=0; i<nvertices; i++)
		{
			// multiplicamos cada vértice por la matriz
			vertices.push_back(matriz*vertices[i]);

			t.second = coordenadasT[i];
			coordTex.push_back(t);
		}		
	}

	// creamos las caras
	int a = 1;

	tam = vertices.size();
	if(textura)
		tam -= nvertices;

	for(unsigned i=0; i<tam; i++)
	{
		// calculamos las caras para todos los vértices excepto los superiores
		if(i!=(a*nvertices-1)) // si no es vértice superior
		{
			// un vértice va con el contiguo a su derecha + el inmediato arriba
			// para formar una cara
			Tupla3i aux;
			aux.idx[0] = (i)%vertices.size();
			aux.idx[1] = (nvertices+i)%vertices.size();
			aux.idx[2] = (i+1)%vertices.size();
			caras.push_back(aux);

			// el vértice superior va con el contiguo a su derecha + el contiguo
			// a la derecha y arriba para formar otra cara
			aux.idx[0] = (i+1)%vertices.size();
			aux.idx[1] = (nvertices+i)%vertices.size();
			aux.idx[2] = (nvertices+i+1)%vertices.size();
			caras.push_back(aux);

			// Así formamos un polígono plano de 4 vértices dividido en 2 triángulos
		}
		else a++;
	}

	if(tapaSUP)
	{
		if(!definidaTapaArriba){
			Tupla3f aux(0, vertices[0].coo[1], 0); // vértice de la tapa
			vertices.push_back(aux);
		} else
			vertices.push_back(arriba);

		// creamos la tapaSUP
		for(int i=0; i<N; i++)
		{
			Tupla3i aux;
			aux.idx[0] = vertices.size()-1; 
			aux.idx[2] = (nvertices*i)%(vertices.size()-1);
			aux.idx[1] = (nvertices*(i+1))%(vertices.size()-1);
			caras.push_back(aux);
		}
	}

	if(tapaINF)
	{
		if(!definidaTapaAbajo){
			Tupla3f aux(0,vertices[nvertices-1].coo[1],0); // vértice de la base
			vertices.push_back(aux);
		} else
			vertices.push_back(arriba);

		int n = vertices.size()-1;
		if(tapaSUP)
			n--;

		// creamos la tapaINF
		for(int i=0; i<N; i++)
		{
			Tupla3i aux;
			aux.idx[0] = vertices.size()-1;
			aux.idx[1] = ((nvertices-1)+(i*nvertices))%(n);
			aux.idx[2] = ((nvertices-1)+((i+1)*nvertices))%(n);
			caras.push_back(aux);
		}
	}

	// Guardamos las caras pares en la primera mitad y las impares en la segunda
	vector<Tupla3i> par, impar;
	for(int i=0; i<caras.size(); i++)
	{
		if(i%2) par.push_back(caras[i]);
		if(!(i%2)) impar.push_back(caras[i]);
	}

	caras.clear();
	caras = par;

	for(int i=0; i<impar.size(); i++)
	{
		caras.push_back(impar[i]);
	}
	//------------------------------//
}


/*********************************************************************/

void Model::barrido(const char * archivo, bool cerrado, double N)
{
	vector<float> v;

	ply::read_vertices(archivo, v);

	for(int i=0; i<v.size(); i+=3)
	{
		Tupla3f aux(v[i], v[i+1], v[i+2]);

		vertices.push_back(aux);
	}

	// Calculamos la normal a los vértices para poder trasladar los puntos
	Tupla3f A = vertices[0];
	Tupla3f B = vertices[1];
	Tupla3f C = vertices[2];
	Tupla3f BA, AC;
	BA.coo[0] = B.coo[0] - A.coo[0];
	BA.coo[1] = B.coo[1] - A.coo[1];
	BA.coo[2] = B.coo[2] - A.coo[2];
	AC.coo[0] = A.coo[0] - C.coo[0];
	AC.coo[1] = A.coo[1] - C.coo[1];
	AC.coo[2] = A.coo[2] - C.coo[2];

	Tupla3f normal;
	normal.coo[0] = ((BA.coo[1]*AC.coo[2]) - (BA.coo[2]*AC.coo[1]));
	normal.coo[1] = ((BA.coo[2]*AC.coo[0]) - (BA.coo[0]*AC.coo[2]));
	normal.coo[2] = ((BA.coo[0]*AC.coo[1]) - (BA.coo[1]*AC.coo[0]));

	float len = sqrt((normal.coo[0]*normal.coo[0])+(normal.coo[1]*normal.coo[1])+(normal.coo[2]*normal.coo[2]));

	len = 1.0/len;

	normal.coo[0] *= len;
	normal.coo[1] *= len;
	normal.coo[2] *= len;
	//----------------------------------//


	// movemos los vértices en el sentido de la normal
	int nvertices = vertices.size();

	for(int i=0; i<nvertices; i++)
	{
		Tupla3f aux(vertices[i].coo[0]+normal.coo[0]*N, vertices[i].coo[1]+normal.coo[1]*N, vertices[i].coo[2]+normal.coo[2]*N);

		vertices.push_back(aux);
	}
	//------------------------------//

	// Calculamos el barrido según sea abierto o cerrado
	if(cerrado)
	{
		// creamos las caras del contorno cerrado
		for(unsigned i=0; i<vertices.size(); i++)
		{
			Tupla3i aux;
			if(i<nvertices)
			{
				aux.idx[0] = (i);
				aux.idx[2] = (i+1)%nvertices;
				aux.idx[1] = (nvertices+i);
				caras.push_back(aux);
			}
			else
			{
				aux.idx[0] = (i);
				aux.idx[2] = (i+1)%vertices.size();
				aux.idx[1] = (i+1-nvertices);
				caras.push_back(aux);
			}
		}
	}
	else
	{
		// creamos las caras del contorno abierto
		for(unsigned i=0; i<vertices.size(); i++)
		{
			Tupla3i aux;
			if(i<nvertices-1)
			{
				aux.idx[0] = (i);
				aux.idx[1] = (i+1)%nvertices;
				aux.idx[2] = (nvertices+i);
				caras.push_back(aux);
			}
			else if(i>nvertices-1 && i<vertices.size()-1)
			{
				aux.idx[0] = (i);
				aux.idx[2] = (i+1)%vertices.size();
				aux.idx[1] = (i+1-nvertices);
				caras.push_back(aux);
			}
		}
	}
}


void Model::calcularPuntoMedio()
{
	double mayorX, menorX, mayorY, menorY, mayorZ, menorZ;

	mayorX = -10000000; menorX = 10000000; mayorY = -10000000;
	menorY = 10000000; mayorZ = -10000000; menorZ = 10000000;

	for(int i=0; i<vertices.size(); i++)
	{
		if(vertices[i].coo[0] > mayorX)
			mayorX = vertices[i].coo[0];
		if(vertices[i].coo[0] < menorX)
			menorX = vertices[i].coo[0];
		if(vertices[i].coo[1] > mayorY)
			mayorY = vertices[i].coo[1];
		if(vertices[i].coo[1] < menorY)
			menorY = vertices[i].coo[1];
		if(vertices[i].coo[2] > mayorZ)
			mayorZ = vertices[i].coo[2];
		if(vertices[i].coo[2] < menorZ)
			menorZ = vertices[i].coo[2];
	}

	puntoCentro = Tupla3f((mayorX+menorX)/2,(mayorY+menorY)/2,(mayorZ+menorZ)/2);
}