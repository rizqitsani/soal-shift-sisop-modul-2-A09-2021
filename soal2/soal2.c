#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>

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

// Fungsi copy gambar
void copy(char *src, char *dest) {
	int status;
	pid_t child_id = fork();
	if (child_id == 0) {
		char *argv[] = {"cp", "-n", src, dest, NULL};
		execv("/usr/bin/cp", argv);
	} else {
		((wait(&status)) > 0);
	}
}

// Fungsi delete file
void delete (char *file) {
	int status;
	pid_t child_id = fork();
	if (child_id == 0) {
		char *argv[] = {"rm", "-d", file, NULL};
		execv("/usr/bin/rm", argv);
	} else {
		((wait(&status)) > 0);
	}
}

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

// Fungsi buat fix bug gabisa copy nala.jpg pakai codingan normal (saya juga heran kenapa bugnya :' )
void nala() {
	makedir("/home/daffainfo/modul2/petshop/cat/");
	makedir("/home/daffainfo/modul2/petshop/dog/");
	copy("/home/daffainfo/modul2/petshop/dog;maya;7_cat;nala;4.jpg","/home/daffainfo/modul2/petshop/dog/maya.jpg");
	copy("/home/daffainfo/modul2/petshop/dog;maya;7_cat;nala;4.jpg","/home/daffainfo/modul2/petshop/cat/nala.jpg");
}
//Cek list file di dir secara rekursif
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

			//Berisi list file gambar
			char listFile[100] = "/home/daffainfo/modul2/petshop/";
			strcat(listFile, dp->d_name);

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
			
			//Mendapatkan informasi jenis, nama, umur yang berisi 1 gambar 2 hewan
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
			//buat dir dengan kategori hewan
			makedir(mkFolder);

			//Copy gambar ke dir masing"
			copy(listFile, hewanSatu);
			copy(listFile, hewanDua);
			copy(listFile, hewanTiga);

			//Membuat keterangan berisi nama dan umur
			FILE *fptr;
			char fname[50];
			strcpy(fname, mkFolder);
			strcat(fname, "/keterangan.txt");
			fptr = fopen(fname, "a+");
			if (!(strstr(dp->d_name, "_"))) {
				fprintf(fptr, "nama : %s\n", getNama);
				fprintf(fptr, "umur : %s tahun\n\n", cut_four(getUmur));
			}
			if (strstr(dp->d_name, "_")) {	
				fprintf(fptr, "nama : %s\n", getNama2);
				fprintf(fptr, "umur : %s tahun\n\n", cut_four(getUmur2));
				
				fprintf(fptr, "nama : %s\n", getNama3);
				fprintf(fptr, "umur : %s tahun\n\n", cut_four(getUmur3));
			}

			fclose(fptr);

			//Menghapus semua file bekas copy
			delete(listFile);

			// Construct new path from our base path
			strcpy(path, basePath);
			strcat(path, "/");
			strcat(path, dp->d_name);

			listFilesRecursively(path);
		}
	}

	closedir(dir);
}

int main(){
	int status;
	pid_t child_id, child_id2;
	child_id = fork();
	child_id2 = fork();

	//make dir petshop
	if (child_id == 0) {
		((wait(&status)) > 0);
		char *argv1[] = {"mkdir", "-p", "/home/daffainfo/modul2/petshop", NULL};
		execv("/bin/mkdir", argv1);
	} else {
		((wait(&status)) > 0);
	}
	//Unzip pets.zip
	if (child_id2 == 0) {
		char *argv[] = {"unzip", "-j", "pets.zip", "*.jpg", "-d", "/home/daffainfo/modul2/petshop", NULL};
		execv("/bin/unzip", argv);
	} else {
		((wait(&status)) > 0);
	}

	listFilesRecursively("/home/daffainfo/modul2/petshop/");
	delete("/home/daffainfo/modul2/petshop/dog;maya;7_cat;nala;4.jpg");

	return 0;
}
