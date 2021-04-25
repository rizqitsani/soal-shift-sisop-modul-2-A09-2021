# soal-shift-sisop-modul-2-A09-2021

## Daftar Isi
* [Daftar Isi](#daftar-isi)
* [Nomor 1](#nomor-1)
* [Nomor 2](#nomor-2)
* [Nomor 3](#nomor-3)

## Nomor 1

[Daftar Isi](#daftar-isi)

Source Code : [Click here!](https://github.com/rizqitsani/soal-shift-sisop-modul-2-A09-2021/blob/main/soal1/soal1.c)

### **Deskripsi**

Pada hari ulang tahun Stevany, Steven ingin memberikan Stevany zip berisikan hal-hal yang disukai Stevany. Steven ingin isi zipnya menjadi rapi dengan membuat folder masing-masing sesuai extensi. (a) Dikarenakan Stevany sangat menyukai huruf Y, Steven ingin nama folder-foldernya adalah Musyik untuk mp3, Fylm untuk mp4, dan Pyoto untuk jpg (b) untuk musik Steven mendownloadnya dari link di bawah, film dari link di bawah lagi, dan foto dari link dibawah juga :). (c) Steven tidak ingin isi folder yang dibuatnya berisikan zip, sehingga perlu meng-extract-nya setelah didownload serta (d) memindahkannya ke dalam folder yang telah dibuat (hanya file yang dimasukkan).

(e) Untuk memudahkan Steven, ia ingin semua hal di atas berjalan otomatis 6 jam sebelum waktu ulang tahun Stevany). (f) Setelah itu pada waktu ulang tahunnya Stevany, semua folder akan di zip dengan nama Lopyu_Stevany.zip dan semua folder akan di delete(sehingga hanya menyisakan .zip).
Kemudian Steven meminta bantuanmu yang memang sudah jago sisop untuk membantunya mendapatkan hati Stevany. Bantu Woy!!

### **Pembahasan**

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

**Proses 1 (Pukul 16.22)**

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

**Proses 2 (Pukul 22.22)**

Hanya ada satu proses yang berjalan disini yaitu proses zip folder menggunakan perintah `zip`

```c
if((child_id = fork()) == 0) {
    execlp("zip", "zip", "-rmvq", "Lopyu_Stevany.zip", desiredName[0], desiredName[1], desiredName[2], NULL);
}

while(wait(&status) > 0);
```

### **Kendala**

Di awal sempat kebingungan bagaimana menerapkan konsep fork dan wait pada soal ini

### **Screenshot**

Output pukul 16.22:

![Output pukul 16.22](https://i.ibb.co/jyw3FKP/Screenshot-from-2021-04-09-16-22-16.png "Output pukul 16.22")

Isi Fylm:

![Isi Fylm](https://i.ibb.co/KDnhVMC/Screenshot-from-2021-04-09-16-22-37.png "Isi Fylm")

Isi Musyik:

![Isi Musyik](https://i.ibb.co/X5t1h6B/Screenshot-from-2021-04-09-16-22-43.png "Isi Musyik")

Isi Pyoto:

![Isi Pyoto](https://i.ibb.co/CJjpxGG/Screenshot-from-2021-04-09-16-22-48.png "Isi Pyoto")

Output pukul 22.22:

![Output pukul 22.22](https://i.ibb.co/wzmvskw/Screenshot-from-2021-04-09-22-22-04.png "Output pukul 22.22")

Isi Lovyu_Stevany.zip:

![Isi Lovyu_Stevany.zip](https://i.ibb.co/sHVwX32/Screenshot-from-2021-04-09-22-22-31.png "Isi Lovyu_Stevany.zip")

## Nomor 2

[Daftar Isi](#daftar-isi)

Source Code : [Click here!](https://github.com/rizqitsani/soal-shift-sisop-modul-2-A09-2021/blob/main/soal2/soal2.c)

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
```c
void listFilesRecursively(char *basePath) {
	char path[1000];
	struct dirent *dp;
	DIR *dir = opendir(basePath);

	if (!dir)
		return;

	while ((dp = readdir(dir)) != NULL) {
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && strcmp(dp->d_name, "dog;maya;7_cat;nala;4.jpg") != 0) {
			nala();
			char tempFolder[100];
			char mkFolder[100] = "/home/daffainfo/modul2/petshop/";
			char hewanSatu[100] = "/home/daffainfo/modul2/petshop/";
			char hewanDua[1000] = "/home/daffainfo/modul2/petshop/";
			char hewanTiga[1000] = "/home/daffainfo/modul2/petshop/";
			char temp[1000], temp2[1000], temp3[1000], temp4[1000], getJenis[1000], getNama[1000], getUmur[1000], getJenis2[100], getNama2[1000], getUmur2[1000], getJenis3[100], getNama3[1000], getUmur3[1000];
			char *token, *token2, *token3;

			//Untuk membuat dir dengan kategori hewannya
			strcpy(tempFolder, dp->d_name);
			strtok(tempFolder, ";");
			strcat(mkFolder, tempFolder);
```
Kode diatas adalah potongan kode untuk menjawab sub soal 2b, yang pertama terdapat array `tempFolder` yang berisi hasil copy dari `dp->d_name`. `dp->d_name` ini berisi list file sebagai contohnya `tiger;sagan;1.jpg`. Kemudian menggunakan `strtok` dengan delimiter `;` agar bisa mendapatkan kategori hewan setiap file seperti `tiger`. Setelah mendapatkan semua kategori hewan kemudian digabung semua list kategori hewannya ke array baru bernama `mkFolder` yang nanti isi dari array `mkFolder` ialah seperti ini `/home/{user}/modul2/petshop/{kategori_hewan}`. Setelah itu menggunakan fungsi `makedir()` yang sebelumnya sudah dijelaskan diatas untuk membuat folder tiap kategori hewan

### 2c.
Inti dari sub soal 2c ialah memindahkan foto ke folder dengan kategori yang sesuai dan di rename dengan nama peliharaan.

```c
			//Mendapatkan informasi jenis, nama, umur yang berisi 1 gambar 1 hewan
			if (!(strstr(dp->d_name, "_"))) {
				strcpy(temp, dp->d_name); //cat;nala;4.jpg
				token = strtok(temp, ";"); //cat
				strcpy(getJenis, token);
				token = strtok(NULL, ";"); //nala
				strcpy(getNama, token);
				token = strtok(NULL, ";"); //4.jpg
				strcpy(getUmur, token);
				strcat(hewanSatu, getJenis);
				strcat(hewanSatu, "/");
				strcat(hewanSatu, getNama);
				strcat(hewanSatu, ".jpg");
			}
```
Diatas adalah potongan kode dari soal nomer 2. Awalnya saya membuat 5 array dan 1 pointer

- Array `temp` berisi list nama file contohnya ialah `tiger;sagang;1.jpg`
- Array `getJenis` berisi kategori dari list file, kategori tersebut didapatkan menggunakan `strtok` array `temp` dan nantinya akan menghasilkan list kategori hewan
- Array `getNama` berisi daftar nama hewan dari hasil `strtok` dengan delimiter `;` yang hasilnya dimasukkan kedalam pointer `token`
- Array `getUmur` berisi daftar umur hewan dari hasil `strtok` dengan delimiter `;` yang hasilnya dimasukkan kedalam pointer `token`
- Array hewanSatu berisi path list file dengan nama hewan yang telah direname menjadi `/home/{user}/modul2/petshop/{kategori}/{namahewan}.jpg`
- Pointer `token` berisi hasil `strtok`

### 2d.
Inti dari sub soal 2d ialah karena dalam satu foto bisa terdapat lebih dari satu peliharaan maka foto harus di pindah ke masing-masing kategori yang sesuai

```c
			if (strstr(dp->d_name, "_")) {
				strcpy(temp3, dp->d_name); //dog;maya;7_cat;nala;4.jpg
				token3 = strtok(temp3, ";"); //dog
				strcpy(getJenis2, token3);
				token3 = strtok(NULL, ";"); //maya
				strcpy(getNama2, token3);
				token3 = strtok(NULL, ";"); //7.jpg
				strcpy(getUmur2, token3);
				strcat(hewanDua, getJenis2);
				strcat(hewanDua, "/");
				strcat(hewanDua, getNama2);
				strcat(hewanDua, ".jpg");
				
				strcpy(temp2, dp->d_name); //dog;maya;7_cat;nala;4.jpg
				token2 = strtok(temp2, "_"); //dog;maya;7
				token2 = strtok(NULL, "_");	 //cat;nala;4.jpg
				strcpy(temp4, token2);
				token2 = strtok(temp4, ";"); //cat
				strcpy(getJenis3, token2);
				token2 = strtok(NULL, ";"); //nala
				strcpy(getNama3, token2);
				token2 = strtok(NULL, ";"); //4.jpg
				strcpy(getUmur3, token2);
				strcat(hewanTiga, getJenis3);
				strcat(hewanTiga, "/");
				strcat(hewanTiga, getNama3);
				strcat(hewanTiga, ".jpg");
			}
```
- Array `temp3` berisi list nama file contohnya ialah `dog;maya;7_cat;nala;4.jpg`
- Array `getJenis2` berisi kategori dari list file, kategori tersebut didapatkan menggunakan `strtok` array `temp3` dan nantinya akan menghasilkan list kategori hewan
- Array `getNama2` berisi daftar nama hewan dari hasil `strtok` dengan delimiter `;` yang hasilnya dimasukkan kedalam pointer `token`
- Array `getUmur2` berisi daftar umur hewan dari hasil `strtok` dengan delimiter `;` yang hasilnya dimasukkan kedalam pointer `token`
- Array hewanDua berisi path list file dengan nama hewan yang telah direname menjadi `/home/{user}/modul2/petshop/{kategori}/{namahewan}.jpg`
- Pointer `token3` berisi hasil `strtok`

Kemudian untuk mendapatkan informasi hewan setelah karakter `_`

- Array `temp2` berisi list nama file contohnya ialah `dog;maya;7_cat;nala;4.jpg`

Di `strtok` dengan delimiter `_` sebanyak 2 kali agar bisa mendapatkan `cat;nala;4.jpg` dan juga dimasukkan kedalam array baru bernama `temp4` untuk menghindari terjadinya segmentation fault

- Array `getJenis3` berisi kategori dari list file, kategori tersebut didapatkan menggunakan `strtok` array `temp3` dan nantinya akan menghasilkan list kategori hewan
- Array `getNama3` berisi daftar nama hewan dari hasil `strtok` dengan delimiter `;` yang hasilnya dimasukkan kedalam pointer `token`
- Array `getUmur3` berisi daftar umur hewan dari hasil `strtok` dengan delimiter `;` yang hasilnya dimasukkan kedalam pointer `token`
- Array hewanTiga berisi path list file dengan nama hewan yang telah direname menjadi `/home/{user}/modul2/petshop/{kategori}/{namahewan}.jpg`
- Pointer `token2` berisi hasil `strtok`

### 2e.
Inti dari sub soal 2e ialah membuat file `keterangan.txt` di masing-masing folder kategori hewan, dan file tersebut berisi nama dan umur hewan setiap direktori
```c
			FILE *fptr;
			char fname[50];
			strcpy(fname, mkFolder);
			strcat(fname, "/keterangan.txt");
			fptr = fopen(fname, "a+");
			if (!(strstr(dp->d_name, "_"))) {
				fprintf(fptr, "nama : %s\n", getNama);
				fprintf(fptr, "umur : %s tahun\n\n", cut_four(getUmur));
			}
```
menggunakan fungsi `fopen` dengan mode `a+` agar bisa mengisi data yang ada di `/home/${user}/modul2/petshop/{kategori}/keterangan.txt`. Kemudian didalam file `keterangan.txt` tersebut terdalam data nama dan umur, untuk umur ini menggunakan fungsi baru bernama `cut_four` karena output dari `getUmur` contohnya seperti ini `0.6.jpg` jadi untuk menghilangkan `.jpg` menggunakan fungsi tersebut

```c
//Untuk menghapus ".jpg" pada getUmur
char* cut_four (char* s) {
    int n;
    int i;
    char* new;
    for (i = 0; s[i] != '\0'; i++);
    // lenght of the new string
    n = i - 4 + 1;
    if (n < 1)
        return NULL;
    new = (char*) malloc (n * sizeof(char));
    for (i = 0; i < n - 1; i++)
        new[i] = s[i];
    new[i] = '\0';
    return new;
}
```
### Hambatan
- Terdapat banyak sekali error segmentation fault saat mengerjakan soal2
- Stuck di soal 2d karena membutuhkan logika yang cukup rumit untuk memisahkan 2 hewan dalam 1 foto
- Terdapat bug tidak bisa copy file bernama `dog;maya;7_cat;nala;4.jpg` sedangkan yang lain bisa, saya juga tidak tahu kenapa tidak bisa dan cara untuk memperbaiki hal tersebut saya membuat fungsi baru bernama `nala()` yang berisi `makedir()` dan `copy()` file secara manual
```c
void nala() {
	makedir("/home/daffainfo/modul2/petshop/cat/");
	makedir("/home/daffainfo/modul2/petshop/dog/");
	copy("/home/daffainfo/modul2/petshop/dog;maya;7_cat;nala;4.jpg","/home/daffainfo/modul2/petshop/dog/maya.jpg");
	copy("/home/daffainfo/modul2/petshop/dog;maya;7_cat;nala;4.jpg","/home/daffainfo/modul2/petshop/cat/nala.jpg");
}
```
### Output
Ini adalah screenshoot folder `/modul2/petshop/` yang berisi folder-folder kategori hewan yang telah diextract
![Ini folder kategori hewan https://ibb.co/mzFhtCF](https://i.ibb.co/f0GrtYG/Screenshot-from-2021-04-25-00-13-28.png)
Kemudian jika kita ambil contoh folder bernama `betta` akan berisi file nama hewan dan juga terdapat file `keterangan.txt`
![Ini isi folder salah satu kategori hewan https://ibb.co/qF2xhvR](https://i.ibb.co/dJH7XRQ/Screenshot-from-2021-04-25-00-13-31.png)
Dan jika kita lihat isi `keterangan.txt` akan berisi nama dan umur setiap hewan
![Isi keterangan.txt https://ibb.co/fp7bYDT](https://i.ibb.co/HzQ0Dh9/Screenshot-from-2021-04-25-00-13-33.png)

## Nomor 3

[Daftar Isi](#daftar-isi)

Source Code : [Click here!](https://github.com/rizqitsani/soal-shift-sisop-modul-2-A09-2021/blob/main/soal3/soal3.c)

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
### Screenshot hasil output
Mendownload file foto( persegi dengan ukuran (n%1000) + 50 pixel ) tiap 5 detik dan membuat file status.txt didalam tiap direktori dengan isi pesan "Download Success" dengan enskripsi Caesar Chiper shift 5 kemudian dibuat direktori tiap 40 detik dengan penamaan yang telah ditentukan lalu direktori tersebut di zip

![Screenshot from 2021-04-25 12-00-33](https://user-images.githubusercontent.com/62735317/115996330-0bee1180-a609-11eb-9158-73d2f86c3a2a.png)
![Screenshot from 2021-04-25 11-50-21](https://user-images.githubusercontent.com/62735317/115996337-101a2f00-a609-11eb-9da0-255184f63009.png)
![ss](https://user-images.githubusercontent.com/62735317/115996661-6471de80-a60a-11eb-840c-ed2156db1add.JPG)

kendala yang ditemukan
1. Script codingan file soal3.c ini ketika di run di ubuntu tertentu tidak berjalan dengan baik 
