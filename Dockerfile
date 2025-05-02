FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /nanokernel

COPY . .

RUN mkdir build && \
    cd build && \
    cmake .. && \
    make

CMD ["./build/nanokernel"] 