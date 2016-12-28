#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <windows.h>
#include "Mines sweeper functions.h"


    int l=0;//indicating for losing the game

    /** prototype of the function used in the game describe the stages of the game **/
    void start(int n,int m);
    void openEmpty(int n, int m,int grid[n][m],char gameGrid[n][m],int cell_x,int cell_y );
    int nextmove(int n,int m,int grid[n][m],char gameGrid[n][m],int cell_x,int cell_y,int minescount);
    void CheckingFlag(int n,int m,int grid[n][m],char gameGrid[n][m],int cell_x,int cell_y);
    void openAroundflage(int n,int m,int grid[n][m],char gameGrid[n][m],int cell_x,int cell_y);
    void lose(int n, int m,int grid[n][m],char gameGrid[n][m],int cell_x,int cell_y);
    void win(int n,int m,char arr[n][m]);

    /** the main function describe the main menu in the game and calls from it the function of the game **/
int main()
{
    HANDLE hConsole;//define handle to change the color of the inputs
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,15);


    int n,m,chose;//n for the number of rows m for the number of columns chose indicate the user choice in the main manu
    printf("chose what you want\n");
    printf("1-start\n2-load\n3-score\n        for exit press 0");
    printf("\n");
    chose=getInt(4,4);


//chose == 1  start new game
   if(chose==1){
    n=getInt(0,30);
    m=getInt(1,30);
    start(n,m);
   }

   FILE *fp;// file pointer to handle the scores files and the save file
   int i,j;
   //chose == 2 load the last game from the file saving.txt
   if(chose==2){
     int timeing;//the timing before the user save
     fp=fopen("saving.txt","r");
     fscanf(fp,"%d %d",&n,&m);
     fscanf(fp,"%d",&moves);//moves of the user game
     fscanf(fp,"%d",&timeing);
     timeload=timeing;//add the time before the load to the game
     time(&t0);
     int arr[n][m];//indicating the bombs and the number and used to display the result of the user choice
     char charr[n][m];//the grid that scaned from the file contain the game grid

     for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            fscanf(fp,"%d",&arr[i][j]);;
        }
    }


    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            fscanf(fp,"%d",&charr[i][j]);
        }
    }
    fclose(fp);


    int c=0;//breaking condition for the loop c=1 when the game finished(losing or winning)
          while(c==0){
           c=nextmove(n,m,arr,charr,0,0,1+(n*m)/10);
          }
   }

   // chose 3 load the scores of the player who finished the game
    if(chose==3){
        fp=fopen("scorenames.txt","r");//file contain the user names sorted
        struct scoring player[50];//structure to hold the user names & the scores
        int i=0;
        int j=0,z=0;
        char temp [250];
        while(!feof(fp)){
        fflush(stdin);
        fflush(stdout);
        fgets(temp,250,fp);
        //fscanf(fp,"%s",temp);

        strcpy(player[i].name,temp);//copy the name to the structure
        i++;
   }
        fclose(fp);
        fp=fopen("scorevalue.txt","r");//file contain the user's scores sorted
        while(!feof(fp)){
            fflush(stdin);
            fflush(stdout);
            fscanf(fp,"%f",&player[j].value);//copy the score from the file
            j++;
        }
        fclose(fp);
        //printing the score board
        for(j=0;j<i;j++){
            printf("%s>>",player[j].name);
            printf("%.2f",player[j].value);
            printf("\n\n");
        }
        printf("\n\n");
         main();
    }


    return 0;

}


/** first move in the game
  this function create the grid and take the first index from the user and random the mines
**/

