#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Anton Hrusha <anton25348@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int hello_count = 1;
module_param(hello_count, uint, 0444);

MODULE_PARM_DESC(hello_count, "How many times to display 'Hello, world!'");

struct event_data {
	struct list_head node;
	ktime_t timestamp;
};

static LIST_HEAD(event_list);

static int __init hello_init_module(void)
{
	int i;

	if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10)) {
		pr_warn("Wrong number of hello_count.\n");
	} else if (hello_count > 10) {
		pr_err("Wrong number of hello_count. This module is not loaded.\n");
		BUG();
	}

	for (i = 0; i < hello_count; i++) {
		struct event_data *event_item = kmalloc(sizeof(*event_item), GFP_KERNEL);
		if (i == 4) {
			event_item = 0;
		}
		event_item->timestamp = ktime_get();
		list_add(&event_item->node, &event_list);
		printk(KERN_EMERG "Hello, world!\n");
	}
	return 0;
}

static void __exit hello_exit_module(void)
{
	struct event_data *event_item, *tmp;

	list_for_each_entry_safe(event_item, tmp, &event_list, node) {
		ktime_t duration = ktime_sub(ktime_get(), event_item->timestamp);

		pr_info("Event time: %lld ns\n", ktime_to_ns(duration));
		list_del(&event_item->node);
		kfree(event_item);
	}
}

module_init(hello_init_module);
module_exit(hello_exit_module);