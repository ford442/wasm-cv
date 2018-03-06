#ifdef __cplusplus
extern "C" {
#endif

// Function prototypes
int sumNeighbors(unsigned char inputBuf[], int p0, int w);
unsigned char* conv3(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, int k[], int norm);
EMSCRIPTEN_KEEPALIVE unsigned char* dilate(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project);
EMSCRIPTEN_KEEPALIVE unsigned char* erode(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project);
unsigned char* kDilate3x3(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, BinaryStructuringElement3x3 k);
unsigned char* kDilate5x5(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, BinaryStructuringElement5x5 k);
unsigned char* kErode3x3(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, BinaryStructuringElement3x3 k);
unsigned char* kErode5x5(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, BinaryStructuringElement5x5 k);
EMSCRIPTEN_KEEPALIVE unsigned char* findEdges(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project);
EMSCRIPTEN_KEEPALIVE unsigned char* deSaltPepper(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project); 
EMSCRIPTEN_KEEPALIVE unsigned char* boxBlur(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project);
EMSCRIPTEN_KEEPALIVE unsigned char* gaussianApprox(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project);
EMSCRIPTEN_KEEPALIVE unsigned char* close3x3(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, BinaryStructuringElement3x3 k);
EMSCRIPTEN_KEEPALIVE unsigned char* close5x5(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, BinaryStructuringElement5x5 k);
EMSCRIPTEN_KEEPALIVE unsigned char* open3x3(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, BinaryStructuringElement3x3 k);
EMSCRIPTEN_KEEPALIVE unsigned char* open5x5(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, BinaryStructuringElement5x5 k);
EMSCRIPTEN_KEEPALIVE unsigned char* sub(unsigned char inputBufA[], unsigned char inputBufB[], unsigned char outputBuf[], Wasmcv* project);
EMSCRIPTEN_KEEPALIVE unsigned char* topHat3x3White(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, BinaryStructuringElement3x3 k);
EMSCRIPTEN_KEEPALIVE unsigned char* topHat5x5White(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, BinaryStructuringElement5x5 k);
EMSCRIPTEN_KEEPALIVE unsigned char* topHat3x3Black(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, BinaryStructuringElement3x3 k);
EMSCRIPTEN_KEEPALIVE unsigned char* topHat5x5Black(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, BinaryStructuringElement5x5 k);
EMSCRIPTEN_KEEPALIVE bool findCornerExternal(unsigned char inputBuf[], Wasmcv* project, int loc);
EMSCRIPTEN_KEEPALIVE bool findCornerInternal(unsigned char inputBuf[], Wasmcv* project, int loc);
EMSCRIPTEN_KEEPALIVE uint32_t* findAllCorners(unsigned char inputBuf[], Wasmcv* project);
EMSCRIPTEN_KEEPALIVE int countForegroundObjects(unsigned char inputBuf[], Wasmcv* project);
EMSCRIPTEN_KEEPALIVE int16_t* getConnectedComponentsRecursive(unsigned char inputBuf[], Wasmcv* project);
void searchConnected(Wasmcv* project, std::vector<int16_t>& map, int label, int offset);
EMSCRIPTEN_KEEPALIVE int16_t* getConnectedComponents(unsigned char inputBuf[], Wasmcv* project);
int findParent(int label, int unionFindStructure[]);
int* constructUnion(int labelX, int labelY, int unionFindStructure[]);
EMSCRIPTEN_KEEPALIVE unsigned char* segmentationVisualizer(int16_t* map, unsigned char outputBuf[], Wasmcv* project);

#ifdef __cplusplus
}
#endif