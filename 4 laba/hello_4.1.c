// SPDX-License-Identifier: Dual BSD/GPL
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"


MODULE_AUTHOR("Anton Hrusha <anton25348@gmail.com>");
MODULE_DESCRIPTION("Hello, world from Linux Kernel Try Hello1");
MODULE_LICENSE("Dual BSD/GPL");

struct event_data {
	struct list_head list_node;
	ktime_t timestamp;
	ktime_t print_duration;
};

static LIST_HEAD(Head_Node);

void print_hello(unsigned int hello_count)
{
	int i;

	for (i = 0; i < hello_count; i++) {
		struct event_data *md = kmalloc(sizeof(*md), GFP_KERNEL);

		md->timestamp = ktime_get();

		pr_info("Hello, world!\n");

		md->print_duration = ktime_sub(ktime_get(), md->timestamp);

		list_add(&md->list_node, &Head_Node);
	}
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("Hello1 module initialized.\n");
return 0;
}

static void __exit hello1_exit(void)
{
	struct event_data *md, *tmp;

	list_for_each_entry_safe(md, tmp, &Head_Node, list_node) {
		ktime_t duration = ktime_sub(ktime_get(), md->timestamp);

		pr_info("Event time: %lld ns\nPrinting time : %lld ns\n",
			ktime_to_ns(duration), ktime_to_ns(md->print_duration));
		list_del(&md->list_node);
		kfree(md);
	}
	pr_info("Hello1 module exited.\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
