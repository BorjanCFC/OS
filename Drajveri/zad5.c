// Да се напише драјвер за комуникација со уред. Уредот има меморија од 10KB, од која
// првата половина се користи за читање, додека пак втората половина за запишување.
// Податоците во меморијата се запишуваат/читаат во вид на кружен бафер. Доколку
// корисникот се обиде да прочита од уредот N бајти, тогаш драјверот ги чита првите N бајти.
// При следното читање на М бајти од страна на корисникот, драјверот ги чита бајтите од
// N+1 до N+M+1, доколку стигне до крај на меморијата, драјверот почнува од почеток да
// чита. Податоците се запишуваат во меморијата на ист начин. 

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE (10 * 1024)
#define READ_LIMIT (5 * 1024) 
#define WRITE_START (5 * 1024)  

char mem[BUFFER_SIZE];
int r, w; 

ssize_t my_read(struct file* filep, char* buffer, size_t count, loff_t* off);
ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off);

struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
};

static int r_init(void) {
    r = 0;
    w = WRITE_START;

    if (register_chrdev(60, "primer", &my_fops)) {
        printk(KERN_ERR "Neuspesna registracija\n");
        return -1;
    }
}

static void r_cleanup(void) {
    printk(KERN_INFO "Sayonara\n");
    unregister_chrdev(60, "primer");
}

ssize_t my_read(struct file *filep, char *buffer, size_t count, loff_t *off) {
    if (count > READ_LIMIT) {
        count = READ_LIMIT; 
    }

    if ((r + count) <= READ_LIMIT) {
        if (copy_to_user(buffer, mem + r, count)) {
            return -EFAULT;
        }
        r += count;
    } else {
        int preostanati = READ_LIMIT - r;
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
    if (count > (BUFFER_SIZE - WRITE_START)) {
        count = BUFFER_SIZE - WRITE_START; 
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
        if (copy_from_user(mem + WRITE_START, buffer + preostanati, novi)) {
            return -EFAULT;
        }

        w = WRITE_START + novi; 
    }

    return count;
}
