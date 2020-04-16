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
const char* szSlider = "Slider";

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
            return new MyNode(szWindow,SlotTypeOutput,
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
    },
    {
        szSlider, []() -> MyNode*
        {
            return new MyNode(szSlider,
            {
                {szItems,NodeKindItems},
                {szSameLine,NodeKindSameLine}
            },
            {
                {szSameLine,NodeKindSameLine},
                {szOnMouseHover,NodeKindHover}
            });
        }
    }
};

void ProcessWindowContent(MyNode* node, float zoom)
{
    if (node->title == szWindow)
    {
        node->iNodeType = NodeTypeWindow;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::InputTextWithHint(
            /*Title*/XorStr<0x4B, 6, 0x492A3A0B>("\x1F\x25\x39\x22\x2A" + 0x492A3A0B).s,
            /*MyHack - BloodSharp*/XorStr<0x85, 20, 0xC1CA5C24>("\xC8\xFF\xCF\xE9\xEA\xE1\xAB\xA1\xAD\xCC\xE3\xFF\xFE\xF6\xC0\xFC\xF4\xE4\xE7" + 0xC1CA5C24).s,
            node->szText, IM_ARRAYSIZE(node->szText));
        ImGui::PopItemWidth();
    }
}

void ProcessTabBarContent(MyNode* node, float zoom)
{
    if (node->title == szTabBar)
    {
        node->iNodeType = NodeTypeTabBar;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::InputTextWithHint(
            /*ID*/XorStr<0x8D, 3, 0x21F1E17F>("\xC4\xCA" + 0x21F1E17F).s,
            /*Any identifier you want*/XorStr<0x9B, 24, 0x6DD82A89>("\xDA\xF2\xE4\xBE\xF6\xC4\xC4\xCC\xD7\xCD\xC3\xCF\xC2\xDA\x89\xD3\xC4\xD9\x8D\xD9\xCE\xDE\xC5" + 0x6DD82A89).s,
            node->szText, IM_ARRAYSIZE(node->szText));
        ImGui::PopItemWidth();
    }
}

void ProcessTabBarItemContent(MyNode* node, float zoom)
{
    if (node->title == szTabItem)
    {
        node->iNodeType = NodeTypeTabItem;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::InputText(/*Title bar*/XorStr<0x54, 10, 0x2EAA65C1>("\x00\x3C\x22\x3B\x3D\x79\x38\x3A\x2E" + 0x2EAA65C1).s, node->szText, IM_ARRAYSIZE(node->szText));
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
        node->iNodeType = NodeTypeGroup;
    }
}

void ProcessTextContent(MyNode* node, float zoom)
{
    if (node->title == szText)
    {
        node->iNodeType = NodeTypeText;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::InputText(/*Value*/XorStr<0xCB, 6, 0x96EE403F>("\x9D\xAD\xA1\xBB\xAA" + 0x96EE403F).s, node->szText, IM_ARRAYSIZE(node->szText));
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
        node->iNodeType = NodeTypeComboBox;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::Combo(
            /*Hack variable*/XorStr<0x58, 14, 0xA34A3640>("\x10\x38\x39\x30\x7C\x2B\x3F\x2D\x09\x00\x00\x0F\x01" + 0xA34A3640).s,
            &node->iVariable_1,
            /*Wall Hack\0Extra Walls\0Extra Models\0Aim Team\0\0*/XorStr<0xE0, 46, 0x3F94CFF6>("\xB7\x80\x8E\x8F\xC4\xAD\x87\x84\x83\xE9\xAF\x93\x98\x9F\x8F\xCF\xA7\x90\x9E\x9F\x87\xF5\xB3\x8F\x8C\x8B\x9B\xDB\xB1\x92\x9A\x9A\x6C\x72\x02\x42\x6D\x68\x26\x53\x6D\x68\x67\x0B\x0C" + 0x3F94CFF6).s);
        ImGui::PopItemWidth();
    }
}

void ProcessCheckBoxContent(MyNode* node, float zoom)
{
    if (node->title == szCheckBox)
    {
        node->iNodeType = NodeTypeCheckBox;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::Combo(
            /*Hack variable*/XorStr<0xEF, 14, 0xB9FEF6CA>("\xA7\x91\x92\x99\xD3\x82\x94\x84\x9E\x99\x9B\x96\x9E" + 0xB9FEF6CA).s,
            &node->iVariable_1,
            /*Master Switch\0No Smoke\0No Flash\0No Sky\0ESP Box\0BunnyHop\0Zoom\0\0*/XorStr<0x24, 63, 0x456C56CE>("\x69\x44\x55\x53\x4D\x5B\x0A\x78\x5B\x44\x5A\x4C\x58\x31\x7C\x5C\x14\x66\x5B\x58\x53\x5C\x3A\x75\x53\x1D\x78\x53\x21\x32\x2A\x43\x0A\x2A\x66\x14\x23\x30\x4A\x0E\x1F\x1D\x6E\x0D\x3F\x29\x52\x11\x21\x3B\x38\x2E\x10\x36\x2A\x5B\x06\x32\x31\x32\x60\x61" + 0x456C56CE).s);
        ImGui::PopItemWidth();
    }
}

void ProcessToolTipContent(MyNode* node, float zoom)
{
    if (node->title == szToolTip)
    {
        node->iNodeType = NodeTypeToolTip;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::InputText(szText, node->szText, IM_ARRAYSIZE(node->szText));
        ImGui::PopItemWidth();
    }
}

void ProcessNewLineContent(MyNode* node)
{
    if (node->title == szNewLine)
        node->iNodeType = NodeTypeNewLine;
}

