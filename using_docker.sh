#!/bin/bash

# MicMac PSMNet Docker Build and Run Script (with Claude Code)
set -e

# Configuration
IMAGE_NAME="micmac-psmnet"
CONTAINER_NAME="micmac-psmnet-container"
DOCKERFILE="Dockerfile_PSMNet"

# Check if Docker is accessible
DOCKER_CMD="docker"
if ! docker info >/dev/null 2>&1; then
    if sudo docker info >/dev/null 2>&1; then
        echo "ℹ️  Using sudo for Docker commands (user not in docker group)"
        DOCKER_CMD="sudo docker"
    else
        echo "❌ Error: Docker daemon is not running or not accessible."
        echo "💡 To fix this, try:"
        echo "   sudo systemctl start docker"
        echo "   sudo systemctl enable docker"
        exit 1
    fi
else
    echo "✅ Docker accessible without sudo"
fi

echo "🐳 Building MicMac PSMNet Docker image (with Claude Code)..."
$DOCKER_CMD build -t $IMAGE_NAME -f $DOCKERFILE .

echo "🚀 Running MicMac PSMNet container..."
# Remove existing container if it exists
$DOCKER_CMD rm -f $CONTAINER_NAME 2>/dev/null || true

# Check if GPU support is available
GPU_FLAGS=""
if command -v nvidia-docker >/dev/null 2>&1 || $DOCKER_CMD run --rm --gpus all nvidia/cuda:11.0-base nvidia-smi >/dev/null 2>&1; then
    echo "🔧 GPU support detected, enabling GPU acceleration..."
    GPU_FLAGS="--gpus all"
else
    echo "⚠️  Warning: No GPU support detected, running in CPU mode..."
fi

echo "📋 Starting interactive container..."
echo "   • MicMac English Fork included"
echo "   • Claude Code included"
echo "   • GPU support: $([[ -n "$GPU_FLAGS" ]] && echo "enabled" || echo "disabled")"
echo ""

# Run container with optional GPU support and volume mount
$DOCKER_CMD run -it \
    --name $CONTAINER_NAME \
    $GPU_FLAGS \
    -v $(pwd):/workspace \
    -w /workspace \
    $IMAGE_NAME

echo "✅ Container session ended."