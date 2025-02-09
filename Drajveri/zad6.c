// Да се напише драјвер за комуникација со уред. Уредот има меморија од 10КВ, притоа се запишува во
// драјверот секогаш од лево на десно, притоа на следното запишување продолжува од каде што претходно
// застанал. Доколку стигне до крај со запишување, почнува од почеток на меморијата. Читањето се прави
// на истиот начин како и запишувањето, со тоа што, не може да прочита повеќе бајти од бројот на бајти
// последно запишани во драјверот. Пример: доколку последно се запишани 50 бајти во драјверот, а на
// читање се обиде да прочита повеќе од 50 бајти, драјверот ќе ги врати само првите 50 бајти.

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE (10 * 1024)

char mem[BUFFER_SIZE];
int r, w;
int last_write_size = 0; 

ssize_t my_read(struct file* filep, char* buffer, size_t count, loff_t* off);
ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off);

struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
};

static int r_init(void) {
    r = 0;
    w = 0;
    last_write_size = 0;

    if(register_chrdev(60, "primer", &my_fops)) {
        printk(KERN_ERR "Neuspesna registracija");
        return -1;
    }

    return 0;
}

static void r_cleanup(void) {
    printk(KERN_INFO "Sayonara");
    unregister_chrdev(60, "primer");
}

ssize_t my_read(struct file *filep, char *buffer, size_t count, loff_t *off) {
    if (last_write_size == 0) {
        return 0; 
    }

    if (count > last_write_size) {
        count = last_write_size; 
    }

    if ((r + count) <= BUFFER_SIZE) {
        if (copy_to_user(buffer, mem + r, count)) {
            return -EFAULT;
        }
        r += count;
    } else {
        int preostanati = BUFFER_SIZE - r;
        int novi = count - preostanati;

        if (copy_to_user(buffer, mem + r, preostanati)) {
            return -EFAULT;
        }
        if (copy_to_user(buffer + preostanati, mem, novi)) {
            return -EFAULT;
        }

        r = novi;
    }

    return count;
}

ssize_t my_write(struct file *filep, const char *buffer, size_t count, loff_t *off) {

    if (count > BUFFER_SIZE) {
        count = BUFFER_SIZE; 
    }

    if ((w + count) <= BUFFER_SIZE) {
        if (copy_from_user(mem + w, buffer, count)) {
            return -EFAULT;
        }
        w += count;
    } else {
        int preostanati = BUFFER_SIZE - w;
        int novi = count - preostanati;

        if (copy_from_user(mem + w, buffer, preostanati)) {
            return -EFAULT;
        }
        if (copy_from_user(mem, buffer + preostanati, novi)) {
            return -EFAULT;
        }

        w = novi;
    }

    last_write_size = count; 

    return count;
}
