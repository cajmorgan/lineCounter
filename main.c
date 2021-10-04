#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "cajlib.h"

struct filesAndFolders {
  char *path;
  char *ext;
  struct filesAndFolders *next;
  struct filesAndFolders *prev;
};

typedef struct fileCounter {
  int c;
  int js;
  int html;
  int css;
  int total;
} result;

result readFilesAndCount(char *path, struct filesAndFolders *allfiles);
int countNumberOfLines(FILE *file);
void findAllFoldersInCurrentDir(char *path, struct filesAndFolders *folders);
void findAllFilesInCurrentDir(char *path, struct filesAndFolders *allfiles);
void countToFileCounter(result *fileCounterByExt, char *path, struct filesAndFolders *allfiles);
void recursiveFolderSearcher(struct filesAndFolders *folders, char *path, result *fileCounterByExt);

int main() {
  char *path = "./filesToCount/";
  result fileCounterByExt = {
    .c = 0,
    .js = 0,
    .html = 0,
    .css = 0,
    .total = 0
  };

  struct filesAndFolders *folders = NULL;
  folders = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
  folders->path = NULL;
  folders->ext = NULL;
  folders->next = NULL;
  folders->prev = NULL;
  findAllFoldersInCurrentDir(path, folders);
  
  struct filesAndFolders *allfiles = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
  allfiles->path = NULL;
  allfiles->ext = NULL;
  allfiles->next = NULL;
  allfiles->prev = NULL;
  findAllFilesInCurrentDir(path, allfiles);
  countToFileCounter(&fileCounterByExt, path, allfiles);
  
  recursiveFolderSearcher(folders, path, &fileCounterByExt);
  
 //After loop
  fileCounterByExt.total += fileCounterByExt.c;
  fileCounterByExt.total += fileCounterByExt.js;
  fileCounterByExt.total += fileCounterByExt.css;
  fileCounterByExt.total += fileCounterByExt.html;
  printf("Total lines: %d\n", fileCounterByExt.total);
  
  if(fileCounterByExt.c > 0)
    printf("C: %d\n", fileCounterByExt.c);
  if(fileCounterByExt.js > 0) 
    printf("JavaScript: %d\n", fileCounterByExt.js);
  if(fileCounterByExt.html > 0)
    printf("HTML: %d\n", fileCounterByExt.html);
  if(fileCounterByExt.css > 0)
    printf("CSS: %d\n", fileCounterByExt.css);
  
  return 0;
}

void recursiveFolderSearcher(struct filesAndFolders *folders, char *path, result *fileCounterByExt) {
   
  while(folders->path != NULL) {
    char *subpath = NULL;
    subpath = calloc(100, sizeof(char));
    string_copy(subpath, 100, path, strlen(path) + 1);
    subpath = string_concat(subpath, folders->path);
    add_char(subpath, '/', 100);

    struct filesAndFolders *subfolders = NULL;
    subfolders = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
    subfolders->path = NULL;
    subfolders->ext = NULL;
    subfolders->next = NULL;
    subfolders->prev = NULL;
    findAllFoldersInCurrentDir(subpath, subfolders);
   
    struct filesAndFolders *suballfiles = NULL;
    suballfiles = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
    suballfiles->path = NULL;
    suballfiles->ext = NULL;
    suballfiles->next = NULL;
    suballfiles->prev = NULL;
    findAllFilesInCurrentDir(subpath, suballfiles);
    countToFileCounter(fileCounterByExt, subpath, suballfiles);
    
    recursiveFolderSearcher(subfolders, subpath, fileCounterByExt);

    if(folders->next != NULL) {
      folders = folders->next;
    } else {
      break;
    }
  }

}

void countToFileCounter(result *fileCounterByExt, char *path, struct filesAndFolders *allfiles) {
  result tempFileCounterByExt = readFilesAndCount(path, allfiles);
  fileCounterByExt->c += tempFileCounterByExt.c;
  fileCounterByExt->js += tempFileCounterByExt.js;
  fileCounterByExt->html += tempFileCounterByExt.html;
  fileCounterByExt->css += tempFileCounterByExt.css;
}

result readFilesAndCount(char *path, struct filesAndFolders *allfiles) {
  FILE *fp;
  char *pathWithFile;
  struct filesAndFolders *currentFile;
  result tempFileCounterByExt = {
    .c = 0,
    .js = 0,
    .html = 0,
    .css = 0,
    .total = 0
  };

  int lineBreaks;
  pathWithFile = (char *)malloc(sizeof(char) * 100);

  currentFile = allfiles;

  if(currentFile->path == NULL) {
    if(currentFile->next != NULL) {
      currentFile = currentFile->next;
    } else {
      return tempFileCounterByExt;
    }
  }
  
  while(currentFile->path != NULL) {
    string_copy(pathWithFile, 100, path, strlen(path) + 1);
    pathWithFile = string_concat(pathWithFile, currentFile->path);
    fp = fopen(pathWithFile, "r");
    if(!fp) {
      perror("Failed to open file!");
      return tempFileCounterByExt;
    }
    
    lineBreaks = countNumberOfLines(fp);
    char *fileExt = strrchr(currentFile->path, '.');
    if(strcmp(fileExt + 1, "js") == 0) {
      tempFileCounterByExt.js += lineBreaks;
    } else if (strcmp(fileExt + 1, "css") == 0) {
      tempFileCounterByExt.css += lineBreaks;
    } else if (strcmp(fileExt + 1, "html") == 0) {
      tempFileCounterByExt.html += lineBreaks;
    } else if (strcmp(fileExt + 1, "c") == 0) {
      tempFileCounterByExt.c += lineBreaks;
    } else if (strcmp(fileExt + 1, "h") == 0) {
      tempFileCounterByExt.c += lineBreaks;
    }

    fclose(fp);

    if (currentFile->next != NULL) {
      currentFile = currentFile->next;
    } else {
      break;
    }
  }

  return tempFileCounterByExt;
}

