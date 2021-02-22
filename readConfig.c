#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readConfig.h"

#define MAX_LINE_LENGTH 20

int readFile(char *filename, int linenumber)
{
    char line[MAX_LINE_LENGTH] = {0};
    unsigned int line_count = 1;
	FILE *fp = fopen(filename, "r");
	if(fp == NULL) {
        perror("Unable to open file!");
        exit(1);
     }
    while (fgets(line, MAX_LINE_LENGTH, fp))
    {
        if(line_count == linenumber) {
            int x = atoi(line);
            return x;
        }
        line_count += 1;
        
    }
    return 0;

}
