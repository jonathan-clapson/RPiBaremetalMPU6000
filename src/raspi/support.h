#ifndef __SUPPORT_H__
#define __SUPPORT_H__

void wait(unsigned int delay);
void write_mmap_register(volatile unsigned int reg, unsigned int value);
int read_mmap_register(volatile unsigned int reg);

#endif
