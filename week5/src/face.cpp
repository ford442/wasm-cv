#include <emscripten/emscripten.h>
#include <vector>
#include <iostream>

#include "util.h"
#include "face.h"

#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE std::vector<int> makeIntegralImage(unsigned char inputBuf[], Wasmcv* project) {
	std::vector<int> integral(project->size);
	std::vector<int> sumTable(project->size);
	for (int i=3; i < project->size; i += 4) {
		auto vec2=offsetToVec2(i - 3, project);
		int x=vec2[0];
		int y=vec2[1];
		int yP=y - 1 < 0 ? 0 : sumTable[i - project->w * 4];
		int xP=x - 1 < 0 ? 0 : integral[i - 4];
		sumTable[i]=yP + inputBuf[i];
		integral[i]=xP + sumTable[i];
	}
	return integral;
}

#ifdef __cplusplus
}
#endif