#include "CSHackCreator.h"

void CleanAllNodesAndConnections()
{
    for (auto it = CSHackCreator::Settings::Nodes.begin(); it != CSHackCreator::Settings::Nodes.end();)
    {
        MyNode* node = *it;
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
        node->connections.clear();
        delete node;
        it = CSHackCreator::Settings::Nodes.erase(it);
    }
}

void CSHackCreator::Project::New()
{
    CleanAllNodesAndConnections();
    CSHackCreator::Settings::New();
}

void CSHackCreator::Project::Open(HWND hwnd)
{
    OPENFILENAME ofn;
    char szFile[MAX_PATH]={0};

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "JavaScript Object Notation (*.json)\0*.json\0All\0*.*\0\0";
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        Json::Value settings;
        std::ifstream configDoc(ofn.lpstrFile, std::ifstream::binary);
        configDoc >> settings;
        configDoc.close();

        CleanAllNodesAndConnections();
        CSHackCreator::Settings::Open(settings);
        CSHackCreator::Settings::OpenNodes(settings);
    }
}

void CSHackCreator::Project::Save(HWND hwnd)
{
    OPENFILENAME ofn;
    char szFile[MAX_PATH] = { 0 };

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "JavaScript Object Notation (*.json)\0*.json\0All\0*.*\0\0";
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrDefExt = "json";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetSaveFileName(&ofn) == TRUE)
    {
        Json::Value settings;
        Json::StyledWriter styledWriter;

        CSHackCreator::Settings::Save(settings);
        CSHackCreator::Settings::SaveNodes(settings);

        std::ofstream configDoc = std::ofstream(ofn.lpstrFile);
        configDoc << styledWriter.write(settings);
        configDoc.close();
    }
}

void CSHackCreator::Project::Build(HWND hwnd)
{
    Json::Value settings;
    Json::FastWriter fastWriter;

    std::fstream fsExecutable(CSHackCreator::Settings::szExeFile, std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);
    std::fstream fsDll(CSHackCreator::Settings::szDllFile, std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);

    CSHackCreator::Settings::Save(settings);

    if (fsExecutable.is_open())
    {
        fsExecutable << fastWriter.write(settings);
        fsExecutable.close();
    }

    if(fsDll.is_open())
    {
        fsDll << fastWriter.write(settings);
        fsDll.close();
    }

    MessageBox(hwnd, "Done enjoy your new hack!", "BloodSharp", MB_ICONINFORMATION);
}