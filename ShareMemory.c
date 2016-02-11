//
//  main.c
//  ShareMemory
//
//  Created by 俆銘村 on 2015/11/4.
//  Copyright © 2015年 Villager. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>




int int_to_binary(int k) {
    if (k == 0) return 0;
    if (k == 1) return 1;                       /* optional */
    return (k % 2) + 10 * int_to_binary(k / 2);
}

int count1s(int k){ // count how many 1's
    int count=0;
    while(k!=0){
        if(k%2==1)
            count++;
        k=k/2;
    }
    return count;
}

int B() {
    const int SIZE = 4096;
    const char *name = "OS";
    int shm_fd;
    void *ptr;
    int i,result;
    static int binary[1000];
    static int ten[1000];
    static int  count[1000];
    static int index=0;
    // read from SM and convert to bunary
    shm_fd = shm_open(name, O_RDONLY,0666);
    if(shm_fd == -1){
        printf("shared memory failed\n");
        exit(-1);
    }
    
    
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if(ptr == MAP_FAILED){
        printf("Map Failed\n");
        exit(-1);
    }
    
    
    i = atoi(ptr);
    
    if(i==-1){ //取最大輸出
        int max = 0;
        for(int b=0;b<index;b++){
            if(count[b]>max){
                max=count[b];
            }
        }
        shm_fd = shm_open(name, O_CREAT|O_RDWR,0666);
        
        
        ftruncate(shm_fd, SIZE);
        ptr = mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if(ptr == MAP_FAILED){
            printf("Map Failed\n");
            return -1;
        }
        
        for(int c=0;c<index;c++){
            if(count[c]==max){
                sprintf(ptr,"%d:%d;",ten[c],binary[c]);
                ptr+=strlen(ptr);
            }
        }
        
    }else if(i==-2){  //取最小輸出
        int min=count[0];
        for(int j=1;j<index-1;j++){
            if(min>count[j]){
                min=count[j];
            }
        }
        
        shm_fd = shm_open(name, O_CREAT|O_RDWR,0666);
        
        ftruncate(shm_fd, SIZE);
        ptr = mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if(ptr == MAP_FAILED){
            printf("Map Failed\n");
            return -1;
        }
        for(int g=0;g<index-1;g++){
            if(count[g]==min){
                sprintf(ptr,"%d:%d;",ten[g],binary[g]);
                ptr+=strlen(ptr);
            }
        }
        
    }
    else{
        
        result= int_to_binary(i);
        
        
        
        //write result back to SM
        
        shm_fd = shm_open(name, O_CREAT|O_RDWR,0666);
        
        
        ftruncate(shm_fd, SIZE);
        ptr = mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if(ptr == MAP_FAILED){
            printf("Map Failed\n");
            return -1;
        }
        
        
        sprintf(ptr,"%d:%d;",i,result);
        
        //store all int and binary
        ten[index]=i;
        binary[index]=result;
        
        
        
        //store the binary to string (for compare later)
        
        count[index] = count1s(i);
        
        
        index++;
        
    }
    
    
    
    return 0;
}



int main() {
    const int SIZE = 4096;
    const char *name = "OS";
    int params;
    int shm_fd;
    void *ptr;
    
    while(1){
        //write input number to SM
        shm_fd = shm_open(name, O_CREAT|O_RDWR,0666);
        
        
        ftruncate(shm_fd, SIZE);
        ptr = mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if(ptr == MAP_FAILED){
            printf("Map Failed\n");
            return -1;
        }
        
        printf("Enter an integer:");
        scanf("%d",&params);
        sprintf(ptr, "%d" ,params);
        
        // call B
        B();
        
        
        
        printf("%s\n",ptr);
        
    }
    
    return 0;
    
    
}
