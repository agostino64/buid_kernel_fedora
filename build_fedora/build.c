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
  clear_screen();
  printf("Write 'h' for help\n");
  printf("\nEsta es su primera compilacion?");
  printf("\n[y/n/h]:"); bgets(input_clean_conf);
}

void config_kernel(char *input_conf)
{
  printf("\nDesea un kernel para produccion (sin depuracion)?\n");
  printf("\n[y/n/h]:"); bgets(input_conf);
}

void optimized_kernel(char *input_optimiced_conf)
{
  printf("\nDesea optimizar el kernel (controladores minimos)?\n");
  printf("\n[y/n/h]:"); bgets(input_optimiced_conf);
}

void optimized_hw_build(char *make_conf)
{
  puts("\nDesea optimizar el kernel (instrucciones para su hw)?");
  printf("\n[y/n/h]:"); bgets(make_conf);
}

void edit_kernel(char *input_edit_conf)
{
  puts("\nDesea modificar la configuracion del kernel?");
  puts("\nNo editar. Solo editar si es nesesario");
  printf("\n[y/n]:"); bgets(input_edit_conf);
}

int rev_clean_kernel(char *input_clean_conf)
{
  // for 'clean_kernel' function
  if (strcmp(input_clean_conf, "y") == 0 || strcmp(input_clean_conf, "Y") == 0)
  {
    puts("\nEliminando compilaciones antiguas");
    cfg_exec("build_fedora/config.cfg", "CLEAN_BUILD");
    return -1;
  }
  else
    return -1;
  if (strcmp(input_clean_conf, "h") == 0 || strcmp(input_clean_conf, "H") == 0)
  {
    puts("Info: Elimina las compilaciones del kernel anterior");
    puts("Warn: Elimina la configuracion del kernel");
    getchar();
  }

  return 1;
}

int rev_config_kernel(char *input_conf)
{
  // for 'config_kernel' function     
  if (strcmp(input_conf, "y") == 0 || strcmp(input_conf, "Y") == 0)
  {
    puts("configuring kernel for stock configuration");
    puts("\nCompilando x86_64_fedora_defconfig");
    cfg_exec("build_fedora/config.cfg", "STOCK_CFG");
    return -1;
  }
  else if (strcmp(input_conf, "h") == 0 || strcmp(input_conf, "H") == 0)
  {
    puts("Y: stock config (sin depuracion)");
    puts("N: stock config (con depuracion)");
    puts("Warn: la depuracion puede llegar a generar codigo mas grande\ny consumir mas recursos de hardware");
    return -1;
  }
  else
  {
    puts("configuring kernel for stock debug configuration");
    puts("\nCompilando x86_64_fedora-debug_defconfig");
    cfg_exec("build_fedora/config.cfg", "DEBUG_CFG");
    return -1;
  }
  return 1;
}

int rev_optimized_kernel(char *input_optimiced_conf)
{
  // for 'optimized_kernel' function
  if (strcmp(input_optimiced_conf, "y") == 0 || strcmp(input_optimiced_conf, "Y") == 0)
  {
    puts("Optimizando el kernel (controladores minimos)");
    cfg_exec("build_fedora/config.cfg", "OPTIMIZE_CFG");
    return -1;
  }
  else if (strcmp(input_optimiced_conf, "h") == 0 || strcmp(input_optimiced_conf, "H") == 0)
  {
    puts("Info: Compila solo los modulos nesesarios para su hardware");
    puts("Info: Puede tardar menos su compilacion al usar esto");
    puts("Warn: Es muy probable que este kernel solo funcione en su PC");
    return -1;
  }
  else
    return -1;
  return 1;
}

int rev_edit_kernel(char *input_edit_conf)
{
  // for 'edit_kernel' function   
  if (strcmp(input_edit_conf, "y") == 0 || strcmp(input_edit_conf, "Y") == 0)
  {
    puts("\nPreparando para editar la configuracion");
    cfg_exec("build_fedora/config.cfg", "EDIT_KERNEL");
    return -1;
  }
  else
    return -1;
  return 1;
}

int rev_optimized_hw_build(char *make_conf)
{
  // for 'optimized_hw_build' function         
    puts("Iniciando compilacion...\n");
      
    if (strcmp(make_conf, "y") == 0 || strcmp(make_conf, "Y") == 0)
    {
      puts("Compilando kernel optimizado para las instrucciones de la CPU local");
      cfg_exec("build_fedora/config.cfg", "OPTIMIZE_HW_CFG");
    }
    else if (strcmp(make_conf, "h") == 0 || strcmp(make_conf, "H") == 0)
    {
      puts("Info: Optimiza el kernel, usando instrucciones espesificas para su prosesador");
      puts("Warn: Este kernel solo funcionara para la arquitectura de su prosesador\nejm: corei7, skylake, x86-64, x86-64-v2, x86-64-v3, x86-64-v4 etc");
    }
    else
    {
      puts("compilando kernel sin optimizacion para el hardware local");
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
