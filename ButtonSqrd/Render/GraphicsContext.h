#pragma once

namespace BtnSqd {
	class GraphicsContext {
	public:
		virtual void SwapBuffers() = 0;
		virtual void Init() = 0;
		virtual void InitImgui() = 0;
		virtual void DrawImGuiToApi() = 0;
		virtual void ResizeCallback(unsigned int width,unsigned int height) = 0;
	};
}