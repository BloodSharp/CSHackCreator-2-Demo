#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cstdio>
#include <fstream>
#include <corecrt_math_defines.h>
#include <cmath>

#include "ch4_lib_utils.h"

#include "JSON/json.h"
#include "JSON/json-forwards.h"

#include "xorstr.h"

typedef struct
{
    bool bActivated;
    int  iWallHack;
    int  iExtraWalls;
    bool bNoSmoke;
    bool bNoFlash;
    bool bNoSky;
    int  iExtraModels;
    int  iWireframeWall;
    int  iWiremodels;
    int  iCrosshair;
    int  iAimTeam;
    bool bEspBox;
    bool bBunnyHop;
    bool bZoom;
    int  iZoom;
    int  iNoRecoil;
    int  iSpeed;
}cvar;

typedef struct
{
    GLboolean get;
    GLfloat highest_x;
    GLfloat highest_y;
    GLfloat highest_z;
    GLfloat lowest_x;
    GLfloat lowest_y;
    GLfloat lowest_z;
    GLfloat vertex;
}model_s;

typedef struct
{
    GLint team;
    GLfloat vTexOneX;
    GLfloat vTexOneY;
    GLfloat vTexOneZ;
    GLfloat vTexTwoX;
    GLfloat vTexTwoY;
    GLfloat vTexTwoZ;
    GLfloat vTexDist;
    GLfloat fr, fg, fb;
    GLfloat vertex;
}player_s;

void (APIENTRY* pOrig_glBegin)(GLenum mode) = 0;
void (APIENTRY* pOrig_glClear)(GLbitfield mask) = 0;
void (APIENTRY* pOrig_glVertex2f)(GLfloat x, GLfloat y) = 0;
void (APIENTRY* pOrig_glVertex3fv)(const GLfloat* v) = 0;
void (APIENTRY* pOrig_glVertex3f)(GLfloat x, GLfloat y, GLfloat z) = 0;
void (APIENTRY* pOrig_glShadeModel)(GLenum mode) = 0;
void (APIENTRY* pOrig_glPushMatrix)() = 0;
void (APIENTRY* pOrig_glPopMatrix)() = 0;
void (APIENTRY* pOrig_glEnable)(GLenum mode) = 0;
void (APIENTRY* pOrig_glDisable)(GLenum mode) = 0;
void (APIENTRY* pOrig_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height) = 0;
void (APIENTRY* pOrig_glFrustum)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) = 0;

WNDPROC pHLWndProc=NULL;
HWND hdHalfLife;

POINT pt; model_s model; player_s player;
char szConfigFile[MAX_PATH];
GLint iView[4]; GLdouble dModel[16], dProy[16]; unsigned int coil = 0;
GLboolean bOnSpeed = 0, bKeyAimbot = 0, bShoot = 0, bHasTarget = 0, bSmoke = 0, bSky = 0, bFlash = 0,
bTex = 0, bDrawn = 0, modelviewport = 0, bLeftButtonDown = 0;
cvar cfg;

void LoadConfig()
{
    FILE* fi = fopen(szConfigFile, "r");
    if (fi)
    {
        fread(&cfg, sizeof(cfg), 1, fi);
        fclose(fi);
    }
}

void SaveConfig()
{
    FILE* fi = fopen(szConfigFile, "w");
    if (fi)
    {
        fwrite(&cfg, sizeof(cfg), 1, fi);
        fclose(fi);
    }
}

GLfloat GetDist(GLfloat firstX, GLfloat firstY, GLfloat firstZ, GLfloat secX, GLfloat secY, GLfloat secZ)
{
    return (GLfloat)sqrt((((secX - firstX) * (secX - firstX)) + ((secY - firstY) * (secY - firstY)) + ((secZ - firstZ) * (secZ - firstZ))));
}