void start(int n,int m){
    // define the grid by the number of rows and columns thats the user wont
      char gameGrid[n][m];//creating the array of character that will display to user
      int grid[n][m];//creating the array of int to compare with it for the game
      int i,j;
      for(i=0;i<n;i++){
        for(j=0;j<m;j++){grid[i][j]=0;gameGrid[i][j]='X';}//creating every cell in gameGrid as "X"
      }

      time(&t0);//the time of the start of the game
      printing(n,m,gameGrid);
      int cell_x,cell_y;
      // taking first input from user
      cell_x=getInt(2,n);
      identfier=1;//global var to communicate with printing function to print some thing
      printing(n,m,gameGrid);
      cell_y=getInt(3,m);

        moves=1;//user made the first move

      //creating mines
      int minesCount=1+(n*m)/10;
      int mines[minesCount];
      //random generating the mine in the array of int
      srand(time(NULL));
    for(i=0;i<minesCount;){
       int z,v;
       z=rand()%n;v=rand()%m;
       if(grid[z][v]==10||(z == cell_x && v == cell_y))
       {continue;}
       //testing whether the random place taken before or it is the first cell chosen by the user
       grid[z][v]=10;//"10" or more in the array of int stand for mines
       mines[i]=z*10+v;//getting the index of the mine in array of mines
       i++;
    }
    //the value >= 10 indicate to a mine
    //creating the number which help the player to locate the mines
    int mines_x,mines_y;
    for(i=0;i<minesCount;i++){
         mines_x=mines[i]/10;mines_y=mines[i]%10;
     //testing if the place to add 1 is out of the array or not
         if(mines_y!= m-1)   {grid[mines_x][mines_y+1]+=1;}
         if(mines_y!= 0)   {grid[mines_x][mines_y-1]+=1;}
         if(mines_x!= n-1)   {grid[mines_x+1][mines_y]+=1;}
         if(mines_x!= 0)   {grid[mines_x-1][mines_y]+=1;}
         if(mines_x!= 0 && mines_y!= 0)   {grid[mines_x-1][mines_y-1]+=1;}
         if(mines_x!= n-1 && mines_y!= m-1)   {grid[mines_x+1][mines_y+1]+=1;}
         if(mines_y!= m-1 && mines_x!= 0)   {grid[mines_x-1][mines_y+1]+=1;}
         if(mines_x!= n-1 && mines_y!= 0)   {grid[mines_x+1][mines_y-1]+=1;}
        }

        //open the empty cell that the user give
        if(grid[cell_x][cell_y]==0)
        {
            openEmpty(n,m,grid,gameGrid,cell_x,cell_y);//function to open the cells around the empty cell
        }
        else{
            gameGrid[cell_x][cell_y]=grid[cell_x][cell_y]+'0';//if the cell was not empty print the number of it
        }

        printing(n,m,gameGrid);//printing the grid after the first move

        FILE *fp;
        fp=fopen("mine.txt","w");
        for(i=0;i<n;i++){
            for(j=0;j<m;j++){
                fprintf(fp,"%2d ",grid[i][j]);
            }
            fprintf(fp,"\n");
        }
        fclose(fp);
        int c=0;//nowing if the game is end or not
        while(c == 0){
            c = nextmove(n,m,grid,gameGrid,cell_x,cell_y,minesCount);
        }
}

/** the function looping the game and taking action from a user
    it return zero if the game still continue
    and it return 1 if the user win or lose
**/

int nextmove(int n,int m,int grid[n][m],char gameGrid[n][m],int cell_x,int cell_y,int minesCount){
    int counter=1;
    char action[150];//string to take the user action

    identfier=0;//determine what the user should in put 0 mean asking for rows
    printing(n,m,gameGrid);

    int i,j;

    int act;//integer define the action the user make

    //the thread indicating the time of the idle
    nx=n;//making the dimension of the grid global variable to call the thread
    mx=m;//making the dimension of the grid global variable to call the thread

    //creating the thread
    pthread_t pth;
    pthread_create(&pth,NULL,timer,gameGrid);//sending the grid to the thread

    //getting the indexes of the cell the user chose it
    cell_x=getInt(2,n);

    x=1;//x=1 to stop the thread
    pthread_join(pth,NULL);//waiting the thread to terminate

    x=0;//making the x=0 to create the thread again
    pthread_create(&pth,NULL,timer,gameGrid);

    identfier=1;//determine what the user should input 1 mean asking for columns
    printing(n,m,gameGrid);
    cell_y=getInt(3,m);

    x=1;
    pthread_join(pth,NULL);

    x=0;
    pthread_create(&pth,NULL,timer,gameGrid);

    identfier=2;//determine what the user should input 2 asking for action
    printing(n,m,gameGrid);

    //get action from user
    act=word(action);// act is a variable describe the user action if it was 1 mean open,2 mean flag,3 mean question,4 mean save

    x=1;
    pthread_join(pth,NULL);

    x=0;
    counter++;
    moves++;
    //FILE *p;
    // testing the input
    if(act==4){// 4 indicate for saving the game
            saveing(n,m,grid,gameGrid);
    }

// act == 1 indicate to open a cell
    else if(act==1 && gameGrid[cell_x][cell_y]!='F' && gameGrid[cell_x][cell_y]!='?' ){

            if(grid[cell_x][cell_y]==0){openEmpty(n,m,grid,gameGrid,cell_x,cell_y);}//if the cell empty open the adjacent cell of it
            else if(grid[cell_x][cell_y]>=10){lose(n,m,grid,gameGrid,cell_x,cell_y);}//if the cell contain mine make the user lose
        //if the cell contain a "number" check the number of flag around it otherwise display the number
            else if(gameGrid[cell_x][cell_y]>='1'&&gameGrid[cell_x][cell_y]<='8'){CheckingFlag(n,m,grid,gameGrid,cell_x,cell_y);}
          //printing what incide the cell
            else{gameGrid[cell_x][cell_y]=character(grid[cell_x][cell_y]);}
    }

    //to remove the question and flags
    else if(gameGrid[cell_x][cell_y]=='F' || gameGrid[cell_x][cell_y]=='?'){gameGrid[cell_x][cell_y]='X';}

    else if(act==2&&gameGrid[cell_x][cell_y]=='X'){gameGrid[cell_x][cell_y]='F';}//put a flag

    else if(act==3&&gameGrid[cell_x][cell_y]=='X'){gameGrid[cell_x][cell_y]='?';}//put a question

    identfier=3;
    printing(n,m,gameGrid);

    int count=0;
    //loops to count how many cell are open
    for(i=0;i<n;i++){
            for(j=0;j<m;j++){
               if(gameGrid[i][j]!='X'&&gameGrid[i][j]!='?'&&gameGrid[i][j]!='F'){count++;}
            }
        }

/** if the number of open cells =                n*m                   -       (n*m/10+1)
                                (the number of all cells in the grid )   (the number of mines)
          mean the all cell which not contain mines are opened
          and the player win
**/


    //printf("%d\n",count);
    if(l!=1&&count!=(n*m-((n*m)/10+1))){return 0;}//if the open cells less than the number of cells that doesn't contain bombs
    else if(l==1){printf("\nyou lost \n");return 1;}//the global variable l indicating for losing
    else {printf("\nyou won \n");win(n,m,gameGrid);return 1;}
    // nextmove function return 1 when the player win or lose to stop the game
}


