#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"
#include "protein.h"

Atom * newAtom(const char *name, const double x, const double y, const double z)
{
    Atom *atom = malloc(sizeof(*atom));
    atom->name = malloc(5 * sizeof(char));
    strcpy(atom->name, name);
    atom->x = x;
    atom->y = y;
    atom->z = z;
    return atom;
}

Residue * newResidue(const char *name, const unsigned long nr_atoms)
{
    Residue *residue = malloc(sizeof(*residue));
    residue->atoms = NULL;
    residue->name = malloc(4 * sizeof(char));
    strcpy(residue->name, name);
    residue->nr_atoms = nr_atoms;
    return residue;
}

Protein * newProtein(const char *name, const unsigned long nr_residues, const unsigned long nr_atoms)
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

void getInformation(const char ** lines, const unsigned long nr_lines, char** name,
                    char** residues, char ** residues_number,
                    double* coordinate1,
                    double* coordinate2, 
                    double* coordinate3) 
{                                                                                                                                             
    unsigned long name_length = 0, residues_length = 0, count = 0, idx = 0,
                  residues_number_length = 0;
    bool previous_space = false;                                                
    char * name_chars,                                                          
         * residues_chars,                                                      
         * residues_number_chars,
         * coordinate1_chars,                                                   
         * coordinate2_chars,                                                   
         * coordinate3_chars;                                                   
                                                                                
    /*
    * going through every line   
    */                                                                        
    for(int i = 0; i < nr_lines; i++)                                           
    {
        /*
        * allocating the help-array where the relevant chars of one line 
        * are saved in
        */                                                                           
        name_chars = malloc(sizeof(char)*10);                                   
        residues_chars = malloc(sizeof(char)*10);                               
        residues_number_chars = malloc(sizeof(char));
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
                    case 2: 
                            name_chars[idx] = lines[i][j];                      
                            name_chars[idx+1] = '\0';
                            name_length++;
                            idx++;
                            break;                                              
                    case 3: residues_chars[idx] = lines[i][j]; 
                            residues_chars[idx+1] = '\0';
                            residues_length++;
                            idx++;                                              
                            break;                                              
                            /*
                            * this case exists to prevent problems if the 
                            * residue number is 4 characters long and thus
                            * connected with the previous column
                            */
                    case 4: previous_space = true;
                            count++;
                            idx = 0;
                            break;
                    case 5: residues_number_chars[idx] = lines[i][j];
                            residues_number_chars[idx+1] = '\0';
                            residues_number_length++;
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
        residues_number[i] = malloc((residues_number_length+1)*sizeof(char));
        strcpy(name[i],name_chars);
        strcpy(residues[i],residues_chars);
        strcpy(residues_number[i],residues_number_chars);
        sscanf(coordinate1_chars,"%lf",&coordinate1[i]);                         
        sscanf(coordinate2_chars,"%lf",&coordinate2[i]);                         
        sscanf(coordinate3_chars,"%lf",&coordinate3[i]);                         
        /*
        * freeing the helping arrays
        */
        free(name_chars);                                                       
        free(residues_chars);                                                   
        free(residues_number_chars);
        free(coordinate1_chars);                                                
        free(coordinate2_chars);                                                
        free(coordinate3_chars);                                                
        count = 0;
        name_length = 0;
        residues_length = 0;
        residues_number_length = 0;
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

char ** getRelevantLines(char *filecontent, unsigned long *nr_lines)
{
    unsigned long idx, nr_lines_counter;
    char **lines, *ptr;

    ptr = filecontent;
    nr_lines_counter = 0;

    /*count the number of relevant lines (starting with "ATOM")*/
    while((*ptr != '\0') && (strstr(ptr, "\nATOM") != NULL))
    {
        ptr = strstr(ptr, "\nATOM") + 1;
        ++nr_lines_counter;
    }
    ptr = filecontent;
    lines = malloc(nr_lines_counter * sizeof(*lines));
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
    for(idx = 0; idx < nr_lines_counter; ++idx)
    {
        ptr = lines[idx];
        ptr = strstr(ptr, "\n");
        *ptr = '\0';
    }

    *nr_lines = nr_lines_counter; 
    return lines;
}

unsigned long *getResidueLengths(char **residues_number,
                                 const unsigned long nr_lines,
                                 unsigned long *nr_residues)
{
    unsigned long atom_idx, res_idx, nr_residues_counter, *res_lengths;
    char *temp;
   
    temp = residues_number[0];
    nr_residues_counter = 1;
    for(atom_idx = 1; atom_idx < nr_lines; ++atom_idx)
    {
        if(strcmp(residues_number[atom_idx], temp))
        {
            ++nr_residues_counter;
            temp = residues_number[atom_idx];
        }
    }
    res_lengths = calloc(nr_residues_counter, sizeof(unsigned long));
    temp = residues_number[0];
    res_idx = 0;
    for(atom_idx = 0; atom_idx < nr_lines; ++atom_idx)
    {
        if(!strcmp(residues_number[atom_idx], temp))
        {
            ++res_lengths[res_idx]; 
        }
        else
        {
            temp = residues_number[atom_idx];
            ++res_idx;
            ++res_lengths[res_idx];
        }
    }
    *nr_residues = nr_residues_counter;

    return res_lengths;
}

Protein * writeInfoIntoStructs(const char **name, const char **residues, char **residues_number, 
                               const char *protein_name, const unsigned long nr_lines,
                               const double* coordinate1,
                               const double* coordinate2,
                               const double* coordinate3)
{
    unsigned long nr_atoms, nr_residues, atom_idx, res_idx, *res_lengths;

    Protein *protein;    
    nr_atoms = nr_lines;
    res_lengths = getResidueLengths(residues_number, nr_lines, &nr_residues);
    protein = newProtein(protein_name, nr_residues, nr_atoms);
    
    for(atom_idx = 0; atom_idx < nr_atoms; ++atom_idx)
    {
        protein->atoms[atom_idx] = newAtom(name[atom_idx],
                                           coordinate1[atom_idx], 
                                           coordinate2[atom_idx],
                                           coordinate3[atom_idx]);
    }

    atom_idx = 0;
    for(res_idx = 0; res_idx < nr_residues; ++res_idx)
    {
        protein->residues[res_idx] = newResidue(residues[atom_idx],
                                                res_lengths[res_idx]);
        protein->residues[res_idx]->atoms = protein->atoms[atom_idx];
        protein->cAlphas[res_idx] = protein->atoms[atom_idx + 1];
        atom_idx += res_lengths[res_idx];
    }
    free(res_lengths);
    return protein;
}

Protein* parse(char *filename)
{
    unsigned long nr_lines, filesize, idx, *nr_residues;
    char *filecontent, *protein_name, **lines, **name, **residues,
         **residues_number;
    double *coordinate1, *coordinate2, *coordinate3;
    Protein *protein;

    filecontent = readFromFile(filename, &filesize);
    lines = getRelevantLines(filecontent, &nr_lines);
    protein_name = getProteinName(filecontent);

    /* create the arrays to store the relevant information */
    name = malloc(nr_lines * sizeof(char*));
    residues = malloc(nr_lines * sizeof(char*));
    residues_number = malloc(nr_lines * sizeof(char*));
    coordinate1 = malloc(nr_lines * sizeof(double));    
    coordinate2 = malloc(nr_lines * sizeof(double));
    coordinate3 = malloc(nr_lines * sizeof(double));
    nr_residues = malloc(sizeof(unsigned long));

    getInformation((const char **)lines, nr_lines, name, residues, residues_number, 
                   coordinate1, coordinate2, coordinate3);    
    
    protein = writeInfoIntoStructs((const char **) name, (const char **) residues, 
                                   residues_number, protein_name, nr_lines,
                                   coordinate1, coordinate2, coordinate3); 
    free(coordinate1);
    free(coordinate2);
    free(coordinate3);
    free(nr_residues);
    for(idx = 0; idx < nr_lines; idx++)
    {
        free(name[idx]);
        free(residues[idx]);
        free(residues_number[idx]);
    }
    free(filecontent);
    free(residues_number);
    free(name);
    free(residues);
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
    //protein = parse("test.txt");
    //protein = parse("pdb1jm7.ent");
    protein = parse("pdb5wf5.ent");    
    
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
