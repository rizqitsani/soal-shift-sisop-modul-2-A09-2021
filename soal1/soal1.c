#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <wait.h>
#include <time.h>
#include <dirent.h>

char *desiredName[] = {"Fylm", "Musyik", "Pyoto"};
char *fileName[] = {"Film_for_Stevany.zip", "Musik_for_Stevany.zip", "Foto_for_Stevany.zip"};
char *folderName[] = {"FILM", "MUSIK", "FOTO"};

void moveFile(char srcDir[], char destDir[]) {
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
}

void downloadThenUnzip() {
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
}

int main() {
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

  if ((chdir("/home/rizqitsani/Sisop/soal-shift-sisop-modul-2-A09-2021/soal1")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);


  while (1) {
    pid_t child_id;
    int status;

    time_t now = time(NULL);
    struct tm * currTime = localtime(&now);

    if (
      currTime->tm_mday == 9  &&
      currTime->tm_mon  == 3  &&
      currTime->tm_hour == 16 &&
      currTime->tm_min  == 22 &&
      currTime->tm_sec  == 0
    ) {
      // Make directories
      if((child_id = fork()) == 0) {
        execlp("mkdir", "mkdir", "-p", desiredName[0], desiredName[1], desiredName[2], NULL);
      }

      while(wait(&status) > 0);

      downloadThenUnzip();

      for(int i = 0; i < 3; i++) {
        moveFile(folderName[i], desiredName[i]);
      }
    } else if (
      currTime->tm_mday == 9  &&
      currTime->tm_mon  == 3  &&
      currTime->tm_hour == 22 &&
      currTime->tm_min  == 22 &&
      currTime->tm_sec  == 0
    ) {
      if((child_id = fork()) == 0) {
        execlp("zip", "zip", "-rmvq", "Lopyu_Stevany.zip", desiredName[0], desiredName[1], desiredName[2], NULL);
      }

      while(wait(&status) > 0);
    }

    while(wait(&status) > 0);

    sleep(1);
  }
}
