#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Required for chdir()
#include "build.h"

// --- Color Definitions ---
#define RESET_COLOR    "\x1b[0m"
#define ROJO_T         "\x1b[31m"
#define VERDE_T        "\x1b[32m"
#define AMARILLO_T     "\x1b[33m"
#define AZUL_T         "\x1b[34m"
#define MAGENTA_T      "\x1b[35m"

// --- Helper Functions ---
void clear_screen(void) { call_bash("clear"); }
void bgets(char *str, int size) { fgets(str, size, stdin); strtok(str, "\n"); }

// --- User Prompt Functions ---
void clone_kernel_prompt(char *clone_answer, char *version_answer) {
    printf(AZUL_T "\nDownload kernel source from git?" RESET_COLOR);
    printf("\n[y/n]:"); bgets(clone_answer, 5);

    if (strcmp(clone_answer, "y") == 0) {
        printf(AZUL_T "\nWhich version to clone?" RESET_COLOR);
        printf("\n[m] mainline (latest) / [l] lts (long-term support):");
        bgets(version_answer, 5);
    }
}

void clean_kernel_prompt(char *input) {
    printf(VERDE_T "Write 'h' for help\n" RESET_COLOR);
    printf(AZUL_T "\nClean previous build artifacts?" RESET_COLOR);
    printf("\n[y/n/h]:"); bgets(input, 5);
    if (strcmp(input, "h") == 0) {
        puts(AMARILLO_T "\nInfo: Deletes prior kernel build files and configurations." RESET_COLOR);
        clean_kernel_prompt(input);
    }
}

void config_kernel_prompt(char *input) {
    printf(AZUL_T "\nUse production (non-debug) kernel config?" RESET_COLOR);
    printf("\n[y/n/h]:"); bgets(input, 5);
    if (strcmp(input, "h") == 0) {
        puts(AMARILLO_T "\nY: Use standard production config (no debug symbols).");
        puts("N: Use standard debug config." RESET_COLOR);
        config_kernel_prompt(input);
    }
}

void optimized_kernel_prompt(char *input) {
    printf(AZUL_T "\nOptimize with minimal drivers for this hardware (localmodconfig)?" RESET_COLOR);
    printf("\n[y/n/h]:"); bgets(input, 5);
    if (strcmp(input, "h") == 0) {
        puts(AMARILLO_T "\nInfo: Compiles only the modules needed for your current hardware." RESET_COLOR);
        optimized_kernel_prompt(input);
    }
}

void optimized_hw_build_prompt(char *input) {
    puts(AZUL_T "\nOptimize build with CPU-specific instructions (-march=native)?" RESET_COLOR);
    printf("\n[y/n/h]:"); bgets(input, 5);
    if (strcmp(input, "h") == 0) {
        puts(AMARILLO_T "\nInfo: Generates a kernel highly optimized for your specific CPU." RESET_COLOR);
        optimized_hw_build_prompt(input);
    }
}

void edit_kernel_prompt(char *input) {
    puts(AZUL_T "\nManually edit kernel configuration (menuconfig)?" RESET_COLOR);
    printf("\n[y/n]:"); bgets(input, 5);
}

// --- Main Application Logic ---
int main(void) {
    KernelConfig config;
    if (load_config("build_fedora/config.cfg", &config) != 0) {
        fprintf(stderr, ROJO_T "Failed to load configuration. Exiting.\n" RESET_COLOR);
        return 1;
    }

    char clone_choice[5] = "n", version_choice[5] = {0};
    char clean_choice[5], config_choice[5], optimize_choice[5], hw_optimize_choice[5], edit_choice[5];
    char command[1024];

    clear_screen();

    // Step 0: Clone sources
    clone_kernel_prompt(clone_choice, version_choice);
    if (strcmp(clone_choice, "y") == 0) {
        char clone_command[512];
        char *repo_dir = NULL;

        if ((strcmp(version_choice, "m") && strcmp(version_choice, "M") && strcmp(version_choice, "mainline")) == 0) {
            puts(MAGENTA_T "Cloning latest mainline kernel..." RESET_COLOR);
            strcpy(clone_command, "git clone --depth=1 https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git");
            repo_dir = "linux";
        } else if ((strcmp(version_choice, "l") && strcmp(version_choice, "L") && strcmp(version_choice, "lts")) == 0) {
            puts(MAGENTA_T "Cloning latest LTS kernel..." RESET_COLOR);
            strcpy(clone_command, "git clone --depth=1 https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git");
            repo_dir = "linux-stable";
        }

        if (repo_dir) {
            call_bash(clone_command);
            if (chdir(repo_dir) != 0) {
                perror(ROJO_T "Failed to change directory");
                return 1;
            }
        }
    }
    
    // Step 1: Gather build options
    clean_kernel_prompt(clean_choice);
    config_kernel_prompt(config_choice);
    optimized_kernel_prompt(optimize_choice);
    optimized_hw_build_prompt(hw_optimize_choice);
    edit_kernel_prompt(edit_choice);

    // Step 2: Clean
    if (strcmp(clean_choice, "y") == 0) {
        puts(MAGENTA_T "\nCleaning old build files..." RESET_COLOR);
        call_bash("make clean && make mrproper");
    }

    // Step 3: Set initial .config
    const char *config_target = (strcmp(config_choice, "y") == 0) ? config.default_config : config.debug_config;
    snprintf(command, sizeof(command), "%s make %s", config.use_llvm_clang ? "CC=clang LLVM=1" : "", config_target);
    puts(MAGENTA_T "\nApplying kernel configuration..." RESET_COLOR);
    call_bash(command);

    // Step 4: Optimize with localmodconfig
    if (strcmp(optimize_choice, "y") == 0) {
        snprintf(command, sizeof(command), "%s make localmodconfig", config.use_llvm_clang ? "CC=clang LLVM=1" : "");
        puts(MAGENTA_T "\nOptimizing with localmodconfig..." RESET_COLOR);
        call_bash(command);
    }

    // Step 5: Manual edit
    if (strcmp(edit_choice, "y") == 0) {
        puts(MAGENTA_T "\nStarting menuconfig..." RESET_COLOR);
        call_bash("make menuconfig");
    }

    // Step 6: Final Build
    char build_command[1024] = {0};
    const char *compiler_prefix = config.use_llvm_clang ? "CC=clang LLVM=1 " : "";
    snprintf(build_command, sizeof(build_command), "%smake -j$(nproc)", compiler_prefix);

    if (strcmp(hw_optimize_choice, "y") == 0 && config.use_native_march) {
        strcat(build_command, " KCFLAGS=\"-march=native\" KCPPFLAGS=\"-march=native\"");
    }
    if (config.use_strip) {
        strcat(build_command, " INSTALL_MOD_STRIP=1");
    }
    strcat(build_command, " rpm-pkg");

    puts(MAGENTA_T "\nStarting final kernel build..." RESET_COLOR);
    call_bash(build_command);

    puts(VERDE_T "\nBuild process finished!" RESET_COLOR);
    return 0;
}