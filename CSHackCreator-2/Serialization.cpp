#include "CSHackCreator.h"

#if defined(_WIN32)
    #include <Windows.h>
    #include <commdlg.h>
#elif defined(__EMSCRIPTEN__)
bool g_bShouldReadProjectFile = false;
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
    EM_ASM({
        function cshcv2onLoadContent(event) {
            FS.writeFile("/Project.txt", event.target.result);
        }

        function cshcv2onReadFiles(event) {
            const file = event.target.files[0];
            cshcv2reader = new FileReader();
            cshcv2reader.addEventListener('load', cshcv2onLoadContent);
            cshcv2reader.readAsText(file, 'UTF-8');
        }

        var cshcv2inputObject = document.createElement('input');
        cshcv2inputObject.type = 'file';
        cshcv2inputObject.accept = '*.json';
        cshcv2inputObject.addEventListener('change', cshcv2onReadFiles);
        cshcv2inputObject.click();
    });

    g_bShouldReadProjectFile = true;
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
    Json::Value settings;
    Json::StyledWriter styledWriter;

    CSHackCreator::Settings::Save(settings);
    CSHackCreator::Settings::SaveNodes(settings);
    EM_ASM({
        var blob = new Blob([UTF8ToString($0,$1)], {type: 'application/json'});// change resultByte to bytes
        var link = document.createElement('a');
        link.href = window.URL.createObjectURL(blob);
        link.download = 'CSHackCreatorProject.json';
        link.click();
    }, styledWriter.write(settings).c_str(), strlen(styledWriter.write(settings).c_str()));
#endif
}

#define CSHACKCREATOR_V2_SIGNATURE "<(BLOODSHARP_CSHACKCREATOR_V2)>"

void CSHackCreator::Project::Build(void* hwnd)
{
#if defined(_WIN32)
    HRSRC hResource;
    DWORD dwResourceSize;
    HGLOBAL hGlob;
#endif
    char* lpBuffer = 0;
    Json::Value settings;
    Json::FastWriter fastWriter;

    std::fstream fsExecutable(CSHackCreator::Settings::szExeFile, std::fstream::out | std::fstream::binary);
    std::fstream fsDll(CSHackCreator::Settings::szDllFile, std::fstream::out | std::fstream::binary);

    CSHackCreator::Settings::Save(settings);
    CSHackCreator::Settings::SaveNodes(settings);

    if (fsExecutable.is_open())
    {
#if defined(_WIN32)
        hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_EXE_STUB), /*EXE_STUB*/XorStr<0xA9, 9, 0x4799BEAD>("\xEC\xF2\xEE\xF3\xFE\xFA\xFA\xF2" + 0x4799BEAD).s);
        dwResourceSize = SizeofResource(NULL, hResource);
        hGlob = LoadResource(NULL, hResource);
        lpBuffer = (LPSTR)LockResource(hGlob);

        fsExecutable.write(lpBuffer, dwResourceSize);
#elif defined(__EMSCRIPTEN__)
        std::fstream fsExeStub("./ExeStub.exe", std::fstream::in | std::fstream::binary);
        if (fsExeStub.is_open())
        {
            fsExeStub.seekg(0, fsExeStub.end);
            int iFileSize = fsExeStub.tellg();
            if (iFileSize > 0)
            {
                fsExeStub.seekg(0, fsExeStub.beg);
                lpBuffer = new char[iFileSize];
                if (lpBuffer)
                {
                    fsExeStub.read(lpBuffer, iFileSize);
                    fsExecutable.write(lpBuffer, iFileSize);
                    fsExeStub.close();
                    delete[]lpBuffer;
                }
                else
                {
                    fsExecutable.close();
                    fsDll.close();
                    fsExeStub.close();
                    return;
                }
            }
            else
            {
                fsExecutable.close();
                fsDll.close();
                fsExeStub.close();
                return;
            }
        }
