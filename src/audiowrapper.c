
#ifndef AUDIOWRAPPER_C
#define AUDIOWRAPPER_C

#include <stdio.h>
#include <stdbool.h>
#include "portaudio.h"

struct audiowrapper_info {
  int samplesPerBuffer;
  int sampleRate;
  int channels;
};

bool audiowrapper_start (struct audiowrapper_info info) {
  int FRAMES_PER_BUFFER = info.samplesPerBuffer;
  int NUM_CHANNELS = info.channels;
  int SAMPLE_RATE = info.sampleRate;

  PaError err = Pa_Initialize();

  PaSampleFormat SAMPLE_FORMAT = paFloat32;

  PaStreamParameters inputParameters;
  PaStreamParameters outputParameters;

  PaStream * stream;

  int sampleBlockByteSize = sizeof(float) * FRAMES_PER_BUFFER;

  float * sampleBlock = malloc(sampleBlockByteSize);

  if( err != paNoError ) {
    printf("Error initializing portaudio: %s", Pa_GetErrorText(err));
    return false;
  }

  /* -- setup input and output -- */
  inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
  inputParameters.channelCount = NUM_CHANNELS;
  inputParameters.sampleFormat = SAMPLE_FORMAT;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency ;
  inputParameters.hostApiSpecificStreamInfo = 0;

  outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
  outputParameters.channelCount = NUM_CHANNELS;
  outputParameters.sampleFormat = SAMPLE_FORMAT;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = 0;

  err = Pa_OpenStream(
    &stream,
    &inputParameters,
    &outputParameters,
    SAMPLE_RATE,
    FRAMES_PER_BUFFER,
    paClipOff, /* we won't output out of range samples so don't bother clipping them */
    0, /* no callback, use blocking API */
    0
  );
  
  if( err != paNoError ) {
    printf("could not open stream %s", Pa_GetErrorText(err));
    return false;
  }
  /* -- start stream -- */
  err = Pa_StartStream( stream );
  if( err != paNoError ) {
    printf("could not open stream %s", Pa_GetErrorText(err));
    return false;
  }

  float currentSample = 0;
  char * currentSamplePointer = (void *) &currentSample;

  bool doProcess = true;

  while (doProcess) {
    for (int j=0; j<FRAMES_PER_BUFFER; j++) {
      currentSamplePointer[0] = (char)fgetc(stdin);
      currentSamplePointer[1] = (char)fgetc(stdin);
      currentSamplePointer[2] = (char)fgetc(stdin);
      currentSamplePointer[3] = (char)fgetc(stdin);

      sampleBlock[j] = currentSample;
    }
    err = Pa_WriteStream( stream, sampleBlock, FRAMES_PER_BUFFER );
    if( err != paNoError ) {
      printf("could not write stream %s", Pa_GetErrorText(err));
      return false;
    }
  }

  /* -- Now we stop the stream -- */
  err = Pa_StopStream( stream );
  if( err != paNoError ) {
    printf("could not stop stream %s", Pa_GetErrorText(err));
    return false;
  }
  /* -- don't forget to cleanup! -- */
  err = Pa_CloseStream( stream );
  if( err != paNoError ) {
    printf("could not close stream %s", Pa_GetErrorText(err));
    return false;
  }
  Pa_Terminate();
  free(sampleBlock);
  return true;
}

#endif
