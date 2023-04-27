#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH 1024

void call_bash(const char *command)
{
    pid_t pid = fork();  // Create a child process

    if (pid == -1) {  // fork() failed
        perror("Error al crear el proceso");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {  // Child process
        // Especificar el camino completo del archivo shell
        char *path = "/bin/bash";

        // Set the UID of the child process
        if (setreuid(getuid(), getuid()) == -1) {
            perror("Error al establecer el UID del proceso hijo");
            exit(EXIT_FAILURE);
        }

        // Use non-blocking I/O to execute the command
        execlp(path, path, "-c", command, (char *)NULL);

        // Si execlp retorna, significa que ha habido un error
        perror("Error al ejecutar el comando");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        // Read the output of the command using non-blocking I/O
        char buffer[4096];
        int n = 0;

        while (1) {
            fd_set set;
            FD_ZERO(&set);
            FD_SET(STDIN_FILENO, &set);

            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 1000;

            int ret = select(STDIN_FILENO + 1, &set, NULL, NULL, &tv);

            if (ret < 0) {
                perror("Error al leer la salida del comando");
                exit(EXIT_FAILURE);
            } else if (ret == 0) {
                // Timeout
                break;
            } else {
                n += read(STDOUT_FILENO, buffer + n, sizeof(buffer) - n);
            }
        }

        // Esperar a que el hijo termine
        int status;
        waitpid(pid, &status, 0);
    }
}

void cfg_exec(char *name_file, char *key_text)
{
    FILE *config_file = fopen(name_file, "r");
    if (!config_file) {
        printf("Failed to open config file\n");
        exit(1);
    }
    
    char line[MAX_LINE_LENGTH];
    char get_command[MAX_LINE_LENGTH] = "";
    char get_command_clang[MAX_LINE_LENGTH] = "CC=clang LLVM=1";
    int clang = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, config_file)) {
        if (line[0] == '#') {
            // Skip comments
            continue;
        }
        if (line[0] == '%') {
          // Use clang and skip '%'
          continue;
          clang = 1;
        }
        
        char key[MAX_LINE_LENGTH] = "";
        char value[MAX_LINE_LENGTH] = "";
        sscanf(line, "%[^= ] = %[^\n]", key, value);
        
        if (strcmp(key, key_text) == 0) {
            sscanf(value, "\"%[^\"]\"", get_command);
        }
    }
    fclose(config_file);

    if (clang = 1)
    {
      snprintf(get_command_clang + strlen(get_command_clang), MAX_LINE_LENGTH - strlen(get_command_clang), " %s", get_command);
      call_bash(get_command_clang);
    }
    else
    {
      call_bash(get_command);
    }
}
