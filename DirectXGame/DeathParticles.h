#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include<array>
#include"WorldTransform.h"

class DeathParticles {
public:
	
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	Model* DeathParticlesModel_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	//パテの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransforms_;
};