void ProcessSliderContent(MyNode* node, float zoom)
{
    if (node->title == szSlider)
    {
        node->iNodeType = NodeTypeSlider;
        ImGui::PushItemWidth(200 * zoom);
        ImGui::Combo(/*Hack variable*/XorStr<0x43, 14, 0xAF990AE1>("\x0B\x25\x26\x2D\x67\x3E\x28\x38\x22\x2D\x2F\x22\x2A" + 0xAF990AE1).s, &node->iVariable_1,
            /*Wireframe Wall\0Wire Models\0CrossHair\0No Recoil\0SpeedHack\0\0*/XorStr<0xA2, 59, 0x729176C6>("\xF5\xCA\xD6\xC0\xC0\xD5\xC9\xC4\xCF\x8B\xFB\xCC\xC2\xC3\xB0\xE6\xDB\xC1\xD1\x95\xFB\xD8\xDC\xDC\xD6\xC8\xBC\xFE\xCC\xD0\xB3\xB2\x8A\xA2\xAD\xB7\xC6\x89\xA7\xE9\x98\xAE\xAF\xA2\xA7\xA3\xD0\x82\xA2\xB6\xB1\xB1\x9E\xB6\xBB\xB2\xDA\xDB" + 0x729176C6).s);
        ImGui::PopItemWidth();

        //SpeedHack
        if (node->iVariable_1 == 4)
        {
            node->iVariable_2 = -9;
            node->iVariable_3 = 9;
        }
        //Wireframe Wall
        else if (node->iVariable_1 == 0)
        {
            node->iVariable_2 = 1;
            node->iVariable_3 = 5;
        }
        else
        {
            node->iVariable_2 = 0;
            node->iVariable_3 = 5;
        }
    }
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
            ProcessSliderContent(node, canvas.zoom);

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
        ImGui::OpenPopup(/*NodesContextMenu*/XorStr<0x4E, 17, 0xF341A15F>("\x00\x20\x34\x34\x21\x10\x3B\x3B\x22\x32\x20\x2D\x17\x3E\x32\x28" + 0xF341A15F).s);
    }

    if (ImGui::BeginPopup(/*NodesContextMenu*/XorStr<0xA7, 17, 0x3F517CFC>("\xE9\xC7\xCD\xCF\xD8\xEF\xC2\xC0\xDB\xD5\xC9\xC6\xFE\xD1\xDB\xC3" + 0x3F517CFC).s))
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
        if (ImGui::MenuItem(/*Reset Zoom*/XorStr<0xBC, 11, 0x474CB760>("\xEE\xD8\xCD\xDA\xB4\xE1\x98\xAC\xAB\xA8" + 0x474CB760).s))
            canvas.zoom = 1;
        if (ImGui::MenuItem(/*Reset Position*/XorStr<0xEB, 15, 0x1FCCD759>("\xB9\x89\x9E\x8B\x9B\xD0\xA1\x9D\x80\x9D\x81\x9F\x98\x96" + 0x1FCCD759).s))
            canvas.offset = ImVec2(0.0f, 0.0f);

        if (ImGui::IsAnyMouseDown() && !ImGui::IsWindowHovered())
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    ImNodes::EndCanvas();
}

const char* GetNodeTitle(int iNodeType)
{
    switch (iNodeType)
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
        case NodeTypeSlider:
            return szSlider;
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

        node->iNodeType = (unsigned int)iNodeType;
        node->szText[0] = 0;
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["Position"]["X"], &node->pos.x);
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["Position"]["Y"], &node->pos.y);
        //
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["Text"], node->szText, MAX_PATH - 1);
        //GetVal(settings["Nodes"][std::to_string(i).c_str()]["dbVariable"], &node->dbVariable);
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["iVariable_1"], &node->iVariable_1);
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["iVariable_2"], &node->iVariable_2);
        GetVal(settings["Nodes"][std::to_string(i).c_str()]["iVariable_3"], &node->iVariable_3);
        //GetVal(settings["Nodes"][std::to_string(i).c_str()]["Size"]["X"], &node->vSize.x);
        //GetVal(settings["Nodes"][std::to_string(i).c_str()]["Size"]["Y"], &node->vSize.y);
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
    //settings["Nodes"][std::to_string(offset).c_str()]["dbVariable"] = CSHackCreator::Settings::Nodes[offset]->dbVariable;
    settings["Nodes"][std::to_string(offset).c_str()]["iVariable_1"] = CSHackCreator::Settings::Nodes[offset]->iVariable_1;
    settings["Nodes"][std::to_string(offset).c_str()]["iVariable_2"] = CSHackCreator::Settings::Nodes[offset]->iVariable_2;
    settings["Nodes"][std::to_string(offset).c_str()]["iVariable_3"] = CSHackCreator::Settings::Nodes[offset]->iVariable_3;
    //settings["Nodes"][std::to_string(offset).c_str()]["Size"]["X"] = CSHackCreator::Settings::Nodes[offset]->vSize.x;
    //settings["Nodes"][std::to_string(offset).c_str()]["Size"]["Y"] = CSHackCreator::Settings::Nodes[offset]->vSize.y;
}

void CSHackCreator::Settings::SaveNodes(Json::Value& settings)
{
    settings["Nodes"]["Count"] = CSHackCreator::Settings::Nodes.size();

    for (unsigned int i = 0; i < CSHackCreator::Settings::Nodes.size(); i++)
    {
        settings["Nodes"][std::to_string(i).c_str()]["Type"] = CSHackCreator::Settings::Nodes[i]->iNodeType;
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