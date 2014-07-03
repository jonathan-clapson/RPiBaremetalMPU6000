/*!	\file support.c
 * 	\brief HAL Support Functions
 */
 
/*!	\fn void wait(unsigned int delay)
 *	\brief Blocking wait
 * 
 *	delay of 1000 seems to be ~2KHz
 *	@param[in] delay
 */
void wait(unsigned int delay)
{
	while(delay--)
		asm("mov r0, r0");	/* No-op */
}

/*!	\fn void write_mmap_register(volatile unsigned int reg, unsigned int value)
 *	\brief write a 32 bit value to a memory mapped hardware device
 * 
 *	@param[in] reg address to write to
 *	@param[in] value value to write to address
 */
void write_mmap_register(volatile unsigned int reg, unsigned int value)
{
	volatile unsigned int *mmap_reg = (volatile unsigned int*) reg;
	*mmap_reg = value;
}

/*!	\fn int read_mmap_register(volatile unsigned int reg)
 *	\brief read a 32 bit value from a memory mapped hardware device
 * 
 *	@param[in] reg address to read from
 *	\return 32bit value returned from memory mapped device
 */
int read_mmap_register(volatile unsigned int reg)
{
	volatile unsigned int *mmap_reg = (volatile unsigned int*) reg;
	return *mmap_reg;
}
