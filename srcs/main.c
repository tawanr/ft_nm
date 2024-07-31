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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        ft_putstr_fd("Invalid argument(s)\n", STDERR_FILENO);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    Elf_File64 elf_file;
    parse_file64(fd, &elf_file);
    sort_symbols(&(elf_file.symbols), elf_file.symbol_count);
    print_symbols(elf_file.symbols, elf_file.symbol_count);
    clean_elf64(&elf_file);

    return 0;
}