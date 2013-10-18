/* rectangle.h */
#ifndef TRIANGLE_H
#define TRIANGLE_H

#define TRIANGLE_T 8572957

/** triangle_t -- infinite rectangle data **/
typedef struct triangle_type
{
  int    magic;
  double length1;
  double length2;
  edge_t edges[3];
  void * recDerived;
} triangle_t;

/** Triangle function prototype statements **/
entity_t * newTriangle(char *, int code);
void       loadTriangle(FILE *, entity_t *, char *);
void       completeTriangle(scene_t *, entity_t *);
void       dumpTriangle(FILE *, entity_t *);
int        hitTriangle(entity_t *, point_t, vector_t, hitinfo_t *);

#endif
