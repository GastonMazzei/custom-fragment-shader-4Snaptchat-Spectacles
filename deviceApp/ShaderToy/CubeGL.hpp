/* Copyright 2021 Snap Inc.
 */

#pragma once

#include <memory>


#include "glad/glad.h"

#include <AppKit/Types.hpp>

class CubeGL
{
public:
    using Ptr = std::unique_ptr<CubeGL>;

    enum class ColorMode
    {
        Red,
        RedGreenBlue,
        White
    };

    static Ptr create(float sizeInMeters, ColorMode colorMode, uint32_t numActiveViews = 1);

    void draw(const AppKit::RenderParams* renderParams, uint32_t firstView, uint32_t lastView, float global_time);

    ~CubeGL();

    void putInFront(const VCSE3F& eyeCenterToWorld, float distance, bool rotated);

    static ColorMode strToColorMode(const char* str);
    static std::string receivedFrgShader;

private:
    bool init(float sizeInMeters, ColorMode ColorMode, uint32_t numActiveViews = 1);
    CubeGL();
    GLuint mVBOIndices{ 0 };
    GLuint mVBOVertices{ 0 };
    GLuint mVBOVertexColors{ 0 };
    GLuint mVAO{ 0 };
    GLuint mShaderProg{ 0 };
    GLint mShaderProgramMVPMatrixLocation{ 0 };
    GLsizei mNumIndices{ 0 };
    VCMat4F mModelMatrix{ { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } } };
    bool mDebugMultiview{ false };
    float time=0;
};
