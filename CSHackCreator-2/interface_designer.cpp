#include "CSHackCreator.h"

std::vector<MyNode*> CSHackCreator::Settings::Nodes;

enum NodeKindTypes
{
    NodeKindItems = 1, // ID can not be 0
    NodeKindSameLine,
    NodeKindHover,
    //NodeKindClick,
    NodeKindTabItem,
};

// Nodes strings
const char* szTabBar = "TabBar";
const char* szTabItem = "TabItem";
const char* szWindow = "Window";
const char* szGroupBox = "Group";
const char* szText = "Text";
//const char* szButton = "Button";
const char* szComboBox = "ComboBox";
const char* szCheckBox = "CheckBox";
const char* szToolTip = "ToolTip";
const char* szNewLine = "New Line";
//const char* szVariableEquals = "Variable Equals";
//const char* szVariableSetValue = "Variable Set Value";

// Nodes inputs/outputs strings
const char* szTabItems = "Tab Item";
const char* szItems = "Items";
const char* szSameLine = "Same Line";
const char* szOnMouseHover = "On Mouse Hover";
const char* szOnMouseClick = "On Mouse Click";

std::map<std::string, MyNode * (*)()> available_nodes
{
    {
        szTabBar, []() -> MyNode*
        {
            return new MyNode(szTabBar,
            {
                {szItems,NodeKindItems}
            },
            {
                {szTabItems,NodeKindTabItem}
            });
        }
    },
    {
        szTabItem, []() -> MyNode*
        {
            return new MyNode(szTabItem,
            {
                {szTabItems,NodeKindTabItem}
            },
            {
                {szItems,NodeKindItems}
            });
        }
    },
    /*
    {
        szVariableEquals, []() -> MyNode*
        {
            return new MyNode(szVariableEquals,
            {
                {szItems,NodeKindItems}
            },
            {
                {szItems,NodeKindItems}
            });
        }
    },
    {
        szVariableSetValue, []() -> MyNode*
        {
            return new MyNode(szVariableSetValue,SlotTypeInput,
            {
                {szOnMouseClick,NodeKindClick}
            });
        }
    },
    */
    {
        szWindow, []() -> MyNode*
        {
            return new MyNode(szWindow,
            {
                {"Variable",NodeKindItems}
            },
            {
                {szItems,NodeKindItems}
            });
        }
    },
    {
        szGroupBox, []() -> MyNode*
        {
            return new MyNode(szGroupBox,
            {
                {szItems,NodeKindItems},
                {szSameLine,NodeKindSameLine}
            },
            {
                {szItems,NodeKindItems},
                {szSameLine,NodeKindSameLine},
                {szOnMouseHover,NodeKindHover},
                //{szOnMouseClick,NodeKindClick}
            });
        }
    },
    {
        szText, []() -> MyNode*
        {
            return new MyNode(szText,
            {
                {szItems,NodeKindItems},
                {szSameLine,NodeKindSameLine}
            },
            {
                {szSameLine,NodeKindSameLine},
                {szOnMouseHover,NodeKindHover}
            });
        }
    },
    /*
    {
        szButton, []() -> MyNode*
        {
            return new MyNode(szButton,
            {
                {szItems,NodeKindItems},
                {szSameLine,NodeKindSameLine}
            },
            {
                {szSameLine,NodeKindSameLine},
                {szOnMouseHover,NodeKindHover},
                {szOnMouseClick,NodeKindClick}
            });
        }
    },
    */
    {
        szComboBox, []() -> MyNode*
        {
            return new MyNode(szComboBox,
            {
                {szItems,NodeKindItems},
                {szSameLine,NodeKindSameLine}
            },
            {
                {szSameLine,NodeKindSameLine},
                {szOnMouseHover,NodeKindHover}
            });
        }
    },
    {
        szCheckBox, []() -> MyNode*
        {
            return new MyNode(szCheckBox,
            {
                {szItems,NodeKindItems},
                {szSameLine,NodeKindSameLine}
            },
            {
                {szSameLine,NodeKindSameLine},
                {szOnMouseHover,NodeKindHover}
            });
        }
    },
    {
        szToolTip, []() -> MyNode*
        {
            return new MyNode(szToolTip,SlotTypeInput,
            {
                {szOnMouseHover,NodeKindHover}
            });
        }
    },
    {
        szNewLine,[]() -> MyNode*
        {
            return new MyNode(szNewLine,SlotTypeInput,
            {
                {szItems,NodeKindItems}
            });
        }
    }
};

