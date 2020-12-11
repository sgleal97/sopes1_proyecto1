#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/tick.h>
#include <linux/delay.h>

//Metodos obtenidos de stat.c
static u64 get_idle_time(int cpu)
{
	u64 idle, idle_time = -1ULL;

	if (cpu_online(cpu))
		idle_time = get_cpu_idle_time_us(cpu, NULL);

	if (idle_time == -1ULL)
		idle = kcpustat_cpu(cpu).cpustat[CPUTIME_IDLE];
	else
		idle = nsecs_to_jiffies64(idle_time);

	return idle;
}

//Metodos obtenidos de stat.c
static u64 get_iowait_time(int cpu)
{
	u64 iowait, iowait_time = -1ULL;

	if (cpu_online(cpu))
		iowait_time = get_cpu_iowait_time_us(cpu, NULL);

	if (iowait_time == -1ULL)
		iowait = kcpustat_cpu(cpu).cpustat[CPUTIME_IOWAIT];
	else
		iowait = nsecs_to_jiffies64(iowait_time);

	return iowait;
}


static int cpu_stat_show(struct seq_file *m, void *v){
    int i;
    u64 Total = 0;
	unsigned long long total_jiffies_1, total_jiffies_2;
	unsigned long long work_jiffies_1, work_jiffies_2;
	unsigned int work_over_period, total_over_period;
	unsigned int usage;

    u64 user, nice, system, idle, iowait, irq, softirq, steal;
 	u64 guest, guest_nice;
    
    user = nice = system = idle = iowait = irq = softirq = steal = 0;
	guest = guest_nice = 0;
    //recorremos todos los cpus y sumamos sus procesos
    for_each_possible_cpu(i) {
		user += kcpustat_cpu(i).cpustat[CPUTIME_USER];
		nice += kcpustat_cpu(i).cpustat[CPUTIME_NICE];
		system += kcpustat_cpu(i).cpustat[CPUTIME_SYSTEM];
		idle += get_idle_time(i);
		iowait += get_iowait_time(i);
		irq += kcpustat_cpu(i).cpustat[CPUTIME_IRQ];
		softirq += kcpustat_cpu(i).cpustat[CPUTIME_SOFTIRQ];
		steal += kcpustat_cpu(i).cpustat[CPUTIME_STEAL];
		guest += kcpustat_cpu(i).cpustat[CPUTIME_GUEST];
		guest_nice += kcpustat_cpu(i).cpustat[CPUTIME_GUEST_NICE];
	}
    Total = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
	work_jiffies_1 = jiffies_64_to_clock_t(user+nice+system);
	total_jiffies_1 = jiffies_64_to_clock_t(Total);
    //delay para obtener otra medicion
	mdelay(200);
	user = nice = system = idle = iowait = irq = softirq = steal = 0;
	guest = guest_nice = 0;
    for_each_possible_cpu(i) {
		user += kcpustat_cpu(i).cpustat[CPUTIME_USER];
		nice += kcpustat_cpu(i).cpustat[CPUTIME_NICE];
		system += kcpustat_cpu(i).cpustat[CPUTIME_SYSTEM];
		idle += get_idle_time(i);
		iowait += get_iowait_time(i);
		irq += kcpustat_cpu(i).cpustat[CPUTIME_IRQ];
		softirq += kcpustat_cpu(i).cpustat[CPUTIME_SOFTIRQ];
		steal += kcpustat_cpu(i).cpustat[CPUTIME_STEAL];
		guest += kcpustat_cpu(i).cpustat[CPUTIME_GUEST];
		guest_nice += kcpustat_cpu(i).cpustat[CPUTIME_GUEST_NICE];
	}
    Total = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
	work_jiffies_2 = jiffies_64_to_clock_t(user+nice+system);
	total_jiffies_2 = jiffies_64_to_clock_t(Total);

	work_over_period = work_jiffies_2-work_jiffies_1;
	total_over_period = total_jiffies_2-total_jiffies_1;
	usage = work_over_period*100/total_over_period;

	
	seq_printf(m, "{\n\"CPU\":%d%%\n}\n",usage);
    return 0;
}

static ssize_t proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos){
    return 0;
}
static int meminfo_proc_open(struct inode *inode, struct file *file){
    return single_open(file, cpu_stat_show, NULL);
}
static const struct file_operations meminfo_proc_fops = {
    .owner = THIS_MODULE,
    .open = meminfo_proc_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
    .write = proc_write
};

static int __init Inicio(void){
    
    struct proc_dir_entry *entry;
    entry = proc_create("cpu_201503953",0,NULL, &meminfo_proc_fops);
    if(!entry){
        return -1;
    }else{
        printk(KERN_INFO "Sergio Geovany Leal Cardona\n");
        return 0;
    }
}

static void __exit Terminar(void){
    remove_proc_entry("cpu_201503953", NULL);
    printk(KERN_INFO "Diciembre 2020\n");
}

MODULE_AUTHOR("Sergio Leal");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modulo para calcular el uso de CPU");
module_init(Inicio);
module_exit(Terminar);