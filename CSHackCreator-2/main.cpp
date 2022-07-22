#include "CSHackCreator.h"

// Emscripten requires to have full control over the main loop. We're going to store our SDL book-keeping variables globally.
// Having a single function that acts as a loop prevents us to store state in the stack of said function. So we need some location for this.
SDL_Window* g_Window = NULL;
SDL_GLContext   g_GLContext = NULL;
#if defined(__EMSCRIPTEN__)
void* g_Hwnd = 0;
#elif defined(_WIN32)
HWND g_Hwnd = 0;
#endif
bool g_bDone = false;

void main_loop(void* arg)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_UNUSED(arg);

    // Our state (make them static = more or less global) as a convenience to keep the example terse.
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // We can pass this argument as the second parameter of emscripten_set_main_loop_arg(), but we don't use that.
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
#if not defined(__EMSCRIPTEN__)
        if (event.type == SDL_QUIT)
            g_bDone = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(g_Window))
            g_bDone = true;
#endif
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    //const ImGuiStyle& style = ImGui::GetStyle();
    if (ImGui::Begin("CSHackCreator", nullptr,
        /*ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |*/ ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
    ))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Project##MAIN_MENU"))
            {
                if (ImGui::MenuItem("New##MAIN_MENU"))
                {
                    CSHackCreator::Project::New();
                }
                if (ImGui::MenuItem("Open##MAIN_MENU"))
                {
                    CSHackCreator::Project::Open((void*)g_Hwnd);
                }
                if (ImGui::MenuItem("Save##MAIN_MENU"))
                {
                    CSHackCreator::Project::Save((void*)g_Hwnd);
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Build##MAIN_MENU"))
                {
                    CSHackCreator::Project::Build((void*)g_Hwnd);
                }
                if (ImGui::MenuItem("Decompile##MAIN_MENU"))
                {
                    CSHackCreator::Project::Decompile((void*)g_Hwnd);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help##MAIN_MENU"))
            {
                if (ImGui::MenuItem("Designer help##MAIN_MENU"))
                {
#if defined(_WIN32)
                    MessageBox(g_Hwnd,
                        "Usage:\n\nRight mouse button : Display nodes popup menu.\nMiddle mouse button : Move around the canvas.\nDouble left mouse click : Delete connections.\nDelete key : Delete all selected nodes.",
                        "CSHackCreator v2 - BloodSharp", MB_ICONINFORMATION);
#endif
                }
                ImGui::Separator();
                if (ImGui::MenuItem("About##MAIN_MENU"))
                {
#if defined(_WIN32)
                    MessageBox(g_Hwnd,
                        "Coded by Agustin Alejandro dos Santos in Argentina\n\nCredits:\nOmar Cornut from Dear ImGui library.\nBaptiste Lepilleur from JSONCPP library.\nAnd many people from Gamedeception.net community...",
                        "CSHackCreator v2 - BloodSharp", MB_ICONINFORMATION);
#endif
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (ImGui::BeginTabBar("CSHackCreator - TabBar"))
        {
            if (ImGui::BeginTabItem("Settings##MAIN_TABBAR"))
            {
                CSHackCreator::Interface::Settings();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Designer##MAIN_TABBAR"))
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
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(g_Window);
}

// Main code
#if defined _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
int main(int, char**)
#endif
{
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

#if not defined(__EMSCRIPTEN__)
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
#else
    // For the browser using Emscripten, we are going to use WebGL1 with GL ES2. See the Makefile. for requirement details.
    // It is very likely the generated file won't work in many browsers. Firefox is the only sure bet, but I have successfully
    // run this code on Chrome for Android for example.
    const char* glsl_version = "#version 100";
    //const char* glsl_version = "#version 300 es";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
#if defined(__EMSCRIPTEN__)
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
#endif
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    g_Window = SDL_CreateWindow("CSHackCreator v2 Demo - BloodSharp", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, window_flags);
    g_GLContext = SDL_GL_CreateContext(g_Window);
    if (!g_GLContext)
    {
        fprintf(stderr, "Failed to initialize WebGL context!\n");
        return 1;
    }
#if not defined(__EMSCRIPTEN__)
    SDL_GL_MakeCurrent(g_Window, g_GLContext);
#endif
    SDL_GL_SetSwapInterval(1); // Enable vsync

#if defined(_WIN32)
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(g_Window, &wmInfo);
    g_Hwnd = (HWND)wmInfo.info.win.window;
#else
    g_Hwnd = 0;
#endif
    CSHackCreator::Settings::New();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(g_Window, g_GLContext);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Main loop
#if not defined(__EMSCRIPTEN__)
    g_bDone = false;
    while (!g_bDone)
    {
        main_loop(0);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(g_GLContext);
    SDL_DestroyWindow(g_Window);
    SDL_Quit();
#else
    // This function call won't return, and will engage in an infinite loop, processing events from the browser, and dispatching them.
    emscripten_set_main_loop_arg(main_loop, NULL, 0, true);
#endif

    return 0;
}
