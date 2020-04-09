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

#define CSHACKCREATOR_V2_SIGNATURE "<(BLOODSHARP_CSHACKCREATOR_V2)>"

void CSHackCreator::Project::Build(HWND hwnd)
{
    HRSRC hResource;
    DWORD dwResourceSize;
    HGLOBAL hGlob;
    LPSTR lpBuffer;

    Json::Value settings;
    Json::FastWriter fastWriter;

    std::fstream fsExecutable(CSHackCreator::Settings::szExeFile, std::fstream::out | std::fstream::binary);
    std::fstream fsDll(CSHackCreator::Settings::szDllFile, std::fstream::out | std::fstream::binary);

    CSHackCreator::Settings::Save(settings);
    CSHackCreator::Settings::SaveNodes(settings);

    if (fsExecutable.is_open())
    {
        hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_EXE_STUB), "EXE_STUB");
        dwResourceSize = SizeofResource(NULL, hResource);
        hGlob = LoadResource(NULL, hResource);
        lpBuffer = (LPSTR)LockResource(hGlob);

        fsExecutable.write(lpBuffer, dwResourceSize);
        fsExecutable.write(CSHACKCREATOR_V2_SIGNATURE, strlen(CSHACKCREATOR_V2_SIGNATURE) + 1);
        fsExecutable << fastWriter.write(settings);
        fsExecutable.close();
    }

    if(fsDll.is_open())
    {
        hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_DLL_STUB), "DLL_STUB");
        dwResourceSize = SizeofResource(NULL, hResource);
        hGlob = LoadResource(NULL, hResource);
        lpBuffer = (LPSTR)LockResource(hGlob);

        fsDll.write(lpBuffer, dwResourceSize);
        fsDll.write(CSHACKCREATOR_V2_SIGNATURE, strlen(CSHACKCREATOR_V2_SIGNATURE) + 1);
        fsDll << fastWriter.write(settings);
        fsDll.close();
    }

    MessageBox(hwnd, "Done enjoy your new hack!", "BloodSharp", MB_ICONINFORMATION);
}

bool IsThisAddressContainString(PBYTE dwAddress, PBYTE string)
{
    for (UINT i = 0; i < strlen((char*)string) + 1; i++)
        if (string[i] != dwAddress[i])
            return false;
    return true;
}

void CSHackCreator::Project::Decompile(HWND hwnd)
{
    OPENFILENAME ofn;
    char szFile[MAX_PATH] = { 0 };

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Hack files (*.dll)(*.exe)\0*.dll;*.exe\0All\0*.*\0\0";
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        std::ifstream fsHackFile;
        fsHackFile.open(ofn.lpstrFile, std::ifstream::in | std::ifstream::binary);
        if (fsHackFile.is_open())
        {
            DWORD dwFileSize;
            fsHackFile.seekg(0, fsHackFile.end);
            dwFileSize = (DWORD)fsHackFile.tellg();
            if (dwFileSize)
            {
                fsHackFile.seekg(0, fsHackFile.beg);
                PBYTE pFileBuffer = new BYTE[dwFileSize];
                if (pFileBuffer)
                {
                    fsHackFile.read((char*)pFileBuffer, dwFileSize);

                    for (PBYTE i = pFileBuffer; i < (PBYTE)(((DWORD)pFileBuffer) + dwFileSize); i++)
                    {
                        if (IsThisAddressContainString(i, (PBYTE)CSHACKCREATOR_V2_SIGNATURE))
                        {
                            Json::Value settings;
                            Json::Reader reader;
                            std::string jsonstring;

                            i += strlen(CSHACKCREATOR_V2_SIGNATURE) + 1;
                            jsonstring = ((const char*)i);
                            if (reader.parse(jsonstring, settings))
                            {
                                CleanAllNodesAndConnections();
                                CSHackCreator::Settings::Open(settings);
                                CSHackCreator::Settings::OpenNodes(settings);
                            }
                            break;
                        }
                    }
                    delete[]pFileBuffer;
                }
            }

            fsHackFile.close();
        }
    }
}