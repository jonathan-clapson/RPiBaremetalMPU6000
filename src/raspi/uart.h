#ifndef __UART_H__
#define __UART_H__
#include <stdint.h>

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

void uart_init(void);
void uart_putc(unsigned char c);
int uart_getc(uint8_t *c);
void uart_puts(uint8_t *string);
void uart_putbuf(uint8_t *buf, int count);

#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028
#define GPPUD       0x20200094
#define GPPUD_DISABLEPULLUPDOWN	0x0
#define GPPUD_ENABLEPULLDOWN	0x1
#define GPPUD_ENABLEPULLUP		0x2

#define GPPUDCLK0   0x20200098
#define GPPUDCLK_PUDCLK_RESET 0x0
#define GPPUDCLK_CLK14 (1<<14)
#define GPPUDCLK_CLK15 (1<<15)

#define AUX_ENABLES     0x20215004
#define AUX_ENABLES_MINIUART_EN 0x1

#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044

#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_IIR_REG_RESET_INTPEND	0x01
#define AUX_MU_IIR_REG_CLR_RX_FIFO		0x02
#define AUX_MU_IIR_REG_CLR_TX_FIFO		0x04
#define AUX_MU_IIR_REG_EN_FIFOS			0xC0

#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_LCR_REG_8BITMODE 0x3

#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_LSR_REG_TX_EMPTY		0x20
#define AUX_MU_LSR_REG_TX_NOT_FULL	0x10
#define AUX_MU_LSR_REG_RX_OVERRUN	0x02
#define AUX_MU_LSR_REG_RX_NOT_EMPTY 0x01

#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

#endif
