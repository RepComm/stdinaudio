
#ifndef UTILS_C
#define UTILS_C

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/**Concat a bunch of strings together
 * 
 * count - number of strings in strs
 * strs - an array of strings
 * joiner - a string to insert in between each string in strs
 */
char * string_concat (char** strs, int start, int count, char* joiner) {
  //total length of chars in the result (not including null terminator)
  int totalLength = 0;

  //joiner info
  bool useJoiner = joiner != 0;
  int joinerLength;
  if (useJoiner) joinerLength = strlen(joiner);

  //reusable loop string
  char * current;
  //reusable loop string's length
  int currentLength = 0;
  //lengths for each string in strs
  int strslengths[count];

  //loop through strs
  for (int i=start; i<count; i++) {
    current = strs[i];
    //calculate the length
    currentLength = strlen(current);

    //copy into list of lengths
    strslengths[i] = currentLength;
    //add to total
    totalLength += currentLength;

    //if a joiner is specified, add it for each item except the last one
    if (useJoiner && i != count-1) totalLength += joinerLength;
  }

  //the resulting string
  //make sure to add one for the null terminator
  char * result = malloc(totalLength + 1);
  //marker for the loop when copying chars
  int offset = 0;

  //loop through strs again
  for (int i=start; i<count; i++) {
    current = strs[i]; //current string
    currentLength = strslengths[i]; //current length

    //loop through the current string
    for (int j=0; j<currentLength; j++) {
      //copy the contents into result in the right spot
      //offset marks the result starting offset where current will be copied to
      //j marks the individual character of the current string
      result[offset + j] = current[j];
    }
    offset += currentLength;
    
    //copy in joiner data as well (except for after last string)
    if (useJoiner && i != count-1) {
      for (int j=0; j<joinerLength; j++) {
        result[offset + j] = joiner[j];
      }
      offset += joinerLength;
    }
  }
  //arrays start at 0, so using totalLength as the index here gives the last index
  //which we reserved for a null terminator
  //lets explicitly set it to null ffs
  result[totalLength] = 0;

  //we're done
  return result;
}

char * string_copy (char * src, int start, int width) {
  char * result = malloc(width + 1); //remember null terminator
  strncpy(result, src+start, width);
  result[width] = 0;
  return result;
}

char * string_copy_const (const char * src, int start, int width) {
  char * result = malloc(width + 1); //remember null terminator
  strncpy(result, src+start, width);
  result[width] = 0;
  return result;
}

bool char_in_string (char* haystack, char needle) {
  int ind = 0;
  char current = haystack[ind];
  while (current != 0) {
    if (current == needle) return true;
    ind ++;
    current = haystack[ind];
  }
  return false;
}

struct StringSplitResult {
  char * errorMessage;
  bool errorOccured;
  char ** strings;
  int stringsCount;
};

int string_char_occurs (char * src, char instance) {
  int len = strlen(src);
  int result = 0;
  for (int i=0; i<len; i++) {
    if (src[i] == instance) result ++;
  }
  return result;
}

void string_split (char * src, char delim, struct StringSplitResult * result) {
  result->errorMessage = 0;
  result->errorOccured = false;
  result->strings = 0;
  result->stringsCount = 0;

  int occurs = string_char_occurs(src, delim);

  //the number of resulting items after cutting source by delim
  //items may be null
  int itemCount = occurs + 1;
  
  char ** items = malloc(sizeof(char *) * itemCount);
  int itemIndex = 0;
  char * item = 0;

  int len = strlen(src);
  char current = 0;

  int lastDelimIndex = 0;
  int widthSinceLastDelim = 0;

  for (int i=0; i<len; i++) {
    current = src[i];

    if (current == delim) {
      widthSinceLastDelim = i - lastDelimIndex;
      //handles edge cases like delimiter at begin/end, directly after another delim
      if (widthSinceLastDelim < 1) {
        items[itemIndex] = 0; //a null string
        itemIndex++;
      } else {
        item = malloc(widthSinceLastDelim+1); //+1 for null terminator

        if (itemIndex == 0) {
          //copy string data for this split
          for (int j=0; j<widthSinceLastDelim; j++) {
            //items do not contain the delim char, so skip it with +1
            item[j] = src[lastDelimIndex + j];
          }
        } else {
          //copy string data for this split
          for (int j=0; j<widthSinceLastDelim-1; j++) {
            //items do not contain the delim char, so skip it with +1
            item[j] = src[lastDelimIndex + 1 + j];
          }
        }
        
        //explicit null terminator
        item[widthSinceLastDelim] = 0;

        items[itemIndex] = item;
        itemIndex++;
      }
      lastDelimIndex = i;
    }

  }

  int lastSize = len - lastDelimIndex;

  if (lastDelimIndex < len) {
    printf("last %i < len %i\n", lastDelimIndex, len);
    item = malloc(lastSize + 1);

    for (int i=0; i<lastSize; i++) {
      item[i] = src[lastDelimIndex+1 + i];
    }

    items[itemIndex] = item;
  }

  result->strings = items;
  result->stringsCount = itemCount;
}

#endif
