/*
 * (C) 2009 by Pablo Neira Ayuso <pablo@netfilter.org>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "conntrackd.h"
#include "process.h"

static LIST_HEAD(process_list);

int fork_process_new(void (*cb)(void *data), void *data)
{
	struct child_process *c;

	c = calloc(sizeof(struct child_process), 1);
	if (c == NULL)
		return -1;

	c->cb = cb;
	c->data = data;

	list_add(&c->head, &process_list);

	return fork();
}

int fork_process_delete(int pid)
{
	struct child_process *this, *tmp;

	list_for_each_entry_safe(this, tmp, &process_list, head) {
		if (this->pid == pid) {
			list_del(&this->head);
			if (this->cb) {
				this->cb(this->data);
			}
			free(this);
			return 1;
		}
	}
	return 0;
}