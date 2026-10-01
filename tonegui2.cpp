
#include <raylib.h>
#include <stdio.h>
#include <lua.hpp>
#include <string>
#include<iostream>

typedef void* HMODULE;

extern "C" HMODULE LoadLibraryA(const char* lpLibFileName);
extern "C" void* GetProcAddress(HMODULE hModule, const char* lpProcName);
extern "C" int FreeLibrary(HMODULE hLibModule);











//==========================================
class button {

    public:
    int width = 200;
    int height = 200;
    char name[3]; 
    int x; //coordinates
    int y;
    int id;
 
    
  


 


  
  //constructor
  button(const char *iname, int ix, int iy, int iid){
    snprintf(name, sizeof(name), "%s", iname);
    x = ix;
    y = iy;
    id = iid;
    
  }




bool checkcollision(button b, int mx, int my){ //this will need the mx and my one scope level above it
    if (mx >= b.x &&
        mx <= b.x + b.width &&
        my >= b.y &&
        my <= b.y + b.height)
    {
        return true;
    }
    else{
        return false;
    }
}

};


//==================================================================================

//prototyping the functon that finds which button was pressed
 int touchfind(button b){

   int mx = GetMouseX();
   int my = GetMouseY();
    if(b.checkcollision(b, mx, my) == true){
        return b.id;
    }
    else{
        return 0;
    }
}

//prototype of the function that draws a button to the screen
void drawb(button b, Color c){
    DrawRectangle(b.x, b.y, b.width, b.height, c);
    DrawText(b.name, (b.x + 100), (b.y + 100), 14, RAYWHITE);
}

//this is for the following function
typedef void (*PlayToneFunc)(int);

PlayToneFunc play_tone = nullptr;


//does the lua tone thing
int l_play_tone(lua_State* L)
{
    int tone = (int)luaL_checkinteger(L, 1);

    play_tone(tone);

    return 0;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


int main(){

//=====================================================
 HMODULE audioDLL = LoadLibraryA("audio.dll");

if (audioDLL == nullptr)
{
    printf("Could not load audio.dll\n");
    return 1;
}
play_tone = (PlayToneFunc)GetProcAddress(audioDLL, "play_tone");

if (play_tone == nullptr)
{
    printf("Could not find play_tone in audio.dll\n");
    FreeLibrary(audioDLL);
    return 1;
}
lua_State *L = luaL_newstate();

    if (L == nullptr){
        return 1;
    }

    luaL_openlibs(L);
    lua_newtable(L);

lua_pushcfunction(L, l_play_tone);
lua_setfield(L, -2, "play_tone");

lua_setglobal(L, "audio");

    if (luaL_dofile(L, "tone.lua") != LUA_OK)
    {
        printf("Lua error: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }
//================================================================





InitWindow(1000, 500, "TONE");
Camera2D camera = { 0 };
BeginMode2D(camera);
SetTargetFPS(15);


//==================================================================

//creating their existence
button D4("D4", 1, 1, 1); //has an id of 1
button F4("F4", 201, 1, 2); //has id of 2
button A4("A4", 401, 1, 3); //y is 1, because it needs to be at the top of the screen
button B4("B4", 601, 1, 4);
button D5("D5", 801, 1, 5);

//==============================================================


while(!WindowShouldClose()){
BeginDrawing();
ClearBackground(BLACK);


//==============================================================================
drawb(D4, RED);
drawb(F4, YELLOW);
drawb(A4, GREEN);
drawb(B4, BLUE);
drawb(D5, PURPLE);
int touch = 0;

DrawText(" welcome to the OoT & MM tone program. CPH 2026 ", 200, 300, 14, RAYWHITE); 



//===============================================================================


if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){ //this routine identifies which button was pressed. 
touch = touchfind(D4);
if(touch == 0){
    touch = touchfind(F4);
    if(touch == 0){
        touch = touchfind(A4);
        if(touch == 0){
            touch = touchfind(B4);
            if(touch == 0){
                touch = touchfind(D5);
            }
        }
    }
}}
//===========================================================
   if(touch != 0){ //this will send the input to lua, which will send it to the C program
    lua_getglobal(L, "button_pressed");
    lua_pushinteger(L, touch);

    if (lua_pcall(L, 1, 0, 0) != LUA_OK)
    {
        printf("Lua error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
        touch = 0;
   }


  
   
    EndDrawing();
} //end of the while loop
//========================================================











CloseWindow();

    lua_close(L);
    return 0;
}