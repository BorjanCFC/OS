// Да се напише драјвер за дешифрирање на текст. Драјверот работи така што во својата меморија
// чува текст со големина од најмногу 512 бајти. Текстот е иницијално не е шифриран додека не се
// запише нешто во драјверот, притоа првото запишување во драјверот е шифрата која ќе се
// користи понатаму. Доколку се прочита од драјверот пред да се запише шифрата, се враќаат 0
// бајти. Потоа, дешифрирањето на текстот од драјверот се прави со запишување на шифрата во
// драјверот. Доколку шифрата што се запишува во драјверот е точна, тогаш на следното читање од
// драјверот, текстот е дешифриран. На секое читање на текстот, најпрво истиот мора да биде
// дешифриран со запишување на шифрата. Шифрирањето на текстот се прави со помош на
// готовата функција void) меморија. Кај првиот процес, третиот аргумент е името shifriraj(char *shifra, char *tekst, char *shifriran). Доколку по
// дешифрирање на текстот корисникот запише нешто во драјверот, тогаш тоа ќе ја пребрише
// содржината на текстот во драјверот.

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/string.h>

#define BUFFER_SIZE 512

static char text[BUFFER_SIZE]; 
static char key[128];          
static int key_set = 0;             
static int text_set = 0;             

void shifriraj(char *shifra, char *tekst, char *shifriran) {
    int i, pom, j = strlen(shifra);
    for (i = 0; i < strlen(tekst); i++) {
        pom = ((int)tekst[i]) + ((int)shifra[i % j]);
        if (pom > 255) pom = 255;
        shifriran[i] = pom;
    }
    shifriran[i] = '\0';
}

void deshifriraj(char *shifra, char *shifriran, char *tekst) {
    int i, pom, j = strlen(shifra);
    for (i = 0; i < strlen(shifriran); i++) {
        pom = ((int)shifriran[i]) - ((int)shifra[i % j]);
        if (pom < 0) pom = 0;
        tekst[i] = pom;
    }
    tekst[i] = '\0';
}

ssize_t my_read(struct file* filep, char* buffer, size_t count, loff_t* off);
ssize_t my_write(struct file* filep, const char* buffer, size_t count, loff_t* off);

struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
};


static int r_init(void) {
    if (register_chrdev(0, "primer", &my_fops)) {
        printk(KERN_ERR "Neuspeshna registracija\n");
        return major;
    }
}


static void r_cleanup(void) {
    unregister_chrdev(0, "primer");
    printk(KERN_INFO "Sayonara");
}

ssize_t my_read(struct file *filep, char *buffer, size_t count, loff_t *off) {
    char decrypted[BUFFER_SIZE];

    if (!key_set)  {
        return 0;
    }  

    deshifriraj(key, text, decrypted);
    if (copy_to_user(buffer, decrypted, strlen(decrypted))) {
        return -EFAULT;
    }

    return strlen(decrypted);
}

ssize_t my_write(struct file *filep, const char *buffer, size_t count, loff_t *off) {
    char user_input[BUFFER_SIZE];

    if (count >= BUFFER_SIZE)  {
        return -EINVAL; 
    }

    if (copy_from_user(user_input, buffer, count)) {
        return -EFAULT;
    }
    user_input[count] = '\0';

    if (!key_set) {
        strncpy(key, user_input, sizeof(key));
        key_set = 1;
        return count;
    }

    if (strcmp(user_input, key) == 0) {
        text_set = 1;  
        return count;
    }

    if (text_set) {
        shifriraj(key, user_input, text);
        return count;
    }

}


