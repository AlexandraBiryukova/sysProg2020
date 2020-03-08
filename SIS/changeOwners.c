
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexandra Biryukova");

static int __init changeOwners_init(void)
{
   return 0;
}


static void __exit changeOwners_exit(void)
{
   printk(KERN_ALERT "Module is unloaded\n");
}


module_init(changeOwners_init);
module_exit(changeOwners_exit);