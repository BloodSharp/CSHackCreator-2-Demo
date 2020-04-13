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
	int iVariable_1;
	int iVariable_2;
	int iVariable_3;

	explicit Node()
	{
		connections.clear();
		fYPosition = 0;
		iNodeType = -1;
		iVariable_1 = -1;
		iVariable_2 = -1;
		iVariable_3 = -3;
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
			//GetVal(settings["Nodes"][std::to_string(i).c_str()]["dbVariable"], &tmpNode->dbVariable);
			GetVal(settings["Nodes"][std::to_string(i).c_str()]["iVariable_1"], &tmpNode->iVariable_1);
			GetVal(settings["Nodes"][std::to_string(i).c_str()]["iVariable_2"], &tmpNode->iVariable_2);
			GetVal(settings["Nodes"][std::to_string(i).c_str()]["iVariable_3"], &tmpNode->iVariable_3);
			//GetVal(settings["Nodes"][std::to_string(i).c_str()]["Size"]["X"], &tmpNode->vSize.x);
			//GetVal(settings["Nodes"][std::to_string(i).c_str()]["Size"]["Y"], &tmpNode->vSize.y);

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

void Children(Node* masterNode);

void Children(Node* masterNode)
{
	switch (masterNode->iNodeType)
	{
		case NodeTypeWindow:
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
			if (ImGui::Begin(masterNode->szText, 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				for (unsigned int i = 0; i < masterNode->connections.size(); i++)
					Children(Nodes[masterNode->connections[i].uiTargetNode]);
				ImGui::End();
			}
			break;
		case NodeTypeTabBar:
			if (ImGui::BeginTabBar(masterNode->szText))
			{
				for (unsigned int i = 0; i < masterNode->connections.size(); i++)
					Children(Nodes[masterNode->connections[i].uiTargetNode]);
				ImGui::EndTabBar();
			}
			break;
		case NodeTypeTabItem:
			if (ImGui::BeginTabItem(masterNode->szText))
			{
				for (unsigned int i = 0; i < masterNode->connections.size(); i++)
					Children(Nodes[masterNode->connections[i].uiTargetNode]);
				ImGui::EndTabItem();
			}
			break;
		//NodeTypeVariableEquals,
		//NodeTypeVariableSetValue,
		case NodeTypeGroup:
			ImGui::BeginGroup();
			for (unsigned int i = 0; i < masterNode->connections.size(); i++)
			{
				if (masterNode->connections[i].uiSourceSlot == 0)
					Children(Nodes[masterNode->connections[i].uiTargetNode]);
			}
			ImGui::EndGroup();
			for (unsigned int i = 0; i < masterNode->connections.size(); i++)
			{
				if (masterNode->connections[i].uiSourceSlot == 2)
				{
					if (ImGui::IsItemHovered())
					{
						Children(Nodes[masterNode->connections[i].uiTargetNode]);
					}
				}
				else if (masterNode->connections[i].uiSourceSlot == 1)
				{
					ImGui::SameLine();
					Children(Nodes[masterNode->connections[i].uiTargetNode]);
				}
			}
			break;
		case NodeTypeText:
			ImGui::Text(masterNode->szText);
			for (unsigned int i = 0; i < masterNode->connections.size(); i++)
			{
				if (masterNode->connections[i].uiSourceSlot == 1)
				{
					if (ImGui::IsItemHovered())
					{
						Children(Nodes[masterNode->connections[i].uiTargetNode]);
					}
				}
				else if (masterNode->connections[i].uiSourceSlot == 0)
				{
					ImGui::SameLine();
					Children(Nodes[masterNode->connections[i].uiTargetNode]);
				}
			}
			break;
		//NodeTypeButton,
		case NodeTypeComboBox:
			ImGui::PushItemWidth(105);
			ImGui::Combo(szComboBoxes[masterNode->iVariable_1], &cfg.ComboBoxes[masterNode->iVariable_1], szComboBoxesOptions[masterNode->iVariable_1]);
			ImGui::PopItemWidth();
			for (unsigned int i = 0; i < masterNode->connections.size(); i++)
			{
				if (masterNode->connections[i].uiSourceSlot == 1)
				{
					if (ImGui::IsItemHovered())
					{
						Children(Nodes[masterNode->connections[i].uiTargetNode]);
					}
				}
				else if (masterNode->connections[i].uiSourceSlot == 0)
				{
					ImGui::SameLine();
					Children(Nodes[masterNode->connections[i].uiTargetNode]);
				}
			}
			break;
		case NodeTypeCheckBox:
			ImGui::Checkbox(szCheckBoxes[masterNode->iVariable_1], &cfg.CheckBoxes[masterNode->iVariable_1]);
			for (unsigned int i = 0; i < masterNode->connections.size(); i++)
			{
				if (masterNode->connections[i].uiSourceSlot == 1)
				{
					if (ImGui::IsItemHovered())
					{
						Children(Nodes[masterNode->connections[i].uiTargetNode]);
					}
				}
				else if (masterNode->connections[i].uiSourceSlot == 0)
				{
					ImGui::SameLine();
					Children(Nodes[masterNode->connections[i].uiTargetNode]);
				}
			}
			break;
		case NodeTypeToolTip:
			ImGui::SetTooltip(masterNode->szText);
			break;
		case NodeTypeNewLine:
			ImGui::NewLine();
			break;
		case NodeTypeSlider:
			ImGui::PushItemWidth(105);
			ImGui::SliderInt(szSliders[masterNode->iVariable_1], &cfg.Sliders[masterNode->iVariable_1], masterNode->iVariable_2, masterNode->iVariable_3);
			ImGui::PopItemWidth();
			for (unsigned int i = 0; i < masterNode->connections.size(); i++)
			{
				if (masterNode->connections[i].uiSourceSlot == 1)
				{
					if (ImGui::IsItemHovered())
					{
						Children(Nodes[masterNode->connections[i].uiTargetNode]);
					}
				}
				else if (masterNode->connections[i].uiSourceSlot == 0)
				{
					ImGui::SameLine();
					Children(Nodes[masterNode->connections[i].uiTargetNode]);
				}
			}
			break;
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
