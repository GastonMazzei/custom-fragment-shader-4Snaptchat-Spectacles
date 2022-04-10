/* Copyright 2021 Snap Inc.
 */

#include <vector>
#include <string>
#include <iostream>
#include <string.h>
#include <assert.h>

#include "CubeGL.hpp"
#include <AppKit/Utils.hpp>
#include <AppKit/Logging.hpp>
#include <VisionCore/VCMath.h>


using namespace AppKit;

std::string CubeGL::receivedFrgShader = R"(        
        precision mediump float;
        out vec4 outColor;
        in vec3 frgColor;
        uniform float iGlobalTime;

        void main() {
            // Option 1: let the vertex color attribute
            //           determine the color.
            //
            // outColor = vec4(frgColor, 1.0f);

            // Option 2: specify the color
            //
            outColor = vec4(0,0.5f,0.5f, 1.0f);

            // Option 3: some position-dependent function
            //
            //                        slow spatial period
            //vec3 info = 0.5 + 0.5 * cos(0.01 * gl_FragCoord.xyz * iGlobalTime);
            //outColor = vec4(info, 1.0f);
        }
    )";


CubeGL::CubeGL() = default;

CubeGL::~CubeGL()
{
    if (mShaderProg != 0)
    {
        glDeleteProgram(mShaderProg);
        mShaderProg = 0;
    }

    if (mVAO != 0)
    {
        glDeleteVertexArrays(1, &mVAO);
        mVAO = 0;
    }

    if (mVBOVertexColors != 0)
    {
        glDeleteBuffers(1, &mVBOVertexColors);
        mVBOVertexColors = 0;
    }

    if (mVBOVertices != 0)
    {
        glDeleteBuffers(1, &mVBOVertices);
        mVBOVertices = 0;
    }

    if (mVBOIndices != 0)
    {
        glDeleteBuffers(1, &mVBOIndices);
        mVBOIndices = 0;
    }
}

CubeGL::Ptr
CubeGL::create(float sizeInMeters, ColorMode colorMode, const uint32_t numActiveViews)
{
    CubeGL::Ptr ret{ new CubeGL{} };
    if (ret->init(sizeInMeters, colorMode, numActiveViews))
        return ret;
    else
        return nullptr;
}

namespace {

GLuint
compile_shader(const std::string& shader_source,
               GLenum shader_type,
               const std::vector<std::string>& preprocessor_macros)
{

    GLuint shader_id = glCreateShader(shader_type);

    const GLchar* shader_source_c_str = static_cast<const GLchar*>(shader_source.c_str());
    const GLint shader_source_c_str_length = static_cast<GLint>(shader_source.length());

    std::vector<const GLchar*> sources;
    std::vector<GLint> source_lengths;

    for (const std::string& s : preprocessor_macros)
    {
        sources.push_back(static_cast<const GLchar*>(s.c_str()));
        source_lengths.push_back(static_cast<GLint>(s.length()));
    }

    static const std::string line_zero = "#line 0 \n";

    if (!sources.empty())
    {
        sources.push_back(static_cast<const GLchar*>(line_zero.c_str()));
        source_lengths.push_back(static_cast<GLint>(line_zero.length()));
    }

    sources.push_back(shader_source_c_str);
    source_lengths.push_back(shader_source_c_str_length);

    glShaderSource(shader_id, static_cast<GLsizei>(sources.size()), sources.data(), source_lengths.data());

    glCompileShader(shader_id);

    GLint result = GL_FALSE;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE)
    {

        std::string compiler_error_string = "<unknown>";
        GLint info_log_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0)
        {
            std::vector<char> buffer(info_log_length);
            glGetShaderInfoLog(shader_id, info_log_length, nullptr, buffer.data());
            compiler_error_string = std::string(buffer.data(), buffer.size());
        }

        LOG_ERROR("failed to compile GLSL source '%s', reason: '%s'.", shader_source.c_str(), compiler_error_string.c_str());
        return 0;
    }

    return shader_id;
}

