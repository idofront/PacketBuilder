# PacketBuilder

PacketBuilder は任意のパケットを生成するためのヘルパーです．

利用例として `./main/PacketBuilder.cpp` を用意しています．

## Package

以下は Ubuntu でビルドする際に必要なパッケージです．

```bash
git submodule update --init --recursive
apt install \
    libpcap-dev \
    libspdlog-dev \
    nlohmann-json3-dev \
    libboost-all-dev \
    libfmt-dev
```

## Input Format

以下の入力フォーマットに対応しています．

- JSON
- PCAP

## Output Format

以下の出力フォーマットに対応しています．

- JSON
- PCAP (usec 精度)

# PluginSystem

各プロトコルのパーサをプラグインとして追加できる仕組みを提供するアプリケーションです．

## Package

以下は Ubuntu でビルドする際に必要なパッケージです．

```bash
git submodule update --init --recursive
apt install \
    libpcap-dev \
    libspdlog-dev \
    nlohmann-json3-dev \
    libboost-all-dev \
    libfmt-dev
    libpoco-dev
```

