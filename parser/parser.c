#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "protein.h"

Atom * newAtom()
{
    Atom *atom = malloc(sizeof(*atom));
    atom->name = NULL;
    atom->coordinates = calloc(3, sizeof(double));
}

Residue * newResidue(unsigned long nr_atoms)
{
    Residue *residue = malloc(sizeof(*residue));
    residue->atoms = malloc(nr_atoms * sizeof(residue->atoms));
    residue->nr_atoms = nr_atoms;
    residue->name = NULL;
}

Protein * newProtein(unsigned long nr_residues, unsigned long nr_atoms)
{
    Protein *protein = malloc(sizeof(*protein));
    protein->residues = malloc(nr_residues * sizeof(protein->residues));
    protein->atoms = malloc(nr_atoms * sizeof(protein->atoms));
    protein->cAlphas = malloc(nr_residues * sizeof(protein->atoms));
    protein->nr_residues = nr_residues;
    protein->nr_atoms = nr_atoms;
    protein->name = NULL;
}

char *getProteinName(unsigned char *filecontent, unsigned long filesize)
{
    char *name;    
    unsigned char *ptr;
   
    ptr = filecontent;
    ptr = strstr(ptr, "COMPND   2");
    ptr = strstr(ptr, ":");
    name = ptr + 2;
    ptr = strstr(name, ";");
    *ptr = '\0';

    return name; 
}

void getInformation(char ** lines, int noflines, char** name, char** residues,  
                    float* coordinate1, float* coordinate2, float* coordinate3) 
{                                                                               
    int tokenLength = 0,                                                        
        idx = 0,                                                                
        count = 0;                                                              
    bool previous_space = false;                                                
    char * name_chars,                                                          
         * residues_chars,                                                      
         * coordinate1_chars,                                                   
         * coordinate2_chars,                                                   
         * coordinate3_chars;                                                   
                                                                                
                                                                                
    for(int i = 0; i < noflines; i++)                                           
    {                                                                           
        name_chars = malloc(sizeof(char)*10);                                   
        residues_chars = malloc(sizeof(char)*10);                               
        coordinate1_chars = malloc(sizeof(char)*10);                            
        coordinate2_chars = malloc(sizeof(char)*10);                            
        coordinate3_chars = malloc(sizeof(char)*10);                            
        for(int j = 0;lines[i][j] != '\n';j++)                                  
        {                                                                       
            if(lines[i][j] == ' ' && false == previous_space)                   
            {                                                                   
                count++;                                                        
                previous_space = true;                                          
                idx = 0;                                                        
            }                                                                   
            else if(lines[i][j] != ' ')                                         
            {                                                                   
                previous_space = false;                                         
                switch(count)                                                   
                {                                                               
                    case 2: name_chars[idx] = lines[i][j];                      
                            idx++;                                              
                            break;                                              
                    case 3: residues_chars[idx] = lines[i][j];                  
                            idx++;                                              
                            break;                                              
                    case 6: coordinate1_chars[idx] = lines[i][j];               
                            idx++;                                              
                            break;                                              
                    case 7: coordinate2_chars[idx] = lines[i][j];               
                            idx++;                                              
                            break;                                              
                    case 8: coordinate3_chars[idx] = lines[i][j];               
                            idx++;                                              
                            break;                                              
                }                                                               
            }                                                                   
        }                                                                       
        printf("%s\n",name_chars);                                              
        name[i] = name_chars;                                                   
        residues[i] = residues_chars;                                           
        sscanf(coordinate1_chars,"%f",&coordinate1[i]);                         
        sscanf(coordinate2_chars,"%f",&coordinate2[i]);                         
        sscanf(coordinate3_chars,"%f",&coordinate3[i]);                         
        free(name_chars);                                                       
        free(residues_chars);                                                   
        free(coordinate1_chars);                                                
        free(coordinate2_chars);                                                
        free(coordinate3_chars);                                                
    }                                                                           
    printf("%s %s %.2f %.2f %.2f\n",name[0],residues[0],coordinate1[0],
           coordinate2[0],coordinate3[0]);
}                                                                                             

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

    /*count the number of relevant lines (starting with "ATOM")*/
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

    //get name of the protein 
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

