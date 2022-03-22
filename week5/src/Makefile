
cv: h3.cpp  Makefile
	em++ wasm-cv.cpp util.cpp bina.cpp gray.cpp rgba.cpp face.cpp -flto=thin -DNDEBUG --closure 1 -g0 -std=gnu++2b -O2  -sFULL_ES2=0 -sFULL_ES3=1 -sGL_TESTING=1 \
-sUSE_WEBGL2=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sFORCE_FILESYSTEM=1 -sENVIRONMENT=web \
-sALLOW_MEMORY_GROWTH=0 -sINITIAL_MEMORY=1400mb -sMALLOC="emmalloc" -ffast-math \
-o ../wasm-cv.js  -sEXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \

all: cv
	echo 'Built 1ink.us Computer Vision.'