void ProcessWindowContent(MyNode* node, float zoom)
{
    if (node->title == szWindow)
    {
        node->uiNodeType = NodeTypeWindow;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::InputText("Title", node->szText, IM_ARRAYSIZE(node->szText));
        ImGui::PopItemWidth();
    }
}

void ProcessTabBarContent(MyNode* node, float zoom)
{
    if (node->title == szTabBar)
    {
        node->uiNodeType = NodeTypeTabBar;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::InputText("ID", node->szText, IM_ARRAYSIZE(node->szText));
        ImGui::PopItemWidth();
    }
}

void ProcessTabBarItemContent(MyNode* node, float zoom)
{
    if (node->title == szTabItem)
    {
        node->uiNodeType = NodeTypeTabItem;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::InputText("Title bar", node->szText, IM_ARRAYSIZE(node->szText));
        ImGui::PopItemWidth();
    }
}

/*
void ProcessVariableEqualsContent(MyNode* node, float zoom)
{
    if(node->title == szVariableEquals)
    {
        node->uiNodeType = NodeTypeVariableEquals;
        ImGui::PushItemWidth(100 * zoom);
        ImGui::InputText("ID", node->szText, IM_ARRAYSIZE(node->szText));
        ImGui::InputDouble("Value", &node->dbVariable, 0.0, 0.0, "%.2f");
        ImGui::PopItemWidth();
    }
}

void ProcessVariableSetValueContent(MyNode* node, float zoom)
{
    if (node->title == szVariableSetValue)
    {
        node->uiNodeType = NodeTypeVariableSetValue;
        ImGui::PushItemWidth(100 * zoom);
        ImGui::InputText("ID", node->szText, IM_ARRAYSIZE(node->szText));
        ImGui::InputDouble("Value", &node->dbVariable, 0.0, 0.0, "%.2f");
        ImGui::PopItemWidth();
    }
}
*/

void ProcessGroupContent(MyNode* node)
{
    if (node->title == szGroupBox)
    {
        node->uiNodeType = NodeTypeGroup;
    }
}

void ProcessTextContent(MyNode* node, float zoom)
{
    if (node->title == szText)
    {
        node->uiNodeType = NodeTypeText;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::InputText("Value", node->szText, IM_ARRAYSIZE(node->szText));
        ImGui::PopItemWidth();
    }
}

/*
void ProcessButtonContent(MyNode* node, float zoom)
{
    if (node->title == szButton)
    {
        node->uiNodeType = NodeTypeButton;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::InputText(szText, node->szText, IM_ARRAYSIZE(node->szText));
        ImGui::InputFloat2("Size", (float*)&node->vSize, 0);
        ImGui::PopItemWidth();
    }
}
*/

void ProcessComboBoxContent(MyNode* node, float zoom)
{
    if (node->title == szComboBox)
    {
        node->uiNodeType = NodeTypeComboBox;
        ImGui::PushItemWidth(200 * zoom);
        //ImGui::Combo("Hack variable", &node->iVariable_1, szCvarsStringsComboBox, IM_ARRAYSIZE(szCvarsStringsComboBox), 10);
        ImGui::PopItemWidth();
    }
}

void ProcessCheckBoxContent(MyNode* node, float zoom)
{
    if (node->title == szCheckBox)
    {
        node->uiNodeType = NodeTypeCheckBox;
        ImGui::PushItemWidth(200 * zoom);
        //ImGui::Combo("Hack variable", &node->iVariable_1, szCvarsStringsCheckBox, IM_ARRAYSIZE(szCvarsStringsCheckBox), 10);
        ImGui::PopItemWidth();
    }
}

void ProcessToolTipContent(MyNode* node, float zoom)
{
    if (node->title == szToolTip)
    {
        node->uiNodeType = NodeTypeToolTip;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::InputText(szText, node->szText, IM_ARRAYSIZE(node->szText));
        ImGui::PopItemWidth();
    }
}

void ProcessNewLineContent(MyNode* node)
{
    if (node->title == szNewLine)
        node->uiNodeType = NodeTypeNewLine;
}

