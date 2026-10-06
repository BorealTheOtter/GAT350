#include "pch.h"
#include "TexFrames.h"
#include "Serialization/Json.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture.h"

namespace sr {
    bool TexFrames::Load(const std::string& filename, Renderer& renderer)
    {
        json::document_t doc;

        if (!json::Load(filename, doc)) {
            std::cerr << "Could not load Texture Frames: " << filename << std::endl;
            return false;
        }

        std::string texture_name;
        JSON_READ(doc, texture_name);

        if (texture_name.empty()) return false;

        m_texture = Resources().Get<Texture>(texture_name, renderer);

        if (!m_texture) return false;

        JSON_READ_NAME(doc, "columns", m_cols);
        JSON_READ_NAME(doc, "rows", m_rows);
        JSON_READ_NAME(doc, "start_frame", m_startFrame);
        JSON_READ_NAME(doc, "total_frames", m_totalFrames);

        if (m_cols <= 0 || m_rows == 0) { 
            std::cerr << "Rows or Columns is 0" << std::endl;  
            return false; 
        }

        Vector2 textureSize = m_texture->GetSize();

        m_cellSize = textureSize / Vector2{ (float)m_cols, (float)m_rows };

        return true;
    }

    Rect TexFrames::GetFrameRect(unsigned int frame)
    {
        if (frame >= m_totalFrames) {
            std::cerr << "frame " << frame <<" is out of bounds, defaulting to 0" << std::endl;
            frame = 0;
        }

            int currFrame = m_startFrame + frame;
            int column = currFrame % m_cols;
            int row = currFrame / m_cols;

            float x = column * m_cellSize.x;
            float y = row * m_cellSize.y;
            return Rect{ x, y, m_cellSize.x, m_cellSize.y };
        
    }
}
