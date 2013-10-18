#include "ray.h"

/** genRay **/
vector_t genRay(scene_t *scene, int column, int row) {
   vector_t direction;                       // Directior vector
   entity_t    *ent;
   window_t *window;

   assert(scene->magic == SCENE_T);
   ent = scene->window;
   window = ent->entDerived;
   assert(window->magic == WINDOW_T);

   /* Computer the pixel's real scene coordinates */
   direction.x = ((double)(column)/
      (double)(scene->picture->columns-1))*window->windowWidth;
   direction.x -= window->windowWidth/2.0;
   direction.y = ((double)(row)/
      (double)(scene->picture->rows-1))*window->windowHeight;
   direction.y -= window->windowHeight/2.0;
   direction.z = 0;

   /* And now construct a unit vector from the view point to the pixel */
   direction = ray(window->viewPoint, direction);
   direction = unitize(direction);
   return(direction);
} /* End genRay */

/** rayTrace **/
intensity_t rayTrace(scene_t *scene, point_t base, vector_t unitDir,
                 double total_dist, entity_t *self) {
  intensity_t intensity = ((intensity_t){0, 0, 0});
  entity_t * closestEnt;
  hitinfo_t * hit = malloc(sizeof(hitinfo_t));

  closestEnt = closest(scene, base, unitDir, self, hit);
  if (closestEnt == NULL) {
    free(hit);
    return (intensity);
  }

  total_dist += hit->distance;

  window_t * window = ((window_t *)(scene->window->entDerived));
  sobj_t   * sobj   = ((sobj_t   *)(closestEnt->entDerived));

  intensity = ((tuple_t){window->ambient.x * sobj->color.r,
                         window->ambient.y * sobj->color.g,
                         window->ambient.z * sobj->color.b});

  intensity_t light = lighting(scene, closestEnt, hit);
  intensity.x += light.x;
  intensity.y += light.y;
  intensity.z += light.z;

  intensity.x /= 255;
  intensity.y /= 255;
  intensity.z /= 255;
  
  intensity.x /= total_dist;
  intensity.y /= total_dist;
  intensity.z /= total_dist;

  sobj_t * closestSobj = closestEnt->entDerived;
  if (length(((tuple_t)(closestSobj->reflective))) != 0) {
    vector_t U = scale(unitDir, -1);
    vector_t N = hit->normal;
    vector_t V = unitize(add(scale(N, (2 * dot(U, N))), unitDir));

    intensity_t reflection;
    reflection = rayTrace(scene, hit->hitpoint, V, total_dist, closestEnt);

    multiply(reflection, closestSobj->reflective);
    intensity = add(intensity, reflection);
  }


  free(hit);
  return (intensity);

} /* End rayTrace */

entity_t *closest(scene_t *scene, point_t base, vector_t unitDir, 
                  entity_t *self, hitinfo_t *hit) {
  /* Flags
   *  1 - has been set
   *  0 - has not been set
   */
  int flag_closest = 0;

  entity_t   * ent;            //Shortcut to entity in list
  sobj_t     * sobj;           //Shortcut to ent->entDerived
  entity_t   * closest = NULL; //Will be returned at end of function
  hitinfo_t    hit_closest;
  iterator_t * iter = newIterator(scene->sobjList);

  l_begin(iter);
  while((ent = l_next(iter)) != NULL) {
    /** Run Hit Checks and store closest Entity **/
    if (ent != self) {
      sobj = ent->entDerived;
      if (sobj->hit(ent, base, unitDir, &hit_closest) == 1) {
        if (flag_closest == 1) {
          if (hit->distance > hit_closest.distance) {
            hit->distance = hit_closest.distance;
            hit->normal   = hit_closest.normal;
            hit->hitpoint = hit_closest.hitpoint;
            closest = ent;
          }
        }
        else { //first time there has been a "hit"
          hit->distance = hit_closest.distance;
          hit->normal   = hit_closest.normal;
          hit->hitpoint = hit_closest.hitpoint;

          flag_closest = 1;
          closest = ent;
        }
      } // end of if(hitFuncPtr())
    } //end of if(ent != self)
  } // end of while()
  free(iter);
  return closest;
} /* End closest */
