/* pointlight.c */

#include "ray.h"

entity_t* newPointLight(char * objtype, int code) {
  entity_t * ent;
  pointlight_t * light;

  ent = newEntity(objtype, code);
  light = malloc(sizeof(pointlight_t));
  assert(light != NULL);
  ent->entDerived = light;

  light->magic      = POINTLIGHT_T;
  light->color      = ((pixel_t){0, 0, 0});
  light->brightness = 0.0;
  light->center     = ((point_t){0, 0, 0});

  ent->load     = loadPointLight;
  ent->complete = completePointLight;
  ent->dump     = dumpPointLight;

  return ent;
}

void loadPointLight(FILE * inFP, entity_t * obj, char * token) {
  char *attributes[] =  {"color", "brightness", "center", NULL};
  int ndx;

  assert(obj->magic == ENTITY_T);
  pointlight_t * light = obj->entDerived;
  assert(light->magic == POINTLIGHT_T);

  ndx = getindex(token, attributes);

  if (ndx == 0)
    light->color = readColor(inFP, "Could not read pointlight color");
  else if (ndx == 1)
    light->brightness = readDouble(inFP, "Could not read pointlight brightness");
  else if (ndx == 2)
    light->center = ((point_t)(readTuple(inFP, "Could not read pointlight center")));
  else
    loadEntity(inFP, obj, token);
}

void completePointLight(scene_t * scene, entity_t * obj) {
  assert(scene->magic == SCENE_T);

  l_add(scene->lightList, obj);
}

void dumpPointLight(FILE * outFP, entity_t * obj) {
  assert(obj->magic == ENTITY_T);
  pointlight_t * light = obj->entDerived;
  assert(light->magic == POINTLIGHT_T);

  dumpEntity(outFP, obj);

  fprintf(outFP, "   color:        %6d    %6d    %6d\n",
    light->color.r, light->color.g, light->color.b);
  fprintf(outFP, "   brightness: %8.4lf\n", light->brightness);
  fprintf(outFP, "   center:     %8.4lf  %8.4lf  %8.4lf\n",
    light->center.x, light->center.y, light->center.z);
}

intensity_t processPointLight(scene_t * scene, entity_t * ent,
                              entity_t * light, hitinfo_t * hit) {
  intensity_t processedLight = {0, 0, 0};

  hitinfo_t * hit_local = malloc(sizeof(hitinfo_t));

  pointlight_t * lightObj   = light->entDerived;
  tuple_t        hitTOlight = unitize(ray(hit->hitpoint, lightObj->center));

  /* Self occlusion test. Positive indicates it is not self-occluded. */
  double occlusion = dot(hitTOlight, hit->normal);
  if (occlusion < 0) {
    free(hit_local);
    return processedLight;
  }

    /* Occluded by other object? */
    point_t    base        = lightObj->center;

    tuple_t lightTOhit     = unitize(ray(lightObj->center, hit->hitpoint));
    entity_t * closest_ent = closest(scene, base, ((vector_t)(lightTOhit)), NULL, hit_local);


    if (closest_ent == ent) {
      double      constant = dot(hit->normal, hitTOlight) / hit_local->distance;
      intensity_t diffuse  = ((sobj_t *)(ent->entDerived))->diffuse;
      processedLight.x = diffuse.x * lightObj->color.r * lightObj->brightness * constant;
      processedLight.y = diffuse.y * lightObj->color.g * lightObj->brightness * constant;
      processedLight.z = diffuse.z * lightObj->color.b * lightObj->brightness * constant;
    }

  free(hit_local);
  return processedLight;
}
