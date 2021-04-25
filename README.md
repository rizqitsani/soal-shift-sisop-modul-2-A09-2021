# soal-shift-sisop-modul-2-A09-2021
## Nomor 1

Solusi pada soal ini menggunakan daemon karena pada soal disebutkan bahwa semua poin dijalankan di background. Berikut code yang menghasilkan daemon process.

```c
pid_t pid, sid;

pid = fork();

if (pid < 0) {
exit(EXIT_FAILURE);
}

if (pid > 0) {
exit(EXIT_SUCCESS);
}

umask(0);

sid = setsid();
if (sid < 0) {
exit(EXIT_FAILURE);
}

// Mengubah working directory
if ((chdir("/home/rizqitsani/Sisop/soal-shift-sisop-modul-2-A09-2021/soal1")) < 0) {
exit(EXIT_FAILURE);
}

close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);


while (1) {
    // Semua proses berjalan disini
}
```

Proses yang berjalan dibagi menjadi dua, yaitu:
1. Enam jam sebelum waktu ulang tahun Stevany (9 April 2021, 16:22):
    - Membuat folder Pyoto, Musyik, dan Fylm
    - Menjalankan fungsi `downloadThenUnzip` yang akan mengunduh file .zip dari drive lalu di-unzip
    - Menjalankan fungsi `moveFile` untuk tiap folder hasil unzip. Fungsi ini akan memindah isi folder hasil unzip ke folder yang telah dibuat pada langkah pertama (Pyoto, Musyik, dan Fylm) dan menghapus folder hasil unzip
2. Saat waktu ulang tahun Stevany (9 April 2021, 22:22):
   - Menjalankan perintah untuk compress folder menjadi satu file dengan nama `Lopyu_Stevany.zip`

Variabel global untuk membantu:
```c
// Nama folder untuk menyimpan file dari berkas .zip
char *desiredName[] = {"Fylm", "Musyik", "Pyoto"};

// Nama folder hasil unduhan dari drive
char *fileName[] = {"Film_for_Stevany.zip", "Musik_for_Stevany.zip", "Foto_for_Stevany.zip"};

// Nama folder setelah unzip
char *folderName[] = {"FILM", "MUSIK", "FOTO"};
```

Inisiasi variabel-variabel yang dibutuhkan sebagai acuan waktu:

```C
time_t now = time(NULL);
struct tm * currTime = localtime(&now);
```

Percabangan proses:
```c
if (
    currTime->tm_mday == 9  &&
    currTime->tm_mon  == 3  &&
    currTime->tm_hour == 16 &&
    currTime->tm_min  == 22 &&
    currTime->tm_sec  == 0
) {
    // Proses 1 (16.22)
} else if (
    currTime->tm_mday == 9  &&
    currTime->tm_mon  == 3  &&
    currTime->tm_hour == 22 &&
    currTime->tm_min  == 22 &&
    currTime->tm_sec  == 0
) {
    // Proses 2 (22.22)
}
```

### Proses 1 (Pukul 16.22)

Pertama akan dibuat tiga folder menggunakan perintah `mkdir` dibantu dengan variabel global `desiredName`

```c
if((child_id = fork()) == 0) {
    execlp("mkdir", "mkdir", "-p", desiredName[0], desiredName[1], desiredName[2], NULL);
}

while(wait(&status) > 0);
```

Kemudian akan dipanggil fungsi `downloadThenUnzip` yang isinya seperti ini:

```c
char *url[] = {
    "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download",
    "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download",
    "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download"
};

pid_t child_id;
int status;

for(int i = 0; i < 3; i++) {
    if((child_id = fork()) == 0) {
        execlp("wget", "wget", "--no-check-certificate", url[i], "-O", fileName[i], "-q", NULL);
    }

    while(wait(&status) > 0);

    if((child_id = fork()) == 0) {
        execlp("unzip", "unzip", "-qq", fileName[i], NULL);
    }

    while(wait(&status) > 0);
}
```

Terdapat dua proses yang dilakukan pada fungsi ini yaitu proses download (`wget`) dan unzip. Kedua proses berjalan untuk tiap objek (Foto, Musik, dan Film)

