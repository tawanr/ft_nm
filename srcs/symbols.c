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

void merge(Elf_Symbol **symbols, const int begin, const int mid,
           const int end) {
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
    while (i < n1 && j < n2) {
        if (ft_strcmp(left[i]->name, right[j]->name) <= 0) {
            symbols[k] = left[i];
            i++;
        } else {
            symbols[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        symbols[k] = left[i];
        i++;
        k++;
    }
    while (j < n2) {
        symbols[k] = right[j];
        j++;
        k++;
    }
    free(left);
    free(right);
}

Elf_Symbol **merge_sort(Elf_Symbol **symbols, const int begin, const int end) {
    if (begin < end) {
        const int mid = begin + (end - begin) / 2;
        merge_sort(symbols, begin, mid);
        merge_sort(symbols, mid + 1, end);
        merge(symbols, begin, mid, end);
    }
    return symbols;
}

void sort_symbols(Elf_Symbol ***symbols, size_t count) {
    *symbols = merge_sort(*symbols, 0, count - 1);
}

char get_symbol_type(Elf_Symbol *symbol) {
    int type = symbol->type;
    int bind = symbol->binding;
    int special_index = symbol->special_index;
    unsigned long flags = symbol->flags;
    unsigned int sh_type = symbol->sh_type;

    (void)bind;
    char rtn = 0;
    if (type == STT_FILE)
        return rtn;
    if (bind == STB_GNU_UNIQUE)
        rtn = 'u';
    else if (bind == STB_WEAK) {
        rtn = 'W';
        if (special_index == SHN_UNDEF)
            rtn = 'w';
    } else if (bind == STB_WEAK && type == STT_OBJECT) {
        rtn = 'V';
        if (special_index == SHN_UNDEF)
            rtn = 'v';
    } else if (special_index == SHN_UNDEF) {
        rtn = 'U';
    } else if (special_index == SHN_ABS) {
        rtn = 'A';
    } else if (special_index == SHN_COMMON) {
        rtn = 'C';
    } else if (sh_type == SHT_NOBITS && flags == (SHF_ALLOC | SHF_WRITE)) {
        rtn = 'B';
    } else if (sh_type == SHT_PROGBITS && flags == SHF_ALLOC) {
        rtn = 'R';
    } else if (sh_type == SHT_PROGBITS && flags == (SHF_ALLOC | SHF_WRITE)) {
        rtn = 'D';
    } else if (sh_type == SHT_PROGBITS &&
               flags == (SHF_ALLOC | SHF_EXECINSTR)) {
        rtn = 'T';
    } else if (sh_type == SHT_DYNAMIC) {
        rtn = 'D';
    } else if (special_index == 26) {
        rtn = 'B';
    } else if (special_index == 25) {
        rtn = 'D';
    } else if (special_index == 19 || special_index == 4) {
        rtn = 'R';
    } else if (special_index == 22 || special_index == 21) {
        rtn = 'D';
    } else {
        rtn = '?';
    }
    if (bind == STB_LOCAL && rtn != '?')
        rtn = ft_tolower(rtn);
    return rtn;
}

void print_symbols(Elf_Symbol **symbols, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (*symbols[i]->name == '\0')
            continue;
        char type = get_symbol_type(symbols[i]);
        if (type == '\0')
            continue;
        if (symbols[i]->special_index != SHN_UNDEF)
            ft_printf("%016x ", symbols[i]->value);
        else
            ft_printf("                 ");
        if (type != 0)
            ft_printf("%c ", type);
        else {
            ft_printf("  ");
        }
        ft_printf("%s\n", symbols[i]->name);
    }
}