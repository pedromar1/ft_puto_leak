/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puto_leak.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedromar <pedromar@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 13:42:55 by pedromar          #+#    #+#             */
/*   Updated: 2022/11/25 13:51:10 by pedromar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* REFERENCE:
 *	https://github.com/TheAlgorithms/C.git
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "malloc_tem.h"

#undef malloc
#undef calloc
#undef free

typedef struct s_mem_info
{
	void* ptr;
	const char* fileName;
	const char* functionName;
	size_t bytes;
	int line;
	struct s_mem_info* next;
	struct s_mem_info* previous;
} t_mem_info;

t_mem_info* memoryInformation = NULL;

int atexitCalled = 0;

t_mem_info* addMemInfo(t_mem_info* memoryInfo, void* ptrToReturn, size_t bytes, int line, const char* filename, const char* functionName)
{
	t_mem_info* newMemInfo = (t_mem_info*)malloc(sizeof(t_mem_info));
	if (!newMemInfo)
		return NULL;
	newMemInfo->ptr = ptrToReturn;
	newMemInfo->bytes = bytes;
	newMemInfo->line = line;
	newMemInfo->fileName = filename;
	newMemInfo->functionName = functionName;
	newMemInfo->next = memoryInfo;
	newMemInfo->previous = NULL;
	if (memoryInformation)
		memoryInformation->previous = newMemInfo;
	return newMemInfo;
}

int inList(const char* filename, int line)
{
	t_mem_info* tmp = memoryInformation;
	int counter = 0;
	int len =(int) strlen(filename);

	while (tmp)
	{
		if (len == (int) strlen(tmp->fileName))
		{
			if (!memcmp(filename, tmp->fileName, len) && tmp->line == line)
			{
				return counter;
			}
		}
		tmp = tmp->next;
		counter++;
	}
	return -1;
}

void editInfo(int elemPos, size_t bytes)
{
	int counter = 0;
	t_mem_info* tmp = memoryInformation;

	while (counter != elemPos)
	{
		tmp = tmp->next;
		counter++;
	}
	tmp->bytes += bytes;
}

void* malloc_tem(size_t bytes, int line, const char* filename, const char* functionName)
{
	void* ptrToReturn = malloc(bytes);
	int pos = 0;
	if (!ptrToReturn)
		return NULL;
	if (!atexitCalled)
	{
		atexit(printLeaks);
		atexitCalled = 1;
	}
	pos = inList(filename, line);
	if (pos == -1)
	{
		memoryInformation = addMemInfo(memoryInformation, ptrToReturn, bytes, line, filename, functionName);
		if (!memoryInformation)
		{
			free(ptrToReturn);
			return NULL;
		}
	}
	else
		editInfo(pos, bytes);
	return ptrToReturn;
}

void* calloc_tem(size_t elementCount, size_t elementSize, int line, const char* filename, const char* functionName)
{
	void* ptrToReturn = calloc(elementCount, elementSize);
	if (!ptrToReturn)
		return NULL;
	if (!atexitCalled)
	{
		atexit(printLeaks);
		atexitCalled = 1;
	}
	memoryInformation = addMemInfo(memoryInformation, ptrToReturn, elementCount * elementSize, line, filename, functionName);
	if (!memoryInformation)
	{
		free(ptrToReturn);
		return NULL;
	}
	return ptrToReturn;
}

void free_tem(void* ptrToFree)
{
	t_mem_info* tmp = memoryInformation;
	t_mem_info* toFree = NULL;
	t_mem_info* previous = NULL;

	if (tmp->ptr == ptrToFree)
	{
		toFree = tmp;
		memoryInformation = tmp->next;
		free(toFree->ptr);
		free(toFree);
		if (memoryInformation)
		{
			memoryInformation->previous = NULL;
		}
		return;
	}
	while (tmp)
	{
		if (tmp->ptr == ptrToFree)
		{
			toFree = tmp;
			tmp = tmp->next;
			previous = toFree->previous;
			if (previous)
				previous->next = tmp;
			if (tmp)
				tmp->previous = previous;
			free(toFree->ptr);
			if (toFree == memoryInformation)
				memoryInformation = NULL;
			free(toFree);
			return ;
		}
		tmp = tmp->next;
	}
}

void printLeaks()
{
	t_mem_info* tmp = memoryInformation;
	t_mem_info* previous = NULL;
	size_t sum = 0;
	int nbBlocks = 0;

	if (tmp)
		printf("⠀⠀⠀⠀⠀(\\__/)⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀(•ㅅ•)⠀⠀No boy... He has memory leaks...\n⠀＿ノ⠀ヽ⠀ノ⠀＼＿⠀⠀\n/ ⠀️⠀Y⠀⌒Ｙ⌒⠀Ｙ⠀️⠀️ヽ\n(⠀️⠀️⠀️(三ヽ人⠀⠀/⠀⠀⠀|\n|⠀️⠀️⠀️ﾉ⠀¯¯\\⠀￣￣ヽノ\nヽ＿＿＿⠀⠀＞､＿_／\n⠀⠀⠀｜⠀(⠀王⠀)〈⠀⠀ (\\__/) \n⠀⠀⠀/⠀⠀ﾐ`——彡⠀\\   (•ㅅ•)  Moulinette, is he a good programmer?\n");
	else
		printf("|￣￣￣￣￣￣|\n|    GOOD!!  | \n|＿＿＿＿＿__| \n(\\__/) || \n(•ㅅ•) || \n/ 　 づ\n");
	while (tmp)
	{
		previous = tmp;
		printf("\n%ld bytes perdidos\n", tmp->bytes);
		printf("El puto leak esta en:	0x%p  funcion %s\t%s: linea %d\n", tmp->ptr, tmp->functionName, tmp->fileName, tmp->line);
		sum += tmp->bytes;
		tmp = tmp->next;
		free(previous);
		nbBlocks++;
	}
	printf("RESUMEN :\n%ld bytes perdidos en %d blocks\n", sum, nbBlocks);
}



