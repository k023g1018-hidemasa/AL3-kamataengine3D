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
#include"MapchipField.h"
#include"player.h"
#include"CameraController.h"
#include"Enemy.h"
#include"DeathParticles.h"

//ゲームのフェーズ
enum class Phase {
	kPlay,//ゲームプレい
	kDeath,//です演出
};



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

	void GenerateBlocks();
	//すべての当たり判定を行う
	void CheckAllCollision();

	void ChangePhase();

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

	//model_ = Model::CreateFromOBJ("")

	//これの名前で呼び出せばｈにあるやつは使えるからわかりやすく？
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	MapChipField* mapChipField_;
	//キャラクターのテクスチャ
	//uint32_t texturHandle_ = 0u;

	Model* model_ = nullptr;
	Player* player_ = nullptr;

	CameraController* cameraController_ = nullptr;

	//えねみぃのテクスチャとかドローに使うやつ

	//uint32_t enemyTexturHandle_ = 0u;

	Model* enemyModel_ = nullptr;
	std::list<Enemy*> enemies_ ;//リストにしたら入れるのめんどい
	static inline const int32_t kEnemyNumber = 1;

	DeathParticles* deathParticles_ = nullptr;
	Model* deathParticlesModel_ = nullptr;
	// ゲームの現在のフェーズ（変数）
	Phase phase_;//変数系はちゃんとプライベートに
};