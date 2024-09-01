#include "Goal.h"

Goal::Goal() {}

Goal::~Goal() {}

void Goal::Initialize(Model* enemyModel, ViewProjection* viewProjection, const Vector3& position) {
	assert(enemyModel);
	enemyModel_ = enemyModel;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	worldTransform_.translation_ = position; // ここで場所を代入している

}

void Goal::Update() {}

void Goal::Draw() {
	goalModel_->Draw(worldTransform_, *viewProjection_, textureHandle_);


}
