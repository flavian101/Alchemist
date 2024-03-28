#pragma once
#include "Graphics\Utilis.h"
#include "ShaderManager.h"
#include "models\Transform.h"
#include "MathUtils\MathUtils.h"

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
	XMMATRIX GetTransform()const;
	XMVECTOR GetTranslation()const;
	XMVECTOR GetRotation()const;
	XMVECTOR GetScale()const;
	void setTranslation(const XMFLOAT3& translation);
	void setRotation(const XMFLOAT3& rotation);
	void setScale(const XMFLOAT3& scale);


	ShaderManager GetShadermanager();
	
private:
	Graphics& m_graphics;


protected:
	ShaderManager m_shaderManager;
	Transform m_transform;

};

