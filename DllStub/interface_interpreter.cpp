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
