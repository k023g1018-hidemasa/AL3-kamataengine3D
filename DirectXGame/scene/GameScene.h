#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include<vector>
#include"MTFunction.h"
#include "DebugCamera.h"
#include"Skydome.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Model* blockModel_=nullptr;
	uint32_t blockTextureHandle_ = 0u;
	// 要素数が分からないから(可変できる配列)、多分ふぉｒで回してブロックの数文っていうやり方？
	// ：の後ろを参照して消す：の前が一個ずつずらしてくれる、とりあえず全部に命令できる
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	//これの名前で呼び出せばｈにあるやつは使えるからわかりやすく？
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;


};