void CSHackCreator::Interface::Designer()
{
    static ImNodes::CanvasState canvas{};

    ImNodes::BeginCanvas(&canvas);
    for (auto it = CSHackCreator::Settings::Nodes.begin(); it != CSHackCreator::Settings::Nodes.end();)
    {
        MyNode* node = *it;

        if (ImNodes::Ez::BeginNode(node, node->title, &node->pos, &node->selected))
        {
            ImNodes::Ez::InputSlots(node->input_slots.data(), node->input_slots.size());

            // Custom node content may go here
            ProcessWindowContent(node, canvas.zoom);
            ProcessTabBarContent(node, canvas.zoom);
            ProcessTabBarItemContent(node, canvas.zoom);
            //ProcessVariableEqualsContent(node, canvas.zoom);
            //ProcessVariableSetValueContent(node, canvas.zoom);
            ProcessGroupContent(node);
            ProcessTextContent(node, canvas.zoom);
            //ProcessButtonContent(node, canvas.zoom);
            ProcessComboBoxContent(node, canvas.zoom);
            ProcessCheckBoxContent(node, canvas.zoom);
            ProcessToolTipContent(node, canvas.zoom);
            ProcessNewLineContent(node);

            ImNodes::Ez::OutputSlots(node->output_slots.data(), node->output_slots.size());

            // Store new connections when they are created
            Connection new_connection;
            if (ImNodes::GetNewConnection(&new_connection.input_node, &new_connection.input_slot,
                &new_connection.output_node, &new_connection.output_slot))
            {
                ((MyNode*)new_connection.input_node)->connections.push_back(new_connection);
                ((MyNode*)new_connection.output_node)->connections.push_back(new_connection);
            }

            // Render output connections of this node
            for (const Connection& connection : node->connections)
            {
                // Node contains all it's connections (both from output and to input slots). This means that multiple
                // nodes will have same connection. We render only output connections and ensure that each connection
                // will be rendered once.
                if (connection.output_node != node)
                    continue;

                if (!ImNodes::Connection(connection.input_node, connection.input_slot, connection.output_node,
                    connection.output_slot))
                {
                    // Remove deleted connections
                    ((MyNode*)connection.input_node)->DeleteConnection(connection);
                    ((MyNode*)connection.output_node)->DeleteConnection(connection);
                }
            }
        }
        ImNodes::Ez::EndNode();

        if (node->selected && ImGui::IsKeyPressedMap(ImGuiKey_Delete))
        {
            // Deletion order is critical: first we delete connections to us
            for (auto& connection : node->connections)
            {
                if (connection.output_node == node)
                {
                    ((MyNode*)connection.input_node)->DeleteConnection(connection);
                }
                else
                {
                    ((MyNode*)connection.output_node)->DeleteConnection(connection);
                }
            }
            // Then we delete our own connections, so we don't corrupt the list
            node->connections.clear();

            delete node;
            it = CSHackCreator::Settings::Nodes.erase(it);
        }
        else
            ++it;
    }

    const ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsMouseReleased(1) && ImGui::IsWindowHovered() && !ImGui::IsMouseDragging(1))
    {
        ImGui::FocusWindow(ImGui::GetCurrentWindow());
        ImGui::OpenPopup("NodesContextMenu");
    }

    if (ImGui::BeginPopup("NodesContextMenu"))
    {
        for (const auto& desc : available_nodes)
        {
            if (ImGui::MenuItem(desc.first.c_str()))
            {
                CSHackCreator::Settings::Nodes.push_back(desc.second());
                ImNodes::AutoPositionNode(CSHackCreator::Settings::Nodes.back());
            }
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Reset Zoom"))
            canvas.zoom = 1;

        if (ImGui::IsAnyMouseDown() && !ImGui::IsWindowHovered())
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    ImNodes::EndCanvas();
}

const char* GetNodeTitle(int uiNodeType)
{
    switch (uiNodeType)
    {
        //case NodeTypeButton:
        //    return szButton;
        case NodeTypeCheckBox:
            return szCheckBox;
        case NodeTypeComboBox:
            return szComboBox;
        case NodeTypeGroup:
            return szGroupBox;
        case NodeTypeNewLine:
            return szNewLine;
        case NodeTypeTabBar:
            return szTabBar;
        case NodeTypeTabItem:
            return szTabItem;
        case NodeTypeText:
            return szText;
        case NodeTypeToolTip:
            return szToolTip;
        /*
        case NodeTypeVariableEquals:
            return szVariableEquals;
        case NodeTypeVariableSetValue:
            return szVariableSetValue;
        */
        case NodeTypeWindow:
            return szWindow;
        default:
            return 0;
    }
}

/*
void ProcessSpecialOpenVariables(Json::Value& settings, MyNode* node)
{
    switch (node->uiNodeType)
    {
        case NodeTypeButton:
            break;
        case NodeTypeCheckBox:
            break;
        case NodeTypeComboBox:
            break;
        case NodeTypeGroup:
            break;
        case NodeTypeNewLine:
            break;
        case NodeTypeTabBar:
            break;
        case NodeTypeTabItem:
            break;
        case NodeTypeText:
            break;
        case NodeTypeToolTip:
            break;
        case NodeTypeVariableEquals:
            break;
        case NodeTypeVariableSetValue:
            break;
        case NodeTypeWindow:
            break;
    }
}
*/


void CSHackCreator::Settings::OpenNodes(Json::Value& settings)
{
    int iNodesCount = 0;
    GetVal(settings["Nodes"]["Count"], &iNodesCount);
    bool bFailedFileProcess = false;

    for (int i = 0; i < iNodesCount; i++)
    {
        MyNode *node = 0;
        int iNodeType = -1;
        const char* szNodeTitle = 0;

        // Buscar el nodo por titulo y asignar constructor mapeado...
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["Type"], &iNodeType);
        if (iNodeType == -1)
        {
            std::ostringstream strCorrupted;
            strCorrupted << "Corrupted file (Corrupted node " << i << ")";
            MessageBox(GetActiveWindow(), strCorrupted.str().c_str(), "CSHackCreator v2 - BloodSharp", MB_ICONERROR);
            bFailedFileProcess = true;
            continue;
        }
        szNodeTitle = GetNodeTitle(iNodeType);
        if (!szNodeTitle)
            continue;
        for (const auto& desc : available_nodes)
            if (!strcmp(szNodeTitle, desc.first.c_str()))
                node = desc.second();
        if (!node)
            continue;

        node->uiNodeType = (unsigned int)iNodeType;
        node->szText[0] = 0;
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["Position"]["X"], &node->pos.x);
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["Position"]["Y"], &node->pos.y);
        //
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["Text"], node->szText, MAX_PATH - 1);
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["dbVariable"], &node->dbVariable);
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["iVariable_1"], &node->iVariable_1);
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["Size"]["X"], &node->vSize.x);
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["Size"]["Y"], &node->vSize.y);
        //

        // Procesar el nodo al vector
        CSHackCreator::Settings::Nodes.push_back(node);
    }

    if (!bFailedFileProcess)
    {
        for (unsigned int i = 0; i < CSHackCreator::Settings::Nodes.size(); i++)
        {
            int iConnectionsOutputs = 0;
            int iConnectionsInputs = 0;

            GetVal(settings["Nodes"][std::to_string(i).c_str()]["Connections"]["OutputsCount"], &iConnectionsOutputs);
            GetVal(settings["Nodes"][std::to_string(i).c_str()]["Connections"]["InputsCount"], &iConnectionsInputs);

            // Procesamos conexiones del output del vector
            for (int j = 0; j < iConnectionsOutputs; j++)
            {
                int iTargetNode = -1;
                int iTargetSlot = -1;
                int iSourceSlot = -1;
                Connection connection;
                connection.output_node = CSHackCreator::Settings::Nodes[i];
                GetVal(settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Outputs"][std::to_string(j)]["SourceSlot"], &iSourceSlot);
                if (iSourceSlot == -1)
                {
                    #ifdef _DEBUG
                        std::ostringstream strCorrupted;
                        strCorrupted << "Corrupted file (Corrupted node " << i << " - " << j << " - No SourceSlot)";
                        MessageBox(GetActiveWindow(), strCorrupted.str().c_str(), "CSHackCreator v2 - BloodSharp", MB_ICONERROR);
                    #endif // _DEBUG
                    continue;
                }
                connection.output_slot = CSHackCreator::Settings::Nodes[i]->output_slots[iSourceSlot].title;
                GetVal(settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Outputs"][std::to_string(j)]["TargetNode"], &iTargetNode);
                GetVal(settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Outputs"][std::to_string(j)]["TargetSlot"], &iTargetSlot);
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
                connection.input_node = CSHackCreator::Settings::Nodes[iTargetNode];
                connection.input_slot = CSHackCreator::Settings::Nodes[iTargetNode]->input_slots[iTargetSlot].title;

                //CSHackCreator::Settings::Nodes[i]->connections.push_back(connection);
                ((MyNode*)connection.input_node)->connections.push_back(connection);
                ((MyNode*)connection.output_node)->connections.push_back(connection);
            }

            /*
            // Procesamos conexiones del input del vector
            for (int j = 0; j < iConnectionsInputs; j++)
            {
                int iTargetNode = -1;
                int iTargetSlot = -1;
                int iSourceSlot = -1;
                Connection connection;
                connection.output_node = CSHackCreator::Settings::Nodes[i];
                GetVal(settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Inputs"][std::to_string(j)]["SourceSlot"], &iSourceSlot);
                if (iSourceSlot == -1)
                    return;
                connection.output_slot = CSHackCreator::Settings::Nodes[i]->output_slots[iSourceSlot].title;
                GetVal(settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Inputs"][std::to_string(j)]["TargetNode"], &iTargetNode);
                GetVal(settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Inputs"][std::to_string(j)]["TargetSlot"], &iTargetSlot);
                if (iTargetNode == -1 || iTargetSlot == -1)
                    return;
                connection.input_node = CSHackCreator::Settings::Nodes[iTargetNode];
                connection.input_slot = CSHackCreator::Settings::Nodes[iTargetNode]->input_slots[iTargetSlot].title;
                CSHackCreator::Settings::Nodes[i]->connections.push_back(connection);
            }
            */
        }
    }
}

void ProcessSpecialSaveVariables(Json::Value& settings, int offset)
{
    settings["Nodes"][std::to_string(offset).c_str()]["Text"] = CSHackCreator::Settings::Nodes[offset]->szText;
    settings["Nodes"][std::to_string(offset).c_str()]["dbVariable"] = CSHackCreator::Settings::Nodes[offset]->dbVariable;
    settings["Nodes"][std::to_string(offset).c_str()]["iVariable_1"] = CSHackCreator::Settings::Nodes[offset]->iVariable_1;
    settings["Nodes"][std::to_string(offset).c_str()]["Size"]["X"] = CSHackCreator::Settings::Nodes[offset]->vSize.x;
    settings["Nodes"][std::to_string(offset).c_str()]["Size"]["Y"] = CSHackCreator::Settings::Nodes[offset]->vSize.y;
}

void CSHackCreator::Settings::SaveNodes(Json::Value& settings)
{
    settings["Nodes"]["Count"] = CSHackCreator::Settings::Nodes.size();

    for (unsigned int i = 0; i < CSHackCreator::Settings::Nodes.size(); i++)
    {
        settings["Nodes"][std::to_string(i).c_str()]["Type"] = CSHackCreator::Settings::Nodes[i]->uiNodeType;
        settings["Nodes"][std::to_string(i).c_str()]["Position"]["X"] = CSHackCreator::Settings::Nodes[i]->pos.x;
        settings["Nodes"][std::to_string(i).c_str()]["Position"]["Y"] = CSHackCreator::Settings::Nodes[i]->pos.y;
        #ifdef _DEBUG
            settings["Nodes"][std::to_string(i).c_str()]["Title"] = CSHackCreator::Settings::Nodes[i]->title;
        #endif
        ProcessSpecialSaveVariables(settings, i);

        if (CSHackCreator::Settings::Nodes[i]->connections.size())
        {
            unsigned int iInputNode = 0;
            for (unsigned int j = 0; j < CSHackCreator::Settings::Nodes[i]->connections.size(); j++)
            {
                if (CSHackCreator::Settings::Nodes[i]->connections[j].output_node != CSHackCreator::Settings::Nodes[i])
                {
                    // Filtramos solo los outputs
                    iInputNode++;
                    continue;
                }

                #ifdef _DEBUG
                    settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Outputs"][std::to_string(j - iInputNode).c_str()]["SourceSlotName"] = CSHackCreator::Settings::Nodes[i]->connections[j].output_slot;
                    settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Outputs"][std::to_string(j - iInputNode).c_str()]["TargetSlotName"] = CSHackCreator::Settings::Nodes[i]->connections[j].input_slot;
                #endif

                settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Outputs"][std::to_string(j - iInputNode).c_str()]["SourceNode"] = i;                
                for (unsigned int k = 0; k < CSHackCreator::Settings::Nodes.size(); k++)
                {
                    if (CSHackCreator::Settings::Nodes[i]->connections[j].input_node == CSHackCreator::Settings::Nodes[k])
                    {
                        settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Outputs"][std::to_string(j - iInputNode).c_str()]["TargetNode"] = k;
                        for (unsigned int l = 0; l < CSHackCreator::Settings::Nodes[k]->input_slots.size(); l++)
                            if (!strcmp(CSHackCreator::Settings::Nodes[k]->input_slots[l].title, CSHackCreator::Settings::Nodes[i]->connections[j].input_slot))
                                settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Outputs"][std::to_string(j - iInputNode).c_str()]["TargetSlot"] = l;
                    }
                    else if (CSHackCreator::Settings::Nodes[i]->connections[j].output_node == CSHackCreator::Settings::Nodes[k])
                    {
                        for (unsigned int l = 0; l < CSHackCreator::Settings::Nodes[k]->output_slots.size(); l++)
                            if (!strcmp(CSHackCreator::Settings::Nodes[k]->output_slots[l].title, CSHackCreator::Settings::Nodes[i]->connections[j].output_slot))
                                settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Outputs"][std::to_string(j - iInputNode).c_str()]["SourceSlot"] = l;
                    }
                }
            }
            settings["Nodes"][std::to_string(i).c_str()]["Connections"]["OutputsCount"] = CSHackCreator::Settings::Nodes[i]->connections.size() - iInputNode;

            /*
            unsigned int iOutputNode = 0;
            for (unsigned int j = 0; j < CSHackCreator::Settings::Nodes[i]->connections.size(); j++)
            {
                if (CSHackCreator::Settings::Nodes[i]->connections[j].output_node == CSHackCreator::Settings::Nodes[i])
                {
                    // Filtramos solo los inputs
                    iOutputNode++;
                    continue;
                }

                #ifdef _DEBUG
                    settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Inputs"][std::to_string(j).c_str()]["SourceSlotName"] = CSHackCreator::Settings::Nodes[i]->connections[j].input_slot;
                    settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Inputs"][std::to_string(j).c_str()]["TargetSlotName"] = CSHackCreator::Settings::Nodes[i]->connections[j].output_slot;
                #endif

                settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Inputs"][std::to_string(j).c_str()]["SourceNode"] = i;
                for (unsigned int k = 0; k < CSHackCreator::Settings::Nodes.size(); k++)
                {
                    if (CSHackCreator::Settings::Nodes[i]->connections[j].output_node == CSHackCreator::Settings::Nodes[k])
                    {
                        settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Inputs"][std::to_string(j).c_str()]["TargetNode"] = k;
                        for (unsigned int l = 0; l < CSHackCreator::Settings::Nodes[k]->output_slots.size(); l++)
                            if (!strcmp(CSHackCreator::Settings::Nodes[k]->output_slots[l].title, CSHackCreator::Settings::Nodes[i]->connections[j].output_slot))
                                settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Inputs"][std::to_string(j).c_str()]["TargetSlot"] = l;
                    }
                    else if (CSHackCreator::Settings::Nodes[i]->connections[j].input_node == CSHackCreator::Settings::Nodes[k])
                    {
                        for (unsigned int l = 0; l < CSHackCreator::Settings::Nodes[k]->input_slots.size(); l++)
                            if (!strcmp(CSHackCreator::Settings::Nodes[k]->input_slots[l].title, CSHackCreator::Settings::Nodes[i]->connections[j].input_slot))
                                settings["Nodes"][std::to_string(i).c_str()]["Connections"]["Inputs"][std::to_string(j).c_str()]["SourceSlot"] = l;
                    }
                }
            }

            settings["Nodes"][std::to_string(i).c_str()]["Connections"]["InputsCount"] = CSHackCreator::Settings::Nodes[i]->connections.size() - iOutputNode;
            */
        }
    }
}