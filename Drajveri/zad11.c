// Да се напише модул (драјвер) кој работи со големина од 1КВ. Драјверот дозволува
// последователно запишување и читање од меморијата во принцип на кружен бафер, така што, до
// каде што претходно запишал/прочитал, продолжува од таму со запишување/читање. Не може
// да се прочита повеќе од запишаното во меморијата. Се што има прочитано корисникот се смета
// за избришано од меморијата т.е. може да се запишува повторно таму. Доколку се обиде
// корисникот да се запише повеќе од што има преостанато меморија, и доколку од почеток е
// прочитано Х бајти, корисникот ја пополнува меморијата, а преостанатото го запишува од
// почеток (до каде што е прочитано т.е. кружен бафер). 

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE (10 * 1024)

char mem[BUFFER_SIZE];
int r, w;
int used;

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
    used = 0;

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
    if (used == 0) {
        return 0; 
    }

    if (count > used) {
        count = used;
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

    used -= count;
    return count;
}

ssize_t my_write(struct file *filep, const char *buffer, size_t count, loff_t *off) {

    if (count > BUFFER_SIZE) {
        count = BUFFER_SIZE; 
    }

    if (count > (BUFFER_SIZE - used)) {
        int diff = count - (BUFFER_SIZE - used);
    
        if (r + diff < BUFFER_SIZE) {
            r += diff;
        } else {
            r = r + diff - BUFFER_SIZE; 
        }
    
        used -= diff; 
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

    used += count;
    return count;
}