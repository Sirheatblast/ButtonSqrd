#pragma once

#ifdef BTN_PLATFORM_WINDOWS

extern BtnSqd::Application* BtnSqd::CreateApp();

int main(int argc, char** argv) {
	auto app = BtnSqd::CreateApp();
	try {
		app->RunApp();
		delete app;
	}
	catch(...){

	}
}
#endif