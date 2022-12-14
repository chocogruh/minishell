/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_exception_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthiesso <mthiesso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 19:59:38 by mthiesso          #+#    #+#             */
/*   Updated: 2022/10/06 15:52:44 by mthiesso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	check_qts_split(char *s, int i, char c)
{
	if (s[i] == c)
	{
		i++;
		while (s[i] != c)
		{
			if (!s[i])
				return (NO_RESULT);
			i++;
			if ((s[i] == c && s[i + 1] == ' ')
				|| (s[i] == c && s[i + 1] == '\0')
				|| (s[i] == c && s[i + 1] == '|'))
				return (i);
		}
		if (s[i] == c && s[i + 1] != ' ')
		{
			while (i < (int) ft_strlen(s) && s[i] != ' ')
				i++;
		}
	}
	return (i);
}
