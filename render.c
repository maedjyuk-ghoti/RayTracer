#include "ray.h"

/** render **/
void render(scene_t *scene) {
  assert(scene->magic == SCENE_T);

  image_t * picture = scene->picture;
  int r, c;
  for (r = 0; r < picture->rows; r++)
    for (c = 0; c < picture->columns; c++) {
      int rowthingy = ((picture->rows - (r + 1)) * picture->columns);
      *(picture->image + (rowthingy + c)) = makePixel(scene, c, r);
    }
} /* End render */

/** makePixel **/
pixel_t makePixel(scene_t *scene, int colndx, int rowndx) {
  intensity_t intensity;
  vector_t dir;
  window_t * windowPtr;

  dir = genRay(scene, colndx, rowndx);
  windowPtr = scene->window->entDerived;
  intensity = rayTrace(scene, windowPtr->viewPoint, dir, 0.0, NULL);
  if (intensity.x > 1.0) intensity.x = 1.0;
  if (intensity.y > 1.0) intensity.y = 1.0;
  if (intensity.z > 1.0) intensity.z = 1.0;


  return((pixel_t){(255 * intensity.x),
                   (255 * intensity.y),
                   (255 * intensity.z)});
} /* End makePixel */

