#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "string.h"

int match(const char *name, const char *pattern) {
    int len = strlen(pattern);
    if(pattern[len-1] == '*') {
        return strncmp(name, pattern, len-1) == 0;
    }
    return strcmp(name, pattern) == 0;
}

void find(char *path, char *pattern) {
    char buf[512];
    int fd;
    struct stat st;
    struct dirent de;

    if((fd = open(path, 0)) < 0) return;
    if(fstat(fd, &st) < 0){ close(fd); return; }

    if(st.type == T_FILE){
        if(match(path, pattern))
            printf(1, "%s\n", path);
        close(fd);
        return;
    }

    if(st.type == T_DIR){
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0) continue;
            if(strcmp(de.name,".")==0 || strcmp(de.name,"..")==0) continue;
            snprintf(buf, sizeof(buf), "%s/%s", path, de.name);
            find(buf, pattern);
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if(argc < 3 || strcmp(argv[1], "-name") != 0){
        printf(1, "Usage: find -name <pattern>\n");
        exit();
    }
    find(".", argv[2]);
    exit();
}
