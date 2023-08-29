#include <glm/glm.hpp>
#include "manager/OGL_SceneManager.h"
#include "manager/OGL_BaseApplication.h"
#include "interface/OGL_IPipelineStateManager.h"
#include "dispatchPass/OGL_BRDFIntegrator.h"
#include "drawPass/OGL_OverlayPass.h"
#include "drawPass/OGL_ShadowMapPass.h"
#include "drawPass/OGL_ForwardGeometryPass.h"
#include "sceneGraph/OGL_Scene.h"
#include "sceneGraph/OGL_Mesh.h"
#include "sceneGraph/OGL_Entity.h"
#include "sceneGraph/OGL_Light.h"
#include "sceneGraph/OGL_Animator.h"
#include "sceneGraph/OGL_Transform.h"
#include "sceneGraph/OGL_EditorCamera.h"
#include "platform/windows/OGL_Application.h"
#include "OGL_GraphicsManager.h"

using namespace OGL;

OGL_GraphicsManager::OGL_GraphicsManager() 
{
    mFrames.resize(OGL_GfxConfiguration::kMaxInFlightFrameCount);
}

bool OGL_GraphicsManager::Initialize()
{
    const OGL_GfxConfiguration& conf = mApp->GetConfiguration();

    auto app = static_cast<OGL_BaseApplication*>(mApp);
    auto pipelineStateManager = app->GetModule<OGL_IPipelineStateManager>();

    if (pipelineStateManager)
    {
        mInitPasses.push_back(CreateRef<OGL_BRDFIntegrator>(this, pipelineStateManager));
        mDrawPasses.push_back(CreateRef<OGL_ShadowMapPass>(this, pipelineStateManager));
        mDrawPasses.push_back(CreateRef<OGL_ForwardGeometryPass>(this, pipelineStateManager));
        mDrawPasses.push_back(CreateRef<OGL_OverlayPass>(this, pipelineStateManager));
    }

    InitConstants();

    mInitialize = true;

    return false;
}

void OGL_GraphicsManager::Finalize()
{
    EndScene();
}

void OGL_GraphicsManager::Tick(double dt)
{
    auto app = static_cast<OGL_BaseApplication*>(mApp);
    auto sceneManager = app->GetModule<OGL_SceneManager>();

    if (sceneManager)
    {
        auto rev = sceneManager->GetSceneRevision();
        if (!rev)
            return;

        if (mSceneRevision < rev) 
        {
            EndScene();
            const auto scene = sceneManager->GetSceneForRendering();
            assert(scene);
            BeginScene(*scene);
            mSceneRevision = rev;
        }
    }

    UpdateConstants(dt);

    BeginFrame(mFrames[mFrameIndex]);
    Draw();
    EndFrame(mFrames[mFrameIndex]);

    Present();
}

void OGL_GraphicsManager::Draw()
{
    auto& frame = mFrames[mFrameIndex];

    for (auto& pDispatchPass : mDispatchPasses) 
    {
        pDispatchPass->BeginPass(frame);
        pDispatchPass->Dispatch(frame);
        pDispatchPass->EndPass(frame);
    }

    for (auto& pDrawPass : mDrawPasses) 
    {
        pDrawPass->BeginPass(frame);
        pDrawPass->Draw(frame);
        pDrawPass->EndPass(frame);
    }
}

