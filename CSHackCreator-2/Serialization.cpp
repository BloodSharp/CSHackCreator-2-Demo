#include "CSHackCreator.h"

#if defined(_WIN32)
    #include <Windows.h>
    #include <commdlg.h>
#endif

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

void CSHackCreator::Project::Open(void* hwnd)
{
#if defined(_WIN32)
    OPENFILENAME ofn;
    char szFile[MAX_PATH]={0};

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = (HWND)hwnd;
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
#elif defined(__EMSCRIPTEN__)
#endif
}

void CSHackCreator::Project::Save(void* hwnd)
{
#if defined(_WIN32)
    OPENFILENAME ofn;
    char szFile[MAX_PATH] = { 0 };

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = (HWND)hwnd;
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
#elif defined(__EMSCRIPTEN__)
#endif
}

#define CSHACKCREATOR_V2_SIGNATURE /*<(BLOODSHARP_CSHACKCREATOR_V2)>*/XorStr<0xD2,32,0xCB7E3612>("\xEE\xFB\x96\x99\x99\x98\x9C\x8A\x92\x9A\x8E\x8D\x81\x9C\xB3\xA9\xA3\xA0\xAF\xA6\xB4\xA2\xA9\xBD\xA5\xB9\xB3\xBB\xDC\xC6\xCE"+0xCB7E3612).s

void CSHackCreator::Project::Build(void* hwnd)
{
#if defined(_WIN32)
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
        hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_EXE_STUB), /*EXE_STUB*/XorStr<0xA9, 9, 0x4799BEAD>("\xEC\xF2\xEE\xF3\xFE\xFA\xFA\xF2" + 0x4799BEAD).s);
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
        hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_DLL_STUB), /*DLL_STUB*/XorStr<0x39, 9, 0xDBF4E089>("\x7D\x76\x77\x63\x6E\x6A\x6A\x02" + 0xDBF4E089).s);
        dwResourceSize = SizeofResource(NULL, hResource);
        hGlob = LoadResource(NULL, hResource);
        lpBuffer = (LPSTR)LockResource(hGlob);

        fsDll.write(lpBuffer, dwResourceSize);
        fsDll.write(CSHACKCREATOR_V2_SIGNATURE, strlen(CSHACKCREATOR_V2_SIGNATURE) + 1);
        fsDll << fastWriter.write(settings);
        fsDll.close();
    }

    MessageBox((HWND)hwnd, /*Done enjoy your new hack!*/XorStr<0x1A, 26, 0x5670094F>("\x5E\x74\x72\x78\x3E\x7A\x4E\x4B\x4D\x5A\x04\x5C\x49\x52\x5A\x09\x44\x4E\x5B\x0D\x46\x4E\x53\x5A\x13" + 0x5670094F).s, /*CSHackCreator v2 - BloodSharp*/XorStr<0xCB, 30, 0x38F92DF7>("\x88\x9F\x85\xAF\xAC\xBB\x92\xA0\xB6\xB5\xA1\xB9\xA5\xF8\xAF\xE8\xFB\xF1\xFD\x9C\xB3\x8F\x8E\x86\xB0\x8C\x84\x94\x97" + 0x38F92DF7).s, MB_ICONINFORMATION);
#elif defined(__EMSCRIPTEN__)
#endif
}

bool IsThisAddressContainString(PBYTE dwAddress, PBYTE string)
{
    for (UINT i = 0; i < strlen((char*)string) + 1; i++)
        if (string[i] != dwAddress[i])
            return false;
    return true;
}

void CSHackCreator::Project::Decompile(void* hwnd)
{
#if defined(_WIN32)
    OPENFILENAME ofn;
    char szFile[MAX_PATH] = { 0 };

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = (HWND)hwnd;
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
#elif defined(__EMSCRIPTEN__)
#endif
}