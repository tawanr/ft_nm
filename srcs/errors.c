/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                             :+:      :+:    :+: */
/*                                                    +:+ +:+         +:+     */
/*   By: tratanat <tawan.rtn@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 16:13:44 by tratanat          #+#    #+#             */
/*   Updated: 2024/07/31 15:27:16 by tratanat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void error_file_not_found(Config *config) {
    ft_putstr_fd("ft_nm: ", STDERR_FILENO);
    ft_putstr_fd(config->file_name, STDERR_FILENO);
    ft_putstr_fd(": no such file or directory\n", STDERR_FILENO);
}

void error_file_format_not_recognized(Config *config) {
    ft_putstr_fd("ft_nm: ", STDERR_FILENO);
    ft_putstr_fd(config->file_name, STDERR_FILENO);
    ft_putstr_fd(": file format not recognized\n", STDERR_FILENO);
}

void error_no_symbols(Config *config) {
    ft_putstr_fd("ft_nm: ", STDERR_FILENO);
    ft_putstr_fd(config->file_name, STDERR_FILENO);
    ft_putstr_fd(": no symbols\n", STDERR_FILENO);
}