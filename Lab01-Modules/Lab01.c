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

#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>


//#include <linux/sem.h>
//#include <linux/list.h>
#include <linux/slab.h>

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

static bool writable = false;
module_param(writable, bool, 0644);

static char* dir_name = "mydir";
module_param(dir_name, charp, 0644);

static char* file_name = "myfile";
module_param(file_name, charp, 0644);

static char *content = NULL;

static int proc_show(struct seq_file *m, void *v) {
    seq_printf(m, "YZT's proc:\n");

    if(content!=NULL)
        seq_printf(m, content);
    else
    {
        seq_printf(m, "Content is Empty!\n");
    }
    

    return 0;
}

static int proc_open(struct inode *inode, struct file *file) {
    return single_open(file, proc_show, NULL);
}

// get content from IO
// using echo
static ssize_t proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos) {
    char *tmp = kzalloc((count+1), GFP_KERNEL);
    if(!tmp) return -ENOMEM;
    if(copy_from_user(tmp, buffer, count)){
        kfree(tmp);
        return EFAULT;
    }
    
    content = tmp;
    return count;
}


static const struct file_operations mkfile_fops = {
    .owner= THIS_MODULE,
    .open = proc_open,
    .read = seq_read,
    .write = proc_write, 
    .llseek = seq_lseek,
    .release = single_release
};

struct proc_dir_entry *my_dir;
struct proc_dir_entry *my_file;

static int __init init_fun(void)
{
    int i = 0;

    printk(KERN_INFO "Lab01: module load!\n");
    
    printk(KERN_INFO "para_int=%d\n", para_int);
    printk(KERN_INFO "para_int_array: \n");
    for (i =0; i < para_int_num; ++i)
        printk(KERN_INFO"%d ", para_int_array[i]);
    printk(KERN_INFO "\n");
    printk(KERN_INFO "para_chars=%s\n", para_chars);
    
    if(mkdir && dir_name)
    {
        my_dir = proc_mkdir(dir_name, NULL);
        if(mkfile){
            if(!writable)
                my_file = proc_create(file_name, 0x0444, my_dir, &mkfile_fops);
            else
                my_file = proc_create(file_name, 0x0666, my_dir, &mkfile_fops);
        }
    }

    return 0;
}

static void __exit exit_fun(void)
{
    if(mkdir && para_chars)
    {
        if (mkfile)
            remove_proc_entry(file_name, my_dir);
        remove_proc_entry(dir_name, NULL);
    }

    kfree(content);
    printk(KERN_INFO "Lab01: module exit!\n"); 

}


module_init(init_fun);
module_exit(exit_fun);
