//this links to 3dvfunc.cpp
#ifndef OPENCLLINK_H
#define OPENCLLINK_H
#include <cstdint>

typedef struct {
  void *data;
  uint32_t size;
} Data;
float* CoordPointer[3];

Data GetVector(uint32_t num,uint8_t vector, uint32_t raynum);
void LoadOBJ(const char *path, uint32_t coordnum);
#endif