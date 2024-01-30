#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void print_command(int argc, char * const argv[]){
    for(int i=0;i<argc;i++){
        fprintf(stdout,"%s ", argv[i]);
    }
    fprintf(stdout,"\n");
}

int main(int argc, char *argv[]){
    if(argc<2){
        perror("Please enter a valid command!");
        return 1;
    }

    if(strcmp(argv[1],"init") == 0){
        print_command(argc,argv);
        
    }

    return 0;
}