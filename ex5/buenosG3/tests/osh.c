/*
 * OSM shell.
 */

#include "tests/lib.h"

#define BUFFER_SIZE 100
#define PATH_LENGTH 256
#define VFS_NAME_LENGTH 16

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
int cmd_cmp(int, char**);
int cmd_fcount(int, char**);
int cmd_file(int, char**);
int cmd_ls(int, char**);

cmd_t commands[] =
  { {"echo", cmd_echo, "print the arguments to the screen"},
    {"show", cmd_show, "print the contents of the given file to the screen"},
    {"read", cmd_read, "read a line from standard in and write it to a new file"},
    {"help", cmd_help, "show this help message"},
    {"exit", cmd_exit, "Terminates the shell"},
    {"rm", cmd_rm, "Remove file" },
    {"cp", cmd_cp, "Copy content of one file to another"},
    {"cmp",cmd_cmp, "Compare content of one file to another"},
    {"fcount", cmd_fcount, "Returns the number of files in a given volume"},
    {"file", cmd_file, "Copies the name of a file or volume at a specified"
      " index to a specified buffer."},
    {"ls", cmd_ls, "Prints all files or disks if no argument given or files of"
      " a volume if given"}
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
  printf("run_program, %s\n",prog);
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
int cmd_rm(int argc, char** argv) {
  if(argc < 2) {
    printf("Not enough arguments for remove\n");
    return 0;
  }
  syscall_delete(argv[1]);
  printf("Removed file %s\n",argv[1]); 
  return 0;
} 

int cmd_cp(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: cp <file> <file>\n");
    return 0;
  }
  int file1;
  int file2;
  if ((file1=syscall_open(argv[1])) < 0) {
    printf("Could not open %s.  Reason: %d\n", argv[1], file1);
    return 0;
  }
  char buffer[BUFFER_SIZE];
    syscall_delete(argv[2]);
// Laver fil med fil2 navn, med størrelse som file1.
   int counter = 0;
   int size = 0;
// Finder størrelsen på file1
   while ((counter = syscall_read(file1, buffer, BUFFER_SIZE))) {
      size += counter;
    }
    printf("%s copied to %s\n",argv[1], argv[2]);
// Laver en ny fil, file2 med den samme størrelse som file1
    syscall_create(argv[2],size);

  syscall_close(file1);
  file1 = syscall_open(argv[1]);
  int rd;
  file2 = syscall_open(argv[2]);
// Skriver file1 over i file2
   while ((rd = syscall_read(file1, buffer, BUFFER_SIZE))) {
    int wr = 0;
    int thiswr = 0;
    while (wr < rd) {
       if((thiswr = syscall_write(file2, buffer+wr, rd-wr)) <= 0) {
         syscall_close(file1);
         syscall_close(file2);
        return 1;
      }
      wr += thiswr;
    }
  }
  if (rd < 0) {
    printf("\nCall to syscall_read() failed.  Reason: %d.\n", rd);
    syscall_close(file1);
    syscall_close(file2);
    return 1;
  } else {
    syscall_close(file1);
    syscall_close(file2);
    return 0;
  }
}

int cmd_cmp(int argc, char** argv) {
 if (argc != 3) {
   printf("Usage: cmp <file> <file>\n");
   return 1;
  }
  int fd;
  int fe;
  if ((fd=syscall_open(argv[1])) < 0) {
    printf("Could not open %s. Reason: %d\n", argv[1], fd);
    return 1;
  }
  if ((fe=syscall_open(argv[2])) < 0) {
    printf("Could not open %s. Reason: %d\n", argv[2], fe);
  }

  int rd;
  char buffer[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
// Tjekker om file1 og file2 er lige store.
  while ((rd = syscall_read(fd, buffer, BUFFER_SIZE))) {
    if (rd != syscall_read(fe, buffer2, BUFFER_SIZE)) {
      printf("The files are not equal\n");
      return 0;
    }
    int wr = 0;
    while (wr < rd) {
// Tjekker byte for byte om de to filer er ens.
      if ((buffer[wr] != buffer2[wr])) {
        printf("The files are not equal\n");
        syscall_close(fd);
        syscall_close(fe);
        return 1;
      }
      wr++;
    }
  }
// Tjekker om størrelsen er mindre end 0
  if (rd < 0) {
    printf("\nCall to syscall_read() failed. Reason: %d.\n", rd);
    syscall_close(fd);
    syscall_close(fe);
    return 1;
// Hvis alle ovenstående går igennem er filerne ens.
  } else {
    printf("\nThe files are equal!\n");
    syscall_close(fd);
    syscall_close(fe);
    return 0;
  }
}

int cmd_fcount(int argc, char** argv) {
 //Giver null hvis der kun er 1 argument
 if(argc >= 2) {
   printf("Number of files: %d\n",syscall_filecount(argv[1]));
   return syscall_filecount(argv[1]); 
 }
 printf("%d\n",syscall_filecount(NULL));
   return syscall_filecount(NULL);

} 
int cmd_file(int argc, char** argv) {
  int index = argc;
  char buffer[BUFFER_SIZE];
  if(argc < 2) {
    syscall_file(NULL,index,buffer);
    int i = 0; 
    for(i=0; i < VFS_NAME_LENGTH; i++) { 
      printf("%c",buffer[i]);
    }
      printf("\n");
      return 0;
  } 
  syscall_file(argv[1],index,buffer);
  int i = 0;
  for(i=0; i < VFS_NAME_LENGTH; i++) {
    printf("%c",buffer[i]);
  }
    return 0;
}
int cmd_ls (int argc, char** argv) {
  char buffer[BUFFER_SIZE];
  int i=0;
  if(argc < 2) {
    int count = syscall_filecount(NULL);
    for(i=0;i < count; i++) {
      syscall_file(NULL,i,buffer);
      printf("%s\n",buffer);
    }
    return 0;
  }
// I tilfælde af at name er navnet på en Mounted volume
  int count = syscall_filecount(argv[1]);
  int n = 0;
  while(i < count) {
    syscall_file(argv[1],n,buffer);
    if(strlen(buffer) > 0) {
      printf(" %s\n", buffer);
      i++;
      n++;
    }
    else n++;
  } 
  return 0;
}


