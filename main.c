#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#define DELAY 35
#define TAM 20

typedef struct node{
    struct node *location[3];
    char name;
    bool final;
}automata;

void doRenderMatrix(SDL_Renderer *renderer/*, automata *tabletop[TAM][TAM]*/);

int handleEventsGame(SDL_Window *window){
    SDL_Event event;
    bool done = 0;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_WINDOWEVENT_CLOSE:{
                if(window){
                    SDL_DestroyWindow(window);
                    window=NULL;
                    done=1;
                }
            }
                break;
            case SDL_KEYDOWN:
                {
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        done=1;
                        break;
                }
                break;
            }
            case SDL_QUIT:
                done=1;
                break;
        }
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    /*
    if(state[SDL_SCANCODE_UP]){
        tanque1->dirTanque='u';
    }else if(state[SDL_SCANCODE_RIGHT]){
        tanque1->dirTanque='r';
    }else if(state[SDL_SCANCODE_DOWN]){
        tanque1->dirTanque='d';
    }else if(state[SDL_SCANCODE_LEFT]){
        tanque1->dirTanque='l';
    }
    if(state[SDL_SCANCODE_O]){
        tanque1->disparando=1;
    }
    if(state[SDL_SCANCODE_P]){
        printf("%d",configuraciones.players);
        doPrintMatrix(&bloques);
    }
    */
    return done;
}



int main(int argc, char* argv[]) {
    printf("El Programa se encuentra corriendo\n");
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);
    window=SDL_CreateWindow("BattleTank",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            120,
                            120,
                            0
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool done=0;
    while(1){
        while(!done){
            done=handleEventsGame(window);
//            doRenderMatrix();
            SDL_RenderPresent(renderer);
            SDL_Delay(DELAY);
            doRenderMatrix(renderer);
            SDL_RenderPresent(renderer);
            SDL_RenderClear(renderer);
        }
        break;
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);


    SDL_Quit();
    return 0;
}

void doRenderMatrix(SDL_Renderer *renderer/*, automata *tabletop[TAM][TAM]*/){
    SDL_Rect guide;
    guide.x=12;
    guide.y=12;
    guide.w=12;
    guide.h=12;
    SDL_RenderDrawRect(renderer, &guide);
}

