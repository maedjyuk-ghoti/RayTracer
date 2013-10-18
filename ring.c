#include "ray.h"

entity_t * newRing(char * enttype, int code) {
  entity_t * ent;
  ring_t   * ring;

  ent = newPlane(enttype, code);

  sobj_t  * sobj  = ent->entDerived;
  plane_t * plane = sobj->sobjDerived;

  ring = malloc(sizeof(ring_t));
  plane->planeDerived = ring;

  ring->magic   = RING_T;
  ring->radius1 = 0;
  ring->width   = 0;
  ring->radius2 = 0;

  ent->load     = loadRing;
  ent->complete = completeRing;
  ent->dump     = dumpRing;

  sobj->hit = hitRing;

  return ent;
}

void loadRing(FILE * inFP, entity_t * ent, char * attribute) {
  char * attributes[] = {"radius", "width", NULL};
  int ndx;
  assert(ent->magic == ENTITY_T);
  sobj_t *sobj = ent->entDerived;
  assert(sobj->magic == SCENEOBJ_T);
  plane_t *plane = sobj->sobjDerived;
  assert(plane->magic == PLANE_T);
  ring_t * ring = plane->planeDerived;
  assert(ring->magic == RING_T);

  ndx = getindex(attribute, attributes);

  if (ndx == 0)
    ring->radius1 = readDouble(inFP, "Could not read ring radius");
  else if (ndx == 1)
    ring->width = readDouble(inFP, "Could not read ring width");
  else
    loadPlane(inFP, ent, attribute);
}

void completeRing(scene_t * scene, entity_t * ent) {
  completePlane(scene, ent);

  sobj_t  * sobj  = ent->entDerived;
  plane_t * plane = sobj->sobjDerived;
  ring_t  * ring  = plane->planeDerived;
  assert(ring->magic == RING_T);

  ring->radius2 = ring->radius1 + ring->width;
}

void dumpRing(FILE * outFP, entity_t * ent) {
  assert(ent->magic == ENTITY_T);
  sobj_t *sobj = ent->entDerived;
  assert(sobj->magic == SCENEOBJ_T);
  plane_t *plane = sobj->sobjDerived;
  assert(plane->magic == PLANE_T);
  ring_t * ring = plane->planeDerived;
  assert(ring->magic == RING_T);

  dumpPlane(outFP, ent);

  fprintf(outFP, "   radius:     %8.4lf\n", ring->radius1);
  fprintf(outFP, "   width:      %8.4lf\n", ring->width);
}

int hitRing(entity_t * ent, point_t base, vector_t dir, hitinfo_t * hit) {
  if(hitPlane(ent, base, dir, hit) != 1) return 0;

  assert(ent->magic == ENTITY_T);
  sobj_t *sobj = ent->entDerived;
  assert(sobj->magic == SCENEOBJ_T);
  plane_t *plane = sobj->sobjDerived;
  assert(plane->magic == PLANE_T);
  ring_t * ring = plane->planeDerived;
  assert(ring->magic == RING_T);

  tuple_t r_centerTOhit = ray(plane->point, hit->hitpoint);
  double  d_hitpoint    = length(r_centerTOhit);
  double  d_inner       = length(scale(unitize(r_centerTOhit), ring->radius1));
  double  d_outer       = length(scale(unitize(r_centerTOhit), ring->radius2));

  return ((d_hitpoint > d_inner) && (d_hitpoint < d_outer));
}