int countNumberOfLines(FILE *file) 
{ 
  int c, counter = 0;
  while((c = fgetc(file)) != EOF) {
    if(c == '\n') {
      counter += 1;
    }
  }

  return counter;
}

void findAllFoldersInCurrentDir(char *path, struct filesAndFolders *folders) {
  DIR *currentDirectory = NULL;
  currentDirectory = opendir(path);
  struct filesAndFolders *current;
  struct dirent *dir = NULL;

  current = folders;
  current->prev = NULL;

  while((dir = readdir(currentDirectory)) != NULL) {
    char target = '.';
    char *test_if_execution_str = (char *)malloc(sizeof(char) * 100);
    string_copy(test_if_execution_str, 100, path, strlen(path) + 1);
    char *concatted_string = (char *)malloc(sizeof(char) * 100);
    concatted_string = string_concat(test_if_execution_str, dir->d_name);
    add_char(concatted_string, '/', 100);
    if(strrchr(dir->d_name, target) == NULL && access(concatted_string, X_OK) != -1) {
      current->path = (char *)malloc(sizeof(char) * 100);
      int length_of_dir_name = strlen(dir->d_name);
      string_copy(current->path, 100, dir->d_name, length_of_dir_name + 1);
      current->next = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
      current->next->prev = current;
      current = current->next;
      free(test_if_execution_str);
      free(concatted_string);
      test_if_execution_str = NULL;
      concatted_string = NULL;
    }
  }

  current = current->prev;

  if(current != NULL) {
    free(current->next);
    current->next = NULL;
  }

  closedir(currentDirectory);
}

void findAllFilesInCurrentDir(char *path, struct filesAndFolders *allfiles) {  
  DIR *currentDirectory = NULL;
  currentDirectory = opendir(path);
  struct filesAndFolders *current;
  struct dirent *dir = NULL;

  current = allfiles;
  current->prev = NULL;

  while((dir = readdir(currentDirectory)) != NULL) {
    if(strcmp(dir->d_name, ".file") > 0) {
      if(strrchr(dir->d_name, '.') != NULL) {
        char *fileExt = strrchr(dir->d_name, '.');
        //Refactor
        if(strcmp(fileExt + 1, "js") == 0) {
          current->path = (char *)malloc(sizeof(char) * 100);
          int length_of_dir_name = strlen(dir->d_name);
          string_copy(current->path, 100, dir->d_name, length_of_dir_name + 1);
          current->ext = (fileExt + 1);
          current->next = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
          current->next->prev = current;
          current = current->next; 
        } else if(strcmp(fileExt + 1, "css") == 0) {
          current->path = (char *)malloc(sizeof(char) * 100);
          int length_of_dir_name = strlen(dir->d_name);
          string_copy(current->path, 100, dir->d_name, length_of_dir_name + 1);
          current->ext = (fileExt + 1);
          current->next = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
          current->next->prev = current;
          current = current->next;
        } else if(strcmp(fileExt + 1, "html") == 0) {
          current->path = (char *)malloc(sizeof(char) * 100);
          int length_of_dir_name = strlen(dir->d_name);
          string_copy(current->path, 100, dir->d_name, length_of_dir_name + 1);
          current->ext = (fileExt + 1);
          current->next = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
          current->next->prev = current;
          current = current->next;
        } else if(strcmp(fileExt + 1, "c") == 0) {
          current->path = (char *)malloc(sizeof(char) * 100);
          int length_of_dir_name = strlen(dir->d_name);
          string_copy(current->path, 100, dir->d_name, length_of_dir_name + 1);
          current->ext = (fileExt + 1);
          current->next = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
          current->next->prev = current;
          current = current->next;
        } else if(strcmp(fileExt + 1, "h") == 0) {
          current->path = (char *)malloc(sizeof(char) * 100);
          int length_of_dir_name = strlen(dir->d_name);
          string_copy(current->path, 100, dir->d_name, length_of_dir_name + 1);
          current->ext = (fileExt + 1);
          current->next = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
          current->next->prev = current;
          current = current->next;
        } 
      }
    }
  }
  
  current = current->prev;
  
  if(current != NULL) {
    free(current->next);
    current->next = NULL;
  }
  
  closedir(currentDirectory);
}

