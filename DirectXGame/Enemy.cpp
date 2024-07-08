#include "Enemy.h"
#include"assert.h"
Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(Model* enemyModel, ViewProjection* viewProjection, const Vector3& position) {
	assert(enemyModel);
	enemyModel_ = enemyModel;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	worldTransform_.translation_ = position;//ここで場所を代入している

	//速度の設定
	verocity_ = {kWalkSpeed, 0, 0};

}

void Enemy::Update() { 
	worldTransform_.UpdateMatrix();//これを通ったらアフィンとかを通るからアップデートにはこれが必要

	worldTransform_.translation_ += verocity_;
}

void Enemy::Draw() {
  enemyModel_->Draw(worldTransform_,*viewProjection_,textureHandle_);


}
