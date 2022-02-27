#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sql.c"

#define MAX_LINE_LENGTH 4096

int searchEnebaPrice(char *path){

    char line[MAX_LINE_LENGTH] = {0};
    char search[11] = "\"lowPrice\"";
    int i,y,x,count,price;

    FILE *file = fopen(path, "r");
    
    if(!file){
        return -1;
    }

    while(fgets(line, MAX_LINE_LENGTH, file)){
        if( (strstr(line,search)) != 0 ){
            for(i = 0; i < MAX_LINE_LENGTH; i++){
                if(strstr(line,search)){
                    for(y = 0; y < MAX_LINE_LENGTH; y++){
                        if(line[y] == search[0]){
                            for(x = 0; x < sizeof(search); x++){
                                if(search[x] == line[y+x]){
                                    count = x;
                                } else {
                                    x = sizeof(search);
                                }
                            }
                            if(count == 9){
                                int start = y + sizeof(search);
                                // printf("%c %c %c\n", line[start], line[start+1], line[start+2]);
                                if(line[start+2] == 44 || line[start+2] == 46){
                                    price = ((line[start]-48)*10) + (line[start+1]-48);
                                } else if(line[start+1] == 44 || line[start+1] == 46){
                                    price = line[start]-48;
                                } else{
                                    price = ((line[start]-48)*100) + ((line[start+1]-48)*10) + (line[start+2]-48);
                                }
                                y = i = MAX_LINE_LENGTH;                            
                            }
                        }  
                    }
                }
            }
        }
    }

    if (fclose(file)){
        return EXIT_SUCCESS;
    };

    

    return price;
}