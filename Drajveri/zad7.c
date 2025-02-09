// Да се напише драјвер за комуникација со уред. Уредот има меморија од 10КВ, притоа се запишува во
// драјверот секогаш од лево на десно, притоа на следното запишување продолжува од каде што претходно
// застанал. Доколку стигне до крај со запишување, почнува од почеток на меморијата. Читањето се прави
// така што најпрво се запишува во драјверот или 1 или 2, па потоа се чита од драјверот. Доколку се
// запише 1, тогаш почнува да чита од почеток на меморијата, додека пак ако запише 2, тогаш чита од
// крајот на меморијата (последните К бајти). Доколку не се запише ништо а се обидеме да прочитаме од
// драјверот, тогаш одбира случајно од кој бајт да чита, притоа мора да одбере валиден бајт (т.е. да има
// доволно податоци десно од бајтот од каде треба да се чита.
// Напомена: Да се користат следниве наредби за да се генерира случаен број од 0 до 100:
// int i, randomNumber100;
// get_random_bytes(&i, sizeof(i));
// randomNumber100 = i % 100; 

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>

#define BUFFER_SIZE (10 * 1024)

char mem[BUFFER_SIZE];
int r, w;
int last_write_size = 0; 
char read_mode = 0;  

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
    read_mode = 0;

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
    int i, randomNumber100;

    if (last_write_size == 0) {
        return 0; 
    }

    if (read_mode == 1) {
        r = 0;  
    } else if (read_mode == 2) {
        r = (w >= count) ? (w - count) : 0;  
    } else {
        get_random_bytes(&i, sizeof(i));
        randomNumber100 = i % last_write_size;
        r = (w >= randomNumber100) ? randomNumber100 : 0;  
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

    read_mode = 0;  
    return count;
}

ssize_t my_write(struct file *filep, const char *buffer, size_t count, loff_t *off) {
    char first_byte;

    if (copy_from_user(&first_byte, buffer, 1)) {
        return -EFAULT;
    }

    if (first_byte == '1') {
        read_mode = 1;
    } else if (first_byte == '2') {
        read_mode = 2;
    }

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

