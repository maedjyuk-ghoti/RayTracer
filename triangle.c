/** triangle.c **/

#include "ray.h"

entity_t * newTriangle(char * enttype, int code)
{
  entity_t   *ent;
  triangle_t *triangle;

  ent = newPlane(enttype, code);

  sobj_t  *sobj  = ent->entDerived;
  plane_t *plane = sobj->sobjDerived;

  triangle = malloc(sizeof(triangle_t));
  plane->planeDerived = triangle;

  triangle->length1 = 0.0;
  triangle->length2 = 0.0;
  int i;
  for (i = 0; i < 3; i++) {
    triangle->edges[i].point = ((tuple_t){0, 0, 0});
    triangle->edges[i].side  = ((tuple_t){0, 0, 0});
  }

  triangle->magic = TRIANGLE_T;

  ent->load     = loadTriangle;
  ent->complete = completeTriangle;
  ent->dump     = dumpTriangle;

  sobj->hit     = hitTriangle;
  return ent;
}

void loadTriangle(FILE * inFP, entity_t * ent, char * attribute) {
  char *attributes[] = {"length1", "length2", NULL};
  int ndx;
  assert(ent->magic == ENTITY_T);
  sobj_t *sobj = ent->entDerived;
  assert(sobj->magic == SCENEOBJ_T);
  plane_t *plane = sobj->sobjDerived;
  assert(plane->magic == PLANE_T);
  triangle_t * triangle = plane->planeDerived;
  assert(triangle->magic == TRIANGLE_T);

  ndx = getindex(attribute, attributes);

  if (ndx == 0)
    triangle->length1 = readDouble(inFP, "Could not read triangle length1");
  else if (ndx == 1)
    triangle->length2 = readDouble(inFP, "Could not read triangle length2");
  else
    loadPlane(inFP, ent, attribute);
}

void completeTriangle(scene_t * scene, entity_t * ent) {
  completePlane(scene, ent);

  sobj_t     * sobj  = ent->entDerived;
  plane_t    * plane = sobj->sobjDerived;
  triangle_t * tri   = plane->planeDerived;
  assert(tri->magic == TRIANGLE_T);

  tri->edges[0].point = plane->point;
  tri->edges[0].side  = unitize(((tuple_t)(plane->orient1)));

  tri->edges[1].point = scale(tri->edges[0].side, tri->length1);
  tri->edges[1].point = add(tri->edges[0].point, tri->edges[1].point);

  tri->edges[2].point = scale(unitize(plane->orient2), tri->length2);
  tri->edges[2].point = add(tri->edges[0].point, tri->edges[2].point);

  tri->edges[2].side = unitize(ray(tri->edges[2].point, tri->edges[0].point));
  tri->edges[1].side = unitize(ray(tri->edges[1].point, tri->edges[2].point));

}

void dumpTriangle(FILE * outFP, entity_t * ent) {
  assert(ent->magic == ENTITY_T);
  sobj_t *sobj = ent->entDerived;
  assert(sobj->magic == SCENEOBJ_T);
  plane_t *plane = sobj->sobjDerived;
  assert(plane->magic == PLANE_T);
  triangle_t * triangle = plane->planeDerived;
  assert(triangle->magic == TRIANGLE_T);

  dumpPlane(outFP, ent);

  fprintf(outFP, "   length1:    %8.4lf\n", triangle->length1);
  fprintf(outFP, "   length2:    %8.4lf\n", triangle->length2);
  int i;
  for (i = 0; i < 3; i++) {
    fprintf(outFP, "   Edge %i:\n", i);
    printTuple(outFP, "      point:     ", triangle->edges[i].point);
    printTuple(outFP, "      side:      ", triangle->edges[i].side);
  }
}

int hitTriangle(entity_t * ent, point_t base, vector_t dir, hitinfo_t * hit) {
  if (hitPlane(ent, base, dir, hit) != 1) return 0; /* Are we on the plane? */

  assert(ent->magic == ENTITY_T);
  sobj_t *sobj = ent->entDerived;
  assert(sobj->magic == SCENEOBJ_T);
  plane_t *plane = sobj->sobjDerived;
  assert(plane->magic == PLANE_T);
  triangle_t * triangle = plane->planeDerived;
  assert(triangle->magic == TRIANGLE_T);

  return polyhit(triangle->edges, hit->hitpoint, 3);
}

