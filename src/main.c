
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "portaudio.h"

#define SAMPLE_RATE 44100
#define NUM_CHANNELS 1
#define FRAMES_PER_BUFFER 1024

float randomFloat () {
  return rand() / (float) RAND_MAX;
}

int main(int argc, char **argv) {
  PaError err = Pa_Initialize();

  PaSampleFormat SAMPLE_FORMAT = paFloat32;

  PaStreamParameters inputParameters;
  PaStreamParameters outputParameters;

  PaStream * stream;

  float * sampleBlock = malloc(sizeof(float) * FRAMES_PER_BUFFER);

  if( err != paNoError ) {
    printf("Error initializing portaudio: %s", Pa_GetErrorText(err));
    return 0;
  }
  /* -- setup input and output -- */
  inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
  inputParameters.channelCount = NUM_CHANNELS;
  inputParameters.sampleFormat = SAMPLE_FORMAT;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency ;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
  outputParameters.channelCount = NUM_CHANNELS;
  outputParameters.sampleFormat = SAMPLE_FORMAT;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;


  err = Pa_OpenStream(
    &stream,
    &inputParameters,
    &outputParameters,
    SAMPLE_RATE,
    FRAMES_PER_BUFFER,
    paClipOff, /* we won't output out of range samples so don't bother clipping them */
    NULL, /* no callback, use blocking API */
    NULL
  );
  if( err != paNoError ) {
    printf("could not open stream %s", Pa_GetErrorText(err));
    return 0;
  }
  /* -- start stream -- */
  err = Pa_StartStream( stream );
  if( err != paNoError ) {
    printf("could not open stream %s", Pa_GetErrorText(err));
    return 0;
  }
  // printf("Wire on. Will run one minute.\n"); fflush(stdout);

  // int byteA = fgetc(stdin);
  float currentSample = 0;
  char * currentSamplePointer = (void *) &currentSample;

  bool doProcess = true;

  int stepTo = 16;
  int stepCounter = 0;
  bool stepDir = true;

  while (doProcess) {

    for (int j=0; j<FRAMES_PER_BUFFER; j++) {
      // scanf("%c", currentSamplePointer);
      // scanf("%c", currentSamplePointer+1);
      // scanf("%c", currentSamplePointer+2);
      // scanf("%c", currentSamplePointer+3);
      currentSamplePointer[0] = (char)fgetc(stdin);
      currentSamplePointer[1] = (char)fgetc(stdin);
      currentSamplePointer[2] = (char)fgetc(stdin);
      currentSamplePointer[3] = (char)fgetc(stdin);

      // if (stepCounter > stepTo) stepDir = true;
      // if (stepCounter < stepTo) stepDir = false;
      
      // if (stepDir) {
      //   currentSample = 1.0;
      // } else {
      //   currentSample = -1.0;
      // }
      sampleBlock[j] = currentSample;
    }
    err = Pa_WriteStream( stream, sampleBlock, FRAMES_PER_BUFFER );
    if( err != paNoError ) {
      printf("could write stream %s", Pa_GetErrorText(err));
      return 0;
    }
  }

  /* -- Now we stop the stream -- */
  err = Pa_StopStream( stream );
  if( err != paNoError ) {
    printf("could write stream %s", Pa_GetErrorText(err));
    return 0;
  }
  /* -- don't forget to cleanup! -- */
  err = Pa_CloseStream( stream );
  if( err != paNoError ) {
    printf("could write stream %s", Pa_GetErrorText(err));
    return 0;
  }
  Pa_Terminate();

  return 0;
}
