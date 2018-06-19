#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*void getInformation(char ** lines, int noflines, char* name,                    
                    float* coordiante1, float* coordinate2, float* coordiante3) 
{                                                                               
    int tokenLength = 0,                                                        
        lineLength,                                                             
        k = 0;                                                                  
    char ** token;                                                              
    token = malloc(sizeof(token));                                              
    for(int i = 0; i < noflines; i++)                                           
    {                                                                           
        for(int j = 0;lines[i][j] != '\n';j++)                                  
        {                                                                       
            if(lines[i][j] == ' ' && tokenLength != 0)                          
            {                                                                   
                token[k] = malloc((tokenLength)*sizeof(char));                  
                token[k] = &lines[i][j-tokenLength];                            
                lines[i][j] = '\0';                                             
                tokenLength = 0;                                                
                k++;                                                            
            }                                                                   
            else if(tokenLength != 0)                                           
            {                                                                   
                tokenLength++;                                                  
            }                                                                   
        }                                                                       
        name[i] = token[1];                                                     
        coordinate1[i] = (float)token[5];                                      
        coordinate2[i] = (float)token[6];                                      
        coordinate3[i] = (float)token[7];                                      
        k = 0;                                                                  
    }                                                                           
                                                                                
}*/  

unsigned char * readFromFile(const char *filename, unsigned long *filesize)
{
    FILE *fp = fopen(filename, "r");
    unsigned long fs;
    unsigned char *filecontent;
    
    if(fp == NULL)
    {
        fprintf(stderr, "%s: cannot open file \"%s\"\n", __func__, filename);
        return NULL;
    }
    if(fseek(fp, 0, SEEK_END) != 0)
    {
        fprintf(stderr, "%s: fseek failed\n", __func__);
    }
    fs = ftell(fp);
    rewind(fp);
    filecontent = malloc(sizeof(*filecontent) * fs);
    if(fread(filecontent, sizeof(*filecontent), fs, fp) != fs)
    {
        fprintf(stderr, "%s: fread failed\n", __func__);
        return NULL;
    }
    *filesize = fs;
    fclose(fp);

    return filecontent;
}

unsigned char ** getRelevantLines(unsigned char *filecontent, 
                                  unsigned long *nLines)
{
    unsigned long idx, n;
    unsigned char **lines, *ptr;

    ptr = filecontent;
    n = 0;

    /*count the number of relevant lines*/
    while((*ptr != '\0') && (strstr(ptr, "\nATOM") != NULL))
    {
        ptr = strstr(ptr, "\nATOM") + 1;
        ++n;
    }
    ptr = filecontent;
    lines = malloc(n * sizeof(*lines));
    idx = 0;
   
    /*save the relevant lines*/ 
    while((*ptr != '\0') && (strstr(ptr, "\nATOM") != NULL))
    {
        lines[idx] = strstr(ptr, "\nATOM");
        ++lines[idx];
        ptr = lines[idx];
        ++idx;
    }

    /*terminate the individual lines with '\0'*/
    for(idx = 0; idx < n; ++idx)
    {
        ptr = lines[idx];
        ptr = strstr(ptr, "\n");
        *ptr = '\0';
    }

    *nLines = n; 
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

