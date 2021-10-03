#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
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
  findAllFoldersInCurrentDir(path, folders);
  
  struct filesAndFolders *allfiles = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
  findAllFilesInCurrentDir(path, allfiles);
  countToFileCounter(&fileCounterByExt, path, allfiles);
  
  recursiveFolderSearcher(folders, path, &fileCounterByExt);

  //First, find all folders and put that into a linked list. 
  //Then find all files in every folder
  //Scan all files first from all folders? 
  

  
  
 //After loop
  fileCounterByExt.total += fileCounterByExt.c;
  fileCounterByExt.total += fileCounterByExt.js;
  fileCounterByExt.total += fileCounterByExt.css;
  fileCounterByExt.total += fileCounterByExt.html;
  printf("Total lines: %d", fileCounterByExt.total);
  // countToFileCounter(&fileCounterByExt, path, allfiles);

  //File counter -- Put in another func
  
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
    findAllFoldersInCurrentDir(subpath, subfolders);
    struct filesAndFolders *suballfiles = NULL;
    suballfiles = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
    findAllFilesInCurrentDir(subpath, suballfiles);
    countToFileCounter(fileCounterByExt, subpath, suballfiles);
    subpath = NULL;
    subfolders = NULL;
    suballfiles = NULL;
    free(subpath);
    free(subfolders);
    free(suballfiles);
    //Manual
    // char *subpath = "./filesToCount/ohalo/";
    // struct filesAndFolders *subfolders = NULL;
    // subfolders = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
    // findAllFoldersInCurrentDir(subpath, subfolders);
    // struct filesAndFolders *suballfiles = NULL;
    // suballfiles = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
    // findAllFilesInCurrentDir(subpath, suballfiles);
    // countToFileCounter(fileCounterByExt, subpath, suballfiles);
   
   
    // struct filesAndFolders *foldersInSub = NULL;
    // foldersInSub = calloc(1, sizeof(struct filesAndFolders));
    // struct filesAndFolders *filesInSub = NULL;
    // filesInSub = calloc(1, sizeof(struct filesAndFolders));
    // char *subfolderPath = (char *)malloc(sizeof(char) * 100);
   
   
    // findAllFoldersInCurrentDir(subfolderPath, foldersInSub);
    // // findAllFilesInCurrentDir(subfolderPath, filesInSub);
    // // countToFileCounter(fileCounterByExt, subfolderPath, filesInSub);


    // // Görr en recursion, if folders in sub != NULL, recurse. 
    // // Fill the allfiles everytime and clear that. THIS IS SOME BUG, MAKE SURE TO REALLOC/declare ALLFILEs
    // // free(subfolderPath);
    // // subfolderPath = NULL;
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

  // allfiles = NULL;
  // free(allfiles);
}

result readFilesAndCount(char *path, struct filesAndFolders *allfiles) {
  FILE *fp;
  char *pathWithFile;
  struct filesAndFolders *currentFile = NULL;
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


  while(currentFile != NULL) {
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

    currentFile = currentFile->next;
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
  dir = (struct dirent *)malloc(sizeof(struct dirent));

  current = folders;
  current->prev = NULL;

  while((dir = readdir(currentDirectory)) != NULL) {
    char target = '.';
    if(strchr(dir->d_name, target) == NULL) {
      current->path = dir->d_name; 
      current->next = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
      current->next->prev = current;
      current = current->next;
    }
  }

  current = current->prev;

  if(current != NULL) {
    current->next = NULL;
    free(current->next);
  }
  
  dir = NULL;
  free(dir);
  closedir(currentDirectory);

}

void findAllFilesInCurrentDir(char *path, struct filesAndFolders *allfiles) {  
  DIR *currentDirectory = NULL;
  currentDirectory = opendir(path);
  struct filesAndFolders *current;
  struct dirent *dir = NULL;
  dir = (struct dirent *)malloc(sizeof(struct dirent));

  current = allfiles;
  current->prev = NULL;

  while((dir = readdir(currentDirectory)) != NULL) {
    if(strcmp(dir->d_name, ".file") > 0) {
      if(strrchr(dir->d_name, '.') != NULL) {
        char *fileExt = strrchr(dir->d_name, '.');
        //Refactor
        if(strcmp(fileExt + 1, "js") == 0) {
          current->path = dir->d_name;
          current->ext = (fileExt + 1);
          current->next = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
          current->next->prev = current;
          current = current->next; 
        } else if(strcmp(fileExt + 1, "css") == 0) {
          current->path = dir->d_name;
          current->ext = (fileExt + 1);
          current->next = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
          current->next->prev = current;
          current = current->next;
        } else if(strcmp(fileExt + 1, "html") == 0) {
          current->path = dir->d_name;
          current->ext = (fileExt + 1);
          current->next = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
          current->next->prev = current;
          current = current->next;
        } else if(strcmp(fileExt + 1, "c") == 0) {
          current->path = dir->d_name;
          current->ext = (fileExt + 1);
          current->next = (struct filesAndFolders *)malloc(sizeof(struct filesAndFolders));
          current->next->prev = current;
          current = current->next;
        } else if(strcmp(fileExt + 1, "h") == 0) {
          current->path = dir->d_name;
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
    current->next = NULL;
    free(current->next);
  }
  
  dir = NULL;
  free(dir);
  closedir(currentDirectory);
  // return allfiles;
}

