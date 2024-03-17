#pragma once
#include "Graphics\Utilis.h"
#include "ShaderManager.h"
#include "models\Transform.h"

/// <summary>
/// set the pixel constant buffer
/// </summary>

class RenderableObject
{
public:
	RenderableObject(Graphics& g, ShaderManager shaderManager);
	~RenderableObject();
	virtual void Update(float time);
	virtual void Render();
	XMMATRIX GetTransform();
	void setTranslation(XMFLOAT3& translation);
	void setRotation(XMFLOAT3& rotation);
	void setScale(XMFLOAT3& scale);


	ShaderManager GetShadermanager();
	
private:
	Graphics& m_graphics;


protected:
	ShaderManager m_shaderManager;
	Transform m_transform;

};

