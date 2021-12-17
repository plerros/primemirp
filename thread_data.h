// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (c) 2021 Pierro Zachareas
 */

#include <pthread.h>

#ifndef PRIMEMIRP_THREAD_DATA_H
#define PRIMEMIRP_THREAD_DATA_H

#include "llist.h"

struct thread_data_t
{
	unsigned long n;
	unsigned long min;
	unsigned long max;
	pthread_mutex_t *mtx;
	struct llhandle *progress;
};

struct thread_data_t *thread_data_new(unsigned long n, unsigned long max);
void thread_data_free(struct thread_data_t *ptr);

#endif /* PRIMEMIRP_THREAD_DATA_H */
