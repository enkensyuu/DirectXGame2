#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

float pi = 3.14f;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	//delete debugCamera_;
	delete player_;
	delete enemy_;
	delete modelEnemy_;
	delete modelBullet_;
	delete modelSkydome_;
	delete sprite1_;
	delete sprite2_;
	delete sprite3_;
}

void GameScene::CheckAllCollisions()
{
	// 判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();
		Vector3 len = Vectornorm(posA, posB);
		float dis = Length(len);
		float radius = player_->Radius() + bullet->Radius();
		if (dis <= radius)
		{
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	posA = enemy_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();
		Vector3 len = Vectornorm(posA, posB);
		float dis = Length(len);
		float radius = enemy_->Radius() + bullet->Radius();
		if (dis <= radius)
		{
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

}

float GameScene::Angle(float angle)
{
	return angle * pi / 180;
}


void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//	ファイル名を指定してテクスチャを読み込む
	textureHandle1_ = TextureManager::Load("title.png");
	textureHandle2_ = TextureManager::Load("gameover.png");
	textureHandle3_ = TextureManager::Load("gamecler.png");

	sprite1_ = Sprite::Create(textureHandle1_, { 0,0 });
	sprite2_ = Sprite::Create(textureHandle2_, { 0,0 });
	sprite3_ = Sprite::Create(textureHandle3_, { 0,0 });

	//	3Dモデルの生成
	model_ = Model::CreateFromOBJ("Player",true);
	modelEnemy_ = Model::CreateFromOBJ("Enemy",true);
	modelBullet_ = Model::CreateFromOBJ("Bullet",true);
	modelSkydome_ = Model::CreateFromOBJ("WorldBase", true);


	// 自キャラの生成
	player_ = new Player();

	// 自キャラの初期化
	player_->Initialize(model_, modelBullet_);

	// 敵キャラの生成
	enemy_ = new Enemy();

	// 敵キャラの初期化
	enemy_->Initialize(modelEnemy_, modelBullet_);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	//レ-ルカメラ
	railcamera_ = std::make_unique<RailCamera>();
	railcamera_->Initialize(Vector3(0, 0, -50), Vector3(0, 0, 0));

	//レールカメラとプレイヤーの親子構造
	player_->SetCamera(railcamera_->GetWorldMatrix());

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

}

void GameScene::Update()
{
	switch (Scene)
	{
	case 1:
		Initialize();
		if (input_->TriggerKey(DIK_SPACE))
		{
			Scene = 2;
		}
		break;
	case 2:
		if (player_->Hp() <= 0)
		{
			Scene = 3;
		}
		if (enemy_->Hp() <= 0)
		{
			Scene = 4;
		}
		railcamera_->Update();

		//railCameraをゲームシーンに適応させる
		viewProjection_.matView = railcamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railcamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();

		// 自キャラの更新
		player_->Update();

		// 敵キャラの更新
		enemy_->Update();

		CheckAllCollisions();

		// 天球の更新
		skydome_->Update();
		break;
	case 3:
		if (input_->TriggerKey(DIK_LSHIFT))
		{
			Scene = 1;
		}
		break;
	case 4:
		if (input_->TriggerKey(DIK_LSHIFT))
		{
			Scene = 1;
		}
		break;
	}

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
	switch (Scene)
	{
	case 1:
		sprite1_->Draw();
		if (input_->TriggerKey(DIK_SPACE))
		{
			Scene = 2;

		}
		break;
	case 3:
		sprite2_->Draw();
		if (input_->TriggerKey(DIK_LSHIFT))
		{
			Scene = 1;
		}
		break;
	case 4:
		sprite3_->Draw();
		if (input_->TriggerKey(DIK_LSHIFT))
		{
			Scene = 1;
		}
		break;
	}
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	switch (Scene)
	{
	case 2:
		if (player_->Hp() <= 0)
		{
			Scene = 3;
		}
		if (enemy_->Hp() <= 0)
		{
			Scene = 4;
		}
		player_->Draw(viewProjection_);

		enemy_->Draw(viewProjection_);

		skydome_->Draw(viewProjection_);
		break;
	
	}
	// ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//PrimitiveDrawer::GetInstance()->DrawLine3d({ 0,0,0 }, { 70,-70,0 }, { 1,0,0,1 });

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
