#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/sysinfo.h>
#include <linux/seq_file.h>
#include <linux/swap.h>

static int meminfo_proc_show(struct seq_file *m, void *v){
    struct sysinfo sys_info;
    unsigned long totalram;
    unsigned long freeram;
    unsigned long available;
    unsigned long resta;
    unsigned long multiplicacion;
    unsigned long usage;
    si_meminfo(&sys_info);

    totalram = sys_info.totalram*(unsigned long long)sys_info.mem_unit / 1024;
    freeram = sys_info.freeram *(unsigned long long)sys_info.mem_unit/ 1024;
    available = si_mem_available() *(unsigned long long)sys_info.mem_unit/ 1024;
    
    resta = totalram-freeram;
    multiplicacion = resta*100;

    usage = multiplicacion/totalram;
    seq_printf(m, "{\n\"Total\": %ld,\n\"Libre\": %ld,\n\"Usada\": %ld\n}\n", totalram,freeram,usage);
    
    return 0;
}

static ssize_t proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos){
    return 0;
}
static int meminfo_proc_open(struct inode *inode, struct file *file){
    return single_open(file, meminfo_proc_show, NULL);
}
static const struct file_operations meminfo_proc_fops = {
    .owner = THIS_MODULE,
    .open = meminfo_proc_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
    .write = proc_write
};

static int __init inicio(void){
    
    struct proc_dir_entry *entry;
    entry = proc_create("memo_201503953",0,NULL, &meminfo_proc_fops);
    if(!entry){
        return -1;
    }else{
        printk(KERN_INFO "201503953\n");
    }
    return 0;
}

static void __exit Terminar(void){
    remove_proc_entry("memo_201503953", NULL);
    printk(KERN_INFO "Sistemas Operativos 1\n");
}

MODULE_AUTHOR("Sergio Leal");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modulo para calcular el uso de RAM");
module_init(inicio);
module_exit(Terminar);