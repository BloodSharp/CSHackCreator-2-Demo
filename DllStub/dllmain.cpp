#include "DllStub.h"

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

BOOL(WINAPI* pOrig_QueryPerformanceCounter)(LARGE_INTEGER* lpPerformanceCount) = 0;
BOOL(WINAPI* pOrig_wglSwapBuffers)(HDC hDc) = 0;
BOOL(WINAPI* pOrig_SwapBuffers)(HDC hDc) = 0;

WNDPROC pHLWndProc=NULL;
HWND hdHalfLife;

POINT pt; model_s model; player_s player;
char szConfigFile[MAX_PATH];
GLint iView[4]; GLdouble dModel[16], dProy[16]; unsigned int coil = 0;
GLboolean bOnSpeed = 0, bKeyAimbot = 0, bShoot = 0, bHasTarget = 0, bSmoke = 0, bSky = 0, bFlash = 0,
bTex = 0, bDrawn = 0, modelviewport = 0, bLeftButtonDown = 0, bMiddleButtonDown = 0;
cvar cfg;

bool bInterfaceOpen = true;
bool bInitializeHack = false;

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
    pOrig_glVertex2f(x - dist * 2.5f, y + dist * 3.0f);
    pOrig_glVertex2f(x - dist * 2.5f, y + dist * 10.0f);
    pOrig_glVertex2f(x + dist * 2.5f, y + dist * 10.0f);
    pOrig_glVertex2f(x + dist * 2.5f, y + dist * 3.0f);
    pOrig_glVertex2f(x - dist * 2.5f, y + dist * 3.0f);
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

void FillRGBA(GLint x, GLint y, GLint w, GLint h, int r, int g, int b, int a)
{
    pOrig_glPushMatrix();
    glLoadIdentity();

    pOrig_glDisable(GL_TEXTURE_2D);
    pOrig_glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f);
    pOrig_glBegin(GL_QUADS);
    pOrig_glVertex2f(GLfloat(x), GLfloat(y));
    pOrig_glVertex2f(GLfloat(x + w), GLfloat(y));
    pOrig_glVertex2f(GLfloat(x + w), GLfloat(y + h));
    pOrig_glVertex2f(GLfloat(x), GLfloat(y + h));
    glEnd();
    pOrig_glDisable(GL_BLEND);
    pOrig_glEnable(GL_TEXTURE_2D);

    pOrig_glPopMatrix();
}

void DrawCross(int type)
{
    GLint iDim[4];
    glGetIntegerv(GL_VIEWPORT, iDim);
    int xcross = iDim[2] / 2;
    int ycross = iDim[3] / 2;

    int r = 255, g = 255, b = 255, a = 200;
    int R = 255, G = 0, B = 0, A = 200;

    switch (type)
    {
        case 1:
            FillRGBA(xcross - 14, ycross, 9, 1, r, g, b, a);
            FillRGBA(xcross + 5, ycross, 9, 1, r, g, b, a);
            FillRGBA(xcross, ycross - 14, 1, 9, r, g, b, a);
            FillRGBA(xcross, ycross + 5, 1, 9, r, g, b, a);
            FillRGBA(xcross, ycross, 1, 1, R, G, B, A); // center
            break;
        case 2:
            FillRGBA(xcross - 25, ycross, 50, 1, r, g, b, a);
            FillRGBA(xcross, ycross - 25, 1, 50, r, g, b, a);
            FillRGBA(xcross - 5, ycross, 10, 1, R, G, B, A);
            FillRGBA(xcross, ycross - 5, 1, 10, R, G, B, A);
            break;
        case 3:
            FillRGBA(xcross - 25, ycross, 50, 2, r, g, b, a);
            FillRGBA(xcross, ycross - 25, 2, 50, r, g, b, a);
            FillRGBA(xcross - 5, ycross, 10, 2, R, G, B, A);
            FillRGBA(xcross, ycross - 5, 2, 10, R, G, B, A);
            break;
        case 4:
            FillRGBA(xcross - 25, ycross - 25, 50, 1, 255, 255, 255, 180);
            FillRGBA(xcross - 25, ycross + 25, 50, 1, 255, 255, 255, 180);
            FillRGBA(xcross - 25, ycross - 25 + 1, 1, 50 - 1, 255, 255, 255, 180);
            FillRGBA(xcross + 25, ycross - 25, 1, 50 + 1, 255, 255, 255, 180);
            FillRGBA(xcross, ycross - 25 + 1, 1, 50 - 1, 0, 160, 0, 180);
            FillRGBA(xcross - 25 + 1, ycross, 50 - 1, 1, 0, 160, 0, 180);
            break;
        case 5:
            FillRGBA(0, ycross, 2 * xcross, 1, r, g, b, a);
            FillRGBA(xcross, 0, 1, 2 * ycross, r, g, b, a);
            FillRGBA(xcross - 5, ycross, 10, 1, R, G, B, A);
            FillRGBA(xcross, ycross - 5, 1, 10, R, G, B, A);
            break;
        default:
            FillRGBA(xcross - 1, ycross, 3, 1, R, G, B, A);
            FillRGBA(xcross, ycross - 1, 1, 3, R, G, B, A);
            break;
    }
}

