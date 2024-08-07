#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include"numbers"
#include "TextureManager.h"



class TitleScene {
public:




	/// <summary>
	/// コンストクラタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();


	bool IsFinished() const {return finished_; }
	

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;


	Model* titleModel_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	uint32_t textureHandle_ = 0u;
	bool finished_ = false;

	//int timer_;
};

