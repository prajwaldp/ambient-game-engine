#pragma once

#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Types.h"

#include <glm/glm.hpp>
#include <memory>

namespace Ambient
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0,
            OpenGL = 1
        };

        virtual void Init() = 0;

        virtual void SetClearColor() = 0;

        virtual void Clear() = 0;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count) = 0;

        inline static API GetAPI()
        {
            return s_API;
        }

    private:
        static API s_API;
    };

/**
 * A static wrapper for OpenGL commands
 **/
    class RenderCommand
    {
    public:
        inline static void Init()
        {
            s_RendererAPI->Init();
        }

        inline static void SetClearColor()
        {
            s_RendererAPI->SetClearColor();
        }

        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }

        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        inline static void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count = 0)
        {
            s_RendererAPI->DrawIndexed(vertex_array, index_count);
        }

    private:
        static RendererAPI* s_RendererAPI;
    };

    class Renderer
    {

    public:
        static void Init();

        static void OnWindowResize(uint32_t width, uint32_t height);

        static void BeginScene(OrthographicCamera& camera);

        static void EndScene();

        static void Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray,
                const glm::mat4 transformMatrix = glm::mat4(1.0f));

        static void Flush();

        // Inline functions

        inline static RendererAPI::API GetAPI()
        {
            return RendererAPI::GetAPI();
        }

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* m_SceneData;
    };

} // namespace Ambient
