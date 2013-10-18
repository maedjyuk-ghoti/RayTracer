#include "ray.h"

/** newSphere **/
entity_t *newSphere(char *enttype, int code) {
  entity_t *ent;
  sphere_t *sphere;
  vector_t zero = {0, 0, 0};

  /* create new sphere structure */
  ent = newSceneObj(enttype, code);
  sphere = malloc(sizeof(sphere_t));
  assert(sphere != NULL);
  ((sobj_t *)(ent->entDerived))->sobjDerived = sphere;

  /* Set default values */
  sphere->center = zero;
  sphere->radius = 1;
  sphere->magic = SPHERE_T;

  ent->load     = loadSphere;
  ent->complete = completeSphere;
  ent->dump     = dumpSphere;

  ((sobj_t *)(ent->entDerived))->hit = hitSphere;

  return(ent);
}

/** loadSphere **/
void loadSphere(FILE *inFP, entity_t *ent, char *attribute) {
   char *attributes[] = {"center", "radius", NULL};
   int ndx;
   assert(ent->magic == ENTITY_T);
   sobj_t *obj = ent->entDerived;
   assert(obj->magic == SCENEOBJ_T);
   sphere_t *sphere = obj->sobjDerived;
   assert(sphere->magic == SPHERE_T);

   ndx = getindex(attribute, attributes);

   switch (ndx) {
      case 0:
         /** center **/
         sphere->center = readTuple(inFP, 
              "Could not read sphere center coordinates");
         break;

      case 1:
         /** radius **/
         sphere->radius = readDouble(inFP, "Could not read sphere radius");
         break;

      default:
         loadSceneObj(inFP, ent, attribute);
   }


};

/** completeShere **/
void completeSphere(scene_t *scene, entity_t *ent) {
     assert(scene->magic == SCENE_T);
     assert(ent->magic == ENTITY_T);
     completeSceneObj(scene, ent);
}

/** dumpSphere **/
void dumpSphere(FILE *out, entity_t *ent) {
   assert(ent->magic == ENTITY_T);
   sobj_t *obj = ent->entDerived;
   assert(obj->magic == SCENEOBJ_T);
   sphere_t *sphere = obj->sobjDerived;
   assert(sphere->magic == SPHERE_T);

   dumpSceneObj(out, ent);
   printTuple(out,  "   center:    ", sphere->center);
   fprintf(out,     "   radius:     %8.4lf\n", sphere->radius);
}

/** hitSphere **/
int hitSphere(entity_t *ent, point_t base, vector_t dir, hitinfo_t *hit) {
  /** asserts **/
  assert(ent->magic == ENTITY_T);
  sobj_t * sobj = ent->entDerived;
  assert(sobj->magic == SCENEOBJ_T);
  sphere_t * sphere = sobj->sobjDerived;
  assert(sphere->magic == SPHERE_T);

  point_t  V  = base;                   //View/Starting point
  vector_t D  = dir;                    //Unit vector in direction of travel
  point_t  C  = sphere->center;         //Center of the sphere
  double   r  = sphere->radius;         //Radius of the sphere
  point_t  Cp = add(C, scale(C, (-1))); //Adjusted center of sphere (0,0,0)
  point_t  Vp = add(V, scale(C, (-1))); //Adjust viewpoint of sphere
  double   t;

  if (length(Cp) != 0) fprintf(stderr, "Cp does not equal (0,0,0)\n");
  //Quadratic formula to determine distance and if a hit occured
  double a = dot(D, D);
  double b = (dot(Vp, D) * 2.0);
  double c = (dot(Vp, Vp) - pow(r, 2));

  t = (pow(b, 2) - (4 * a * c)); //dscriminant

  if (t <= 0) return 0; //miss

  t = ((b*(-1) - pow(t, 1.0/2))/(2 * a)); //quadratic equation

  if (t < 0) return 0; //behind us

  hit->hitpoint = add(V, scale(D, t));

  if (hit->hitpoint.z > 0) return 0;

  hit->normal = add(hit->hitpoint, scale(C, (-1)));
  hit->normal = unitize(hit->normal);
  hit->distance = t;
  return 1;
}
  /* End sphere_hits */
