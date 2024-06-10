#define NOMINMAX
#pragma once
#include<ViewProjection.h>
#include <algorithm>



class Player;


class CameraController {
public:

	////くけい
	//struct Rect {
	//	float left = 0.0f;
	//	float right = 1.0f;
	//	float bottom = 0.0f;
	//	float top = 1.0f;
	//};
	
	void Initalize(ViewProjection* viewprojection);

	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

//	void SetMovableArea(Rect area) { movableArea_=area; }



private:

	ViewProjection* viewProjection_=nullptr;
	Player* target_ = nullptr;
	//追従対象とカメラの座標の差（おふぅセット）
	Vector3 targetOffset_ = {0, 0, -15.0f};
	//カメラ移動範囲
//	Rect movableArea_ = {0,100,0,100};

	//カメラの目標座標
	Vector3 targetCoordinates;
	//座標保管割合
	static inline const float kInterpolationRate = 0.5f;
	//速度掛け率　速度に調整用の数値をかけてから足す
	static inline const float kVelocityBias = 0.2f;

};

