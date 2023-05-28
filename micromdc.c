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

/* all MDC stuff */
#include "micromdc_common.h"

static void show_help(char *name) {
	printf(
		"\x1b[1;36mMicro\x1b[1;35mMDC\x1b[0;0m, an MDC-1200 encoder\n"
		"Version %s\n"
		"\n"
		"  This can create audio files with custom MDC-1200 data. Intended\n"
		" for GMRS but can be used for amateur radio too. Audio is 8 bit unsigned\n"
		" and 8 kHz sample rate format. By default, it produces C array data.\n"
		" Licensed under GPLv3.\n"
		"\n"
		" This uses the MDC encoder library by Matthew Kaufman.\n"
		"\n"
#ifdef WAVEOUT
		" WAVE file output supported.\n"
		"\n"
		" Usage: %s DATAPARAMS [-p preamble] -o outfile [-h]\n"
#else
		" Usage: %s DATAPARAMS [-p preamble] [-h]\n"
#endif /* WAVEOUT */
		"\n"
		" Where DATAPARAMS can be:\n"
		"  opcode,arg,unitID\n"
		"          OR\n"
		"  opcode,arg,unitID,ext1,ext2,ext3,ext4\n"
		"\n"
		"  All data fields are parsed as hex\n"
		"\n"
		"  Example: 01,00,0123 or 01,00,0123,45,67,89,ab\n"
		"\n"
		" -p: extra preamble length in bytes\n"
#ifdef WAVEOUT
		" -o: output wav file\n"
#endif /* WAVEOUT */
		" -h: show this help\n"
		"\n", VERSION, name
	);
}

int main(int argc, char **argv) {
	int opt;
#ifdef WAVEOUT
	SNDFILE *sf;
	SF_INFO sf_info;
	char file[64 + 1];
	const char *short_opts = "p:o:h";
#else
	const char *short_opts = "p:h";
#endif /* WAVEOUT */
	const int sample_rate = 8000;
	int samples = sample_rate;
	mdc_encoder_t *my_encoder;
	mdc_sample_t *my_buffer;
	int ret;
	int extra_preamble = 0;
	int c_inc_out = 1;
	const int delay = 2400;

	/* data */
	char *data_str;
	unsigned char double_fmt;
	unsigned char opcode, arg;
	unsigned short unit_id;
	unsigned char ext[4];

#ifdef WAVEOUT
	memset(file, 0, 64);
#endif /* WAVEOUT */

	while ((opt = getopt(argc, argv, short_opts)) != -1) {
		switch (opt) {
			case 'p':
				extra_preamble = strtoul(optarg, NULL, 10);
				break;
#ifdef WAVEOUT
			case 'o':
				strncpy(file, optarg, 64);
				c_inc_out = 0;
				break;
#endif /* WAVEOUT */
			case 'h':
			default:
				show_help(argv[0]);
				return 1;
		}
	}

	if (optind < argc) {
		data_str = argv[optind];
		if (sscanf(data_str, "%02hhx,%02hhx,%04hx,"
			"%02hhx,%02hhx,%02hhx,%02hhx",
			&opcode, &arg, &unit_id,
			&ext[0], &ext[1], &ext[2], &ext[3]) == 7) {
			double_fmt = 1;
		} else if (sscanf(data_str, "%02hhx,%02hhx,%04hx",
			&opcode, &arg, &unit_id) == 3) {
			double_fmt = 0;
		} else {
			printf("parsing failed.\n");
			show_help(argv[0]);
			return 1;
		}
	} else {
		fprintf(stderr, "nothing to parse.\n");
		show_help(argv[0]);
		return 1;
	}

#ifdef WAVEOUT
	if (!file[0] && !c_inc_out) {
		fprintf(stderr, "no output file.\n");
		show_help(argv[0]);
		return 1;
	}
#endif /* WAVEOUT */

	my_encoder = mdc_encoder_new(sample_rate);
	my_buffer = malloc(samples * sizeof(mdc_sample_t));

	if (!my_encoder) {
		fprintf(stderr, "encoder init failed.\n");
		free(my_buffer);
		return 1;
	}

	if (extra_preamble) {
		printf("extra preamble: %d bytes\n", extra_preamble);
		ret = mdc_encoder_set_preamble(my_encoder, extra_preamble);

		if (ret) {
			fprintf(stderr, "set extra preamble failed.\n");
			goto fail;
		}
	}

	if (double_fmt) {
		printf("opt: 0x%02hhx, arg: 0x%02hhx, unitID: 0x%04hx\n"
			"ext1: 0x%02hhx, ext2: 0x%02hhx, "
			"ext3: 0x%02hhx, ext4: 0x%02hhx\n",
			opcode, arg, unit_id, ext[0], ext[1], ext[2], ext[3]);
		ret = mdc_encoder_set_double_packet(my_encoder,
			opcode, arg, unit_id, ext[0], ext[1], ext[2], ext[3]);
	} else {
		printf("opt: 0x%02hhx, arg: 0x%02hhx, unitID: 0x%04hx\n",
			opcode, arg, unit_id);
		ret = mdc_encoder_set_packet(my_encoder,
			opcode, arg, unit_id);
	}

	if (ret) {
		fprintf(stderr, "set packet params failed.\n");
		goto fail;
	}

	ret = mdc_encoder_get_samples(my_encoder, my_buffer + delay, samples);

	if (ret < 0) {
		fprintf(stderr, "get samples failed.\n");
		goto fail;
	}

	/* if we are at this point the encoder did not blow up */

#ifdef WAVEOUT
	if (c_inc_out)
		printf("writing C array data to stderr.\n");
	else
		printf("writing to file: %s.\n", file);
#endif /* WAVEOUT */

	/* do something with the audio */
	samples = ret;

	/* include delay in sample count */
	samples += delay;

	/* blank part of the preamble (it is too long) */
	memset(my_buffer + delay, 0, 120 * sizeof(short));

#ifdef WAVEOUT
	memset(&sf_info, 0, sizeof(SF_INFO));
	sf_info.samplerate = sample_rate;
	sf_info.channels = 1;
	sf_info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_U8;
	sf = sf_open(file, SFM_WRITE, &sf_info);
	sf_write_short(sf, my_buffer, samples);
	sf_close(sf);
#endif /* WAVEOUT */

	/* output C array data */
	if (c_inc_out)
		print_c_inc(my_buffer, samples);

	free(my_buffer);
	mdc_encoder_close(my_encoder);
	printf("done!\n");
	return 0;

fail:
	free(my_buffer);
	mdc_encoder_close(my_encoder);
	return 1;
}
