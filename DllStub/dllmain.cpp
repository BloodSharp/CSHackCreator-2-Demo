#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
    HMODULE hOpenGL32 = 0;
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        hOpenGL32 = LoadLibrary("opengl32.dll");
        pOrig_glBegin = (decltype(pOrig_glBegin))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, "glBegin")), (PBYTE)HOOK_glBegin);
        pOrig_glClear= (decltype(pOrig_glClear))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, "glClear")), (PBYTE)HOOK_glClear);
        pOrig_glVertex2f= (decltype(pOrig_glVertex2f))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, "glVertex2f")), (PBYTE)HOOK_glVertex2f);
        pOrig_glVertex3fv= (decltype(pOrig_glVertex3fv))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, "glVertex3fv")), (PBYTE)HOOK_glVertex3fv);
        pOrig_glVertex3f= (decltype(pOrig_glVertex3f))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, "glVertex3f")), (PBYTE)HOOK_glVertex3f);
        pOrig_glShadeModel= (decltype(pOrig_glShadeModel))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, "glShadeModel")), (PBYTE)HOOK_glShadeModel);
        pOrig_glPushMatrix= (decltype(pOrig_glPushMatrix))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, "glPushMatrix")), (PBYTE)HOOK_glPushMatrix);
        pOrig_glPopMatrix= (decltype(pOrig_glPopMatrix))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, "glPopMatrix")), (PBYTE)HOOK_glPopMatrix);
        pOrig_glEnable= (decltype(pOrig_glEnable))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, "glEnable")), (PBYTE)HOOK_glEnable);
        pOrig_glDisable= (decltype(pOrig_glDisable))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, "glDisable")), (PBYTE)HOOK_glDisable);
        pOrig_glViewport= (decltype(pOrig_glViewport))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, "glViewport")), (PBYTE)HOOK_glViewport);
        pOrig_glFrustum= (decltype(pOrig_glFrustum))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, "glFrustum")), (PBYTE)HOOK_glFrustum);

        pOrig_QueryPerformanceCounter = (decltype(pOrig_QueryPerformanceCounter))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(LoadLibrary("kernel32.dll"), "QueryPerformanceCounter")), (PBYTE)HOOK_QueryPerformanceCounter);
    }
    return TRUE;
}

