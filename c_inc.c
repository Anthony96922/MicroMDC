/*
 * MicroMDC
 * Copyright (C) 2023 Anthony96922
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>

void print_c_inc(short *data, int len) {
	int newline_counter = 0;
	const int newline_limit = 12;

	fprintf(stderr, "\n\nunsigned char audio[%d] = {\n ", len);

	for (int i = 0; i < len; i++) {
		fprintf(stderr, " 0x%02x,", (data[i] + 32767) >> 8);
		if (++newline_counter == newline_limit) {
			fprintf(stderr, "\n ");
			newline_counter = 0;
		}
	}

	fprintf(stderr, "\n};\n\n");
}