void DrawBox(GLfloat x, GLfloat y, GLfloat dist)
{
    GLint mMode;
    glGetIntegerv(GL_MATRIX_MODE, &mMode);
    pOrig_glEnable(GL_POINT_SMOOTH);
    pOrig_glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    pOrig_glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    pOrig_glPushMatrix();
    glLoadIdentity();

    pOrig_glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glLineWidth(4.0f);
    glColor4f(player.fr, player.fg, player.fb, 0.6f);
    pOrig_glBegin(GL_CURRENT_BIT | GL_POINT_BIT);
    pOrig_glVertex2f(x - dist * 2.5, y + dist * 3);
    pOrig_glVertex2f(x - dist * 2.5, y + dist * 10);
    pOrig_glVertex2f(x + dist * 2.5, y + dist * 10);
    pOrig_glVertex2f(x + dist * 2.5, y + dist * 3);
    pOrig_glVertex2f(x - dist * 2.5, y + dist * 3);
    glEnd();

    pOrig_glDisable(GL_BLEND);

    glMatrixMode(GL_PROJECTION);
    pOrig_glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    pOrig_glPopMatrix();

    glMatrixMode(mMode);

    pOrig_glEnable(GL_TEXTURE_2D);
    pOrig_glDisable(GL_POINT_SMOOTH);
}


