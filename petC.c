#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "petC.h"

int dice(){
  srand(time(NULL));
  return rand()%6+1;
}

void MoveHorse(int *line, int *col, int dice){
  //Primer giro
  if(*line == 14 && *col <= 14){
    *col = *col + dice;
    if (*col > 14)
    {
      *line = *line - (*col-14);
      *col = 14;
    }
    return;
  }
  //Segundo giro
  if(*line <= 14 && *col == 14)
  {
    *line = *line - dice;
    if(*line < 1)
    {
      *col = *col + abs(*line-1);
      *line = 1;
    }
    return;
  }

  if(*line == 1 && *col <= 16)
  {
    *col = *col + dice;
    if(*col > 16)
    {
      *line = *line + (*col-16);
      *col = 16;
    }
    return;
  }

  /*if(*line == 14 && *col >= 16)
  {
    *col = *col + dice;
    if(*col > 16)
    {
      //*line = *line + (*col-14);
      *col = 16;
    }
    return;
  }*/
  /*int* results[] = {*line,*col};
  return results;*/
}

void startGame(){

  int nper, action, dicet, nch, posi, posj;
  int DS; //Deplacer ou Sortir le cheval
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

  /*for (int i = 0; i < nper; i++)
  {
    for (int j = 0; j <= 4; j++)
    {
      Players[i].CJ[j].name = 'C'+j;    //Tengo un problema aquí
    }
    
  }*/
  int* toOrder = (int *) malloc(nper*sizeof(int));
  int* Ordered = (int *) malloc(nper*sizeof(int));
  int x, y, cnt=0, acc=0, SN=0;

  printf("\nLe chiffre le plus haut commence le jeu\n");
  for (int i = 0; i < nper; ++i)
  {
    printf("\n%s, appuyez ENTER pour lancer le de ", (Players+i)->name);
    getchar();
    toOrder[i] = dice();
    printf("Vous avez eu %d\n", toOrder[i]);

    while (i > 0 && toOrder[i] == toOrder[i-1])
    {
      printf("\nVous avez eu le meme chiffre que le dernier joueur, s'il vous plait lancer le des encore une fois");
      printf("\n%s, appuyez ENTER pour lancer le des ", (Players+i)->name);
      getchar();
      toOrder[i] = dice();
      printf("Vous avez eu %d\n", toOrder[i]);
    }
    
  }
  
  // while (SN<nper)
  // {
  //   y = toOrder[SN];
  //   for (int j = 1; j < nper; j++)
  //   {
  //     while (y == toOrder[j])
  //     {
  //       printf("%s et %s, vous avez eu la meme chifre, s'il vous plait lancer encore une fois\n", Players[SN].name, Players[j].name);
  //       printf("\n%s, appuyez ENTER pour lancer le des ", (Players+j)->name);
  //       getchar();
  //       toOrder[i] = dice();
  //       printf("Vous avez eu %d\n", toOrder[i]);
  //     } 
  //   }
  //   SN++;
  // }
  //
  // for (int i = 0; i < nper; i++)
  // {
  //   printf("%i  ", toOrder[i]);
  // }
  // printf("\n");

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

  for (int i = 0; i < nper; i++)
  {
    printf("%i  ", Ordered[i]);
  }
  printf("\n");
  for (int i = 0; i < nper; ++i)
  {
    printf("P%d : %s\n", i+1, (Players+i)->name);
  }
  
  int turn, turnCnt = 0;
  for (int i = 0; i < nper; i++)
  {
    Players[i].HorseCnt = 0;
    Players[i].IsDone = 0;
  }
  int HorseCntP0 = 0, HorseCntP1 = 0, HorseCntP2 = 0, HorseCntP3 = 0;
  while (Players[0].IsDone != 3 || Players[1].IsDone != 3) //Voy a tener problemas
  {
    turn = Ordered[turnCnt];
    printf("\n%s, c'est a vous de jouer !", Players[turn].name);
    printf("\n1. Lancer le des\n2. Sauvegarder la partie\n3. Sortir\n");
    scanf("%d", &action);
    switch (action)
    {
    case 1:
      dicet = dice();
      if (dicet != 6 && Players[turn].HorseCnt == 0)
      {
        printf("Vous ne pouvez pas sortir vos chevaux\n");
        printf("%d\n",dicet);
      }
      
      if (dicet == 6 && Players[turn].HorseCnt > 0)
      {
        printf("Vous voulez sortir ou deplacer un cheval ?\n1. Sortir\n2. Deplacer\n");
        scanf("%d", &DS);
        switch (DS)
        {
        case 1:

          if (turn == 0){
            Players[turn].CJ[HorseCntP0].posi = 14;
            Players[turn].CJ[HorseCntP0].posj = 1;
            HorseCntP0++;
          }
          if (turn == 1){
            Players[turn].CJ[HorseCntP1].posi = 1;
            Players[turn].CJ[HorseCntP1].posj = 16;
            HorseCntP1++;
          }
          if (turn == 2){
            Players[turn].CJ[HorseCntP2].posi = 16;
            Players[turn].CJ[HorseCntP2].posj = 29;
            HorseCntP2++;
          }
          if (turn == 3){
            Players[turn].CJ[HorseCntP3].posi = 29;
            Players[turn].CJ[HorseCntP3].posj = 14;
            HorseCntP3++;
          }
          printf("Cheval %d dehors !\n", Players[turn].HorseCnt+1);
          printf("i: %d\nj: %d", Players[turn].CJ[Players[turn].HorseCnt].posi, Players[turn].CJ[Players[turn].HorseCnt].posj);
          if (Players[turn].HorseCnt <= 4) Players[turn].HorseCnt++;
          
        break;
          
          case 2:
            printf("Vous avez eu %d\n Vous voulez deplacer quel cheval ? (1-%d):\n", dicet, Players[turn].HorseCnt);
            scanf("%d", &nch);
            while (nch < 1 || nch > Players[turn].HorseCnt){
              printf("\nS'il vous plait sasir un numero valable: ");
              scanf("%d", &nch);
            }
            // Codigo para mover el caballo
            posi = Players[turn].CJ[nch].posi;
            posj = Players[turn].CJ[nch].posj;
            MoveHorse(&posi, &posj, dicet);
            printf("i: %d\nj: %d", Players[turn].CJ[nch].posi, Players[turn].CJ[nch].posj);
            printf("\nLe cheval %d s'est deplacé %d case(s)\n", nch, dicet);
            break;
          }

        }

      if (dicet == 6 && Players[turn].HorseCnt == 0)
      {
        if (turn == 0){
          Players[turn].CJ[HorseCntP0].posi = 14;
          Players[turn].CJ[HorseCntP0].posj = 1;
          HorseCntP0++;
        }
        if (turn == 1){
          Players[turn].CJ[HorseCntP1].posi = 1;
          Players[turn].CJ[HorseCntP1].posj = 16;
          HorseCntP1++;
        }
        if (turn == 2){
          Players[turn].CJ[HorseCntP2].posi = 16;
          Players[turn].CJ[HorseCntP2].posj = 29;
          HorseCntP2++;
        }
        if (turn == 3){
          Players[turn].CJ[HorseCntP3].posi = 29;
          Players[turn].CJ[HorseCntP3].posj = 14;
          HorseCntP3++;
        }
        
        printf("Cheval %d dehors !\n", Players[turn].HorseCnt+1);
        printf("i: %d\nj: %d", Players[turn].CJ[Players[turn].HorseCnt].posi, Players[turn].CJ[Players[turn].HorseCnt].posj);
        if (Players[turn].HorseCnt <= 4) Players[turn].HorseCnt++;
      }
      
      
      if(dicet != 6 && Players[turn].HorseCnt > 0)
      {
        printf("\nVous avez eu %d\nVous voulez deplacer quel cheval ? (1-%d):\n", dicet, Players[turn].HorseCnt);
        scanf("%d", &nch);
        while (nch < 1 || nch > Players[turn].HorseCnt){
          printf("\nS'il vous plait sasir un numero valable: ");
          scanf("%d", &nch);
        }
        // Codigo para mover el caballo
        MoveHorse(Players[turn].CJ[nch].posi, Players[turn].CJ[nch].posj, dicet);
        printf("i: %d\nj: %d", Players[turn].CJ[nch].posi, Players[turn].CJ[nch].posj);
        printf("\nLe cheval %d s'a deplace %d cases\n", nch, dicet);
      }
        
      break;
    
    case 2:
      printf("La partie est sauvegardée");
      break;
    
    case 3:
      free(Players);
      free(Ordered);
      printf("A bientot !");
      exit(0);
    }
    turnCnt++;
    if(turnCnt == nper) turnCnt = 0;
    for (int i = 0; i < nper; i++)
      for (int j = 0; j < 4; j++)
        if(Players[i].CJ[j].posi == 15 && Players[i].CJ[j].posj == 15) Players[i].IsDone++;
    
  }

}

char** setTable(char table[31][31]){
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

  for(i=15, j=15; i<=30, j<=30; i++, j++){
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

  for(i=0; i<31; i++){
    for(j=0; j<31; j++){
      printf("%c ", table[i][j]);
    }
  printf("\n");
  }
  return table;
}

int main(){
  startGame();
  //setTable();
  return 0;
}