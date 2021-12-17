// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (c) 2021 Pierro Zachareas
 */

#ifndef PRIMEMIRP_LLIST_H
#define PRIMEMIRP_LLIST_H

struct llhandle;
struct llhandle *llhandle_new(void);
void llhandle_free(struct llhandle *ptr);
void llhandle_push(struct llhandle *ptr, unsigned long number);
unsigned long llhandle_get(struct llhandle *ptr);
unsigned long llhandle_pop(struct llhandle *ptr);

#endif /* PRIMEMIRP_LLIST_H */
