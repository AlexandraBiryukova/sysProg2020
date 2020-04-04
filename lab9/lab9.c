#include <linux/module.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>

MODULE_LICENSE("GPL");

static struct timer_list my_timer;
static struct timespec t;
static int start;

static struct hrtimer hr_timer;
unsigned long first = 0;
unsigned long second = 1;
static int counter = 0;
ktime_t currtime , interval, ktime;

void print_time(void) {
	printk("CURRENT TIME: %.2lu:%.2lu:%.2lu\n",
                   (t.tv_sec / 3600 + 6) % (24),
                   (t.tv_sec / 60) % (60),
                    t.tv_sec % 60);
	printk("JIFFIES VALUE: %ld (Hours: %ld Minutes: %ld Seconds: %ld)\n",
	                jiffies, 
			jiffies / HZ / 3600,
			jiffies / HZ / 60,
			jiffies / HZ);
	printk("-------------------------------\n");
}

enum hrtimer_restart timer_callback( struct hrtimer *timer_for_restart )
{
	getnstimeofday(&t);
	unsigned long sum = first + second;
	first = second;
	second = sum;
  	currtime  = ktime_get();
  	interval = ktime_set(second,0);
  	hrtimer_forward(timer_for_restart, currtime , interval);
	counter++;
	printk("Current interval: %ld", first);
	printk("Number of timer executions: %d", counter);
	print_time();
	
	return HRTIMER_RESTART;
}

void hrtimer_example(void) {
	printk("<hrtimer.h> example started");
	ktime = ktime_set( second, 0 );
	hrtimer_init( &hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
	hr_timer.function = &timer_callback;
 	hrtimer_start( &hr_timer, ktime, HRTIMER_MODE_REL );
}

void timer_function(struct timer_list *tl) {
	getnstimeofday(&t);
	print_time();
	mod_timer(tl, jiffies + msecs_to_jiffies(1000));
	if ( jiffies / HZ - start >= 11) {
		printk("<timer.h> example ended");
		del_timer(&my_timer);
		hrtimer_example();
	}
}

void timerExample(void) {
	printk("<timer.h> example started");
	start = jiffies / HZ;
	my_timer.expires = jiffies + msecs_to_jiffies(1000);
        timer_setup(&my_timer, timer_function, 0);
	add_timer(&my_timer);
}

int init_module(void)
{
	printk("LAB 9 started\n");
        timerExample();

	return 0;
}

void cleanup_module(void)
{
	del_timer(&my_timer);
	hrtimer_cancel(&hr_timer);
	printk("<hrtimer.h> example ended");
	printk("Lab 9 stopped\n");
}
