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
	if (Input::GetInstance()->PushKey(DIK_X)) {
		isDead_ = true;

	}
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.move = velocity_;
	CollisionMap(collisionMapInfo); // 先生の書き方//衝突判定
	TouchCeiling(collisionMapInfo); // オングラウンドの中に入ってた
	TouchWell(collisionMapInfo);
	if (onGround_) { // 接地状態切り替え
		FuncOnground(collisionMapInfo);
		FuncMove(collisionMapInfo); // こっれでええんかな旋回制御？
		// 判定結果の移動
		worldTransform_.translation_.x += velocity_.x;
		worldTransform_.translation_.y += velocity_.y; // ターンタイマーの上？
		worldTransform_.translation_.z += velocity_.z;

		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空ちゅう状態に移行
			onGround_ = false;
		}
		// 移動
		// 算術演算子が違った
		// 空中
	} else {
		FuncSky(collisionMapInfo);
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

WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

void Player::TouchCeiling(const CollisionMapInfo& info) {
	// 天井に当たったか
	if (info.ceiling) { // 衝突フラグ
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}
void Player::TouchWell(const CollisionMapInfo& info) {

	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenutionWall);
	}
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos{};
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Player::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb{};

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	// ジャンプ開始（仮処理）
	isDead_ = true;//	ここで変更
}

// 移動りょり
void Player::FuncMove(CollisionMapInfo& info) {
	// 左右移動操作////	移動入力
	// 判定結果の移動
	worldTransform_.translation_.x += info.move.x;
	worldTransform_.translation_.y += info.move.y; // ターンタイマーの上？//インフォに変えた
	worldTransform_.translation_.z += info.move.z;
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

void Player::FuncOnground(CollisionMapInfo& info) {
	if (velocity_.y > 0.0f) {
		onGround_ = false;
	} else {
		// 移動後の四つの過度の座標
		std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew{}; // ここは宣言

		for (uint32_t i = 0; i < positionsNew.size(); ++i) {

			positionsNew[i] = CornerPostion(worldTransform_.translation_ + info.move, static_cast<Corner>(i)); // これは移動した後のよんかく
		}

		MapChipType mapChipType;
		// 真sitaの当たり判定を行う
		bool hit = false; // お前ホンマにそこなんか？
		// 左下テンの判定
		IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kLeftBottom)] + Vector3{0, -1.6f, 0}); // トップの移植
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
		// 右下点
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kRightBottom)] + Vector3{0, -1.6f, 0}); //???効いてない？
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
		if (!hit) {

			onGround_ = false;
		}
	}
}
void Player::FuncSky(CollisionMapInfo& info) {
	worldTransform_.translation_.x += info.move.x; // インフォを持ってきてるからそれを使って動く値を変更する
	worldTransform_.translation_.y += info.move.y;
	// 落下速度
	velocity_.y += -kGravityAccleration;
	// 落下速度制限
	velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	// 接地フラグ
	if (info.landing) {
		// 接地状態に切り替える
		onGround_ = true;
		// 着地時にｘ速度を減衰
		velocity_.x *= (1.0f - kAttenuation);
		// ｙの速度をゼロに
		velocity_.y = 0.0f;
	}
}

// 衝突判定
void Player::CollisionMap(CollisionMapInfo& info) {
	CollisionMapTop(info);
	CollisionMapBottom(info);
	CollisionMapLeft(info);
	CollisionMapRight(info);
}

void Player::CollisionMapTop(CollisionMapInfo& info) {

	// jyousyouari?
	if (info.move.y <= 0) {
		return;
	}
	// 移動後の四つの過度の座標
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew{}; // ここは宣言

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		positionsNew[i] = CornerPostion(worldTransform_.translation_ + info.move, static_cast<Corner>(i)); // これは移動した後のよんかく
	}

	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false; // お前ホンマにそこなんか？
	// 左上テンの判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kLeftTop)]); // x1y3が正しいかもここはワールド座標でみるといい//ちゃんとうごいた
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);        // 飼料の何処にあんねん
	if (mapChipType == MapChipType::kBlock) {                                                    // 途中のブロックでもあたってはいる
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

		info.move.y = std::max(0.0f, (rect.bottom - worldTransform_.translation_.y) - (0.1f + kHeight / 2.0f)); // ｙ移動量5-5-1.8/2//ムーブがゼロ//
		                                                                                                        // ブロックの下-自キャラの高さ+キャラの半径
		// 天井に当たったことを記録する
		info.ceiling = true;
	}
}

void Player::CollisionMapBottom(CollisionMapInfo& info) {
	if (info.move.y >= 0) {
		return;
	}
	// 移動後の四つの過度の座標
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew{}; // ここは宣言

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		positionsNew[i] = CornerPostion(worldTransform_.translation_ + info.move, static_cast<Corner>(i)); // これは移動した後のよんかく
	}

	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false; // お前ホンマにそこなんか？
	// 左下テンの判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kLeftBottom)]); // トップの移植
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kRightBottom)]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kRightBottom)]);
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		float offset = (0.1f + kHeight / 2.0f);
		float topToTranslation = rect.top - worldTransform_.translation_.y;
		float moveYotei = topToTranslation + offset; // ここを分ける
		info.move.y = std::min(0.0f, moveYotei);     // ｙ移動量5-5-1.8/2//ムーブがゼロ//ミニマムかマックスか
		                                             // ブロックの下-自キャラの高さ+キャラの半径
		// 地面に当たったことを記録する
		info.landing = true;
	}
}

void Player::CollisionMapLeft(CollisionMapInfo& info) {
	if (info.move.x >= 0) {
		return;
	}
	// 移動後の四つの過度の座標
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew{}; // ここは宣言

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		positionsNew[i] = CornerPostion(worldTransform_.translation_ + info.move, static_cast<Corner>(i)); // これは移動した後のよんかく
	}

	MapChipType mapChipType;

	bool hit = false; // お前ホンマにそこなんか？
	// 左上テンの判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kLeftTop)]); // トップの移植
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 左下点
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kLeftBottom)]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kLeftTop)]);
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		info.move.x = std::max(0.0f, (rect.right - worldTransform_.translation_.x) + (0.2f + kWidth / 2.0f)); // 合ってるか分からん

		info.hitWall = true;
	}
}

void Player::CollisionMapRight(CollisionMapInfo& info) {
	if (info.move.x <= 0) {
		return;
	}
	// 移動後の四つの過度の座標
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew{}; // ここは宣言

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		positionsNew[i] = CornerPostion(worldTransform_.translation_ + info.move, static_cast<Corner>(i)); // これは移動した後のよんかく
	}

	MapChipType mapChipType;

	bool hit = false; // お前ホンマにそこなんか？
	// 右上テンの判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kRightTop)]); // トップの移植
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kRightBottom)]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[int(Corner::kRightBottom)]); // ここ穴あき//ここはぶつかったブロックの底辺//ベクターで返すやつがいる探して
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		float offSet = (0.2f + kHeight / 2.0f);
		float rightToTranslation = rect.left - worldTransform_.translation_.x;
		float moveYoteiX = rightToTranslation - offSet; // ここを分ける                                                       ミニマムかマックスか
		info.move.x = std::min(0.0f, moveYoteiX);       // 合ってるか分からん

		info.hitWall = true;
	}
}

Vector3 Player::CornerPostion(const Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  rightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  LeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  RightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}, //  LeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}
