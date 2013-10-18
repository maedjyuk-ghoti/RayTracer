/* rectangle.h */
#ifndef RECTANGLE_H
#define RECTANGLE_H

#define RECTANGLE_T 1452785

/** rectangle_t -- infinite rectangle data **/
typedef struct rectangle_type
{
  int    magic;
  double width;
  double height;
  edge_t edges[4];
  void * rectangleDerived;
} rectangle_t;

/** rectangle function prototype statements **/
entity_t * newRectangle(char *, int code);
void       loadRectangle(FILE *, entity_t *, char *);
void       completeRectangle(scene_t *, entity_t *);
void       dumpRectangle(FILE *, entity_t *);
int       hitRectangle(entity_t *, point_t, vector_t, hitinfo_t *);

#endif
