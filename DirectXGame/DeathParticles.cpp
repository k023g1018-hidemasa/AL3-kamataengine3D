#include "DeathParticles.h"
#include <iostream>
#include <algorithm>

void DeathParticles::Initialize(Model* DeathParticlesModel, ViewProjection* viewProjection, const Vector3& position) {
	DeathParticlesModel_ = DeathParticlesModel;
	// 行列を定数バッファに転送//定数バッファ＝グラボ
	viewProjection_ = viewProjection;
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize(); // whay
		worldTransform.translation_ = position;
	}
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
	colorG = 1.0f;
}
void DeathParticles::Update() {

	if (isFinished_) {
		return;
	}
	// ワールド返還の更新12ｐ
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}

	for (uint32_t i = 0; i < kNumParticles; ++i) {
		// 基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};
		// 回転角の計算
		float angle = kAngleUnit * i;
		// Z軸回り回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		// 基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		// 移動処理
		worldTransforms_[i].translation_ += velocity;
	}
	// カウンターを１フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;
	// 存続時間の上限に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		// 終了扱いにする
		isFinished_ = true;
	}
	colorG -= 0.00f;
	color_.w = std::clamp(colorG, 0.0f, 1.0f); // 都度変更
	//色変更オブジェに色の数値おwせってり
	objectColor_.SetColor(color_);
	//色変更オブジェクトをVramに転送
	objectColor_.TransferMatrix();
}

void DeathParticles::Draw() {
	// モデルの描画13ｐ

	if (isFinished_) {
		return;
	}
	for (auto& worldTransform : worldTransforms_) {
		DeathParticlesModel_->Draw(worldTransform, *viewProjection_, &objectColor_);
	}
}
