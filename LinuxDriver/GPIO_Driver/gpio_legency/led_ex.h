#ifndef __LED_MODUEL_H_
#define __LED_MODUEL_H_

/* Get address from data sheet */
#define GPIO0_BASE_ADDR         0x44E07000
#define GPIO0_END_ADDR          0x44E07FFF
#define GPIO0_SIZE              (GPIO0_END_ADDR - GPIO0_BASE_ADDR)

#define GPIO_OE                 0x134
#define GPIO_DATAOUT            0x13c
#define GPIO_CLEARDATAOUT       0x190
#define GPIO_SETDATAOUT         0x194

#define GPIO0_30                (1 << 30)   /* P9_11 */

#endif __LED_MODUEL_H_