void APIENTRY HOOK_glBegin(GLenum mode)
{
    float col[4];
    if (cfg.bActivated)
    {
        switch (cfg.iWallHack)
        {
            case 1:
                if (mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN)
                    pOrig_glDisable(GL_DEPTH_TEST);
                else if (mode != GL_QUADS && mode != GL_LINES)
                    pOrig_glEnable(GL_DEPTH_TEST);
                break;
            case 2:
                if (!(mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN || mode == GL_QUADS))
                {
                    glGetFloatv(GL_CURRENT_COLOR, col);
                    pOrig_glDisable(GL_DEPTH_TEST);
                    pOrig_glEnable(GL_BLEND);
                    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE);
                    glColor4f(col[0], col[1], col[2], .667f);
                }
                else if (mode == GL_TRIANGLES || mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN)
                {
                    pOrig_glEnable(GL_DEPTH_TEST);
                    pOrig_glDisable(GL_BLEND);
                }
                break;
            default:
                break;
        }
        if (!(mode == GL_TRIANGLES || mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN || mode == GL_QUADS))
        {
            if (cfg.bNightmode)
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
            if (cfg.bWhiteWalls)
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
            if (cfg.bFullbright && bTex)
                pOrig_glDisable(GL_TEXTURE_2D);
        }
        else if (cfg.bNiggermodels)
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

        if (mode == GL_QUADS)
        {
            if (cfg.bNoFlash || cfg.bNoSmoke)
            {
                glGetFloatv(GL_CURRENT_COLOR, col);
                bSmoke = (col[0] == col[1] && col[0] == col[2] && col[0] != 0.0 && col[0] != 1.0);
                bFlash = (col[0] == 1.0 && col[1] == 1.0 && col[2] == 1.0 && col[3] > 0.2);
            }
            bSky = 1;
        }
        else
            bSky = 0;

        if (cfg.iWallHack == 3)
        {
            if (mode == GL_POLYGON)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glLineWidth(1.0f);
                glColor3f(1.0f, 1.0f, 1.0f);
            }
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        if (cfg.bWiremodels)
        {
            if (mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glLineWidth(1.0f);
            }
            else if (!cfg.bWireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    pOrig_glBegin(mode);
}

void APIENTRY HOOK_glClear(GLbitfield mask)
{
    if (cfg.bActivated && mask == GL_DEPTH_BUFFER_BIT)
    {
        mask += GL_COLOR_BUFFER_BIT;
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }
    pOrig_glClear(mask);
}

void APIENTRY HOOK_glVertex2f(GLfloat x, GLfloat y)
{
    if (cfg.bActivated && cfg.bNoFlash && bFlash && x == 0.0 && y == 0.0)
        glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    pOrig_glVertex2f(x, y);
}

void APIENTRY HOOK_glVertex3fv(const GLfloat* v)
{
    if (cfg.bActivated)
    {
        if (cfg.bNoSmoke && bSmoke)
            return;
        if ((cfg.bNoSky || cfg.iWallHack == 2) && bSky)
            return;
        modelviewport = 1;
    }
    pOrig_glVertex3fv(v);
}

void APIENTRY HOOK_glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
    if (cfg.bActivated)
    {
        if (player.vertex == 8)
        {
            player.vTexOneX = x;
            player.vTexOneY = y;
            player.vTexOneZ = z;
        }
        if (player.vertex == 9)
        {
            player.vTexTwoX = x;
            player.vTexTwoY = y;
            player.vTexTwoZ = z;
        }
        if (player.vertex == 10)
        {
            player.vTexDist = GetDist(player.vTexOneX, player.vTexOneY, player.vTexOneZ, player.vTexTwoX, player.vTexTwoY, player.vTexTwoZ);
            //CT'S
            if (
                ((player.vTexDist > 3.63f) && (player.vTexDist < 3.65f)) ||
                ((player.vTexDist > 10.6f) && (player.vTexDist < 10.8f)) ||
                ((player.vTexDist > 0.34f) && (player.vTexDist < 0.36f)) ||
                ((player.vTexDist > 2.85f) && (player.vTexDist < 2.87f)) ||
                ((player.vTexDist > 3.31f) && (player.vTexDist < 3.33f)))
                player.team = 1;
            //T'S
            else if (
                ((player.vTexDist > 2.31f) && (player.vTexDist < 2.33f)) ||
                ((player.vTexDist > 3.69f) && (player.vTexDist < 3.71f)) ||
                ((player.vTexDist > 2.22f) && (player.vTexDist < 2.24f)) ||
                ((player.vTexDist > 2.08f) && (player.vTexDist < 2.10f)))
                player.team = 2;
            //colors
            if (player.team == 1)
            {
                player.fr = 0.0f;
                player.fg = 0.0f;
                player.fb = 1.0f;
                /*
                player.fr = Colors[3].r / 255;
                player.fg = Colors[3].g / 255;
                player.fb = Colors[3].b / 255;
                */
            }
            else if (player.team == 2)
            {
                player.fr = 1.0f;
                player.fg = 0.0f;
                player.fb = 0.0f;
                /*
                player.fr = Colors[2].r / 255;
                player.fg = Colors[2].g / 255;
                player.fb = Colors[2].b / 255;
                */
            }
            else
            {
                player.fr = 0.0f;
                player.fg = 1.0f;
                player.fb = 0.0f;
                /*
                player.fr = Colors[4].r / 255;
                player.fg = Colors[4].g / 255;
                player.fb = Colors[4].b / 255;
                */
            }
        }
        if (model.get)
        {
            model.vertex++;
            if ((z > model.highest_z) || (model.highest_z == -99999))
            {
                model.highest_x = x;
                model.highest_y = y;
                model.highest_z = z;
            }
            if ((z < model.lowest_z) || (model.lowest_z == -99999))
            {
                model.lowest_x = x;
                model.lowest_y = y;
                model.lowest_z = z;
            }
        }
        player.vertex++;
        if (cfg.bLambert)
            glColor3f(1.0f, 1.0f, 1.0f);
        if (cfg.bColorLambert && player.team)
            glColor3f(player.fr, player.fg, player.fb);
    }
    pOrig_glVertex3f(x, y, z);
}

void APIENTRY HOOK_glShadeModel(GLenum mode)
{
    GLdouble wx, wz, wy;
    if (cfg.bActivated)
    {
        if ((mode == GL_SMOOTH) && (!model.get))
        {
            model.get = 1;
            model.highest_z = -99999;
            model.lowest_z = -99999;
            model.vertex = 0;
        }
        else
        {
            if (model.get && !player.fg)
            {
                if (model.vertex > 550 && (model.highest_z - model.lowest_z) > 30.0f)
                {
                    glGetDoublev(GL_MODELVIEW_MATRIX, dModel);
                    glGetDoublev(GL_PROJECTION_MATRIX, dProy);
                    glGetIntegerv(GL_VIEWPORT, iView);
                    gluProject(model.highest_x, model.highest_y, model.highest_z, dModel, dProy, iView, &wx, &wy, &wz);
                    if (cfg.bEspBox)
                        DrawBox((wx / (iView[2] / 2)) - 1.0f, (wy / (iView[3] / 2)) - 1.0f, wz - 1.0f);
                    if (cfg.bAimTeam)
                    {
                        bHasTarget = ((abs((int)(iView[2] / 2 - wx)) < (iView[2] / 10)) && (abs((int)(iView[3] / 2 - wy)) < (iView[3] / 10)));
                        if (bKeyAimbot && bHasTarget && (cfg.bAimTeam == 3 || cfg.bAimTeam == player.team))
                        {
                            pt.x = (long)wx; pt.y = (long)(iView[3] - (wy - .01 / (wz - 1)));//(long)(iView[3]-wy);
                            if(ClientToScreen(hdHalfLife,&pt))
                                SetCursorPos(pt.x,pt.y);
                            bShoot = ((iView[2] / 2) == wx && (iView[3] / 2) == wy);
                        }
                        if (!bHasTarget)
                            bShoot = 0;
                    }
                }
            }
        }
    }
    pOrig_glShadeModel(mode);
}


void APIENTRY HOOK_glPushMatrix()
{
    if (cfg.bActivated)
    {
        player.vertex = 0;
        player.team = 0;
    }
    pOrig_glPushMatrix();
}

void APIENTRY HOOK_glPopMatrix()
{
    if (cfg.bActivated && model.get)
        model.get = 0;
    pOrig_glPopMatrix();
}

void APIENTRY HOOK_glEnable(GLenum mode)
{
    if (cfg.bActivated)
    {
        if (mode == GL_TEXTURE_2D)
            bTex = 1;
    }
    pOrig_glEnable(mode);
}

void APIENTRY HOOK_glDisable(GLenum mode)
{
    if (cfg.bActivated && mode == GL_TEXTURE_2D)
        bTex = 0;
    pOrig_glDisable(mode);
}

void APIENTRY HOOK_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    if (cfg.bActivated)
    {
        if (modelviewport)
        {
            model.highest_z = -99999;
            model.lowest_z = -99999;
            modelviewport = 0;
        }
        if (cfg.iNoRecoil)
        {
            coil++;
            if (coil >= 6)
            {
                coil = 0;
                if (bLeftButtonDown)
                {
                    GetCursorPos(&pt);
                    SetCursorPos(pt.x,pt.y+cfg.iNoRecoil);
                }
            }
        }
    }
    pOrig_glViewport(x, y, width, height);
}

