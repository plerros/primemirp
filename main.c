// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (c) 2021 Pierro Zachareas
 */

#include <gmp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "configuration.h"
#include "thread_data.h"

void mpz_print(FILE *fp, mpz_t op)
{
	char *str =  mpz_get_str(NULL, 10, op);
	fprintf(fp, "%s\n", str);
	free(str);
}

void mpz_shift_concat(mpz_t op, unsigned long number)
{

	unsigned long multiplicand = 1;
	for (unsigned long i = number; i > 0; i /= 10)
		multiplicand *= 10;
	mpz_mul_ui(op, op, multiplicand);
	mpz_add_ui(op, op, number);
}

void *primemirp(void *args)
{
	struct thread_data_t *data = (struct thread_data_t *) args;

	mpz_t num;
	mpz_t root;
	mpz_t iterator;
	mpz_t mod;
	mpz_init(num); // init to 0
	mpz_init(root);
	mpz_init(iterator);
	mpz_init(mod);

	for (unsigned long n = 1; n <= data->max;) {
		mpz_set_ui(num, 0);
		for (unsigned long i = 1; i <= n; i++)
			mpz_shift_concat(num, i);
		for (unsigned long i = n-1; i > 0; i--)
			mpz_shift_concat(num, i);


		int tmp = mpz_probab_prime_p(num, 50);
		if (tmp == 1) { // possibly prime	
			// no need to check for divisibility by 2

			mpz_mod_ui(mod, num, 3);
			if (mpz_cmp_ui(mod, 0) == 0)
				continue;

			// no need to check for divisibility by 5

			mpz_root(root, num, 2);

			/*
			 * To skip multiples of 2 we do:
			 * (3) +2, +2, ....
			 *
			 * To skip multiples of 2,3 we do:
			 * (5) +2+4, +2+4 ....
			 *
			 * To skip multiples of 2,3,5 we do:
			 * (7) +4+2+4+2+4+6+2+6, +4+2+4+2+4+6+2+6 ....
			 */
			for(mpz_set_ui(iterator, 7); mpz_cmp(iterator, root) <= 0;){

				mpz_mod(mod, num, iterator);
				if (mpz_cmp_ui(mod, 0) == 0)
					break;
				mpz_add_ui(iterator, iterator, 4);
		
				mpz_mod(mod, num, iterator);
				if (mpz_cmp_ui(mod, 0) == 0)
					break;
				mpz_add_ui(iterator, iterator, 2);

				mpz_mod(mod, num, iterator);
				if (mpz_cmp_ui(mod, 0) == 0)
					break;
				mpz_add_ui(iterator, iterator, 4);
		
				mpz_mod(mod, num, iterator);
				if (mpz_cmp_ui(mod, 0) == 0)
					break;
				mpz_add_ui(iterator, iterator, 2);

				mpz_mod(mod, num, iterator);
				if (mpz_cmp_ui(mod, 0) == 0)
					break;
				mpz_add_ui(iterator, iterator, 4);
		
				mpz_mod(mod, num, iterator);
				if (mpz_cmp_ui(mod, 0) == 0)
					break;
				mpz_add_ui(iterator, iterator, 6);

				mpz_mod(mod, num, iterator);
				if (mpz_cmp_ui(mod, 0) == 0)
					break;
				mpz_add_ui(iterator, iterator, 2);
		
				mpz_mod(mod, num, iterator);
				if (mpz_cmp_ui(mod, 0) == 0)
					break;
				mpz_add_ui(iterator, iterator, 6);
			}
		}

		pthread_mutex_lock(data->mtx);
		//printf("n = %ld\n", n); // print progress
		switch (tmp) {
		case 1:
			if (mpz_cmp(iterator, root) > 0)
				mpz_print(stderr, num);
			else
				mpz_print(stderr, iterator);
			break;

		case 2:
			mpz_print(stderr, num);
			break;

		default:
			break;
		}

		/*
		 * Print out the global progress.
		 * That allows the user to recover from that point.
		 */

		for (unsigned long i = n; i == llhandle_get(data->progress); i++) {
			printf("n = %ld\n", llhandle_pop(data->progress));
		}

		n = data->n;
		if (data->n <= data->max) {
			llhandle_push(data->progress, n);
			data->n += 1;
		}

		pthread_mutex_unlock(data->mtx);
	}

	mpz_clear(mod);
	mpz_clear(iterator);
	mpz_clear(root);
	mpz_clear(num);

	return NULL;
}

int main()
{
	struct thread_data_t *data = thread_data_new(MIN, MAX);

	pthread_t threads[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; i++)
		pthread_create(&threads[i], NULL, primemirp, (void *) data);
	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], 0);

	thread_data_free(data);
	return 0;
}
