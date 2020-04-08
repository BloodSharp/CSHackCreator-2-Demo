#pragma once

enum SlotType
{
    SlotTypeNone = 0,
    SlotTypeInput,
    SlotTypeOutput,
};

/// A structure defining a connection between two slots of two nodes.
struct Connection
{
    /// `id` that was passed to BeginNode() of input node.
    void* input_node = nullptr;
    /// Descriptor of input slot.
    const char* input_slot = nullptr;
    /// `id` that was passed to BeginNode() of output node.
    void* output_node = nullptr;
    /// Descriptor of output slot.
    const char* output_slot = nullptr;

    bool operator==(const Connection& other) const
    {
        return input_node == other.input_node &&
            input_slot == other.input_slot &&
            output_node == other.output_node &&
            output_slot == other.output_slot;
    }

    bool operator!=(const Connection& other) const
    {
        return !operator ==(other);
    }
};

enum NodeType
{
    NodeTypeWindow = 0,
    NodeTypeTabBar,
    NodeTypeTabItem,
    NodeTypeVariableEquals,
    NodeTypeVariableSetValue,
    NodeTypeGroup,
    NodeTypeText,
    NodeTypeButton,
    NodeTypeComboBox,
    NodeTypeCheckBox,
    NodeTypeToolTip,
    NodeTypeNewLine
};

/// A structure holding node state.
struct MyNode
{
    /// Title which will be displayed at the center-top of the node.
    const char* title = nullptr;
    /// Flag indicating that node is selected by the user.
    bool selected = false;
    /// Node position on the canvas.
    ImVec2 pos{};
    /// List of node connections.
    std::vector<Connection> connections{};
    /// A list of input slots current node has.
    std::vector<ImNodes::Ez::SlotInfo> input_slots{};
    /// A list of output slots current node has.
    std::vector<ImNodes::Ez::SlotInfo> output_slots{};

    /// Properties of each node
    char szText[MAX_PATH]; // Window title or simple text
    unsigned int uiNodeType = 0;
    double dbVariable = 0.0;
    int iVariable_1 = 0;
    int iVariable_2 = 0;
    ImVec2 vSize = ImVec2(0.0f, 0.0f);

    explicit MyNode(const char* title,
        const std::vector<ImNodes::Ez::SlotInfo>&& input_slots,
        const std::vector<ImNodes::Ez::SlotInfo>&& output_slots)
    {
        this->title = title;
        this->input_slots = input_slots;
        this->output_slots = output_slots;
        this->szText[0] = 0;
    }

    explicit MyNode(const char* title,
        SlotType type,
        const std::vector<ImNodes::Ez::SlotInfo>&& slots)
    {
        this->title = title;
        if (type == SlotTypeInput)
            this->input_slots = slots;
        else if (type == SlotTypeOutput)
            this->output_slots = slots;
        this->szText[0] = 0;
    }

    /// Deletes connection from this node.
    void DeleteConnection(const Connection& connection)
    {
        for (auto it = connections.begin(); it != connections.end(); ++it)
        {
            if (connection == *it)
            {
                connections.erase(it);
                break;
            }
        }
    }
};

namespace CSHackCreator
{
    namespace Interface
    {
        void Designer();
    }
    namespace Settings
    {
        extern std::vector<MyNode*> Nodes;
    }
}