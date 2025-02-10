// Да се напише драјвер за комуникација со уред. Уредот има меморија од 1КВ. Податоците во меморијата
// се заклучени иницијално со 2 клучеви така што меморијата мора да се отклучи пред истата да се користи.
// Доколку меморијата е заклучена не може да се чита ниту да се запишува во драјверот. За отклучување, во
// драјверот мора да се запише првин клучот, па доколку е точен, следната операција кон драјверот е
// овозможена. Првиот клуч има вредност „juni123“, додека пак, вториот клуч има вредност „juni!@#“.
// Доколку се запише првиот клуч, драјверот е отклучен само за една операција после клучот, и после
// повторно е заклучен истиот. Доколку се запише вториот клуч, драјверот е отклучен за повеќе операции
// после тоа, т.е. е отворен за операции додека повторно не се запише вториот клуч во драјверот. 

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "custom_driver"
#define BUFFER_SIZE 1024
#define KEY_TEMP_UNLOCK "juni123"
#define KEY_PERM_UNLOCK "juni!@#"

char mem[BUFFER_SIZE];
int unlocked; // 1 - tmp unlock, 2 -  perm unlcok, 0 - locked
int r , w;
char last_command[16];

ssize_t my_read(struct file* filep, char* buffer, size_t count, loff_t* off);
ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off);

struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
};

static int r_init(void) {
    unlocked = 0;
    r = 0;
    w = 0;

    if (register_chrdev(60, "primer", &my_fops)) {
        printk(KERN_ERR "Neuspesna registracija\n");
        return -1;
    }

}

static void r_cleanup(void) {
    unregister_chrdev(60, "primer");
    printk(KERN_INFO "Drajverot e izbrisan\n");
}

ssize_t my_read(struct file* filep, char* buffer, size_t count, loff_t* off) {
    if (!unlocked) {
        return 0;  
    }

    if (count > (BUFFER_SIZE - r)) {
        count = BUFFER_SIZE - r;
    }

    if (copy_to_user(buffer, mem + read_pos, count)) {
        return -EFAULT;
    }

    r += count;
    
    if (strcmp(last_command, KEY_TEMP_UNLOCK) == 0) {
        unlocked = 0;  
    }

    return count;
}

ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off) {
    char pom[16];

    if (count < sizeof(pom)) {
        if (copy_from_user(pom, buffer, count)) {
            return -EFAULT;
        }

        if (strcmp(pom, KEY_TEMP_UNLOCK) == 0) {
            unlocked = 1;
        } else if (strcmp(pom, KEY_PERM_UNLOCK) == 0) {
            unlocked = 2;
        } else {
            if (!unlocked) {
                return 0;
            }
        }

        strcpy(last_command, pom);
        return count;
    }

    if (!unlocked) {
        return 0;
    }

    if (count > (BUFFER_SIZE - w)) {
        count = BUFFER_SIZE - w;
    }

    if (copy_from_user(mem + write_pos, buffer, count)) {
        return -EFAULT;
    }

    w += count;

    if (strcmp(last_command, KEY_TEMP_UNLOCK) == 0) {
        unlocked = 0;
    }

    return count;
}