/**counting the number of the flag around the cell that the user chose **/
void CheckingFlag(int n,int m,int grid[n][m],char gameGrid[n][m],int cell_x,int cell_y){
    int counter=0;//counter for the flags

    if(cell_x!=n-1&&cell_y!=m-1){
         if(gameGrid[cell_x+1][cell_y+1]=='F'){counter++;}
        }
    if(cell_x!=0&&cell_y!=0){
         if(gameGrid[cell_x-1][cell_y-1]=='F'){counter++;}
    }
    if(cell_x!=n-1){
         if(gameGrid[cell_x+1][cell_y]=='F'){counter++;}
    }
    if(cell_x!=0){
         if(gameGrid[cell_x-1][cell_y]=='F'){counter++;}
    }
    if(cell_y!=m-1){
         if(gameGrid[cell_x][cell_y+1]=='F'){counter++;}
    }
    if(cell_y!=0){
         if(gameGrid[cell_x][cell_y-1]=='F'){counter++;}
    }
    if(cell_x!=n-1&&cell_y!=0){
         if(gameGrid[cell_x+1][cell_y-1]=='F'){counter++;}
    }
    if(cell_x!=0&&cell_y!=m-1){
         if(gameGrid[cell_x-1][cell_y+1]=='F'){counter++;}
    }
        // the number of flags around the cell that contain a number = number
        //open the cells around it
        if(gameGrid[cell_x][cell_y]==character(counter)){
            openAroundflage(n,m,grid,gameGrid,cell_x,cell_y);
        }
}