void APIENTRY HOOK_glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
    if (cfg.bActivated && cfg.bZoom)
    {
        top = zNear * tan((50 - cfg.iZoom * 5) * M_PI / 360);
        bottom = -top;
        left = bottom * 4 / 3;
        right = top * 4 / 3;
    }
    pOrig_glFrustum(left, right, bottom, top, zNear, zFar);
}

BOOL(WINAPI* pOrig_QueryPerformanceCounter)(LARGE_INTEGER* lpPerformanceCount) = 0;
BOOL WINAPI HOOK_QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount)
{
    LONGLONG newvalue;BOOL ret;
    static LONGLONG oldfakevalue=0;
    static LONGLONG oldrealvalue=0;
    double factor=1.0;
    if(oldfakevalue==0||oldrealvalue==0)
    {
        oldfakevalue=lpPerformanceCount->QuadPart;
        oldrealvalue=lpPerformanceCount->QuadPart;
    }
    ret=pOrig_QueryPerformanceCounter(lpPerformanceCount);
    if(bKeyAimbot&&cfg.iSpeed)
    {
        if(cfg.iSpeed<0)
        {
            if(cfg.iSpeed<-9)
                cfg.iSpeed=-9;
            factor=1.0+(float)cfg.iSpeed/10;
        }
        else
            factor=cfg.iSpeed*3;
    }
    newvalue=lpPerformanceCount->QuadPart;
    newvalue=oldfakevalue+(LONGLONG)((newvalue-oldrealvalue)*factor);
    oldrealvalue=lpPerformanceCount->QuadPart;
    oldfakevalue=newvalue;
    lpPerformanceCount->QuadPart=newvalue;
    return ret;
}

#define CSHACKCREATOR_V2_SIGNATURE /*<(BLOODSHARP_CSHACKCREATOR_V2)>*/XorStr<0x88, 32, 0xC7F83205>("\xB4\xA1\xC8\xC7\xC3\xC2\xCA\xDC\xD8\xD0\xC0\xC3\xCB\xD6\xC5\xDF\xD9\xDA\xD1\xD8\xCE\xD8\xDF\xCB\xEF\xF3\xFD\xF5\x96\x8C\x98" + 0xC7F83205).s

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

void InitializeDllStub(HMODULE hModule)
{
    Json::Value settings;
    Json::Reader reader;
    std::string jsonstring;
    PBYTE pFileBuffer = 0;
    DWORD dwFileSize = 0;
    std::ifstream fsFile;

    szConfigFile[0] = 0;
    GetModuleFileName(hModule, szConfigFile, MAX_PATH - 1);
    if (szConfigFile[0])
    {
        fsFile.open(szConfigFile, std::ifstream::in | std::ifstream::binary);
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
                            }
                            break;
                        }
                    }
                    delete[]pFileBuffer;
                }
            }
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
    HMODULE hOpenGL32 = 0;
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);

        // Default values
        RtlZeroMemory(&cfg, sizeof(cfg));
        cfg.iZoom = -5;

        InitializeDllStub(hModule);

        // Hooking
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

