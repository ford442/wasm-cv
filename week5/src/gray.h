#include <emscripten/emscripten.h>

#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE unsigned char* threshold(unsigned char inputBuf[], BufferPool* pool, Wasmcv* project, int t = 127);
EMSCRIPTEN_KEEPALIVE unsigned char* thresholdOCR(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project);
EMSCRIPTEN_KEEPALIVE unsigned char* otsu(unsigned char inputBuf[], BufferPool* pool, Wasmcv* project);

#ifdef __cplusplus
}
#endif