GLuint
create_glsl_program_with_fs_and_vs(
    const std::string& vertex_shader_source,
    const std::string& fragment_shader_source,
    const std::vector<std::string>& vertex_shader_source_preprocessor_macros,
    const std::vector<std::string>& fragment_shader_source_preprocessor_macros)
{

    GLuint vertex_shader_id =
        compile_shader(vertex_shader_source, GL_VERTEX_SHADER, vertex_shader_source_preprocessor_macros);

    if (vertex_shader_id == 0)
        return 0;

    GLuint fragment_shader_id =
        compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER, fragment_shader_source_preprocessor_macros);

    if (fragment_shader_id == 0)
        return 0;

    GLuint program_id = glCreateProgram();
    //    glProgramParameteri(program_id, GL_PROGRAM_SEPARABLE, GL_TRUE);
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    GLint result = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);

    if (result != GL_TRUE)
    {
        std::string linker_error_string = "<unknown>";
        GLint info_log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0)
        {
            std::vector<char> buffer(info_log_length);
            glGetProgramInfoLog(program_id, info_log_length, nullptr, buffer.data());
            linker_error_string = std::string(buffer.data(), buffer.size());
        }

        LOG_ERROR("Failed to compile GLSL program with vertex shader '%s' and fragment shader: '%s', reason: '%s'.",
                  vertex_shader_source.c_str(), fragment_shader_source.c_str(), linker_error_string.c_str());
        return 0;
    }

    // Shader objects won't actually be deleted until they
    // are detached from all programs.

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}

const std::vector<GLfloat> cubeVertices = {
    // clang-format off
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,

    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,

     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f
    // clang-format on
};

const std::vector<GLushort> cubeIndices = {
    // clang-format off
    0, 1, 2,
    2, 3, 0,
    4, 6, 5,
    6, 4, 7,
    8, 10, 9,
    10, 8, 11,
    12, 13, 14,
    14, 15, 12,
    16, 17, 18,
    18, 19, 16,
    20, 22, 21,
    22, 20, 23
    // clang-format on
};

const std::vector<GLfloat> cubeNormals = {
    // clang-format off
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,

    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,

    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,

    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,

    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,

    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0
    // clang-format on
};

const std::vector<GLfloat> cubeVertexColorsRed = {
    // clang-format off
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.85f, 0.0f, 0.0f,
    0.85f, 0.0f, 0.0f,
    0.85f, 0.0f, 0.0f,
    0.85f, 0.0f, 0.0f,

    0.85f, 0.0f, 0.0f,
    0.85f, 0.0f, 0.0f,
    0.85f, 0.0f, 0.0f,
    0.85f, 0.0f, 0.0f,

    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,

    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    0.8f, 0.0f, 0.0f,
    // clang-format on
};

const std::vector<GLfloat> cubeVertexColorsRedGreenBlue = {
    // clang-format off
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    // clang-format on
};

const std::vector<GLfloat> cubeVertexColorsWhite = {
    // clang-format off
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,

    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,

    0.85f, 0.85f, 0.85f,
    0.85f, 0.85f, 0.85f,
    0.85f, 0.85f, 0.85f,
    0.85f, 0.85f, 0.85f,

    0.85f, 0.85f, 0.85f,
    0.85f, 0.85f, 0.85f,
    0.85f, 0.85f, 0.85f,
    0.85f, 0.85f, 0.85f,

    0.8f, 0.8f, 0.8f,
    0.8f, 0.8f, 0.8f,
    0.8f, 0.8f, 0.8f,
    0.8f, 0.8f, 0.8f,

    0.8f, 0.8f, 0.8f,
    0.8f, 0.8f, 0.8f,
    0.8f, 0.8f, 0.8f,
    0.8f, 0.8f, 0.8f,
    // clang-format on
};

const std::vector<GLfloat> cubeTexCoords = {
    // clang-format off
    0, 1,
    0, 0,
    1, 0,
    1, 1,

    0, 1,
    0, 0,
    1, 0,
    1, 1,

    0, 1,
    0, 0,
    1, 0,
    1, 1,

    0, 1,
    0, 0,
    1, 0,
    1, 1,

    0, 1,
    0, 0,
    1, 0,
    1, 1,

    0, 1,
    0, 0,
    1, 0,
    1, 1
    // clang-format on
};


std::vector<GLfloat> polyVertices = {
        // clang-format off
        -1.0f, -1.0f, .0f,
        -1.0f,  1.0f, .0f,
        1.0f,  1.0f, .0f,
        1.0f, -1.0f, .0f
        // clang-format on
};


std::vector<GLushort> polyIndices = {
        // clang-format off
        0, 2, 1,
        2, 0, 3
        // clang-format on
};




std::vector<GLfloat> polyVertexColorsWhite = {
        // clang-format off
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        // clang-format on
};

