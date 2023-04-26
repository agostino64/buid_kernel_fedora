//
// Linux/build_fedora/build.c
// Linux kernel
//
// Created by Aarch-64 on 23/4/2023
// Copyright © 2023 Aarch-64. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "build.h"

#define RESET_COLOR    "\x1b[0m"
#define NEGRO_T        "\x1b[30m"
#define NEGRO_F        "\x1b[40m"
#define ROJO_T     "\x1b[31m"
#define ROJO_F     "\x1b[41m"
#define VERDE_T        "\x1b[32m"
#define VERDE_F        "\x1b[42m"
#define AMARILLO_T "\x1b[33m"
#define    AMARILLO_F  "\x1b[43m"
#define AZUL_T     "\x1b[34m"
#define    AZUL_F      "\x1b[44m"
#define MAGENTA_T  "\x1b[35m"
#define MAGENTA_F  "\x1b[45m"
#define CYAN_T     "\x1b[36m"
#define CYAN_F     "\x1b[46m"
#define BLANCO_T   "\x1b[37m"
#define BLANCO_F   "\x1b[47m"

void clear_screen(void)
{
  call_bash("clear");
}

void bgets(char *str)
{
  fgets(str, 5, stdin);
  strtok(str, "\n");
}

void clean_kernel(char *input_clean_conf)
{
  printf(VERDE_T "Write 'h' for help\n" RESET_COLOR);
  printf(AZUL_T "\nLimpiar antiguas compilaciones?" RESET_COLOR);
  printf("\n[y/n/h]:"); bgets(input_clean_conf);
  
  if (strcmp(input_clean_conf, "h") == 0 || strcmp(input_clean_conf, "H") == 0)
  {
    puts(AMARILLO_T "\nInfo: Elimina las compilaciones del kernel anterior");
    puts("Warn: Elimina la configuracion del kernel\n" RESET_COLOR);
    fflush(stdin);
    clean_kernel(input_clean_conf);
  }
}

void config_kernel(char *input_conf)
{
  printf(AZUL_T "\nDesea un kernel para produccion (sin depuracion)?\n" RESET_COLOR);
  printf("\n[y/n/h]:"); bgets(input_conf);
  
  if (strcmp(input_conf, "h") == 0 || strcmp(input_conf, "H") == 0)
  {
    puts(AMARILLO_T "\nY: stock config (sin depuracion)");
    puts("N: stock config (con depuracion)");
    puts("\nWarn: la depuracion puede llegar a generar codigo mas grande\ny consumir mas recursos de hardware\n" RESET_COLOR);
    fflush(stdin);
    config_kernel(input_conf);
  }
}

void optimized_kernel(char *input_optimiced_conf)
{
  printf(AZUL_T "\nDesea optimizar el kernel (controladores minimos)?\n" RESET_COLOR);
  printf("\n[y/n/h]:"); bgets(input_optimiced_conf);
  
  if (strcmp(input_optimiced_conf, "h") == 0 || strcmp(input_optimiced_conf, "H") == 0)
  {
    puts(AMARILLO_T "\nInfo: Compila solo los modulos nesesarios para su hardware");
    puts("Info: Puede tardar menos su compilacion al usar esto");
    puts("Warn: Es muy probable que este kernel solo funcione en su PC\n" RESET_COLOR);
    fflush(stdin);
    optimized_kernel(input_optimiced_conf);
  }
}

void optimized_hw_build(char *make_conf)
{
  puts(AZUL_T "\nDesea optimizar el kernel (instrucciones para su hw)?" RESET_COLOR);
  printf("\n[y/n/h]:"); bgets(make_conf);
  
  if (strcmp(make_conf, "h") == 0 || strcmp(make_conf, "H") == 0)
  {
    puts(AMARILLO_T "\nInfo: Optimiza el kernel, usando instrucciones espesificas para su prosesador");
    puts("Warn: Este kernel solo funcionara para la arquitectura de su prosesador\nejm: corei7, skylake, x86-64, x86-64-v2, x86-64-v3, x86-64-v4 etc\n" RESET_COLOR);
    fflush(stdin);
    optimized_hw_build(make_conf);
  }
}

