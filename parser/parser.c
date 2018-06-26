#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "protein.h"

Atom * newAtom(char *name, double x, double y, double z)
{
    Atom *atom = malloc(sizeof(*atom));
    atom->name = malloc(5 * sizeof(char));
    strcpy(atom->name, name);
    atom->x = 0;
    atom->y = 0;
    atom->z = 0;
    return atom;
}

Residue * newResidue(char *name, unsigned long nr_atoms)
{
    Residue *residue = malloc(sizeof(*residue));
    residue->atoms = malloc(nr_atoms * sizeof(residue->atoms));
    residue->name = malloc(4 * sizeof(char));
    strcpy(residue->name, name);
    residue->nr_atoms = nr_atoms;
    return residue;
}

Protein * newProtein(char *name, unsigned long nr_residues, unsigned long nr_atoms)
{
    Protein *protein = malloc(sizeof(*protein));
    protein->residues = malloc(nr_residues * sizeof(protein->residues));
    protein->atoms = malloc(nr_atoms * sizeof(protein->atoms));
    protein->cAlphas = malloc(nr_residues * sizeof(protein->atoms));
    protein->name = malloc(50 * sizeof(char));
    strcpy(protein->name, name);
    protein->nr_residues = nr_residues;
    protein->nr_atoms = nr_atoms;
    return protein;
}

char *getProteinName(char *filecontent, unsigned long filesize)
{
    char *name;    
    char *ptr;
   
    ptr = filecontent;
    ptr = strstr(ptr, "COMPND   2");
    ptr = strstr(ptr, ":");
    name = ptr + 2;
    ptr = strstr(name, ";");
    *ptr = '\0';

    return name; 
}

void getInformation(const char ** lines, const int noflines, char** name,
                    char** residues, double* coordinate1,
                                     double* coordinate2, 
                                     double* coordinate3) 
{                                                                               
    int idx = 0,                                                                
        count = 0;                                                              
    unsigned long name_length = 0, residues_length = 0;
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
                            name_length++;
                            idx++;                                              
                            break;                                              
                    case 3: residues_chars[idx] = lines[i][j]; 
                            residues_chars[idx+1] = '\0';
                            residues_length++;
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
        name[i] = malloc((name_length+1)*sizeof(char));
        residues[i] = malloc((residues_length+1)*sizeof(char));                          
        strcpy(name[i],name_chars);
        strcpy(residues[i],residues_chars);
        sscanf(coordinate1_chars,"%lf",&coordinate1[i]);                         
        sscanf(coordinate2_chars,"%lf",&coordinate2[i]);                         
        sscanf(coordinate3_chars,"%lf",&coordinate3[i]);                         
        /*
        * freeing the helping arrays
        */
        free(name_chars);                                                       
        free(residues_chars);                                                   
        free(coordinate1_chars);                                                
        free(coordinate2_chars);                                                
        free(coordinate3_chars);                                                
        count = 0;
        name_length = 0;
        residues_length = 0;
    }   
    /*
    * only relevant for testing
    */
    for(int i = 0; i < noflines;i++)
    {
        printf("%s %s %.2f %.2f %.2f\n",name[i],residues[i],coordinate1[i],
               coordinate2[i],coordinate3[i]);
    }  


}                                                                                             

char * readFromFile(const char *filename, unsigned long *filesize)
{
    FILE *fp = fopen(filename, "r");
    unsigned long fs;
    char *filecontent;
    
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

char ** getRelevantLines(char *filecontent, unsigned long *nLines)
{
    unsigned long idx, n;
    char **lines, *ptr;

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

unsigned long *getResidueLengths(char **residues, unsigned long nLines,
                                 unsigned long *nofresidues)
{
    unsigned long atom_idx, res_idx, nResidues, *res_lengths;
    char *temp;
   
    temp = residues[0];
    nResidues = 1;
    for(atom_idx = 1; atom_idx < nLines; ++atom_idx)
    {
        if(strcmp(residues[atom_idx], temp))
        {
            ++nResidues;
            temp = residues[atom_idx];
        }
    }
    res_lengths = calloc(nResidues, sizeof(unsigned long));
    temp = residues[0];
    res_idx = 0;
    for(atom_idx = 0; atom_idx < nLines; ++atom_idx)
    {
        if(!strcmp(residues[atom_idx], temp))
        {
            ++res_lengths[res_idx]; 
        }
        else
        {
            temp = residues[atom_idx];
            ++res_idx;
            ++res_lengths[res_idx];
        }
    }
    *nofresidues = nResidues;

    return res_lengths;
}


/*
 * Protein * writeInfoIntoStructs(char **name, char **residues, char *protein_name,
                               unsigned long nLines, double* coordinate1,
                                                     double* coordinate2,
                                                     double* coordinate3)
{
    unsigned long nofatoms, nofresidues, atom_idx, res_idx, *res_lengths;
    char *temp;

    Protein protein;    
    nofatoms = nLines;
    res_lengths = getResidueLengths(residues, nLines, &nofresidues);
    protein = newProtein(protein_name, nofresidues, nofatoms);
    
    for(atom_idx = 0; atom_idx < nofatoms; ++atom_idx)
    {
        protein->atoms[atom_idx] = newAtom(name[atom_idx][0],
                                           coordinate1[atom_idx], 
                                           coordinate2[atom_idx],
                                           coordinate3[atom_idx]);
    }

    atom_idx = 0;
    for(res_idx = 0; res_idx < nofresidues; ++res_idx)
    {
        protein->residues[res_idx] = newResidue(residues[atom_idx],
                                                res_lengths[res_idx]);
        protein->residues[res_idx]->atoms = protein->atoms[atom_idx];
        atom_idx += res_lengths[res_idx];
    }
}
*/

void parse(char *filename)
{
    unsigned long nLines, filesize, idx;
    char *filecontent, *protein_name, **lines, **name, **residues;
    double *coordinate1, *coordinate2, *coordinate3;

    filecontent = readFromFile(filename, &filesize);
    lines = getRelevantLines(filecontent, &nLines);

    name = malloc(nLines * sizeof(char*));
    residues = malloc(nLines * sizeof(char*));
    coordinate1 = malloc(nLines * sizeof(double));    
    coordinate2 = malloc(nLines * sizeof(double));
    coordinate3 = malloc(nLines * sizeof(double));

    getInformation((const char **)lines, nLines, name, residues, 
                   coordinate1, coordinate2, coordinate3);    

    protein_name = getProteinName(filecontent, filesize);
    //Write information into structs
    //funtion to free memory of structs
    
    free(coordinate1);
    free(coordinate2);
    free(coordinate3);
    for(idx = 0; idx < nLines; idx++)
    {
        free(name[idx]);
        free(residues[idx]);
    }
    free(name);
    free(residues);
    free(filecontent);
    free(lines);
}

int main(int argc, char * argv[])
{
    parse("test.txt");
    //parse("pdb5wf5.ent");
}
