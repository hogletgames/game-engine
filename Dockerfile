FROM ubuntu:bionic

# Build tools
RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y software-properties-common make cmake git

# Compilers
RUN apt-add-repository ppa:ubuntu-toolchain-r/test
RUN apt-get update
RUN apt-get install -y gcc-10 g++-10

# xorg
RUN apt-get install -y xorg-dev libglu1-mesa libgl1-mesa-dev mesa-utils

# xvfb
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y xvfb libxinerama1 libxcursor1
ADD tools/xvfb/xvfb_init /etc/init.d/xvfb
ADD tools/xvfb/xvfb_daemon_start /usr/bin/xvfb-daemon-start
ENV DISPLAY :99.0
RUN xvfb-daemon-start

RUN apt-get install -y python3 python3-pip gdb
RUN pip3 install --user pytest
