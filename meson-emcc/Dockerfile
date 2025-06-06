FROM emscripten/emsdk:4.0.9-arm64

RUN apt-get update && apt-get install -y python3-pip zip git && pip3 install meson ninja

WORKDIR /src

CMD ["/bin/bash"]
