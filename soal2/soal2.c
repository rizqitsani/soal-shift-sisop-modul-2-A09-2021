#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>

void listFilesRecursively(char *basePath) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            char file[100] = "/home/daffainfo/modul2/petshop/";
            int status;
            pid_t child_id;
            child_id = fork();
            strtok(dp->d_name, ";");
            strcat(file, dp->d_name);
            ((wait(&status)) > 0);
            if (child_id == 0) {
                char *argv[] = {"mkdir", "-p", file, NULL};
                execv("/bin/mkdir", argv);
            }

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
        ((wait(&status)) > 0);
        char *argv[] = {"unzip", "-j", "pets.zip", "*.jpg", "-d","/home/daffainfo/modul2/petshop", NULL};
        execv("/bin/unzip", argv);
    }
    else {
        ((wait(&status)) > 0);
    }
    listFilesRecursively("/home/daffainfo/modul2/petshop");

    return 0;
}
