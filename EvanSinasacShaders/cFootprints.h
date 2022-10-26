#pragma once

#include "iEntity.h"

class cFootprints : public iEntity
{
public:
	cFootprints();
	cFootprints(glm::vec3 startPos);
	virtual ~cFootprints();

	virtual void Update(float dt);

	void Activate();

	void SetTextures(int typeOfTexture);
	void SetOrientation(glm::vec3 newOri);
protected:

private:
	bool active;
	float timer;
};