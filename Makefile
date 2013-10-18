# Makefile for ray tracer 3
CC = gcc
CFLAGS = -g -Wall

OBJS= main.o       \
      scene.o      \
      entity.o     \
      sceneobj.o   \
      plane.o      \
      sphere.o     \
      list.o       \
      image.o      \
      readVals.o   \
      window.o     \
      render.o     \
      raytrace.o   \
      tuple.o	   \
      pointlight.o \
      lighting.o   \
      triangle.o   \
      rectangle.o  \
      polyhit.o    \
      ring.o       \
      tiled.o

INCS=   ray.h        \
        scene.h      \
        window.h     \
        image.h      \
        list.h       \
        entity.h     \
        sceneobj.h   \
        plane.h      \
        sphere.h     \
        tuple.h      \
        render.h     \
        raytrace.h   \
        readVals.h   \
        pointlight.h \
        lighting.h   \
        triangle.h   \
        rectangle.h  \
        polyhit.h    \
        ring.h       \
        tiled.h

ray3: ${OBJS}
	@echo
	@echo Linking $@
	gcc -o $@ -Wall -g ${OBJS} -lm

${OBJS}: ${INCS} Makefile

info:
	@echo "\n\tAvailable Make Commands:"
	@echo "\t  test    triangle rectangle"
	@echo "\t  shadow  shadow2  newscene"
	@echo "\t  myscene clean\n"

test:
	./ray3 scene.txt output.ppm
	display output.ppm

myscene:
	./ray3 myscene.txt output.ppm
	display output.ppm

clean:
	rm -f *.o  ray3
