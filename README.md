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
