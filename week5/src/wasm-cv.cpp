#include <iostream>
#include <vector>
#include "util.h"
#include "bina.h"
#include "gray.h"
#include "rgba.h"
#include "face.h"

#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE void update();
unsigned int Siz=EM_ASM_INT({return parseInt(window.innerHeight,10);});
Wasmcv* project=new Wasmcv(Siz, Siz);
BufferPool* bufferPool=new BufferPool(Siz, Siz);

int main() {
	std::cout << "Object detection on!\n";
	return 0;
}

EMSCRIPTEN_KEEPALIVE void update() {
	int grayscaleChecked=EM_ASM_INT(return toGrayscale.checked);
	int thresholdChecked=EM_ASM_INT(return threshold.checked);
	int cornersChecked=EM_ASM_INT(return corners.checked);
	int segmentsChecked=EM_ASM_INT(return segments.checked);
	int segmentVisualizerChecked=EM_ASM_INT(return segmentVisualizer.checked);
	int centroidsChecked=EM_ASM_INT(return centroids.checked);
	int perimeterChecked=EM_ASM_INT(return perimeter.checked);
	int boundingChecked=EM_ASM_INT(return bounding.checked);
	EM_ASM(
		outputOverlayCtx.clearRect(0, 0, webcamWidth, webcamHeight);
		t1=performance.now();
		inputCtx.drawImage(video, 0, 0);
		inputImgData=inputCtx.getImageData(0, 0, webcamWidth, webcamHeight);
	);
	int inputBufInt=EM_ASM_INT(
		const inputBuf=Module._malloc(inputImgData.data.length);
		Module.HEAPU8.set(inputImgData.data, inputBuf);
		return inputBuf;
	);
	unsigned char* inputBuf=(unsigned char*)inputBufInt;
	bufferPool->copyToNew(inputBuf);
	if (grayscaleChecked) {
		toGrayscale(bufferPool->getCurrent(), bufferPool, project);
		auto integral=makeIntegralImage(bufferPool->getCurrent(), project);
	}
	if (thresholdChecked) otsu(bufferPool->getCurrent(), bufferPool, project);
	unsigned char* processedImage=bufferPool->getCurrent();
	if (cornersChecked) {
		auto corners=findAllCorners(processedImage, project);
		for (int i=0; i < corners.size(); i += 1) {
			auto vec2=offsetToVec2(corners[i], project);
			EM_ASM_({
				outputOverlayCtx.strokeStyle='#ff33cc';
		    	outputOverlayCtx.lineWidth=1;
		    	outputOverlayCtx.beginPath();
				outputOverlayCtx.moveTo($0, $1 - 11);
				outputOverlayCtx.lineTo($0, $1 + 11);
				outputOverlayCtx.stroke();
				outputOverlayCtx.beginPath();
				outputOverlayCtx.moveTo($0 - 11, $1);
				outputOverlayCtx.lineTo($0 + 11, $1);
				outputOverlayCtx.stroke();
			}, vec2[0], vec2[1]);
		}
	}
	if (segmentsChecked) {
		auto segmentationMap=getConnectedComponents(bufferPool->getCurrent(), project);
		EM_ASM(
			segmentVisualizer.disabled=false;
			centroids.disabled=false;
			perimeter.disabled=false;
		);
		if (segmentVisualizerChecked) {
			unsigned char* visualized=segMapToImage(segmentationMap, bufferPool, project);
			EM_ASM({
				const outputImgData=new ImageData(webcamWidth, webcamHeight);
				for (var i=0, len=outputImgData.data.length; i < len; i += 1) {
					outputImgData.data[i]=Module.HEAPU8[$0 + i];
				}
				outputOverlayCtx.putImageData(outputImgData, 0, 0);
			}, visualized);
		} 
		if (centroidsChecked) {
			auto allCentroids=getAllRegionCentroids(segmentationMap, 40, project);
			for (int i=0; i < allCentroids.size(); i += 1) {
				auto vec2=offsetToVec2(allCentroids[i], project);
				EM_ASM({
					outputOverlayCtx.strokeStyle='#66ff33';
				 	outputOverlayCtx.lineWidth=6;
			    	outputOverlayCtx.beginPath();
					outputOverlayCtx.moveTo($0, $1 - 11);
					outputOverlayCtx.lineTo($0, $1 + 11);
					outputOverlayCtx.stroke();
					outputOverlayCtx.beginPath();
					outputOverlayCtx.moveTo($0 - 11, $1);
					outputOverlayCtx.lineTo($0 + 11, $1);
					outputOverlayCtx.stroke(); 
				}, vec2[0], vec2[1]);
			}
		}
		if (perimeterChecked) {
			EM_ASM(
				bounding.disabled=false;
			);
			auto perimeterMap=getRegionPerimeter(segmentationMap, segmentationMap[615683], project);
			unsigned char* perimeterMapImage=perimeterMapToImage(perimeterMap, bufferPool, project);
			EM_ASM({
				const outputImgData=new ImageData(webcamWidth, webcamHeight);
				for (var i=0, len=outputImgData.data.length; i < len; i += 1) {
					outputImgData.data[i]=Module.HEAPU8[$0 + i];
				}
				outputOverlayCtx.putImageData(outputImgData, 0, 0);
			}, perimeterMapImage);
			EM_ASM({
				outputOverlayCtx.strokeStyle='#ff0000';
			 	outputOverlayCtx.lineWidth=2;
		    	outputOverlayCtx.beginPath();
				outputOverlayCtx.moveTo($0, $1 - 11);
				outputOverlayCtx.lineTo($0, $1 + 11);
				outputOverlayCtx.stroke();
				outputOverlayCtx.beginPath();
				outputOverlayCtx.moveTo($0 - 11, $1);
				outputOverlayCtx.lineTo($0 + 11, $1);
				outputOverlayCtx.stroke(); 
			}, project->centerX, project->centerY);
			if (boundingChecked) {
				auto boundingBox=getBoundingBox(perimeterMap, project);
				auto val=getSecondOrderColumnMoment(segmentationMap, segmentationMap[615683], project);
				std::cout << val << std::endl;
				EM_ASM({
					outputOverlayCtx.strokeStyle='#005ce6';
					outputOverlayCtx.lineWidth=4;
					outputOverlayCtx.beginPath();
					outputOverlayCtx.rect($0, $1, $2, $3);
					outputOverlayCtx.stroke();
				}, boundingBox[0], boundingBox[1], boundingBox[2], boundingBox[3]);
			}
		} else {
			EM_ASM(
				bounding.checked=false;
				bounding.disabled=true;
			);
		}
	} else {
		EM_ASM(
			segmentVisualizer.checked=false;
			segmentVisualizer.disabled=true;
			centroids.checked=false;
			centroids.disabled=true;
			perimeter.checked=false;
			perimeter.disabled=true;
			bounding.checked=false;
			bounding.disabled=true;
		);
	}
	EM_ASM_({
		const outputImgData=new ImageData(webcamWidth, webcamHeight);
		for (var i=0, len=outputImgData.data.length; i < len; i += 1) {
			outputImgData.data[i]=Module.HEAPU8[$0 + i];
		}
		outputCtx.putImageData(outputImgData, 0, 0);
		Module._free($1);
		t2=performance.now() - t1;
		outputCtx.font='30px Arial';
		outputCtx.fillStyle='red';
		outputCtx.fillText(t2.toFixed(2) + ' ms', 10, 50);
		loopId=window.requestAnimationFrame(Module._update);
	}, processedImage, inputBuf);
}

#ifdef __cplusplus
}
#endif
