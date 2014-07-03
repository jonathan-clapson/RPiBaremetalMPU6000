/*!	\file irq.h
 * 	\brief HAL IRQ handling
 */

#ifndef __IRQ_H__
#define __IRQ_H__


extern volatile unsigned int GPIO_INT0;
extern volatile unsigned int GPIO_INT1;
extern volatile unsigned int GPIO_INT2;
extern volatile unsigned int GPIO_INT3;

volatile unsigned int IRQ_IRQ2ENABLE;

extern void enable_irq( void );
int irq_gpio_falling_edge_en ( int pin );

int enable_interrupt_for_irq(int irq);
int register_irq_handler ( int irq, void (*irq_handler_func)(void) );
void c_enable_irq( void );
#endif /* __IRQ_H__ */
