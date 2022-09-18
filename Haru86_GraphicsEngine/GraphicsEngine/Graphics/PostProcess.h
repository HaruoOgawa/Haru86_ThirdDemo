#pragma once
#include <memory>
#include <functional>

class Texture;
class MeshRendererComponent;

class PostProcess
{
	std::shared_ptr<MeshRendererComponent> m_PolygonePPRenderer;
public:
	static void CreateInstance();
	static PostProcess* GetInstance();
	static void DestroyInstance();
	
	PostProcess();
	virtual ~PostProcess()=default;
	void DrawPolygonPostProcess(const std::shared_ptr<Texture>& SrcTexture,const unsigned int& DestBuffer)const;
	void DrawLatePostProcess(const std::shared_ptr<Texture>& SrcTexture, const unsigned int& DestBuffer)const;
	
	//
	bool m_UsePostProcess;
	std::shared_ptr<MeshRendererComponent> m_LateMeshRenderer;
	std::function<void(void)> m_LatePostProcesCallBack;

	// SSR
	bool m_UseSSR;
protected:
	static PostProcess* instance;
};

