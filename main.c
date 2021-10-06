#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "cajlib.h"

struct files_and_folders {
  char *path;
  char *ext;
  struct files_and_folders *next;
  struct files_and_folders *prev;
};

typedef struct fileCounter {
  int c;
  int js;
  int html;
  int css;
  int total;
} result;

result read_files_and_count(char *path, struct files_and_folders *allfiles);
int count_number_of_lines(FILE *file);
void find_all_folders_in_current_dir(char *path, struct files_and_folders *folders);
void find_all_files_in_current_dir(char *path, struct files_and_folders *allfiles);
void count_to_file_counter(result *file_counter_by_ext, char *path, struct files_and_folders *allfiles);
void recursive_folder_searcher(struct files_and_folders *folders, char *path, result *file_counter_by_ext);

int main() {
  char *path = "./";
  
  result file_counter_by_ext = {
    .c = 0,
    .js = 0,
    .html = 0,
    .css = 0,
    .total = 0
  };

  struct files_and_folders *folders = NULL;
  folders = (struct files_and_folders *)malloc(sizeof(struct files_and_folders));
  folders->path = NULL;
  folders->ext = NULL;
  folders->next = NULL;
  folders->prev = NULL;
  find_all_folders_in_current_dir(path, folders);
  
  struct files_and_folders *allfiles = (struct files_and_folders *)malloc(sizeof(struct files_and_folders));
  allfiles->path = NULL;
  allfiles->ext = NULL;
  allfiles->next = NULL;
  allfiles->prev = NULL;
  find_all_files_in_current_dir(path, allfiles);
  count_to_file_counter(&file_counter_by_ext, path, allfiles);
  
  recursive_folder_searcher(folders, path, &file_counter_by_ext);
  
 //After loop
  file_counter_by_ext.total += file_counter_by_ext.c;
  file_counter_by_ext.total += file_counter_by_ext.js;
  file_counter_by_ext.total += file_counter_by_ext.css;
  file_counter_by_ext.total += file_counter_by_ext.html;
  printf("Total lines: %d\n", file_counter_by_ext.total);
  
  if(file_counter_by_ext.c > 0)
    printf("C: %d\n", file_counter_by_ext.c);
  if(file_counter_by_ext.js > 0) 
    printf("JavaScript: %d\n", file_counter_by_ext.js);
  if(file_counter_by_ext.html > 0)
    printf("HTML: %d\n", file_counter_by_ext.html);
  if(file_counter_by_ext.css > 0)
    printf("CSS: %d\n", file_counter_by_ext.css);
  
  return 0;
}

void recursive_folder_searcher(struct files_and_folders *folders, char *path, result *file_counter_by_ext) {
   
  while(folders->path != NULL) {
    char *subpath = NULL;
    subpath = calloc(100, sizeof(char));
    string_copy(subpath, 100, path, strlen(path) + 1);
    subpath = string_concat(subpath, folders->path);
    add_char(subpath, '/', 100);

    struct files_and_folders *subfolders = NULL;
    subfolders = (struct files_and_folders *)malloc(sizeof(struct files_and_folders));
    subfolders->path = NULL;
    subfolders->ext = NULL;
    subfolders->next = NULL;
    subfolders->prev = NULL;
    find_all_folders_in_current_dir(subpath, subfolders);
   
    struct files_and_folders *suballfiles = NULL;
    suballfiles = (struct files_and_folders *)malloc(sizeof(struct files_and_folders));
    suballfiles->path = NULL;
    suballfiles->ext = NULL;
    suballfiles->next = NULL;
    suballfiles->prev = NULL;
    find_all_files_in_current_dir(subpath, suballfiles);
    count_to_file_counter(file_counter_by_ext, subpath, suballfiles);
    
    recursive_folder_searcher(subfolders, subpath, file_counter_by_ext);

    if(folders->next != NULL) {
      folders = folders->next;
    } else {
      break;
    }
  }

}

void count_to_file_counter(result *file_counter_by_ext, char *path, struct files_and_folders *allfiles) {
  result tempfile_counter_by_ext = read_files_and_count(path, allfiles);
  file_counter_by_ext->c += tempfile_counter_by_ext.c;
  file_counter_by_ext->js += tempfile_counter_by_ext.js;
  file_counter_by_ext->html += tempfile_counter_by_ext.html;
  file_counter_by_ext->css += tempfile_counter_by_ext.css;
}

result read_files_and_count(char *path, struct files_and_folders *allfiles) {
  FILE *fp;
  char *pathWithFile;
  struct files_and_folders *current_file;
  result tempfile_counter_by_ext = {
    .c = 0,
    .js = 0,
    .html = 0,
    .css = 0,
    .total = 0
  };

  int lineBreaks;
  pathWithFile = (char *)malloc(sizeof(char) * 100);

  current_file = allfiles;

  if(current_file->path == NULL) {
    if(current_file->next != NULL) {
      current_file = current_file->next;
    } else {
      return tempfile_counter_by_ext;
    }
  }
  
  while(current_file->path != NULL) {
    string_copy(pathWithFile, 100, path, strlen(path) + 1);
    pathWithFile = string_concat(pathWithFile, current_file->path);
    fp = fopen(pathWithFile, "r");
    if(!fp) {
      perror("Failed to open file!");
      return tempfile_counter_by_ext;
    }
    
    lineBreaks = count_number_of_lines(fp);
    char *fileExt = strrchr(current_file->path, '.');
    if(strcmp(fileExt + 1, "js") == 0) {
      tempfile_counter_by_ext.js += lineBreaks;
    } else if (strcmp(fileExt + 1, "css") == 0) {
      tempfile_counter_by_ext.css += lineBreaks;
    } else if (strcmp(fileExt + 1, "html") == 0) {
      tempfile_counter_by_ext.html += lineBreaks;
    } else if (strcmp(fileExt + 1, "c") == 0) {
      tempfile_counter_by_ext.c += lineBreaks;
    } else if (strcmp(fileExt + 1, "h") == 0) {
      tempfile_counter_by_ext.c += lineBreaks;
    }

    fclose(fp);

    if (current_file->next != NULL) {
      current_file = current_file->next;
    } else {
      break;
    }
  }

  return tempfile_counter_by_ext;
}

