/**
 * Timecode Library: Easy-to-use vinyl timecode access
 * Copyright (c) 2018 Thomas Perl <m@thp.io>
 *
 * This library uses the "timecoder" from xwax, which is licensed
 * under the GNU GPL and parts of the PortAudio library.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 **/

#include "portaudio.h"

extern "C" {
#include "timecoder.h"
}

#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <string.h>

#include <Windows.h>

#define SRATE 44100

static int timecodeCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	struct timecoder *tc = (struct timecoder *)userData;
	const int16_t *buf = (const int16_t *)inputBuffer;
	timecoder_submit(tc, buf, framesPerBuffer);
	return 0;
}

static struct timecoder tc;
static PaStream *stream;
static bool g_inited = false;

extern "C" __declspec(dllexport) int timecode_library_init(const char *timecoder_name) {
	if (g_inited) {
		return 5;
	}

	PaError err = Pa_Initialize();

	if (err != paNoError) {
		printf("Cannot init PortAudio!\n");
		return 1;
	}
	else {
		printf("Using %s\n", Pa_GetVersionText());
		int input_device = Pa_GetDefaultInputDevice();
		for (int i = 0; i < Pa_GetDeviceCount(); i++) {
			if (i == input_device) {
				const PaDeviceInfo *devinfo = Pa_GetDeviceInfo(i);
				printf("Recording device: %s\n", devinfo->name);
			}
		}
	}

	timecode_def *def = timecoder_find_definition(timecoder_name);
	if (def == nullptr) {
		printf("Cannot find timecode def for '%s'\n", timecoder_name);
		return 2;
	}
	printf("Timecode definition: %s (%s)\n", def->name, def->desc);

	timecoder_init(&tc, def, 1.f, SRATE, false);

	err = Pa_OpenDefaultStream(&stream, 2, 0, paInt16, SRATE,
		paFramesPerBufferUnspecified, timecodeCallback,
		&tc);

	if (err != paNoError) {
		printf("Could not open device for recording: %s\n",
			Pa_GetErrorText(err));
		return 3;
	}

	err = Pa_StartStream(stream);

	if (err != paNoError) {
		printf("Could not start stream: %s\n",
			Pa_GetErrorText(err));
		return 4;
	}

	g_inited = true;

	return 0;
}

extern "C" __declspec(dllexport) float timecode_library_get_speed() {
	return timecoder_get_pitch(&tc);
}

extern "C" __declspec(dllexport) void timecode_library_deinit() {
	if (g_inited) {
		Pa_StopStream(stream);
		Pa_CloseStream(stream);

		timecoder_clear(&tc);
		g_inited = false;
	}
}