#include "pch.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"
#include "Math/MathUtils.h"
#include "Math/Rect.h"
#include "Pipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace sr
{
    bool Renderer::Initialize(const char* name, int width, int height)
    {
		m_width = width;
		m_height = height;

        SDL_Init(SDL_INIT_VIDEO);

        if (!TTF_Init()) {
            std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
            return false;
        }

        m_window = SDL_CreateWindow(name, width, height, 0);
        if (m_window == nullptr) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }

        SDL_GPUShaderFormat formats = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL;
        m_gpuDevice = SDL_CreateGPUDevice(formats, true, "vulkan");
        if (!m_gpuDevice)
        {
            std::cerr << "Failed to create GPU Device: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }

        SDL_ClaimWindowForGPUDevice(m_gpuDevice, m_window);
        std::cout << SDL_GetGPUDeviceDriver(m_gpuDevice) << std::endl;
        CreateDepthBuffer();
        return true;
    }

    void Renderer::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
    {
        SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    }

    void Renderer::SetColor(float r, float g, float b, float a) const
    {
		SDL_SetRenderDrawColorFloat(m_renderer, r, g, b, a);
    }

    void Renderer::SetColor(const Vector3& v, float a) const
    {
        SDL_SetRenderDrawColorFloat(m_renderer, v.r, v.g, v.b, a);
    }

	void Renderer::Clear() const
	{
		SDL_RenderClear(m_renderer);
	}

    void Renderer::DrawPoint(float x, float y) const
    {
        float cameraX = (m_camActive) ? m_camera.x : 0;
        float cameraY = (m_camActive) ? m_camera.y : 0;

        SDL_RenderPoint(m_renderer, x - (cameraX - m_width * 0.5f), y - (cameraY - m_height * 0.5f));
    }

    void Renderer::DrawFillRect(float x, float y, float width, float height) const
    {
        SDL_FRect rect{ x, y, width, height };
        SDL_RenderFillRect(m_renderer, &rect);
    }

    void Renderer::DrawRect(float x, float y, float width, float height) const
    {
		SDL_FRect rect{ x, y, width, height };
		SDL_RenderRect(m_renderer, &rect);
	
    }

    void Renderer::DrawLine(float x1, float y1, float x2, float y2) const
    {
		SDL_RenderLine( m_renderer, x1, y1, x2, y2 );
    }



    void Renderer::Present() const
    {
        SDL_RenderPresent(m_renderer);
    }

    bool Renderer::BeginFrame()
    {
        m_cmdBuf = SDL_AcquireGPUCommandBuffer(m_gpuDevice);
        if (!m_cmdBuf)
        {
            std::cerr << "Could not acquire command buffer: " << SDL_GetError() << std::endl;
            return false;
        }


        SDL_GPUTexture* swapchainTexture = nullptr;
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(m_cmdBuf, m_window, &swapchainTexture, nullptr, nullptr))
        {
            std::cerr << "Could not acquire swapchain texture: " << SDL_GetError() << std::endl;
            return false;
        }

        if (swapchainTexture != nullptr)
        {
            // configure the color target attachments (This handles clearing the screen)
            SDL_GPUColorTargetInfo color_target_info{};
            color_target_info.texture = swapchainTexture;
            color_target_info.clear_color = SDL_FColor{ 0.2f, 0.2f, 0.2f, 1.0f };
            color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
            color_target_info.store_op = SDL_GPU_STOREOP_STORE;

            // configure the depth target (This handles the depth sorting)
            SDL_GPUDepthStencilTargetInfo depthTargetInfo = {};
            depthTargetInfo.texture = m_depthTexture;
            depthTargetInfo.clear_depth = 1.0f;
            depthTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
            depthTargetInfo.store_op = SDL_GPU_STOREOP_DONT_CARE;
            depthTargetInfo.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE;
            depthTargetInfo.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;

            m_renderPass = SDL_BeginGPURenderPass(m_cmdBuf, &color_target_info, 1, &depthTargetInfo);
            
        }

        return true;
    }

    bool Renderer::EndFrame() const
    {
        SDL_EndGPURenderPass(m_renderPass);
        if (!SDL_SubmitGPUCommandBuffer(m_cmdBuf))
        {
            std::cerr << "Could not submit command buffer: " << SDL_GetError() << std::endl;
            return false;
        }
        

        return true;
    }

    void Renderer::DebugText(float x, float y, const char* text) const
    {
        SDL_RenderDebugText(m_renderer, x, y, text);
    }

    void Renderer::SetPipeline(const Pipeline& pipeline)
    {
        SDL_BindGPUGraphicsPipeline(m_renderPass, pipeline.m_gpuPipeline);
    }

    void Renderer::SetVertexBuffer(const VertexBuffer& vertexBuffer)
    {
        SDL_GPUBufferBinding binding{.buffer = vertexBuffer.m_gpuBuffer,.offset = 0};

        SDL_BindGPUVertexBuffers(m_renderPass, 0, &binding, 1);
    }

    void Renderer::SetIndexBuffer(const IndexBuffer& indexBuffer)
    {
        SDL_GPUBufferBinding binding{ .buffer = indexBuffer.m_gpuBuffer,.offset = 0 };

        SDL_BindGPUIndexBuffer(m_renderPass, &binding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
    }

    void Renderer::Draw(uint32_t vertexCount)
    {
        SDL_DrawGPUPrimitives(m_renderPass, vertexCount, 1, 0, 0);
    }

    void Renderer::DrawIndex(const IndexBuffer& indexBuffer)
    {
        SDL_DrawGPUIndexedPrimitives(m_renderPass, indexBuffer.GetIndexCount(), 1, 0, 0, 0);
    }

    void Renderer::SetVertexUniform(uint32_t slot, const void* data, uint32_t size)
    {
        SDL_PushGPUVertexUniformData(m_cmdBuf, slot, data, size);
    }

    void Renderer::SetFragmentUniform(uint32_t slot, const void* data, uint32_t size)
    {
        SDL_PushGPUFragmentUniformData(m_cmdBuf, slot, data, size);
    }

    void Renderer::SetFragmentTexture(uint32_t slot, const Texture& texture)
    {
        SDL_GPUTextureSamplerBinding binding
        {
            .texture = texture.m_gpuTexture,
            .sampler = texture.m_gpuSampler
        };

        SDL_BindGPUFragmentSamplers(m_renderPass, slot, &binding, 1);
    }

    void Renderer::CreateDepthBuffer(){
        SDL_GPUTextureCreateInfo depthInfo{
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
        .usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
        .width = (uint32_t)m_width,
        .height = (uint32_t)m_height,
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1
        };

        m_depthTexture = SDL_CreateGPUTexture(m_gpuDevice, &depthInfo);
    }

    void Renderer::Quit() const
    {
        TTF_Quit();
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
   
}
