#include "ExeStub.h"

// Data
static LPDIRECT3D9           g_pD3D = NULL;
static LPDIRECT3DDEVICE9     g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS g_d3dpp = {};

char szTitle[MAX_PATH];
char szInjectButton[MAX_PATH];
char szWaitForGame[MAX_PATH];
char szInjectionError[MAX_PATH];
char szDllNotFound[MAX_PATH];

char szDllLibrary[MAX_PATH];
char szModulePath[MAX_PATH];

HWND hwnd;
int WINDOW_WIDTH = 400;
int WINDOW_HEIGHT = 240;

HANDLE hInjection = 0;

#define CSHACKCREATOR_V2_SIGNATURE /*<(BLOODSHARP_CSHACKCREATOR_V2)>*/XorStr<0xB4,32,0x84103E60>("\x88\x9D\xF4\xFB\xF7\xF6\xFE\xE8\xF4\xFC\xEC\xEF\x9F\x82\x91\x8B\x85\x86\x8D\x84\x9A\x8C\x8B\x9F\x83\x9F\x91\x99\xE2\xF8\xEC"+0x84103E60).s

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool bProcessMouse);

//int display_w, display_h;
ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
    WNDCLASS wcWindowClass = { 0 };
    wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
    wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
    wcWindowClass.hInstance = hInst;
    wcWindowClass.lpszClassName = lpzClassName;
    wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
    return RegisterClass(&wcWindowClass);
}

bool WINAPI InyectarDLL()
{
    DWORD pid;
    HANDLE proc, hToken = NULL;;
    LPVOID RemoteString, nLoadLibrary;
    HWND VentanaHL;
    TOKEN_PRIVILEGES tkp;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
        tkp.PrivilegeCount = 1; tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, 0, &tkp, sizeof(tkp), NULL, NULL);
    }

    while ((VentanaHL = FindWindowA(/*Valve001*/XorStr<0x06, 9, 0x105F9F6A>("\x50\x66\x64\x7F\x6F\x3B\x3C\x3C" + 0x105F9F6A).s, 0)) == NULL && (VentanaHL = FindWindowA(0,/*Half-Life*/XorStr<0x19, 10, 0x6B9F95B8>("\x51\x7B\x77\x7A\x30\x52\x76\x46\x44" + 0x6B9F95B8).s)) == NULL)
        Sleep(100);

    GetWindowThreadProcessId(VentanaHL, &pid);

    if ((proc = OpenProcess(0xF0000 | 0x100000L | 0xFFF, 0, pid)) == NULL)
        return false;

    if ((nLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandle(/*Kernel32.dll*/XorStr<0xD2, 13, 0x47902CEA>("\x99\xB6\xA6\xBB\xB3\xBB\xEB\xEB\xF4\xBF\xB0\xB1" + 0x47902CEA).s),/*LoadLibraryA*/XorStr<0x88, 13, 0xFA9E3BD2>("\xC4\xE6\xEB\xEF\xC0\xE4\xEC\xFD\xF1\xE3\xEB\xD2" + 0xFA9E3BD2).s)) == NULL)
        return false;
    if ((RemoteString = (LPVOID)VirtualAllocEx(proc, NULL, strlen(szModulePath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)) == NULL)
        return false;
    if (!WriteProcessMemory(proc, (LPVOID)RemoteString, szModulePath, strlen(szModulePath), NULL))
        return false;
    if (!CreateRemoteThread(proc, NULL, 0, (LPTHREAD_START_ROUTINE)nLoadLibrary, (LPVOID)RemoteString, 0, NULL))
        return false;
    CloseHandle(proc);
    return true;
}

void PrepararInyeccionNormal()
{
    if (!InyectarDLL())
        MessageBox(hwnd,szInjectionError,szTitle, MB_ICONERROR | MB_TOPMOST);
    hInjection = 0;
    ExitThread(0);
}

void RenderLoader(bool* pbOpen)
{
    //CH4::Interface::PushWindowsStyles();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    if (ImGui::Begin((const char*)szTitle, pbOpen, ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoBringToFrontOnFocus))
    {
        CH4::Interface::Particles::Render(35);
        if (ImGui::Button((const char*)szInjectButton, ImVec2(ImGui::GetIO().DisplaySize.x - 17, 20)))
        {
            if (!hInjection)
                hInjection = CreateThread(NULL, (DWORD)NULL, (LPTHREAD_START_ROUTINE)PrepararInyeccionNormal, NULL, (DWORD)NULL, NULL);
            else if (hInjection)
                MessageBox(hwnd, szWaitForGame, szTitle, MB_ICONINFORMATION | MB_TOPMOST);
        }
        ImGui::End();
    }
    ImGui::PopStyleVar();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    //Interface();
    ImGui::PopStyleVar(2);
}

void InitializeLoader()
{
    ImGuiStyle& styles = ImGui::GetStyle();
    styles.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    styles.Colors[ImGuiCol_Button].w = 1.0f;
    styles.Colors[ImGuiCol_ButtonActive].w = 1.0f;
    styles.Colors[ImGuiCol_ButtonHovered].w = 1.0f;
}

void GetVal(Json::Value& config, char* setting, int bufferSize)
{
    if (config.isNull())
        return;

    strcpy_s(setting, bufferSize, config.asCString());
}

bool IsThisAddressContainString(PBYTE dwAddress, PBYTE string)
{
    for (UINT i = 0; i < strlen((char*)string) + 1; i++)
        if (string[i] != dwAddress[i])
            return false;
    return true;
}

