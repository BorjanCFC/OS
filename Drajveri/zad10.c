// Да се напише модул за работа со меморија. Модулот има три мемории од 512 бајти. Кога некој ќе се
// обиде да запише во модулот, се запишува паралелно во сите три мемории истиот податок, притоа секое
// следно запишување продолжува од каде што прекинало претходното (append тип на запишување).
// Доколку стигне на крајот од меморијата, почнува да запишува од почеток, игнорирајќи го остатокот од
// меморијата.
// При читање од драјверот, на првото (четвртото, седмо, десетто и т.н.) читање се чита од првата
// меморија, на второто (петто, осмо, единаесето и т.н.) читање од втората меморија, и на трето (шесто,
// деветто, дванаесето и т.н.) се чита од третата меморија. Притоа, кога е четвртото читање (тоа значи од
// првата меморија), тоа продолжува да чита од таму од каде што прекинало претходното читање на таа
// меморија (во случајот првото читање) и т.н.

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 512

char mem1[BUFFER_SIZE];
char mem2[BUFFER_SIZE];
char mem3[BUFFER_SIZE];

int w;
int r[3];
int rc;

ssize_t my_read(struct file* filep, char* buffer, size_t count, loff_t* off);
ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off);

struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
};

static int my_init(void) {

    w = 0;
    r[0] = r[1] = r[2] = 0;
    rc = 0;
    last_write_size = 0;

    if(register_chrdev(60, "primer", &my_fops)) {
        printk(KERN_ERR "Neuspesna registracija");
        return -1;
    }
}

static void r_cleanup(void) {
    unregister_chrdev(0, "primer");
    printk(KERN_INFO "Sayonara");
}

ssize_t my_read(struct file* filep, char* buffer, size_t count, loff_t* off) {
    if(w == 0) {
        return 0;
    }

    char *mem;
    int index = rc % 3;
    rc++;

    if (index == 0) {
        mem = mem1;
    } else if (index == 1) {
        mem = mem2;
    } else {
        mem = mem3;
    }

    int available_bytes;

    if (w >= r) {
        available_bytes = w - r;
    } else {
        available_bytes = BUFFER_SIZE - r + w;
    }

    if (available_bytes == 0) {
        return 0;
    }

    if (count > available_bytes) {
        count = available_bytes;
    }

    if((r[index] + count) <= BUFFER_SIZE) {
        if(copy_to_user(buffer, mem + r[index], count)) {
            return -EFAULT;
        }

        r[index] += count;
    } else {
        int preostanati = BUFFER_SIZE - r[index];
        int novi = count - preostanati;

        if(copy_to_user(buffer, mem + r[index], preostanati)) {
            return -EFAULT;
        }
        if (copy_to_user(buffer + preostanati, mem, novi)) {
            return -EFAULT;
        }

        r[index] = novi;
    }

    return count;
}

ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off) {
    if (count > BUFFER_SIZE) {
        count = BUFFER_SIZE; 
    }

    if ((w + count) <= BUFFER_SIZE) {
        if (copy_from_user(mem1 + w, buffer, count)) {
            return -EFAULT;
        }
        if (copy_from_user(mem2 + w, buffer, count)) {
            return -EFAULT;
        }
        if (copy_from_user(mem3 + w, buffer, count)) {
            return -EFAULT;
        }
        w += count;
    } else {
        int preostanati = BUFFER_SIZE - w;
        int novi = count - preostanati;

        if (copy_from_user(mem1 + w, buffer, preostanati)) {
            return -EFAULT;
        }
        if (copy_from_user(mem2 + w, buffer, preostanati)) {
            return -EFAULT;
        }
        if (copy_from_user(mem3 + w, buffer, preostanati)) {
            return -EFAULT;
        }

        if (copy_from_user(mem1, buffer + preostanati, novi)) {
            return -EFAULT;
        }
        if (copy_from_user(mem2, buffer + preostanati, novi)) {
            return -EFAULT;
        }
        if (copy_from_user(mem3, buffer + preostanati, novi)) {
            return -EFAULT;
        }

        w = novi;
    }

    last_write_size = count;

    return count;
}
