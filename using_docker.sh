#!/bin/bash

# MicMac PSMNet Docker Build and Run Script (with Claude Code)
set -e

# Configuration
IMAGE_NAME="micmac-psmnet"
CONTAINER_NAME="micmac-psmnet-container"
DOCKERFILE="Dockerfile_PSMNet"

# Check if user is in docker group
if ! groups $(whoami) | grep -q docker; then
    echo "❌ Error: User $(whoami) is not in the 'docker' group."
    echo "💡 To fix this, run the following commands:"
    echo "   sudo usermod -aG docker $(whoami)"
    echo "   newgrp docker  # or logout and login again"
    echo "   Then run this script again."
    exit 1
fi

# Check if Docker is running
if ! docker info >/dev/null 2>&1; then
    echo "❌ Error: Docker daemon is not running or not accessible."
    echo "💡 To fix this, try:"
    echo "   sudo systemctl start docker"
    echo "   sudo systemctl enable docker"
    exit 1
fi

echo "🐳 Building MicMac PSMNet Docker image (with Claude Code)..."
docker build -t $IMAGE_NAME -f $DOCKERFILE .

echo "🚀 Running MicMac PSMNet container..."
# Remove existing container if it exists
docker rm -f $CONTAINER_NAME 2>/dev/null || true

# Check if GPU support is available
GPU_FLAGS=""
if command -v nvidia-docker >/dev/null 2>&1 || docker run --rm --gpus all nvidia/cuda:11.0-base nvidia-smi >/dev/null 2>&1; then
    echo "🔧 GPU support detected, enabling GPU acceleration..."
    GPU_FLAGS="--gpus all"
else
    echo "⚠️  Warning: No GPU support detected, running in CPU mode..."
fi

# Run container with optional GPU support and volume mount
docker run -it \
    --name $CONTAINER_NAME \
    $GPU_FLAGS \
    -v $(pwd):/workspace \
    -w /workspace \
    $IMAGE_NAME \
    /bin/bash

echo "✅ Container session ended."