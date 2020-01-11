#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "petC.h"
char colors[4][8] = { "\033[34m", "\033[91m", "\033[93m", "\033[32m" };

int dice(){
  srand(time(NULL));
  return rand()%6+1;
}

void MoveHorse(int *line, int *col, int dice){
  //Primer giro
  if(*line == 13 && *col <= 13){
    *col = *col + dice*2;
    if (*col > 13)
    {
      *line = *line - (*col-13);
      *col = 13;
    }
    return;
  }
  //Segundo y tercer giro
  if(*line <= 13 && *col == 13)
  {
    *line = *line - dice*2;
    if(*line < 1)
    {
      *col = *col + abs(*line-1);
      *line = 1;
      if(*col > 17){
        *line = *line + (*col-17);
        *col = 17;
      }
    }
    return;
  }

  //cuarto giro
  if(*line <= 13 && *col == 17)
  {
    *line = *line + dice*2;
    if(*line > 13)
    {
      *col = *col + (*line-13);
      *line = 13;
    }
    return;
  }
  //quinto y sexto giro
  if(*line == 13 && *col >= 17)
  {
    *col = *col + dice*2;
    if(*col > 29)
    {
      *line = *line + (*col-29);
      *col = 29;
      if (*line > 17)
      {
        *col = *col - (*line-17);
        *line = 17;
      }
      
    }
    return;
  }

  //septimo giro
  if(*line == 17 && *col >= 17)
  {
    *col = *col - dice*2;
    if(*col < 17)
    {
      *line = *line + abs(*col-17);
      *col = 17;
    }
    return;
  }
  //octavo y noveno giro
  if(*line >= 17 && *col == 17)
  {
    *line = *line + dice*2;
    if(*line > 29)
    {
      *col = *col - (*line-29);
      *line = 29;
      if (*col < 13)
      {
        *line = *line - abs(*col-13);
        *col = 13;
      }
      
    }
    return;
  }

  //decimo giro
  if(*line >= 17 && *col == 13)
  {
    *line = *line - dice*2;
    if(*line < 17)
    {
      *col = *col - abs(*line-17);
      *line = 17;
    }
  }
  //undecimo y doceavo giro
  if(*line == 17 && *col <= 13)
  {
    *col = *col - dice*2;
    if(*col < 1)
    {
      *line = *line - abs(*col-1);
      *col = 1;
      if (*line < 13)
      {
        *col = *col + abs(*line-13);
        *line = 13;
      }
      
    }
    return;
  }

}

