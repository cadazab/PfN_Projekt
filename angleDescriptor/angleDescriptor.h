#ifndef ANGLEDESCRIPTOR_H
#define ANGLEDESCRIPTOR_H

typedef struct {
  double angle;
  double distance;
} Angle;

//Return the angle between the planes
//created by an ngram
Angle* get_angle(Protein *protein);

//Free the memory
void free_angle(Angle *angle);


#endif


