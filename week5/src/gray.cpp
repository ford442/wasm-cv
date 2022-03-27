#include <emscripten/emscripten.h>
#include <array>
#include <cmath>

#include "util.h"
#include "gray.h"

#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE unsigned char* threshold(unsigned char inputBuf[], BufferPool* pool, Wasmcv* project, int t) {
	unsigned char* outputBuf = pool->getNew();
	for (int i = 0; i < project->size; i += 4) {
		outputBuf[i] = 0;
		outputBuf[i + 1] = 0;
		outputBuf[i + 2] = 0;
		outputBuf[i + 3] = inputBuf[i + 3] < t ? 0 : 255;
	}
	return outputBuf;
}
EMSCRIPTEN_KEEPALIVE unsigned char* thresholdOCR(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project) {
	std::array<int, 361> o = project->offsets._19x19;
	const int minrange = 255 / 5;
	for (int i = 3; i < project->size; i += 4) {
		int hist[361];
		int t;
		for (int j = 0; j < 361; j += 1) {
			hist[j] = inputBuf[i + o[j]];
		}
		std::sort(hist, hist + 361);
		int range = hist[360] - hist[0];
		if (range > minrange) {
			t = (hist[0] + hist[360]) / 2;
		} else {
			t = (hist[360] - minrange) / 2;
		}
		outputBuf[i] = inputBuf[i] > t ? 0 : 255;
	}
	return outputBuf;
}
EMSCRIPTEN_KEEPALIVE unsigned char* otsu(unsigned char inputBuf[], BufferPool* pool, Wasmcv* project) {
	unsigned char hist[256] = {0};
	for (int i = 3; i < project->size; i += 4) {
		hist[inputBuf[i]] += 1;
	}
	float vMax = 0;
	int thresh = 0;
	for (int t = 0; t < 256; t += 1) {
		int bSum = 0, wbSum = 0;
		for (int i = 0; i < t; i += 1) {
			bSum += hist[i];
			wbSum += hist[i] * i;
		}
		float wB = float(bSum) / float(project->pixelArea);
		int fSum = 0, wfSum = 0;
		for (int i = t; i < 256; i += 1) {
			fSum += hist[i];
			wfSum += hist[i] * i;
		}
		float wF = float(fSum) / float(project->pixelArea);
		float bMean = float(wbSum) / float(bSum);
		float fMean = float(wfSum) / float(fSum);
		float m = (wB * bMean) + (wF * fMean);
		float v = (wB * std::pow(bMean - m, 2)) + (wF * std::pow(fMean - m, 2));
		if (v > vMax) {
			vMax = v;
			thresh = t;
		}
	}
	return threshold(inputBuf, pool, project, thresh);
}
EMSCRIPTEN_KEEPALIVE unsigned char* rank3x3(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, int r = 5) {
	std::array<int, 9> o = project->offsets._3x3;
	for (int i = 3; i < project->size; i += 4) {
		unsigned char hist[9] = {0};
		for (int j = 0; j < 9; j += 1) {
			hist[j] = inputBuf[i + o[j]];
		}
		std::sort(hist, hist + 9);
		outputBuf[i - 3] = 0;
		outputBuf[i - 2] = 0;
		outputBuf[i - 1] = 0;
		outputBuf[i] = hist[r];
	}
	return outputBuf;
}

#ifdef __cplusplus
}
#endif