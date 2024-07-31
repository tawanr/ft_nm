/* ************************************************************************** */
/*                                                                            */

/*   symbols.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tratanat <tawan.rtn@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 08:05:22 by tratanat          #+#    #+#             */
/*   Updated: 2024/07/31 12:16:52 by tratanat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void merge(Elf_Symbol **symbols, const int begin, const int mid, const int end)
{
    int n1 = mid - begin + 1;
    int n2 = end - mid;
    int i, j, k;

    Elf_Symbol **left = malloc(sizeof(Elf_Symbol) * n1);
    Elf_Symbol **right = malloc(sizeof(Elf_Symbol) * n2);

    for (i = 0; i < n1; i++)
        left[i] = symbols[begin + i];
    for (j = 0; j < n2; j++)
        right[j] = symbols[mid + j + 1];

    i = 0;
    j = 0;
    k = begin;
    while (i < n1 && j < n2)
    {
        if (ft_strcmp(left[i]->name, right[j]->name) <= 0)
        {
            symbols[k] = left[i];
            i++;
        }
        else
        {
            symbols[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        symbols[k] = left[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        symbols[k] = right[j];
        j++;
        k++;
    }
    free(left);
    free(right);
}

Elf_Symbol **merge_sort(Elf_Symbol **symbols, const int begin, const int end)
{
    if (begin < end)
    {
        const int mid = begin + (end - begin) / 2;
        merge_sort(symbols, begin, mid);
        merge_sort(symbols, mid + 1, end);
        merge(symbols, begin, mid, end);
    }
    return symbols;
}

void sort_symbols(Elf_Symbol ***symbols, size_t count)
{
    *symbols = merge_sort(*symbols, 0, count - 1);
}

char get_symbol_type(const int type, const int bind, const int rel)
{
    (void)bind;
    char rtn = 0;
    if (bind == STB_WEAK)
        rtn = 'w';
    else if (type == STT_FILE)
        rtn = 0;
    else if ((type == 0 || rel == 0) && rtn == '\0')
        rtn = 'u';
    else if (type == 1)
        rtn = 'd';
    else if (type == 2)
        rtn = 't';
    if (bind == 1)
        rtn = ft_toupper(rtn);
    return rtn;
}

void print_symbols(Elf_Symbol **symbols, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        if (*symbols[i]->name == '\0')
            continue;
        char type = get_symbol_type(symbols[i]->type, symbols[i]->binding, symbols[i]->section_rel);
        if (type == '\0')
            continue;
        if (symbols[i]->value != 0)
            ft_printf("%016x ", symbols[i]->value);
        else
            ft_printf("                 ");
        if (type != 0)
            ft_printf("%c ", type);
        else
        {
            ft_printf("  ");
        }
        ft_printf("%s\n", symbols[i]->name);
    }
}