void APIENTRY HOOK_glBegin(GLenum mode)
{
    float col[4];
    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH])
    {
        switch (cfg.ComboBoxes[LIBRARY_COMBOBOX_WALLHACK])
        {
            case 1:
                if (mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN)
                    pOrig_glDisable(GL_DEPTH_TEST);
                else if (mode != GL_QUADS && mode != GL_LINES)
                    pOrig_glEnable(GL_DEPTH_TEST);
                break;
            case 2:
                if (mode == GL_POLYGON)
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
        if (mode == GL_POLYGON)
        {
            if (cfg.ComboBoxes[LIBRARY_COMBOBOX_EXTRAWALLS] == 3)
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
            else if (cfg.ComboBoxes[LIBRARY_COMBOBOX_EXTRAWALLS] == 2)
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
            else if (cfg.ComboBoxes[LIBRARY_COMBOBOX_EXTRAWALLS] == 1 && bTex)
                pOrig_glDisable(GL_TEXTURE_2D);
        }
        else if (cfg.ComboBoxes[LIBRARY_COMBOBOX_EXTRAMODELS] == 2)
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

        if (mode == GL_QUADS)
        {
            if (cfg.CheckBoxes[LIBRARY_CHECKBOX_NO_FLASH] || cfg.CheckBoxes[LIBRARY_CHECKBOX_NO_SMOKE])
            {
                glGetFloatv(GL_CURRENT_COLOR, col);
                bSmoke = (col[0] == col[1] && col[0] == col[2] && col[0] != 0.0 && col[0] != 1.0);
                bFlash = (col[0] == 1.0 && col[1] == 1.0 && col[2] == 1.0 && col[3] > 0.2);
            }
            bSky = 1;
        }
        else
            bSky = 0;

        if (cfg.ComboBoxes[LIBRARY_COMBOBOX_WALLHACK] == 3)
        {
            if (mode == GL_POLYGON)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glLineWidth((GLfloat)cfg.Sliders[LIBRARY_SLIDER_WIREFRAMEWALL]);
                glColor3f(1.0f, 1.0f, 1.0f);
            }
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        if (cfg.Sliders[LIBRARY_SLIDER_WIREMODELS])
        {
            if (mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glLineWidth((GLfloat)cfg.Sliders[LIBRARY_SLIDER_WIREMODELS]);
            }
            else if (cfg.ComboBoxes[LIBRARY_COMBOBOX_WALLHACK] != 3)
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    pOrig_glBegin(mode);
}

void APIENTRY HOOK_glClear(GLbitfield mask)
{
    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH] && mask == GL_DEPTH_BUFFER_BIT)
    {
        mask += GL_COLOR_BUFFER_BIT;
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }
    pOrig_glClear(mask);
}

