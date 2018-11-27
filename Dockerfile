FROM fefrei/carl:latest
COPY / /root/hypro/
RUN apt-get update \
&& apt-get install -y \
    g++-8-multilib \
    openjdk-8-jre \
    uuid-dev \
    pkg-config \
    libboost-dev
RUN cd /root/hypro \
&& mkdir build && cd build && cmake .. \
&& make resources \
&& make
