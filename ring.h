#ifndef RING_H
#define RING_H

#define RING_T 3687185

typedef struct ring {
  int    magic;
  double radius1;
  double width;
  double radius2;
  void * ringDerived;
} ring_t;

entity_t * newRing(char *, int);
void       loadRing(FILE *, entity_t *, char *);
void       completeRing(scene_t *, entity_t *);
void       dumpRing(FILE *, entity_t *);
int        hitRing(entity_t *, point_t, vector_t, hitinfo_t *);


#endif
