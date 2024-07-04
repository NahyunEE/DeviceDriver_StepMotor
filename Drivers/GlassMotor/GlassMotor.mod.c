#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x92997ed8, "_printk" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x5f754e5a, "memset" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x17b8e2f4, "gpio_to_desc" },
	{ 0xbaa0f895, "gpiod_unexport" },
	{ 0xfe990052, "gpio_free" },
	{ 0x695bf5e9, "hrtimer_cancel" },
	{ 0xd910fabf, "device_destroy" },
	{ 0x25775ba2, "class_destroy" },
	{ 0xf1ff7262, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xe66b7d3b, "cdev_init" },
	{ 0x1341b5e, "cdev_add" },
	{ 0x7b7fcf8d, "__class_create" },
	{ 0xeed31c02, "device_create" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xd8ecb590, "gpiod_direction_output_raw" },
	{ 0xa23c1219, "gpiod_export" },
	{ 0xa362bf8f, "hrtimer_init" },
	{ 0xec523f88, "hrtimer_start_range_ns" },
	{ 0x7f8b7106, "gpiod_set_raw_value" },
	{ 0x5cc2a511, "hrtimer_forward" },
	{ 0x78a319e7, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "BE49561B2570F67403D1940");
