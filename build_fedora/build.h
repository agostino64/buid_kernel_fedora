#ifndef BUILD_H
#define BUILD_H

#include <stdbool.h>

/**
 * @brief Holds all configuration settings loaded from config.cfg.
 */
typedef struct {
    bool use_llvm_clang;
    bool use_strip;
    bool use_native_march;
    char default_config[128];
    char debug_config[128];
} KernelConfig;

/**
 * @brief Parses the config file and populates the KernelConfig struct.
 * @param filename The path to the configuration file.
 * @param config A pointer to the KernelConfig struct to populate.
 * @return 0 on success, -1 on failure.
 */
int load_config(const char *filename, KernelConfig *config);

/**
 * @brief Executes a shell command.
 * @param command The command string to execute.
 */
void call_bash(const char *command);

// --- User Interaction Functions ---
void clone_kernel_prompt(char *clone_answer, char *version_answer);
void clean_kernel_prompt(char *input);
void config_kernel_prompt(char *input);
void optimized_kernel_prompt(char *input);
void edit_kernel_prompt(char *input);
void optimized_hw_build_prompt(char *input);

#endif // BUILD_H