void APIENTRY HOOK_glVertex2f(GLfloat x, GLfloat y)
{
    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH] && cfg.CheckBoxes[LIBRARY_CHECKBOX_NO_FLASH] && bFlash && x == 0.0 && y == 0.0)
        glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    pOrig_glVertex2f(x, y);
}

void APIENTRY HOOK_glVertex3fv(const GLfloat* v)
{
    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH])
    {
        if (cfg.CheckBoxes[LIBRARY_CHECKBOX_NO_SMOKE] && bSmoke)
            return;
        if ((cfg.CheckBoxes[LIBRARY_CHECKBOX_NO_SKY] || cfg.ComboBoxes[LIBRARY_COMBOBOX_WALLHACK] == 2) && bSky)
            return;
        modelviewport = 1;
    }
    pOrig_glVertex3fv(v);
}

void APIENTRY HOOK_glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH])
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
        if (cfg.ComboBoxes[LIBRARY_COMBOBOX_EXTRAMODELS] == 1)
            glColor3f(1.0f, 1.0f, 1.0f);
        if (cfg.ComboBoxes[LIBRARY_COMBOBOX_EXTRAMODELS] == 3 && player.team)
            glColor3f(player.fr, player.fg, player.fb);
    }
    pOrig_glVertex3f(x, y, z);
}

void APIENTRY HOOK_glShadeModel(GLenum mode)
{
    GLdouble wx, wz, wy;
    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH])
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
                    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_ESP_BOX])
                        DrawBox((GLfloat(wx) / (iView[2] / 2)) - 1.0f, (GLfloat(wy) / (iView[3] / 2)) - 1.0f, GLfloat(wz) - 1.0f);
                    if (cfg.ComboBoxes[LIBRARY_COMBOBOX_AIMTEAM])
                    {
                        bHasTarget = ((abs((int)(iView[2] / 2 - wx)) < (iView[2] / 10)) && (abs((int)(iView[3] / 2 - wy)) < (iView[3] / 10)));
                        if (bKeyAimbot && bHasTarget && (cfg.ComboBoxes[LIBRARY_COMBOBOX_AIMTEAM] == 3 || cfg.ComboBoxes[LIBRARY_COMBOBOX_AIMTEAM] == player.team))
                        {
                            pt.x = (long)wx; pt.y = (long)(iView[3] - (wy - .01 / (wz - 1)));//(long)(iView[3]-wy);
                            ClientToScreen(hdHalfLife, &pt);
                            SetCursorPos(pt.x, pt.y);
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
    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH])
    {
        player.vertex = 0;
        player.team = 0;
    }
    pOrig_glPushMatrix();
}

void APIENTRY HOOK_glPopMatrix()
{
    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH] && model.get)
        model.get = 0;
    pOrig_glPopMatrix();
}

void APIENTRY HOOK_glEnable(GLenum mode)
{
    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH])
    {
        if (mode == GL_TEXTURE_2D)
            bTex = 1;
        //GUI
        if (bDrawn)
        {
            if (cfg.Sliders[LIBRARY_SLIDER_CROSSHAIR])
                DrawCross(cfg.Sliders[LIBRARY_SLIDER_CROSSHAIR]);
            bDrawn = FALSE;
        }
    }
    pOrig_glEnable(mode);
}

void APIENTRY HOOK_glDisable(GLenum mode)
{
    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH] && mode == GL_TEXTURE_2D)
        bTex = 0;
    pOrig_glDisable(mode);
}

void APIENTRY HOOK_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    bDrawn = TRUE;//GUI
    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH])
    {
        if (modelviewport)
        {
            model.highest_z = -99999;
            model.lowest_z = -99999;
            modelviewport = 0;
        }
        if (cfg.Sliders[LIBRARY_SLIDER_NORECOIL])
        {
            coil++;
            if (coil >= 6)
            {
                coil = 0;
                if (bLeftButtonDown)
                {
                    GetCursorPos(&pt);
                    SetCursorPos(pt.x, pt.y + cfg.Sliders[LIBRARY_SLIDER_NORECOIL]);
                }
            }
        }
    }
    pOrig_glViewport(x, y, width, height);
}