const std::vector<GLfloat> polyTexCoords = {
        // clang-format off
        0, 1,
        0, 0,
        1, 0,
        1, 1
        // clang-format on
};



}

bool
CubeGL::init(float sizeInMeters, ColorMode colorMode, const uint32_t numActiveViews)
{
    // apply scale
    std::vector<GLfloat> verticesScaled = polyVertices;//cubeVertices;
    for (auto& v : verticesScaled)
        v *= sizeInMeters;

    // set up VBOs
    glGenBuffers(1, &mVBOIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBOIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLushort) * polyIndices.size(), //cubeIndices.size(),
                 polyIndices.data(), //cubeIndices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenBuffers(1, &mVBOVertices);
    glBindBuffer(GL_ARRAY_BUFFER, mVBOVertices);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * verticesScaled.size(),
                 verticesScaled.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    const GLfloat* colorData = nullptr;
    size_t colorDataSize = 0;

    switch (colorMode)
    {
    case ColorMode::Red:
        colorData = cubeVertexColorsRed.data();
        colorDataSize = cubeVertexColorsRed.size();
        break;
    case ColorMode::RedGreenBlue:
        colorData = cubeVertexColorsRedGreenBlue.data();
        colorDataSize = cubeVertexColorsRedGreenBlue.size();
        break;
    case ColorMode::White:
        colorData = polyVertexColorsWhite.data();// cubeVertexColorsWhite.data();
        colorDataSize = polyVertexColorsWhite.size();//cubeVertexColorsWhite.size();
        break;
    default:
        std::cerr << "invalid color mode\n";
        return false;
    }

    glGenBuffers(1, &mVBOVertexColors);
    glBindBuffer(GL_ARRAY_BUFFER, mVBOVertexColors);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * colorDataSize,
                 colorData,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBOIndices);
    mNumIndices = static_cast<GLsizei>(polyIndices.size());//cubeIndices.size());

    // colors
    glBindBuffer(GL_ARRAY_BUFFER, mVBOVertexColors);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<char*>(NULL));
    glEnableVertexAttribArray(1);

    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBOVertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<char*>(NULL));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    static const std::string vtxShader = R"(
        #if defined(OVR_MULTIVIEW)
        #extension GL_OVR_multiview : enable
        #extension GL_OVR_multiview2 : enable
        #extension GL_OVR_multiview_multisampled_render_to_texture : enable
        layout(num_views = NUM_VIEWS) in;
        #define VIEW_ID gl_ViewID_OVR

        uniform mat4 modelViewProjectionMatrix[NUM_VIEWS];
        #else
        uniform mat4 modelViewProjectionMatrix;
        #endif

        layout(location = 0) in vec3 inPosition;
        layout(location = 1) in vec3 inColor;
        
        out vec3 frgColor;

        void main()
        {
            #if defined(OVR_MULTIVIEW)
            gl_Position = modelViewProjectionMatrix[VIEW_ID] * vec4(inPosition, 1.0);
            #else
            gl_Position = modelViewProjectionMatrix * vec4(inPosition, 1.0);
            #endif
            
            #if defined(OVR_MULTIVIEW_DEBUG) && defined(OVR_MULTIVIEW)
            frgColor = vec3(1.0 - float(VIEW_ID), float(VIEW_ID), 0.0) * inColor;
            #else
            frgColor = inColor;
            #endif

        }
    )";

    const std::string frgShader = CubeGL::receivedFrgShader;

    std::vector<std::string> vtxShaderMacros;
    std::vector<std::string> frgShaderMacros;

#if defined(AK_TARGET_GL_CORE)
    vtxShaderMacros.emplace_back("#version 330 core\n");
    frgShaderMacros.emplace_back("#version 330 core\n");
#else
    vtxShaderMacros.emplace_back("#version 300 es\n");
    frgShaderMacros.emplace_back("#version 300 es\n");
#endif

    if (numActiveViews > 1)
    {
        vtxShaderMacros.emplace_back("#define OVR_MULTIVIEW\n");
        vtxShaderMacros.emplace_back("#define NUM_VIEWS " + std::to_string(numActiveViews) + "\n");

        if (mDebugMultiview)
            vtxShaderMacros.emplace_back("#define OVR_MULTIVIEW_DEBUG\n");
    }

    mShaderProg = create_glsl_program_with_fs_and_vs(vtxShader, frgShader, vtxShaderMacros,
                                                     frgShaderMacros);

    if (mShaderProg == 0)
    {
        return false;
    }

    mShaderProgramMVPMatrixLocation = glGetUniformLocation(mShaderProg, "modelViewProjectionMatrix");

    if (mShaderProgramMVPMatrixLocation == -1)
    {
        std::cerr << "failed to find 'modelViewProjectionMatrix' uniform location\n";
        return false;
    }

    return true;
}

