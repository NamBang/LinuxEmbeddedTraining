#include <linux/module.h> /* Define module_init(), module_exit() */
#include <linux/gpio.h>>  /* Define ioremap */

#include "led_ex.h"       /* Define gpio address */

#define DRIVER_AUTHOR  "nambangn@gmail.com"
#define DRIVER_DESC    "GPIO legency"
#define DRIVER_VERS    "1.0"

/* pointer store remap address */
uint32_t __iomem *gpio0_base_addr;

/* Constructor - load when we insmod kernel module */
static int __init gpio_init(void)
{
    /* Map the physical addres of an I/O device to the kernel virtual address. */
    gpio0_base_addr = ioremap(GPIO0_BASE_ADDR, GPIO0_SIZE);

     printk(KERN_INFO "GPIO0 vitrual address: %d \n", *gpio0_base_addr);

    if(!gpio0_base_addr)
    {
        pr_err("Failed to remap memory \n");
    }

    /* Enable GPIO0 OUTPUT */
    *(gpio0_base_addr + GPIO_OE/4) &= ~GPIO0_30;

    /* Set GPIO0_30 to HIGH */
    *(gpio0_base_addr + GPIO_SETDATAOUT/4) |= GPIO0_30;

    printk(KERN_INFO "GPIO0 example \n");
    return 0;
}

/* Destructor - call when we unsmod kernel module */
static void __exit gpio_exit(void)
{
    printk(KERN_INFO "Remove Led_ex driver\n");
    /* Set GPIO0_30 to LOW */
    *(gpio0_base_addr + GPIO_CLEARDATAOUT / 4) |= GPIO0_30;
}

/* init kernel module */
module_init(gpio_init);

/* exit kernel module */
module_exit(gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);