#endif

        fsExecutable.write(CSHACKCREATOR_V2_SIGNATURE, strlen(CSHACKCREATOR_V2_SIGNATURE) + 1);
        fsExecutable << fastWriter.write(settings);
        fsExecutable.close();
    }

    if(fsDll.is_open())
    {
#if defined(_WIN32)
        hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_DLL_STUB), /*DLL_STUB*/XorStr<0x39, 9, 0xDBF4E089>("\x7D\x76\x77\x63\x6E\x6A\x6A\x02" + 0xDBF4E089).s);
        dwResourceSize = SizeofResource(NULL, hResource);
        hGlob = LoadResource(NULL, hResource);
        lpBuffer = (LPSTR)LockResource(hGlob);
        fsDll.write(lpBuffer, dwResourceSize);
#elif defined(__EMSCRIPTEN__)
        std::fstream fsDllStub("./DllStub.dll", std::fstream::in | std::fstream::binary);
        if (fsDllStub.is_open())
        {
            fsDllStub.seekg(0, fsDllStub.end);
            int iFileSize = fsDllStub.tellg();
            if (iFileSize > 0)
            {
                fsDllStub.seekg(0, fsDllStub.beg);
                lpBuffer = new char[iFileSize];
                if (lpBuffer)
                {
                    fsDllStub.read(lpBuffer, iFileSize);
                    fsDll.write(lpBuffer, iFileSize);
                    fsDllStub.close();
                    delete[]lpBuffer;
                }
                else
                {
                    fsExecutable.close();
                    fsDll.close();
                    fsDllStub.close();
                    return;
                }
            }
            else
            {
                fsExecutable.close();
                fsDll.close();
                fsDllStub.close();
                return;
            }
        }
#endif
        fsDll.write(CSHACKCREATOR_V2_SIGNATURE, strlen(CSHACKCREATOR_V2_SIGNATURE) + 1);
        fsDll << fastWriter.write(settings);
        fsDll.close();
    }
#if defined(_WIN32)
    MessageBox((HWND)hwnd, /*Done enjoy your new hack!*/XorStr<0x1A, 26, 0x5670094F>("\x5E\x74\x72\x78\x3E\x7A\x4E\x4B\x4D\x5A\x04\x5C\x49\x52\x5A\x09\x44\x4E\x5B\x0D\x46\x4E\x53\x5A\x13" + 0x5670094F).s, /*CSHackCreator v2 - BloodSharp*/XorStr<0xCB, 30, 0x38F92DF7>("\x88\x9F\x85\xAF\xAC\xBB\x92\xA0\xB6\xB5\xA1\xB9\xA5\xF8\xAF\xE8\xFB\xF1\xFD\x9C\xB3\x8F\x8E\x86\xB0\x8C\x84\x94\x97" + 0x38F92DF7).s, MB_ICONINFORMATION);
#elif defined(__EMSCRIPTEN__)
    SDL_Delay(1000);
    EM_ASM({
        var blob=new Blob([FS.readFile('/'+UTF8ToString($0,$1))], {type: 'application/exe'});// change resultByte to bytes
        var link=document.createElement('a');
        link.href=window.URL.createObjectURL(blob);
        link.download=UTF8ToString($0,$1);
        link.click();
    }
    , CSHackCreator::Settings::szExeFile, strlen(CSHackCreator::Settings::szExeFile));

    EM_ASM({
        var blob=new Blob([FS.readFile('/'+UTF8ToString($0,$1))], {type: 'application/dll'});// change resultByte to bytes
        var link=document.createElement('a');
        link.href=window.URL.createObjectURL(blob);
        link.download=UTF8ToString($0,$1);
        link.click();
    }
    , CSHackCreator::Settings::szDllFile, CSHackCreator::Settings::szDllFile);
#endif

}

bool IsThisAddressContainString(unsigned char* dwAddress, unsigned char* string)
{
    for (unsigned int i = 0; i < strlen((char*)string) + 1; i++)
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