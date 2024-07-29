#include "DeathParticles.h"


void DeathParticles::Initialize(Model* DeathParticlesModel, ViewProjection* viewProjection, const Vector3& position) {
	DeathParticlesModel_ = DeathParticlesModel;
	// 行列を定数バッファに転送//定数バッファ＝グラボ
	viewProjection_ = viewProjection;
	 for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();//whay
		worldTransform.translation_ = position;
	}
	

}
void DeathParticles::Update() {}

void DeathParticles::Draw() {}
