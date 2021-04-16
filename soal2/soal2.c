#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>

void copy(char *src, char *dest) {
  pid_t child_id;
  child_id = fork();
  if (child_id == 0) {
    char *argv[] = {"cp", src, dest, NULL};
    execv("/usr/bin/cp", argv);
  }
}

void delete(char *file) {
  pid_t child_id;
  child_id = fork();
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
      pid_t child_id = fork();

      char file0[100];
      strcpy(file0, dp->d_name);
      strtok(file0, ";");
      char file[100] = "/home/daffainfo/modul2/petshop/";
      strcat(file, file0);

      char move[100] = "/home/daffainfo/modul2/petshop/";
      strcat(move, dp->d_name);

      char temp[1000];
      strcpy(temp, dp->d_name);
      char *token;
      token = strtok(temp, ";");
      char getJenis[1000];
      strcpy(getJenis, token);
      token = strtok(NULL, ";");
      char getNama[1000];
      strcpy(getNama, token);
      char file2[100] = "/home/daffainfo/modul2/petshop/";
      strcat(file2, getJenis);
      strcat(file2, "/");
      strcat(file2, getNama);

      char temp2[1000] = "";
      strcpy(temp2, dp->d_name);
      char *token2;
      token2 = strtok(temp2, "_");
	  //fix segmentation fault, entah kenapa gabisa? :( )
	  char temp3[1000];
	  strcpy(temp3, token2);
      token2 = strtok(temp3, ";");
      char getJenis2[100];
      strcpy(getJenis2, token2);
      token2 = strtok(NULL, ";");
      char getNama2[1000] = "";
      strcpy(getNama2, token2);
      char file3[1000] = "/home/daffainfo/modul2/petshop/";
      strcat(file3, getJenis2);
      strcat(file3, "/");
      strcat(file3, getNama2);

	  char deletefile[1000] = "/home/daffainfo/modul2/petshop/";
	  strcat(deletefile, dp->d_name);

      if (child_id == 0) {
        char *argv[] = {"mkdir", "-p", file, NULL};
        execv("/bin/mkdir", argv);
      }

      while ((wait(&status)) > 0);
      copy(move, file2);
	  
	  while ((wait(&status)) > 0);
	  copy(move, file3);
	  
	  while ((wait(&status)) > 0);
	  delete(deletefile);

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
  }
  else {
    ((wait(&status)) > 0);
  }
  if (child_id2 == 0) {
    char *argv[] = {"unzip", "-j", "pets.zip", "*.jpg", "-d", "/home/daffainfo/modul2/petshop", NULL};
    execv("/bin/unzip", argv);
  }
  else {
    ((wait(&status)) > 0);
  }
  
  listFilesRecursively("/home/daffainfo/modul2/petshop");

  return 0;
}
