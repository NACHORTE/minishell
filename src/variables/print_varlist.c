/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_varlist.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 16:58:17 by orudek            #+#    #+#             */
/*   Updated: 2023/10/01 16:47:22 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "t_var.h"

// Text attributes
#define RESET "\x1b[0m"      // Reset all attributes to default
#define BOLD "\x1b[1m"       // Bold or increased intensity
#define DIM "\x1b[2m"        // Dim (decreased intensity)
#define UNDERLINE "\x1b[4m"  // Underline
#define BLINK "\x1b[5m"      // Blink
#define REVERSE "\x1b[7m"    // Reverse (swap foreground and background colors)
#define HIDDEN "\x1b[8m"     // Hidden (not widely supported)

// Text colors
#define BLACK "\x1b[30m"     	// Black text
#define RED "\x1b[31m"       	// Red text
#define GREEN "\x1b[32m"     	// Green text
#define YELLOW "\x1b[33m"    	// Yellow text
#define BLUE "\x1b[34m"      	// Blue text
#define MAGENTA "\x1b[35m"   	// Magenta text
#define CYAN "\x1b[36m"      	// Cyan text
#define WHITE "\x1b[37m"     	// White text
#define ORANGE "\x1b[38;5;208m"	// Orange text

// Background colors
#define BG_BLACK "\x1b[40m"  // Black background
#define BG_RED "\x1b[41m"    // Red background
#define BG_GREEN "\x1b[42m"  // Green background
#define BG_YELLOW "\x1b[43m" // Yellow background
#define BG_BLUE "\x1b[44m"   // Blue background
#define BG_MAGENTA "\x1b[45m" // Magenta background
#define BG_CYAN "\x1b[46m"   // Cyan background
#define BG_WHITE "\x1b[47m"  // White background

void	print_varlist(t_list *varlist)
{
	int	j;

	j = 0;
	if (!varlist)
	{
		printf(BLUE"list %d: "CYAN"(%p)\n"RESET, j, varlist);
		return ;
	}
	while (varlist)
	{
		printf(BLUE"List %d: "CYAN"(%p)"BLUE", next "CYAN"(%p)\n"RESET, j++, varlist, varlist->next);
		if (!varlist->content)
			printf(GREEN"\t(NULL)"RESET"\n");
		else
		{
		printf(GREEN"\tname = "RESET"%s\n", ((t_var *)varlist->content)->name);
		printf(GREEN"\tcontent = "RESET"%s\n", ((t_var *)varlist->content)->content);
		if (((t_var *)varlist->content)->type == LOCAL_VAR)
			printf(GREEN"\ttype = "RESET"LOCAL_VAR\n");
		else if (((t_var *)varlist->content)->type == ENV_VAR)
			printf(GREEN"\ttype = "RESET"ENV_VAR\n");
		else
			printf(GREEN"\ttype = "RED"UNDEFINED"RESET"\n");
		}
		varlist = varlist->next;
	}
}
