#pragma once

#include <glm/glm.hpp>

namespace Ambient
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        void SetProjection(float left, float right, float bottom, float top);

        inline const glm::vec3& GetPosition()
        {
            return m_Position;
        }

        inline const float GetRotation()
        {
            return m_Rotation;
        }

        inline void SetPosition(const glm::vec3& position)
        {
            m_Position = position;
            RecalculateViewMatrix();
        }

        inline void SetRotation(float rotation)
        {
            m_Rotation = rotation;
            RecalculateViewMatrix();
        }

        inline const glm::mat4& GetProjectionMatrix() const
        {
            return m_ProjectionMatrix;
        }

        inline const glm::mat4& GetViewMatrix() const
        {
            return m_ViewMatrix;
        }

        inline const glm::mat4& GetViewProjectionMatrix() const
        {
            return m_ViewProjectionMatrix;
        }

    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;

        // Cache the view projection matrix
        glm::mat4 m_ViewProjectionMatrix;

        // TODO Move to OrthographicCameraController
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        float m_Rotation = 0.0f;

        void RecalculateViewMatrix();
    };
} // namespace Ambient
