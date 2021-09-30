#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

typedef struct filesAndFolders {
  char *path;
  char *ext;
  struct filesAndFolders *next;
} directories, files;

typedef struct fileCounter {
  int c;
  int js;
  int html;
  int css;
  int total;
} result;

result readFilesAndCount(char *path, files *allfiles);
int countNumberOfLines(FILE *file);
directories *findAllFoldersInCurrentDir(char *path);
files *findAllFilesInCurrentDir(char *path);

int main() 
{
  char *path = "./filesToCount/";
  result fileCounterByExt = {
    .c = 0,
    .js = 0,
    .html = 0,
    .css = 0,
    .total = 0
  };

  directories *folders = findAllFoldersInCurrentDir(path);
  files *allfiles = findAllFilesInCurrentDir(path);
  
  //First, find all folders and put that into a linked list. 
  //Then find all files in every folder
  

  //File counter -- Put in another func
  result tempFileCounterByExt = readFilesAndCount(path, allfiles);
  fileCounterByExt.c += tempFileCounterByExt.c;
  fileCounterByExt.js += tempFileCounterByExt.js;
  fileCounterByExt.html += tempFileCounterByExt.html;
  fileCounterByExt.css += tempFileCounterByExt.css;

  //After loop
  fileCounterByExt.total += fileCounterByExt.c;
  fileCounterByExt.total += fileCounterByExt.js;
  fileCounterByExt.total += fileCounterByExt.html;
  fileCounterByExt.total += fileCounterByExt.css;
  printf("Total lines: %d", fileCounterByExt.total);


  return 0;
}

result readFilesAndCount(char *path, files *allfiles) 
{
  FILE *fp;
  char *pathWithFile;
  files *currentFile;
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
  while(currentFile->next != NULL) {
    strcpy(pathWithFile, path);
    strcat(pathWithFile, currentFile->path);
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

directories *findAllFoldersInCurrentDir(char *path)
{
  DIR *currentDirectory;
  currentDirectory = opendir(path);
  directories *folders, *current;
  struct dirent *dir;

  folders = (directories *)malloc(sizeof(directories));
  current = folders;

  while((dir = readdir(currentDirectory)) != NULL) {
    char target = '.';
    if(strrchr(dir->d_name, target) == NULL) {
      current->path = dir->d_name; 
      current->next = (directories *)malloc(sizeof(directories));
      current = current->next;
    }
  }

  closedir(currentDirectory);
  return folders;
}

files *findAllFilesInCurrentDir(char *path) 
{  
  DIR *currentDirectory;
  currentDirectory = opendir(path);
  files *allfiles, *current;
  struct dirent *dir;

  allfiles = (files *)malloc(sizeof(files));
  current = allfiles;

  while((dir = readdir(currentDirectory)) != NULL) {
    if(strcmp(dir->d_name, ".file") > 0) {
      if(strrchr(dir->d_name, '.') != NULL) {
        char *fileExt = strrchr(dir->d_name, '.');
        //Refactor
        if(strcmp(fileExt + 1, "js") == 0) {
          current->path = dir->d_name;
          current->ext = (fileExt + 1);
          current->next = (files *)malloc(sizeof(files));
          current = current->next; 
        } else if(strcmp(fileExt + 1, "css") == 0) {
          current->path = dir->d_name;
          current->ext = (fileExt + 1);
          current->next = (files *)malloc(sizeof(files));
          current = current->next;
        } else if(strcmp(fileExt + 1, "html") == 0) {
          current->path = dir->d_name;
          current->ext = (fileExt + 1);
          current->next = (files *)malloc(sizeof(files));
          current = current->next;
        } else if(strcmp(fileExt + 1, "c") == 0) {
          current->path = dir->d_name;
          current->ext = (fileExt + 1);
          current->next = (files *)malloc(sizeof(files));
          current = current->next;
        } else if(strcmp(fileExt + 1, "h") == 0) {
          current->path = dir->d_name;
          current->ext = (fileExt + 1);
          current->next = (files *)malloc(sizeof(files));
          current = current->next;
        } 
      }
    }
  }

  closedir(currentDirectory);
  return allfiles;
}

