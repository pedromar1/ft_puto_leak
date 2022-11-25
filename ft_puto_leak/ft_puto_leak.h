/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puto_leak.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedromar <pedromar@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 13:43:12 by pedromar          #+#    #+#             */
/*   Updated: 2022/11/25 13:51:35 by pedromar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_tem_H
#define MALLOC_tem_H

#define malloc(bytes) malloc_tem(bytes, __LINE__, __FILE__, __FUNCTION__)
#define calloc(elemCount, elemSize) calloc_tem(elemCount, elemSize, __LINE__, __FILE__, __FUNCTION__)
#define free(ptr) free_tem(ptr)

void* malloc_tem(size_t bytes, int line, const char* filename, const char* functionName);
void* calloc_tem(size_t elementCount, size_t elementSize, int line, const char* filename, const char* functionName);
void free_tem(void* ptrToFree);
void printLeaks(void);

#endif
