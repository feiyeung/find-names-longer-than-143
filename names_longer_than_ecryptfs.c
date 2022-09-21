#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define ECRYPTFS_FILENAME_MAX 143
#define PATH_MAX 4096

int walkdir(const char *dirname)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];
    unsigned int name_len;
    FILE* outs;

    if (!(dir = opendir(dirname)))
        return 1;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);
        name_len = strlen(entry->d_name);
        outs = (name_len <= ECRYPTFS_FILENAME_MAX) ? stderr : stdout ;
        fprintf(outs, "%3lu \"%s\"\n", strlen(entry->d_name), path);

        if (entry->d_type == DT_DIR) {
            walkdir(path);
        }
    }
    closedir(dir);
    return 0;
}

int main(int argc, char** argv) {
    char* path;
    if (argc < 2) {
        path=getenv("PWD");
        return walkdir(path);
    } else {
        int i, ret=0;
        for (i = 1; i < argc; ++i) {
            ret = walkdir(argv[i]);
            if (!ret) return ret;
        }
    }

    // fprintf(stderr, "launching from path \"%s\"\n\n", path);

    return 0;

}