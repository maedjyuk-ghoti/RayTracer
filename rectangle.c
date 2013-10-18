/** rectangle.c **/

#include "ray.h"

entity_t * newRectangle(char * enttype, int code)
{
  entity_t    * ent;
  rectangle_t * rectangle;

  ent = newPlane(enttype, code);

  sobj_t  * sobj = ent->entDerived;
  plane_t * plane = sobj->sobjDerived;

  rectangle = malloc(sizeof(rectangle_t));
  plane->planeDerived = rectangle;

  rectangle->width  = 0.0;
  rectangle->height = 0.0;
  int i;
  for (i = 0; i < 4; i++) {
    rectangle->edges[i].point = ((tuple_t){0, 0, 0});
    rectangle->edges[i].side  = ((tuple_t){0, 0, 0});
  }

  rectangle->magic = RECTANGLE_T;

  ent->load     = loadRectangle;
  ent->complete = completeRectangle;
  ent->dump     = dumpRectangle;

  sobj->hit = hitRectangle;

  return ent;
}

void loadRectangle(FILE * inFP, entity_t * ent, char * attribute) {
  char *attributes[] = {"width", "height", NULL};
  int ndx;
  assert(ent->magic == ENTITY_T);
  sobj_t *sobj = ent->entDerived;
  assert(sobj->magic == SCENEOBJ_T);
  plane_t *plane = sobj->sobjDerived;
  assert(plane->magic == PLANE_T);
  rectangle_t * rectangle = plane->planeDerived;
  assert(rectangle->magic == RECTANGLE_T);

  ndx = getindex(attribute, attributes);

  if (ndx == 0)
    rectangle->width = readDouble(inFP, "Could not read rectangle width");
  else if (ndx == 1)
    rectangle->height = readDouble(inFP, "Could not read rectangle height");
  else
    loadPlane(inFP, ent, attribute);
}

void completeRectangle(scene_t * scene, entity_t * ent) {
  completePlane(scene, ent);

  sobj_t      * sobj  = ent->entDerived;
  plane_t     * plane = sobj->sobjDerived;
  rectangle_t * rect  = plane->planeDerived;
  assert(rect->magic == RECTANGLE_T);

  rect->edges[0].point = plane->point;
  rect->edges[0].side  = unitize(plane->orient1);

  rect->edges[1].point = scale(rect->edges[0].side, rect->width);
  rect->edges[1].point = add(rect->edges[0].point, rect->edges[1].point);
  rect->edges[1].side  = unitize(cross(plane->normal, rect->edges[0].side));

  rect->edges[2].point = scale(rect->edges[1].side, rect->height);
  rect->edges[2].point = add(rect->edges[1].point, rect->edges[2].point);
  rect->edges[2].side  = scale(rect->edges[0].side, -1);

  rect->edges[3].point = scale(rect->edges[2].side, rect->width);
  rect->edges[3].point = add(rect->edges[2].point, rect->edges[3].point);
  rect->edges[3].side  = scale(rect->edges[1].side, -1);
}

void dumpRectangle(FILE * outFP, entity_t * ent) {
  assert(ent->magic == ENTITY_T);
  sobj_t *sobj = ent->entDerived;
  assert(sobj->magic == SCENEOBJ_T);
  plane_t *plane = sobj->sobjDerived;
  assert(plane->magic == PLANE_T);
  rectangle_t * rectangle = plane->planeDerived;
  assert(rectangle->magic == RECTANGLE_T);

  dumpPlane(outFP, ent);

  fprintf(outFP, "   width:      %8.4lf\n", rectangle->width);
  fprintf(outFP, "   height:     %8.4lf\n", rectangle->height);
  int i;
  for (i = 0; i < 4; i++) {
    fprintf(outFP, "   Edge %i:\n", i);
    printTuple(outFP, "      point:     ", rectangle->edges[i].point);
    printTuple(outFP, "      side:      ", rectangle->edges[i].side);
  }


}

int hitRectangle(entity_t * ent, point_t base, vector_t dir, hitinfo_t * hit) {
  if (hitPlane(ent, base, dir, hit) != 1) return 0;

  assert(ent->magic == ENTITY_T);
  sobj_t *sobj = ent->entDerived;
  assert(sobj->magic == SCENEOBJ_T);
  plane_t *plane = sobj->sobjDerived;
  assert(plane->magic == PLANE_T);
  rectangle_t * rectangle = plane->planeDerived;
  assert(rectangle->magic == RECTANGLE_T);


  return polyhit(rectangle->edges, hit->hitpoint, 4);
}

