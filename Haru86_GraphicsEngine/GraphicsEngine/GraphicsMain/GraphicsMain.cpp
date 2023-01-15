#include "GraphicsMain.h"
#include <string>
#include "../Graphics/Mesh.h"
#include <vector>
#include <algorithm>
#include "Assets/App/GradDemo/GradDemo.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Sound/SoundPlayer.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Text/TTFFactory.h"
#include <time.h>

#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG

#include "GraphicsEngine/Graphics/ShaderLib.h"

GraphicsMain* GraphicsMain::s_pInstance = nullptr;

void GraphicsMain::Create()
{
	if (!s_pInstance)s_pInstance = new GraphicsMain;
}

void GraphicsMain::Destroy()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

GraphicsMain::GraphicsMain()
	: 
	isRunning(true),
	m_SecondsTime(0.0f),
	m_SecondsTimeOffset(0.0f),
	m_MilliSecondsTime(0.0f),
	m_DeltaTime(0.0f),
	previousTime(0.0f),
	mouseStateBool(false),
#ifdef _DEBUG
	m_ShowDebugLog(false),
#endif // _DEBUG

	renderingTarget(ERerderingTarget::COLOR),
	m_TargetFrameIndex(0),
	m_MainCamera(nullptr),
	m_UsingCamera(nullptr),
	m_SoundPlayer(nullptr),
	m_TTFFactory(nullptr),
	m_ShaderEditor(nullptr),
	m_GroabalLightPosition(nullptr)
{
}

GraphicsMain::~GraphicsMain() {
	GraphicsRenderer::Destroy();
}

bool GraphicsMain::CreateApp() {
	if (glfwInit() == GL_FALSE)return false;
	GraphicsRenderer::Create();
	if (!GraphicsRenderer::GetInstance()->Initialize(500, 500))return false;

	return true;
}

bool GraphicsMain::Initialize() {
	// メモリ確保
	m_App = new app::GradDemo();
	LoadData();
	
	return true;
}

void GraphicsMain::LoadData() {
	//
	m_ShaderEditor = std::make_shared<editor::CShaderEditor>();

	//
	m_SoundPlayer = std::make_shared<sound::SoundPlayer>();

	//
	m_TTFFactory = std::make_shared<text::TTFFactory>();
	if (!m_TTFFactory->Load()) isRunning = false;

	//
	m_App->Start();

	if (m_MainCamera == nullptr) m_MainCamera = std::make_shared<TransformComponent>(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	if (!m_GroabalLightPosition) m_GroabalLightPosition = std::make_shared<TransformComponent>(glm::vec3(10.0f));

	//
	if (m_SecondsTimeOffset != 0.0f && m_SecondsTimeOffset > 0.0f)
	{
		m_SoundPlayer->Skip(m_SecondsTimeOffset);
	}
	else
	{
		m_SoundPlayer->Play();
	}

}

bool GraphicsMain::RunLoop() {
	while (isRunning)
	{
		UpdateTimeline();
		InputProcess();
		Update();
		Draw();
	}

	return false;
}

unsigned int GraphicsMain::GetAppSceneIndex()const
{
	return m_App->GetSceneIndex();
}

void GraphicsMain::UpdateTimeline() {
	m_App->UpdateTimeline();
}

void GraphicsMain::InputProcess() {
	// イベントを発行
	glfwPollEvents();
	glfwSetKeyCallback(GraphicsRenderer::GetInstance()->GetWindow(), key_callback);
}

void GraphicsMain::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		GraphicsMain::GetInstance()->isRunning=false;
	}
#ifdef _DEBUG
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		//Console::Log("Space Push\n");
		GraphicsMain::GetInstance()->m_ShowDebugLog = true;
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		//Console::Log("Space Release\n");
		GraphicsMain::GetInstance()->m_ShowDebugLog = false;
	}
#endif // _DEBUG

}

void GraphicsMain::Update() {
	m_MilliSecondsTime = static_cast<float>(clock()) + m_SecondsTimeOffset*1000.0f;

	// Debug用(コミットしない)
	//m_MilliSecondsTime = glm::mod(static_cast<float>(clock()), (87.0f - 40.0f) * 1000.0f) + 40.0f * 1000.0f;

	m_SecondsTime = m_MilliSecondsTime * 0.001f;
	m_DeltaTime = (m_MilliSecondsTime - previousTime) * 0.001f;

#ifdef _DEBUG
	// FPSの計測と表示(60FPSを基準とする)
	float FPS = 60.0f / (m_DeltaTime * 60.0f);
	Console::Log("[FPS] %f fps / [CurrentTime] %f s\n", FPS, m_SecondsTime);
#endif // _DEBUG


	previousTime = m_MilliSecondsTime;
	if (m_App)m_App->Update();
	m_ShaderEditor->Update();
}

// ここのDrawではカメラ位置を変える
void GraphicsMain::Draw() {
	// 垂直同期の待機時間
	glfwSwapInterval(1);

	// 通常の描画(画面に表示される部分)
	GraphicsRenderer::GetInstance()->Draw(m_MainCamera, true,0, []() {},GraphicsRenderer::GetInstance()->GetScreenSize().x, GraphicsRenderer::GetInstance()->GetScreenSize().y);

	// 演出用ShaderEditorを上書きする
	m_ShaderEditor->Draw();

	//カラーバッファを入れ替える
	glfwSwapBuffers(GraphicsRenderer::GetInstance()->GetWindow());
}
