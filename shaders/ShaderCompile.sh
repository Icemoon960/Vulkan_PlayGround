#!/bin/bash

glslc ./shaders/shader.vert -o ./build/.shaders/vert.spv
glslc ./shaders/shader.frag -o ./build/.shaders/frag.spv
