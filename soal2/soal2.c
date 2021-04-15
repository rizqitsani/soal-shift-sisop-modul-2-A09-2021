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
            //Untuk membuat folder berdasarkan jenis hewan
            int status;
            pid_t child_id, child_id2;
            char file[1000] = "/home/daffainfo/modul2/petshop/";
            char temp[1000] = "";
            child_id = fork();
            child_id2 = fork();
            strcpy(temp, dp->d_name);
            strtok(temp, ";");
            strcat(file, temp);
            while ((wait(&status)) > 0);
            if (child_id == 0) {
                char *argv[] = {"mkdir", "-p", file, NULL};
                execv("/bin/mkdir", argv);
            }
            char move[1000] = "/home/daffainfo/modul2/petshop/";
            strcat(move, dp->d_name);
            //Kalau 2 hewan 1 foto
            if (strstr(dp->d_name, "_")) {
                int status;
                pid_t child_id, child_id2;
                char file2[1000] = "/home/daffainfo/modul2/petshop/";
                char file3[1000] = "/home/daffainfo/modul2/petshop/";
                char getJenis[1000] = "";
                char getNama[1000] = "";
                char getHewan[1000] = "";
                char getJenis2[1000] = "";
                char getNama2[1000] = "";
                char temp[1000] = "";
                char temp2[1000] = "";
                char *token, *token2;
                child_id = fork();
                child_id2 = fork();

                strcpy(temp, dp->d_name);
                token = strtok(temp, ";");
                strcpy(getJenis, token);
                token = strtok(NULL, ";");
                strcpy(getNama, token);
                strcat(file2, getJenis);
                strcat(file2,"/");
                strcat(file2, getNama);

                strcpy(temp2, dp->d_name);
                token2 = strtok(temp2,"_");
                token2 = strtok(NULL,";");
                strcpy(getJenis2,token2);
                token2 = strtok(NULL,";");
                strcpy(getNama2,token2);
                strcat(file3,getJenis2);
                strcat(file3,"/");
                strcat(file3, getNama2);

                //copy seluruh file ke folder masing"
                while ((wait(&status)) > 0);
                if (child_id == 0) {
                    char *argv[] = {"cp", move, file2, NULL};
                    execv("/bin/cp", argv);
                } if (child_id2 == 0) {
                    char *argv[] = {"cp", move, file3, NULL};
                    execv("/bin/cp", argv);
                }
                
            } else {
                //Jika 1 hewan 1 foto
                int status;
                pid_t child_id;
                char file4[1000] = "/home/daffainfo/modul2/petshop/";
                char getJenis[1000] = "";
                char getNama[1000] = "";
                char temp[1000] = "";
                char* token;
                child_id = fork();

                //  file4 ini berisi nama folder tujuan dan juga rename
                // /home/daffainfo/modul2/petshop/jenisnya/namahewan.jpg
                strcpy(temp, dp->d_name);
                token = strtok(temp, ";");
                strcpy(getJenis, token);
                token = strtok(NULL, ";");
                strcpy(getNama, token);
                strcat(file4, getJenis);
                strcat(file4,"/");
                strcat(file4, getNama);

                //copy seluruh file ke folder masing"
                while ((wait(&status)) > 0);
                if (child_id == 0) {
                    char *argv[] = {"cp", move, file4, NULL};
                    execv("/bin/cp", argv);
                }
            }
            //Hapus semua foto jika sudah dicopy ke foldernya masing"
            char test[1000] = "/home/daffainfo/modul2/petshop/";
            strcat(test, dp->d_name);
            while ((wait(&status)) > 0);
            if (child_id2 == 0) {
                char *argv[] = {"rm", "-d", test, NULL};
                execv("/bin/rm", argv);
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

    //Membuat folder modul2/petshop
    while ((wait(&status)) > 0);
    if (child_id == 0) {
        char *argv[] = {"mkdir", "-p", "/home/daffainfo/modul2/petshop", NULL};
        execv("/bin/mkdir", argv);
    }
    //Unzip hanya gambar saja ke folder modul2/petshop
    if (child_id2 == 0) {
        while ((wait(&status)) > 0);
        char *argv[] = {"unzip", "-j", "pets.zip", "*.jpg", "-d","/home/daffainfo/modul2/petshop", NULL};
        execv("/bin/unzip", argv);
    }

    listFilesRecursively("/home/daffainfo/modul2/petshop");

    return 0;
}
