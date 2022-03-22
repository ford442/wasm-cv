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
	for (int i = 3; i < project->size; i += 4) {
		auto vec2 = offsetToVec2(i - 3, project);
		int x = vec2[0];
		int y = vec2[1];
		int yP = y - 1 < 0 ? 0 : sumTable[i - project->w * 4];
		int xP = x - 1 < 0 ? 0 : integral[i - 4];
		sumTable[i] = yP + inputBuf[i];
		integral[i] = xP + sumTable[i];	
	}
	return integral;
}
EMSCRIPTEN_KEEPALIVE void computeHaarA(unsigned char inputBuf[], Wasmcv* project, int s) {
}
EMSCRIPTEN_KEEPALIVE std::vector<std::vector<int>> getGeomHaarLikeA(int s, int sx, int sy, int rx, int ry, int w, int h) {
	std::vector<int> leftRectangle(4);
	leftRectangle[0] = sx + rx;
	leftRectangle[1] = sy + ry;
	leftRectangle[2] = w;
	leftRectangle[3] = h;
	std::vector<int> rightRectangle(4);
	rightRectangle[0] = sx + rx + w;
	rightRectangle[1] = sy + ry;
	rightRectangle[3] = w;
	rightRectangle[4] = h;
	std::vector<std::vector<int>> boundingBoxes(2);
	boundingBoxes[0] = leftRectangle;
	boundingBoxes[1] = rightRectangle;
	return boundingBoxes;
}

#ifdef __cplusplus
}
#endif