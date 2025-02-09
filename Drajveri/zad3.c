// Да се напише драјвер за комуникација со уред.Уредот има меморија од 10КВ,
// поделена во два дела(6 КВ се за читање и запишување, 4 КВ се резервирани за
// дополнителнокористење).Податоците во меморијата се запишуваат од лево на десно и
// доколку нема слободна меморија, не се запишува ништо.Доколку корисникот се обиде да
// запише N Бајти меморија, тие ги запишува во слободниот простор во кој нема веќе
// запишано меморија.Доколку има помалку од N бајти слободна меморија, тие податоци
// воопшто не се запишуваат во уредот. Читањето од уредот се прави од лево на десно, така
// што доколку корисникот се обиде да прочита N бајти од уредот, се читаат последните
// запишани N бајти во уредот почнувајќи од последниот.Доколку има преостанато помалку
// од N бајти за читање, тогаш се враќаат само преостанатите бајти.Кога ќе се прочитаат
// N(или помалку од N бајти) од уредот, истите се бришат(т.е.се поставуваат како слободни
// за запишување).

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE (1024 * 6)  
char mem[BUFFER_SIZE];
int w = 0;  
int used = 0; 

ssize_t my_read(struct file* filep, char* buffer, size_t count, loff_t* off);
ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off);

struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
};

static int r_init(void) {
    w = 0;
    used = 0;
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
    if (used == 0) {
        return 0;  
    }

    int bytesToRead = (count > used) ? used : count;
    int startIndex = w - used; 

    if (copy_to_user(buffer, mem + startIndex, bytesToRead)) {
        return -EFAULT;
    }

    memset(mem + startIndex, 0, bytesToRead);  
    used -= bytesToRead; 

    if (used == 0) {
        w = 0;  
    }

    return bytesToRead;
}

ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off) {
    if (count > (BUFFER_SIZE - used)) {
        return 0;  
    }

    if (copy_from_user(mem + w, buffer, count)) {
        return -EFAULT;
    }

    w += count;
    used += count;
    return count;
}

