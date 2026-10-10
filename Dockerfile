
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    libjsoncpp-dev \
    uuid-dev \
    zlib1g-dev \
    libssl-dev \
    libpq-dev \
    libc-ares-dev \
    libbrotli-dev \
    libhiredis-dev \
    libyaml-cpp-dev \
    libsqlite3-dev \
    libmariadb-dev \
    && rm -rf /var/lib/apt/lists/*

RUN git clone --depth 1 --branch v1.9.13 \
    https://github.com/drogonframework/drogon.git /tmp/drogon \
    && cd /tmp/drogon \
    && git submodule update --init --recursive \
    && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build -j2 \
    && cmake --install build \
    && rm -rf /tmp/drogon

WORKDIR /app

COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build -j2

EXPOSE 8080

CMD ["sh", "-c", "./build/MaliniMart"]
