#include "Player.h"
#include<cassert>
//基本装備は＜＞自前の｜｜エンジンのは””


void Player::Initialize(Model* model, uint32_t textureHandle) { 

	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

}



