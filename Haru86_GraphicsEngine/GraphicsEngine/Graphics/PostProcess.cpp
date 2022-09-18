#include "PostProcess.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include <glew.h>
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"

//instance‚ð’è‹`‚·‚é
PostProcess* PostProcess::instance = nullptr;

void PostProcess::CreateInstance() {
	if (!instance)instance = new PostProcess();
}

PostProcess* PostProcess::GetInstance() {
	if (instance) return instance;
}

void PostProcess::DestroyInstance() {
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

PostProcess::PostProcess():
	m_UsePostProcess(false),
	m_UseSSR(false),
	m_LatePostProcesCallBack([]() {}),
	m_PolygonePPRenderer(nullptr)
{
	m_PolygonePPRenderer = std::make_shared<MeshRendererComponent>(
		std::make_shared<TransformComponent>(),
		PrimitiveType::BOARD,
		RenderingSurfaceType::RASTERIZER,
		shaderlib::ShaderLib::StandardRenderBoard_vert, 
		shaderlib::ShaderLib::PolygonPostProcess_frag
	);
	
	m_LateMeshRenderer = std::make_shared<MeshRendererComponent>(
		std::make_shared<TransformComponent>(),
		PrimitiveType::BOARD,
		RenderingSurfaceType::RASTERIZER,
		shaderlib::ShaderLib::StandardRenderBoard_vert,
		shaderlib::ShaderLib::LatePostProcess_frag
	);
}

void PostProcess::DrawPolygonPostProcess(const std::shared_ptr<Texture>& SrcTexture, const unsigned int& DestBuffer)const {
	// Draw PostProcess Result
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, DestBuffer);
	glViewport(0, 0, static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion), static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	// draw PostProcess Board
	m_PolygonePPRenderer->Draw(GL_TRIANGLES, false, 0, [&]() {
		// Set SrcTexture
		SrcTexture->SetActive(GL_TEXTURE1);
		m_PolygonePPRenderer->m_material->SetTexUniform("_SrcTexture", 1);
	});

	SrcTexture->SetEnactive(GL_TEXTURE1);
}

// SSR
void PostProcess::DrawLatePostProcess(const std::shared_ptr<Texture>& SrcTexture, const unsigned int& DestBuffer)const {
	glBindFramebuffer(GL_FRAMEBUFFER, DestBuffer);
	glViewport(0, 0, static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x), static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	m_LateMeshRenderer->Draw(GL_TRIANGLES, false, 0, [&]() {
		SrcTexture->SetActive(GL_TEXTURE0);
		m_LateMeshRenderer->m_material->SetTexUniform("_SrcTexture", 0);
	
	});
	SrcTexture->SetEnactive(GL_TEXTURE0);
}
