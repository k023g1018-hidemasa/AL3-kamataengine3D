#pragma once
#include"Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include"Vector3.h"
#include<numbers>
#include<cassert>
#include "Input.h"

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
	Vector3 velocity_ = {};





};









