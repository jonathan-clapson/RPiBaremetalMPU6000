/* delay*/
//wait 1000 seems to be ~2KHz
void wait(unsigned int delay)
{
	while(delay--)
		asm("mov r0, r0");	/* No-op */
}

void write_mmap_register(volatile unsigned int* reg, unsigned int value)
{
	*reg = value;
}

int read_mmap_register(volatile unsigned int* reg)
{
	return *reg;
}
