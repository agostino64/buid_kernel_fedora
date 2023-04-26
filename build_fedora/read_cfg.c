#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

        // Ejecutar el archivo shell con los privilegios de usuario necesarios
        setuid(getuid());
        execlp(path, path, "-c", command, (char *)NULL);

        // Si execlp retorna, significa que ha habido un error
        perror("Error al ejecutar el comando");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        // Esperar a que el hijo termine
        int status;
        waitpid(pid, &status, 0);
    }
}

void cfg_exec(char *name_file, char *key_text)
{
    char get_command[256] = "";
    char get_command_sh[256] = "";
    
    FILE *config_file = fopen(name_file, "r");
    if (!config_file) {
        printf("Failed to open config file\n");
        exit(1);
    }
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, config_file)) {
        if (line[0] == '#') {
            // Skip comments
            continue;
        }
        
        char key[MAX_LINE_LENGTH] = "";
        char value[MAX_LINE_LENGTH] = "";
        sscanf(line, "%[^= ] = %[^\n]", key, value);
        
        if (strcmp(key, key_text) == 0) {
            sscanf(value, "\"%[^\"]\"", get_command);
        }
    }
    fclose(config_file);

    strcpy(get_command_sh, "/bin/bash ");
    strncat(get_command_sh, get_command, sizeof(get_command_sh) - strlen(get_command_sh) - 1);
    call_bash(get_command_sh);
}
