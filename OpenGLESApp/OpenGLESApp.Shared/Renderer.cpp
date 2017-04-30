#include "Renderer.h"

using namespace OpenGLESRenderer;

Renderer::Renderer(const std::shared_ptr<GameEngine::GameManager>& gameManager) :
	m_gameManager(gameManager)
{	
}

void Renderer::Initialize(float width, float height)
{
	// Initialize GL state.
	glDisable(GL_DITHER);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glClearColor(1.0f, 0.41f, 0.71f, 1.0f);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, width, height);
	auto ratio = static_cast<GLfloat>(width) / height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustumf(-ratio, ratio, -1, 1, 1, 10);

	// TODO subsribe to events and create 
}
void Renderer::Shutdown()
{
}

void Renderer::FrameUpdate(const Common::Timer& timer)
{
}
void Renderer::Render(const Common::Timer& timer) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
}

void Renderer::OnBoxCreated(void* sender, const GameEngine::MeshComponent<GameEngine::BoxGeometry>& box)
{
}
