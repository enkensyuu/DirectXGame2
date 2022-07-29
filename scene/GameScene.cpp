#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include"Procession.h"

using namespace DirectX;

float pi = 3.14f;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	//delete debugCamera_;
	delete player_;
	delete enemy_;
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

	// デバッグカメラの生成
	//debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	// 乱数シード生成器
	std::random_device seed_gen;

	// メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	// 乱数範囲の指定
	std::uniform_real_distribution<float>RotDist(0.0f, XM_2PI);

	std::uniform_real_distribution<float>TransDist(-10.0f, 10.0f);

	// 範囲forで全てのワールドトランスフォームを順に処理する
	/*for (WorldTransform& worldTransform : worldTransforms_)
	{
	}*/
	// 親(0番)
	//worldTransforms_[0].Initialize();
	//
	//// 子(1番)
	//worldTransforms_[1].Initialize();
	//worldTransforms_[1].translation_ = { 0,4.5f,0 };
	//worldTransforms_[1].parent_ = &worldTransforms_[0];

//	// キャラクターの大元
//	worldTransforms_[PartId::kRoot].Initialize();
//
//	// 脊椎
//	worldTransforms_[PartId::kSpine].Initialize();
//	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
//	worldTransforms_[PartId::kSpine].translation_ = { 0,0,0 };
//
//#pragma region 上半身
//	// 胸
//	worldTransforms_[PartId::kChest].Initialize();
//	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
//	worldTransforms_[PartId::kChest].translation_ = { 0,0,0 };
//
//	// 頭
//	worldTransforms_[PartId::kHead].Initialize();
//	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
//	worldTransforms_[PartId::kHead].translation_ = { 0,3.0f,0 };
//
//	// 左腕
//	worldTransforms_[PartId::kArmL].Initialize();
//	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
//	worldTransforms_[PartId::kArmL].translation_ = { 3.0f,0,0 };
//
//	// 右腕
//	worldTransforms_[PartId::kArmR].Initialize();
//	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
//	worldTransforms_[PartId::kArmR].translation_ = { -3.0f,0,0 };
//#pragma endregion
//
//#pragma region 下半身
//	// 尻
//	worldTransforms_[PartId::kHip].Initialize();
//	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
//	worldTransforms_[PartId::kHip].translation_ = { 0,-3.0f,0 };
//
//	// 左足
//	worldTransforms_[PartId::kLegL].Initialize();
//	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
//	worldTransforms_[PartId::kLegL].translation_ = { 3.0f,-3.0f,0 };
//
//	// 右足
//	worldTransforms_[PartId::kLegR].Initialize();
//	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
//	worldTransforms_[PartId::kLegR].translation_ = { -3.0f,-3.0f,0 };
//#pragma endregion
//
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
}