void APIENTRY HOOK_glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH] && cfg.CheckBoxes[LIBRARY_CHECKBOX_ZOOM])
    {
        top = zNear * tan((50 - cfg.iZoom * 5) * M_PI / 360);
        bottom = -top;
        left = bottom * 4 / 3;
        right = top * 4 / 3;
    }
    pOrig_glFrustum(left, right, bottom, top, zNear, zFar);
}

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
    if (bMiddleButtonDown && cfg.Sliders[LIBRARY_SLIDER_SPEEDHACK])
    {
        if (cfg.Sliders[LIBRARY_SLIDER_SPEEDHACK] < 0)
        {
            if (cfg.Sliders[LIBRARY_SLIDER_SPEEDHACK] < -9)
                cfg.Sliders[LIBRARY_SLIDER_SPEEDHACK] = -9;
            factor = 1.0 + (float)cfg.Sliders[LIBRARY_SLIDER_SPEEDHACK] / 10;
        }
        else
            factor = cfg.Sliders[LIBRARY_SLIDER_SPEEDHACK] * 3;
    }
    newvalue=lpPerformanceCount->QuadPart;
    newvalue=oldfakevalue+(LONGLONG)((newvalue-oldrealvalue)*factor);
    oldrealvalue=lpPerformanceCount->QuadPart;
    oldfakevalue=newvalue;
    lpPerformanceCount->QuadPart=newvalue;
    return ret;
}

#define CSHACKCREATOR_V2_SIGNATURE /*<(BLOODSHARP_CSHACKCREATOR_V2)>*/XorStr<0x88, 32, 0xC7F83205>("\xB4\xA1\xC8\xC7\xC3\xC2\xCA\xDC\xD8\xD0\xC0\xC3\xCB\xD6\xC5\xDF\xD9\xDA\xD1\xD8\xCE\xD8\xDF\xCB\xEF\xF3\xFD\xF5\x96\x8C\x98" + 0xC7F83205).s

bool IsThisAddressContainString(PBYTE dwAddress, PBYTE string)
{
    for (UINT i = 0; i < strlen((char*)string) + 1; i++)
        if (string[i] != dwAddress[i])
            return false;
    return true;
}