/**open the cell around number beside the flag
   how the function work
   there are 8 if conditions every one of them
   test whether the cell contain a number or an empty cell or a mine
**/
void openAroundflage(int n,int m,int grid[n][m],char gameGrid[n][m],int cell_x,int cell_y){

    if(cell_x!=n-1&&cell_y!=m-1){
     if(gameGrid[cell_x+1][cell_y+1]!='F'){
      if(grid[cell_x+1][cell_y+1]<10&&grid[cell_x+1][cell_y+1]!=0){gameGrid[cell_x+1][cell_y+1]=character(grid[cell_x+1][cell_y+1]);}
      else if(grid[cell_x+1][cell_y+1]==0){openEmpty(n,m,grid,gameGrid,cell_x+1,cell_y+1);}
      else{lose(n,m,grid,gameGrid,cell_x+1,cell_y+1);}
     }
    }

        if(cell_x!=0&&cell_y!=0){
         if(gameGrid[cell_x-1][cell_y-1]!='F'){
           if(grid[cell_x-1][cell_y-1]<10&&grid[cell_x-1][cell_y-1]!=0){gameGrid[cell_x-1][cell_y-1]=character(grid[cell_x-1][cell_y-1]);}
           else if(grid[cell_x-1][cell_y-1]==0){openEmpty(n,m,grid,gameGrid,cell_x-1,cell_y-1);}
           else{lose(n,m,grid,gameGrid,cell_x-1,cell_y-1);}
         }
        }

        if(cell_x!=n-1){
         if(gameGrid[cell_x+1][cell_y]!='F'){
          if(grid[cell_x+1][cell_y]<10&&grid[cell_x+1][cell_y]!=0){gameGrid[cell_x+1][cell_y]=character(grid[cell_x+1][cell_y]);}
            else if(grid[cell_x+1][cell_y]==0){openEmpty(n,m,grid,gameGrid,cell_x+1,cell_y);}
              else{lose(n,m,grid,gameGrid,cell_x+1,cell_y);}
         }
        }

        if(cell_x!=0){
         if(gameGrid[cell_x-1][cell_y]!='F'){
          if(grid[cell_x-1][cell_y]<10&&grid[cell_x-1][cell_y]!=0){gameGrid[cell_x-1][cell_y]=character(grid[cell_x-1][cell_y]);}
            else if(grid[cell_x-1][cell_y]==0){openEmpty(n,m,grid,gameGrid,cell_x-1,cell_y);}
              else{lose(n,m,grid,gameGrid,cell_x-1,cell_y);}
         }
        }

        if(cell_y!=m-1){
         if(gameGrid[cell_x][cell_y+1]!='F'){
          if(grid[cell_x][cell_y+1]<10&&grid[cell_x][cell_y+1]!=0){gameGrid[cell_x][cell_y+1]=character(grid[cell_x][cell_y+1]);}
            else if(grid[cell_x][cell_y+1]==0){openEmpty(n,m,grid,gameGrid,cell_x,cell_y+1);}
              else{lose(n,m,grid,gameGrid,cell_x,cell_y+1);}
         }
        }

        if(cell_y!=0){
         if(gameGrid[cell_x][cell_y-1]!='F'){
          if(grid[cell_x][cell_y-1]<10&&grid[cell_x][cell_y-1]!=0){gameGrid[cell_x][cell_y-1]=character(grid[cell_x][cell_y-1]);}
            else if(grid[cell_x][cell_y-1]==0){openEmpty(n,m,grid,gameGrid,cell_x,cell_y-1);}
              else{lose(n,m,grid,gameGrid,cell_x,cell_y-1);}
         }
        }

        if(cell_x!=n-1&&cell_y!=0){
         if(gameGrid[cell_x+1][cell_y-1]!='F'){
          if(grid[cell_x+1][cell_y-1]<10&&grid[cell_x+1][cell_y-1]!=0){gameGrid[cell_x+1][cell_y-1]=character(grid[cell_x+1][cell_y-1]);}
            else if(grid[cell_x+1][cell_y-1]==0){openEmpty(n,m,grid,gameGrid,cell_x+1,cell_y-1);}
              else{lose(n,m,grid,gameGrid,cell_x+1,cell_y-1);}
         }
        }

        if(cell_x!=0&&cell_y!=m-1){
         if(gameGrid[cell_x-1][cell_y+1]!='F'){
          if(grid[cell_x-1][cell_y+1]<10&&grid[cell_x-1][cell_y+1]!=0){gameGrid[cell_x-1][cell_y+1]=character(grid[cell_x-1][cell_y+1]);}
            else if(grid[cell_x-1][cell_y+1]==0){openEmpty(n,m,grid,gameGrid,cell_x-1,cell_y+1);}
              else{lose(n,m,grid,gameGrid,cell_x-1,cell_y+1);}
         }
        }
}


void lose(int n, int m,int grid[n][m],char gameGrid[n][m],int cell_x,int cell_y){
         int i,j;
         //displaying the grid after losing
         for(i=0;i<n;i++){
            for(j=0;j<m;j++){
             if(grid[i][j]>=10){
                switch(gameGrid[i][j]){
                  case 'F':gameGrid[i][j]='M';break;
                  case 'X':gameGrid[i][j]='*';break;
                  case '?':gameGrid[i][j]='*';break;
                }
             }
             if(grid[i][j]<10&&gameGrid[i][j]=='F'){gameGrid[i][j]='-';}
            }
         }
        gameGrid[cell_x][cell_y]='!';
        l=1;//changing the global variable to indicate the losing of the game
}

