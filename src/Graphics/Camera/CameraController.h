#pragma once
#include <random>
#include "MathUtils/MathUtils.h"
#include "stdafx.h"
class PerspectiveCamera;

class CameraController
{

public:

	
	float RandomFloat(float min, float max);

	void CameraShake(PerspectiveCamera* cam, float intensity, float duration, float delta);

  
private:
	/************************************New Stuff****************************************************/
	XMVECTOR currCharDirection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR oldCharDirection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR charPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	float charCamDist = 15.0f; // This is the distance between the camera and the character
	/*************************************************************************************************/




    
};

