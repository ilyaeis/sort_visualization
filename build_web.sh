#!/bin/sh
# Builds the sorting algorithms to WebAssembly (see "Web build" in README.md).
# Needs em++ from emsdk on the PATH. Output: web/dist/sortvis.js and web/dist/sortvis.wasm.
set -e
cd "$(dirname "$0")"
mkdir -p web/dist
em++ -Oz -std=c++17 -fno-exceptions -fno-rtti \
  web/shim.cpp Code/SortAlgorithms.cpp Code/ShuffleAlgorithms.cpp \
  -sMODULARIZE -sEXPORT_ES6 -sEXPORT_NAME=createSortVis \
  -sENVIRONMENT=web,worker -sALLOW_MEMORY_GROWTH -sFILESYSTEM=0 -sSTACK_SIZE=1MB \
  -sEXPORTED_FUNCTIONS=_sv_srand,_sv_rand,_sv_fill,_sv_gen_shuffle,_sv_shuffle,_sv_sort,_sv_log,_sv_array,_sv_length,_malloc,_free \
  -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,HEAP32 \
  -o web/dist/sortvis.js
ls -l web/dist
