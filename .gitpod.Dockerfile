FROM gitpod/workspace-full

# Install custom tools, runtimes, etc.
# For example "bastet", a command-line tetris clone:
# RUN brew install bastet
#
# More information: https://www.gitpod.io/docs/config-docker/

USER root
# Install dependencies.
# =====================

# HDF5 libraries and links
RUN apt-get update \
    && apt-get -y install libgtk2.0-dev libvdpau-dev libva-dev \
    && apt-get -y install \
        libblas-dev \
        liblapack-dev \
        libhdf5-dev \
        libhdf5-serial-dev \
        libhdf5-103 \
        libhdf5-cpp-103 \
    && apt-get -y install doxygen \
    && apt-get autoremove -yq \
    && rm -rf /var/lib/apt/lists/*
RUN echo /usr/lib/x86_64-linux-gnu >> /etc/ld.so.conf
RUN echo /usr/lib/x86_64-linux-gnu/hdf5/serial >> /etc/ld.so.conf
RUN cat /etc/ld.so.conf
RUN ldconfig

USER gitpod
RUN pip install conan
