#pragma once
#define _USE_MATH_DEFINES
#include<cmath>
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
	uint32_t textureHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;
	//パテの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransforms_;//	これがトランスフォームを8つ作ってることになる

	//存続時間<秒>
	static inline const float kDuration = 3.0f;
	static inline const float kDuration = 30.0f;
	//移動の速さ
	static inline const float kSpeed = 0.05f;
	static inline const float kSpeed = 0.35f;
	//分割した一個分の角度
	static inline const float kAngleUnit = 3.141592f * 2 / kNumParticles;//PI使えん

	//終了フラグ
	bool isFinished_ = false;
	//経過時間カウント
	float counter_=0.0f;
	//色変化オブジェクト
	ObjectColor objectColor_;
	//色の数値
	Vector4 color_;
	float colorG;

};