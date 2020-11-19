#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <linux/limits.h>
#include "error.h"

#define BUF_SIZE (4096)
#define MAX_NAME_LEN (256)
#define STAT     ("stat")
#define STATUS   ("status")
#define CMDLINE  ("cmdline")
#define UID_SUBS ("Uid:")
#define NAME_SUBS ("Name:")

#define IS_PID(proc_ent_name) (atoi(proc_ent_name) != 0)

// TODO:stuff left to do
// 1. remove malloc situtation DONE
// 2. fix join_proc_path DONE
// 3. fix error code situtation
// 4. check return value of each function
// 5. remove linked list from solution? DONE
// 6. fix spagehtti code with name and cmdline DONE

#define PROC_PATH "/proc"
#define PROC_FORMAT (PROC_PATH "/%s/%s")

typedef struct {
    int ruid;
    int euid;
    int gid;
    int pid;
    int ppid;
    char name[MAX_NAME_LEN];
    char cmdline[PATH_MAX];

} pid_info_t;

// this is static because its only used in this C file
static error_status_t join_proc_path(char* path, const char* pid, const char* pid_info) {
    error_status_t ret_status = SUCCESS_STATUS;
    CHECK(snprintf(path, PATH_MAX, PROC_FORMAT,  pid, pid_info) > 0)
    
cleanup:
    return  ret_status;
}


error_status_t parse_cmdline(pid_info_t* info, const char* cmdline_path) {
    error_status_t ret_status = SUCCESS_STATUS;
    FILE* fp = NULL;
    int r_bytes = 0;

    CHECK(info != NULL);
    CHECK(cmdline_path != NULL);

    fp = fopen(cmdline_path, "r");
    CHECK_ERR(fp != NULL, GENERIC_FAILED_STATUS);
    
    //TODO: define the 1
    r_bytes = fread(&info->cmdline, 1, BUF_SIZE, fp);
    //TODO: figure out how to check fread's return value

    // cmdline is separated by NULL bytes, change to valid string
    for (int i = 0; i < r_bytes; i++) {
        if (info->cmdline[i] == '\0') {
            info->cmdline[i] = ' ';
        }
    }

cleanup:
    return ret_status;
}

error_status_t parse_stat(pid_info_t* info, const char* stat_path) {
    error_status_t ret_status = SUCCESS_STATUS;
    FILE* fp = NULL;
    
    CHECK(info != NULL);
    CHECK(stat_path != NULL);

    fp = fopen(stat_path, "r");
    CHECK(fp != NULL);

    // TODO: check return value correctly
    // TODO: make name size in fscanf correct
    CHECK(fscanf(fp, "%d (%256[^)]) %*c %d %d", &info->pid, &info->name, &info->ppid, &info->gid) == 4);

    CHECK(fclose(fp) == 0);

cleanup:
    return ret_status;
}

error_status_t parse_status(pid_info_t* info, const char* stat_path) { 
    error_status_t ret_status = SUCCESS_STATUS;
    int r_bytes = 0;
    FILE* fp = NULL;
    char status_data[BUF_SIZE] = {0};
    char* uid_index = NULL;
    char* name_index = NULL;

    CHECK(info != NULL);
    CHECK(stat_path != NULL);

    fp = fopen(stat_path, "r");
    CHECK(fp != NULL); 
    
    //TODO: define the 1
    r_bytes = fread(status_data, 1, BUF_SIZE, fp);
    //TODO: figure out how to check fread's return value

    uid_index = strstr(status_data, UID_SUBS); 
    CHECK(uid_index != NULL);

    // TODO: check return value of sscanf
    sscanf(uid_index, "%*s\t%d\t%d\t%*d\t%*d\n", &info->ruid, &info->euid);

cleanup:
    return ret_status;
}

error_status_t parse_pid(pid_info_t* info, char* pid) {

    error_status_t ret_status = SUCCESS_STATUS;
    char stat_path[PATH_MAX] = {0};
    char status_path[PATH_MAX] = {0};
    char cmdline_path[PATH_MAX] = {0};
    CHECK(info != NULL);
    CHECK(pid != NULL);

    join_proc_path(stat_path, pid, STAT); 
    CHECK(parse_stat(info, stat_path) == SUCCESS_STATUS);
    
    join_proc_path(status_path, pid, STATUS);
    CHECK(parse_status(info, status_path) == SUCCESS_STATUS);

    join_proc_path(cmdline_path, pid, CMDLINE);
    CHECK(parse_cmdline(info, cmdline_path) == SUCCESS_STATUS);

cleanup:
   return ret_status;
}

/*
void print_pid(void* info) {

    pid_info_t* i = (pid_info_t*)info;
    printf("%-10d %-10d %-10d %-10d %s %s\n", i->euid, i->pid, i->ppid, i->gid, i->name, i->cmdline);

}
void print_pids(struct Node** pids) {

    list_print(pids, print_pid);
}
*/
void print_pid(pid_info_t* info) {
    printf("%-10d %-10d %-10d %-10d %s %s\n", info->euid, info->pid, info->ppid, info->gid, info->name, info->cmdline);
}

int main() {
    
    struct dirent* proc_ent = NULL;
    DIR* proc_dir = NULL;

    proc_dir = opendir(PROC_PATH);
    if (proc_dir == NULL) {
        printf("Cannot open /proc\n");
        return -1;
    }
    
    proc_ent = readdir(proc_dir);
    //TODO: check return value
    
    printf("%-10s %-10s %-10s %-10s %-10s %-10s\n", "EUID", "PID", "PPID", "GID", "NAME", "CMD");
    while (proc_ent != NULL) {
        pid_info_t info = {0};
        if (IS_PID(proc_ent->d_name)) {
            parse_pid(&info, proc_ent->d_name);
            //TODO: check return value
            print_pid(&info);
        }
        proc_ent = readdir(proc_dir);
    }

    return 0;
}
