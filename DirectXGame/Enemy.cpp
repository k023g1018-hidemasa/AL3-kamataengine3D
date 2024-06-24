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

}

void Enemy::Update() { 
	worldTransform_.UpdateMatrix();//これを通ったらアフィンとかを通るからアップデートにはこれが必要
}

void Enemy::Draw() {
  enemyModel_->Draw(worldTransform_,*viewProjection_,textureHandle_);


}