int count_number_of_lines(FILE *file) 
{ 
  int c, counter = 0;
  while((c = fgetc(file)) != EOF) {
    if(c == '\n') {
      counter += 1;
    }
  }

  return counter;
}

void find_all_folders_in_current_dir(char *path, struct files_and_folders *folders) {
  DIR *current_directory = NULL;
  current_directory = opendir(path);
  struct files_and_folders *current;
  struct dirent *dir = NULL;

  current = folders;
  current->prev = NULL;

  while((dir = readdir(current_directory)) != NULL) {
    char target = '.';
    char *test_if_execution_str = (char *)malloc(sizeof(char) * 100);
    string_copy(test_if_execution_str, 100, path, strlen(path) + 1);
    char *concatted_string = (char *)malloc(sizeof(char) * 100);
    concatted_string = string_concat(test_if_execution_str, dir->d_name);
    add_char(concatted_string, '/', 100);
    if(strrchr(dir->d_name, target) == NULL && access(concatted_string, X_OK) != -1) {
      if(str_is_identical(dir->d_name, "node_modules") != 0) {
         current->path = (char *)malloc(sizeof(char) * 100);
        int length_of_dir_name = strlen(dir->d_name);
        string_copy(current->path, 100, dir->d_name, length_of_dir_name + 1);
        current->next = (struct files_and_folders *)malloc(sizeof(struct files_and_folders));
        current->next->prev = current;
        current = current->next;
        // free(test_if_execution_str);
        // free(concatted_string);
        test_if_execution_str = NULL;
        concatted_string = NULL;
      }
    }
  }

  current = current->prev;

  if(current != NULL) {
    // free(current->next);
    // current->next = NULL;
  }

  closedir(current_directory);
}

void find_all_files_in_current_dir(char *path, struct files_and_folders *allfiles) {  
  DIR *current_directory = NULL;
  current_directory = opendir(path);
  struct files_and_folders *current;
  struct dirent *dir = NULL;

  current = allfiles;
  current->prev = NULL;

  while((dir = readdir(current_directory)) != NULL) {
    if(strcmp(dir->d_name, ".file") > 0) {
      if(strrchr(dir->d_name, '.') != NULL) {
        char *fileExt = strrchr(dir->d_name, '.');
        //Refactor
        if(strcmp(fileExt + 1, "js") == 0) {
          current->path = (char *)malloc(sizeof(char) * 100);
          int length_of_dir_name = strlen(dir->d_name);
          string_copy(current->path, 100, dir->d_name, length_of_dir_name + 1);
          current->ext = (fileExt + 1);
          current->next = (struct files_and_folders *)malloc(sizeof(struct files_and_folders));
          current->next->prev = current;
          current = current->next; 
        } else if(strcmp(fileExt + 1, "css") == 0) {
          current->path = (char *)malloc(sizeof(char) * 100);
          int length_of_dir_name = strlen(dir->d_name);
          string_copy(current->path, 100, dir->d_name, length_of_dir_name + 1);
          current->ext = (fileExt + 1);
          current->next = (struct files_and_folders *)malloc(sizeof(struct files_and_folders));
          current->next->prev = current;
          current = current->next;
        } else if(strcmp(fileExt + 1, "html") == 0) {
          current->path = (char *)malloc(sizeof(char) * 100);
          int length_of_dir_name = strlen(dir->d_name);
          string_copy(current->path, 100, dir->d_name, length_of_dir_name + 1);
          current->ext = (fileExt + 1);
          current->next = (struct files_and_folders *)malloc(sizeof(struct files_and_folders));
          current->next->prev = current;
          current = current->next;
        } else if(strcmp(fileExt + 1, "c") == 0) {
          current->path = (char *)malloc(sizeof(char) * 100);
          int length_of_dir_name = strlen(dir->d_name);
          string_copy(current->path, 100, dir->d_name, length_of_dir_name + 1);
          current->ext = (fileExt + 1);
          current->next = (struct files_and_folders *)malloc(sizeof(struct files_and_folders));
          current->next->prev = current;
          current = current->next;
        } else if(strcmp(fileExt + 1, "h") == 0) {
          current->path = (char *)malloc(sizeof(char) * 100);
          int length_of_dir_name = strlen(dir->d_name);
          string_copy(current->path, 100, dir->d_name, length_of_dir_name + 1);
          current->ext = (fileExt + 1);
          current->next = (struct files_and_folders *)malloc(sizeof(struct files_and_folders));
          current->next->prev = current;
          current = current->next;
        } 
      }
    }
  }
  
  current = current->prev;
  
  if(current != NULL) {
    // free(current->next);
    // current->next = NULL;
  }
  
  closedir(current_directory);
}

