#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

unsigned filename_max = 143;
int v = 0;
#define PATH_MAX 4096

int walkdir(const char *dirname)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];
    unsigned int name_len;

    if (!(dir = opendir(dirname)))
        return 1;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);
        name_len = strlen(entry->d_name);
        if (v) {
            fprintf((name_len > filename_max) ? stdout : stderr,
                    "%3lu \"%s\"\n", strlen(entry->d_name), path);
        } else if (name_len > filename_max) {
            fprintf(stdout,
                    "%3lu \"%s\"\n", strlen(entry->d_name), path);
        }

        if (entry->d_type == DT_DIR) {
            walkdir(path);
        }
    }
    closedir(dir);
    return 0;
}

int main(int argc, char** argv) {
    int opt;

    while ((opt = getopt(argc, argv, "l:v")) != -1) {
        switch (opt) {
            case 'l': // override the size limit for checks
                filename_max = (unsigned) strtoul(optarg, NULL, 0);
                if (!filename_max) {
                    fprintf(stderr, "failed to read filename size limit from %s\n", optarg);
                    return 2;
                }
                fprintf(stderr, "filename size limit set to %u bytes\n", filename_max);
                break;
            case 'v': // verbose
                ++v;
                break;
            case '?':
                fprintf(stderr, "print usage?\n");
            default:
                return 1;
        }
    }

    int idx, ret=0;
    for (idx = optind; (idx < argc) && !ret; ++idx) {
        fprintf(stderr, "walking: \"%s\"\n", argv[idx]);
        ret |= walkdir(argv[idx]);
    }
    return ret;
}