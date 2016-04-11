#include "coleccionFL.hpp"


void ColeccionFL::insertar(FuenteLuz*  fl)
{
	fuentes.push_back(fl);
}

void ColeccionFL::activar()
{
	for(int i=0; i<fuentes.size(); i++)
	{
		fuentes[i]->activar(i);
	}
}


void ColeccionFL::desactivar()
{
	for(int i=0; i<fuentes.size(); i++)
	{
		fuentes[i]->desactivar(i);
	}
}


FuenteLuz* ColeccionFL::get(int i)
{
	return fuentes[i];
}