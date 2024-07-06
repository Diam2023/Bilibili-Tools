FROM alpine AS builder

ENV CMAKE_C_COMPILER=gcc \
    CMAKE_CXX_COMPILER=g++ \
    CMAKE_MAKE_PROGRAM=make \
    AR=gcc-ar \
    RANLIB=gcc-ranlib \
    IROOT=/root

# RUN pacman -Syu --noconfirm && pacman -S --needed base-devel cmake c-ares hiredis jsoncpp mariadb --noconfirm
# RUN apt update && apt install -y cmake g++ gcc libjsoncpp-dev uuid-dev zlib1g-dev postgresql-server-dev-all libmariadb-dev-compat openssl libssl-dev libhiredis-dev
RUN apk add hiredis-dev mariadb-dev libuuid util-linux-dev jsoncpp-dev  zlib-dev gcc g++ make cmake

ENV SERVER_ROOT="$IROOT/BiliBiliTools"

RUN mkdir $SERVER_ROOT

WORKDIR $SERVER_ROOT

COPY . $SERVER_ROOT

RUN ls -l

# 赋予运行权限
RUN chmod +x ./build.sh

RUN ./build.sh

FROM alpine

RUN apk add hiredis-dev mariadb-dev libuuid util-linux-dev jsoncpp-dev zlib-dev

COPY --from=builder /root/BiliBiliTools/build/out/BiliBiliTools /root
COPY --from=builder /root/BiliBiliTools/config.json /root

WORKDIR /root

# 暴露端口号
EXPOSE 8998

# 启动服务器
CMD ["/root/BiliBiliTools", "/root/config.json"]
