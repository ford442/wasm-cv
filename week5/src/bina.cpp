#include <emscripten/emscripten.h>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>

#include "util.h"
#include "bina.h"

#ifdef __cplusplus
extern "C" {
#endif

int sumNeighbors(unsigned char inputBuf[], int p0, int w) {
	return inputBuf[p0 - 4 - w * 4] + inputBuf[p0 - w * 4] + inputBuf[p0 + 4 - w * 4] + inputBuf[p0 - 4] + inputBuf[p0 + 4] + 
		inputBuf[p0 - 4 + w * 4] + inputBuf[p0 + w * 4] + inputBuf[p0 + 4 + w * 4];
}
unsigned char* conv3(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project, int k[], int norm) {
	for (int i = 3; i < project->size; i += 4) {
		outputBuf[i - 3] = 0;
		outputBuf[i - 2] = 0;
		outputBuf[i - 1] = 0;
		outputBuf[i] = ((inputBuf[i - 4 - project->w * 4] * k[0]) + (inputBuf[i - project->w * 4] * k[1]) + (inputBuf[i + 4 - project->w * 4] * k[2]) + 
			(inputBuf[i - 4] * k[3]) + (inputBuf[i] * k[4]) + (inputBuf[i + 4] * k[5]) + (inputBuf[i- 4 + project->w * 4] * k[6]) + 
				(inputBuf[i + project->w * 4] * k[7]) + (inputBuf[i + 4 + project->w * 4] * k[8])) / norm;
	}
	return outputBuf;
}
EMSCRIPTEN_KEEPALIVE unsigned char* deSaltPepper(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project) {
	for (int i = 3; i < project->size; i += 4) {
		int sigma = sumNeighbors(inputBuf, i, project->w);
		if (sigma == 0) {
			outputBuf[i] = 0;
		} else if (sigma == 2040) {
			outputBuf[i] = 255;
		} else {
			outputBuf[i] = inputBuf[i];
		}
	}
	return outputBuf;
}
EMSCRIPTEN_KEEPALIVE unsigned char* boxBlur(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project) {
	int k[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	return conv3(inputBuf, outputBuf, project, k, 9);
}
EMSCRIPTEN_KEEPALIVE unsigned char* gaussianApprox(unsigned char inputBuf[], unsigned char outputBuf[], Wasmcv* project) {
	int k[9] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
	return conv3(inputBuf, outputBuf, project, k, 16);
}
EMSCRIPTEN_KEEPALIVE unsigned char* sub(unsigned char inputBufA[], unsigned char inputBufB[], BufferPool* pool, Wasmcv* project) {
	unsigned char* outputBuf = pool->getNew();
	for (int i = 3; i < project->size; i += 4) {
		outputBuf[i - 3] = 0;
		outputBuf[i - 2] = 0;
		outputBuf[i - 1] = 0;
		outputBuf[i] = inputBufA[i] - inputBufB[i];	
	}
	return outputBuf;
}
EMSCRIPTEN_KEEPALIVE bool findCornerExternal(unsigned char inputBuf[], Wasmcv* project, int loc) {
	std::array<int, 4> o = project->offsets._2x2;
	int hits = 0;
	for (int i = 0; i < 4; i += 1) {
		if (inputBuf[loc + o[i]] == project->se._2x2ecul.kernel[i]) {
			hits += 1;
		}
	}
	if (hits == 4) {
		return true;
	}
	hits = 0;
	for (int i = 0; i < 4; i += 1) {
		if (inputBuf[loc + o[i]] == project->se._2x2ecur.kernel[i]) {
			hits += 1;
		}
	}
	if (hits == 4) {
		return true;
	}
	hits = 0;
	for (int i = 0; i < 4; i += 1) {
		if (inputBuf[loc + o[i]] == project->se._2x2ecll.kernel[i]) {
			hits += 1;
		}
	}
	if (hits == 4) {
		return true;
	}
	hits = 0;
	for (int i = 0; i < 4; i += 1) {
		if (inputBuf[loc + o[i]] == project->se._2x2eclr.kernel[i]) {
			hits += 1;
		}
	}
	if (hits == 4) {
		return true;
	}
	return false;
}
EMSCRIPTEN_KEEPALIVE bool findCornerInternal(unsigned char inputBuf[], Wasmcv* project, int loc) {
	std::array<int, 4> o = project->offsets._2x2;
	int hits = 0;
	for (int i = 0; i < 4; i += 1) {
		if (inputBuf[loc + o[i]] == project->se._2x2icul.kernel[i]) {
			hits += 1;
		}
	}
	if (hits == 4) {
		return true;
	}
	hits = 0;
	for (int i = 0; i < 4; i += 1) {
		if (inputBuf[loc + o[i]] == project->se._2x2icur.kernel[i]) {
			hits += 1;
		}
	}
	if (hits == 4) {
		return true;
	}
	hits = 0;
	for (int i = 0; i < 4; i += 1) {
		if (inputBuf[loc + o[i]] == project->se._2x2icll.kernel[i]) {
			hits += 1;
		}
	}
	if (hits == 4) {
		return true;
	}
	hits = 0;
	for (int i = 0; i < 4; i += 1) {
		if (inputBuf[loc + o[i]] == project->se._2x2iclr.kernel[i]) {
			hits += 1;
		}
	}
	if (hits == 4) {
		return true;
	}
	return false;
}
EMSCRIPTEN_KEEPALIVE std::vector<int> findAllCorners(unsigned char inputBuf[], Wasmcv* project) {
	std::vector<int> corners;
	int len = 0;
	for (int i = 3; i < project->size; i += 4) {
		if (findCornerInternal(inputBuf, project, i) || findCornerExternal(inputBuf, project, i)) {
			corners.push_back(i - 3);
		} 
	}
	return corners;
}
EMSCRIPTEN_KEEPALIVE int countForegroundObjects(unsigned char inputBuf[], Wasmcv* project) {
	int e = 0;
	int i = 0;
	for (int i = 3; i < project->size; i += 4) {
		if (findCornerInternal(inputBuf, project, i)) e += 1;
		if (findCornerExternal(inputBuf, project, i)) i += 1;
	}
	return (e - i) / 4;
}
EMSCRIPTEN_KEEPALIVE int16_t* getConnectedComponentsRecursive(unsigned char inputBuf[], Wasmcv* project) {
	std::vector<int16_t> map(project->size, 0);
	for (int i = 3; i < project->size; i += 4) {
		map[i] = inputBuf[i] == 255 ? -1 : 0;
	}
	int label = 0;
	for (int i = 3; i < project->size; i += 4) {
		if (map[i] == -1) {
			searchConnected(project, map, label, i);
		}
	}
	return map.data();
}
void searchConnected(Wasmcv* project, std::vector<int16_t>& map, int label, int offset) {
	map[offset] = label;
	for (int i = 0; i < 5; i += 1) {
		if (offset + project->offsets._4n[i] >= 0 && offset + project->offsets._4n[i] < project->size) {
			if (map[offset + project->offsets._4n[i]] == -1) {
				searchConnected(project, map, label, offset + project->offsets._4n[i]);
			}
		}
	}
}
EMSCRIPTEN_KEEPALIVE std::vector<int> getConnectedComponents(unsigned char inputBuf[], Wasmcv* project) {
	int label = 1;
	int disjointSet[1200] = {0};
	std::vector<int> map(project->size, 0);
	for (int i = 3; i < project->size; i += 4) {
		if (inputBuf[i] == 255) {
			int16_t priorNeighborLabels[2] = {0};
			if (isInImageBounds(project, i + project->offsets._4n[0])) {
				priorNeighborLabels[0] = map[i + project->offsets._4n[0]]; // should i actually find the parent node here?
			}
			if (isInImageBounds(project, i + project->offsets._4n[1]) && ((i - 3) / 4) % project->w != 0) {
				priorNeighborLabels[1] = map[i + project->offsets._4n[1]]; // should i actually find the parent node here?
			}
			int m;
			if (priorNeighborLabels[0] == 0 && priorNeighborLabels[1] == 0) {
				m = label;
				label = label == 1199 ? 0 : label += 1;
			} else {
				if (priorNeighborLabels[1] > priorNeighborLabels[0]) std::swap(priorNeighborLabels[0], priorNeighborLabels[1]);
				m = priorNeighborLabels[0] == 0 ? priorNeighborLabels[1] : priorNeighborLabels[0];
			}
			map[i] = m;
			for (int j = 0; j < 2; j += 1) {
				if (priorNeighborLabels[j] != 0 && priorNeighborLabels[j] != m) {
					constructUnion(m, priorNeighborLabels[j], disjointSet);
				}
			}
		}
	}
	for (int i = 3; i < project->size; i += 4) {
		if (inputBuf[i] == 255) {
			map[i] = findParent(map[i], disjointSet);
		}
	}
	return map;
}
int findParent(int label, int disjointSet[]) {
	while (disjointSet[label] != 0) {	
		label = disjointSet[label];
	}
	return label;
}
int* constructUnion(int labelX, int labelY, int disjointSet[]) {
	while (disjointSet[labelX] != 0) {
		labelX = disjointSet[labelX];
	}
	while (disjointSet[labelY] != 0) {
		labelY = disjointSet[labelY];
	}
	if (labelX != labelY) {
		disjointSet[labelY] = labelX;
	}
	return disjointSet;
}
EMSCRIPTEN_KEEPALIVE unsigned char* segMapToImage(std::vector<int> map, BufferPool* pool, Wasmcv* project) {
	unsigned char* outputBuf = pool->getNew();
	float r = float(rand()) / float(RAND_MAX);
	float g = float(rand()) / float(RAND_MAX);
	float b = float(rand()) / float(RAND_MAX);
	for (int i = 3; i < project->size; i += 4) {
		outputBuf[i - 3] = (unsigned char)(float(map[i]) * float(r));
		outputBuf[i - 2] = (unsigned char)(float(map[i]) * float(g));
		outputBuf[i - 1] = (unsigned char)(float(map[i]) * float(b));
		outputBuf[i] = 255;
	}
	return outputBuf;
}
EMSCRIPTEN_KEEPALIVE unsigned char* perimeterMapToImage(std::vector<int> map, BufferPool* pool, Wasmcv* project) {
	unsigned char* outputBuf = pool->getNew();
	for (int i = 0; i < project->size; i += 1) {
		outputBuf[i] = 0;
	}
	for (int i = 0; i < project->size; i += 1) {
		outputBuf[map[i]] = 102;
		outputBuf[map[i] + 1] = 255;
		outputBuf[map[i] + 2] = 51;
		outputBuf[map[i] + 3] = 255;
	}
	return outputBuf;
}
EMSCRIPTEN_KEEPALIVE int getRegionArea(std::vector<int> map, int label, Wasmcv* project) {
	int area = 0;
	for (int i = 3; i < project->size; i += 4) {
		if (map[i] == label) {
			area += 1;
		}
	}
	return area;
}
EMSCRIPTEN_KEEPALIVE std::vector<int> getAllRegionAreas(std::vector<int> map, Wasmcv* project) {
 	std::vector<int> hist(1200, 0);
	for (int i = 3; i < project->size; i += 4) {
		hist[map[i]] += 1;
	}
	return hist;
}
EMSCRIPTEN_KEEPALIVE std::vector<int> getRegionCentroid(std::vector<int> map, int label, Wasmcv* project) {
	std::vector<int> centroid;
	long accumulatedX = 0;
	long accumulatedY = 0;
	int area = 0;
	for (int i = 3; i < project->size; i += 4) {
		if (map[i] == label) {
			int pixelOffset = (i - 3) / 4;
			int x = pixelOffset % project->w;
	 		int y = pixelOffset / project->w;
	 		accumulatedX += x;
	 		accumulatedY += y;
	 		area += 1;
		}
	}
	centroid.push_back(accumulatedX / area);
	centroid.push_back(accumulatedY / area);
	return centroid;
}
EMSCRIPTEN_KEEPALIVE std::vector<int> getAllRegionCentroids(std::vector<int> map, int areaThresh, Wasmcv* project) {
	std::vector<int> centroids(1200, 0);
	long accumulatedX[1200] = {0};
	long accumulatedY[1200] = {0};
	int area[1200] = {0};
	for (int i = 3; i < project->size; i += 4) {
		if (map[i] != 0) {
			int pixelOffset = (i - 3) / 4;
			int x = pixelOffset % project->w;
	 		int y = pixelOffset / project->w;
	 		accumulatedX[map[i]] += x;
	 		accumulatedY[map[i]] += y;
			area[map[i]] += 1;
		}
	}
	for (int i = 0; i < 1200; i += 1) {
		if (area[i] > areaThresh) {
			int avgX = accumulatedX[i] / area[i];
			int avgY = accumulatedY[i] / area[i];
			int offset = avgY * project->w * 4 + (avgX * 4);
			centroids[i] = offset;
		}
	}
	return centroids;
}
EMSCRIPTEN_KEEPALIVE std::vector<int> getRegion(std::vector<int> map, int label, Wasmcv* project) {
	std::vector<int> region;
	for (int i = 3; i < project->size; i += 4) {
		if (map[i] == label) {
			region.push_back(i);
		}
	}
	return region;
}
EMSCRIPTEN_KEEPALIVE std::vector<int> getRegionPerimeter(std::vector<int> map, int label, Wasmcv* project) {
	std::vector<int> perimeter;
	auto region = getRegion(map, label, project);
	for (int i = 0; i < region.size(); i += 1) {
		std::vector<int> neighborLabels;
		for (int j = 0; j < 5; j += 1) {
			if (isInImageBounds(project, region[i] + project->offsets._4n[j])) {
				neighborLabels.push_back(map[region[i] + project->offsets._4n[j]]);
			}
		}
		int value = label;
		for (int j = 0; value == label && j < neighborLabels.size(); j += 1) {
			value = neighborLabels[j];
		}
		if (value != label) {
			perimeter.push_back(region[i] - 3);
		}
	}
	return perimeter;
}
EMSCRIPTEN_KEEPALIVE std::vector<int> getBoundingBox(std::vector<int> perimeterMap, Wasmcv* project) {
	auto vec2 = offsetToVec2(perimeterMap[0], project);
	int xMax, xMin = vec2[0];
	int yMax, yMin = vec2[1];
	std::vector<int> boundingBox(4);;
	for (int i = 1; i < perimeterMap.size(); i += 1) {
		auto vec2 = offsetToVec2(perimeterMap[i], project);
	 	if (vec2[0] > xMax) xMax = vec2[0];
	 	if (vec2[0] < xMin) xMin = vec2[0];
	 	if (vec2[1] > yMax) yMax = vec2[1];
	 	if (vec2[1] < yMin) yMin = vec2[1];
	}
	boundingBox[0] = xMin;
	boundingBox[1] = yMin;
	boundingBox[2] = xMax - xMin;
	boundingBox[3] = yMax - yMin;
	return boundingBox;
}
EMSCRIPTEN_KEEPALIVE int getExtremalAxisLength(int x1, int y1, int x2, int y2) {
	float angle = std::atan2(y2 - y1, x2 - x1);
	float increment = angle < 0.785398 ? 1 / std::cos(angle) : 1 / std::sin(angle);
	return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2) + increment);
}
EMSCRIPTEN_KEEPALIVE float getSecondOrderRowMoment(std::vector<int> map, int label, Wasmcv* project) {
	auto region = getRegion(map, label, project);
	std::vector<int> yVals;
	float yAcc = 0;
	for (int i = 0; i < region.size(); i += 1) {
		auto vec2 = offsetToVec2(region[i], project);
		yVals.push_back(vec2[1]);
		yAcc += vec2[1];
	}
	float mean = yAcc / float(region.size());
	float sum = 0;
	for (int i = 0; i < region.size(); i += 1) {
		sum += std::pow(float(yVals[i]) - mean, 2);
	}
	float secondOrderRowMoment = sum / float(region.size());
	return secondOrderRowMoment;
}
EMSCRIPTEN_KEEPALIVE float getSecondOrderColumnMoment(std::vector<int> map, int label, Wasmcv* project) {
	auto region = getRegion(map, label, project);
	std::vector<int> xVals;
	float xAcc = 0;
	for (int i = 0; i < region.size(); i += 1) {
		auto vec2 = offsetToVec2(region[i], project);
		xVals.push_back(vec2[0]);
		xAcc += vec2[0];
	}
	float mean = xAcc / float(region.size());
	float sum = 0;
	for (int i = 0; i < region.size(); i += 1) {
		sum += std::pow(float(xVals[i]) - mean, 2);
	}
	float secondOrderColumnMoment = sum / float(region.size());
	return secondOrderColumnMoment;
}
EMSCRIPTEN_KEEPALIVE float getSecondOrderMixedMoment(std::vector<int> map, int label, Wasmcv* project) {
	auto region = getRegion(map, label, project);
	std::vector<int> xVals, yVals;
	float xAcc = 0, yAcc = 0;
	for (int i = 0; i < region.size(); i += 1) {
		auto vec2 = offsetToVec2(region[i], project);
		xVals.push_back(vec2[0]);
		yVals.push_back(vec2[1]);
		xAcc += vec2[0];
		yAcc += vec2[1];
	}
	float xMean = xAcc / float(region.size());
	float yMean = yAcc / float(region.size());
	float sum = 0;
	for (int i = 0; i < region.size(); i += 1) {
		sum += (yVals[i] - yMean) * (xVals[i] - xMean);
	}
	float secondOrderMixedMoment = sum / float(region.size());
	return secondOrderMixedMoment;
}

#ifdef __cplusplus
}
#endif