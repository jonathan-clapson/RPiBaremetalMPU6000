/* delay*/
//wait 1000 seems to be ~2KHz
void wait(unsigned int delay)
{
	while(delay--)
		asm("mov r0, r0");	/* No-op */
}
