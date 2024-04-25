#include "Player.h"
#include<cassert>
//基本装備は＜＞自前の｜｜エンジンのは””


Player::Player() {}

Player::~Player() {}//未定義、参照されてないは関数の作り忘れ

void Player::Initialize(Model* model, uint32_t textureHandle,ViewProjection* viewProjection) { 

	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	//行列を定数バッファに転送//定数バッファ＝グラボ
	viewProjection_ = viewProjection;


}

void Player::Update() {


	worldTransform_.TransferMatrix();
}

void Player::Draw() { model_->Draw(
	worldTransform_, 
	*viewProjection_,
	textureHandle_
);




}



