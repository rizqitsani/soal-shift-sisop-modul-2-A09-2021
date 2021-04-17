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
	pid_t child_id;
	child_id = fork();
	if (child_id == 0) {
		char *argv[] = {"cp", "-n", src, dest, NULL};
		execv("/usr/bin/cp", argv);
	}
}

// Fungsi delete file
void delete (char *file) {
	pid_t child_id;
	child_id = fork();
	if (child_id == 0) {
		char *argv[] = {"rm", "-d", file, NULL};
		execv("/usr/bin/rm", argv);
	}
}
//Cek list file di dir secara rekursif
void listFilesRecursively(char *basePath) {
	char path[1000];
	struct dirent *dp;
	DIR *dir = opendir(basePath);

	if (!dir)
		return;

	while ((dp = readdir(dir)) != NULL) {
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
			char tempFolder[100];
			char mkFolder[100] = "/home/daffainfo/modul2/petshop/";
			char hewanSatu[100] = "/home/daffainfo/modul2/petshop/";
			char hewanDua[1000] = "/home/daffainfo/modul2/petshop/";
			char temp[1000], temp2[1000], temp3[1000], getJenis[1000], getNama[1000], getUmur[1000], getJenis2[100], getNama2[1000], getUmur2[1000];
			char *token, *token2;
			int status;
			pid_t child_id = fork();

			//Untuk membuat dir dengan kategori hewannya
			strcpy(tempFolder, dp->d_name);
			strtok(tempFolder, ";");
			strcat(mkFolder, tempFolder);

			//Berisi list file gambar
			char listFile[100] = "/home/daffainfo/modul2/petshop/";
			strcat(listFile, dp->d_name);

			//Mendapatkan informasi jenis, nama, umur yang berisi 1 gambar 1 hewan
			strcpy(temp, dp->d_name);
			token = strtok(temp, ";");
			strcpy(getJenis, token);
			token = strtok(NULL, ";");
			strcpy(getNama, token);
			token = strtok(NULL, ";");
			strcpy(getUmur, token);
			// printf("%s\n",getUmur);
			strcat(hewanSatu, getJenis);
			strcat(hewanSatu, "/");
			strcat(hewanSatu, getNama);
			strcat(hewanSatu, ".jpg");
			
			//Mendapatkan informasi jenis, nama, umur yang berisi 1 gambar 2 hewan
			if (strstr(dp->d_name, "_")) {
				strcpy(temp2, dp->d_name); //dog;wkwk;6_cat;wkwk;5
				char *token2;
				token2 = strtok(temp2, "_"); //dog;wkwk;6
				token2 = strtok(NULL, "_");	 //dog;wkwk;6
				strcpy(temp3, token2);
				token2 = strtok(temp3, ";");
				strcpy(getJenis2, token2);
				token2 = strtok(NULL, ";");
				strcpy(getNama2, token2);
				// printf("%s\n",getNama2);
				token2 = strtok(NULL, ";");
				strcpy(getUmur2, token2);
				strcat(hewanDua, getJenis2);
				strcat(hewanDua, "/");
				strcat(hewanDua, getNama2);
				strcat(hewanDua, ".jpg");
			}
			//make dir
			if (child_id == 0) {
				char *argv[] = {"mkdir", "-p", mkFolder, NULL};
				execv("/bin/mkdir", argv);
			}

			//Copy gambar ke dir masing"
			while ((wait(&status)) > 0);
			copy(listFile, hewanSatu);

			while ((wait(&status)) > 0);
			copy(listFile, hewanDua);

			//Membuat keterangan berisi nama dan umur
			FILE *fptr;
			char fname[50];
			strcpy(fname, mkFolder);
			strcat(fname, "/keterangan.txt");
			fptr = fopen(fname, "a+");
			if (!(strstr(dp->d_name,"_"))) {
				fprintf(fptr, "nama : %s\n", getNama);
				fprintf(fptr, "umur : %s tahun\n\n", cut_four(getUmur));
			} else {
				fprintf(fptr, "nama : %s\n", getNama2);
				fprintf(fptr, "umur : %s tahun\n\n", cut_four(getUmur2));
			}

			fclose(fptr);

			//Menghapus semua file bekas copy
			while ((wait(&status)) > 0);
			delete (listFile);

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

	listFilesRecursively("/home/daffainfo/modul2/petshop");

	return 0;
}
