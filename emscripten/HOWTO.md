# How to build wasm SDLAPL?
Using Docker.

execute:
```sh
docker run -v $(pwd):/src -u $(id -u):$(id -g) -i -t emscripten/emsdk sh
```

then
```sh
cd emscripten
make
```

to test:
```
python3 -m http.server
```
then visit:
http://localhost:8000/sdlpal.html

Emscripten headers:
https://github.com/emscripten-core/emscripten/blob/main/system/include/emscripten/emscripten.h
