#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "petC.h"

int dice(){
  srand(time(NULL));
  return rand()%6+1;
}

void startGame(){

  int nper, action, dicet, nch/*, k=0*/;
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

  printf("\nLa chifre la plus haut commence le jeu\n");
  for (int i = 0; i < nper; ++i)
  {
    printf("\n%s, appuyez ENTER pour lancer le des ", (Players+i)->name);
    getchar();
    toOrder[i] = dice();
    printf("Vous avez eu %d\n", toOrder[i]);

    while (i > 0 && toOrder[i] == toOrder[i-1])
    {
      printf("\nVous avez eu la meme chifre que le dernier joueur, s'il vous plait lancer le des encore une fois");
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
      if (x > toOrder[i])
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
    printf("\nP%d : %s\n", i+1, (Players+i)->name);
  }
  
  int turn, turnCnt = 0;
  for (int i = 0; i < nper; i++)
  {
    Players[i].HorseCnt = 4;
  }
  
  while (1) //Voy a tener problemas
  {
    turn = Ordered[turnCnt];
    printf("%s, c'est a vous !", Players[turn].name);
    printf("\n1. Lancer le des\n2. Sauvegarder la partie\n3. Sortir\n");
    scanf("%d", &action);
    switch (action)
    {
    case 1:
      dicet = dice();
      if (dicet != 6 && Players[turn].HorseCnt == 4)
      {
        printf("Vous ne pouvez pas sortir vos chevaux\n");
      }
      else
      {
        if (dicet == 6 && Players[turn].HorseCnt == 4)
        {
          printf("Vous pouvez sortir un cheval !");
        }
        
        //Sacar el caballo que sea 
        if (dicet == 6 && Players[turn].HorseCnt < 4)
        {
          printf("Vous voulez sortir ou deplacer un cheval ?\n1. Sortir\n2. Deplacer\n");
          scanf("%d", &DS);
          switch (DS)
          {
          case 1:
            //Sacar el caballo
            Players[1].HorseCnt--;
            break;
          
          case 2:
            printf("Vous avez eu %d\n Vous voulez deplacer quel cheval ? (1-4):\n", dicet);
            scanf("%d", &nch);
            while (nch<1 || nch>4){
              printf("S'il vous plait sasir un numero valable: ");
              scanf("%d", &nch);
            }
            printf("Le cheval %d s'a deplacé %d cases\n", nch, dicet);
            break;
          }

        }
        if(dicet != 6 && Players[turn].HorseCnt > 0)
        {
          printf("Vous avez eu %d\n Vous voulez deplacer quel cheval ? (1-4):\n", dicet);
          scanf("%d", &nch);
          while (nch<1 || nch>4){
            printf("S'il vous plait sasir un numero valable: ");
            scanf("%d", &nch);
          }
          printf("Le cheval %d s'a deplacé %d cases\n", nch, dicet);
        }
        
      }
      break;
    
    case 2:
      printf("La partie est sauvegardée");
      break;
    
    case 3:
      exit;
    }
    turnCnt++;
    if(turnCnt == nper) turnCnt = 0;
  }

}

void setTable(){
  char table[31][31];
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
}

int main(){
  startGame();
  //setTable();
  return 0;
}