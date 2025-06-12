/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tratanat <tawan.rtn@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 16:13:44 by tratanat          #+#    #+#             */
/*   Updated: 2024/07/31 15:27:16 by tratanat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        ft_putstr_fd("Invalid argument(s)\n", STDERR_FILENO);
        return 1;
    }
    Config config = {argv[0], argv[1]};

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        error_file_not_found(&config);
        return 1;
    }
    Elf_File64 elf_file;
    elf_file.symbol_count = 0;
    parse_file64(&config, fd, &elf_file);
    if (elf_file.symbol_count == 0) {
        error_no_symbols(&config);
        return 1;
    }
    if (elf_file.file == NULL)
        return 1;
    sort_symbols(&(elf_file.symbols), elf_file.symbol_count);
    print_symbols(elf_file.symbols, elf_file.symbol_count);
    clean_elf64(&elf_file);

    return 0;
}