FROM gitpod/workspace-full

# install dependencies
RUN sudo apt-get update \
 && sudo apt-get install -y \
    python3-dev \
    libclang-12-dev \
    libffi-dev \
    clangd-12 \
    bear \
    manpages-posix-dev \
    manpages-dev \
    python-pygments \
    libasan6 \
    make \
    build-essential \
    zlib1g-dev \
    libncurses5-dev \
    libgdbm-dev \
    libnss3-dev \
    libssl-dev \
    libsqlite3-dev \
    libreadline-dev \
    libffi-dev curl \
    libbz2-dev \
    pkg-config \
    && sudo rm -rf /var/lib/apt/lists/*

# downloading and extracting premake5
RUN sudo wget https://github.com/premake/premake-core/releases/download/v5.0.0-beta1/premake-5.0.0-beta1-linux.tar.gz \
 && sudo tar xzf premake-5.0.0-beta1-linux.tar.gz \
 && sudo mv premake5 /usr/bin \
 && sudo rm premake-5.0.0-beta1-linux.tar.gz

# downloading qlibc and installing
RUN git clone https://github.com/wolkykim/qlibc.git \
 && cd qlibc \
 && ./configure --prefix=/usr/ \
 && make \
 && sudo make install \
 && cd .. \
 && rm -fr qlibc/

# downloading and installing python3.10 in debug mode
RUN wget https://www.python.org/ftp/python/3.10.7/Python-3.10.7.tar.xz \
 && tar -xf Python-3.10.7.tar.xz \
 && cd Python-3.10.7 \
 && ./configure --enable-shared --with-trace-refs --with-assertions --with-address-sanitizer --with-pydebug \
 && make \
 && sudo make altinstall \
 && sudo ldconfig $(pwd) \
 && cd .. \
 && rm Python-3.10.7.tar.xz
