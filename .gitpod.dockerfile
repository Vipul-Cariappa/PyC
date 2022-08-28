FROM gitpod/workspace-full

# install dependencies
RUN sudo apt-get update \
 && sudo apt-get install -y \
    python3-dev \
    build-essential \
    zlib1g-dev \
    libncurses5-dev \
    libgdbm-dev \
    libnss3-dev \
    libssl-dev \
    libreadline-dev \
    libffi-dev \
    wget \
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

# download and install qlibc
RUN git clone https://github.com/wolkykim/qlibc.git \
 && cd qlibc \
 && ./configure --prefix=/usr/ \
 && make \
 && sudo make install \
 && cd .. \
 && rm -fr qlibc/

# download and install python3.10 with Address Sanitizer
RUN wget https://www.python.org/ftp/python/3.9.13/Python-3.9.13.tgz \
 && tar -xf Python-3.9.13.tgz \
 && cd Python-3.9.13 \
 && ./configure --with-address-sanitizer --with-assertions --with-pydebug --enable-shared \
 && make \
 && sudo make altinstall \
 && cd .. \
 && rm Python-3.9.13.tgz \
 && rm Python-3.9.13
