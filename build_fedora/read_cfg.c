#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "build.h"

#define MAX_LINE_LENGTH 256

/**
 * @brief Parses the config file to populate the KernelConfig struct.
 */
int load_config(const char *filename, KernelConfig *config) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening config file");
        return -1;
    }

    // Initialize with default values
    config->use_llvm_clang = false;
    config->use_strip = false;
    config->use_native_march = false;
    strcpy(config->default_config, "defconfig");
    strcpy(config->debug_config, "debug_defconfig");

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') {
            continue; // Skip comments and empty lines
        }

        char key[128], value[128];
        if (sscanf(line, "%[^= ] = %s", key, value) == 2) {
            if (strcmp(key, "USE_LLVM_CLANG") == 0) {
                config->use_llvm_clang = (strcmp(value, "true") == 0);
            } else if (strcmp(key, "USE_STRIP") == 0) {
                config->use_strip = (strcmp(value, "true") == 0);
            } else if (strcmp(key, "USE_NATIVE_MARCH") == 0) {
                config->use_native_march = (strcmp(value, "true") == 0);
            } else if (strcmp(key, "DEFAULT_CONFIG") == 0) {
                sscanf(value, "\"%[^\"]\"", config->default_config);
            } else if (strcmp(key, "DEBUG_CONFIG") == 0) {
                sscanf(value, "\"%[^\"]\"", config->debug_config);
            }
        }
    }

    fclose(file);
    return 0;
}

/**
 * @brief Executes a shell command using the standard system() call.
 */
void call_bash(const char *command) {
    printf("Executing: %s\n", command);
    int status = system(command);
    if (status != 0) {
        fprintf(stderr, "Command failed with exit status %d\n", status);
    }
}