void startGame(){

  int nper, action, dicet, nch, posi, posj;
  int DS; //Deplacer ou Sortir le cheval
  char horse = "H";
  char table[31][31];
  
  setTable(table);
  printf("LES PETITS CHEVAUX\n Vous voulez jouer avec combiens personnes ?(2-4): ");
  scanf("%d", &nper);
  while (nper<=1 || nper>=5)
  {
    printf("S'il vous plait, saisir un numero valable: ");
    scanf("%d", &nper);
  }

  struct Player *Players = (struct Player *) malloc(nper*sizeof(struct Player));
  if(Players==NULL)
  {
    puts("Error");
    exit(1);
  }
  for (int i = 0; i < nper; ++i)
  {
    printf("\nSaisir le nom du joueur numero %d: ", i+1);
    scanf("%s", &(Players+i)->name);
    fflush(stdin);
  }

  for (int i = 0; i < nper; i++)
  {
    for (int j = 1; j <= 4; j++)
    {
      Players[i].CJ[j-1].ID = j;
    }
    
  }

  for (int i = 0; i < nper; ++i)
  {
    printf("\nP%d : %s", i+1, (Players+i)->name);
  }
  int* toOrder = (int *) malloc(nper*sizeof(int));
  int* Ordered = (int *) malloc(nper*sizeof(int));
  int x, y, cnt=0, acc=0, SN=0, tempi, tempj, oldi, oldj;

  printf("\nLe chiffre le plus haut commence le jeu\n");
  for (int i = 0; i < nper; ++i)
  {
    printf("\n%s, appuyez ENTER pour lancer le de ", (Players+i)->name);
    getchar();
    toOrder[i] = dice();
    printf("Vous avez eu %d\n", toOrder[i]);

    while (i > 0 && toOrder[i] == toOrder[i-1])
    {
      printf("\nVous avez eu le meme chiffre que le dernier joueur, s'il vous plait lancer le de encore une fois");
      printf("\n%s, appuyez ENTER pour lancer le de ", (Players+i)->name);
      getchar();
      toOrder[i] = dice();
      printf("Vous avez eu %d\n", toOrder[i]);
    }
    
  }

  while (acc < nper)
  {
    x = toOrder[acc];

    for (int i = 0; i < nper; i++)
    {
      if (x < toOrder[i])
      {
        cnt++;
      }
    }
    Ordered[acc] = cnt;
    cnt = 0;
    acc++;
  }
  
  free(toOrder);
  
  int turn, turnCnt = 0;
  for (int i = 0; i < nper; i++)
  {
    Players[i].HorseCnt = 0;
    Players[i].IsDone = 0;
    Players[i].num = i;
  }
  int HorseCntP0 = 0, HorseCntP1 = 0, HorseCntP2 = 0, HorseCntP3 = 0;
  while (Players[0].IsDone != 3 || Players[1].IsDone != 3) //Voy a tener problemas
  {
    turn = Ordered[turnCnt];
    printf("\n%s, c'est a vous de jouer !", Players[turn].name);
    printf("\n1. Lancer le de\n2. Sauvegarder la partie\n3. Sortir\n");
    scanf("%d", &action);
    switch (action)
    {
    case 1:
      dicet = dice();
      if (dicet != 6 && Players[turn].HorseCnt == 0)
      {
        printf("Vous ne pouvez pas sortir vos chevaux\n");
        printf("%d\n",dicet);
        break;
      }

      if (dicet == 6 && Players[turn].HorseCnt == 0)
      {
        if (turn == 0){
          Players[turn].CJ[HorseCntP0].posi = 13;
          Players[turn].CJ[HorseCntP0].posj = 1;
          HorseCntP0++;
        }
        if (turn == 1){
          Players[turn].CJ[HorseCntP1].posi = 1;
          Players[turn].CJ[HorseCntP1].posj = 17;
          HorseCntP1++;
        }
        if (turn == 2){
          Players[turn].CJ[HorseCntP2].posi = 17;
          Players[turn].CJ[HorseCntP2].posj = 29;
          HorseCntP2++;
        }
        if (turn == 3){
          Players[turn].CJ[HorseCntP3].posi = 29;
          Players[turn].CJ[HorseCntP3].posj = 13;
          HorseCntP3++;
        }
        
        printTable(table, Players[turn].CJ[Players[turn].HorseCnt].ID, Players[turn].num,Players[turn].CJ[Players[turn].HorseCnt].posi, Players[turn].CJ[Players[turn].HorseCnt].posj, Players[turn].CJ[Players[turn].HorseCnt].posi, Players[turn].CJ[Players[turn].HorseCnt].posj);
        printf("Cheval %d dehors !\n", Players[turn].HorseCnt+1);
        if (Players[turn].HorseCnt < 4) Players[turn].HorseCnt++;
        break;
      }
      
      if (dicet == 6 && Players[turn].HorseCnt > 0 && Players[turn].HorseCnt <= 4)
      {
        printf("Vous voulez sortir ou deplacer un cheval ?\n1. Sortir\n2. Deplacer\n");
        scanf("%d", &DS);
        switch (DS)
        {
        case 1:

          if (turn == 0){
            Players[turn].CJ[HorseCntP0].posi = 13;
            Players[turn].CJ[HorseCntP0].posj = 1;
            HorseCntP0++;
          }
          if (turn == 1){
            Players[turn].CJ[HorseCntP1].posi = 1;
            Players[turn].CJ[HorseCntP1].posj = 17;
            HorseCntP1++;
          }
          if (turn == 2){
            Players[turn].CJ[HorseCntP2].posi = 17;
            Players[turn].CJ[HorseCntP2].posj = 29;
            HorseCntP2++;
          }
          if (turn == 3){
            Players[turn].CJ[HorseCntP3].posi = 29;
            Players[turn].CJ[HorseCntP3].posj = 13;
            HorseCntP3++;
          }
          printTable(table, Players[turn].CJ[Players[turn].HorseCnt].ID, Players[turn].num,Players[turn].CJ[Players[turn].HorseCnt].posi, 
            Players[turn].CJ[Players[turn].HorseCnt].posj, Players[turn].CJ[Players[turn].HorseCnt].posi, Players[turn].CJ[Players[turn].HorseCnt].posj);
          printf("Cheval %d dehors !\n", Players[turn].HorseCnt+1);
          printf("\n");
          if (Players[turn].HorseCnt < 4) Players[turn].HorseCnt++;
          
        break;
          
          case 2:
            printf("Vous avez eu %d\n Vous voulez deplacer quel cheval ? (1-%d):\n", dicet, Players[turn].HorseCnt);
            scanf("%d", &nch);
            while (nch < 1 || nch > Players[turn].HorseCnt){
              printf("\nS'il vous plait sasir un numero valable: ");
              scanf("%d", &nch);
            }

            tempi = Players[turn].CJ[nch-1].posi;
            tempj = Players[turn].CJ[nch-1].posj;
            oldi = tempi;
            oldj = tempj;
            MoveHorse(&tempi, &tempj, dicet);
            Players[turn].CJ[nch-1].posi = tempi;
            Players[turn].CJ[nch-1].posj = tempj;
            //printf("i: %d\nj: %d", Players[turn].CJ[nch-1].posi, Players[turn].CJ[nch-1].posj);
            //Players[turn].CJ[nch-1].name nombre del caballo
            printTable(table, Players[turn].CJ[nch-1].ID, Players[turn].num, Players[turn].CJ[nch-1].posi, Players[turn].CJ[nch-1].posj, oldi, oldj);
            printf("\nLe cheval %d s'est deplace %d case(s)\n", nch, dicet);
            break;
          }
          break;
        }

      if(dicet != 6 && Players[turn].HorseCnt > 0 && Players[turn].HorseCnt <= 4)
      {
        printf("\nVous avez eu %d\nVous voulez deplacer quel cheval ? (1-%d):\n", dicet, Players[turn].HorseCnt);
        scanf("%d", &nch);
        while (nch < 1 || nch > Players[turn].HorseCnt){
          printf("\nS'il vous plait sasir un numero valable: ");
          scanf("%d", &nch);
        }
        
        tempi = Players[turn].CJ[nch-1].posi;
        tempj = Players[turn].CJ[nch-1].posj;
        oldi = tempi;
        oldj = tempj;
        MoveHorse(&tempi, &tempj, dicet);
        Players[turn].CJ[nch-1].posi = tempi;
        Players[turn].CJ[nch-1].posj = tempj;
        printf("i: %d\nj: %d", Players[turn].CJ[nch-1].posi, Players[turn].CJ[nch-1].posj);
        //Players[turn].CJ[nch-1].name
        printTable(table, Players[turn].CJ[nch-1].ID, Players[turn].num, tempi, tempj, oldi, oldj);
        printf("\nLe cheval %d s'est deplace %d case(s)\n", nch, dicet);
        break;
      }
        
      //break;
    
    case 2:
      //struct Game *NewGame = (struct Game*) malloc(sizeof(struct Game));
      //FILE *order;
      // FILE *ply;
      // ply = fopen("C:\\PeCh.txt","wb");
      // fwrite(Players, sizeof(struct Player), nper, ply);
      // fclose(ply);
      
      // printf("La partie est sauvegardée");
      // exit(0);
    
    case 3:
      free(Players);
      free(Ordered);
      printf("A BIENTOT !");
      exit(0);
    }
    turnCnt++;
    if(turnCnt == nper) turnCnt = 0;
    for (int i = 0; i < nper; i++)
      for (int j = 0; j < 4; j++)
        if(Players[i].CJ[j].posi == 15 && Players[i].CJ[j].posj == 15) Players[i].IsDone++;
    
  }

}

