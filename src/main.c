
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "portaudio.h"
#include "utils.c"

// #define SAMPLE_RATE 44100
// #define NUM_CHANNELS 1
// #define FRAMES_PER_BUFFER 1024

#include "./audiowrapper.c"

int main(int argc, char **argv) {
  struct audiowrapper_info info;
  
  struct StringSplitResult splitInfo;
  
  char * arg;
  for (int i=1; i<argc; i++) {
    arg = argv[i];

    string_split(arg, '=', &splitInfo);
    if (splitInfo.errorOccured) {
      printf("couldn't split argument: %s\n", splitInfo.errorMessage);
      break;
    }
    if (splitInfo.stringsCount < 2) {
      printf("ignoring argument %s as it didn't include an =\n", arg);
      continue;
    }

    for (int i=0; i<splitInfo.stringsCount; i++) {
      printf("item %s\n", splitInfo.strings[i]);
    }

    char * key = splitInfo.strings[0];
    char * value = splitInfo.strings[1];

    printf("key %s value %s\n", key, value);
  }

  // audiowrapper_start(info);

  return 0;
}
