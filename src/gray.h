#ifdef __cplusplus
extern "C" {
#endif

// Function prototypes
EMSCRIPTEN_KEEPALIVE unsigned char* binarize(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, int t);
EMSCRIPTEN_KEEPALIVE unsigned char* median3x3(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project);
EMSCRIPTEN_KEEPALIVE unsigned char* rank3x3(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, int r);
EMSCRIPTEN_KEEPALIVE unsigned char* tmf(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project);
EMSCRIPTEN_KEEPALIVE unsigned char* binarizeOCR(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project);

#ifdef __cplusplus
}
#endif