# anita.ganji
//silamm
OS Project
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <limits.h>

#define ANSI_COLOR_BLUE "\x1b[94m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define MAX_PATH_LENGTH 256

typedef struct {
    char path[MAX_PATH_LENGTH];
    int file_count;
    int image_count;
    int txt_count;
    int doc_count;
    int c_count;
    char smallest_file[MAX_PATH_LENGTH];
    char largest_file[MAX_PATH_LENGTH];
    long long smallest_file_size;
    long long largest_file_size;
    pthread_mutex_t mutex;
} DirectoryData;

void* process_directory(void* arg) {
    DirectoryData* data = (DirectoryData*)arg;
    char path[MAX_PATH_LENGTH];
    struct dirent* entry;
    struct stat file_stat;

    DIR* dir = opendir(data->path);
    if (dir == NULL) {
        perror("Error opening directory");
        pthread_exit(NULL);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(path, MAX_PATH_LENGTH, "%s/%s", data->path, entry->d_name);

        if (stat(path, &file_stat) == -1) {
            perror("Error getting file stat");
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            pthread_t thread;
            DirectoryData sub_data;
            strcpy(sub_data.path, path);
            sub_data.file_count = 0;
            sub_data.image_count = 0;
            sub_data.txt_count = 0;
            sub_data.doc_count = 0;
            sub_data.c_count = 0;
            strcpy(sub_data.smallest_file, "");
            strcpy(sub_data.largest_file, "");
            sub_data.smallest_file_size = LLONG_MAX;
            sub_data.largest_file_size = 0;
            pthread_mutex_init(&sub_data.mutex, NULL);

            if (pthread_create(&thread, NULL, process_directory, &sub_data) != 0) {
                perror("Error creating thread");
                pthread_mutex_destroy(&sub_data.mutex);
                continue;
            }

            pthread_join(thread, NULL);

            pthread_mutex_lock(&data->mutex);
            data->file_count += sub_data.file_count;
            data->image_count += sub_data.image_count;
            data->txt_count += sub_data.txt_count;
            data->doc_count += sub_data.doc_count;
            data->c_count += sub_data.c_count;

            if (sub_data.smallest_file_size < data->smallest_file_size) {
                strcpy(data->smallest_file, sub_data.smallest_file);
                data->smallest_file_size = sub_data.smallest_file_size;
            }

            if (sub_data.largest_file_size > data->largest_file_size) {
                strcpy(data->largest_file, sub_data.largest_file);
                data->largest_file_size = sub_data.largest_file_size;
            }

            pthread_mutex_unlock(&data->mutex);

            pthread_mutex_destroy(&sub_data.mutex);
        } else if (S_ISREG(file_stat.st_mode)) {
            pthread_mutex_lock(&data->mutex);
            data->file_count++;

            if (strstr(entry->d_name, ".jpg") != NULL || strstr(entry->d_name, ".png") != NULL)
                data->image_count++;
            else if (strstr(entry->d_name, ".txt") != NULL)
                data->txt_count++;
            else if (strstr(entry->d_name, ".doc") != NULL)
                data->doc_count++;
            else if (strstr(entry->d_name, ".c") != NULL)
                data->c_count++;

            long long file_size = file_stat.st_size;

            if (file_size < data->smallest_file_size) {
                strcpy(data->smallest_file, path);
                data->smallest_file_size = file_size;
            }

            if (file_size > data->largest_file_size) {
                strcpy(data->largest_file, path);
                data->largest_file_size = file_size;
            }

            pthread_mutex_unlock(&data->mutex);
        }
    }

    closedir(dir);
    pthread_exit(NULL);
}

int main() {
    char root_path[MAX_PATH_LENGTH];
    printf("Enter the root directory path: ");
    fgets(root_path, sizeof(root_path), stdin);

    // Remove trailing newline character from root_path
    root_path[strcspn(root_path, "\n")] = '\0';

    DirectoryData root_data;
    strcpy(root_data.path, root_path);
    root_data.file_count = 0;
    root_data.image_count = 0;
    root_data.txt_count = 0;
    root_data.doc_count = 0;
    root_data.c_count = 0;
    strcpy(root_data.smallest_file, "");
    strcpy(root_data.largest_file, "");
    root_data.smallest_file_size = LLONG_MAX;
    root_data.largest_file_size = 0;
    pthread_mutex_init(&root_data.mutex, NULL);

    pthread_t root_thread;
    if (pthread_create(&root_thread, NULL, process_directory, &root_data) != 0) {
        perror("Error creating thread");
        pthread_mutex_destroy(&root_data.mutex);
        return 1;
    }

    pthread_join(root_thread, NULL);

    printf("Total files: %d\n", root_data.file_count);
    printf("Total .images: %d\n", root_data.image_count);
    printf("Total .txt files: %d\n", root_data.txt_count);
    printf("Total .doc files: %d\n", root_data.doc_count);
    printf("Total .c files: %d\n", root_data.c_count);

    if (strcmp(root_data.smallest_file, "") != 0) {
        printf(ANSI_COLOR_BLUE "Smallest file address: %s\n", root_data.smallest_file);
        printf(ANSI_COLOR_RESET "Smallest file size: %lld bytes\n", root_data.smallest_file_size);
    }

    if (strcmp(root_data.largest_file, "") != 0) {
        printf(ANSI_COLOR_BLUE"Largest file address:  %s\n", root_data.largest_file);
        printf(ANSI_COLOR_RESET "Largest file size: %lld bytes\n", root_data.largest_file_size);
    }

    pthread_mutex_destroy(&root_data.mutex);

    return 0;
}
