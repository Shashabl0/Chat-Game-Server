#include<stdio.h>
#include<string.h>


int main(){

    char buffer[30] = "hey three";

    char c[10] = "h\nb";

    printf(" ::%s:: ",c);
    
    memset(c,'\0',10);
    //char *token = strtok(c,"\n");
    
    printf(" ::%ld:: ",strlen(c));
    return 0;
}