#include<linux/module.h>
#include<linux/init.h>
#include<linux/proc_fs.h>
#include<linux/sched.h>
#include<linux/uaccess.h>
#include<linux/fs.h>
#include<linux/seq_file.h>
#include<linux/slab.h>
#include <linux/sysinfo.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/kernel.h>   

#include <linux/hugetlb.h>

static char *str = NULL;      
struct sysinfo inf;      
unsigned long size;


char buffer[256];
long int cont_ejecucion,cont_suspendidos,cont_detenidos,cont_zombies;

char * get_task_state(long state,bool sumarTipo)
{
    switch (state) {
        case TASK_RUNNING:
            if (sumarTipo)cont_ejecucion+=1;
            return "TASK_RUNNING";
        case 1:
            if (sumarTipo)cont_suspendidos+=1; 
             return "s(sleeping)";
        case TASK_UNINTERRUPTIBLE:
            if (sumarTipo)cont_suspendidos+=1; 
            return "I(TASK_UNINTERRUPTIBLE)";
        case __TASK_STOPPED:
            if (sumarTipo)cont_detenidos+=1; 
            return "__TASK_STOPPED";
        case __TASK_TRACED:
            if (sumarTipo)cont_detenidos+=1; 
            return "__TASK_TRACED";
        case TASK_PARKED :
            if (sumarTipo)cont_detenidos+=1;
            return "TASK_PARKED";
        case TASK_DEAD  :
            if (sumarTipo)cont_detenidos+=1; 
            return "TASK_DEAD";
        case TASK_WAKEKILL:
            if (sumarTipo)cont_detenidos+=1; 
            return "TASK_WAKEKILL";
        case TASK_WAKING :
            if (sumarTipo)cont_suspendidos+=1; 
            return "TASK_WAKING";
        case TASK_NOLOAD :
             if (sumarTipo)cont_suspendidos+=1; 
            return "TASK_NOLOAD";
        case TASK_NEW  :
            if (sumarTipo)cont_suspendidos+=1; 
            return "TASK_NEW ";
        case TASK_STATE_MAX    :
            if (sumarTipo)cont_suspendidos+=1; 
            return "TASK_STATE_MAX ";

        case EXIT_DEAD :
            if (sumarTipo)cont_detenidos+=1; 
            return "EXIT_DEAD - EXIT_TRACE ";
        case EXIT_ZOMBIE:
            if (sumarTipo)cont_zombies+=1; 
            return "EXIT_ZOMBIE - EXIT_TRACE ";

        case 1026:
            if (sumarTipo)cont_suspendidos+=1; 
            return "I";
        default:
        {
            return "Unknown Type";
        }
    }
}


void listado_hijos(struct seq_file *m,struct task_struct *task_father){
        bool comaHijos = false;
        struct list_head *list;
        list_for_each(list, &task_father->children){    
            struct task_struct *task_child;
            task_child = list_entry( list, struct task_struct, sibling );
            if (comaHijos){seq_printf(m, "                 ,{\n");}
            else {seq_printf(m, "                 {\n"); comaHijos=true;}

            seq_printf(m, "                      \"Pid\": \"%d\",\n", task_child->pid);
            seq_printf(m, "                      \"Nombre\": \"%s\",\n", task_child->comm);
            seq_printf(m, "                      \"Usuario\": \"%d\",\n", task_child->cred->uid.val);
            seq_printf(m, "                      \"Estado\": \"%s\",\n", get_task_state(task_child->state,false));
            seq_printf(m, "                      \"ram\": \"%ld\",\n", task_child->mm?task_child->mm->total_vm:0);
            seq_printf(m, "                      \"hijos\": [\n");
            listado_hijos(m,task_child);
            seq_printf(m, "                               ]\n");

            seq_printf(m, "                 }\n"); 

         }
    


}




static int my_proc_show(struct seq_file *m,void *v){
    //long total_memoria ;
    struct task_struct *task;   
    bool coma = false;
    cont_ejecucion =0;
    cont_suspendidos=0;
    cont_detenidos=0;
    cont_zombies=0;

    seq_printf(m, "{\n");
         
    seq_printf(m, "\"ProcessList\": [\n");
    for_each_process( task ){      
    //sumarTipo =true;
    //seq_printf(m,"PID: %d NOMBRE: %s ESTADO: %ld-%s ",task->pid, task->comm,  task->state,get_task_state(task->state));
    //seq_printf(m, "   \"ram\": \"%ld\"\n", task->mm?task->mm->total_vm:0);
             
    if (coma){seq_printf(m, ",   {\n");}
    else {seq_printf(m, "    {\n"); coma=true;}

    seq_printf(m, "   \"Pid\": \"%d\",\n", task->pid);
    seq_printf(m, "   \"Nombre\": \"%s\",\n", task->comm);
    seq_printf(m, "   \"Usuario\": \"%d\",\n", task->cred->uid.val); 
    seq_printf(m, "   \"Estado\": \"%s\",\n", get_task_state(task->state,true));
    seq_printf(m, "   \"ram\": \"%ld\",\n", task->mm?task->mm->total_vm:0);
    seq_printf(m, "   \"hijos\": [\n");
    listado_hijos(m,task);
    seq_printf(m, "            ]\n");
    seq_printf(m, "    }\n");            
        
    }

    seq_printf(m, "               ]\n");


    //Contadores Procesos
    seq_printf(m, ",\"ProcessInfo\": {\n");
    seq_printf(m, "   \"Total\": \"%li\",\n", cont_ejecucion+cont_suspendidos+cont_detenidos+cont_zombies);
    seq_printf(m, "   \"Ejecucion\": \"%li\",\n", cont_ejecucion);
    seq_printf(m, "   \"suspendidos\": \"%li\",\n", cont_suspendidos);
    seq_printf(m, "   \"detenidos\": \"%li\",\n", cont_detenidos);
    seq_printf(m, "   \"zombies\": \"%li\"\n", cont_zombies);

    seq_printf(m, "               }\n");
    seq_printf(m, "}\n");
    return 0;
}

static ssize_t my_proc_write(struct file* file,const char __user *buffer,size_t count,loff_t *f_pos){
    char *tmp = kzalloc((count+1),GFP_KERNEL);
    if(!tmp)return -ENOMEM;
    if(copy_from_user(tmp,buffer,count)){
        kfree(tmp);
        return EFAULT;
    }
    kfree(str);
    str=tmp;
    return count;
}

static int my_proc_open(struct inode *inode,struct file *file){
    return single_open(file,my_proc_show,NULL);
}

static struct file_operations my_fops={
    .owner = THIS_MODULE,
    .open = my_proc_open,
    .release = single_release,
    .read = seq_read,
    .llseek = seq_lseek,
    .write = my_proc_write
};

static int __init Inicio(void){
    struct proc_dir_entry *entry;
    entry = proc_create("proceso_201503953",0777,NULL,&my_fops);
    if(!entry){
        printk(KERN_INFO "Error al crear Archivo proceso_201503953\n");
        return -1;  
    }else{
        printk(KERN_INFO "Inicio proceso_201503953\n");
    }
    return 0;
}

static void __exit Terminar(void){
    remove_proc_entry("proceso_201503953",NULL);
    printk(KERN_INFO "Fin proceso_201503953\n");
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modulo para obtener las estadisticas de los procesos");
MODULE_AUTHOR("Sergio Leal");
module_init(Inicio);
module_exit(Terminar);