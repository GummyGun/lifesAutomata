#include <SDL.h>
#include <string.h>
#include <stdlib.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#define DELAY 35
#define TAM 40

#define SCREENX 600
#define SCREENY 500
#define CASILLA 9

typedef struct node{
    struct node *location[3];
    char name;
    bool final;
}automata;

void readFile(char startcase[TAM][TAM]);
bool generateAutomata(automata **value);
void action(automata **value, char element);
void debug(automata *tabletop[TAM][TAM]);
void life(automata *tabletop[2][TAM][TAM], automata *sentinel, char state);
void initialize(automata *tabletop[2][TAM][TAM], char matrix[TAM][TAM], automata *sentinel);
int printTabletop(automata *tabletop[TAM][TAM]);
    
void doRenderMatrix(SDL_Renderer *renderer, automata *tabletop[TAM][TAM]);

int handleEventsMenu(SDL_Window *window){
    SDL_Event event;
    char done = 0;
    printf("hola");
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_WINDOWEVENT_CLOSE:{
                printf("hola\n");
                if(window){
                    SDL_DestroyWindow(window);
                    window=NULL;
                    done=-1;
                }
            }
                break;
            case SDL_KEYDOWN:
                {
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        done=-1;
                        break;
                }
                break;
            }
            case SDL_QUIT:
                printf("hola\n");
                done=-1;
                break;
        }
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_P]){
        done=2;
    }
    return done;
}

int handleEventsGame(SDL_Window *window){
    SDL_Event event;
    char done = 0;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_WINDOWEVENT_CLOSE:{
                printf("hola\n");
                if(window){
                    SDL_DestroyWindow(window);
                    window=NULL;
                    done=-1;
                }
            }
                break;
            case SDL_KEYDOWN:
                {
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        done=-1;
                        printf("hola");
                        break;
                }
                break;
            }
            case SDL_QUIT:
                printf("hola\n");
                done=-1;
                break;
        }
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_P]){
        done=2;
    }
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
                            SCREENX,
                            SCREENY,
                            0
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    char inicial[TAM][TAM]={};
    inicial[1][1]=1;
    inicial[2][2]=1;
    inicial[3][2]=1;
    inicial[3][1]=1;
    inicial[3][0]=1;
    automata *tabletop[2][TAM][TAM];
    automata *base;
    generateAutomata(&base);
    initialize(tabletop, inicial, base);
    
    for(int a=0; a<TAM; a++){
        for(int b=0; b<TAM; b++){
            printf("%d ", tabletop[0][a][b]!=base);
        }
        printf("\n");
    }
    
    char state=1;
    char value;
    
    char done=0;
    char contador=0;
    while(1){
        while(done>=0){
            if(done==2){
                done=0;
                while(!(done==2 || done<0)){
                    SDL_Delay(DELAY);
                    done=handleEventsMenu(window);
                }
                if(done<0){
                    break;
                }
                SDL_Delay(DELAY);
            }
            done=handleEventsGame(window);
            if(contador==6){
                contador=0;
                SDL_RenderClear(renderer);
                life(tabletop, base, state);
                doRenderMatrix(renderer, tabletop[state]);
                SDL_RenderPresent(renderer);
            }else{
                contador++;
            }
            SDL_Delay(DELAY);
            state^=1;
            
        }
        break;
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    

    SDL_Quit();
    return 0;
}