void InitializeDllStub(HMODULE hModule)
{
    char szModulePath[MAX_PATH];
    Json::Value settings;
    Json::Reader reader;
    std::string jsonstring;
    PBYTE pFileBuffer = 0;
    DWORD dwFileSize = 0;
    std::ifstream fsFile;

    szModulePath[0] = 0;
    GetModuleFileName(hModule, szModulePath, MAX_PATH - 1);
    if (szModulePath[0])
    {
        fsFile.open(szModulePath, std::ifstream::in | std::ifstream::binary);
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
                                char* pos = szModulePath + strlen(szModulePath);
                                while (pos >= szModulePath && *pos != '\\')
                                    --pos;
                                pos[1] = 0;

                                GetVal(settings["Files"]["Configurations"], szConfigFile, MAX_PATH - 1);
                                strcat_s(szModulePath, MAX_PATH - 1, szConfigFile);
                                strcpy_s(szConfigFile, MAX_PATH - 1, szModulePath);
                                RtlZeroMemory(szModulePath, MAX_PATH);

                                GetVal(settings["Library"]["MasterSwitch"], &cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH]);
                                GetVal(settings["Library"]["NoSmoke"], &cfg.CheckBoxes[LIBRARY_CHECKBOX_NO_SMOKE]);
                                GetVal(settings["Library"]["NoFlash"], &cfg.CheckBoxes[LIBRARY_CHECKBOX_NO_FLASH]);
                                GetVal(settings["Library"]["NoSky"], &cfg.CheckBoxes[LIBRARY_CHECKBOX_NO_SKY]);
                                GetVal(settings["Library"]["EspBox"], &cfg.CheckBoxes[LIBRARY_CHECKBOX_ESP_BOX]);
                                GetVal(settings["Library"]["BunnyHop"], &cfg.CheckBoxes[LIBRARY_CHECKBOX_BUNNY_HOP]);
                                GetVal(settings["Library"]["Zoom"], &cfg.CheckBoxes[LIBRARY_CHECKBOX_ZOOM]);

                                GetVal(settings["Library"]["WallHack"], &cfg.ComboBoxes[LIBRARY_COMBOBOX_WALLHACK]);
                                GetVal(settings["Library"]["ExtraWalls"], &cfg.ComboBoxes[LIBRARY_COMBOBOX_EXTRAWALLS]);
                                GetVal(settings["Library"]["ExtraModels"], &cfg.ComboBoxes[LIBRARY_COMBOBOX_EXTRAMODELS]);
                                GetVal(settings["Library"]["AimTeam"], &cfg.ComboBoxes[LIBRARY_COMBOBOX_AIMTEAM]);

                                GetVal(settings["Library"]["WireFrameWall"], &cfg.Sliders[LIBRARY_SLIDER_WIREFRAMEWALL]);
                                GetVal(settings["Library"]["WireModels"], &cfg.Sliders[LIBRARY_SLIDER_WIREMODELS]);
                                GetVal(settings["Library"]["CrossHair"], &cfg.Sliders[LIBRARY_SLIDER_CROSSHAIR]);
                                GetVal(settings["Library"]["Recoil"], &cfg.Sliders[LIBRARY_SLIDER_NORECOIL]);
                                GetVal(settings["Library"]["Speed"], &cfg.Sliders[LIBRARY_SLIDER_SPEEDHACK]);

                                LoadConfig();

                                DllStub::Interface::LoadInterpreter(settings);
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

LRESULT CALLBACK HOOK_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (bInitializeHack)
    {
        if (uMsg == WM_KEYDOWN && wParam == VK_INSERT)
        {
            bInterfaceOpen = !bInterfaceOpen;
            if (!bInterfaceOpen)
            {
                SaveConfig();
            }
            return TRUE;
        }
        if (bInterfaceOpen)
        {
            if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
            {
                if ((uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP) && (wParam < 256))
                    return CallWindowProc(pHLWndProc, hWnd, uMsg, wParam, lParam);
                return TRUE;
            }
        }
        else
        {
            if (uMsg == WM_KEYDOWN && wParam == VK_SPACE)
            {
                if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH] && cfg.CheckBoxes[LIBRARY_CHECKBOX_BUNNY_HOP])
                    keybd_event(VK_SPACE, MapVirtualKey(VK_SPACE, 0), KEYEVENTF_KEYUP, 0);
            }
            if (uMsg == WM_LBUTTONDOWN)
            {
                bKeyAimbot = TRUE;
                bLeftButtonDown = TRUE;
            }
            if (uMsg == WM_LBUTTONUP)
            {
                bKeyAimbot = FALSE;
                bLeftButtonDown = FALSE;
            }
            if (uMsg == WM_MBUTTONDOWN)
                bMiddleButtonDown = TRUE;
            if (uMsg == WM_MBUTTONUP)
                bMiddleButtonDown = FALSE;
            if (uMsg == WM_MOUSEWHEEL)
            {
                if (cfg.CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH])
                {
                    if (cfg.CheckBoxes[LIBRARY_CHECKBOX_ZOOM])
                    {
                        SHORT zDelta = HIWORD(wParam);
                        if (zDelta > 0)
                        {
                            cfg.iZoom += 1;
                            if (cfg.iZoom > 9)
                                cfg.iZoom = 9;
                        }
                        if (zDelta < 0)
                        {
                            cfg.iZoom -= 1;
                            if (cfg.iZoom < -9)
                                cfg.iZoom = -9;
                        }
                    }
                }
            }
        }
    }
    return CallWindowProc(pHLWndProc, hWnd, uMsg, wParam, lParam);
}

