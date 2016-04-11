#ifndef IG_NODOGE
#define IG_NODOGE

#include <iostream>
#include "tuplas.hpp"
#include <vector>
#include "matriz4f.hpp"
#include <stack>
#include "model.hpp"


using namespace std;


struct ParMatriz4f
{
	unsigned tipo ;   // 0 -> rotacion, 1 -> escalado, 2 -> traslación, 3 -> color
	Tupla3f  tupla ; // eje de rotac., vector de desplz, factores de escala o color
	float    angulo ; // ángulo de rotacion (si tipo == 0)
};


// ---------------------------------------------------------------------
// entrada del nodo del grafo de escena


class EntradaNGE;

class NodoGE
{

public:

	static vector<Model*> objetos;

	NodoGE();
	NodoGE(const char * archivo);

	NodoGE(const std::vector<EntradaNGE> ent);

	void abrir(const char * archivo);

	
	void pintar();
	void pintarSeleccion();

	void incluirEntrada(EntradaNGE e);
	std::vector<EntradaNGE> getEntradas();
	int size();
	void getGrados(vector<ParMatriz4f*> & vector);
	void incluirGrado(ParMatriz4f p);
	int nGrados();

	void Transformacion(ParMatriz4f par);
	void PintarObjetoSimple(int o, bool s = false);
private:
	std::vector<EntradaNGE> entradas;
	std::vector<ParMatriz4f> grados;

	static void inicializaObjetos();
};

class EntradaNGE
{
private:
	int tipo ; // entre 0 y 4, ambos incluidos
	NodoGE 	*   subesc;      // 0 = puntero a un sub-árbol
	Matriz4f    matriz;      // 1 = matriz 4x4 de transformación
	ParMatriz4f pars_matriz; // 2 = parámetros de una matriz
	int    		obj_sim;     // 3 = número de objeto simple
	int 		grado;		 // 4 = número de grado de libertad
	Model * 	modelo;		 // 5 = puntero a un modelo
public:
	EntradaNGE(int t, NodoGE* nGE);
	EntradaNGE(int t, Matriz4f m);
	EntradaNGE(int t, ParMatriz4f p);
	EntradaNGE(int t, int n);
	EntradaNGE(int t, int n, int k);
	EntradaNGE(int t, Model * m);

	int getTipo();
	NodoGE* getNodo();
	Matriz4f getMatriz();
	ParMatriz4f getParMatriz();
	int getObjeto();
	int getGrado();
	Model* getModelo();
};

typedef NodoGE GrafoEscena;


#endif