Setelah fungsi `downloadThenUnzip` selesai dijalankan, folder hasil unzip dipindah isinya ke folder yang sudah dibuat dan folder asli dihapus dengan fungsi `moveFile`

```c
for(int i = 0; i < 3; i++) {
    moveFile(folderName[i], desiredName[i]);
}
```

Berikut isi fungsi `moveFile`

```c
pid_t child_id;
int status;

char srcPath[20], destPath[20];

strcpy(srcPath, srcDir);
strcat(srcPath, "/.");

strcpy(destPath, destDir);
strcat(destPath, "/");

if((child_id = fork()) == 0) {
    execlp("cp", "cp", "-a", srcPath, destPath, NULL);	
}

while(wait(&status) > 0);

if((child_id = fork()) == 0) {
    execlp("rm", "rm", "-r", srcDir, NULL);
}

while(wait(&status) > 0);
```

Digunakan perintah `cp` untuk memindah isi folder dan perintah `rm` untuk menghapus folder asal

### Proses 2 (Pukul 22.22)

Hanya ada satu proses yang berjalan disini yaitu proses zip folder menggunakan perintah `zip`

```c
if((child_id = fork()) == 0) {
    execlp("zip", "zip", "-rmvq", "Lopyu_Stevany.zip", desiredName[0], desiredName[1], desiredName[2], NULL);
}

while(wait(&status) > 0);
```

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
### 3b.Direktori yang telah dibuat diisi dengan 10 gambar yang didownload dari [https://picsum.photos/](https://picsum.photos/), nama berdasarkan format timestamp [YYYY-mm-dd_HH:ii:ss] dan gambar persegi berukuran (n%1000) + 50 pixel dimana n adalah detik Epoch Unix

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

### 3c. Setelah direktori terisis dengan 10 gambar, membuar file status.txt yang berisi pesan "Download Success" dengan enkripsi caesar chipper shift 5.kemudian direktori tersebut dizip

**Pembahasan : **
mendeklarasikan variabel *status* dengan isi "Download Success" kemudian membuat file bernama `status.txt` 
```bash 
char dir[50], status[50] = "Download Success" ;
strcpy(dir, buff) ;
strcat(dir, "/") ;
strcat(dir, "status.txt") ;
FILE* txt = fopen(dir, "w") ;
```
kemudian dilakukan "Download Process" proses enskripsi dengan teknik Caesar Cipher dan dengan shift 5 lalu di puts dalam file `status.txt`
```bash
for (int i = 0 ; i < strlen(status) ; i++) {
     if ((status[i] >= 'a' && status[i] <= 'z') ||
          (status[i] >= 'A' && status[i] <= 'Z'))
     {
     	status[i] += 5;
        if ((status[i] > 'z') || (status[i] > 'Z' && status[i] < 'a'))
        {
           status[i] -= 26;
        }
     }
}
fputs(status,txt);
fclose(txt) ;
```
kemudian 10 foto yang telah didownload dan file status.txt disatukan menjadi file zip dengan penamaan sesuai nama direktori
```bash
strcpy(dir, buff) ;
strcat(dir, ".zip") ;
char* argz[] = {"zip", dir, "-q", "-m", "-r", buff, NULL} ;
execv("/bin/zip", argz) ;
```
### 3d. Membuat program “Killer” yang nantinya harus merupakan program bash
```bash
void pkiller(char a[]) {
	FILE* src = fopen("killer.sh", "w") ;
	fputs(a, src) ;
	fclose(src) ;
}
```
###3e. Membuat dua program utama yaitu command -x dan -z

1. "-z" command untuk ketika program "killer" dijalankan dapat menghentikan semua operasi proses
```bash
if (!strcmp(argv[1], "-z")) {
    strcpy(b, "#!/bin/bash\nkillall -9 ./soal3\nrm $0\n") ;
    make_program(b) ;
}
```
2. "-x" command untuk melakukan terminasi kepada semua proses setelah semua proses selesai 
```bash
else if (!strcmp(argv[1], "-x")) {
    strcpy(b, "#!/bin/bash\nkillall -15 ./soal3\nrm $0\n") ;
    make_program(b) ;
    signal(SIGTERM, custom_signal_x) ;
}
```
