#!/bin/bash
# One-time WSL2 Ubuntu 24.04 setup for PufferLib 5.0 training on an RTX 50-series GPU.
# Installs NVIDIA's CUDA 12.9 toolkit (WSL repo, no driver - Windows provides it),
# NCCL, clang + OpenMP and the GL headers raylib links against.
set -e
cd /tmp
wget -q https://developer.download.nvidia.com/compute/cuda/repos/wsl-ubuntu/x86_64/cuda-keyring_1.1-1_all.deb
sudo dpkg -i cuda-keyring_1.1-1_all.deb
sudo apt-get update
sudo apt-get install -y cuda-toolkit-12-9 libnccl2 libnccl-dev clang libomp-dev ccache unzip \
    libgl1-mesa-dev
grep -q 'cuda/bin' ~/.bashrc || echo 'export PATH=/usr/local/cuda/bin:$PATH' >> ~/.bashrc
/usr/local/cuda/bin/nvcc --version | tail -1
echo "SETUP DONE"
