// Да се напише модул (драјвер) кој работи со големина од 1КВ. Драјверот дозволува
// последователно запишување и читање од меморијата, така што, до каде што претходно
// запишал/прочитал, продолжува од таму со запишување/читање. Не може да се прочита повеќе
// од запишаното во меморијата. Доколку се обиде корисникот да се запише повеќе од што има
// преостанато меморија, се ресетираат бројачите, запишувањето и читањето почнува од почеток
// (тековно што сакал да запише корисникот, ако надминува преостанатата меморија, целото се
// запишува од почеток и бројачот за прочитано се ресетира). За секоја изведена операција
// (читање или запишување), драјверот следните 5 секунди не дозволува никаква операција.
// Дообјаснување: за пресметка за поминати секунди, може да се користи функцијата:
// #include <linux/time.h>
// void getnstimeofday(struct timespec *ts)
// Функцијата на влез ја добива структурата:
// struct timespec {
//  time_t tv_sec; /* seconds */
//  long tv_nsec; /* nanoseconds */
// };
// каде преку tv_sec враќа моменталното време во секунди. 

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/time.h>

#define BUFFER_SIZE 1024
char mem[BUFFER_SIZE];
int r, w;

struct timespec pauseTime;

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
    getnstimeofday(&pauseTime);

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

ssize_t my_read(struct file* filep, char* buffer, size_t count, loff_t* off) {
    struct timespec current_time;
    getnstimeofday(&current_time);

    if (current_time.tv_sec - pauseTime.tv_sec < 5) {
        return 0; 
    }

    pauseTime = current_time; 

    if (r >= w) {
        return 0; 
    }

    if (count > w - r) {
        count = w - r; 
    }

    if (copy_to_user(buffer, mem + r, count)) {
        return -EFAULT;
    }

    r += count;
    return count;
}

ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off) {
    struct timespec current_time;
    getnstimeofday(&current_time);


    if (current_time.tv_sec - pauseTime.tv_sec < 5) {
        return 0;
    }

    pauseTime = current_time; 

    if (count > BUFFER_SIZE - w) {
        w = 0;
        r = 0; 
    }

    if (copy_from_user(&mem[w], buffer, count)) {
        return -EFAULT;
    }

    w += count;
    return count;
}
