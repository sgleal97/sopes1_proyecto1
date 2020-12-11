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

    int ejecutando = 0;
    int detenido = 0;
    int suspendido = 0;
    int zombie = 0;
    int desc = 0;

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
        seq_printf(m, "\"Usuario\": \"%d\",\n",tcred->uid);
        seq_printf(m, "\"Memoria\": \"%ld\",\n", memori);
        seq_printf(m, "\"Nombre\": \"%s\",\n", task->comm);
        seq_printf(m, "\"PID\": \"%d\",\n", task->pid);
        seq_printf(m, "\"Estado\": \"%ld\",\n", task->state);
        seq_printf(m, "\"Salida\": \"%d\",\n", task->exit_state);

        if(task->state == 0){
            ejecutando = ejecutando + 1;
        }else if(task->state == 1){
            suspendido = suspendido + 1;
        }else if(task->state == 4){
            detenido = detenido + 1;
        }else if(task->state == 32){
            zombie = zombie + 1;
        }else if(task->state == 1026){
            suspendido = suspendido + 1;
        }

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
    seq_printf(m, "*Ejecutandose: %d|", ejecutando);
    seq_printf(m, " Suspendido: %d|", suspendido);
    seq_printf(m, " Detenido: %d|", detenido);
    seq_printf(m, " Zombie: %d|", zombie);
    seq_printf(m, " Desconocido: %d|", desc);
    seq_printf(m, " Total: %d", cont);
    seq_printf(m, " }\n");

    

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