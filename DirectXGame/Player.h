#pragma once
#include "Input.h"
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <cassert>
#include <imgui.h>
#include <numbers>

class MapChipField;
class Enemy;

struct CollisionMapInfo {
	bool ceiling = false;     // 天井
	bool landing = false;     // ュカ
	bool hitWall = false;     // KV
	Vector3 move = {0, 0, 0}; // ここでセットするわけではない？
};
// 角
enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上

	kNumCorner // enumの要素数ここを見ると何個あるかわかる
};

enum class LRDirection {
	kRight,
	kLeft,
};



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
	WorldTransform& GetWorldTransform(); 

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_=mapChipField; };
	void TouchCeiling(const CollisionMapInfo& info);
	void TouchWell(const CollisionMapInfo& info);


	Vector3 GetWorldPosition();
	AABB GetAABB();
	void OnCollision(const Enemy* enemy);

private:
	void FuncMove(CollisionMapInfo& info);
	void FuncOnground(CollisionMapInfo& info);
	void FuncSky(CollisionMapInfo& info);
	void CollisionMap(CollisionMapInfo& info); // 紺ストは付けない中身は変わるから
	void CollisionMapTop(CollisionMapInfo& info);
	void CollisionMapBottom(CollisionMapInfo& info);
	void CollisionMapLeft(CollisionMapInfo& info);
	void CollisionMapRight(CollisionMapInfo& info);

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
	static inline const float kLimitRunSpeed = 0.5f;
	// 重力加速度（下方向）
	static inline const float kGravityAccleration = 0.03f;
	// 最大落下速度（下方向）
	static inline const float kLimitFallSpeed = 1.2f;
	// ジャンプ初速（上方向）
	static inline const float kJumpAccleration = 0.4f;

	Vector3 velocity_ = {};
	LRDirection lrDirection_ = LRDirection::kRight;
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	// 接地状態フラグ
	bool onGround_ = true;
	//自キャラの速度を取得
	const Vector3& GetVelocity() const { return velocity_; }
	//マップチップでフィールドを作った
	MapChipField* mapChipField_ = nullptr;

	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;

	Vector3 CornerPostion(const Vector3& center, Corner corner);
	static inline const float kBlank = 0.2f;//めり込まない数値？
	
	static inline const float kAttenutionLanding = 0.4f; // 着地時の速度減衰
	static inline const float kAttenutionWall = 0.9f;


};
