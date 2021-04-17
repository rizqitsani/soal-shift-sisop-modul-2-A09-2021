#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <wait.h>
#include <signal.h>

void sig() {
    int run = 0 ;
}

void pkiller(char a[]) {
    FILE* src = fopen("killer.sh", "w") ;
    fputs(a, src) ;
    fclose(src) ;
}

int main(int argc, char** argv) {
    if (argc == 2) {
        char a[80] ;
        if (!strcmp(argv[1], "-z")) {
            strcpy(a, "#!/bin/bash\nkillall -9 ./soal3\nrm $0\n") ;
            pkiller(a) ;
        }
        else if (!strcmp(argv[1], "-x")) {
            strcpy(a, "#!/bin/bash\nkillall -15 ./soal3\nrm $0\n") ;
            pkiller(a) ;
            signal(SIGTERM, sig) ;
        }
        else {
            return 0 ;
        }
    }
    else {
        return 0 ;
    }

    while(1) {
        
        pid_t child_id =fork();
 
        char path[100] ;
        char buff[100] ;

        time_t timenow = time(0) ;
        struct tm t = *localtime(&timenow) ;
        strftime(buff, sizeof(buff), "%Y-%m-%d_%X", &t) ;
        

        if (child_id < 0) {
            exit(EXIT_FAILURE) ;
        }

        if (child_id == 0) {
            if (fork() == 0) {
                char* arg[] = {"mkdir", "-p", buff, NULL} ;
                execv("/bin/mkdir", arg) ;
            }
        }
        else {
            sleep(1) ;
            for (int i = 0 ; i < 10 ; i++, sleep(5)) {

                if (fork() == 0) {
                    continue ;
                }

                time_t ntnow = time(0) ;
                struct tm new_tstruct = *localtime(&ntnow) ;
                char new_buff[80] ;
                strftime(new_buff, sizeof(new_buff), "%Y-%m-%d_%X", &new_tstruct) ;
                
                strcpy(path, buff) ;
                strcat(path, "/") ;
                strcat(path, new_buff) ;
                
                char link[80] = "https://picsum.photos/" ;
                int picsize = (((long)mktime(&t)) % 1000) + 50 ;
                char num[10] ;
                sprintf(num, "%d", picsize) ;
                strcat(link, num) ;

                char* arg[] = {"wget", "-q", "-O", path, link, NULL} ;
                execv("/bin/wget", arg) ;
            }
            
            char dir[50], status[50] = "Download Success" ;
            strcpy(dir, buff) ;
            strcat(dir, "/") ;
            strcat(dir, "status.txt") ;
            FILE* txt = fopen(dir, "w") ;
            
            if(txt == NULL){
                exit(EXIT_FAILURE);
            }
            
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
       
            strcpy(dir, buff) ;
            strcat(dir, ".zip") ;
            char* argz[] = {"zip", dir, "-q", "-m", "-r", buff, NULL} ;
            execv("/bin/zip", argz) ;
        }
        time_t first = time(0), second ;
        while (difftime(second, first) != 40) {
            second = time(0) ;
        }
    }
}
