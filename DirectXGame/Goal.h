#pragma once
#include"Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include"numbers"
class Goal {
public:
Goal();
~Goal();

void Initialize(Model* enemyModel, ViewProjection* viewProjection, const Vector3& position);

void Update();

void Draw();
	
private:

		Model* goalModel_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;

	uint32_t textureHandle_ = 0u;

};

