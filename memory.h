#ifndef __MEMORY_H_
#define __MEMORY_H_

#include <unistd.h>

void DestroyBuffers();
void CopyToInputBuffer(void* data, uint32_t size);
void CopyFromOutputBuffer(void* data, uint32_t size);
void CreateBuffer(uint32_t inputSize, uint32_t outputSize);

#endif
