//
// Created by loumouli on 11/10/23.
//

#include "woody.h"

int save_new_file(t_bin *ptr) {
  int fd = open("./woody", O_WRONLY | O_CREAT, 0777);
  if (fd == -1) {
    printf("Error opening file\n");
    return 1;
  }
  write(fd, ptr->raw_data, ptr->data_len);
  close(fd);
  return 0;
}

int read_file(int file, char **result, size_t *len) {
  while (1) {
    char buff[8192];
    ssize_t retval = read(file, buff, sizeof(buff));
    if (retval == -1) {
      free(*result);  // Free allocated memory before returning
      return 3;
    }
    if (retval == 0) {
      break;
    }
    char *new_result = realloc(*result, *len + retval); // Check realloc return value
    if (new_result == NULL) {
      free((void *) result); // Free previously allocated memory
      return 4; // Return a different error code
    }
    *result = new_result;
    memcpy(*result + *len, buff, retval);
    *len += retval;
  }
  return 0;
}

int get_key(uint8_t *key) {
  int fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1) {
    dprintf(2, "Failed to open /dev/urandom\n");
    return 1;
  }
  if (read(fd, key, KEY_SIZE) == -1) {
    dprintf(2, "Failed to read /dev/urandom\n");
    return 1;
  }
  close(fd);
  return 0;
}