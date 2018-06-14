#include <stdint.h>
typedef void *Memory;

typedef struct {
  uint32_t id;
} Thing;

typedef struct {
  uint32_t StableMemorySize;
  Memory StableMemory;
} State;
