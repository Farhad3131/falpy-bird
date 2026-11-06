#include <SDL.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

constexpr int W=480, H=640;
struct Pipe { float x; float gapY; bool scored=false; };
SDL_Window* window=nullptr; SDL_Renderer* renderer=nullptr;
float birdY=H*0.45f, velocity=0, gravity=0.38f, flap=-6.8f;
int score=0, best=0;
bool started=false, gameOver=false;
std::vector<Pipe> pipes;

void rect(int x,int y,int w,int h, SDL_Color c){
    SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a);
    SDL_Rect r{x,y,w,h}; SDL_RenderFillRect(renderer,&r);
}
void circle(int cx,int cy,int radius, SDL_Color c){
    SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a);
    for(int dy=-radius;dy<=radius;dy++){
        int dx=(int)std::sqrt((double)radius*radius-dy*dy);
        SDL_RenderDrawLine(renderer,cx-dx,cy+dy,cx+dx,cy+dy);
    }
}
void text(const std::string& s,int x,int y,int scale,SDL_Color c){
    // Tiny built-in 3x5 font; avoids external font dependencies.
    const char* glyphs[10]={"111101101101111","010110010010111","111001111100111","111001111001111","101101111001001","111100111001111","111100111101111","111001001001001","111101111101111","111101111001111"};
    for(char ch:s){
        if(ch>='0'&&ch<='9'){
            const char* g=glyphs[ch-'0'];
            for(int i=0;i<15;i++) if(g[i]=='1') rect(x+(i%3)*scale,y+(i/3)*scale,scale,scale,c);
            x+=4*scale;
        } else x+=3*scale;
    }
}
void reset(){
    birdY=H*0.45f; velocity=0; score=0; started=false; gameOver=false;
    pipes.clear(); pipes.push_back({W+80, 250+(std::rand()%180),false});
}
void flapBird(){
    if(gameOver){reset(); return;}
    started=true; velocity=flap;
}
void addPipe(){ pipes.push_back({W+20, 190+(std::rand()%250),false}); }
void draw(){
    SDL_SetRenderDrawColor(renderer,110,195,245,255); SDL_RenderClear(renderer);
    // distant clouds
    circle(90,105,25,{245,250,255,255}); circle(117,105,19,{245,250,255,255}); circle(66,111,16,{245,250,255,255});
    circle(340,160,20,{245,250,255,255}); circle(365,160,26,{245,250,255,255}); circle(389,164,16,{245,250,255,255});
    for(auto &p:pipes){
        int top=(int)(p.gapY-105), bottom=(int)(p.gapY+105);
        rect((int)p.x,0,64,top,{55,178,75,255}); rect((int)p.x-6,top-22,76,22,{38,145,57,255});
        rect((int)p.x,bottom,64,H-bottom-55,{55,178,75,255}); rect((int)p.x-6,bottom,76,22,{38,145,57,255});
        rect((int)p.x+9,0,8,top-22,{105,215,112,255});
    }
    rect(0,H-55,W,55,{222,190,112,255});
    rect(0,H-55,W,8,{95,190,75,255});
    // bird
    circle(130,(int)birdY,17,{255,220,45,255});
    circle(137,(int)birdY-6,5,{255,255,255,255});
    circle(139,(int)birdY-6,2,{25,35,45,255});
    rect(143,(int)birdY-1,15,7,{245,115,35,255});
    rect(117,(int)birdY+4,14,7,{235,165,30,255});
    if(started && !gameOver) text(std::to_string(score),W/2-10,35,6,{255,255,255,255});
    if(!started && !gameOver){
        text("1",W/2-10,220,8,{255,255,255,255});
        text("2",W/2-10,280,5,{255,255,255,255});
    }
    if(gameOver){
        rect(78,220,324,150,{25,45,65,220});
        text("9",W/2-8,245,9,{255,220,70,255});
        text(std::to_string(score),W/2-8,295,7,{255,255,255,255});
        text(std::to_string(best),W/2-8,330,4,{200,225,245,255});
    }
    SDL_RenderPresent(renderer);
}
int main(int argc,char**argv){
    std::srand((unsigned)std::time(nullptr));
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS)!=0) return 1;
    window=SDL_CreateWindow("Flappy Bird",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,W,H,SDL_WINDOW_SHOWN);
    renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(!renderer) renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    reset(); bool quit=false; Uint32 last=SDL_GetTicks();
    while(!quit){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT) quit=true;
            if(e.type==SDL_KEYDOWN && (e.key.keysym.sym== SDLK_SPACE || e.key.keysym.sym== SDLK_UP)) flapBird();
            if(e.type==SDL_MOUSEBUTTONDOWN || (e.type==SDL_FINGERDOWN)) flapBird();
        }
        Uint32 now=SDL_GetTicks(); float dt=std::min(2.0f,(now-last)/16.666f); last=now;
        if(started && !gameOver){
            velocity+=gravity*dt; birdY+=velocity*dt;
            for(auto &p:pipes){
                p.x-=2.7f*dt;
                if(!p.scored && p.x+64<130){p.scored=true;score++;best=std::max(best,score);}
                if(130+17>p.x && 130-17<p.x+64 && (birdY-17<p.gapY-105 || birdY+17>p.gapY+105)) gameOver=true;
            }
            if(!pipes.empty() && pipes.back().x<W-190) addPipe();
            while(!pipes.empty() && pipes.front().x<-90) pipes.erase(pipes.begin());
            if(birdY>H-72 || birdY<0) gameOver=true;
        }
        draw(); SDL_Delay(16);
    }
    SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); SDL_Quit(); return 0;
}