void
CubeGL::putInFront(const VCSE3F& eyeCenterToWorld, float distance, bool rotated)
{
    //multiply cube offset with current viewer pose
    VCSE3F modelToWorld;
    VCSE3F modelOffsetPose{ { 0, 0, 0, 1 }, { 0, 0, -distance } };

    // overwrite rotation to false
    rotated = false;

    const VCVec3F xAxis = { 1, 0, 0 };
    VCQuaternionF xAxisRot;
    VCQuaternionFCreateFromAngleAxis(rotated ? 45.0f : 0.0f, &xAxis, &xAxisRot);

    const VCVec3F yAxis = { 0, 1, 0 };
    VCQuaternionF yAxisRot;
    VCQuaternionFCreateFromAngleAxis(rotated ? 45.0f : 0.0f, &yAxis, &yAxisRot);

    VCQuaternionFMultiplyQuaternionF(&xAxisRot, &yAxisRot, &modelOffsetPose.rotation);

    VCResult res = VCSE3FMultiplySE3F(&eyeCenterToWorld, &modelOffsetPose, &modelToWorld);
    if (VC_FAILURE(res))
        LOG_ERROR("failed to multiply poses");

    // (optional for testing) reset rotational part to align cube to world rotation
    //modelToWorld.rotation = { 0, 0, 0, 1 };

    res = VCSE3FConvertToMatrix(&modelToWorld, &mModelMatrix);
    if (VC_FAILURE(res))
        LOG_ERROR("failed to convert to matrix");
}

void
CubeGL::draw(const RenderParams* renderParams, uint32_t firstView, uint32_t lastView, float global_time)
{
    time = global_time; // set local time with the global time received by the app
    glProgramUniform1f ( mShaderProg, glGetUniformLocation(mShaderProg, "iGlobalTime"), time);
    //glProgramUniform1f ( frgShader, glGetUniformLocation(frgShader, "iGlobalTime"), time) (frgShader instead of mShaderProg)


    assert(firstView <= lastView);
    assert(lastView < kRenderView_Count);

    // Viewport is always the same if single pass rendering.
    const int viewportOriginX = renderParams[firstView].viewPort.origin.x;
    const int viewportOriginY = renderParams[firstView].viewPort.origin.y;
    const int viewportWidth = renderParams[firstView].viewPort.dimensions.width;
    const int viewportHeight = renderParams[firstView].viewPort.dimensions.height;

    glViewport(viewportOriginX, viewportOriginY, viewportWidth, viewportHeight);

    VCMat4F modelViewProj[kRenderView_Count];
    for (uint32_t iView = firstView; iView <= lastView; ++iView)
    {
        const VCMat4F& viewMatrix = renderParams[iView].viewMatrix;
        const VCMat4F& projectionMatrix = renderParams[iView].projectionMatrix;

        VCMat4F modelView;
        VCResult res = VCMat4FMultiplyMat4F(&viewMatrix, &mModelMatrix, &modelView);
        if (VC_FAILURE(res))
            LOG_ERROR("failed to multiply mat");

        res = VCMat4FMultiplyMat4F(&projectionMatrix, &modelView, &modelViewProj[iView]);
        if (VC_FAILURE(res))
            LOG_ERROR("failed to multiply mat");
    }

    glUseProgram(mShaderProg);

    glUniformMatrix4fv(
        mShaderProgramMVPMatrixLocation, (GLsizei)((lastView - firstView) + 1), false, reinterpret_cast<const float*>(&modelViewProj[firstView].m[0][0]));

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}

CubeGL::ColorMode
CubeGL::strToColorMode(const char* str)
{
    if (strcmp(str, "Red") == 0)
    {
        return ColorMode::Red;
    }
    else if (strcmp(str, "White") == 0)
    {
        return ColorMode::White;
    }
    else if (strcmp(str, "RedGreenBlue") == 0)
    {
        return ColorMode::RedGreenBlue;
    }

    LOG_ERROR("unknown ColorMode string: %s", str);
    return ColorMode::Red;
}
