/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tratanat <tawan.rtn@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 16:11:52 by tratanat          #+#    #+#             */
/*   Updated: 2024/07/31 15:26:10 by tratanat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_NM_H__
#define __FT_NM_H__

#include "libft.h"
#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>

typedef struct {
    char *program_name;
    char *file_name;
} Config;

typedef struct {
    char *name;
    int value;
    int type;
    int binding;
    int section_rel;
    int special_index;
    unsigned long flags;
    unsigned int sh_type;
} Elf_Symbol;

typedef struct {
    Elf64_Sym *symtab;
    int symcount;
    char *symstr;
} Elf_Symtab64;

typedef struct {
    Elf32_Sym *symtab;
    int symcount;
    char *symstr;
} Elf_Symtab32;

typedef struct {
    Elf64_Ehdr *file_headers;
    Elf64_Shdr *section_headers;
    Elf_Symbol **symbols;
    size_t symbol_count;
    void *file;
    long unsigned int file_size;
} Elf_File64;

typedef struct {
    Elf32_Ehdr *file_headers;
    Elf32_Shdr *section_headers;
    Elf_Symbol **symbols;
    size_t symbol_count;
    void *file;
    long unsigned int file_size;
} Elf_File32;

void parse_file64(Config *config, int fd, Elf_File64 *elf_file);
void clean_elf64(Elf_File64 *elf_file);
void sort_symbols(Elf_Symbol ***symbols, size_t count);
void print_symbols(Elf_Symbol **symbols, size_t count);
void error_file_not_found(Config *config);
void error_file_format_not_recognized(Config *config);
void error_no_symbols(Config *config);

#endif