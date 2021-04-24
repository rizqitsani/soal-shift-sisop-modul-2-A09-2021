# soal-shift-sisop-modul-2-A09-2021
## Nomor 1
## Nomor 2
### 2a.
Inti dari sub soal 2a ialah mengunzip file jpg yang ada didalam `pets.zip` kemudian membuat folder baru di `/home/${USER}/modul2/petshop`. Disini saya membuat 2 fungsi baru bernama `unzip()` dan `makedir()` untuk menyelesaikan soal 2a.
```c
void makedir(char *dest) {
	int status;
	pid_t child_id = fork();
	if (child_id == 0) {
		char *argv[] = {"mkdir", "-p", dest, NULL};
		execv("/usr/bin/mkdir", argv);
	} else {
		((wait(&status)) > 0);
	}
}
```
Saya membuat fungsi baru bernama `makedir()` agar mempermudah saya  karena makedir akan digunakan selanjut agar tidak perulangan
```c
void unzip() {
	int status;
	pid_t child_id = fork();
	if (child_id == 0) {
		char *argv[] = {"unzip", "-j", "pets.zip", "*.jpg", "-d", "/home/daffainfo/modul2/petshop", NULL};
		execv("/usr/bin/unzip", argv);
	} else {
		((wait(&status)) > 0);
	}
}
```
Agar hanya file dengan ekstensi `jpg` yang terextract saya menggunakan argumen `*.jpg` agar mengextract hanya foto. Kemudian 2 fungsi ini dimasukkan ke dalam fungsi `main()`

### 2b.
Inti dari sub soal 2b ialah membuat folder berdasarkan nama kategori gambar hewan yang telah diextract, saya menggunakan directory listing secara rekursif agar bisa membaca seluruh file yang ada didalam folder `/modul2/petshop`

### 2c.
Inti dari sub soal 2c ialah memindahkan foto ke folder dengan kategori yang sesuai dan di rename dengan nama peliharaan.

### 2d.
Inti dari sub soal 2d ialah karena dalam satu foto bisa terdapat lebih dari satu peliharaan maka foto harus di pindah ke masing-masing kategori yang sesuai

### 2e.
Inti dari sub soal 2e ialah membuat file `keterangan.txt` di masing-masing folder kategori hewan, dan file tersebut berisi nama dan umur hewan setiap direktori

## Nomor 3
Source Code : [click here](https://github.com/rizqitsani/soal-shift-sisop-modul-2-A09-2021/blob/main/soal3/soal3.c)

### 3a.Membuat program .C yang bertujuan membuat sebuah direktori setiap 40 detik dengan format nama timestamp YYYY-mm-dd_HH:ii:ss

**Pembahasan :**

mendeklarasikan sebuah format nama direktori dengan menggunakan timestamp localtime atau waktu sekarang dan disimpan dalam variable bertipe data string `buff`
```bash
 time_t timenow = time(0) ;
 struct tm t = *localtime(&timenow) ;
 strftime(buff, sizeof(buff), "%Y-%m-%d_%X", &t) ;
```
kemudian dilakukan sebuah `fork()` yang dimana dengan memanfaatkan dalam child proses-nya untuk melakukan pembuatan direktori `mkdir` dengan nama variable *buff* yang dibuat sebelumnya lalu dijalankan menggunakan execv untuk membuat direktori

```bash
if (child_id == 0) {
    if (fork() == 0) {
        char* arg[] = {"mkdir", "-p", buff, NULL} ;
        execv("/bin/mkdir", arg) ;
    }
}
```
kemudian dijalankan menggunakan loop menunggu setiap 40 detik
```bash
time_t first = time(0), second ;
        while (difftime(second, first) != 40) {
            second = time(0) ;
        }
```        
### 3b.Direktori yang telah dibuat diisi dengan 10 gambar yang didownload dari [https://picsum.photos/](https://picsum.photos/), nama berdasarkan format timestamp [YYYY-mm-dd_HH:ii:ss] dan gambar persegi berukuran (n%1000) + 50 pixel dimana n adalah detik Epoch Unix.

**Pembahasan :**

mendeklarasikan sebuah format nama timestamp localtime atau waktu sekarang dan disimpan dalam variable bertipe data string `new_buff` sebagai nama timestamp
```bash
 time_t ntnow = time(0) ;
 struct tm new_tstruct = *localtime(&ntnow) ;
 char new_buff[80] ;
 strftime(new_buff, sizeof(new_buff), "%Y-%m-%d_%X", &new_tstruct) ;
```
mendeklarasikan variable string *link* berupa link dari sumber download photo lalu mengset gambar dengan persegi berukuran (n%1000) + 50 pixel dimana n adalah detik Epoch Unix dan dimasukkan ke variable *num* yang telah dibuat dan detik Epoch Unix menggunakan *mktime* dengan *time_t*, foto disimpan pada char *link* yang di strcat dengan *num* size foto.
```bash
char link[80] = "https://picsum.photos/" ;
int picsize = (((long)mktime(&t)) % 1000) + 50 ;
char num[10] ;
sprintf(num, "%d", picsize) ;
strcat(link, num) ;
```
kemudian dijalankan dengan menggunakan execv
```bash
 char* arg[] = {"wget", "-q", "-O", path, link, NULL} ;
 execv("/bin/wget", arg) ;
```
dan proses-proses program pada 3b ini dijalankan dengan diawali *sleep(1)* agar tiap prosesnya menunggu child proses dibawahnya diproses terlebih dahulu lalu dijalankan lagi