/**  to open every empty cells beside the empty cell the user opened   **/

void openEmpty(int n, int m,int grid[n][m],char gameGrid[n][m],int cell_x,int cell_y ){

        if(grid[cell_x][cell_y]==0){gameGrid[cell_x][cell_y]=' ';}//making the cell open.
           else{gameGrid[cell_x][cell_y]=character(grid[cell_x][cell_y]);}//if the cell contain number then show it.

        if(grid[cell_x][cell_y]==0){
                grid[cell_x][cell_y]=9;//making the visited cell marked with 9.
        }

        //checking the contain of the number grid
        //if it zero then calling the function "openEmpty"again by recursion
        //if it contain a number show the number.
        if(cell_x!=n-1&&cell_y!=m-1){
        if(grid[cell_x+1][cell_y+1]==0){ openEmpty(n,m,grid,gameGrid,cell_x+1,cell_y+1); }
        else if(grid[cell_x+1][cell_y+1]<9) { gameGrid[cell_x+1][cell_y+1]=character(grid[cell_x+1][cell_y+1]); }
        }

        if(cell_x!=0&&cell_y!=0){
        if(grid[cell_x-1][cell_y-1]==0){ openEmpty(n,m,grid,gameGrid,cell_x-1,cell_y-1); }
        else if(grid[cell_x-1][cell_y-1]<9){ gameGrid[cell_x-1][cell_y-1]=character(grid[cell_x-1][cell_y-1]); }
        }

        if(cell_x!=n-1){
        if(grid[cell_x+1][cell_y]==0){ openEmpty(n,m,grid,gameGrid,cell_x+1,cell_y); }
        else if(grid[cell_x+1][cell_y]<9){ gameGrid[cell_x+1][cell_y]=character(grid[cell_x+1][cell_y]); }
        }

        if(cell_x!=0){
        if(grid[cell_x-1][cell_y]==0){ openEmpty(n,m,grid,gameGrid,cell_x-1,cell_y); }
        else if(grid[cell_x-1][cell_y]<9){gameGrid[cell_x-1][cell_y]=character(grid[cell_x-1][cell_y]);}
        }

        if(cell_y!=m-1){
        if(grid[cell_x][cell_y+1]==0){ openEmpty(n,m,grid,gameGrid,cell_x,cell_y+1); }
        else if(grid[cell_x][cell_y+1]<9){ gameGrid[cell_x][cell_y+1]=character(grid[cell_x][cell_y+1]); }
        }

        if(cell_y!=0){
        if(grid[cell_x][cell_y-1]==0){ openEmpty(n,m,grid,gameGrid,cell_x,cell_y-1); }
        else if(grid[cell_x][cell_y-1]<9){ gameGrid[cell_x][cell_y-1]=character(grid[cell_x][cell_y-1]); }
        }

        if(cell_x!=n-1&&cell_y!=0){
        if(grid[cell_x+1][cell_y-1]==0){ openEmpty(n,m,grid,gameGrid,cell_x+1,cell_y-1); }
        else if(grid[cell_x+1][cell_y-1]<9){ gameGrid[cell_x+1][cell_y-1]=character(grid[cell_x+1][cell_y-1]); }
        }

        if(cell_x!=0&&cell_y!=m-1){
        if(grid[cell_x-1][cell_y+1]==0){ openEmpty(n,m,grid,gameGrid,cell_x-1,cell_y+1); }
        else if(grid[cell_x-1][cell_y+1]<9){ gameGrid[cell_x-1][cell_y+1]=character(grid[cell_x-1][cell_y+1]); }
        }

}

// winning the game
void win(int n,int m,char arr[n][m]){
   int i,j;
   //display to the user flags on all mines
   for(i=0;i<n;i++){
     for(j=0;j<m;j++){
        if(arr[i][j]=='X'){arr[i][j]='F';}
     }
   }

   identfier=3;//to print the grid without asking for indexes or actions
   printing(n,m,arr);


   char name[100];
   printf("congratulation you win \n");
   printf("enter your name :");
   fflush(stdin);
   fflush(stdout);
   gets(name);//getting the name of the user after winning
   time_t t;
   time(&t);
   float score;
   score=pow(n,4)*pow(m,4)/((t-t0+timeload)*moves);
   printf("your score = %.2f\n",score);//calculate the score of the user and display it

   struct scoring player[50];//create structure for the player to sort it
   reading(score,name,player);

}

