//structure to handle the score
struct scoring{
    char name[100];
    float value;
};

int nx;//to pass the number of rows to the timer thread
int mx;//to pass the number of columns to the timer thread



time_t t0;//the start time

time_t t;//the end time


int x=0;// variable to communicate wit thread if it 0 thread work
//if it 1 thread stop
void* timer(void* p){
    int i=0;
    char **v=p;
    while(x==0){
        Sleep(10);
        i++;
        if(i%2000==0){
                printing(nx,mx,v);
        } //wait 20 seconds then print the grid with the time update

    }
    return NULL;
}

/**  function to take int   **/
int getInt(int vx,int lim){


    int i=0,value;
    char buffer[50];
    char temp[5];
    char *chk;
    int c;
    do{
        i=0;
    switch(vx){
        case 0:printf("Enter a number of rows = ");break;
        case 1:printf("Enter a number of columns = ");break;
        default:;
    }
    fflush(stdout);
    fflush(stdin);
    gets(buffer);
    int v=0;
    c=0;
    while(buffer[v]!='\0'&&buffer[v]!='\n'){
        if(buffer[v]!=' '&&buffer[v]!='\t'){
            temp[c]=buffer[v];
            c++;
        }
        v++;
    }
    temp[c]='\0';
        value = (int) strtol(temp, &chk, 10);/* assume decimal format */

    if(vx==0||vx==1){
         if(value<2){i=1;}

    }
    } while (!isspace(*chk) && *chk != 0 || value>=lim || value<0 || i==1);

    return value;
}

int moves=0;//moves of the player

int identfier;//global variable to communicate with printing function to print something
int timeload=0;//for load the time from a save game

void printing(int n,int m, char arr[n][m]){
    system("cls");

     printf("Moves = %d",moves);


     time(&t);
     int minute,second;
     minute=(t-t0+timeload)/60;
     second=(t-t0+timeload)%60;
     printf("    TIME : %d:%d",minute,second);
     int flags=1+(n*m)/10;//indicating the number of flags
     int question=0;
     int i,j;
     for(i=0;i<n;i++){
       for(j=0;j<m;j++){
        if(arr[i][j]=='F'){flags--;}
        if(arr[i][j]=='?'){question++;}
       }
     }

     printf("     Flags = %d",flags);
     printf("     question = %d",question);


     printf("\n\n");
       HANDLE  hConsole;
       hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int z;
    SetConsoleTextAttribute(hConsole,15);
    for(i=0;i<n;i++){
        printf("   ");
        for(z=0;z<m*4;z++){printf("-");}
        printf("\n");
        printf("%d>>",i);
        for(j=0;j<m;j++){
         if(arr[i][j]=='x'){
            SetConsoleTextAttribute(hConsole,15);
             printf("%2c",arr[i][j]);
            SetConsoleTextAttribute(hConsole,15);
         }
         else if(arr[i][j]>='1'&&arr[i][j]<='8'){
            SetConsoleTextAttribute(hConsole,12);
             printf("%2c",arr[i][j]);
            SetConsoleTextAttribute(hConsole,15);
         }
         else if(arr[i][j]=='F'||arr[i][j]=='?') {
            SetConsoleTextAttribute(hConsole,10);
             printf("%2c",arr[i][j]);
            SetConsoleTextAttribute(hConsole,15);
         }
         else{
            SetConsoleTextAttribute(hConsole,14);
             printf("%2c",arr[i][j]);
            SetConsoleTextAttribute(hConsole,15);
         }
         printf(" |");
        }
        printf("\n");
    }
    printf("   ");
    for(i=0;i<m;i++){printf("----");}
    printf("\n");
    printf(" ");
    for(i=0;i<m;i++){printf("%4d",i);}
    printf("\n");
    switch(identfier){
        case 0:printf("Enter The row index = ");break;
        case 1:printf("Enter The column index = ");break;
        case 2:printf("choose your action (open,flag,question,save)\nEnter your action : ");break;
        default:;
    }

}
/**     function turn integers to character  **/
char character(int x){
    char z;
    switch(x){
        case 0:z=' ';break;
        case 1:z='1';break;
        case 2:z='2';break;
        case 3:z='3';break;
        case 4:z='4';break;
        case 5:z='5';break;
        case 6:z='6';break;
        case 7:z='7';break;
        case 8:z='8'; break;
    }
    return z;
}
    //test if the string given from user valid or not
int compare(char* str){

     if(!strcmp(str,"open")||!strcmp(str,"o")){return 1;}
     else if(!strcmp(str,"flag")||!strcmp(str,"f")){return 2;}
     else if(!strcmp(str,"save")||!strcmp(str,"s")){return 4;}
     else if(!strcmp(str,"question")||!strcmp(str,"?")){return 3;}
    else{return 0;}

}
// turn string to lower case
void lowercase(char* str){
    int i=0;
    while(str[i]!='\0'){
        if(str[i]>='A'&&str[i]<='Z'){str[i]=tolower(str[i]);}
        i++;
    }

}
// get string describe the action from the user
int word(char* str){
       do{
        //printf("Please enter your action: ");
        fflush(stdin);
        fflush(stdout);
        scanf("%s",str);
        lowercase(str);
       }while(!compare(str));

       return compare(str);
}


void reading(float score,char str[100],struct scoring sheet[]){

    FILE *fp;

    fp=fopen("scorenames.txt","r");
    int i=0;
    char temp[100];
    while(!feof(fp)){
            fflush(stdin);
            fflush(stdout);
           fgets(temp,100,fp);
           int x=0;
       while(temp[x]!='\0'&&temp[x]!='\n'){
            sheet[i].name[x]=temp[x];
            x++;
       }
       sheet[i].name[x]='\0';//to treminate the string
           i++;
    }
    fclose(fp);
    fp=fopen("scorevalue.txt","r");
    int j=0;
    int z;
    while(!feof(fp)){
           fscanf(fp,"%f",&sheet[j].value);
           j++;
    }
    fclose(fp);
    int f=0;
    lowercase(str);


    for(z=0;z<i;z++){
        if(strcmp(str,sheet[z].name)== 0){f=1;break;}
    }

    if(f==1)
    {
        sheet[z].value=score;
    }

    else{
        strcpy(sheet[i].name,str);
        sheet[i].value=score;
        i++;
    }

    int arr[i];
    for(j=0;j<i;j++){arr[j]=j;}
    for(j=0;j<i;j++){
        for(z=j+1;z<i;z++){
         if(sheet[j].value<sheet[z].value){
                int temp=arr[j];
                arr[j]=arr[z];
                arr[z]=temp;
         }
        }
    }
    fp=fopen("scorenames.txt","w");
    for(j=0;j<i-1;j++){
        fprintf(fp,"%s\n",sheet[arr[j]].name);
    }
    fprintf(fp,"%s",sheet[arr[j]].name);
    fclose(fp);

    fp=fopen("scorevalue.txt","w");
    for(j=0;j<i-1;j++){
        fprintf(fp,"%f\n",sheet[arr[j]].value);
    }
    fprintf(fp,"%f",sheet[arr[j]].value);
    fclose(fp);

}

void saveing(int n,int m,int arr[n][m],char charr[n][m] ){
    int i,j;
    FILE *fp;
    fp=fopen("saving.txt","w");
    fprintf(fp,"%d %d ",n,m);
    fprintf(fp,"%d ",moves);
    fprintf(fp,"%d ",(t-t0+timeload));

    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            fprintf(fp,"%d ",arr[i][j]);
        }
    }


    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            fprintf(fp,"%d ",charr[i][j]);
        }
    }
    fclose(fp);

}