void OGL_GraphicsManager::BeginScene(const OGL_Scene& scene)
{
    for (const auto& pPass : mInitPasses) 
    {
        pPass->BeginPass(mFrames[0]);
        pPass->Dispatch(mFrames[0]);
        pPass->EndPass(mFrames[0]);
    }

    if (!mFrames[0].frameContext.shadowMap.handler) 
    {
        mFrames[0].frameContext.shadowMap.width = OGL_GfxConfiguration::kShadowMapWidth;
        mFrames[0].frameContext.shadowMap.height = OGL_GfxConfiguration::kShadowMapHeight;
        mFrames[0].frameContext.shadowMap.size = OGL_GfxConfiguration::kMaxShadowMapCount;
        GenerateTextureArray(mFrames[0].frameContext.shadowMap);
    }

    if (!mFrames[0].frameContext.globalShadowMap.handler) 
    {
        mFrames[0].frameContext.globalShadowMap.width = OGL_GfxConfiguration::kShadowMapWidth;
        mFrames[0].frameContext.globalShadowMap.height = OGL_GfxConfiguration::kShadowMapHeight;
        mFrames[0].frameContext.globalShadowMap.size = OGL_GfxConfiguration::kMaxShadowMapCount;
        GenerateTextureArray(mFrames[0].frameContext.globalShadowMap);
    }

    if (!mFrames[0].frameContext.cubeShadowMap.handler) 
    {
        mFrames[0].frameContext.cubeShadowMap.width = OGL_GfxConfiguration::kShadowMapWidth;
        mFrames[0].frameContext.cubeShadowMap.height = OGL_GfxConfiguration::kShadowMapHeight;
        mFrames[0].frameContext.cubeShadowMap.size = OGL_GfxConfiguration::kMaxShadowMapCount;
        GenerateCubemapArray(mFrames[0].frameContext.cubeShadowMap);
    }

    if (scene.mEntitys.size()) 
    {
        InitializeGeometries(scene);
    }
    if (scene.mSkyBox) 
    {
        InitializeSkyBox(scene);
    }

    for (int32_t i = 0; i < OGL_GfxConfiguration::kMaxInFlightFrameCount; i++) 
    {
        mFrames[i] = mFrames[0];
        mFrames[i].frameIndex = i;
    }

    CreateFrameBuffers();
}

void OGL_GraphicsManager::EndScene()
{
    for (auto& texture : mTextures) 
    {
        ReleaseTexture(texture);
    }

    for (auto& frame : mFrames) 
    {
        for (auto& texture : frame.colorTextures) 
        {
            ReleaseTexture(texture);
        }

        ReleaseTexture(frame.depthTexture);
    }
}

void OGL_GraphicsManager::CalculateCameraMatrix()
{
    auto app = static_cast<OGL_Application*>(mApp);
    auto sceneManager = app->GetModule<OGL_SceneManager>();

    if (sceneManager)
    {
        auto& scene = sceneManager->GetSceneForRendering();
        //auto cameraNode = scene->GetFirstCameraNode();
        auto& frameContext = mFrames[mFrameIndex].frameContext;
        if (false)
        {
            //auto cameraObject = scene->GetCamera(cameraNode->GetSceneObjectRef());
            //auto transform = cameraNode->GetCalculatedTransform();
            //frameContext.camPos = glm::vec4(transform[3][0], transform[3][1], transform[3][2], 0.0f);
            //
            //auto camPos = glm::vec3(frameContext.camPos);
            //frameContext.viewMatrix = glm::mat4(glm::mat3(transform)) * glm::lookAt(camPos, camPos + cameraObject->mLookAt, cameraObject->mUp);
        
            //float screenAspect = (float)mCanvasWidth / (float)mCanvasHeight;
            //frameContext.projectionMatrix = glm::perspective(cameraObject->mHorizontalFOV, screenAspect, cameraObject->mClipPlaneNear, cameraObject->mClipPlaneFar);
        }
        else
        {
            auto i = glm::identity<glm::mat4>();
            i = glm::translate(i, glm::vec3(0.0f, -1.5f, -3.0f));

            auto camPos = app->mEditorCamera->mPosition;
            frameContext.viewMatrix = glm::lookAt(camPos, camPos + app->mEditorCamera->mFront, app->mEditorCamera->mUp) * i;

            auto info = app->mEditorCamera->mCameraInfo;
            float screenAspect = (float)info.width / (float)info.height;
            frameContext.projectionMatrix = glm::perspective(glm::radians(app->mEditorCamera->mZoom), screenAspect, info.zNear, info.zFar);
        }
    }
}

