/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_bad.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedromar <pedromar@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 13:43:39 by pedromar          #+#    #+#             */
/*   Updated: 2022/11/25 13:44:48 by pedromar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "malloc_tem.h"

int main()
{
	int* iptr = malloc(10 * sizeof(int));
	char* cptr = calloc(256, sizeof(char));

	free(iptr);
	//free(cptr);

	return 0;
}
