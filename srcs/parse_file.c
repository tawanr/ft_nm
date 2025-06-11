/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tratanat <tawan.rtn@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 16:10:44 by tratanat          #+#    #+#             */
/*   Updated: 2024/07/31 15:26:31 by tratanat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void get_symtab_info(Elf_File64 *elf_file, Elf_Symtab64 *info,
                     const unsigned int section_type) {
    info->symtab = NULL;
    for (int i = 0; i < elf_file->file_headers->e_shnum; i++) {
        if (elf_file->section_headers[i].sh_type == section_type) {
            info->symtab =
                (Elf64_Sym *)(elf_file->file +
                              elf_file->section_headers[i].sh_offset);
            info->symcount = elf_file->section_headers[i].sh_size /
                             elf_file->section_headers[i].sh_entsize;
            info->symstr =
                (char *)(elf_file->file +
                         elf_file
                             ->section_headers[elf_file->section_headers[i]
                                                   .sh_link]
                             .sh_offset);
            return;
        }
    }
}

Elf_Symbol **extract_symbols64(Elf_File64 *elf_file) {
    Elf_Symtab64 info;
    Elf_Symbol **symbols = NULL;
    info.symcount = 0;
    info.symtab = NULL;
    info.symstr = NULL;

    get_symtab_info(elf_file, &info, SHT_SYMTAB);
    if (info.symtab == NULL)
        return NULL;

    elf_file->symbol_count = info.symcount;
    symbols = malloc(sizeof(Elf_Symbol) * info.symcount);
    for (int i = 0; i < info.symcount; i++) {
        symbols[i] = malloc(sizeof(Elf_Symbol));
        symbols[i]->name = info.symstr + info.symtab[i].st_name;
        symbols[i]->value = info.symtab[i].st_value;
        symbols[i]->type = ELF64_ST_TYPE(info.symtab[i].st_info);
        symbols[i]->binding = ELF64_ST_BIND(info.symtab[i].st_info);
        symbols[i]->section_rel = 0;
        if (info.symtab[i].st_shndx < elf_file->file_headers->e_shentsize)
            symbols[i]->section_rel =
                elf_file->section_headers[info.symtab[i].st_shndx].sh_type;
    }
    return symbols;
}

void parse_file64(Config *config, int fd, Elf_File64 *elf_file) {
    struct stat file_stat;

    elf_file->file = NULL;
    fstat(fd, &file_stat);
    if (file_stat.st_size <= 0)
        return;
    elf_file->file_size = file_stat.st_size;
    if (elf_file->file_size <= 0)
        return;
    elf_file->file =
        mmap(0, elf_file->file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (elf_file == NULL)
        return;
    elf_file->file_headers = (Elf64_Ehdr *)(elf_file->file);
    if (elf_file->file_headers->e_ident[EI_MAG0] != ELFMAG0 ||
        elf_file->file_headers->e_ident[EI_MAG1] != ELFMAG1 ||
        elf_file->file_headers->e_ident[EI_MAG2] != ELFMAG2 ||
        elf_file->file_headers->e_ident[EI_MAG3] != ELFMAG3) {
        error_file_format_not_recognized(config);
        munmap(elf_file->file, elf_file->file_size);
        elf_file->file = NULL;
        return;
    }
    if (elf_file->file_headers->e_shoff > elf_file->file_size ||
        elf_file->file_headers->e_shoff <= sizeof(Elf64_Ehdr))
        return;
    elf_file->section_headers =
        (Elf64_Shdr *)(elf_file->file + elf_file->file_headers->e_shoff);
    elf_file->symbols = extract_symbols64(elf_file);
}

void clean_elf64(Elf_File64 *elf_file) {
    for (size_t i = 0; i < elf_file->symbol_count; i++)
        free(elf_file->symbols[i]);
    munmap(elf_file->file, elf_file->file_size);
    free(elf_file->symbols);
}