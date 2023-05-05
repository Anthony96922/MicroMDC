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

int main() {
	SNDFILE *sf;
	SF_INFO sf_info;
	const int sample_rate = 16000;
	int samples = 16000;
	mdc_encoder_t *my_encoder;
	mdc_sample_t *my_buffer;
	int ret;

	my_encoder = mdc_encoder_new(sample_rate);
	my_buffer = malloc(samples * sizeof(mdc_sample_t));

	if (!my_encoder) {
		fprintf(stderr, "encoder init failed.\n");
		free(my_buffer);
		return 1;
	}

	ret = mdc_encoder_set_packet(my_encoder, 0x12, 0x34, 0x5678);

	if (ret) {
		fprintf(stderr, "set packet params failed.\n");
		goto fail;
	}

	ret = mdc_encoder_get_samples(my_encoder, my_buffer, samples);

	if (ret < 0) {
		fprintf(stderr, "get samples failed.\n");
		goto fail;
	}

	/* if we are at this point the encoder did not blow up */

	printf("done!\n");

	/* do somethiing with the audio */
	samples = ret;
	memset(&sf_info, 0, sizeof(SF_INFO));
	sf_info.samplerate = sample_rate;
	sf_info.channels = 1;
	sf_info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

	sf = sf_open("/tmp/test.wav", SFM_WRITE, &sf_info);
	sf_write_short(sf, my_buffer, samples);
	sf_close(sf);

	free(my_buffer);
	mdc_encoder_close(my_encoder);
	return 0;

fail:
	free(my_buffer);
	mdc_encoder_close(my_encoder);
	return 1;
}