void edit_kernel(char *input_edit_conf)
{
  puts(AZUL_T "\nDesea modificar la configuracion del kernel?" RESET_COLOR);
  puts(AMARILLO_T "\nNo editar. Solo editar si es nesesario" RESET_COLOR);
  printf("\n[y/n]:"); bgets(input_edit_conf);
}

int rev_clean_kernel(char *input_clean_conf)
{
  // for 'clean_kernel' function
  if (strcmp(input_clean_conf, "y") == 0 || strcmp(input_clean_conf, "Y") == 0)
  {
    puts(MAGENTA_T "\nEliminando compilaciones antiguas" RESET_COLOR);
    cfg_exec("build_fedora/config.cfg", "CLEAN_BUILD");
    return -1;
  }
  else
    return -1;
}

int rev_config_kernel(char *input_conf)
{
  // for 'config_kernel' function     
  if (strcmp(input_conf, "y") == 0 || strcmp(input_conf, "Y") == 0)
  {
    puts(MAGENTA_T "configuring kernel for stock configuration");
    puts("\nCompilando x86_64_fedora_defconfig" RESET_COLOR);
    cfg_exec("build_fedora/config.cfg", "STOCK_CFG");
    return -1;
  }
  else
  {
    puts(MAGENTA_T "configuring kernel for stock debug configuration");
    puts("\nCompilando x86_64_fedora-debug_defconfig" RESET_COLOR);
    cfg_exec("build_fedora/config.cfg", "DEBUG_CFG");
    return -1;
  }
}

int rev_optimized_kernel(char *input_optimiced_conf)
{
  // for 'optimized_kernel' function
  if (strcmp(input_optimiced_conf, "y") == 0 || strcmp(input_optimiced_conf, "Y") == 0)
  {
    puts(MAGENTA_T "Optimizando el kernel (controladores minimos)" RESET_COLOR);
    cfg_exec("build_fedora/config.cfg", "OPTIMIZE_CFG");
    return -1;
  }
  else
    return -1;
}

int rev_edit_kernel(char *input_edit_conf)
{
  // for 'edit_kernel' function   
  if (strcmp(input_edit_conf, "y") == 0 || strcmp(input_edit_conf, "Y") == 0)
  {
    puts(MAGENTA_T "\nPreparando para editar la configuracion" RESET_COLOR);
    cfg_exec("build_fedora/config.cfg", "EDIT_KERNEL");
    return -1;
  }
  else
    return -1;
}

int rev_optimized_hw_build(char *make_conf)
{
  // for 'optimized_hw_build' function         
    puts("Iniciando compilacion...\n");
      
    if (strcmp(make_conf, "y") == 0 || strcmp(make_conf, "Y") == 0)
    {
      puts(MAGENTA_T "Compilando kernel optimizado para las instrucciones de la CPU local" RESET_COLOR);
      cfg_exec("build_fedora/config.cfg", "OPTIMIZE_HW_CFG");
    }
    else
    {
      puts(MAGENTA_T "compilando kernel sin optimizacion para el hardware local" RESET_COLOR);
      cfg_exec("build_fedora/config.cfg", "NON_OPTIMIZE_HW_CFG");
    }
    return 1;
}

int main(void)
{
  char input_clean_conf[5];
  char input_conf[5];
  char input_optimiced_conf[5];
  char input_edit_conf[5];
  char make_conf[5];
  
  clear_screen();
    
  // get results
  clean_kernel(input_clean_conf);
  config_kernel(input_conf);
  optimized_kernel(input_optimiced_conf);
  optimized_hw_build(make_conf);
  edit_kernel(input_edit_conf);
  
  // check results
  rev_clean_kernel(input_clean_conf);
  rev_config_kernel(input_conf);
  rev_optimized_kernel(input_optimiced_conf);
  rev_edit_kernel(input_edit_conf);
  rev_optimized_hw_build(make_conf);
  
  return 0;
}
