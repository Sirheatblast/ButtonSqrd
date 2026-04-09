#pragma once
#include<ButtonSqrd.h>

class GameEditLayer : public BtnSqd::Layer {
	GameEditLayer() : editorFrameBuffer(nullptr) {}

	void  OnUpdate() override;

	void  OnAttach() override;

	void  OnDetach() override;

	void  OnEvent(BtnSqd::Event* e) override;
private:
	BtnSqd::FrameBuffer* editorFrameBuffer;
};