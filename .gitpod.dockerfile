FROM gitpod/workspace-full

# install dependencies
RUN sudo apt-get update \
 && sudo apt-get install -y \
    python3-dev \
    libclang-dev \
    libffi-dev \
    clangd-12 \
    bear \
    manpages-posix-dev \
    manpages-dev \
    python-pygments \
    && sudo rm -rf /var/lib/apt/lists/*

# downloading and extracting premake5
RUN sudo wget https://github.com/premake/premake-core/releases/download/v5.0.0-beta1/premake-5.0.0-beta1-linux.tar.gz \
 && sudo tar xzf premake-5.0.0-beta1-linux.tar.gz \
 && sudo mv premake5 /usr/bin \
 && sudo rm premake-5.0.0-beta1-linux.tar.gz

RUN git clone https://github.com/wolkykim/qlibc.git \
 && cd qlibc \
 && ./configure --prefix=/usr/ \
 && make \
 && sudo make install \
 && cd .. \
 && rm -fr qlibc/
