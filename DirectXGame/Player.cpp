#include "Player.h"


Player::Player() {}

Player::~Player() {} // 未定義、参照されてないは関数の作り忘れ

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
//	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	// 行列を定数バッファに転送//定数バッファ＝グラボ
	viewProjection_ = viewProjection;
   
	worldTransform_.translation_ = position;
	//初期回転角の指定//Y軸を90度右に回転、2π
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;



}
void Player::Update() {
	worldTransform_.TransferMatrix();

	if (Input::GetInstance()->PushKey(DIK_RIGHT)|| Input::GetInstance()->PushKey(DIK_LEFT)) {
	//左右加速
	    	Vector3 acceleration = {};
	    if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			    acceleration.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			acceleration.x -= kAcceleration;
		
		}
	//加速原則
		velocity_ += acceleration;
	
	} else {
	//火入力時は移動減衰を×
		velocity_.x *= (1.0f - kAttenuation);
	    if (velocity_.x = velocity_.x < 0.001f) {
			velocity_.x = 0;
	    }
	}

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
	}
