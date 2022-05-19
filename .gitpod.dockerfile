FROM gitpod/workspace-full

# install dependencies
RUN sudo apt-get update \
 && sudo apt-get install -y \
    python3-dev \
    libclang-dev \
    libffi-dev \
    clangd-12 \
    bear \
 && sudo rm -rf /var/lib/apt/lists/*

# downloading and extracting premake5
RUN sudo wget https://github.com/premake/premake-core/releases/download/v5.0.0-beta1/premake-5.0.0-beta1-linux.tar.gz \
 && sudo tar xzf premake-5.0.0-beta1-linux.tar.gz \
 && sudo mv premake5 /usr/bin \
 && sudo rm premake-5.0.0-beta1-linux.tar.gz