HGLRC hRealContext, hNewContext;
void WINAPI GenericSwapBuffers(HDC hDC)
{
    if (!bInitializeHack)
    {
        hdHalfLife = WindowFromDC(hDC);
        if (hdHalfLife)
        {
            pHLWndProc = (WNDPROC)SetWindowLong(hdHalfLife, GWL_WNDPROC, (LONG)HOOK_WndProc);
            ImGui::CreateContext();
            DllStub::Interface::Initialize();
            ImGui_ImplWin32_Init(hdHalfLife);
            ImGui_ImplOpenGL2_Init();
            hRealContext = wglGetCurrentContext();
            hNewContext = wglCreateContext(hDC);
            bInitializeHack = true;
        }
    }
    wglMakeCurrent(hDC, hNewContext);
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::GetIO().DeltaTime = 0;

    ImGui::GetIO().MouseDrawCursor = bInterfaceOpen;

    DllStub::Interface::Background();
    if(bInterfaceOpen)
        DllStub::Interface::Interface();

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    wglMakeCurrent(hDC, hRealContext);
}

BOOL WINAPI HOOK_wglSwapBuffers(HDC hDc)
{
    GenericSwapBuffers(hDc);
    return pOrig_wglSwapBuffers(hDc);
}

BOOL WINAPI HOOK_SwapBuffers(HDC hDc)
{
    GenericSwapBuffers(hDc);
    return pOrig_SwapBuffers(hDc);
}

