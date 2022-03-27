
cv:
	em++ wasm-cv.cpp util.cpp bina.cpp gray.cpp rgba.cpp face.cpp -flto=thin -O3 -std=c++1z  \
-sUSE_WEBGL2=1  -sFORCE_FILESYSTEM=1 -sENVIRONMENT=web \
-sALLOW_MEMORY_GROWTH=0 -sINITIAL_MEMORY=1400mb -sMALLOC="emmalloc" \
-o ../wasm-cv.js  -sEXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \

all: cv
	echo 'Built 1ink.us Computer Vision.'
