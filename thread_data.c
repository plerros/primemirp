// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (c) 2021 Pierro Zachareas
 */

#include <stdlib.h>
#include "thread_data.h"

struct thread_data_t *thread_data_new(unsigned long n, unsigned long max)
{
	struct thread_data_t *new = malloc(sizeof(struct thread_data_t));
	if (new == NULL)
		abort();

	new->n = n;
	new->max = max;

	new->mtx = malloc(sizeof(pthread_mutex_t));
	if (new->mtx == NULL)
		abort();
	pthread_mutex_init(new->mtx, NULL);

	new->progress = llhandle_new();
	return new;
}

void thread_data_free(struct thread_data_t *ptr)
{
	pthread_mutex_destroy(ptr->mtx);
	free(ptr->mtx);
	llhandle_free(ptr->progress);
	free(ptr);
}
