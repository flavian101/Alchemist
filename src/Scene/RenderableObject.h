#pragma once
#include "Graphics\Utilis.h"
#include "ShaderManager.h"
#include "models\Transform.h"
#include "MathUtils\MathUtils.h"
#include <string>

/// <summary>
/// set the pixel constant buffer
/// </summary>

class RenderableObject
{
public:
	RenderableObject(const std::string& name,Graphics& g, ShaderManager shaderManager);
	~RenderableObject();
	virtual void Update(float time);
	virtual void Render();
	XMMATRIX GetTransform()const;
	XMVECTOR GetTranslation()const;
	XMVECTOR GetRotation()const;
	XMVECTOR GetScale()const;
	void setTranslation(const XMFLOAT3& translation);
	void setRotation(const XMFLOAT4& rotation);
	void setScale(const XMFLOAT3& scale);


	ShaderManager GetShadermanager()const;
	std::string getName()const;
	void setName(const std::string& name);
	virtual void controlWindow();
	
private:
	Graphics& m_graphics;
	ShaderManager m_shaderManager;
	Transform m_transform;


protected:
	std::string m_name;

};

