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
	delete debugCamera_;
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
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	// 乱数シード生成器
	std::random_device seed_gen;

	// メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	// 乱数範囲の指定
	std::uniform_real_distribution<float>RotDist(0.0f, XM_2PI);

	std::uniform_real_distribution<float>TransDist(-10.0f, 10.0f);

	

	// 範囲forで全てのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		// ワールドトランスフォームの初期化
		worldTransform.Initialize();

		// X,Y,Z方向のスケーリングを設定
		worldTransform.scale_ = { 1,1,1 };

		// スケーリング行列を宣言
		Matrix4 matScale;

		matScale.m[0][0] = worldTransform.scale_.x;
		matScale.m[1][1] = worldTransform.scale_.y;
		matScale.m[2][2] = worldTransform.scale_.z;
		matScale.m[3][3] = 1;

		//  単位行列を宣言
		Matrix4 matIdentity;

		matIdentity.m[0][0] = 1;
		matIdentity.m[1][1] = 1;
		matIdentity.m[2][2] = 1;
		matIdentity.m[3][3] = 1;

		// X,Y,Z軸周りの回転角を設定
		worldTransform.rotation_ = { RotDist(engine),RotDist(engine) ,RotDist(engine) };

		// X,Y,Z軸回転行列を宣言
		Matrix4 matRotX, matRotY, matRotZ;

		// 合成用回転行列を宣言
		Matrix4 matRot = matIdentity;

		// Z軸回転行列
		matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
		matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
		matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
		matRotZ.m[1][1] = cos(worldTransform.rotation_.z);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// X軸回転行列
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cos(worldTransform.rotation_.x);
		matRotX.m[1][2] = sin(worldTransform.rotation_.x);
		matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
		matRotX.m[2][2] = cos(worldTransform.rotation_.x);
		matRotX.m[3][3] = 1;

		// Y軸回転行列
		matRotY.m[0][0] = cos(worldTransform.rotation_.y);
		matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
		matRotY.m[1][1] = 1;
		matRotY.m[2][0] = sin(worldTransform.rotation_.y);
		matRotY.m[2][2] = cos(worldTransform.rotation_.y);
		matRotY.m[3][3] = 1;

		// 各軸の回転行列を合成
		matRot *= matRotZ;
		matRot *= matRotX;
		matRot *= matRotY;

		// X,Y,Z軸周りの平行移動を設定
		worldTransform.translation_ = { TransDist(engine),TransDist(engine) ,TransDist(engine) };

		// 平行移動行列を宣言
		Matrix4 matTrans = MathUtility::Matrix4Identity();

		// 平行移動行列
		matTrans.m[0][0] = 1;
		matTrans.m[1][1] = 1;
		matTrans.m[2][2] = 1;
		matTrans.m[3][0] = worldTransform.translation_.x;
		matTrans.m[3][1] = worldTransform.translation_.y;
		matTrans.m[3][2] = worldTransform.translation_.z;
		matTrans.m[3][3] = 1;

		worldTransform.matWorld_ = matIdentity;

		worldTransform.matWorld_ *= matScale;

		//worldTransform.matWorld_ *= matRotZ;
		//worldTransform.matWorld_ *= matRotX;
		//worldTransform.matWorld_ *= matRotY;

		worldTransform.matWorld_ *= matRot;

		worldTransform.matWorld_ *= matTrans;

		// 行列の転送
		worldTransform.TransferMatrix();
	}

	// カメラ視点座標を設定
	viewProjection_.eye = { 0,0,-40 };

	// カメラ注視点座標を設定
	viewProjection_.target = { 10,0,0 };

	// カメラ上方向ベクトルを設定(右上45度指定)
	viewProjection_.up = { cosf(pi / 4.0f),sinf(pi / 4.0f),0.0f };

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();


}

void GameScene::Update() {
	// 視点の移動ベクトル
	Vector3	move = { 0,0,0 };

	// 視点の移動速さ
	const float kEyeSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W))
	{
		move.z += kEyeSpeed;
	}
	else if (input_->PushKey(DIK_S))
	{
		move.z -= kEyeSpeed;
	}

	// 視点移動(ベクトルの加算)
	viewProjection_.eye += move;

	// 行列の再計算
	viewProjection_.UpdateMatrix();

	// デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	// 注視点移動処理
	{
		// 注視点の移動ベクトル
		Vector3 move = { 0,0,0 };

		// 注視点の移動速さ
		const float kTargetSpeed = 0.2f;

		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT))
		{
			move.x -= kTargetSpeed;
		}
		else if (input_->PushKey(DIK_RIGHT))
		{
			move.x += kTargetSpeed;
		}

		// 注視点移動(ベクトルの加算)
		viewProjection_.target += move;

		// 行列の再計算
		viewProjection_.UpdateMatrix();

		// デバッグ用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

		// 上方向回転処理
		{
			// 上方向の回転速さ[ラジアン/frame]
			const float kUpRotSpeed = 0.05f;

			// 押した方向で移動ベクトルを変更
			if (input_->PushKey(DIK_SPACE))
			{
				viewAngle += kUpRotSpeed;

				// 2πk超えたら0に戻す
				viewAngle = fmodf(viewAngle, pi * 2.0f);
			}

			// 上方向ベクトルを計算(半径1の円周上の座標)
			viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

			// 行列の再計算
			viewProjection_.UpdateMatrix();

			// デバッグ用表示
			debugText_->SetPos(50, 90);
			debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
		}
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
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
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
