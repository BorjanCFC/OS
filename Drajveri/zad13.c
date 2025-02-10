// Да се напише Модул за Линукс ОС за запишување на податоци. Модулот има првична меморија од
// 1024KB и огледална (mirror) меморија од исто толку KB. Иницијално меморијата е празна. Кога
// првичната меморија не е полна, се запишува во таа меморија сите податоци што се запишуваат во
// модулот, а се прави дополнителна копија во огледалната меморија (од крајот кон почетокот). Во
// моментот кога ќе се наполни оригиналната меморија, огледалната меморија служи како дополнителна
// меморија, така што дополнителните податоци се запишуваат таму почнувајќи од почеток. Кога ќе се
// прочита од модулот, доколку огледалната меморија има копија од првичната меморија се прави
// проверка дали тоа што се чита е исто и во првичната и во огледалната (Double Check). Доколку е исто,
// на корисникот му се враќа информација дека меморијата е Double Checked. Доколку не е исто, на
// корисникот се враќа информација дека имало Double Check ама истиот не е успешно извршен. Како трет
// случај се зема кога огледалната меморија се користи како дополнителна меморија, тогаш се враќа
// информација на корисникот дека не е возможно да се направи Double Check. 

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/time.h>

#define BUFFER_SIZE 10 * 1024
char memOrig[BUFFER_SIZE];
char memMirr[BUFFER_SIZE];
int r, w;
int used;

ssize_t my_read(struct file* filep, char* buffer, size_t count, loff_t* off);
ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off);

struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
    .init = r_init,
    .release = r_cleanup,
};

static int r_init(void) {
    r = 0;
    w = 0;
    used = 0

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
    int i;

    if (used == 0)
        return 0; 

    if (count > used)
        count = used; 

    for (i = 0; i < count; i++) {
        buffer[i] = memOrig[r];

        if (r < BUFFER_SIZE) {
            if (memMirr[BUFFER_SIZE - r - 1] == memOrig[r]) {
                printk(KERN_INFO "Double Checked: OK\n");
            } else {
                printk(KERN_INFO "Double Checked: FAIL\n");
            }
        } else {
            printk(KERN_INFO "Double Check not possible\n");
        }

        r++;
        if (r >= 2 * BUFFER_SIZE) {
            r = 0;
        }
        used--;
    }

    return count;
}

ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off) {
    int i;

    if (count + used > 2 * BUFFER_SIZE) {
        count = 2 * BUFFER_SIZE - used; 
    }

    for (i = 0; i < count; i++) {
        if (used < BUFFER_SIZE) {
            memOrig[w] = buffer[i];

            memMirr[BUFFER_SIZE - w - 1] = buffer[i];
        } else {
            memMirr[w - BUFFER_SIZE] = buffer[i];
        }

        w++;
        if (w >= 2 * BUFFER_SIZE) {
            w = 0;
        }
        used++;
    }

    return count;
}