void OGL_GraphicsManager::CalculateLights()
{
    auto& frameContext = mFrames[mFrameIndex].frameContext;
    auto& lightInfo = mFrames[mFrameIndex].lightInfo;

    auto app = static_cast<OGL_BaseApplication*>(mApp);
    auto sceneManager = app->GetModule<OGL_SceneManager>();

    const OGL_GfxConfiguration& conf = mApp->GetConfiguration();

    frameContext.numLights = 0;
    frameContext.clipSpaceType = 1;

    if (sceneManager)
    {
        auto& scene = sceneManager->GetSceneForRendering();
        for (const auto& [_, oglEntity] : scene->mEntitys)
        {
            if (!oglEntity->HasComponent<OGL_Light>())
            {
                continue;
            }

            const auto& oglLight = oglEntity->GetComponent<OGL_Light>();
            const auto& oglTransform = oglEntity->GetComponent<OGL_Transform>();
            auto transform = oglTransform.mTransform;
            auto& light = lightInfo.lights[frameContext.numLights];
            light.lightPosition = glm::vec4(transform[3][0], transform[3][1], transform[3][2], 0.0f);
            light.lightDirection = glm::vec4(oglLight.mDirection, 0.0f);

            /*if (lightObject)
            {
                light.lightColor = glm::vec4(lightObject->mColorDiffuse, 1.0);
                light.lightIntensity = lightObject->mAttenuationLinear;
                light.lightCastShadow = false;

                glm::mat4 view;
                glm::mat4 projection;

                float nearClipDistance = 1.0f;
                float farClipDistance = 1000.0f;

                if (lightObject->mType == SceneObjectType::kSceneObjectTypeLightInfi) 
                {
                    light.lightType = LightType::DIRECTIONAL;

                    glm::vec4 target = { 0.0f, 0.0f, 0.0f, 1.0 };

                    auto cameraNode = scene->GetFirstCameraNode();
                    if (cameraNode) 
                    {
                        auto camera = scene->GetCamera(cameraNode->GetSceneObjectRef());
                        farClipDistance = camera->mClipPlaneFar;
                        nearClipDistance = camera->mClipPlaneNear;

                        target[2] = -(0.75f * nearClipDistance + 0.25f * farClipDistance);

                        auto transform = cameraNode->GetCalculatedTransform();
                        glm::translate(transform, glm::vec3(target));
                    }

                    light.lightPosition = target - light.lightDirection * farClipDistance;
                    glm::vec3 position(light.lightPosition);
                    glm::vec3 lookAt(target);
                    glm::vec3 up = { 0.0f, 0.0f, 1.0f };
                    if (abs(light.lightDirection[0]) <= 0.2f && abs(light.lightDirection[1]) <= 0.2f)
                    {
                        up = { 0.1f, 0.1f, 1.0f };
                    }
                    view = glm::lookAt(position, lookAt, up);

                    float sm_half_dist = glm::min(farClipDistance * 0.25f, 800.0f);

                    projection = glm::ortho(-sm_half_dist, sm_half_dist, sm_half_dist, -sm_half_dist, nearClipDistance, farClipDistance + sm_half_dist);

                    light.lightPosition[3] = 0.0f;
                }
                else 
                {
                    glm::vec3 position(light.lightPosition);
                    glm::vec3 lookAt(light.lightPosition + light.lightDirection);
                    glm::vec3 up = { 0.0f, 0.0f, 1.0f };
                    if (abs(light.lightDirection[0]) <= 0.1f && abs(light.lightDirection[1]) <= 0.1f) 
                    {
                        up = { 0.0f, 0.707f, 0.707f };
                    }
                    view = glm::lookAt(position, lookAt, up);

                    if (lightObject->mType == SceneObjectType::kSceneObjectTypeLightSpot)
                    {
                        light.lightType = LightType::SPOT;

                        auto spotLight = dynamic_pointer_cast<SceneObjectSpotLight>(light);
                        float fieldOfView = light.lightAngleAttenCurveParams[0][1] * 2.0f;
                        float screenAspect = 1.0f;

                        projection = glm::perspective(fieldOfView, screenAspect, nearClipDistance, farClipDistance);
                    }
                    else if (lightObject->mType == SceneObjectType::kSceneObjectTypeLightArea)
                    {
                        light.lightType = LightType::AREA;

                        auto areaLight = dynamic_pointer_cast<SceneObjectAreaLight>(light);
                        light.lightSize = areaLight->GetDimension();
                    }
                    else
                    {
                        light.lightType = LightType::AMBIENT;

                        float fieldOfView = glm::pi<float>() / 2.0f;
                        float screenAspect = 1.0f;

                        projection = glm::perspective(fieldOfView, screenAspect, nearClipDistance, farClipDistance);
                    }
                }

                light.lightViewMatrix = view;
                light.lightProjectionMatrix = projection;
                
            }*/

            frameContext.numLights++;
        }
    }
}

