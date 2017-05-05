#include "Renderer.h"

using namespace OpenGLESRenderer;

float m_rotation = 0.0f;

Renderer::Renderer(const std::shared_ptr<GameEngine::GameManager>& gameManager) :
	m_gameManager(gameManager)
{	
}
Renderer::~Renderer()
{
	Shutdown();
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

	m_gameManager->OnBoxCreated() += Common::MemberCallback(*this, &Renderer::OnBoxCreated);
}
void Renderer::Shutdown()
{
	m_gameManager->OnBoxCreated() -= Common::MemberCallback(*this, &Renderer::OnBoxCreated);
}

void Renderer::FrameUpdate(const Common::Timer& timer)
{
	m_rotation += 1.f;
}
void Renderer::Render(const Common::Timer& timer) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3.0f);
	glRotatef(m_rotation * 0.25f, 1, 0, 0);  // X
	glRotatef(m_rotation, 0, 1, 0);          // Y

	for(const auto& renderItem : m_colorRenderItems)
		renderItem.Draw();

}

void Renderer::OnBoxCreated(void* sender, const GameEngine::MeshComponent<GameEngine::BoxGeometry>& box)
{
	m_colorRenderItems.emplace_back(ColorRenderItem(box.Geometry()->GenerateMeshData<TueMeshData>()));
}
