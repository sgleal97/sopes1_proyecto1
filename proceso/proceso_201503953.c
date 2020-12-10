#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>

static int meminfo_proc_show(struct seq_file *m, void *v){
    struct task_struct *task;
    struct mm_struct *mm;
    long memori = 0;
    int cont = 0;
    struct task_struct *task_child;
    struct list_head *list;
    seq_printf(m, "{ \"procesos\": [\n");

    const struct cred *tcred;
    for_each_process(task){
        memori = 0;
        tcred = __task_cred(task);
        mm = task->mm;
        if(mm){
            memori = (mm->total_vm<<(PAGE_SHIFT-10));
        }
        if(cont>0){
            seq_printf(m, ",");
        }
        #define K(x)((x)<<(PAGE_SHIFT-10))
        seq_printf(m,"{\n");
        seq_printf(m, "\"Usuario\": \"%d\", \"Memoria\": \"%ld\", \"Nombre\": \"%s\", \"PID\": \"%d\", \"Estado\": \"%ld\", \"Salida\": \"%d\"",
        tcred->uid,
        memori,
        task->comm,
        task->pid,
        task->state,
        task->exit_state);

        cont = cont + 1;
        seq_printf(m, ",\"hijos\": \"");
        list_for_each(list, &task->children){
            task_child = list_entry(list, struct task_struct, sibling);
            seq_printf(m, "%d;", task_child->pid);
        }
        seq_printf(m, "\"\n");
        seq_printf(m, " }");
    }
    seq_printf(m, "]\n");
    seq_printf(m, " }");

    return 0;
}

static int meminfo_proc_open(struct inode *inode, struct file *file){
    return single_open(file, meminfo_proc_show, NULL);
}

static const struct file_operations meminfo_proc_fops = {
    .open = meminfo_proc_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

static int __init inicio(void){
    printk(KERN_INFO "Sergio Leal\n");
    printk(KERN_INFO "Proceso inicio\n");
    proc_create("proceso_201503953", 0, NULL, &meminfo_proc_fops);
    return 0;
}

static void __exit Terminar(void){
    remove_proc_entry("proceso_201503953", NULL);
    printk(KERN_INFO "Sistemas Operativos 1.\n");
    printk(KERN_INFO "Proceso Fin\n");
}

module_init(inicio);
module_exit(Terminar);