void OGL_GraphicsManager::UpdateConstants(double dt)
{
    auto& frame = mFrames[mFrameIndex];

    for (auto& dbc : frame.batchContexts) 
    {
        const auto& transform = dbc->entity->GetComponent<OGL_Transform>();
        dbc->modelMatrix = transform.GetTransform();

        if (dbc->entity->HasComponent<OGL_Animator>())
        {
            auto& animator = dbc->entity->GetComponent<OGL_Animator>();
            animator.Tick(dt);

            for (size_t i = 0; i < animator.mFinalBoneMatrices.size(); i++)
            {
                dbc->finalBonesMatrices[i] = animator.mFinalBoneMatrices[i];
            }
        }
    }

    CalculateCameraMatrix();
    CalculateLights();
}

void OGL_GraphicsManager::CreateFrameBuffers()
{
    auto conf = mApp->GetConfiguration();

    for (int32_t i = 0; i < OGL_GfxConfiguration::kMaxInFlightFrameCount; i++) 
    {
        for (int32_t j = 0; j < mFrames[i].colorTextures.size(); j++) 
        {
            if (j == 0) 
            {
                ReleaseTexture(mFrames[i].colorTextures[0]);
            }
            else 
            {
                if (i == 0) 
                {
                    ReleaseTexture(mFrames[0].colorTextures[j]);
                }
            }
        }

        mFrames[i].colorTextures.clear();
    }

    if (mFrames[0].depthTexture.handler) 
    {
        ReleaseTexture(mFrames[0].depthTexture);
    }

    for (int32_t i = 0; i < OGL_GfxConfiguration::kMaxInFlightFrameCount; i++) 
    {
        Texture2D colorTexture;
        colorTexture.width = mCanvasWidth;
        colorTexture.height = mCanvasHeight;
        colorTexture.pixelFormat = PixelFormat::RGBA8;
        colorTexture.mips = 1;
        colorTexture.samples = 0;

        GenerateTexture(colorTexture);

        mFrames[i].colorTextures.push_back(colorTexture);

        if (i == 0) 
        {
            if (conf.msaaSamples > 1) 
            {
                colorTexture.samples = conf.msaaSamples;
                GenerateTexture(colorTexture);
                mFrames[0].colorTextures.push_back(colorTexture);
                mFrames[0].enableMSAA = false;
            }

            Texture2D depthbuffer;
            depthbuffer.width = mCanvasWidth;
            depthbuffer.height = mCanvasHeight;
            depthbuffer.pixelFormat = PixelFormat::DEPTH;
            depthbuffer.mips = 1;
            depthbuffer.samples = conf.msaaSamples;

            GenerateTexture(depthbuffer);

            mFrames[0].depthTexture = depthbuffer;
        }
        else 
        {
            mFrames[i].colorTextures.push_back(mFrames[0].colorTextures[0]);
            mFrames[i].depthTexture = mFrames[0].depthTexture;
            mFrames[i].enableMSAA = mFrames[0].enableMSAA;
        }
    }
}
