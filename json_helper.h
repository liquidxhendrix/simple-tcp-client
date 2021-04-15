#ifndef JSON_HELPER
#define JSON_HELPER

#include "shapetype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

void shapes_to_JSON(ShapeType *shapes, char *json_string, size_t size);
void JSON_to_shapes(char *json_string,size_t size,ShapeType *shapes);
void DumpShape(ShapeType *s);

#endif