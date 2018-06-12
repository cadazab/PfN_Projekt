#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char * readFromFile(const char *filename, unsigned long *filesize)
{
    FILE *fp = fopen(filename, "r");
    unsigned long fs;
    unsigned char *filecontent;
    
    if(fp == NULL)
    {
        fprintf(stderr, "%s: cannot open file \"%s\"\n", __func__, filename);
    }
    fseek(fp, 0, SEEK_END);
    fs = ftell(fp);
    rewind(fp);
    filecontent = malloc(sizeof(*filecontent) * fs);
    *filesize = fs;

    return filecontent;
}

unsigned char ** getRelevantLines(unsigned char *filecontent, 
                                  unsigned long *nLines)
{
    unsigned long idx;
    unsigned char **lines, *ptr;
    
    ptr = filecontent;
    lines = malloc(sizeof(lines));
    idx = 0;
    while(*ptr != '\0' && strstr(ptr, "\nATOM") != NULL)
    {
        lines[idx] = strstr(ptr, "\nATOM") + 1;
        ptr = lines[idx];
        idx++;
        ptr = strstr(ptr, "\n");
        *ptr = '\0';
        ptr++;
    }
    *nLines = idx;
    
    return lines;
}

void parse(char *filename)
{
    unsigned long nLines, filesize, idx;
    unsigned char *filecontent, **lines;
    
    filecontent = readFromFile(filename, &filesize);
    lines = getRelevantLines(filecontent, &nLines);
    
    //get relevant information
    //Write information into structs
    //funtion to free memory of structs

    free(filecontent);
    for(idx = 0; idx < nLines; idx++)
    {
        free(lines[idx]);
    }
    free(lines);
}

