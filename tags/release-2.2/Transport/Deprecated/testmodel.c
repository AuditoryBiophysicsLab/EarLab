/*
 * Whale Model control program
 */

#include <stdio.h>
#include <math.h>

#include <local/std.h>
#include <daffie/event.h>

#include "efievent.h"

#define MODEL 1

#include "tl.h"
#include "cl.h"

#include "model.h"

EXPORT void
define_model(void) {
	int   intype[MAX_ARGS], outtype[MAX_ARGS];

	/* Define static property of modules. i.e number and type of args */
	intype[0]  = EFI_NONE;
	outtype[0] = EFI_WAVEFORM_PRESSURE;
	define_module("Reader", 0, intype, 1, outtype);

/***************************************************************
	intype[0]  = EFI_WAVEFORM_PRESSURE;
	outtype[0] = EFI_WAVEFORM_PRESSURE;
	define_module("UpSample", 1, intype, 1, outtype);

	intype[0]  = EFI_WAVEFORM_PRESSURE;
	outtype[0] = EFI_WAVEFORM_PRESSURE;
	define_module("MidEar", 1, intype, 1, outtype);

	intype[0]  = EFI_WAVEFORM_PRESSURE;
	outtype[0] = EFI_WAVEFORM_DISPLACEMENT;
	define_module("Gamma", 1, intype, 1, outtype);

	intype[0]  = EFI_WAVEFORM_DISPLACEMENT;
	outtype[0] = EFI_WAVEFORM_VOLTAGE;
	define_module("Haircell", 1, intype, 1, outtype);

	intype[0]  = EFI_WAVEFORM_VOLTAGE;
	outtype[0] = EFI_WAVEFORM_VOLTAGE;
	define_module("TTS", 1, intype, 1, outtype);

***************************************************************/

	intype[0]  = EFI_WAVEFORM_VOLTAGE;
	outtype[0] = EFI_NONE;
	define_module("Writer", 1, intype, 0, outtype);
}

EXPORT void
init_model(void) {
	float samplerate;
	int   inputsamples, samples, channels;
	int   id;

	whale_init(&samplerate, &inputsamples, &samples, &channels);

	printf("Sample rate %.3f: input samples %d, samples %d, channels %d\n",
		samplerate, inputsamples, samples, channels);

	/* Set runtime sizes in module i/o descriptors */

	/* Reader */
	id = lookup_module("Reader");
	config_module_output(id, 1, inputsamples, 1, 1, 1);


  /***********************/
  /* START TEST CONFIG **/
  id = lookup_module("Writer");
  config_module_input(id, 1, inputsamples, 1, 1, 1);
  return;
  /* END TEST CONFIG    */
  /**********************/

	/* Up Sample */
	id = lookup_module("UpSample");
	config_module_input(id, 1, inputsamples, 1, 1, 1);
	config_module_output(id, 1, samples, 1, 1, 1);


	/* Midear */
	id = lookup_module("MidEar");
	config_module_input(id, 1, samples, 1, 1, 1);
	config_module_output(id, 1, samples, 1, 1, 1);

	/* Gamma */
	id = lookup_module("Gamma");
	config_module_input(id, 1, samples, 1, 1, 1);
	config_module_output(id, 1, samples, channels, 1, 1);

	/* Haircell */
	id = lookup_module("Haircell");
	config_module_input(id, 1, samples, channels, 1, 1);
	config_module_output(id, 1, samples, channels, 1, 1);

	/* Writer */
	id = lookup_module("Writer");
	config_module_input(id, 1, samples, channels, 1, 1);
}

EXPORT void
wire_model(void) {
	int   reader, upsamp, midear, gamma, haircell, tts, writer;


  /* START TEST CONFIG */
  reader = lookup_module("Reader");
  writer = lookup_module("Writer");
  wire_module(reader, 1, writer, 1);
  return;
  /* END TEST CONFIG */

	upsamp   = lookup_module("UpSample");
	midear   = lookup_module("MidEar");
	gamma    = lookup_module("Gamma");
	haircell = lookup_module("Haircell");
	tts      = lookup_module("TTS");
	writer   = lookup_module("Writer");

	wire_module(reader, 1, upsamp, 1);
	wire_module(upsamp, 1, midear, 1);
	wire_module(midear, 1, gamma,  1);
	wire_module(gamma,  1, writer, 1);

}

EXPORT void
set_parameters_model(void) {
	int   reader, upsamp, midear, gamma, haircell, tts, writer;

	reader   = lookup_module("Reader");
	set_parameters_module(reader, "sine_120dB.txt");

  WaitModuleReady();

  /* START TEST CONFIG */
  writer = lookup_module("Writer");
  set_parameters_module(writer, "writer.par");

  WaitModuleReady();

  /* END TEST CONFIG */
#ifdef REAL_CONFIG
	upsamp   = lookup_module("UpSample");
	midear   = lookup_module("MidEar");
	gamma    = lookup_module("Gamma");
	haircell = lookup_module("Haircell");
	tts      = lookup_module("TTS");
	writer   = lookup_module("Writer");

	set_parameters_module(upsamp,    "upamp.par");
	set_parameters_module(midear,    "midear.par");
	set_parameters_module(gamma,     "gamma.par");
	set_parameters_module(haircell,  "haircell.par");
	set_parameters_module(tts,       "tts.par");
	set_parameters_module(writer,    "whale.out");
#endif

}

/******************************************************************/
/*  whale.c                                                       */
/*  Test fixture file for Whale model                             */
/*                                                                */
/*  5 September 2001      Beta release                            */
/******************************************************************/


whale_init(float *sr, int *ninsamp, int *nsamp, int *nchan) {
  int SSSR, SamplesPerInputFrame, SamplesPerFrame, Channels;
  float SampleRate, InputSampleRate, FrameDuration;
  int k;

  // 10 milliseconds per frame, for now
  FrameDuration = 10.0e-3;   // seconds

  // Species Specific Sampling Rate     80 kHz (for balenoptera)
  SSSR = 80000;   // temporary

  InputSampleRate = 25000.0;  // temporary, get this from file

  // Compute the sampling rate for processing, based on the
  // input sampling rate and the SSSR

  if (InputSampleRate < SSSR) {
	k = (int)ceil((double)SSSR / InputSampleRate);
	SampleRate = k * InputSampleRate;
  } else {
	SampleRate = InputSampleRate;
  }

  SamplesPerInputFrame = (int)ceil(InputSampleRate * FrameDuration);
  SamplesPerFrame = (int)ceil(SampleRate * FrameDuration);

  *sr      = SampleRate;
  *ninsamp = SamplesPerInputFrame;
  *nsamp   = SamplesPerFrame;
  *nchan   = 64;

  return;

}
