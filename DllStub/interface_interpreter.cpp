#include "DllStub.h"

enum NodeType
{
	NodeTypeWindow = 0,
	NodeTypeTabBar,
	NodeTypeTabItem,
	//NodeTypeVariableEquals,
	//NodeTypeVariableSetValue,
	NodeTypeGroup,
	NodeTypeText,
	//NodeTypeButton,
	NodeTypeComboBox,
	NodeTypeCheckBox,
	NodeTypeToolTip,
	NodeTypeNewLine,
	NodeTypeSlider
};

struct Connections
{
	unsigned int uiTargetNode;
	unsigned int uiTargetSlot;
	unsigned int uiSourceSlot;
	float fTargetPositionY;

	bool operator < (const Connections& other) const
	{
		return fTargetPositionY < other.fTargetPositionY;
	}
};

struct Node
{
	std::vector<Connections> connections;
	int iNodeType;
	float fYPosition;

	//Variables
	char szText[MAX_PATH];
	ImVec2 vSize;
	double dbVariable;
	int iVariable_1;

	explicit Node()
	{
		connections.clear();
		fYPosition = 0;
		iNodeType = -1;
		vSize = ImVec2(0.0f, 0.0f);
	}
};

std::vector<Node*> Nodes;

void DllStub::Interface::LoadInterpreter(Json::Value&settings)
{
	unsigned int uiNodesCount = -1;

	GetVal(settings["Nodes"]["Count"], &uiNodesCount);
	if (uiNodesCount > 0)
	{
		for (unsigned int i = 0; i < uiNodesCount; i++)
		{
			Node* tmpNode = new Node();
			int iNodeType = -1;

			// Buscar el nodo por titulo y asignar constructor mapeado...
			GetVal(settings["Nodes"][std::to_string(i).c_str()]["Type"], &iNodeType);
			if (iNodeType == -1)
			{
				delete tmpNode;
				continue;
			}

			tmpNode->iNodeType = iNodeType;
			tmpNode->szText[0] = 0;
			GetVal(settings["Nodes"][std::to_string(i).c_str()]["Position"]["Y"], &tmpNode->fYPosition);
			//
			GetVal(settings["Nodes"][std::to_string(i).c_str()]["Text"], tmpNode->szText, MAX_PATH - 1);
			GetVal(settings["Nodes"][std::to_string(i).c_str()]["dbVariable"], &tmpNode->dbVariable);
			GetVal(settings["Nodes"][std::to_string(i).c_str()]["iVariable_1"], &tmpNode->iVariable_1);
			GetVal(settings["Nodes"][std::to_string(i).c_str()]["Size"]["X"], &tmpNode->vSize.x);
			GetVal(settings["Nodes"][std::to_string(i).c_str()]["Size"]["Y"], &tmpNode->vSize.y);

			Nodes.push_back(tmpNode);
		}

		// Cargar conexiones
		for (unsigned int i = 0; i < Nodes.size(); i++)
		{
			int iConnectionsOutputs = -1;
			GetVal(settings["Nodes"][std::to_string(i).c_str()]["Connections"]["OutputsCount"], &iConnectionsOutputs);
			if (iConnectionsOutputs == -1)
				continue;

			// Procesamos conexiones del output del vector
			for (int j = 0; j < iConnectionsOutputs; j++)
			{
				int iTargetNode = -1;
				int iTargetSlot = -1;
				int iSourceSlot = -1;

				Connections tmpConnection;

				GetVal(settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Outputs"][std::to_string(j)]["SourceSlot"], &iSourceSlot);
				GetVal(settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Outputs"][std::to_string(j)]["TargetNode"], &iTargetNode);
				GetVal(settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Outputs"][std::to_string(j)]["TargetSlot"], &iTargetSlot);

				if (iSourceSlot == -1)
				{
					#ifdef _DEBUG
						std::ostringstream strCorrupted;
						strCorrupted << "Corrupted file (Corrupted node " << i << " - " << j << " - No SourceSlot)";
						MessageBox(GetActiveWindow(), strCorrupted.str().c_str(), "CSHackCreator v2 - BloodSharp", MB_ICONERROR);
					#endif // _DEBUG
					continue;
				}
				if (iTargetNode == -1)
				{
					#ifdef _DEBUG
						std::ostringstream strCorrupted;
						strCorrupted << "Corrupted file (Corrupted node " << i << " - " << j << " - No TargetNode)";
						MessageBox(GetActiveWindow(), strCorrupted.str().c_str(), "CSHackCreator v2 - BloodSharp", MB_ICONERROR);
					#endif // _DEBUG
					continue;
				}
				if (iTargetSlot == -1)
				{
					#ifdef _DEBUG
						std::ostringstream strCorrupted;
						strCorrupted << "Corrupted file (Corrupted node " << i << " - " << j << " - No TargetSlot)";
						MessageBox(GetActiveWindow(), strCorrupted.str().c_str(), "CSHackCreator v2 - BloodSharp", MB_ICONERROR);
					#endif // _DEBUG
					continue;
				}

				tmpConnection.uiSourceSlot = (unsigned int)iSourceSlot;
				tmpConnection.uiTargetNode = (unsigned int)iTargetNode;
				tmpConnection.uiTargetSlot = (unsigned int)iTargetSlot;
				tmpConnection.fTargetPositionY = Nodes[iTargetNode]->fYPosition;

				Nodes[i]->connections.push_back(tmpConnection);
			}
		}
	}
}

void DllStub::Interface::Initialize()
{
	ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);
}
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
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.0f);
	if (ImGui::Begin(/*Background Window*/XorStr<0x4A, 18, 0xBB0CB6D2>("\x08\x2A\x2F\x26\x29\x3D\x3F\x24\x3C\x37\x74\x02\x3F\x39\x3C\x36\x2D" + 0xBB0CB6D2).s, nullptr,
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
	))
	{
		ImGui::Text(/*Hack built using CSHackCreator v2 by BloodSharp*/XorStr<0x79, 48, 0x4AD837BA>("\x31\x1B\x18\x17\x5D\x1C\x0A\xE9\xED\xF6\xA3\xF1\xF6\xEF\xE9\xEF\xA9\xC9\xD8\xC4\xEC\xED\xE4\xD3\xE3\xF7\xF2\xE0\xFA\xE4\xB7\xEE\xAB\xBA\xF9\xE5\xBD\xDC\xF3\xCF\xCE\xC6\xF0\xCC\xC4\xD4\xD7" + 0x4AD837BA).s);
		ImGui::End();
	}
	ImGui::PopStyleVar();
}
