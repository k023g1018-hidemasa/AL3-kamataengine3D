#define NOMINMAX
#include "Player.h"
#include "MapchipField.h"
#include <DebugText.h>
#include <algorithm>

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
	// 初期回転角の指定//Y軸を90度右に回転、2π
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;


}
void Player::Update() {

	// 衝突判定

	// 結果反転して移動

	// 天井の処理

	// 壁の処理

	// 接地状態切り替え

	// 旋回制御

	// 行列計算

	// 接地フラグ
	bool landing = false;
	if (onGround_) {
		FuncMove(); // こっれでええんかな
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.move = velocity_;
	CollisionMap(collisionMapInfo); // 先生の書き方//衝突判定
	//判定結果の移動
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y; // ターンタイマーの上？
	worldTransform_.translation_.z += velocity_.z;

		TouchCeiling(collisionMapInfo);
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空ちゅう状態に移行
			onGround_ = false;
		}
		// 移動
		// 算術演算子が違った
		worldTransform_.translation_.x += velocity_.x;
		worldTransform_.translation_.y += velocity_.y; // ターンタイマーの上？//旧
		worldTransform_.translation_.z += velocity_.z;
		// 空中
	} else {
		// 移動
		worldTransform_.translation_.x += velocity_.x;
		worldTransform_.translation_.y += velocity_.y;
		worldTransform_.translation_.z += velocity_.z;
		// 落下速度
		velocity_.y += -kGravityAccleration;
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		// 地面との当たり判定
		//  加工中？
		if (velocity_.y < 0) {
			// ｙ座標が地面いかになったら着地
			if (worldTransform_.translation_.y <= 2.0f) {
				landing = true;
			}
		}
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 2.0f;
			// 摩擦で横方向速度が減衰知る
			velocity_.x *= (1.0f - kAttenuation); // お前誰やねん
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}


	// 千回制御
	if (turnTimer_ > 0.0f) {
		// 旋回タイマーをカウントダウン
		turnTimer_ -= 1.0f / 60.0f;

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// じきゃrの角度を設定する
		worldTransform_.rotation_.y = destinationRotationY; // ここに角度保管
	}

	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
	/*ImGui::Begin("window");
	ImGui::InputFloat3("Velocity", &velocity_.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::End();*/
}

WorldTransform& Player::GetWorldTransform() { return worldTransform_;}

void Player::TouchCeiling(const CollisionMapInfo& info) {
	// 天井に当たったか
	if (info.ceiling) { // 衝突フラグ
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

// 移動りょり
void Player::FuncMove() {
	// 左右移動操作////	移動入力
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		// 左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			// 左移動中の右入力
			if (velocity_.x < 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAcceleration);
			}
			// 左右状態切り替え
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				// 旋回開始時の角度を記録する
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				// 旋回タイマーに時間を設定する
				turnTimer_ = kTimeTurn;
			}
			acceleration.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 右移動中の左入力
			if (velocity_.x > 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAcceleration);
			}
			// 左右状態切り替え
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				// 旋回開始時の角度を記録する
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				// 旋回タイマーに時間を設定する
				turnTimer_ = kTimeTurn;
			}
			acceleration.x -= kAcceleration;
		}
		// 加速原則
		velocity_.x += acceleration.x;

		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
		// 非入力時は移動減衰を×
		velocity_.x *= (1.0f - kAttenuation);
		if (fabsf(velocity_.x) < 0.05f) {
			velocity_.x = 0;
		}
	}
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		// ジャンプ初速
		velocity_.y += kJumpAccleration;
	}
}
// 衝突判定
void Player::CollisionMap(CollisionMapInfo& info) {
	CollisionMapTop(info);
	/*CollisionMapBottom(info);
	CollisionMapLeft(info);
	CollisionMapRight(info);*/
}

void Player::CollisionMapTop(CollisionMapInfo& info) {

	// jyousyouari?
	if (info.move.y <= 0) {
		return;
	}
	// 移動後の四つの過度の座標
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew{};//ここは宣言

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		positionsNew[i] = CornerPostion(worldTransform_.translation_ + info.move, static_cast<Corner>(i));//これは移動した後のよんかく
	}

	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;//お前ホンマにそこなんか？
	// 左上テンの判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kLeftTop)]);//x1y3が正しいかもここはワールド座標でみるといい//ちゃんとうごいた
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);//飼料の何処にあんねん
	if (mapChipType == MapChipType::kBlock) {//途中のブロックでもあたってはいる
		hit = true;
	}
	// 右上点
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kRightTop)]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kRightTop)]); // ここ穴あき//ここはぶつかったブロックの底辺//ベクターで返すやつがいる探して
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		info.move.y = std::max(0.0f, rect.bottom - (worldTransform_.translation_.y - kHeight / 2.0f)); // ｙ移動量5-5-1.8/2//ムーブがゼロ
		 
		// 天井に当たったことを記録する
		info.ceiling = true;
	}
}

// void Player::CollisionMapBottom(CollisionMapInfo& info) {}
//
// void Player::CollisionMapLeft(CollisionMapInfo& info) {}
//
// void Player::CollisionMapRight(CollisionMapInfo& info) {}




Vector3 Player::CornerPostion(const Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  rightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  LeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  RightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}, //  LeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}
