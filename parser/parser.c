#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "protein.h"

Atom * newAtom()
{
    Atom *atom = malloc(sizeof(*atom));
    atom->name = NULL;
    atom->x = 0;
    atom->y = 0;
    atom->z = 0;
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
                                                                                
    /*
    * going through every line   
    */                                                                        
    for(int i = 0; i < noflines; i++)                                           
    {
        /*
        * allocating the help-array where the relevant chars of one line 
        * are saved in
        */                                                                           
        name_chars = malloc(sizeof(char)*10);                                   
        residues_chars = malloc(sizeof(char)*10);                               
        coordinate1_chars = malloc(sizeof(char)*10);                            
        coordinate2_chars = malloc(sizeof(char)*10);                            
        coordinate3_chars = malloc(sizeof(char)*10);
        /*
        * going through every character of one line
        */                            
        for(int j = 0;lines[i][j] != '\n';j++)                                  
        { 
            /*
            * dividing one line into several tokens divided by spaces
            */                                                                      
            if(lines[i][j] == ' ' && false == previous_space)                   
            {                                                                   
                count++;                                                        
                previous_space = true;                                          
                idx = 0;                                                        
            } 
            /*
            * checking every character that isn't space
            */                                                                  
            else if(lines[i][j] != ' ')                                         
            {                                                                   
                previous_space = false;                                         
                /*
                * checking if a character belongs to a relevant 
                * information part
                */
                switch(count)                                                   
                {                                                               
                    case 2: name_chars[idx] = lines[i][j];                      
                            name_chars[idx+1] = '\0';
                            idx++;                                              
                            break;                                              
                    case 3: residues_chars[idx] = lines[i][j]; 
                            residues_chars[idx+1] = '\0';
                            idx++;                                              
                            break;                                              
                    case 6: coordinate1_chars[idx] = lines[i][j];  
                            coordinate1_chars[idx+1] = '\0';
                            idx++;                                              
                            break;                                              
                    case 7: coordinate2_chars[idx] = lines[i][j];  
                            coordinate2_chars[idx+1] = '\0';
                            idx++;                                              
                            break;                                              
                    case 8: coordinate3_chars[idx] = lines[i][j];       
                            coordinate3_chars[idx+1] = '\0';
                            idx++;                                              
                            break;                                              
                }                                                               
            }                                                                   
        }         
        /*
        * copying the help arrays as one token into the information arrays
        */                                                              
        strcpy(name[i],name_chars);
        strcpy(residues[i],residues_chars);
        sscanf(coordinate1_chars,"%f",&coordinate1[i]);                         
        sscanf(coordinate2_chars,"%f",&coordinate2[i]);                         
        sscanf(coordinate3_chars,"%f",&coordinate3[i]);                         
        /*
        * freeing the helping arrays
        */
        free(name_chars);                                                       
        free(residues_chars);                                                   
        free(coordinate1_chars);                                                
        free(coordinate2_chars);                                                
        free(coordinate3_chars);                                                
        count = 0;
    }     
    /*
    * only relevant for testing
    */                                                                      
    printf("%s %s %.2f %.2f %.2f\n",name[0],residues[0],coordinate1[0],
           coordinate2[0],coordinate3[0]);
    printf("%s %s %.2f %.2f %.2f\n",name[1],residues[1],coordinate1[1],
           coordinate2[1],coordinate3[1]);
    printf("%s %s %.2f %.2f %.2f\n",name[2],residues[2],coordinate1[2],
           coordinate2[2],coordinate3[2]);


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

int main(int argc, char * argv[])
{
    int noflines = 3;
    char **lines;
    char ** names = malloc(3*sizeof(char*));
    float coordinate1[3];
    float coordinate2[3];
    float coordinate3[3];
    char ** residues = malloc(3*sizeof(char));
    lines = malloc(3*sizeof(char*));
    for(int idx = 0; idx < 3; idx++)
    {
        names[idx] = malloc(10*sizeof(char));
        residues[idx] = malloc(10*sizeof(char));
    }
    for(int idx = 0; idx < 3; idx++)
    {
        lines[idx] = "ATOM      3  C   VAL A   1      36.011  48.598   3.272  1.00 57.04           C"; 
    }
    getInformation(lines,noflines,names,residues,coordinate1,coordinate2,coordinate3);
}




