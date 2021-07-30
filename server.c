#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include "game.h"

const int CLIENT_LIMIT=3;       // check below CLient and thread limit;
int active = 0;
int disconnt = 0;
int flag = 0;

struct client{
    int id;
    int sockid;
    struct sockaddr_in clientAddr;
    int len;
    int connect;
};
struct client Client[3];        //change here to increase limit
pthread_t thread[3];            //change here to increase limit



//*************************
//*************************
// add func to read and write 
//*************************
//*************************








void * Comm(void *ClientDet){
    struct client* clientDetail = (struct client*)ClientDet;
    int clientsocket = clientDetail->sockid;

    printf("Thread ID %ld\n",pthread_self());
    printf("    Client %d is connected\n",clientDetail->id);
    
    char options[]="\n   CHAT SERVERv1.2\nyou have options:\n LIST: lists active clients\n SEND: to send msg to any client\n QUIT: to close connection\n SELF: self id\n PLAY: play tictactoe\n\n";
    
    int w = write(clientsocket,options,strlen(options));

    int t = 1;  //counter for default case
    
    char databuffer[20];
    while(1){
        memset(databuffer,'\0',20);  
        int r = read(clientsocket,databuffer,20);
        if(r<0)
            perror("option read failed");
        if(strncmp("LIST",databuffer,4)==0){                        //LIST
            write(clientsocket,"Listing Client Avaiable\n",1024);
            char buffer[50];
            for(int j=0;j<active;j++){
                if(Client[j].sockid == -1)
                    continue;
                if( Client[j].id != clientDetail->id ){        //client Id here
                    snprintf (buffer, sizeof(buffer), "Client %d\n",Client[j].id);
                    write(clientsocket,buffer,1024);
                    buffer[0]='\0';
                }
            }
            if(active-1 == disconnt){
                write(clientsocket,"No other Client available\n",1024);
            }
        }
        else if(strncmp("SEND",databuffer,4)==0){                   //SEND
            // insert msg function
            char dbuffer[255];
            int i=0;
            write(clientsocket,"(to)\n",strlen("(to)\n"));
            
            dbuffer[0]='\0';
            while(1){
                read(clientsocket,dbuffer,1024);
                if(strlen(dbuffer) !=0)
                    break;
            }
            for(i=0;i<active;i++){
                if(Client[i].sockid == -1)
                    continue;
                if(Client[i].id == atoi(dbuffer) && clientDetail->id != atoi(dbuffer)){
                    printf("Client Available !!\n");
                    break;
                }
            }
            if(Client[i].id == atoi(dbuffer) && i!=active){
                    dbuffer[0]='\0';
                    write(Client[i].sockid,"Someone wants to connect! type YES/NO:\n",strlen("Someone wants to connect! type YES/NO:\n"));
                    Client[i].connect = clientsocket;

                    while(Client[i].connect){
                        dbuffer[0] = '\0';
                        r = read(clientsocket,dbuffer,1024);

                        if(strlen(dbuffer)!=0){
                            write(Client[i].sockid,dbuffer,1024);
                        }

                        if(strncmp("bye",dbuffer,3)==0){
                            write(clientsocket,"*Connection closed now*\n",strlen("*Connection closed now*\n"));
                            write(Client[i].sockid,"*Connection closed now* type bye to exit\n",strlen("*Connection closed now* type bye to exit\n"));
                            printf("connection closed\n");
                            Client[i].connect = 0;
                            break;
                        }
                    }
            }
            else{
                perror("no Client of that id present !!");
            }
        }
        else if(strncmp("SELF",databuffer,4)==0){                       //SELF
            char buffer[100];
            snprintf (buffer, sizeof(buffer), " ID - %d\n",clientDetail->id);
            write(clientsocket,buffer,strlen(buffer));
            buffer[0]='\0';
        }
        else if(strncmp("QUIT",databuffer,4)==0){                       //QUIT
            write(clientsocket,"QUIT",1024);
            clientDetail->sockid = -1;
            disconnt++;
            printf("\nAvailable clients %d\n",active-disconnt);
            break;
        }
        else if(strncmp("YES",databuffer,3)==0){                        //YES
            printf("YES Mode On *add evil smile*\n");
            write(clientDetail->connect,"Connected..\n",strlen("Connected..\n"));
            char buffer[1024];
            while(Client[clientDetail->id].connect!=0){
                buffer[0]='\0';
                r = read(clientsocket,buffer,1024);
                if(strlen(buffer)!=0)
                    write(clientDetail->connect,buffer,1024);
                if(strncmp("bye",buffer,3)==0){
                    printf("connection closed\n");
                    write(clientsocket,"*Connection closed now*\n",strlen("*Connection closed now*\n"));
                    write(clientDetail->connect,"*Connection closed now* type bye to exit\n",strlen("*Connection closed now* type bye to exit\n"));
                    Client[clientDetail->id].connect = 0;
                    break;
                }
            }
        }
        else if(strncmp("NO",databuffer,2)==0){                         //NO
            printf("NO Mode\n");
            write(clientDetail->connect,"Connection refused..\n",strlen("Connection refused.."));
            write(clientsocket,"*Connection closed now*\n",strlen("*Connection closed now*\n"));
            write(clientDetail->connect,"*Connection closed now*\n",strlen("*Connection closed now*\n"));    
            printf("connection closed\n");
            Client[clientDetail->id].connect = 0;
        }
        else if(strncmp("PLAY",databuffer,4)==0){
            // insert msg function
            char dbuffer[255];
            int i=0;
            write(clientsocket,"(to)\n",1024);
            
            dbuffer[0]='\0';
            while(1){
                read(clientsocket,dbuffer,1024);
                if(strlen(dbuffer) !=0)
                    break;
            }
            for(i=0;i<active;i++){
                if(Client[i].sockid == -1)
                    continue;
                if(Client[i].id == atoi(dbuffer) && clientDetail->id != atoi(dbuffer)){
                    printf("Client Available !!\n");
                    break;
                }
            }
            if(Client[i].id == atoi(dbuffer) && i!=active){
                    dbuffer[0]='\0';
                    write(Client[i].sockid,"Someone wants to PLAY Tic-Tac-Toe! type GO:\n",strlen("Someone wants to PLAY Tic-Tac-Toe! type GO:\n"));
                    Client[i].connect = clientsocket;

                    int n=2;
                    while(Client[i].connect != 0 && n ==2){
                        /*dbuffer[0] = '\0';
                        r = read(clientsocket,dbuffer,1024);

                        if(strncmp("bye",dbuffer,3)==0){
                            write(clientsocket,"*Connection closed now*\n",strlen("*Connection closed now*\n"));
                            write(Client[i].sockid,"*Connection closed now*\n",strlen("*Connection closed now*\n"));
                            printf("connection closed\n");
                            Client[i].connect = 0;
                            break;
                        }
                        */
                        n = main_game(Client[i].sockid,clientsocket);
                    }
                    if(n == 0){
                        write(clientsocket,"*Connection closed now*\n",strlen("*Connection closed now*\n"));
                        write(Client[i].sockid,"*Connection closed now*\n",strlen("*Connection closed now*\n"));
                        printf("connection closed\n");
                        Client[i].connect = 0;
                    }
            }
            else{
                perror("no Client of that id present !!");
            }


        }/*
        else if(strncmp("GO",databuffer,3)==0){
            //********************************YES*****************************************************
            printf("GO Mode On *add evil smile*\n");
            write(clientDetail->connect,"Connected..\n",strlen("Connected..\n"));
            //need edit
            }*/
        else if(strncmp("help",databuffer,4)==0){
            memset(options, '\0', sizeof(options));
            strcat(options," \n you have options:\n LIST: lists active clients\n SEND: to send msg to any client\n QUIT: to close connection\n SELF: self id\nPLAY: play tictactoe\n\n");
            write(clientsocket,options,strlen(options));
        }
        databuffer[0]='\0';
    }
    return NULL;
}

int main(int argc,char *argv[]){
    
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    struct sockaddr_in serverAddr;
    bzero((char *) &serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(9800);

    if(bind(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr))<0)
        return -1;

    if(listen(sockfd,2)<0)
        return -1;
    
    printf(" Server Started on port 9800..\n");
    printf(" Current Client limit set %d\n",CLIENT_LIMIT);
    
    
    for(int i=0;i<CLIENT_LIMIT && flag==0;i++){    
        Client[i].sockid = accept(sockfd,(struct sockaddr *)&Client[i].clientAddr,&Client[i].len);

        pthread_create(&thread[i],NULL,Comm,(void *) &Client[i]);
        Client[i].id = i;//thread[i]%1000;
        Client[i].connect = 0;
        active++;
        if(active - disconnt == 0){
            break;
        }
    }

    for(int i=0;i<active;i++){
        pthread_join(thread[i],NULL);
    }

	close(sockfd);
    return 0;
}