void GameScene::Update() {
	//視点移動処理
	{
		//// 視点の移動ベクトル
		//Vector3	move = { 0,0,0 };

		//// 視点の移動速さ
		//const float kEyeSpeed = 0.2f;

		//// 押した方向で移動ベクトルを変更
		//if (input_->PushKey(DIK_W))
		//{
		//	move.z += kEyeSpeed;
		//}
		//else if (input_->PushKey(DIK_S))
		//{
		//	move.z -= kEyeSpeed;
		//}

		//// 視点移動(ベクトルの加算)
		//viewProjection_.eye += move;

		//// 行列の再計算
		//viewProjection_.UpdateMatrix();

		// デバッグ用表示
		//debugText_->SetPos(50, 50);
		//debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	}

	// 注視点移動処理
	{
		//// 注視点の移動ベクトル
		//Vector3 move = { 0,0,0 };

		//// 注視点の移動速さ
		//const float kTargetSpeed = 0.2f;

		//// 押した方向で移動ベクトルを変更
		//if (input_->PushKey(DIK_LEFT))
		//{
		//	move.x -= kTargetSpeed;
		//}
		//else if (input_->PushKey(DIK_RIGHT))
		//{
		//	move.x += kTargetSpeed;
		//}

		//// 注視点移動(ベクトルの加算)
		//viewProjection_.target += move;

		//// 行列の再計算
		//viewProjection_.UpdateMatrix();

		// デバッグ用表示
		//debugText_->SetPos(50, 70);
		//debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	}

	// 上方向回転処理
	{
		//// 上方向の回転速さ[ラジアン/frame]
		//const float kUpRotSpeed = 0.05f;

		//// 押した方向で移動ベクトルを変更
		//if (input_->PushKey(DIK_SPACE))
		//{
		//	viewAngle += kUpRotSpeed;

		//	// 2πk超えたら0に戻す
		//	viewAngle = fmodf(viewAngle, pi * 2.0f);
		//}

		//// 上方向ベクトルを計算(半径1の円周上の座標)
		//viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

		//// 行列の再計算
		//viewProjection_.UpdateMatrix();

		// デバッグ用表示
		//debugText_->SetPos(50, 90);
		//debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	}

	//// FoV変更処理
	//{
	//	// 上キーで視野角が広がる
	//	if (input_->PushKey(DIK_UP))
	//	{
	//		viewProjection_.fovAngleY += 0.01f;
	//		viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
	//	}
	//	// 下キーで視野角が狭まる
	//	else if (input_->PushKey(DIK_DOWN))
	//	{
	//		viewProjection_.fovAngleY -= 0.01f;
	//		viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
	//	}

	//	// 行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	// デバッグ用表示
	//	debugText_->SetPos(50, 110);
	//	debugText_->Printf("fovAngleY(Degree):%f", XMConvertToRadians(viewProjection_.fovAngleY));
	//}

	//// クリップ距離変更処理
	//{
	//	// 上下キーでニアクリップ距離を増減
	//	if (input_->PushKey(DIK_W))
	//	{
	//		viewProjection_.nearZ += 0.1f;
	//	}
	//	else if (input_->PushKey(DIK_S))
	//	{
	//		viewProjection_.nearZ -= 0.1f;
	//	}

	//	// 行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	// デバッグ用表示
	//	debugText_->SetPos(50, 130);
	//	debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
	//}

	// キャラクター移動処理
	//{
	//	// キャラクターの移動ベクトル
	//	Vector3 move = { 0,0,0 };

	//	const float kTargetSpeed = 0.2f;

	//	if (input_->PushKey(DIK_LEFT))
	//	{
	//		move.x -= kTargetSpeed;
	//	}
	//	else if (input_->PushKey(DIK_RIGHT))
	//	{
	//		move.x += kTargetSpeed;
	//	}

	//	worldTransforms_[PartId::kRoot].translation_ += move;
		//worldTransforms_[0].matWorld_ = Mat_Identity();
		//worldTransforms_[0].matWorld_ *= Mat_Scale(worldTransforms_[0].scale_);
		//worldTransforms_[0].matWorld_ *= Mat_Rotation(worldTransforms_[0].rotation_);
		//worldTransforms_[0].matWorld_ *= Mat_Translation(worldTransforms_[0].translation_);

		//worldTransforms_[0].TransferMatrix();

		// デバッグ用表示
		/*debugText_->SetPos(50, 150);
		debugText_->Printf("kRoot:(%f,%f,%f)", worldTransforms_[PartId::kRoot].translation_.x, worldTransforms_[PartId::kRoot].translation_.y, worldTransforms_[PartId::kRoot].translation_.z);
	}*/

	// 子の更新
	//{
	//	worldTransforms_[1].matWorld_ = Mat_Identity();
	//	worldTransforms_[1].matWorld_ *= Mat_Scale(worldTransforms_[1].scale_);
	//	worldTransforms_[1].matWorld_ *= Mat_Rotation(worldTransforms_[1].rotation_);
	//	worldTransforms_[1].matWorld_ *= Mat_Translation(worldTransforms_[1].translation_);
	//
	//	// 行列の再計算
	//	// worldTransforms_[1].matWorld_ *= worldTransforms_[0].matWorld_;
	//	worldTransforms_[1].matWorld_ *= worldTransforms_[1].parent_->matWorld_;
	//
	//	worldTransforms_[1].TransferMatrix();
	//
	//}
	//viewProjection_.UpdateMatrix();

	// 大元から順に更新していく
	//for (size_t i = 0; i < kNumPartId; i++)
	//{
	//	worldTransforms_[i].matWorld_ = Mat_Identity();
	//	worldTransforms_[i].matWorld_ *= Mat_Scale(worldTransforms_[i].scale_);
	//	worldTransforms_[i].matWorld_ *= Mat_Rotation(worldTransforms_[i].rotation_);
	//	worldTransforms_[i].matWorld_ *= Mat_Translation(worldTransforms_[i].translation_);

	//	if (worldTransforms_[i].parent_ != nullptr)
	//	{
	//		worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
	//	}

	//	worldTransforms_[i].TransferMatrix();
	//}

	//// 上半身回転処理
	//{
	//	if (input_->PushKey(DIK_U))
	//	{
	//		worldTransforms_[PartId::kChest].rotation_.y -= 0.2f;
	//	}
	//	if (input_->PushKey(DIK_I))
	//	{
	//		worldTransforms_[PartId::kChest].rotation_.y += 0.2f;
	//	}
	//}

	//// 下半身回転処理
	//{
	//	// 押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_J))
	//	{
	//		worldTransforms_[PartId::kHip].rotation_.y -= 0.2f;
	//	}
	//	if (input_->PushKey(DIK_K))
	//	{
	//		worldTransforms_[PartId::kHip].rotation_.y += 0.2f;
	//	}
	//}

	// 自キャラの更新
	player_->Update();

	// 敵キャラの更新
	enemy_->Update();
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
	// 範囲forで全てのワールドトランスフォームを順に処理する
	/*for (size_t i = 0; i < kNumPartId; i++)
	{
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}*/

	player_->Draw(viewProjection_);

	enemy_->Draw(viewProjection_);

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