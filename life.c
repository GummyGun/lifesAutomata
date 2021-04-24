#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

//#define printhelp
//#define debug
//#define time
#define TAM 20

typedef struct node{
    struct node *location[3];
    char name;
    bool final;
}automata;

void readFile(char startcase[TAM][TAM]){
    int fd;
    struct stat file;
    if((fd=open("./startcase.txt", O_RDONLY, 00777))<0){
        perror("failed to open file\n");
    }
    if(fstat(fd,&file)==-1){
        perror("failed getting size\n");
    }
    void *rawDat=mmap(NULL, file.st_size, PROT_READ, MAP_SHARED, fd, 0);
    char *raw=(char*)rawDat;
    
    printf("%p\n",rawDat);
    
    for(int iterX=0; iterX<TAM; iterX++){
        for(int iterY=0; iterY<TAM; iterY++){
            startcase[iterX][iterY]=raw[21*iterX+iterY];
        }
    }
    munmap(raw, file.st_size);
    close(fd);
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
    debug(tabletop[state]);
    for(int iterX=0; iterX<TAM; iterX++){
        for(int iterY=0; iterY<TAM; iterY++){
            tabletop[state][iterX][iterY]=tabletop[state^1][iterX][iterY];
            action(&tabletop[state][iterX][iterY], 2);
//            printf("%d %d ", iterX, iterY);
            for(int delta=0; delta<8; delta++){
                int y=((iterY+deltaY[delta])<0)?TAM-1:(iterY+deltaY[delta]>=TAM)?0:iterY+deltaY[delta];
                int x=((iterX+deltaX[delta])<0)?TAM-1:(iterX+deltaX[delta]>=TAM)?0:iterX+deltaX[delta];
//                printf("X:%d Y: %d ", x, y);
                bool estado=tabletop[state^1][x][y]->final;
            #ifdef debug
                printf("%d", estado);
            #endif
                action(&tabletop[state][iterX][iterY], estado);
            }
            printf("\n");
        }
    }
}

void initialize(automata *tabletop[2][TAM][TAM], char matrix[TAM][TAM], automata *sentinel){
    for(int iterX=0; iterX<TAM; iterX++){
        for(int iterY=0; iterY<TAM; iterY++){
            tabletop[0][iterX][iterY]=sentinel;
            action(&tabletop[0][iterX][iterY], matrix[iterX][iterY]-'0');
            tabletop[1][iterX][iterY]=tabletop[0][iterX][iterY];
//            printf("%d %p %p\n", matrix[iterX][iterY]-'0', tabletop[0][iterX][iterY], tabletop[1][iterX][iterY]);
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



int main(){
    char inicial[TAM][TAM];
    readFile(inicial);
    automata *tabletop[2][TAM][TAM];
    automata *base;
    generateAutomata(&base);
    initialize(tabletop, inicial, base);
    /**/
    #ifdef debug
        printf("%p\n",base);
        printTabletop(tabletop[0]);
        printf("\n");
        printTabletop(tabletop[0]);
        automata *test=base;
        printf("%p %p %d\n", tabletop, base, state);
        printf("\n\n\n\n%p    name %d\n",test, base->name);
    #endif
    /**/
    char state=1;
    char value;
    printf("\e[1;1H\e[2J\n\n\n\n\n");
    printTabletop(tabletop[state]);
    #ifdef time
    while(1){    
        sleep(1);
    #else
    while((value=fgetc(stdin))!='q'){
    #endif
        /**/
        #ifdef debugtest
        if(value!='\n'){
            printf("index of matrix %d\n", value-'0');
            action(&test, value-'0');
            
            printf("actual node %d\n",test->name);
        }
        /**/
        #endif
        #ifndef debugtest
            #ifdef debug
                printf("main %d\n", state);
            #endif
            /**/
            life(tabletop, base, state);
            printf("\e[1;1H\e[2J\n\n\n\n\n");
            printTabletop(tabletop[state]);
            /**/
        #endif
        /**/
        #ifdef printhelp
            printf("help\n");
            printTabletop(tabletop[state^1]);
            printf("\n");
        #endif
        /**/
        state^=1;
    }
    printf("hola %ld\n",sizeof(automata));
}
