#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"
#include "protein.h"

Atom * newAtom(char *name, double x, double y, double z)
{
    Atom *atom = malloc(sizeof(*atom));
    atom->name = malloc(5 * sizeof(char));
    strcpy(atom->name, name);
    atom->x = x;
    atom->y = y;
    atom->z = z;
    return atom;
}

Residue * newResidue(char *name, unsigned long nr_atoms)
{
    Residue *residue = malloc(sizeof(*residue));
    residue->atoms;
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

char *getProteinName(char *filecontent)
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
                    char** residues, char ** residue_number,
                    double* coordinate1,
                    double* coordinate2, 
                    double* coordinate3) 
{                                                                               
    int idx = 0,                                                                
        count = 0;                                                              
    unsigned long name_length = 0, residues_length = 0,
                  residue_number_length = 0;
    bool previous_space = false;                                                
    char * name_chars,                                                          
         * residues_chars,                                                      
         * residue_number_chars,
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
        residue_number_chars = malloc(sizeof(char));
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
                    case 5: residue_number_chars[idx] = lines[i][j];
                            residue_number_chars[idx+1] = '\0';
                            residue_number_length++;
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
        residue_number[i] = malloc((residue_number_length+1)*sizeof(char));
        strcpy(name[i],name_chars);
        strcpy(residues[i],residues_chars);
        strcpy(residue_number[i],residue_number_chars);
        sscanf(coordinate1_chars,"%lf",&coordinate1[i]);                         
        sscanf(coordinate2_chars,"%lf",&coordinate2[i]);                         
        sscanf(coordinate3_chars,"%lf",&coordinate3[i]);                         
        /*
        * freeing the helping arrays
        */
        free(name_chars);                                                       
        free(residues_chars);                                                   
        free(residue_number_chars);
        free(coordinate1_chars);                                                
        free(coordinate2_chars);                                                
        free(coordinate3_chars);                                                
        count = 0;
        name_length = 0;
        residues_length = 0;
        residue_number_length = 0;
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

unsigned long *getResidueLengths(char **residues, char **residue_number,
                                 unsigned long nLines,
                                 unsigned long *nofresidues)
{
    unsigned long atom_idx, res_idx, nResidues, *res_lengths;
    char *temp;
   
    temp = residue_number[0];
    nResidues = 1;
    for(atom_idx = 1; atom_idx < nLines; ++atom_idx)
    {
        if(strcmp(residue_number[atom_idx], temp))
        {
            ++nResidues;
            temp = residue_number[atom_idx];
        }
    }
    res_lengths = calloc(nResidues, sizeof(unsigned long));
    temp = residue_number[0];
    res_idx = 0;
    for(atom_idx = 0; atom_idx < nLines; ++atom_idx)
    {
        if(!strcmp(residue_number[atom_idx], temp))
        {
            ++res_lengths[res_idx]; 
        }
        else
        {
            temp = residue_number[atom_idx];
            ++res_idx;
            ++res_lengths[res_idx];
        }
    }
    *nofresidues = nResidues;

    return res_lengths;
}

Protein * writeInfoIntoStructs(char **name, char **residues, char **residue_number, 
                               char *protein_name, unsigned long nLines,
                               double* coordinate1,
                               double* coordinate2,
                               double* coordinate3)
{
    unsigned long nofatoms, nofresidues, atom_idx, res_idx, *res_lengths;

    Protein *protein;    
    nofatoms = nLines;
    res_lengths = getResidueLengths(residues, residue_number,
                                    nLines, &nofresidues);
    protein = newProtein(protein_name, nofresidues, nofatoms);
    
    for(atom_idx = 0; atom_idx < nofatoms; ++atom_idx)
    {
        protein->atoms[atom_idx] = newAtom(name[atom_idx],
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
        protein->cAlphas[res_idx] = protein->atoms[atom_idx + 1];
        atom_idx += res_lengths[res_idx];
    }

    return protein;
}

Protein* parse(char *filename)
{
    unsigned long nLines, filesize, idx, *nofresidues, *residue_lengths;
    char *filecontent, *protein_name, **lines, **name, **residues,
         **residue_number;
    double *coordinate1, *coordinate2, *coordinate3;
    Protein *protein;

    filecontent = readFromFile(filename, &filesize);
    lines = getRelevantLines(filecontent, &nLines);

    name = malloc(nLines * sizeof(char*));
    residues = malloc(nLines * sizeof(char*));
    residue_number = malloc(nLines * sizeof(char*));
    coordinate1 = malloc(nLines * sizeof(double));    
    coordinate2 = malloc(nLines * sizeof(double));
    coordinate3 = malloc(nLines * sizeof(double));
    nofresidues = malloc(sizeof(unsigned long));

    getInformation((const char **)lines, nLines, name, residues, residue_number, 
                   coordinate1, coordinate2, coordinate3);    

    protein_name = getProteinName(filecontent);

    residue_lengths = getResidueLengths(residues,residue_number,nLines,
                                       nofresidues);
    protein = writeInfoIntoStructs(name, residues, residue_number, protein_name, nLines,
                                   coordinate1, coordinate2, coordinate3); 
   
    free(coordinate1);
    free(coordinate2);
    free(coordinate3);
    free(nofresidues);
    for(idx = 0; idx < nLines; idx++)
    {
        free(name[idx]);
        free(residues[idx]);
        free(residue_number[idx]);
    }
    free(residue_lengths);
    free(residue_number);
    free(name);
    free(residues);
    free(filecontent);
    free(lines);

    return protein;
}

void freeProteinStruct(Protein *protein)
{
    unsigned long idx;
    for(idx = 0; idx < protein->nr_residues; idx++)
    {
        free(protein->residues[idx]->name);
        free(protein->residues[idx]);
    }
    for(idx = 0; idx < protein->nr_atoms; idx++)
    {
        free(protein->atoms[idx]->name);
        free(protein->atoms[idx]);
    }
    free(protein->cAlphas);
    free(protein->atoms);
    free(protein->residues);
    free(protein->name);
    free(protein);    
}

int main(int argc, char * argv[])
{
    Protein* protein;
    unsigned long idx;
    protein = parse("test.txt");
    //parse("pdb5wf5.ent");
    for(idx = 0; idx < protein->nr_atoms; ++idx)
    {
        printf("%s ", protein->atoms[idx]->name);

        printf("%lf ", protein->atoms[idx]->x);
        printf("%lf ", protein->atoms[idx]->y);
        printf("%lf\n", protein->atoms[idx]->z);
    }
    for(idx = 0; idx < protein->nr_residues; idx++)
    {
        printf("%s %lu\n", protein->residues[idx]->name, protein->residues[idx]->nr_atoms);
    }
    printf("%s %lu %lu\n", protein->name, protein->nr_atoms, protein->nr_residues);
    freeProteinStruct(protein);
}
