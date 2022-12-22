#include <linux/kernel.h>

asmlinkage long sys_hello(void) {
        printk("Greetings from kernel and team No32\n");
        return 0;
} 
