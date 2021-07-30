#include<stdio.h>
#include<stdlib.h>
#include<string.h>


char board[9] = {' ',' ',' ',' ',' ',' ',' ',' ',' '};

//debug
//char board[9] = {'a','b','c','d','e','f','g','h','i'};
void reset(){
    memset(board, ' ', sizeof(board));
}
void insertLetter(char letter,int pos){
    board[pos] = letter;
    printf("updating position %c at %d\n",board[pos],pos+1);
}

int isfreespace(int pos){
    return board[pos] == ' '?1:0;
}
char buffer[60];
char * print(){
    memset(buffer,'\0',60);
    snprintf(buffer,sizeof(buffer),"\n %c | %c | %c \n %c | %c | %c \n %c | %c | %c \n",board[0],board[1],board[2],board[3],board[4],board[5],board[6],board[7],board[8]);
    return buffer;
}
int isWinner(int le){
    int w =    ((board[0] == le && board[1] == le && board[2] == le) ||
                (board[3] == le && board[4] == le && board[5] == le) ||
                (board[6] == le && board[7] == le && board[8] == le) ||
                (board[0] == le && board[3] == le && board[6] == le) ||
                (board[1] == le && board[4] == le && board[7] == le) ||
                (board[2] == le && board[5] == le && board[8] == le) ||
                (board[0] == le && board[4] == le && board[8] == le) ||
                (board[2] == le && board[4] == le && board[6] == le))?1:0;
    return w==1?1:0;
}
int isfull(char board[]){
    int empty =  ((board[0] == ' ' || board[1] == ' ' || board[2] == ' ' ) ||
                (board[3] == ' ' || board[4] == ' ' || board[5] == ' ' ) ||
                (board[6] == ' ' || board[7] == ' ' || board[8] == ' ' ))?1:0;
    return empty==1?1:0;
}

void playermove(char letter,int sockfd){
    
    char buff[255];
    snprintf(buff,sizeof(buff)," %c move \n Enter the position to place :\n",letter);
    write(sockfd,buff,strlen(buff));   

    int run = 1;
    while(run){
        if(strlen(buff)!=0){
            memset(buff,'\0',255);
        }
        int r = read(sockfd,buff,255);
        
        if(strlen(buff) > 1){
            char *token = strtok(buff,"\n");
            int m = atoi(token);
            printf("cbuff  :atoi:%d:m:%d:len:%ld: \n",atoi(token),m,strlen(token));
            
            if(m> 0 && m<= 9 && strlen(token)==1){
                if(isfreespace(m-1)==1){
                    run = 0;
                    insertLetter(letter,m-1);
                }
                else if(isfreespace(m-1)==0){
                    write(sockfd,"This place is already Occupied!\n",strlen("This place is already Occupied!\n"));
                    continue;
                }
            }
            else if(strlen(token)==1){
                write(sockfd,"Enter a valid position!\n",strlen("Enter a valid position!\n"));
                continue;
            }
        }
    }
}
int main_game(int sockid1,int sockid2){

    int flag = 0;           // marks if a winner is there or not
    reset();
    write(sockid1,"Commandline Tic-tac-toe !\n",strlen("Commandline Tic-tac-toe !\n"));
    write(sockid2,"Commandline Tic-tac-toe !\n",strlen("Commandline Tic-tac-toe !\n"));
    
    print();
    
    //printf("buffer::%s::\n",buffer);
    write(sockid1,buffer,strlen(buffer));
    write(sockid2,buffer,strlen(buffer));

    write(sockid1,"\n-- X --\n",strlen("\n-- X --\n"));
    write(sockid2,"\n-- O --\n",strlen("\n-- O --\n"));
    
    while(isfull(board)==1){
        if(isWinner('X')==0){
            print();
            write(sockid2,buffer,strlen(buffer));
            playermove('O',sockid2);
            print();
            write(sockid2,buffer,strlen(buffer));
        }
        else{
            print();
            write(sockid2,buffer,strlen(buffer));
            write(sockid1,"\nX is winner !!\n",strlen("\nX is winner !!\n"));
            write(sockid2,"\nX is winner !!\n",strlen("\nX is winner !!\n"));
            flag = 1;
            break;
        }
        if(isWinner('O')==0){
            print();
            write(sockid1,buffer,strlen(buffer));
            playermove('X',sockid1);
            print();
            write(sockid1,buffer,strlen(buffer));
        }
        else{
            print();
            write(sockid1,buffer,strlen(buffer));
            write(sockid2,"\nO is winner !!\n",strlen("\nO is winner !!\n"));
            write(sockid1,"\nO is winner !!\n",strlen("\nO is winner !!\n"));
            flag = 1;
            break;
        }
        }
    if(isfull(board)==0 && flag == 0){
        write(sockid1,"\n-----Tie Game-----\n",strlen("\n-----Tie Game-----\n"));
        write(sockid2,"\n-----Tie Game-----\n",strlen("\n-----Tie Game-----\n"));
        return 0;
    }
    return 0;
}