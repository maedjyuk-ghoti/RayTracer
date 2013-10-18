#ifndef TILED_H
#define TILED_H

#define TILED_T 3258416

typedef struct tiled {
  int         magic;
  pixel_t     color1;
  intensity_t diffuse1;
  intensity_t reflective1;
  pixel_t     color2;
  intensity_t diffuse2;
  intensity_t reflective2;
  double      tilewidth;
  double      tileheight;
} tiled_t;

entity_t * newTiled(char * objtype, int code);
void       loadTiled(FILE * inFP, entity_t * obj, char *token);
void       completeTiled(scene_t * scene, entity_t * ent);
void       dumpTiled(FILE * outFP, entity_t * ent);
int        hitTiled(entity_t * ent, point_t base, vector_t dir, hitinfo_t * hit);

#endif
