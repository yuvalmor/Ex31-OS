// Yuval Mor - 205380173

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define END_OF_FILE 0
#define FIRST_FILE_NAME 1
#define NUMBER_BYTES_TO_READ 1
#define IDENTITY 1
#define SECOND_FILE_NAME 2
#define DIFFERENT 2
#define SAME 3
#define NUMBER_OF_ELEMENTS 3
#define SYSTEM_CALL_FAILURE (-1)
#define DIFFERENCE_IN_ASCII 32
#define A_VALUE_ASCII 65
#define Z_VALUE_ASCII 122
#define ERROR_MESSAGE "Error in system call\n"

void ExitProgram(){
    fprintf(stderr,ERROR_MESSAGE);
    exit(EXIT_FAILURE);
}

int IsIdentical(const char* firstFileName, const char* secondFileName){
    bool identity = true;
    bool different = false;
    bool flag = true;
    bool read1 = true;
    bool read2 = true;
    int fd1,fd2;
    ssize_t r1=0;
    ssize_t r2=0;
    char c1="";char c2 ="";
    fd1 = open(firstFileName,O_RDONLY);
    fd2 = open(secondFileName,O_RDONLY);
    if(fd1==SYSTEM_CALL_FAILURE || fd2==SYSTEM_CALL_FAILURE){
        ExitProgram();
    }
    while (flag && !different){
        if(read1){
            r1 = read(fd1,&c1,NUMBER_BYTES_TO_READ);
        }
        if(read2){
            r2 = read(fd2,&c2,NUMBER_BYTES_TO_READ);
        }
        // Checks failure in the system call
        if(r1 == SYSTEM_CALL_FAILURE || r2== SYSTEM_CALL_FAILURE){
        ExitProgram();
        }
        // Checks if the file ended
        if(r1==END_OF_FILE || r2==END_OF_FILE){
            flag = false;
            continue;
        }
        if(c1==c2) {
            read1=true;
            read2=true;
            continue;
        } else {
            identity = false;
            if(isspace(c1)){
                read2 = false;
                continue;
            }
            if(isspace(c2)){
                read1 =false;
                continue;
            }
            if(c1>=A_VALUE_ASCII && c2>=A_VALUE_ASCII && c1 <=Z_VALUE_ASCII
            && c2<=Z_VALUE_ASCII && abs(c1-c2)== DIFFERENCE_IN_ASCII){
                read1=true;
                read2=true;
                continue;
            }
            different=true;
        }
    }
    //
    if(r1==END_OF_FILE && r2!=END_OF_FILE){
        do{
            if(!isspace(c2)){
                different=true;
            }
            r2 = read(fd2,&c2,NUMBER_BYTES_TO_READ);
            identity =false;
        }while (r2!=END_OF_FILE);
    }
    if(r2==END_OF_FILE && r1!=END_OF_FILE){
        do{
            if(!isspace(c1)){
                different=true;
            }
            r1 = read(fd1,&c1,NUMBER_BYTES_TO_READ);
            identity =false;
        }while (r1!=END_OF_FILE);

    }
    if(different){
        return DIFFERENT;
    }
    if(identity){
        return IDENTITY;
    }
    return SAME;
}

int main(int argc, char* argv[]) {
    if(argc!= NUMBER_OF_ELEMENTS){
        printf("invalid input\n");
        return -1;
    }
    int result = IsIdentical(argv[FIRST_FILE_NAME],argv[SECOND_FILE_NAME]);
    printf("the result is = %d",result);
    return result;
}