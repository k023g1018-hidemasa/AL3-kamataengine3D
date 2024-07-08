#pragma once
#include"Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
//最初の角度は真っすぐを向いた角度で最後の角度は上下合わせた角度
//求め方は度数砲*π/180＝弧度法らしい3、009（座標）がはなまるぴっぴはよいこだけらしい


class MapChipField;


class Enemy {
public:
	Enemy();
	~Enemy();

	void Initialize(Model* enemyModel, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; };

private:
	Model* enemyModel_=nullptr;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_=nullptr;

	uint32_t textureHandle_ = 0u;
	// マップチップでフィールドを作った
	MapChipField* mapChipField_ = nullptr;
	static inline const float kWalkSpeed = 0.1f;
	Vector3 verocity_ = {};
	//最初の角度（上向き）
	static inline const float kWalkMotionAngleStart = 30;
	//最後の角度（下向き）
	static inline const float kWalkMotionAngleEnd = -30;
	//アニメーションの周期となる時間
	static inline const float kWalkMotionTime = 60 * 2;
	//経過時間
	float wolkTimer_ = 0.0f;



};

