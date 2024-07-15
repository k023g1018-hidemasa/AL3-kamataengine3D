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
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	//速度の設定
	verocity_ = {-kWalkSpeed, 0, 0};
	wolkTimer_ = 0.0f;


}

void Enemy::Update() { 
	//worldTransform_.translation_ += verocity_;
	//wolkTimer_ += 1.0f / 60.0f;
	////回転アニメーション
	//// 
	////22pはこの式であってるのか
	//float param= std::sin((2*3.14f)*wolkTimer_/kWalkMotionTime);
	//float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	//worldTransform_.rotation_.x = radian * 3.14f/360.0f;//よくわからん


	////多分アップデートマトリクスがワールド行列の更新？
	worldTransform_.UpdateMatrix();//これを通ったらアフィンとかを通るからアップデートにはこれが必要



}

void Enemy::Draw() {
  enemyModel_->Draw(worldTransform_,*viewProjection_,textureHandle_);


}
Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;//リターンの値は上で作ったやつを返してやる
}

void Enemy::OnCollision(const Player* player) {
     (void)player;
}
