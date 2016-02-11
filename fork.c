//
//  main.c
//  fork
//
//  Created by 俆銘村 on 2015/11/5.
//  Copyright © 2015年 Villager. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main() {
    pid_t pid;
    
    pid = fork();
    
    if(pid<0){
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid==0){
        execlp("./ShareMemory",NULL);
    }
    else{
        wait(NULL);
        printf("Child Complete\n");
        exit(0);
    }
    
}
