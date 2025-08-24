#!/bin/bash

# MicMac PSMNet Docker Build and Run Script (with Claude Code)
set -e

# Redirect all output to both console and docker.log
exec > >(tee -a docker.log) 2>&1

# Use default password if not provided via environment variable
if [ -z "$SUDO_PASSWORD" ]; then
    SUDO_PASSWORD="juk5097!"
fi

# Configuration
IMAGE_NAME="micmac-psmnet"
CONTAINER_NAME="micmac-psmnet-container"
DOCKERFILE="Dockerfile_PSMNet"

# Check if Docker is accessible
DOCKER_CMD="docker"
echo "🔍 Checking Docker access..."

if docker info >/dev/null 2>&1; then
    echo "✅ Docker accessible without sudo"
    DOCKER_CMD="docker"
else
    echo "ℹ️  Using sudo for Docker commands"
    DOCKER_CMD="echo '$SUDO_PASSWORD' | sudo -S docker"
fi

echo "🐳 Building MicMac PSMNet Docker image (with Claude Code)..."
eval "$DOCKER_CMD build --no-cache -t $IMAGE_NAME -f $DOCKERFILE ."

echo "🚀 Running MicMac PSMNet container..."
# Remove existing container if it exists
eval "$DOCKER_CMD rm -f $CONTAINER_NAME" 2>/dev/null || true

# Check if GPU support is available
GPU_FLAGS=""
if command -v nvidia-docker >/dev/null 2>&1 || eval "$DOCKER_CMD run --rm --gpus all nvidia/cuda:11.0-base nvidia-smi" >/dev/null 2>&1; then
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
eval "$DOCKER_CMD run -it \
    --name $CONTAINER_NAME \
    $GPU_FLAGS \
    -v $(pwd):/workspace \
    -w /workspace \
    $IMAGE_NAME"

echo "✅ Container session ended."