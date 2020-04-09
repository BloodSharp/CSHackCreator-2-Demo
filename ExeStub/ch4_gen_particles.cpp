#include "ch4_gen_particles.h"
#include <algorithm>
#include <vector>
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "util_vector.h"

#define _USE_MATH_DEFINES
#include <cmath>

class Dot
{
public:
	Dot(Vector2D p,Vector2D v);
	void Draw(int index);

	Vector2D RelativePosition=Vector2D(0,0);
	Vector2D Velocity;
};

std::vector<Dot*>Dots;

Dot::Dot(Vector2D p,Vector2D v)
{
    RelativePosition=p;
	Velocity=v;
}

void Dot::Draw(int index)
{
    auto t=std::find(Dots.begin(),Dots.end(),this);
    if(t==Dots.end())
        return;
    for(auto i=t;i!=Dots.end();i++)
    {
        if((*i)==this)
            continue;
        Vector2D Pos=RelativePosition;
        //float Dist=Pos.DistTo((*i)->RelativePosition);
        float Dist=(Pos-((*i)->RelativePosition)).Length();
        if(Dist<128)
        {
            Vector2D Dir=((*i)->RelativePosition-RelativePosition).Normalize();
			//	g_pSurface->DrawSetColor(Color(69, 140, 230, 255));
			//	g_pSurface->Line(Pos.x, Pos.y, (Pos + Dir * Dist).x, (Pos + Dir * Dist).y, Color(255, 111, 15, 255));
            ImGui::GetCurrentWindow()->DrawList->AddLine({Pos.x,Pos.y},{(Pos+Dir*Dist).x,(Pos+Dir*Dist).y},
                                                         ImColor(0,128,255),//CInterface::clWindowsColors[CH4_WINDOW_COLOR_TITLE_BACKGROUND],
                                                         2.0f);
        }
    }
    /*
    ImGui::GetCurrentWindow()->DrawList->AddRectFilled(
			{ RelativePosition.x - 3, RelativePosition.y - 3 },
			{ RelativePosition.x + 3 , RelativePosition.y + 3 },
			CInterface::clWindowsColors[CH4_WINDOW_COLOR_TEXT_CONTENT],
			0.0f);
    */
    ImGui::GetCurrentWindow()->DrawList->AddCircleFilled({RelativePosition.x,RelativePosition.y},4+index/3,ImColor(255,255,255),36);
    /*ImGui::GetCurrentWindow()->DrawList->AddTriangleFilled({RelativePosition.x-4,RelativePosition.y+2},
                                                           {RelativePosition.x+4,RelativePosition.y+2},
                                                           {RelativePosition.x,RelativePosition.y-4},CInterface::clWindowsColors[CH4_WINDOW_COLOR_TEXT_CONTENT]);*/
}

void CH4::Interface::Particles::Render(int iMaxParticles)
{
    /*static Uint32 uiLastTick=0;
    ImGui::Text("SDL_GetTicks: %i",SDL_GetTicks());
    ImGui::Text("Difference Last: %i",(SDL_GetTicks()-uiLastTick)*1000);
    ImGui::Text("Ticks to 60 fps: %.0f",((1.0f/60.0f)*1000));
    if(((SDL_GetTicks()-uiLastTick))<((1.0/60.0f)*1000))
    {
        for (auto i = Dots.begin(); i < Dots.end(); i++)
            (*i)->Draw();
        return;
    }
    uiLastTick=SDL_GetTicks();*/

    int s=rand()%48;

    if(Dots.size()<iMaxParticles)
    {
        if(s==0)
            Dots.push_back(new Dot(Vector2D(rand()%(int)ImGui::GetIO().DisplaySize.x,-19),Vector2D((rand()%7)-3,rand()%3+1)));
        else if(s==1)
            Dots.push_back(new Dot(Vector2D(rand()%(int)ImGui::GetIO().DisplaySize.x,(int)ImGui::GetIO().DisplaySize.y+19),Vector2D((rand()%7)-3,-1*(rand()%3+1))));
    }
    for(auto i=Dots.begin();i<Dots.end();)
    {
		if((*i)->RelativePosition.y<-20||
        (*i)->RelativePosition.y>ImGui::GetIO().DisplaySize.y+20||
        (*i)->RelativePosition.x<-20||(*i)->RelativePosition.x>ImGui::GetIO().DisplaySize.x+20)
		{
            delete (*i);
			i=Dots.erase(i);
		}
		else
        {
            (*i)->RelativePosition=(*i)->RelativePosition+(*i)->Velocity*(0.3);
            i++;
        }
    }
    for(unsigned int i=0;i<Dots.size();i++)
        Dots[i]->Draw(i);
}
