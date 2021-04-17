#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>

void copy(char *src, char *dest) {
	pid_t child_id = fork();
	if (child_id == 0) {
		char *argv[] = {"cp", src, dest, NULL};
		execv("/usr/bin/cp", argv);
	}
}

void delete (char *file) {
	pid_t child_id = fork();
	if (child_id == 0) {
		char *argv[] = {"rm", "-d", file, NULL};
		execv("/usr/bin/rm", argv);
	}
}

void listFilesRecursively(char *basePath) {
	char path[1000];
	struct dirent *dp;
	DIR *dir = opendir(basePath);

	if (!dir)
		return;

	while ((dp = readdir(dir)) != NULL) {
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
			int status;
			char *token, *token2, *hewan;
			char data[360], filename[100], getNama[50], getJenis[50];
			pid_t child_id = fork();

			//Untuk membuat folder dengan kategori hewannya ./modul2/petshop/cat/
			char getJenisHewan[100];
			strcpy(getJenisHewan, dp->d_name);
			strtok(getJenisHewan, ";");
			char mkFolder[100] = "/home/daffainfo/modul2/petshop/";
			strcat(mkFolder, getJenisHewan);

			//List file ./modul2/petshop/owakwoak.jpg
			char listFile[100] = "/home/daffainfo/modul2/petshop/";
			strcat(listFile, dp->d_name);

			strcpy(filename, dp->d_name);

			// Get every animal in a photo
			char *hewan = strtok_r(filename, "_", &token);
			while (hewan) {
				strcpy(getJenis, strtok_r(hewan, ";", &token2));
				strcpy(getNama, strtok_r(NULL, ";", &token2));
				strcat(getNama, ".jpg");
				sprintf(data, "/home/daffainfo/modul2/petshop/%s/%s", getJenis, getNama);
				hewan = strtok_r(NULL, "_", &save1);
			}

			while ((wait(&status)) > 0);
			if (child_id == 0){
				char *argv[] = {"mkdir", "-p", mkFolder, NULL};
				execv("/bin/mkdir", argv);
			}

			while ((wait(&status)) > 0);
			copy(listFile, data);

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

int main() {
	int status;
	pid_t child_id, child_id2;
	child_id = fork();
	child_id2 = fork();

	if (child_id == 0) {
		((wait(&status)) > 0);
		char *argv1[] = {"mkdir", "-p", "/home/daffainfo/modul2/petshop", NULL};
		execv("/bin/mkdir", argv1);
	} else {
		((wait(&status)) > 0);
	}
	if (child_id2 == 0) {
		char *argv[] = {"unzip", "-j", "pets.zip", "*.jpg", "-d", "/home/daffainfo/modul2/petshop", NULL};
		execv("/bin/unzip", argv);
	} else {
		((wait(&status)) > 0);
	}

	listFilesRecursively("/home/daffainfo/modul2/petshop");

	return 0;
}
