// Yuval Mor - 205380173

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define ERROR (-1)
#define SYSTEM_CALL_FAILURE (-1)
#define END_OF_FILE 0
#define INITIAL_VALUE 0
#define FIRST_FILE_NAME 1
#define NUMBER_BYTES_TO_READ 1
#define IDENTITY 1
#define SECOND_FILE_NAME 2
#define DIFFERENT 2
#define SAME 3
#define NUMBER_OF_ELEMENTS 3
#define DIFFERENCE_IN_ASCII 32
#define A_VALUE_ASCII 65
#define Z_VALUE_ASCII 122
#define ERROR_MESSAGE "Error in system call\n"
#define INPUT_ERROR "invalid input\n"


/**
 * The function EndProgram - write the error message (as a result from system call failure),
 * To file descriptor 2 (stderr), and exit the program.
 */
void EndProgram(){
    write(STDERR_FILENO,ERROR_MESSAGE,strlen(ERROR_MESSAGE));
    exit(ERROR);
}

/**
 * The function Compare - get two paths for two files,
 * And compare between them.
 * @param firstFilePath- path to the first file
 * @param secondFilePath- path to the second file
 * @return - 1 if they identical, 2 if they different and 3 if they similar.
 */
int Compare(const char* firstFilePath, const char* secondFilePath){
    // File directory to the files
    int fd1,fd2;
    // The character we read from each file
    char c1,c2;
    // Number of bytes that was read
    ssize_t r1=INITIAL_VALUE;
    ssize_t r2=INITIAL_VALUE;
    bool flag = true;
    bool identity = true;
    bool different = false;
    bool readFromFirstFile = true;
    bool readFromSecondFile = true;
    // Open the files
    fd1 = open(firstFilePath,O_RDONLY);
    fd2 = open(secondFilePath,O_RDONLY);
    // Check the system call open
    if(fd1<INITIAL_VALUE || fd2<INITIAL_VALUE){
        EndProgram();
    }
    /*
     * Keeps running as long as the one of the files didn't end,
     * And they don't different.
     */
    while (flag && !different){
        // Read character from the first file
        if(readFromFirstFile){
            r1 = read(fd1,&c1,NUMBER_BYTES_TO_READ);
        }
        // Read the next char from the second file
        if(readFromSecondFile){
            r2 = read(fd2,&c2,NUMBER_BYTES_TO_READ);
        }
        // Checks the system call read
        if(r1 == SYSTEM_CALL_FAILURE || r2== SYSTEM_CALL_FAILURE){
            close(fd1);
            close(fd2);
            EndProgram();
        }
        // Checks if one of the files ended
        if(r1==END_OF_FILE||r2==END_OF_FILE){
            flag = false;
            continue;
        }
        // Compare between the characters from the files
        if(c1==c2) {
            readFromFirstFile=true;
            readFromSecondFile=true;
            continue;
        } else {
            identity = false;
            /**
             * Checks if one of the character is white space,
             * And if it is, it keeps reading only from the file
             * With the white space character
             */
            if(isspace(c1)){
                readFromSecondFile = false;
                continue;
            }
            if(isspace(c2)){
                readFromFirstFile =false;
                continue;
            }
            if(c1>=A_VALUE_ASCII && c2>=A_VALUE_ASCII && c1 <=Z_VALUE_ASCII
               && c2<=Z_VALUE_ASCII && abs(c1-c2)== DIFFERENCE_IN_ASCII){
                readFromFirstFile=true;
                readFromSecondFile=true;
                continue;
            }
            different=true;
        }
    }

    /**
     * In case that one of the files ended,and the other one didn't,
     * It checks if the other file context assembled from white space only.
     */
    if(r1==END_OF_FILE && r2!=END_OF_FILE && !different){
        identity=false;
        do{
            if(!isspace(c2)){
                different=true;
                break;
            }
            r2 = read(fd2,&c2,NUMBER_BYTES_TO_READ);
        }while (r2!=END_OF_FILE);
    }
    if(r2==END_OF_FILE && r1!=END_OF_FILE && !different){
        identity=false;
        do{
            if(!isspace(c1)){
                different=true;
                break;
            }
            r1 = read(fd1,&c1,NUMBER_BYTES_TO_READ);
        }while (r1!=END_OF_FILE);

    }
    // Close the files.
    if(close(fd1)==SYSTEM_CALL_FAILURE || close(fd2)==SYSTEM_CALL_FAILURE){
        EndProgram();
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
        printf(INPUT_ERROR);
        return ERROR;
    }
    int result = Compare(argv[FIRST_FILE_NAME],argv[SECOND_FILE_NAME]);
    return result;
}