
gcc ./src/main.c ./src/libportaudio.a \
  -lrt \
  -lm \
  -lasound \
  -pthread \
  -g \
  -Wl,-rpath='${ORIGIN}' \
  -Wall \
  -o stdinaudio

chmod +x ./stdinaudio

echo done compiling