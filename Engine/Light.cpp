#include "pch.h"
#include "Light.h"
#include "Transform.h"
#include "Engine.h"

// ----------------
//		Light
// ----------------

Light::Light() : Component(COMPONENT_TYPE::LIGHT)
{

}

Light::~Light()
{

}

void Light::FinalUpdate()
{
	_lightInfo.position = GetTransform()->GetWorldPosition();
}
