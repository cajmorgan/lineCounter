#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

/**************\
*STRING METHODS*
\**************/


int string_copy(char *dest, int size_of_dest, char *src, int size_of_src) {
  if(size_of_dest < size_of_src) {
    errno = 1;
    perror("Size of dest sting needs to be bigger or equal to size of src strirng");
    return 1; 
  }

  //Null string
  for(int i = 0; i < size_of_dest; i++) {
    dest[i] = '\0';
  }

  int position = 0;
  while(src[position] != '\0') {
    dest[position] = src[position];
    position++;
  } 
  
  dest[position] = '\0';

  return 0;
}


char *string_concat(char *string_one, char *string_two) {
  int total_length = strlen(string_one) + strlen(string_two);
  char *new_string = calloc(total_length, sizeof(char));
  string_copy(new_string, total_length + 1, string_one, strlen(string_one) + 1);

  int main_position = (strlen(string_one));
  int string_two_position = 0;
  while(string_two[string_two_position] != '\0') {
    new_string[main_position] = string_two[string_two_position];
    main_position++;
    string_two_position++;
  }

  new_string[main_position] = '\0';

  return new_string;
}

int add_char(char *dest, char char_to_add, int size_of_dest) {
  int dest_length = strlen(dest);
  if (dest_length + 1 >= size_of_dest) {
    errno = 1;
    perror("The size of the string needs to be at least one more than the length");
    return 1; 
  }

  dest[dest_length] = char_to_add;
  dest[dest_length + 1] = '\0';

  return 0;
}

int str_is_identical(char *string_one, char *string_two) {
  int length_of_string_one = strlen(string_one);
  int length_of_string_two = strlen(string_two);
  int is_identical = 0;

  for(int pos = 0; pos < length_of_string_one; pos++) {
    if(string_one[pos] == string_two[pos]) {
      is_identical += 1;
    }
  }

  if(is_identical == length_of_string_one && is_identical == length_of_string_two) {
    return 0;
  }
  
  return 1;
}