void InitializeProgram(HINSTANCE hInstance)
{
    Json::Value settings;
    Json::Reader reader;
    std::string jsonstring;
    PBYTE pFileBuffer = 0;
    DWORD dwFileSize = 0;
    char szProgramFile[MAX_PATH];
    std::ifstream fsFile;

    // Valores por defecto...
    strcpy_s(szTitle, MAX_PATH - 1, /*MyHack*/XorStr<0x2B, 7, 0xB08C9101>("\x66\x55\x65\x4F\x4C\x5B" + 0xB08C9101).s);
    strcpy_s(szInjectButton, MAX_PATH - 1, /*Inject MyHack*/XorStr<0x2A, 14, 0xF42F84B4>("\x63\x45\x46\x48\x4D\x5B\x10\x7C\x4B\x7B\x55\x56\x5D" + 0xF42F84B4).s);
    strcpy_s(szWaitForGame, MAX_PATH - 1, /*Waiting for the game to inject...*/XorStr<0x23, 34, 0x93144A54>("\x74\x45\x4C\x52\x4E\x46\x4E\x0A\x4D\x43\x5F\x0E\x5B\x58\x54\x12\x54\x55\x58\x53\x17\x4C\x56\x1A\x52\x52\x57\x5B\x5C\x34\x6F\x6C\x6D" + 0x93144A54).s);
    strcpy_s(szInjectionError, MAX_PATH - 1, /*Injection error!*/XorStr<0xD0, 17, 0x17BAFF77>("\x99\xBF\xB8\xB6\xB7\xA1\xBF\xB8\xB6\xF9\xBF\xA9\xAE\xB2\xAC\xFE" + 0x17BAFF77).s);
    strcpy_s(szDllNotFound, MAX_PATH - 1,/*MyHack.dll not found!*/XorStr<0x69, 22, 0x746A33B7>("\x24\x13\x23\x0D\x0E\x05\x41\x14\x1D\x1E\x53\x1A\x1A\x02\x57\x1E\x16\x0F\x15\x18\x5C" + 0x746A33B7).s);
    strcpy_s(szDllLibrary, MAX_PATH - 1,/*MyHack.dll*/XorStr<0x49, 11, 0xC0B77395>("\x04\x33\x03\x2D\x2E\x25\x61\x34\x3D\x3E" + 0xC0B77395).s);

    GetModuleFileName(hInstance, szProgramFile, MAX_PATH - 1);
    fsFile.open(szProgramFile, std::ifstream::in | std::ifstream::binary);
    if (fsFile.is_open())
    {
        fsFile.seekg(0, fsFile.end);
        dwFileSize = (DWORD)fsFile.tellg();

        if (dwFileSize)
        {
            fsFile.seekg(0, fsFile.beg);
            pFileBuffer = new BYTE[dwFileSize];
            if (pFileBuffer)
            {
                fsFile.read((char*)pFileBuffer, dwFileSize);
                for (PBYTE i = pFileBuffer; i < (PBYTE)(((DWORD)pFileBuffer) + dwFileSize); i++)
                {
                    if (IsThisAddressContainString(i, (PBYTE)CSHACKCREATOR_V2_SIGNATURE))
                    {
                        i += strlen(CSHACKCREATOR_V2_SIGNATURE) + 1;
                        jsonstring = ((const char*)i);
                        if (reader.parse(jsonstring, settings))
                        {
                            GetVal(settings["Loader"]["Title"], szTitle, MAX_PATH - 1);
                            GetVal(settings["Loader"]["Inject"], szInjectButton, MAX_PATH - 1);
                            GetVal(settings["Loader"]["WaitingForTheGame"], szWaitForGame, MAX_PATH - 1);
                            GetVal(settings["Loader"]["InjectionError"], szInjectionError, MAX_PATH - 1);
                            GetVal(settings["Loader"]["DllNotFound"], szDllNotFound, MAX_PATH - 1);

                            GetVal(settings["Files"]["Library"], szDllLibrary, MAX_PATH - 1);
                        }
                        break;
                    }
                }
                delete[]pFileBuffer;
            }
        }
    }
}

BOOL FileExist(LPCSTR filename)
{
    WIN32_FIND_DATAA finddata;
    return (FindFirstFileA(filename, &finddata) != INVALID_HANDLE_VALUE);
}

// Main code
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    InitializeProgram(hInstance);

    GetCurrentDirectory(MAX_PATH - 1, szModulePath);
    strcat_s(szModulePath, MAX_PATH - 1,/*\\*/XorStr<0x9B, 2, 0xE398A510>("\xC7" + 0xE398A510).s);
    strcat_s(szModulePath, MAX_PATH - 1, szDllLibrary);

    if (!FileExist(szModulePath))
    {
        MessageBox(0, szDllNotFound, szTitle, MB_ICONERROR | MB_TOPMOST);
        return 0;
    }

    if (!RegMyWindowClass(hInstance, (LPCTSTR)szTitle))
        return 0;
    hwnd = ::CreateWindowEx(0, (LPCSTR)szTitle, (LPCSTR)szTitle, WS_POPUP,//WS_OVERLAPPEDWINDOW,
        GetSystemMetrics(SM_CXSCREEN) / 2 - WINDOW_WIDTH / 2,
        GetSystemMetrics(SM_CYSCREEN) / 2 - WINDOW_HEIGHT / 2,
        WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass((LPCSTR)szTitle, hInstance);
        return 1;
    }
    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    InitializeLoader();
    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);
    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    bool bOpen = true;
    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }
        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (!bOpen)
            ExitProcess(0);
        RenderLoader(&bOpen);
        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

        ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(int(color.x * 255), int(color.y * 255), int(color.z * 255), 255);
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass((LPCSTR)szTitle, hInstance);
    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;
    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam, true))
        return true;
    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}