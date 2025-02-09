// Да се напише Модул за Линукс ОС за запишување на податоци.Модулот работи со
// меморија од 512 бајти, притоа не дозволува да се чита од меморијата додека истата не е
// целосно пополнета и обратно.Иницијално, при вклучување на модулот, меморијата е
// празна.Додека меморијата не се пополни целосно, модулот дозволува само запишување
// во меморијата.Откако меморијата ќе се пополни целосно, модулот дозволува само читање
// од меморијата, притоа, читањето го прави од десно на лево.Доколку се побараат 10 бајти
// да бидат прочитани, тие ќе бидат последните 10 бајти од меморијата.На следното читање,
// ќе се вратат предпоследните count бајти и т.н.се додека не се прочита целата меморија до
// почеток.Потоа, системот повторно дозволува само запишување во меморијата до нејзино
// целосно исполнување. 

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 512

char mem[BUFFER_SIZE];
int r, w;
int readWrite;  // 1 = write, 0 = read

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
    readWrite = 1; 

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
    if (readWrite) { 
        printk(KERN_INFO "Read not allowed: Buffer is not full yet\n");
        return -EPERM;
    }

    if (r >= count) { 
        if (copy_to_user(buffer, mem + r - count, count)) {
            return -EFAULT;
        }
        r -= count;
        return count;
    } else { 
        int remaining = r;
        if (copy_to_user(buffer, mem, remaining)) {
            return -EFAULT;
        }
        r = 0;
        w = 0;
        readWrite = 1; 
        return remaining;
    }
}

ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off) {
    if (!readWrite) { 
        printk(KERN_INFO "Write not allowed: Buffer needs to be read first\n");
        return -EPERM;
    }

    if ((w + count) < BUFFER_SIZE) {  
        if (copy_from_user(mem + w, buffer, count)) {
            return -EFAULT;
        }
        w += count;
        return count;
    } else {  
        int remaining = BUFFER_SIZE - w;
        if (copy_from_user(mem + w, buffer, remaining)) {
            return -EFAULT;
        }
        w = BUFFER_SIZE;
        r = BUFFER_SIZE;
        readWrite = 0;  
        return remaining;
    }
}
