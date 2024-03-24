#pragma once
#include "Graphics\Camera\PerspectiveCamera.h"

class FreeLook: public PerspectiveCamera
{
	FreeLook();

	void Update(float delta) override;

};

