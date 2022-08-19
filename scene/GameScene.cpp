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
	delete modelSkydome_;
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

#pragma region 自弾と敵弾の当たり判定
	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet1 : enemyBullets) {
		posA = bullet1->GetWorldPosition();

		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//自弾の座標
			posB = bullet->GetWorldPosition();
			Vector3 len = Vectornorm(posA, posB);
			float dis = Length(len);
			float radius = enemy_->Radius() + bullet->Radius();
			if (dis <= radius)
			{
				bullet1->OnCollision();
				bullet->OnCollision();
			}
		}
	}
#pragma endregion

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//	ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//	3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("WorldBase", true);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 乱数シード生成器
	std::random_device seed_gen;

	// メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	// 乱数範囲の指定
	std::uniform_real_distribution<float>RotDist(0.0f, XM_2PI);

	std::uniform_real_distribution<float>TransDist(-10.0f, 10.0f);

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();

	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	// 敵キャラの生成
	enemy_ = new Enemy();

	// 敵キャラの初期化
	enemy_->Initialize(model_, textureHandle_);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	// 敵キャラの更新
	enemy_->Update();

	CheckAllCollisions();

	// 天球の更新
	skydome_->Update();
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

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_->Draw(viewProjection_);

	enemy_->Draw(viewProjection_);

	skydome_->Draw(viewProjection_);

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
