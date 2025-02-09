// Да се напише драјвер во C програмскиот јазик кој што ќе овозможува контролирано
// читање на блокови од меморијата на даден уред.Меморијата на уредот е 4KB, притоа предефинирано се
// читаат по 64 бајти (блок) од меморија.На секое последователно читање на бајти од
// меморијата покажувачот се поместува во десно.Кога ќе стигне до крајот на меморијата, се
// рестартира покажувачот и почнува од почеток. Доколку се запише нешто во драјверот,
// тогаш се менува големината на блоковите што се читаат од меморија.Доколку се смени
// големината на блокот со вредност која што не е делива со 64, тогаш се чита следната
// големина најблиску до број делив со 64 (пример: за блок од 130 бајти се читаат 192 бајти
// меморија).Доколку се внесе вредност поголема од 4K, тогаш најмногу се чита 4К.

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

char mem[1024 * 4]; 
int size;
int read_pos;

ssize_t my_read(struct file* filep, char* buffer, size_t count, loff_t* off);
ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off);

struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
};

static int r_init(void) {
    size = 64;
    read_pos = 0;

    if (register_chrdev(60, "primer", &my_fops)) {  
        printk(KERN_ERR "Neuspesna registracija\n");
        return -1;
    }

    return 0;
}

static void r_cleanup(void) {
    printk(KERN_INFO "Sayonara\n");
    unregister_chrdev(60, "primer");
}

ssize_t my_read(struct file* filep, char* buffer, size_t count, loff_t* off) {
    int bytesToRead;

    if (count > size) {
        bytesToRead = size;
    } else {
        bytesToRead = count;
    }

    if (read_pos + bytesToRead > 1024 * 4) {  
        bytesToRead = (1024 * 4) - read_pos;  
    }

    if (copy_to_user(buffer, mem + read_pos, bytesToRead)) {
        return -EFAULT;
    }

    read_pos += bytesToRead;

    if (read_pos >= 1024 * 4) {  
        read_pos = 0; 
    }

    return bytesToRead;
}

ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off) {
    char pom[100];

    if (copy_from_user(pom, buffer, count)) {
        return -EFAULT;
    }

    int ns = simple_strtol(pom, NULL, 10);  

    if (ns <= 0) {
        size = 64;
    } else if (ns > 4096) {
        size = 4096;
    } else if (ns % 64 > 0) {
        size = (ns / 64 + 1) * 64;  
    } else {
        size = ns;
    }

    return count;
}

