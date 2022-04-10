/* Copyright 2021 Snap Inc.
 */

#include "CubeDelegate.hpp"
#include "glad/glad.h"
#include <AppKit/Utils.hpp>
#include <AppKit/Logging.hpp>
#include <AppKit/Macros.hpp>
#include <VisionCore/VCMath.h>
#include <array>
#include <memory>

namespace AppKit {



void
CubeDelegate::onLaunch(Runtime& /* self */,
                       OpenGLConfiguration& glConfig,
                       uint32_t& hints)
{
    LOG_INFO("onLaunch");
    glConfig.majorVersion = 3;
    glConfig.minorVersion = 2;
    glConfig.flags = 0;
    glConfig.glMSAASamples = 4;

    if (mEnableHUDLayer)
    {
        hints |= kRuntimeHints_CompositorLayer_Main;
        hints |= kRuntimeHints_CompositorLayer_HUD;
    }
}

int
CubeDelegate::onTerminate(Runtime& /* self */)
{
    return 0;
}

void
CubeDelegate::onResume(Runtime& /* self */)
{
    LOG_INFO("onResume");
    mRepositionToFront = true;
    // TODO: restore GL stuff here properly? Can we maybe merge this with one of the other
    // app-lifecycle calls?
}

bool
CubeDelegate::onGraphicsResume(Runtime& /* self */,
                               const RenderViewSetup& viewSetup,
                               GLLoadProc loadProc)
{
    LOG_INFO("onGraphicsResume");
    if (loadProc == nullptr)
    {
        LOG_ERROR("GL load proc is null");
        return false;
    }

    // initialise glad
    if (gladLoadGLES2Loader(loadProc) == 0)
    {
        LOG_ERROR("failed to initial GLAD GL bindings");
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    uint32_t activeViewCount = 1;
    if (viewSetup.multiviewRendering == kMultiviewRendering_SinglePass)
    {
        bool firstFound = false;
        for (uint32_t iLayer = 0; iLayer < kRenderLayer_Count; ++iLayer)
        {
            for (uint32_t iView = 0; iView < kRenderView_Count; ++iView)
            {
                if (viewSetup.flags[iLayer][iView] & kRenderViewSetupFlags_IsEnabled)
                {
                    if (!firstFound)
                    {
                        mFirstActiveView = static_cast<RenderView>(iView);
                        firstFound = true;
                    }
                    mLastActiveView = static_cast<RenderView>(iView);
                    activeViewCount = static_cast<uint32_t>((mLastActiveView - mFirstActiveView) + 1);
                }
            }
        }
        if (!firstFound)
            LOG_ERROR("could not find active views");
    }

    mMultiviewRendering = viewSetup.multiviewRendering;

    // this has all the GL code to draw a cube
    lastActiveViewCount = activeViewCount;

    update_shader();
    return creator();
}


void
CubeDelegate::update_shader()
{
	// Define the string
	std::string receivedString = "";
	

	// Receive data       VENISE model TPLink 2 for my PC's MAC address  is 192.168.2.102:5000
	//                                          a new paired machine should probably be 
	//                                                                      192.168.2.103:5000
	//                                          [ please ask Mr. Ladeveuze for help ]
	//
	//                    ECLA Campus Massy-Palaiseau for my MAC address is 212.221.45.1:5000
	//
	std::string addr =  "212.221.45.1:5000"; // "127.0.0.1:5000";
	std::string command = "curl --header \"Content-Type: application/json\" \
		                            --request POST  http://" + addr + "/latest_shader";
	const int MAXLENGTH = 200000;
	const char * cmd = command.c_str();
	std::array<char, MAXLENGTH> buffer;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		std::string local_data = static_cast<std::string>(buffer.data());
		receivedString += local_data;
	}

	// other constraints?
	global_time=0;

	// rewrite receivedFrgShader
	CubeGL::receivedFrgShader = receivedString;

	// Log debug
	LOG_INFO("Received string is");
	LOG_INFO(receivedString.c_str());
}


bool
CubeDelegate::creator()
{
    mCubeGL[kRenderLayer_Main] = CubeGL::create(0.25f, mColorMode[kRenderLayer_Main], lastActiveViewCount);

    if (!mCubeGL[kRenderLayer_Main])
    {
        LOG_ERROR("failed to create OpenGL cube");
        return false;
    }

    if (mEnableHUDLayer)
    {
        mCubeGL[kRenderLayer_HUD] = CubeGL::create(0.08f, mColorMode[kRenderLayer_HUD], lastActiveViewCount);

        if (!mCubeGL[kRenderLayer_HUD])
        {
            LOG_ERROR("failed to create OpenGL cube");
            return false;
        }
    }
    return true;
}

void
CubeDelegate::onGraphicsSuspend(Runtime& /* self */)
{
    LOG_INFO("onGraphicsSuspend");
    destroyer();
}


void
CubeDelegate::destroyer()
{
    for (size_t layer = kRenderLayer_Main; layer < kRenderLayer_Count; ++layer)
        mCubeGL[layer] = nullptr;
}


void
CubeDelegate::onSuspend(Runtime& /* self */)
{
    LOG_INFO("onSuspend");
}

void
CubeDelegate::onUpdate(Runtime& /* self */, const UpdateParams& /* updateParams */)
{
	// Update clock
	//t1 = Clock::now();
	//float dt = std::chrono::duration_cast<std::chrono::duration<float, std::chrono::seconds::period>>(t1 - t0);
        global_time += dt;
}

void
CubeDelegate::onRenderPoseUpdate(Runtime& /*self*/, const RenderPoseUpdateParams& renderUpdateParams)
{
    if (mRepositionToFront)
    {
        //reposition cube in front or viewer by using eye-center pose:

        //convert vision vio pose to render eye-center pose
        VCSE3D poseRenderEyeCenterToWorld;
        PCResult pcRes = PCPoseConverterApply(&mPoseConverterVisionIMUToRenderEye, &renderUpdateParams.poseVioImuToWorld, &poseRenderEyeCenterToWorld);
        if (PC_FAILURE(pcRes))
        {
            LOG_ERROR("failed to apply pose converter to imu pose. pcRes=%d", pcRes);
        }

        //cast pose from double to float
        VCSE3F poseRenderEyeCenterToWorldF;
        VCResult res = VCSE3DCastToFloat(&poseRenderEyeCenterToWorld, &poseRenderEyeCenterToWorldF);
        if (VC_FAILURE(res))
            LOG_ERROR("failed to cast pose to float");

        static const float distance = 2.0f; // meters
        mCubeGL[kRenderLayer_Main]->putInFront(poseRenderEyeCenterToWorldF, distance, mRotated[kRenderLayer_Main]);

        if (mEnableHUDLayer)
        {
            mCubeGL[kRenderLayer_HUD]->putInFront(poseRenderEyeCenterToWorldF, distance, mRotated[kRenderLayer_HUD]);
            memset(mSetInitialRenderParams, true, sizeof(bool) * kRenderView_Count);
        }

        mRepositionToFront = false; // set this to false for reposition-on-tap behaviour
    }
}

void
CubeDelegate::onRenderView(Runtime& /* self */, const RenderParams& renderParams)
{
    const auto layer = renderParams.layer;
    const auto view = renderParams.view;

    if (layer == kRenderLayer_Main)
    {
        mRenderParams[layer][view] = renderParams;
    }
    else if (layer == kRenderLayer_HUD)
    {
        // if single pass rendering is used only last view is active one so we have to cache data.
        if (mSetInitialRenderParams[view])
        {
            mRenderParams[layer][view] = renderParams;
            mSetInitialRenderParams[view] = false;
        }
    }

    // finally, pass on all stereo data (viewMat/projMat/viewport) to the renderer
    if (mCubeGL[layer] != nullptr)
    {
        switch (mMultiviewRendering)
        {
        case kMultiviewRendering_MultiPass:
            mCubeGL[layer]->draw(mRenderParams[layer], view, view, global_time);
            break;
        case kMultiviewRendering_SinglePass:
            if (mLastActiveView == view)
                mCubeGL[layer]->draw(mRenderParams[layer], mFirstActiveView, mLastActiveView, global_time);
            break;
        default:
            LOG_ERROR("invalid multiview mode");
        }
    }
}

void
CubeDelegate::onStatisticsUpdate(Runtime& /* self */, const StatisticsUpdateParams& /* statisticsUpdateParams */)
{
    // LOG_INFO("onStatisticsUpdate");
}

void
CubeDelegate::onInput(Runtime& /* self */, const InputEvent& inputEvent)
{
    if (inputEvent.source == InputEventSource::kInputEventSource_Keyboard)
    {
        if (inputEvent.key.code == 'd' && inputEvent.action == InputEventAction::kInputEventAction_KeyUp)
        {
            LOG_DEBUG("KeyBoard, repositioning scene");
            mRepositionToFront = true;
        }
    }
    else if (inputEvent.source == InputEventSource::kInputEventSource_TouchPad)
    {
        if (inputEvent.action == InputEventAction::kInputEventAction_GestureTap)
        {
            LOG_DEBUG("TouchPad tap event, repositioning scene");
            destroyer();
            update_shader();
            creator();
            mRepositionToFront = true;
        }
        else if (inputEvent.action == InputEventAction::kInputEventAction_GestureDragMove)
        {
            LOG_DEBUG("TouchPad drag coordinate: %.2f x %.2f", inputEvent.motion.x, inputEvent.motion.y);
        }
    }
}

void
CubeDelegate::onCalibrationChanged(Runtime& /*self*/, const CalibrationSetup& calibrationSetup)
{
    LOG_INFO("onCalibrationChanged");

    PCRenderCalibrationManager renderCalibrationManager = calibrationSetup.renderCalibration[kRenderLayer_Main];

    // Use renderCalibrationManager e.g. to get pose converters.
    // Important: This instance of the  render calibration manager is not valid outside the scope of this function!

    //Get pose converter to convert vio pose in vision space to eye-center pose in render space:
    PCResult res = PCRenderCalibrationManagerGetPoseConverter(renderCalibrationManager,
                                                              kPCCSVision_VIOIMU,
                                                              kPCCSVision_WorldVIO,
                                                              kPCCSRender_EyeCenter,
                                                              kPCCSRender_WorldVIO,
                                                              &mPoseConverterVisionIMUToRenderEye);
    if (PC_FAILURE(res))
    {
        LOG_ERROR("failed to get pose converter. res=%d", res);
    }
}

Delegate*
createAppKitDelegate(int argc, const char** argv)
{
    const char* colorMode = Utils::getStrArg("--colorMode", argc, argv);
    bool noReposition = Utils::hasArg("--noReposition", argc, argv);
    bool enableHUDLayer = Utils::hasArg("--hud-layer", argc, argv);

    auto d = new CubeDelegate();

    if (colorMode)
        d->setColorMode(CubeGL::strToColorMode(colorMode));

    if (noReposition)
        d->setRepositionToFront(false);
    if (enableHUDLayer)
        d->enableHUDLayer(true);

    return d;
}
void
destroyAppKitDelegate(Delegate* d)
{
    delete d;
}
}
