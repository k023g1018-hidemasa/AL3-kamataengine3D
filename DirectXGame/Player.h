#pragma once
#include "Input.h"
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <cassert>
#include <imgui.h>
#include <numbers>

enum class LRDirection {
	kRight,
	kLeft,
};

class Player {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	WorldTransform GetWorldTransform(); 

private:
	WorldTransform worldTransform_;
	/// <summary>
	/// モデル//一旦預かるだけ
	/// </summary>
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	/// <summary>
	/// テクスチャハンドル
	/// </summary>
	uint32_t textureHandle_ = 0u;

	static inline const float kAcceleration = 0.2f;
	static inline const float kAttenuation = 0.5f;
	static inline const float kLimitRunSpeed = 1.0f;
	// 重力加速度（下方向）
	static inline const float kGravityAccleration = 0.8f;
	// 最大落下速度（下方向）
	static inline const float kLimitFallSpeed = 1.2f;
	// ジャンプ初速（上方向）
	static inline const float kJumpAccleration = 1.2f;

	Vector3 velocity_ = {};
	LRDirection lrDirection_ = LRDirection::kRight;
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	// 接地状態フラグ
	bool onGround_ = true;
	//自キャラの速度を取得
	const Vector3& GetVelocity() const { return velocity_; }
	

};
