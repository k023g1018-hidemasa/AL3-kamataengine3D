#include "Goal.h"

Goal::Goal() {}

Goal::~Goal() {}

void Goal::Initialize(Model* goalModel, ViewProjection* viewProjection, const Vector3& position) {
	assert(goalModel);
	goalModel_ = goalModel;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	worldTransform_.translation_ = position; // ここで場所を代入している
	worldTransform_.UpdateMatrix();

}

void Goal::Update() {

worldTransform_.UpdateMatrix();
}

void Goal::Draw() {
	goalModel_->Draw(worldTransform_, *viewProjection_, textureHandle_);


}
