/*
 * OSM shell.
 */

#include "tests/lib.h"

#define BUFFER_SIZE 100
#define PATH_LENGTH 256


typedef int (*cmd_fun_t)(int, char**);

typedef struct {
  char* name;
  cmd_fun_t func;
  char *desc;
} cmd_t;

int cmd_echo(int, char**);
int cmd_show(int, char**);
int cmd_read(int, char**);
int cmd_help();
int cmd_exit();
int cmd_rm(int, char**);
int cmd_cp(int, char**);

cmd_t commands[] =
  { {"echo", cmd_echo, "print the arguments to the screen"},
    {"show", cmd_show, "print the contents of the given file to the screen"},
    {"read", cmd_read, "read a line from standard in and write it to a new file"},
    {"help", cmd_help, "show this help message"},
    {"exit", cmd_exit, "Terminates the shell"},
    {"rm", cmd_rm, "Remove file" },
    {"cp", cmd_cp, "Copy content of one file to another"}
  };
#define N_COMMANDS sizeof(commands) / sizeof(cmd_t)



void print_prompt(int last_retval) {
  printf("%d> ", last_retval);
}

/* Note that tokenize(cmdline, argv) modifies cmdline by inserting NUL
   characters. */
int tokenize(char* cmdline, char** argv) {
  int argc = 0;
  int inword=0;
  char *s, *p;
  for (s = cmdline, p = cmdline; *s; s++) {
    if (*s == ' ' && inword) {
      inword=0;
      argv[argc++]=p;
      *s = '\0';
    } else if (*s != ' ' && !inword) {
      inword=1;
      p=s;
    }
  }
  if (inword) {
    argv[argc++]=p;
  }
  return argc;
}

int run_program(char* prog) {
  return syscall_join(syscall_exec(prog));
}

int run_command(char* cmdline) {
  char* argv[BUFFER_SIZE];
  int argc = tokenize(cmdline, argv);
  unsigned int i;
  if (argc == 0) {
    return 0;
  }
  for (i = 0; i < N_COMMANDS; i++) {
    if (strcmp(argv[0], commands[i].name) == 0) {
      return commands[i].func(argc, argv);
    }
  }
  return run_program(cmdline);
}

void help() {
  printf("Welcome to the Buenos Shell!\n");
  printf("The following commands are available:\n");
  unsigned int i;
  for (i = 0; i < N_COMMANDS; i++) {
    printf("  %s: %s\n", commands[i].name, commands[i].desc);
  }
}

int main(void) {
  char cmdline[BUFFER_SIZE];
  int ret = 0;
  help();
  while (1) {
    print_prompt(ret);
    readline(cmdline, BUFFER_SIZE);
    run_command(cmdline);
  }
  syscall_halt();
  return 0;
}



int cmd_echo(int argc, char** argv) {
  int i;
  for (i = 1; i < argc; i++) {
    printf("%s ", argv[i]);
  }
  puts("\n");
  return 0;
}

int cmd_show(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: show <file>\n");
    return 1;
  }
  int fd;
  if ((fd=syscall_open(argv[1])) < 0) {
    printf("Could not open %s.  Reason: %d\n", argv[1], fd);
    return 1;
  }

  int rd;
  char buffer[BUFFER_SIZE];
  while ((rd = syscall_read(fd, buffer, BUFFER_SIZE))) {
    int wr=0, thiswr;
    while (wr < rd) {
      if ((thiswr = syscall_write(1, buffer+wr, rd-wr)) <= 0) {
        printf("\nCall to syscall_write() failed.  Reason: %d.\n", wr);
        syscall_close(fd);
        return 1;
      }
      wr += thiswr;
    }
  }
  if (rd < 0) {
    printf("\nCall to syscall_read() failed.  Reason: %d.\n", rd);
    syscall_close(fd);
    return 1;
  } else {
    syscall_close(fd);
    return 0;
  }
}

int cmd_read(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: read <file>\n");
    return 1;
  }
  char text[BUFFER_SIZE];
  int count, ret, fd, wr;
  count = readline(text, BUFFER_SIZE - 1) + 1;
  text[count - 1] = '\n';
  text[count] = '\0';
  if ((ret=syscall_create(argv[1], count)) < 0) {
    printf("Could not create %s with initial size %d.  Reason: %d\n", argv[1], count, ret);
    return 1;
  }
  if ((fd=syscall_open(argv[1])) < 0) {
    printf("Could not open %s.  Reason: %d\n", argv[1], fd);
    return 1;
  }
  if ((wr=syscall_write(fd, text, count)) <= 0) {
    printf("\nCall to syscall_write() failed.  Reason: %d.\n", wr);
    syscall_close(fd);
    return 1;
  }
  syscall_close(fd);
  return 0;
}

int cmd_help() {
  help();
  return 0;
}
int cmd_exit() {
  puts("Exiting the system\n");
  syscall_halt();
  return 0;
}

//Hard to test without files
int cmd_rm(int argv, char** argc) {
  syscall_delete(argc[1]);
  return argv;
} 

int cmd_cp(int argv, char** argc) {
  if(argv < 2) { 
    printf("Not enough arguments in cp\n");
    return 0;
  } 
 
  char buffer[BUFFER_SIZE];
  //gør filerne klar til brug
  int file1 = syscall_open(argc[1]);
  int file2 = syscall_open(argc[2]);
  //Read og write, burde starte hvor der sidst har været læst. 
  //læser byte for byte
  while(syscall_read(file1, buffer, 1)) {
    syscall_write(file2, buffer, 1);
  }
  return 0;
}

int cmd_cmp(int argv, char** argc) {
  if(argv < 2) {
   printf("Not enough arguments in cmp\n");
   return 0;
  }
  char buffer[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  int file1 = syscall_open(argc[1]);
  int file2 = syscall_open(argc[2]);
  
  while(syscall_read(file1, buffer, 1) && syscall_read(file2, buffer, 1)) {
    if(!(buffer == buffer2)) {
      printf("files are not equal\n");
      return 0;
    }
  }
  printf("Files are equal\n");
  return 1;
}


/*
int toInt(char* s) {
int value = 0;
while (*s >= '0' && *s <= '9') {
    value = 10*value + (int)(*s - '0');
    s++;
  }

  return value;
}

//mk is still working out the kinks
int cmd_mk(int argv, char** argc) {

  puts("Hello, World!");
  printf("%s\n",argc[1]);
  printf("%d\n", toInt(argc[2]));
  syscall_create(argc[1],toInt(argc[2]));
  return argv;
}
*/

