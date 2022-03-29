#include <emscripten/emscripten.h>
#include <vector>

#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif
EM_JS(int,Si,(),{
return parseInt(window.innerHeight,10);
})
EMSCRIPTEN_KEEPALIVE Wasmcv* init(int w, int h) {
	int Siz=Si();
	Wasmcv* project=new Wasmcv(Siz, Siz);
	return project;
}
EMSCRIPTEN_KEEPALIVE bool isInImageBounds(Wasmcv* project, int offset) {
	if (offset >= 0 && offset < project->size) {
		return true;
	} else {
		return false;
	}
}
EMSCRIPTEN_KEEPALIVE std::vector<int> offsetToVec2(int offset, Wasmcv* project) {
	std::vector<int> vec2(2);
	int pixelOffset=offset / 4;
	vec2[0]=pixelOffset % project->w;
	vec2[1]=pixelOffset / project->w;
	return vec2;
}
EMSCRIPTEN_KEEPALIVE int vec2ToOffset(int x, int y, Wasmcv* project) {
	return (y * project->w * 4) + (x * 4);
}

#ifdef __cplusplus
}
#endif
