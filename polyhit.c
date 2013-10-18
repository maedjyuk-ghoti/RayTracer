/*polyhit.c*/

#include "ray.h"

int polyhit(edge_t edges[], point_t hitpoint, int numsides) {
  int i;
  tuple_t dir_vector, crossed, previous;
  for (i = 0; i < numsides; i++) {
    dir_vector = unitize(ray(hitpoint, edges[i].point));
    crossed = cross(edges[i].side, dir_vector);
    if (length(crossed) == 0) return 0; /* parallel */

    crossed = unitize(crossed);

    if (i > 0)
      if (dot(crossed, previous) <= 0) return 0;
    previous = crossed;
  }

  return 1;
}
