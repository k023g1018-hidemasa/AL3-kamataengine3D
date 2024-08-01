#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

// #include"ViewProjection.h"

GameScene::GameScene(){};

GameScene::~GameScene() {

	delete blockModel_;
	delete skydome_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
		worldTransformBlocks_.clear();
	}
	delete debugCamera_;
	// 破壊と創造はセットで
	delete modelSkydome_;

	delete mapChipField_;

	delete model_;
	delete player_;

	for (auto* enemies : enemies_) { // 左が自分でなんでも決めれる名前、右が左にコピーする対象したのを変更したら右が（本体）変わる
		delete enemies;
	}
	enemies_.clear(); // デリートするときは全部消したいからアドレス
	// も消すためにいるそれ以外はいらん

	delete deathParticlesModel_; // 解放
	delete deathParticles_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	blockModel_ = Model::Create();
	blockTextureHandle_ = TextureManager::Load("cube/cube.jpg");

	viewProjection_.Initialize();

	debugCamera_ = new DebugCamera(1280, 720);

	// 天球を内部的に作る
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome;
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	// ｃｓｖを通った後にジェネレイトをする
	GenerateBlocks();

	// texturHandle_ = TextureManager::Load("sample.png"); // キャラ画像淹れる
	model_ = Model::CreateFromOBJ("AL3_Player", true);
	worldTransform_.Initialize();

	// えねみぃのクリエイトとか

	// enemyTexturHandle_ = TextureManager::Load("sample.png");
	enemyModel_ = Model::CreateFromOBJ("AL3_Enemy", true);
	worldTransform_.Initialize(); // いるかな？いらんかな

	// 座標をマップっチップ 番号で指定
	Vector3 playrePosition = mapChipField_->GetMaoChipPositionByIndex(1, 18);
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, &viewProjection_, playrePosition);
	// カメラ生成
	cameraController_ = new CameraController();
	// カメラの初期化
	cameraController_->Initalize(&viewProjection_);
	// ロックオン
	cameraController_->SetTarget(player_);
	// リセット
	cameraController_->Reset();
	// ここに移動範囲の指定？

	player_->SetMapChipField(mapChipField_);

	/////////////////////敵の生成
	for (int32_t i = 0; i < kEnemyNumber; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMaoChipPositionByIndex(15 + i, 18 - i);
		newEnemy->Initialize(enemyModel_, &viewProjection_, enemyPosition);

		enemies_.push_back(newEnemy);
	}
	// enemy_= new Enemy();
	// enemy_->Initialize(enemyModel_, &viewProjection_, enemyPosition);

	// enemy_->SetMapChipField(mapChipField_);//最後のほうっだしイラン気がする

	////仮の生成処理、後で消す
	// deathParticles_ = new DeathParticles;
	// deathParticlesModel_ = Model::CreateFromOBJ("AL3_Enemy", true);
	// deathParticles_->Initialize(deathParticlesModel_, &viewProjection_, playrePosition);//プレイヤーの位置があってるのかｐ16

	phase_ = Phase::kPlay;
}

void GameScene::Update() {

			ChangePhase();//たぶんここ？じゃないとブレイクしてしまう
	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	skydome_->Draw();
	// modelSkydome_->Draw(worldTransform_, viewProjection_);

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			blockModel_->Draw(*worldTransformBlock, viewProjection_, blockTextureHandle_);
		}
	}
	// 自キャラの描画
	player_->Draw();
	// 敵描画
	for (auto* enemies : enemies_) { // 左が自分でなんでも決めれる名前、右が左にコピーする対象したのを変更したら右が（本体）変わる
		enemies->Draw();
	}

	if (deathParticles_) {
		deathParticles_->Draw(); // 何が入るの
	}

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks() {

	// 要素数,ここ変えれば配置する数が変わる
	// 要素数
	// バーティ縦ホリゾン横
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更
	// 列数を設定(縦方向のブロック数)
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMaoChipPositionByIndex(j, i);
			}
		}
	}
	// 要素数を変更する、可変長は最初はゼロだからつ要素を作っている（ｎｗＵ）
	worldTransformBlocks_.resize(numBlockHorizontal);
}

void GameScene::CheckAllCollision() {

	// 判定対象1、2の座標
	AABB aabb1, aabb2;
	// 自キャラの座標
	aabb1 = player_->GetAABB(); // ゲットはちゃんと取得してくれてるけどaabb1,2にわたってないっぽい？
	// 自キャラと敵弾すべての当たり判定
	for (Enemy* enemy : enemies_) {
		// 敵弾の座標
		aabb2 = enemy->GetAABB();

		// AABB同士の考査判定
		if (IsCollision(aabb1, aabb2)) {
			// ぶつかった時どうするか
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision(enemy);
		//	player_->IsDead();
			// 敵との衝突時コールバック呼び出し
			enemy->OnCollision(player_);

		
		}
	}
}

void GameScene::ChangePhase() {

	switch (phase_) {
	case Phase::kPlay:

#ifdef _DEBUG

		if (input_->TriggerKey(DIK_0)) {
			isDebugCameraActive_ ^= true;
		}

		if (isDebugCameraActive_) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

			viewProjection_.TransferMatrix();
		} else {
			viewProjection_.UpdateMatrix();
		}

#endif _DEBUG

		skydome_->Update();

		// 自キャラの更sin
		player_->Update();
		// 更新
		cameraController_->Update();
		
		// 敵の更新処理
		for (auto* enemies : enemies_) { // 左が自分でなんでも決めれる名前、右が左にコピーする対象したのを変更したら右が（本体）変わる
			enemies->Update();
		}
		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				// 定数バッファに転送
				worldTransformBlock->TransferMatrix();
			}
		}

		// 死亡演出
		if (player_->IsDead()) {
			// 死亡演出切り替え
			phase_ = Phase::kDeath;
			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			// 自キャラの座標にですパ初期化ｐ13
			deathParticles_ = new DeathParticles;
			deathParticlesModel_ = Model::CreateFromOBJ("AL3_Enemy", true);
			deathParticles_->Initialize(deathParticlesModel_, &viewProjection_, deathParticlesPosition); // プレイヤーの位置があってるのかｐ16
		}
		CheckAllCollision();


		break;
	case Phase::kDeath:

		
#ifdef _DEBUG

		if (input_->TriggerKey(DIK_0)) {
			isDebugCameraActive_ ^= true;
		}

		if (isDebugCameraActive_) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

			viewProjection_.TransferMatrix();
		} else {
			viewProjection_.UpdateMatrix();
		}

#endif _DEBUG

		skydome_->Update();


		// 更新
		cameraController_->Update();
		// 死亡演出
		deathParticles_->Update();
		// 敵の更新処理
		for (auto* enemies : enemies_) { // 左が自分でなんでも決めれる名前、右が左にコピーする対象したのを変更したら右が（本体）変わる
			enemies->Update();
		}
		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				// 定数バッファに転送
				worldTransformBlock->TransferMatrix();
			}
		}

		break;
	}
        

	

}
