FROM manjarolinux/base

ENV CC=gcc \
    CXX=g++ \
    AR=gcc-ar \
    RANLIB=gcc-ranlib \
    IROOT=/root

RUN pacman -Syu --noconfirm && pacman -S --needed base-devel cmake git c-ares hiredis jsoncpp mariadb yay qt5-mqtt yarn yaml-cpp --noconfirm

# Drogon

ENV DROGON_ROOT="$IROOT/drogon"

RUN git clone https://github.com/drogonframework/drogon $DROGON_ROOT

WORKDIR $DROGON_ROOT

RUN ./build.sh

RUN drogon_ctl version


# BiliBiliTools

ENV SERVER_ROOT="$IROOT/BiliBiliTools"

RUN git clone https://github.com/Diam2023/Bilibili-Tools.git $SERVER_ROOT

WORKDIR $SERVER_ROOT

# 赋予运行权限
RUN chmod +x ./build.sh

RUN ./build.sh

ENV BIN_DIR="$SERVER_ROOT/build"

WORKDIR $BIN_DIR

# 配置文件
ENV CONFIG_FILE="$SERVER_ROOT/config.json"

ENV TARGET_FILE="$BIN_DIR/BiliBiliTools"


# 暴露端口号
EXPOSE 8099

# 启动服务器
CMD [ $TARGET_FILE, $CONFIG_FILE ]