void setTable(char table[31][31]){
  //char table[31][31];
  char num[] = {'1','2','3','4','5','6'};
  int i, j, cnt1=0, cnt2=0, cnt3=6, cnt4=6;

  for(i=0; i<31; i++){
    for(j=0; j<31; j++){
      if (i%2!=0 && j==13 /*&& (i<=13 || i>=17)*/) table[i][j] = 43;
      else if (j==17 && i%2!=0 /*&& (i<=13 || i>=17)*/) table[i][j] = 43;
      else if (i==13 && j%2!=0 /*&& (i<=13 || i>=17)*/) table[i][j] = 43;
      else if (i==17 && j%2!=0 /*&& (i<=13 || i>=17)*/) table[i][j] = 43;
      else table[i][j] = 32;
    }
  }

  for(i=3, j=3; i<=14, j<=14; i++, j++){
      if(i%2!=0){
        table[i][15] = num[cnt1];
        cnt1++;
      }
      if(j%2!=0){
        table[15][j] = num[cnt2];
        cnt2++;
      } 
  }

  for(i=15, j=15; i<=30, j<=27; i++, j++){
      if(i%2!=0){
        table[i][15] = num[cnt3];
        cnt3--;
      }
      if(j%2!=0){
        table[15][j] = num[cnt4];
        cnt4--;
      } 
  }

  table[15][1] = 43;
  table[1][15] = 43;
  table[29][15] = 43;
  table[15][29] = 43;
  
}

void printTable(char table[31][31], int horse, int pn, int posi, int posj, int oldi, int oldj){
  
  //char colors[4][8] = { "\033[34m", "\033[91m", "\033[93m", "\033[32m" };
  //table[posi][posj] = colors[pn]+horse+'\033[0m';
  table[posi][posj] = horse+'0';
  if(posi != oldi || posj != oldj) table[oldi][oldj] = 43;
  for(int i=0; i<31; i++){
    for(int j=0; j<31; j++){
      printf("%c ",table[i][j]);
    }
  printf("\n");
  }
}

int main(){
  startGame();
  //setTable();
  return 0;
}