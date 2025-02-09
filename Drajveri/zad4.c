// Да се напише знаковен модул(драјвер) за Linux оперативен систем. Модулот располага
// со меморија од 1КВ знаковна меморија, која иницијално е празна.Запишувањето и
// читањето од меморијата се прави од лево на десно, притоа за секои наредни читања /
// запишувања, продолжува од каде што има застанато со претходното читање /
// запишување, притоа не може да се прочита повеќе од што има запишано во
// меморијата.Модулот, во процесот на запишување во меморијата, дозволува да се запише
// најмногу 50 % од слободната (преостаната) меморија во модулот.Тоа значи дека
// доколку(на пример) во даден момент меморијата е исполнета 512 бајти, најмногу што ќе
// дозволи да се запише во драјверот се половина од преостанатите 512 бајти т.е. 256
// бајти.Доколку се обиде да се запише повеќе од 50 % од преостаната слободна меморија
// во модулот, модулот ќе запише само 50 % од преостанатата меморија.

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 1024

char mem[BUFFER_SIZE];
int r, w;

size_t my_read(struct file* filep, char* buffer, size_t count, loff_t* off);
ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off);

struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
};

static int r_init(void) {
    w = 0;
    r = 0;
    if (register_chrdev(60, "primer", &my_fops)) {
        printk(KERN_ALERT "Neuspesna registracija\n");
        return -1;
    }
    return 0;
}

static void r_cleanup(void) {
    printk(KERN_INFO "Sayonara\n");
    unregister_chrdev(60, "primer");
}

ssize_t my_read(struct file *filep, char *buffer, size_t count, loff_t *off) {
    if (r >= w) {
        r = w = 0; 
        return 0;
    }

    if (count > (w - r)) {
        count = w - r;
    }  

    if (copy_to_user(buffer, mem + r, count)) {
        return -EFAULT;
    }

    r += count;

    if (r == w) {
        r = w = 0;
    }

    return count;
}

ssize_t my_write(struct file *filep, const char*buffer, size_t count, loff_t *off) {
    int free_space = BUFFER_SIZE - w; 
    int max_write = free_space / 2; 

    if (max_write <= 0) {
        return -ENOSPC;
    }  

    if (count > max_write) {
        count = max_write;
    } 

    if (copy_from_user(mem + w, buffer, count)) {
        return -EFAULT;
    }

    w += count;
    return count;
}