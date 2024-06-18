FROM manjarolinux/base

ENV CC=gcc \
    CXX=g++ \
    AR=gcc-ar \
    RANLIB=gcc-ranlib \
    IROOT=/root

RUN pacman -Syu --noconfirm && pacman -S --needed base-devel cmake c-ares hiredis jsoncpp mariadb --noconfirm

# BiliBiliTools

ENV SERVER_ROOT="$IROOT/BiliBiliTools"

RUN mkdir $SERVER_ROOT

WORKDIR $SERVER_ROOT

RUN ls -l

COPY . $SERVER_ROOT

RUN ls -l

# 赋予运行权限
RUN chmod +x ./build.sh

RUN ./build.sh

ENV BIN_DIR="$SERVER_ROOT/build"

WORKDIR $BIN_DIR

# 配置文件
ENV CONFIG_FILE="$SERVER_ROOT/config.json"

ENV TARGET_FILE="$BIN_DIR/BiliBiliTools"


# 暴露端口号
EXPOSE 8998

# 启动服务器
CMD [ "/root/BiliBiliTools/build/BiliBiliTools/BiliBiliTools", "/root/BiliBiliTools/config.json" ]
