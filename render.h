#ifndef RENDER_H
#define RENDER_H

/** Prototype statements for module render.c **/
void    render(scene_t *scene);
pixel_t makePixel(scene_t *scene, int colndx, int rowndx);

#endif
