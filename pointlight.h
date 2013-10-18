/* pointlight.h
*/
#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#define POINTLIGHT_T 9897132

/** pointlight_t -- diffuse lighting data **/
typedef struct lightType {
  int     magic;        /* pointlight_t magic number */
  pixel_t color;        /* light's rgb color         */
  double  brightness;   /* intensity level           */
  point_t center;       /* Center of light           */
  void *  lightDerived; /* Derived object data       */
} pointlight_t;

/** plane function prototype statements **/
entity_t  * newPointLight(char * objtype, int code);
void        loadPointLight(FILE *inFP, entity_t * obj, char * token);
void        completePointLight(scene_t * scene, entity_t * obj);
void        dumpPointLight(FILE * outFP, entity_t * obj);
intensity_t processPointLight(scene_t *, entity_t *, entity_t *, hitinfo_t *);

#endif
