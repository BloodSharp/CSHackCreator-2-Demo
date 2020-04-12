#include "DllStub.h"

void DllStub::Interface::LoadInterpreter(Json::Value&settings)
{
}

void DllStub::Interface::Interface()
{
	std::vector<unsigned int>vWindows;

	for (unsigned int i = 0; i < Nodes.size(); i++)
	{
		// Ordenamos por posicion cada conexion
		std::sort(Nodes[i]->connections.begin(), Nodes[i]->connections.end());

		// Buscamos todas las ventanas
		if (Nodes[i]->iNodeType == NodeTypeWindow)
			vWindows.push_back(i);
	}

	// Renderizamos todas las ventanas y sus nodos hijos...
	for (unsigned int i = 0; i < vWindows.size(); i++)
		Children(Nodes[vWindows[i]]);

	vWindows.clear();
}

void DllStub::Interface::Background()
{
}
