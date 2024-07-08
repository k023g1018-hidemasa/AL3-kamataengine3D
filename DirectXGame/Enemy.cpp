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
	verocity_ = {-kWalkSpeed, 0, 0};
	wolkTimer_ = 0.0f;


}

void Enemy::Update() { 
	worldTransform_.translation_ += verocity_;
	wolkTimer_ += 1.0f / 60.0f;
	//回転アニメーション
	// 
	//22pはこの式であってるのか
	float param= std::sin(wolkTimer_/kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.z = radian *  (180/3.14f);//よくわからん



	//多分アップデートマトリクスがワールド行列の更新？
	worldTransform_.UpdateMatrix();//これを通ったらアフィンとかを通るからアップデートにはこれが必要



}

void Enemy::Draw() {
  enemyModel_->Draw(worldTransform_,*viewProjection_,textureHandle_);


}
