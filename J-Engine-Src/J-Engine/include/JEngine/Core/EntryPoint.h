#pragma once 
#include <JEngine/Core/Application.h>
#include <JEngine/Rendering/Buffers/BufferLayout.h>

extern JEngine::Application* JEngine::createApplication(AppArgs args);

int main(int argc, char** argv)
{
	JEngine::Log::init();
	JE_CORE_TRACE("Initializing J-Engine");

	auto app = JEngine::createApplication({ argc, argv });

	if (app->init()) {
		app->run();
	}

	JE_CORE_TRACE("Releasing J-Engine");
	delete app;
}
