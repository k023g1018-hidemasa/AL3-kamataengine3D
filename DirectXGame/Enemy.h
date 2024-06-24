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


};

