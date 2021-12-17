// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (c) 2021 Pierro Zachareas
 */

#include <assert.h>
#include <stdlib.h>
#include "llist.h"

struct llnode
{
	unsigned long number;
	struct llnode *next;
};

static struct llnode *llnode_new(unsigned long number)
{
	struct llnode *new = malloc(sizeof(struct llnode));
	if (new == NULL)
		abort();

	new->number = number;
	new->next = NULL;
	return new;
}

struct llhandle
{
	struct llnode *first;
	struct llnode *last;
};

struct llhandle *llhandle_new()
{
	struct llhandle *new = malloc(sizeof(struct llhandle));
	if (new == NULL)
		abort();
	
	new->first = NULL;
	new->last = NULL;
	return new;
}

static void llhandle_error_check(struct llhandle *ptr)
{
	assert(ptr != NULL);

	if ((ptr->first == NULL) ^ (ptr->last == NULL))
		assert("llhandle pointers are broken\n");

	if ((ptr->last != NULL) && (ptr->last->next != NULL))
		assert("llhandle last pointer is broken\n");
}

void llhandle_free(struct llhandle *ptr)
{
	if (ptr == NULL)
		return;

	llhandle_error_check(ptr);

	while (ptr->first != NULL) {
		struct llnode *tmp = ptr->first->next;
		free(ptr->first);
		ptr->first = tmp;
	}
	
	free(ptr);
}

void llhandle_push(struct llhandle *ptr, unsigned long number)
{
	llhandle_error_check(ptr);

	struct llnode *new = llnode_new(number);
	if (ptr->first == NULL)
		ptr->first = new;

	if (ptr->last != NULL)
		ptr->last->next = new;

	ptr->last = new;
}

unsigned long llhandle_get(struct llhandle *ptr)
{
	llhandle_error_check(ptr);
	if (ptr->first == NULL)
		return 0;

	return(ptr->first->number);
}

unsigned long llhandle_pop(struct llhandle *ptr)
{
	llhandle_error_check(ptr);
	if (ptr->first == NULL)
		return 0;

	struct llnode *tmp = ptr->first;

	ptr->first = ptr->first->next;
	if (ptr->first == NULL)
		ptr->last = NULL;

	unsigned long ret = tmp->number;
	free(tmp);

	return(ret);
}
