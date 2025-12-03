TOOLCHAIN_DIR="/mnt/d/Dev/Github/buildscripts/emscripten"
export TOOLCHAIN_DIR

cmake . -GNinja -Bbuild -B build/emscripten-release -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_DIR/emsdk-portable/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=BOTH -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=BOTH -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DCMAKE_PREFIX_PATH=$TOOLCHAIN_DIR -DCMAKE_INSTALL_PREFIX=output -DCMAKE_BUILD_TYPE=Release -DPLAYER_JS_BUILD_SHELL=ON -DPLAYER_JS_OUTPUT_NAME=index -DPLAYER_BUILD_LIBLCF=ON
cd build/emscripten-release
cmake --build .