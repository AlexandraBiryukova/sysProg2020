#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");


/*  ISR : interrupt handler
*   This function services keyboard interrupts. It reads the relevant
*   information from the keyboard and then puts information about Key that pressed
*   This example only has 3 key: ESC, F1 and F2
*/

void* HANDLER_KEY;

irqreturn_t irq_handler(int irq, void *dev_id)
{
/*
* This variables are static because they need to be
* accessible (through pointers) to the bottom half routine.
*/

  static unsigned char scancode;
  unsigned char status;

/*
* Read keyboard status
*/
  status = inb(0x64);
  scancode = inb(0x60);

switch (scancode)
{
  case 0x1E:  printk (KERN_INFO "! You pressed A ...\n");
              break;
  case 0x26:  printk (KERN_INFO "! You pressed L ...\n");
              break;
  case 0x12:  printk (KERN_INFO "! You pressed E ...\n");
              break;
  case 0x2D:  printk (KERN_INFO "! You pressed X ...\n");
              break;
  default:
              break;
}

  return IRQ_HANDLED;
}

/*
* Initialize the module - register the IRQ handler
*/
static int __init irq_ex_init(void)
{
    HANDLER_KEY = (void *)(irq_handler);
    /*
    * Request IRQ 1, the keyboard IRQ, to go to our irq_handler.
    */
    return request_irq (1, (irq_handler_t) irq_handler,IRQF_SHARED, "test_keyboard_irq_handler", HANDLER_KEY);
}

static void __exit irq_ex_exit(void)
{
    printk( KERN_INFO "! Module is unload... \n");
    free_irq(1, HANDLER_KEY);
}

module_init(irq_ex_init);
module_exit(irq_ex_exit);
