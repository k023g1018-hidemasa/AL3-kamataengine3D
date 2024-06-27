#define NOMINMAX
#pragma once
#include<ViewProjection.h>




class Player;


class CameraController {
public:

	//くけい
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};
	
	void Initalize(ViewProjection* viewprojection);

	void Update();

	void SetTarget(Player* target) {
		target_ = target;
	}

	void Reset();

	void SetMovableArea(Rect area) { movableArea_=area; }



private:

	ViewProjection* viewProjection_=nullptr;
	Player* target_ = nullptr;
	//追従対象とカメラの座標の差（おふぅセット）
	Vector3 targetOffset_ = {0, 0, -15.0f};
	//カメラ移動範囲
	Rect movableArea_ = {10,200,4,100};//ここの数字は都度都度変える、エリアの設定をするのは自動で合わせられないから？

	//カメラの目標座標
	Vector3 targetCoordinates;//こいつは最終の座標ここに元の座標をもってくる
	//座標保管割合
	static inline const float kInterpolationRate = 0.2f;
	//速度掛け率　速度に調整用の数値をかけてから足す
	static inline const float kVelocityBias = 10.0f;//値が小さいだけやないかい



	
	
	Vector3 targetVelosity={}; // この状態の宣言で大丈夫、中身はｃｐｐで持ってくる


};

