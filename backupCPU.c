#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/tick.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

struct task_struct *task;           //Estructura definida en sched.h para tareas
struct task_struct *task_child;     //Estructura definida en sched.h para tareas secundarias
struct list_head *list;             //Estructura necesaria para recorrer la lista en cada tarea

static int write_file(struct seq_file * archivo, void *v){
    seq_printf(archivo, "**************************************************\n");
    seq_printf(archivo, "*****      Sergio Geovany Leal Cardona       *****\n");
    seq_printf(archivo, "*****      Diciembre 2020                    *****\n");
    seq_printf(archivo, "**************************************************\n");
    return 0;
}

static int al_abrir(struct inode *inode, struct file *file){
    return single_open(file, write_file, NULL);
}

static struct file_operations operaciones = 
{
    .open = al_abrir,
    .read = seq_read
};

//Modulo de inicio
int iniciar(void)
{
    proc_create("cpu_201503953",0,NULL,&operaciones);
    printk(KERN_INFO "CARGANDO MODULO\n");
    printk(KERN_INFO "Sergio Geovany Leal Cardona\n");
    printk(KERN_INFO "Diciembre 20202\n");

    for_each_process(task){
        printk(KERN_INFO "\nPADRE PID: %d PROCESO: %s ESTADO: %ld", task->pid, task->comm, task->state);
        list_for_each(list, &task->children){
            task_child = list_entry(list, struct task_struct, sibling);
            printk(KERN_INFO "\nHIJO DE %s[%d] PID: %d PROCESO: %s ESTADO: %ld", task->comm, task->pid, 
                task_child->pid, task_child->comm, task_child->state);
        }
        printk("\n*---------------------------------------------\n");
    }
    return 0;
}              

void salir(void){
    remove_proc_entry("cpu_201503953", NULL);
    printk(KERN_INFO "Removing ListTasksLinear\n");
}

module_init(iniciar);
module_exit(salir);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("List tasks linearly");
MODULE_AUTHOR("Sergio Leal, 201503953");