void doRenderMatrix(SDL_Renderer *renderer, automata *tabletop[TAM][TAM]){
    int deltaX=(SCREENX-CASILLA*TAM)/2;
    int deltaY=(SCREENY-CASILLA*TAM)/2;
    SDL_Rect guide={0, 0, CASILLA, CASILLA};
    int lDeltaX=0;
    int lDeltaY=0;
    for(int iterY=0; iterY<TAM; iterY++){
        for(int iterX=0; iterX<TAM; iterX++){
            lDeltaX=deltaX+iterX*CASILLA;
            lDeltaY=deltaY+iterY*CASILLA;
            if(tabletop[iterY][iterX]->final){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 100);
                guide=(SDL_Rect){.x=lDeltaX, .y=lDeltaY, .w=CASILLA, .h=CASILLA};
                SDL_RenderFillRect(renderer, &guide);
            }else{
                SDL_SetRenderDrawColor(renderer, 0, 0xff, 0, 100);
                guide=(SDL_Rect){.x=lDeltaX, .y=lDeltaY, .w=CASILLA, .h=CASILLA};
                SDL_RenderFillRect(renderer, &guide);
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
}

bool generateAutomata(automata **value){
    //intial node
    bool states[]={0,1,0,0,1,1,0,0,0,0,1};
    automata *q[11];
    for(int iter=0; iter<11; iter++){
        q[iter]=(automata*)malloc(sizeof(automata));
        q[iter]->final=states[iter];
        q[iter]->name=iter;
    }
    *value=q[0];
        //q0;
    q[0]->location[0]=q[0];
    q[0]->location[1]=q[1];
    q[0]->location[2]=q[7];
    //q1
    q[1]->location[0]=q[1];
    q[1]->location[1]=q[1];
    q[1]->location[2]=q[2];
    //q2
    q[2]->location[0]=q[2];
    q[2]->location[1]=q[3];
    q[2]->location[2]=q[7];
    //q3
    q[3]->location[0]=q[3];
    q[3]->location[1]=q[4];
    q[3]->location[2]=q[7];
    //q4
    q[4]->location[0]=q[4];
    q[4]->location[1]=q[5];
    q[4]->location[2]=q[2];
    //q5
    q[5]->location[0]=q[5];
    q[5]->location[1]=q[6];
    q[5]->location[2]=q[2];
    //q6
    q[6]->location[0]=q[6];
    q[6]->location[1]=q[6];
    q[6]->location[2]=q[7];
    //q7
    q[7]->location[0]=q[7];
    q[7]->location[1]=q[8];
    q[7]->location[2]=q[7];
    //q8
    q[8]->location[0]=q[8];
    q[8]->location[1]=q[9];
    q[8]->location[2]=q[7];
    //q9
    q[9]->location[0]=q[9];
    q[9]->location[1]=q[10];
    q[9]->location[2]=q[7];
    //q10
    q[10]->location[0]=q[10];
    q[10]->location[1]=q[6];
    q[10]->location[2]=q[2];
}


void action(automata **value, char element){
    (*value)=(*value)->location[element];
}

void debug(automata *tabletop[TAM][TAM]){
    automata *help=tabletop[0][0];
    for(int iterX=0; iterX<TAM; iterX++){
        for(int iterY=0; iterY<TAM; iterY++){
            printf("%d ",tabletop[iterX][iterY]==help);
        }
        printf("\n");
    }
}

void life(automata *tabletop[2][TAM][TAM], automata *sentinel, char state){
    int deltaY[]={1,1,1,0,-1,-1,-1,0};
    int deltaX[]={-1,0,1,1,1,0,-1,-1};
    for(int iterX=0; iterX<TAM; iterX++){
        for(int iterY=0; iterY<TAM; iterY++){
            tabletop[state][iterX][iterY]=tabletop[state^1][iterX][iterY];
            action(&tabletop[state][iterX][iterY], 2);
            int delta=0;
            for(;delta<8; delta++){
                int y=((iterY+deltaY[delta])<0)?TAM-1:(iterY+deltaY[delta]>=TAM)?0:iterY+deltaY[delta];
                int x=((iterX+deltaX[delta])<0)?TAM-1:(iterX+deltaX[delta]>=TAM)?0:iterX+deltaX[delta];
                bool estado=tabletop[state^1][x][y]->final;
                action(&tabletop[state][iterX][iterY], estado);
            }
        }
    }
}

void initialize(automata *tabletop[2][TAM][TAM], char matrix[TAM][TAM], automata *sentinel){
    for(int iterX=0; iterX<TAM; iterX++){
        for(int iterY=0; iterY<TAM; iterY++){
            tabletop[0][iterX][iterY]=sentinel;
            action(&tabletop[0][iterX][iterY], matrix[iterX][iterY]);
            tabletop[1][iterX][iterY]=tabletop[0][iterX][iterY];
        }
    }
}

int printTabletop(automata *tabletop[TAM][TAM]){
    for(int iterX=0; iterX<TAM; iterX++){
        printf("\t\t\t\t");
        for(int iterY=0; iterY<TAM; iterY++){
            printf("%d ",tabletop[iterX][iterY]->final);
        }
        printf("\n");
    }
    return 0;
}


