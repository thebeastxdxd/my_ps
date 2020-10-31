#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

#define MIN_BUF_SIZE 256
#define MID_BUF_SIZE 4096
#define STAT     "stat"
#define STATUS   "status"
#define CMDLINE  "cmdline"
#define UID_SUBS "Uid:"
#define READ_FLAG "r"

const char* PROC_PATH = "/proc";

typedef struct _pid_info {
    int ruid;
    int euid;
    int gid;
    int pid;
    int ppid;
    char* cmdline;

} pid_info;

char* join_proc_path(char* pid, char* pid_info) {
    char* full_path = NULL;
    full_path = malloc(strlen(PROC_PATH) + strlen(pid) + strlen(pid_info) + 2);
    if (full_path == NULL) {
        return NULL;
    }

    sprintf(full_path, "%s/%s/%s", PROC_PATH, pid, pid_info); 
    return full_path;
}


int parse_cmdline(char* cmdline_path, pid_info* info) {
    FILE* fp = NULL;
    char* cmdline_data = NULL;
    int r_bytes = 0;

    fp = fopen(cmdline_path, READ_FLAG);
    
    cmdline_data = (char*)malloc(MID_BUF_SIZE);
    if (cmdline_data == NULL) {
        return -1;
    }
    
    r_bytes = fread(cmdline_data, 1, MID_BUF_SIZE, fp);

    for (int i = 0; i < r_bytes; i++) {
        if (cmdline_data[i] == '\0') {
            cmdline_data[i] = ' ';
        }
    }
    if (info->cmdline != NULL) {
        free(info->cmdline);
        info->cmdline = cmdline_data;
    }
    return 0;
}

int parse_stat(char* stat_path, pid_info* info) {
    FILE* fp;

    fp = fopen(stat_path, READ_FLAG);
    if (fp == NULL) {
        return -1;
    }

    fscanf(fp, "%d %ms %*c %d %d", &info->pid, &info->cmdline, &info->ppid, &info->gid);
    fclose(fp);
    return 0;
}

int parse_status(char* stat_path, pid_info* info) { 
    FILE* fp;
    char* status_data = NULL;
    int r_bytes = 0;
    char* uid_index = NULL;

    fp = fopen(stat_path, READ_FLAG);
    if (fp == NULL) {
        return -1;
    }
    
    status_data = (char*)malloc(MID_BUF_SIZE);
    if (status_data == NULL) {
        return -1;
    }
    r_bytes = fread(status_data, 1, MID_BUF_SIZE, fp);

    uid_index = strstr(status_data, UID_SUBS); 
    if (uid_index == NULL) {
        return -1;
    }
    sscanf(uid_index, "%*s\t%d\t%d\t%*d\t%*d\n", &info->ruid, &info->euid);

    return 0;
}

pid_info* parse_pid(char* pid) {
    pid_info* info = NULL; 
    int ret_status = 0;
    char* stat_path = NULL;
    char* status_path = NULL;
    char* cmdline_path = NULL;

    info = (pid_info*)malloc(sizeof(pid_info));
    if (info == NULL) {
        printf("something went wrong\n");
    }

    stat_path = join_proc_path(pid, STAT); 
    ret_status = parse_stat(stat_path, info);
    if (ret_status != 0) {
        printf("something went wrong\n");

    }

    status_path = join_proc_path(pid, STATUS);
    ret_status = parse_status(status_path, info);
    if (ret_status != 0) {
        printf("something went wrong\n");
    }

    cmdline_path = join_proc_path(pid, CMDLINE);
    ret_status = parse_cmdline(cmdline_path, info);
    if (ret_status != 0) {
        printf("something went wrong\n");
    }
    
    printf("pid: %d, cmd: %s, ppid: %d, gid: %d, real_uid: %d, effective_uid: %d\n", info->pid, info->cmdline, info->ppid, info->gid, info->ruid, info->euid);
    free(cmdline_path);
    free(status_path);
    free(stat_path);
    return info;
}


int main() {
    
    struct dirent* proc_ent;
    DIR* proc_dir;

    char* full_path = NULL;
    long file_sz = -1;

    char* error_str = malloc(MID_BUF_SIZE);
    char* buf = NULL;
    int tmp_fd = -1;
   
    proc_dir = opendir(PROC_PATH);
    if (proc_dir == NULL) {
        printf("Cannot open /proc\n");
        return -1;
    }
    
    proc_ent = readdir(proc_dir);
    while (proc_ent != NULL) {
       if (atoi(proc_ent->d_name) != 0) {
            parse_pid(proc_ent->d_name);
        }

        proc_ent = readdir(proc_dir);
                
    }

    free(error_str);
}
