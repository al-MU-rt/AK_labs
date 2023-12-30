#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Hrusha Anton <anton25348@gmail.com>");
MODULE_DESCRIPTION("HELLO_WORLD_KERNEL_MODULE");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int REPEAT_COUNT = 1;
module_param(REPEAT_COUNT, uint, 0644);
MODULE_PARM_DESC(REPEAT_COUNT, "Скільки разів вивести рядок 'Hello, world!'");

struct EVENT_DATA {
	struct list_head LIST;
	ktime_t TIMESTAMP;
};

static LIST_HEAD(EVENT_LIST);

static int __init hello_init(void)
{
	if (REPEAT_COUNT == 0 || (REPEAT_COUNT >= 5 && REPEAT_COUNT <= 10)) {
		pr_warn("Невірне значення параметра.\n");
	} else if (REPEAT_COUNT > 10) {
		pr_err("Помилкове значення параметра. Модуль не завантажено.\n");
		return -EINVAL;
	}

	struct EVENT_DATA *event = kmalloc(sizeof(*event), GFP_KERNEL);

	event->TIMESTAMP = ktime_get();
	list_add(&event->LIST, &EVENT_LIST);

	int i;

	for (i = 0; i < REPEAT_COUNT; i++)
		printk(KERN_EMERG "Hello, world!\n");

	return 0;
}

static void __exit hello_exit(void)
{
	struct EVENT_DATA *event, *temp;

	list_for_each_entry_safe(event, temp, &EVENT_LIST, LIST) {
		ktime_t duration = ktime_sub(ktime_get(), event->TIMESTAMP);

		pr_info("Час події: %lld нс\n", ktime_to_ns(duration));
		list_del(&event->LIST);
	kfree(event);
	}
}

module_init(hello_init);
module_exit(hello_exit);
