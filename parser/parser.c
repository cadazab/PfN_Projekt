#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "parser.h"
#include "../protein.h"

/*
* Creates and initialises a new atom struct
*/
Atom * newAtom(const char *name, const double x,
                                 const double y,
                                 const double z)
{
    Atom *atom = malloc(sizeof(*atom));
    atom->name = malloc(5 * sizeof(char));
    strcpy(atom->name, name);
    atom->x = x;
    atom->y = y;
    atom->z = z;
    return atom;
}

/*
* Creates and initialises a new residue struct
*/
Residue * newResidue(const char *name, const unsigned long nr_atoms)
{
    Residue *residue = malloc(sizeof(*residue));
    residue->atoms = NULL;
    residue->name = malloc(4 * sizeof(char));
    strcpy(residue->name, name);
    residue->nr_atoms = nr_atoms;
    return residue;
}

/*
* Creates and initialises a new protein struct
*/
Protein * newProtein(const char *name, const unsigned long nr_residues,
                     const unsigned long nr_atoms)
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

/*
* Opens the file and returns the filecontent as char *.
* Returns NULL if there exists no file with the given filename.
*/
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
    filecontent = malloc(sizeof(*filecontent) * (fs + 1));
    filecontent[fs] = '\0';
    if(fread(filecontent, sizeof(*filecontent), fs, fp) != fs)
    {
        fprintf(stderr, "%s: fread failed\n", __func__);
        return NULL;
    }
    *filesize = fs;
    fclose(fp);

    return filecontent;
}

/*
* Searches the file for the name of the protein.
*/
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

/*
* Searches the file for lines starting with 'ATOM' and saves these lines.
*/
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

/*
* Searches every line with relevant information to save that information
* in arrays
*/
void getInformation(const char ** lines, unsigned long  *nr_lines, char**name,
                    char** residues, char ** residues_number,
                    double* coordinate1,
                    double* coordinate2,
                    double* coordinate3)
{
    unsigned long idx, idx2, writing_index = 0, starting_nr_lines;
    char * name_chars,                                                          
         * residues_chars,                                                      
         * residues_number_chars,
         * coordinate1_chars,                                                   
         * coordinate2_chars,                                                   
         * coordinate3_chars,
        altlock = ' ';
    name_chars = malloc(5 * sizeof(char));
    residues_chars = malloc(4 * sizeof(char));
    residues_number_chars = malloc(5 * sizeof(char));
    coordinate1_chars = malloc(9 * sizeof(char));
    coordinate2_chars = malloc(9 * sizeof(char));
    coordinate3_chars = malloc(9 * sizeof(char));
    starting_nr_lines = *nr_lines;
    for(idx = 0; idx < starting_nr_lines; idx++)
    {
        altlock = lines[idx][16];
        if(altlock == 'A' || altlock == ' ')
        {
            for(idx2 = 0; idx2 < 4; idx2++)
            {
                name_chars[idx2] = lines[idx][idx2 + 12];
            }
            name_chars[4] = '\0';
            for(idx2 = 0; idx2 < 3; idx2++)
            {
                residues_chars[idx2] = lines[idx][idx2 + 17];
            }
            residues_chars[3] = '\0';
            for(idx2 = 0; idx2  < 4;  idx2++)
            {
                residues_number_chars[idx2] = lines[idx][idx2 + 22];
            }
            residues_number_chars[4] = '\0';
            for(idx2 = 0; idx2 < 8; idx2++)
            {
                coordinate1_chars[idx2] = lines[idx][idx2 + 30];
            }
            coordinate1_chars[8] = '\0';  
            for(idx2 = 0; idx2 < 8; idx2++)
            {
                coordinate2_chars[idx2] = lines[idx][idx2 + 38];
            }
            coordinate2_chars[8] = '\0';
            for(idx2 = 0; idx2 < 8; idx2++)
            {
                coordinate3_chars[idx2] = lines[idx][idx2 + 46];
            }
            coordinate3_chars[8] = '\0';
            name[writing_index] = malloc(5*sizeof(char));
            residues[writing_index] = malloc(4*sizeof(char));                          
            residues_number[writing_index] = malloc(5*sizeof(char));
            strcpy(name[writing_index],name_chars);
            strcpy(residues[writing_index],residues_chars);
            strcpy(residues_number[writing_index],residues_number_chars);
            sscanf(coordinate1_chars,"%lf",&coordinate1[writing_index]);                         
            sscanf(coordinate2_chars,"%lf",&coordinate2[writing_index]);                         
            sscanf(coordinate3_chars,"%lf",&coordinate3[writing_index]);    
            ++writing_index;
        }
    }
    *nr_lines = writing_index;

    free(lines);
    free(name_chars);                                                       
    free(residues_chars);                                                   
    free(residues_number_chars);
    free(coordinate1_chars);                                                
    free(coordinate2_chars);                                                
    free(coordinate3_chars);                       
}

/*
* Counts the lengths of the individual residues and saves them in an array.
* Writes the number of residues in the variable pointed to by *nr_residues.
*/
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

/*
* Saves the information in the protein struct.
*/
Protein * writeInfoIntoStructs(const char **name, const char **residues, 
                               char **residues_number,
                               const char *protein_name, 
                               const unsigned long nr_lines,
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

        /* check if second atom of the residue really is the c-alpha atom */
        if (strstr(protein->atoms[atom_idx + 1]->name, "CA") != NULL)
        {
            protein->cAlphas[res_idx] = protein->atoms[atom_idx + 1]; 
        }
        atom_idx += res_lengths[res_idx];
    }
    free(res_lengths);
    return protein;
}

/*
* The main method. Takes a filename as input and returns a Protein.
*/  
Protein* parse(char *filename)
{
    unsigned long nr_lines, filesize, idx, *nr_residues;
    char *filecontent, *protein_name, **lines, **name, **residues,
         **residues_number;
    double *coordinate1, *coordinate2, *coordinate3;
    Protein *protein;

    filecontent = readFromFile(filename, &filesize);
    assert(filecontent != NULL);
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

    getInformation((const char **)lines, &nr_lines, name, residues,
                    residues_number, coordinate1, coordinate2, coordinate3);    
    
    protein = writeInfoIntoStructs((const char **) name,
                                   (const char **) residues, 
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

    return protein;
}

/*
* Method to free the allocated memory.
*/
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

/*
* Main method for testing.
*/
int main(void)
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
        printf("%s %lu %lf\n", protein->residues[idx]->name, protein->residues[idx]->nr_atoms, protein->cAlphas[idx]->x);
    }
    printf("%s %lu %lu\n", protein->name, protein->nr_atoms, protein->nr_residues);
   
    freeProteinStruct(protein);
}
