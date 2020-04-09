/*
    Student Name: Ziteng Yang
    StudentID: 517021910683
*/

/*
    Simple hello world module
*/

#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include <linux/moduleparam.h>

#include <linux/proc_fs.h>
#include <linux/seq_file.h>

//#include <stdlib.h>
MODULE_LICENSE("Dual BSD/GPL");

#define MAX_PID 32767

static int para_int = 0; 
module_param(para_int, int, 0644);

static int para_int_array[10] = {0};
static int para_int_num = 0;
module_param_array(para_int_array, int, &para_int_num, 0644);

static char* para_chars = "default";
module_param(para_chars, charp, 0644);

// create files in "/proc"
static bool mkdir = false;
module_param(mkdir, bool, 0644);

static bool mkfile = false;
module_param(mkfile, bool, 0644);

static char* dir_name = "mydir";
module_param(dir_name, charp, 0644);

static char* file_name = "myfile";
module_param(file_name, charp, 0644);

static int proc_show(struct seq_file *m, void *v) {
 seq_printf(m, "Hello proc!\n");
 return 0;
}

static int proc_open(struct inode *inode, struct file *file) {
 return single_open(file, proc_show, NULL);
}


static const struct file_operations mkfile_fops = {
    .owner= THIS_MODULE,
    // .open = proc_open,
    // .read = seq_read,
    // //.write = seq_write,
    // .llseek = seq_lseek,
    // .release = single_release
};



static int __init init_fun(void)
{
    int i = 0;
    struct proc_dir_entry *my_dir;
    struct proc_dir_entry *my_file;
    printk(KERN_INFO "Lab01: module load!\n");
    
    printk(KERN_INFO "para_int=%d\n", para_int);
    printk(KERN_INFO "para_int_array: \n");
    for (i =0; i < para_int_num; ++i)
        printk(KERN_INFO"%d ", para_int_array[i]);
    printk(KERN_INFO "\n");
    printk(KERN_INFO "para_chars=%s\n",para_chars);
    
    if(mkdir && para_chars)
    {
        my_dir = proc_mkdir(dir_name, NULL);
        if(mkfile)
            my_file = proc_create(file_name, 0x0644, my_dir, &mkfile_fops);
    }

    return 0;
}

static void __exit exit_fun(void)
{
    printk(KERN_INFO "Lab01: module exit!\n"); 
}


module_init(init_fun);
module_exit(exit_fun);
