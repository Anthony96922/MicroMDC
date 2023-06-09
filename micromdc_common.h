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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

/* wave/flac output */
#include <sndfile.h>

/* MDC library */
#include "mdc-encode-decode/mdc_decode.h"
#include "mdc-encode-decode/mdc_encode.h"
#include "mdc-encode-decode/mdc_types.h"

/* for mdc_encoder_close */
#include "micromdc_extra.h"

/* for C include format output */
#include "c_inc.h"

#define VERSION 	"1.2.2"