DWORD WINAPI InitializeHooks(LPVOID lpThreadParameter)
{
    // Hooking
    HMODULE hOpenGL32 = LoadLibrary(/*opengl32.dll*/XorStr<0xE1, 13, 0x51665709>("\x8E\x92\x86\x8A\x82\x8A\xD4\xDA\xC7\x8E\x87\x80" + 0x51665709).s);
    pOrig_glBegin = (decltype(pOrig_glBegin))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, /*glBegin*/XorStr<0xE1, 8, 0xC6BB1BFB>("\x86\x8E\xA1\x81\x82\x8F\x89" + 0xC6BB1BFB).s)), (PBYTE)HOOK_glBegin);
    pOrig_glClear = (decltype(pOrig_glClear))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, /*glClear*/XorStr<0x97, 8, 0xEEB49253>("\xF0\xF4\xDA\xF6\xFE\xFD\xEF" + 0xEEB49253).s)), (PBYTE)HOOK_glClear);
    pOrig_glVertex2f = (decltype(pOrig_glVertex2f))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, /*glVertex2f*/XorStr<0x14, 11, 0x6AA1AFCE>("\x73\x79\x40\x72\x6A\x6D\x7F\x63\x2E\x7B" + 0x6AA1AFCE).s)), (PBYTE)HOOK_glVertex2f);
    pOrig_glVertex3fv = (decltype(pOrig_glVertex3fv))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, /*glVertex3fv*/XorStr<0x92, 12, 0xA51BAC39>("\xF5\xFF\xC2\xF0\xE4\xE3\xFD\xE1\xA9\xFD\xEA" + 0xA51BAC39).s)), (PBYTE)HOOK_glVertex3fv);
    pOrig_glVertex3f = (decltype(pOrig_glVertex3f))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, /*glVertex3f*/XorStr<0x62, 11, 0xEFF22D3E>("\x05\x0F\x32\x00\x14\x13\x0D\x11\x59\x0D" + 0xEFF22D3E).s)), (PBYTE)HOOK_glVertex3f);
    pOrig_glShadeModel = (decltype(pOrig_glShadeModel))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, /*glShadeModel*/XorStr<0x0F, 13, 0xF3602B6F>("\x68\x7C\x42\x7A\x72\x70\x70\x5B\x78\x7C\x7C\x76" + 0xF3602B6F).s)), (PBYTE)HOOK_glShadeModel);
    pOrig_glPushMatrix = (decltype(pOrig_glPushMatrix))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, /*glPushMatrix*/XorStr<0x18, 13, 0x2A1E4734>("\x7F\x75\x4A\x6E\x6F\x75\x53\x7E\x54\x53\x4B\x5B" + 0x2A1E4734).s)), (PBYTE)HOOK_glPushMatrix);
    pOrig_glPopMatrix = (decltype(pOrig_glPopMatrix))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, /*glPopMatrix*/XorStr<0x52, 12, 0xAE4B0C48>("\x35\x3F\x04\x3A\x26\x1A\x39\x2D\x28\x32\x24" + 0xAE4B0C48).s)), (PBYTE)HOOK_glPopMatrix);
    pOrig_glEnable = (decltype(pOrig_glEnable))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, /*glEnable*/XorStr<0x36, 9, 0x05912C33>("\x51\x5B\x7D\x57\x5B\x59\x50\x58" + 0x05912C33).s)), (PBYTE)HOOK_glEnable);
    pOrig_glDisable = (decltype(pOrig_glDisable))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, /*glDisable*/XorStr<0xBB, 10, 0xD270B7E9>("\xDC\xD0\xF9\xD7\xCC\xA1\xA3\xAE\xA6" + 0xD270B7E9).s)), (PBYTE)HOOK_glDisable);
    pOrig_glViewport = (decltype(pOrig_glViewport))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, /*glViewport*/XorStr<0xB2, 11, 0x18834223>("\xD5\xDF\xE2\xDC\xD3\xC0\xC8\xD6\xC8\xCF" + 0x18834223).s)), (PBYTE)HOOK_glViewport);
    pOrig_glFrustum = (decltype(pOrig_glFrustum))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, /*glFrustum*/XorStr<0x7A, 10, 0x96CFA989>("\x1D\x17\x3A\x0F\x0B\x0C\xF4\xF4\xEF" + 0x96CFA989).s)), (PBYTE)HOOK_glFrustum);

    pOrig_wglSwapBuffers= (decltype(pOrig_wglSwapBuffers))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(hOpenGL32, /*wglSwapBuffers*/XorStr<0x67, 15, 0xE8F23608>("\x10\x0F\x05\x39\x1C\x0D\x1D\x2C\x1A\x16\x17\x17\x01\x07" + 0xE8F23608).s)), (PBYTE)HOOK_wglSwapBuffers);
    pOrig_SwapBuffers = (decltype(pOrig_SwapBuffers))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(LoadLibrary(/*gdi32.dll*/XorStr<0x00, 10, 0x4A65018E>("\x67\x65\x6B\x30\x36\x2B\x62\x6B\x64" + 0x4A65018E).s), /*SwapBuffers*/XorStr<0x55, 12, 0xC2A6AC37>("\x06\x21\x36\x28\x1B\x2F\x3D\x3A\x38\x2C\x2C" + 0xC2A6AC37).s)), (PBYTE)HOOK_SwapBuffers);

    pOrig_QueryPerformanceCounter = (decltype(pOrig_QueryPerformanceCounter))CH4::Utils::DetourFunction(PBYTE(GetProcAddress(LoadLibrary(/*kernel32.dll*/XorStr<0xD7, 13, 0xF2EA8E87>("\xBC\xBD\xAB\xB4\xBE\xB0\xEE\xEC\xF1\x84\x8D\x8E" + 0xF2EA8E87).s), /*QueryPerformanceCounter*/XorStr<0x26, 24, 0x3FB621D7>("\x77\x52\x4D\x5B\x53\x7B\x49\x5F\x48\x40\x42\x5C\x53\x5D\x57\x50\x75\x58\x4D\x57\x4E\x5E\x4E" + 0x3FB621D7).s)), (PBYTE)HOOK_QueryPerformanceCounter);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);

        // Default values
        RtlZeroMemory(&cfg, sizeof(cfg));
        cfg.iZoom = -5;

        InitializeDllStub(hModule);

        CreateThread(0, 0, InitializeHooks, 0, 0, 0);
    }
    return TRUE;
}

