#include "ray.h"

entity_t * newTiled(char * objtype, int code) {
  entity_t * ent;
  tiled_t  * tiled;

  ent = newRectangle(objtype, code);

  sobj_t      * sobj      = ent->entDerived;
  plane_t     * plane     = sobj->sobjDerived;
  rectangle_t * rectangle = plane->planeDerived;

  tiled = malloc(sizeof(tiled_t));
  rectangle->rectangleDerived = tiled;

  tiled->magic       = TILED_T;
  tiled->color1      = tiled->color2      = ((pixel_t){0, 0, 0});
  tiled->diffuse1    = tiled->diffuse2    = ((intensity_t){0, 0, 0});
  tiled->reflective1 = tiled->reflective2 = ((intensity_t){0, 0, 0});
  tiled->tilewidth   = 0.0;
  tiled->tileheight  = 0.0;

  ent->load     = loadTiled;
  ent->complete = completeTiled;
  ent->dump     = dumpTiled;
  sobj->hit     = hitTiled;

  return ent;
}

void loadTiled(FILE * inFP, entity_t * ent, char * token) {
  char *attributes[] = {"color2", "diffuse2", "reflective2",
                        "tilewidth", "tileheight", NULL};
  int ndx;
  assert(ent->magic == ENTITY_T);
  sobj_t *sobj = ent->entDerived;
  assert(sobj->magic == SCENEOBJ_T);
  plane_t *plane = sobj->sobjDerived;
  assert(plane->magic == PLANE_T);
  rectangle_t * rectangle = plane->planeDerived;
  assert(rectangle->magic == RECTANGLE_T);
  tiled_t * tiled = rectangle->rectangleDerived;
  assert(tiled->magic == TILED_T);

  ndx = getindex(token, attributes);

  if (ndx == 0)
    tiled->color2 = readColor(inFP, "Could not read color2 values");
  else if (ndx == 1)
    tiled->diffuse2 = readTuple(inFP, "Could not read diffuse2 values");
  else if (ndx == 2)
    tiled->reflective2 = readTuple(inFP, "Could not read reflective2 values");
  else if (ndx == 3)
    tiled->tilewidth = readDouble(inFP, "Could not read tilewidth");
  else if (ndx == 4)
    tiled->tileheight = readDouble(inFP, "Could not read tileheight");
  else loadRectangle(inFP, ent, token);
}

void completeTiled(scene_t * scene, entity_t * ent) {
  sobj_t      * sobj  = ent->entDerived;
  plane_t     * plane = sobj->sobjDerived;
  rectangle_t * rect  = plane->planeDerived;
  tiled_t     * tiled = rect->rectangleDerived;
  assert(tiled->magic == TILED_T);

  completeRectangle(scene, ent);

  tiled->color1      = sobj->color;
  tiled->diffuse1    = sobj->diffuse;
  tiled->reflective1 = sobj->reflective;
}

void dumpTiled(FILE * outFP, entity_t * ent) {
  assert(ent->magic == ENTITY_T);
  sobj_t *sobj = ent->entDerived;
  assert(sobj->magic == SCENEOBJ_T);
  plane_t *plane = sobj->sobjDerived;
  assert(plane->magic == PLANE_T);
  rectangle_t * rectangle = plane->planeDerived;
  assert(rectangle->magic == RECTANGLE_T);
  tiled_t * tiled = rectangle->rectangleDerived;
  assert(tiled->magic == TILED_T);

  dumpRectangle(outFP, ent);

  fprintf(outFP, "      color2:         %6d   %6d   %6d\n",
                                          tiled->color2.r,
                                          tiled->color2.g,
                                          tiled->color2.b);
  printTuple(outFP, "   diffuse2:       ", tiled->diffuse2);
  printTuple(outFP, "   reflective2:    ", tiled->reflective2);
  fprintf(outFP, "   tilewidth:      %8.4lf\n",  tiled->tilewidth);
  fprintf(outFP, "   tileheight:     %8.4lf\n",  tiled->tileheight);
}

int hitTiled(entity_t * ent, point_t base, vector_t dir, hitinfo_t * hit) {
  if (hitRectangle(ent, base, dir, hit) != 1) return 0;

  assert(ent->magic == ENTITY_T);
  sobj_t *sobj = ent->entDerived;
  assert(sobj->magic == SCENEOBJ_T);
  plane_t *plane = sobj->sobjDerived;
  assert(plane->magic == PLANE_T);
  rectangle_t * rectangle = plane->planeDerived;
  assert(rectangle->magic == RECTANGLE_T);
  tiled_t * tiled = rectangle->rectangleDerived;
  assert(tiled->magic == TILED_T);

  tuple_t p0, p1;
  p0 = project(rectangle->edges[0].side, ray(rectangle->edges[0].point, hit->hitpoint));
  p1 = project(rectangle->edges[1].side, ray(rectangle->edges[0].point, hit->hitpoint));

  double l0, l1;
  l0 = length(p0);
  l1 = length(p1);

  int t0, t1;
  t0 = l0/tiled->tilewidth;
  t1 = l1/tiled->tileheight;

  if (((t0 + t1) % 2) == 0) {
    sobj->color      = tiled->color1;
    sobj->diffuse    = tiled->diffuse1;
    sobj->reflective = tiled->reflective1;
  }
  else {
    sobj->color      = tiled->color2;
    sobj->diffuse    = tiled->diffuse2;
    sobj->reflective = tiled->reflective2;
  }
    
  return 1;
}
