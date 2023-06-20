#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

#define MSG_LEN 5

int try_rw(int desc, char* buff, int needed, char mode) {
  int chars_done = 0,
      possible_buff_len = 100;  // must be longer than needed message len
  char possible_msg[possible_buff_len];

  while (chars_done < needed) {
    int tmp;
    if (mode == 'r')
      tmp =
          read(desc, possible_msg + chars_done, possible_buff_len - chars_done);
    else if (mode == 'w')
      tmp = write(desc, buff + chars_done, needed - chars_done);
    else
      return -2;

    if (tmp == -1) return tmp;
    if (mode == 'r' && tmp > needed) {
      printf("Passed message is %d longer than needed", tmp);
      return -1;
    }

    chars_done += tmp;
  }

  if (mode == 'r') memcpy(buff, possible_msg, needed);

  return 0;
}

void exit_after_failed_read(int err_code) {
  if (err_code == -1) {
    printf("Failed to read!");
    exit(1);
  }
}

void exit_after_failed_write(int err_code) {
  if (err_code == -1) {
    printf("Failed to write!");
    exit(1);
  }
}

void exit_after_failed_close(int err_code) {
  if (err_code == -1) {
    printf("Failed to close descriptor");
    exit(1);
  }
}

void exit_after_failed_pipe(int err_code) {
  if (err_code == -1) {
    printf("Failed to create pipe");
    exit(1);
  }
}

int main() {
  int ret_val, filedesc_parent[2], filedesc_child[2];

  exit_after_failed_pipe(pipe(filedesc_parent));
  exit_after_failed_pipe(pipe(filedesc_child));

  char buff[MSG_LEN];
  char* message;

  switch (fork()) {
    case -1:
      printf("Failed to fork processes");
      exit(1);

    case 0:
      message = "pong";

      exit_after_failed_read(try_rw(filedesc_parent[0], buff, MSG_LEN, 'r'));
      exit_after_failed_close(close(filedesc_parent[0]));

      printf("%d: got %s\n", getpid(), buff);

      exit_after_failed_write(try_rw(filedesc_child[1], message, MSG_LEN, 'w'));
      exit_after_failed_close(close(filedesc_child[1]));

      exit(0);

    default:
      message = "ping";

      exit_after_failed_write(
          try_rw(filedesc_parent[1], message, MSG_LEN, 'w'));
      exit_after_failed_close(close(filedesc_parent[1]));

      ret_val = wait(0);
      if (ret_val == -1) {
        printf("Failed to wait process");
        exit(1);
      }

      exit_after_failed_read(try_rw(filedesc_child[0], buff, MSG_LEN, 'r'));
      exit_after_failed_close(close(filedesc_child[0]));

      printf("%d: got %s\n", getpid(), buff);

      exit(0);
  }
}