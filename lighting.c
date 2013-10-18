/* lighting.c */

#include "ray.h"

intensity_t lighting(scene_t * scene, entity_t * ent, hitinfo_t * hit) {
  intensity_t total_illumination = ((intensity_t){0, 0, 0});
  iterator_t * lightIter = newIterator(scene->lightList);

  entity_t * light_ent;
  while ((light_ent = l_next(lightIter)) != NULL) {
    tuple_t light = processPointLight(scene, ent, light_ent, hit);

    total_illumination = add(((tuple_t)(total_illumination)), light);
  }
  free(lightIter);
  return total_illumination;
}

