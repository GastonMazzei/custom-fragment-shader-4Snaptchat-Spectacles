/* Copyright 2021 Snap Inc.
 */

#pragma once

#include <AppKit/Delegate.hpp>

#include "CubeGL.hpp"

namespace AppKit {

class CubeDelegate final : public Delegate
{
public:
    CubeDelegate() = default;

    void setColorMode(CubeGL::ColorMode colorMode)
    {
        mColorMode[kRenderLayer_Main] = colorMode;
    }

    void setRepositionToFront(bool enabled) { mRepositionToFront = enabled; }

    void enableHUDLayer(bool enabled) { mEnableHUDLayer = enabled; }

    void onLaunch(Runtime& self, OpenGLConfiguration& glConfig, uint32_t& hints) override;

    void onResume(Runtime& self) override;

    bool onGraphicsResume(Runtime& self, const RenderViewSetup& viewSetup, GLLoadProc loadProc) override;

    void onGraphicsSuspend(Runtime& self) override;

    void onSuspend(Runtime& self) override;

    int onTerminate(Runtime& self) override;

    void onUpdate(Runtime& self, const UpdateParams& updateParams) override;

    void onRenderPoseUpdate(Runtime& self, const RenderPoseUpdateParams& renderUpdateParams) override;

    void onRenderView(Runtime& self, const RenderParams& renderParams) override;

    void onStatisticsUpdate(Runtime& self, const StatisticsUpdateParams& statisticsUpdateParams) override;

    void onInput(Runtime& self, const InputEvent& inputEvent) override;

    void onImageSensorData(Runtime& /* self */,
                           AKImageSensorType /*sensorType*/,
                           const std::vector<ImageSensorData>& /*images*/) override
    {
    }

    void onCalibrationChanged(Runtime& self, const CalibrationSetup& calibrationSetup) override;

    void onVioNotification(Runtime& /*self*/, VioEvent /*event*/) override {}

private:
    CubeGL::ColorMode mColorMode[kRenderLayer_Count] = { CubeGL::ColorMode::White,
                                                         CubeGL::ColorMode::White };
    bool mRotated[kRenderLayer_Count] = { true, false };

    CubeGL::Ptr mCubeGL[kRenderLayer_Count];
    bool mRepositionToFront{ true };
    PCPoseConverter mPoseConverterVisionIMUToRenderEye{};

    bool mEnableHUDLayer{ false };

    bool mSetInitialRenderParams[kRenderView_Count]{};
    AppKit::RenderParams mRenderParams[kRenderLayer_Count][kRenderView_Count]{};

    RenderView mFirstActiveView{};
    RenderView mLastActiveView{};
    AppKit::MultiviewRendering mMultiviewRendering{};
};
}
