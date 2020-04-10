#include "CSHackCreator.h"

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

RECT rect;
int width = 800;
int height = 600;

int center_window(HWND parent_window, int width, int height)
{
    GetClientRect(parent_window, &rect);
    rect.left = (rect.right / 2) - (width / 2);
    rect.top = (rect.bottom / 2) - (height / 2);
    return 0;
}

// Main code
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    center_window(GetDesktopWindow(), width, height);

    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("CSHackCreator"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("CSHackCreator v2 - BloodSharp"), WS_OVERLAPPEDWINDOW, rect.left, rect.top, width, height, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    CSHackCreator::Settings::New();
    CH4::Radio::Initialize();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.MouseDrawCursor = true;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
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

        //ImGui::ShowDemoWindow();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
        //const ImGuiStyle& style = ImGui::GetStyle();
        if (ImGui::Begin(/*CSHackCreator*/XorStr<0xF9, 14, 0x32B4D256>("\xBA\xA9\xB3\x9D\x9E\x95\xBC\x72\x64\x63\x77\x6B\x77" + 0x32B4D256).s, nullptr,
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_MenuBar |
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
        ))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu(/*Project*/XorStr<0x19, 8, 0x151F9396>("\x49\x68\x74\x76\x78\x7D\x6B" + 0x151F9396).s))
                {
                    if (ImGui::MenuItem(/*New*/XorStr<0x66, 4, 0xBF259BAF>("\x28\x02\x1F" + 0xBF259BAF).s))
                        CSHackCreator::Project::New();
                    if (ImGui::MenuItem(/*Open*/XorStr<0xBE, 5, 0x93BE5D6D>("\xF1\xCF\xA5\xAF" + 0x93BE5D6D).s))
                        CSHackCreator::Project::Open(hwnd);
                    if (ImGui::MenuItem(/*Save*/XorStr<0x48, 5, 0xB9028F42>("\x1B\x28\x3C\x2E" + 0xB9028F42).s))
                        CSHackCreator::Project::Save(hwnd);
                    ImGui::Separator();
                    if (ImGui::MenuItem(/*Build*/XorStr<0x8F, 6, 0xC92D86F6>("\xCD\xE5\xF8\xFE\xF7" + 0xC92D86F6).s))
                        CSHackCreator::Project::Build(hwnd);
                    if (ImGui::MenuItem(/*Decompile*/XorStr<0x0D, 10, 0xEC36BFAD>("\x49\x6B\x6C\x7F\x7C\x62\x7A\x78\x70" + 0xEC36BFAD).s))
                        CSHackCreator::Project::Decompile(hwnd);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu(/*Help*/XorStr<0xF1, 5, 0xBD0C97EC>("\xB9\x97\x9F\x84" + 0xBD0C97EC).s))
                {
                    if (ImGui::MenuItem(/*Designer help*/XorStr<0xAF, 14, 0x85E2EAC4>("\xEB\xD5\xC2\xDB\xD4\xDA\xD0\xC4\x97\xD0\xDC\xD6\xCB" + 0x85E2EAC4).s))
                    {
                        MessageBox(hwnd,
                            /*Usage:\n\nRight mouse button: Display nodes popup menu.\nMiddle mouse button: Move around the canvas.\nDouble left mouse click: Delete connections.\nDelete key: Delete all selected nodes.*/XorStr<0x95, 183, 0x31FD672D>("\xC0\xE5\xF6\xFF\xFC\xA0\x91\x96\xCF\xF7\xF8\xC8\xD5\x82\xCE\xCB\xD0\xD5\xC2\x88\xCB\xDF\xDF\xD8\xC2\xC0\x95\x90\xF5\xDB\xC0\xC4\xD9\xD7\xCE\x98\xD7\xD5\xDF\xD9\xCE\x9E\xCF\xAF\xB1\xB7\xB3\xE4\xA8\xA3\xA9\xBD\xE7\xC0\x86\xA5\xA9\xAA\xA3\xB5\xF1\xBF\xBC\xA1\xA6\xB3\xF7\xBA\xAC\xAE\xAF\xB3\xB3\xE4\xFF\xAD\x8E\x94\x86\xC4\x84\x94\x88\x9D\x87\x8E\xCB\x98\x85\x8B\xCF\x93\x90\x9C\x85\x95\x86\xD8\xFD\xBC\x96\x8F\x99\x90\x98\xDE\x93\x65\x67\x76\x23\x69\x6A\x73\x74\x6D\x29\x69\x67\x65\x6E\x65\x35\x30\x55\x77\x7F\x71\x61\x73\x37\x7B\x76\x74\x75\x79\x7E\x6A\x76\x4F\x4F\x51\x0D\x2E\x61\x43\x4B\x4D\x5D\x4F\x0B\x47\x48\x57\x15\x10\x75\x57\x5F\x51\x41\x53\x17\x59\x55\x56\x1B\x4F\x58\x52\x5A\x23\x35\x27\x27\x64\x2B\x29\x23\x2D\x3A\x64" + 0x31FD672D).s,
                            /*CSHackCreator v2 - BloodSharp*/XorStr<0x5C, 30, 0x8325D1DD>("\x1F\x0E\x16\x3E\x03\x0A\x21\x11\x01\x04\x12\x08\x1A\x49\x1C\x59\x4C\x40\x4E\x2D\x1C\x1E\x1D\x17\x27\x1D\x17\x05\x08" + 0x8325D1DD).s, MB_ICONINFORMATION);
                    }
                    ImGui::Separator();
                    if (ImGui::MenuItem(/*About*/XorStr<0x5C, 6, 0x0C638648>("\x1D\x3F\x31\x2A\x14" + 0x0C638648).s))
                    {
                        MessageBox(hwnd,
                            /*Coded by Agustin Alejandro dos Santos in Argentina\n\nCredits:\nOmar Cornut from Dear ImGui library.\nBaptiste Lepilleur from JSONCPP library.\nAnd many people from Gamedeception.net community...*/XorStr<0xC6, 191, 0x31C739F8>("\x85\xA8\xAC\xAC\xAE\xEB\xAE\xB4\xEE\x8E\xB7\xA4\xA1\xA7\xBD\xBB\xF6\x96\xB4\xBC\xB0\xBA\xB2\xB9\xAC\xB0\xC0\x85\x8D\x90\xC4\xB6\x87\x89\x9C\x86\x99\xCB\x85\x83\xCE\xAE\x82\x96\x97\x9D\x80\x9C\x98\x96\xF2\xF3\xB9\x89\x99\x99\x97\x8B\x73\x3B\x08\x4C\x69\x64\x74\x27\x4B\x66\x78\x65\x79\x79\x2E\x69\x62\x7E\x7F\x33\x50\x70\x77\x65\x38\x50\x77\x5C\x69\x74\x3E\x73\x49\x43\x50\x42\x56\x5C\x08\x2D\x6A\x48\x5A\x5F\x45\x5E\x5A\x4A\x10\x7D\x57\x43\x5D\x59\x5A\x52\x4D\x4B\x1A\x5D\x4E\x52\x53\x1F\x0A\x12\x0D\x0D\x07\x15\x16\x67\x24\x20\x28\x39\x2D\x3F\x37\x61\x5A\x10\x3C\x37\x74\x38\x37\x39\x21\x79\x2A\x3E\x33\x2D\x32\x3A\x40\x07\x10\x0C\x09\x45\x21\x06\x05\x0C\x0E\x0E\x0F\x08\x1E\x1B\x19\x1E\x1C\x5D\x1A\x10\x02\x57\x1B\x16\x17\x16\x09\x13\x17\x0B\xF9\xAF\xAC\xAD" + 0x31C739F8).s,
                            /*CSHackCreator v2 - BloodSharp*/XorStr<0x45, 30, 0xAEAA793F>("\x06\x15\x0F\x29\x2A\x21\x08\x3E\x28\x2F\x3B\x3F\x23\x72\x25\x66\x75\x7B\x77\x1A\x35\x35\x34\x38\x0E\x36\x3E\x12\x11" + 0xAEAA793F).s, MB_ICONINFORMATION);
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            if (ImGui::BeginTabBar(/*CSHackCreator - TabBar*/XorStr<0xCB, 23, 0x7F7F1E1E>("\x88\x9F\x85\xAF\xAC\xBB\x92\xA0\xB6\xB5\xA1\xB9\xA5\xF8\xF4\xFA\x8F\xBD\xBF\x9C\xBE\x92" + 0x7F7F1E1E).s))
            {
                if (ImGui::BeginTabItem(/*Settings*/XorStr<0xD1, 9, 0x7EC68449>("\x82\xB7\xA7\xA0\xBC\xB8\xB0\xAB" + 0x7EC68449).s))
                {
                    CSHackCreator::Interface::Settings();
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem(/*Designer*/XorStr<0xB9, 9, 0xD120C7B2>("\xFD\xDF\xC8\xD5\xDA\xD0\xDA\xB2" + 0xD120C7B2).s))
                {
                    CSHackCreator::Interface::Designer();
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }

            ImGui::PopStyleVar();
        }
        ImGui::End();
        ImGui::PopStyleVar();

        // Rendering
        ImGui::EndFrame();

        CH4::Radio::Play();

        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*255.0f), (int)(clear_color.y*255.0f), (int)(clear_color.z*255.0f), (int)(clear_color.w*255.0f));
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
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

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

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
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
