#pragma once
#include <SDL3/SDL.h>
#include<SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "Math/Transform.h"
#include "Renderer/Model.h"
#include "Math/Vector3.h"


namespace sr
{
	class Renderer
	{
	public:
		bool Initialize(const char* name, int width, int height);
		void Quit() const;

		void Clear() const;
		void Present() const;

		void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) const;
		void SetColor(float r, float g, float b, float a = 1.0f) const;
		void SetColor(const Vector3& v, float a = 1.0f) const;

		void DrawPoint(float x, float y) const;
		void DrawFillRect(float x, float y, float width, float height) const; 
		void DrawRect(float x, float y, float width, float height) const;
		void DrawLine(float x1, float y1, float x2, float y2) const;

		void DrawModel(const class Model& m, const Transform& t) const;
		
		
		void DebugText(float x, float y, const char* text) const;

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }
		
		void DrawTexture(const class Texture& texture, Transform t, bool flipH = false, const Vector2& origin = {0.5f,0.5f}) const;
		void DrawTexture(const class Texture& texture,const struct Rect& source, Transform t, bool flipH = false, const Vector2& origin = { 0.5f,0.5f }) const;

		void SetCamera(const Vector2& camera) { m_camera = camera; }
		void EnableCamera(bool active = true) { m_camActive = active; }
	private:
		friend class Text;
		friend class Texture;

		SDL_Window* m_window = nullptr;
		SDL_Renderer* m_renderer = nullptr;

		Vector2 m_camera;
		bool m_camActive = true;

		int m_width = 0;
		int m_height = 0;
	};
}

