#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>
#include"DebugCamera.h"
#include"Player.h"
#include"Enemy.h"
#include"Procession.h"
#include"Skydome.h"
#include"RailCamera.h"

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

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	float Angle(float angle);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle1_ = 0; 
	uint32_t textureHandle2_ = 0; 
	uint32_t textureHandle3_ = 0; 

	Sprite* sprite1_ = nullptr;
	Sprite* sprite2_ = nullptr;
	Sprite* sprite3_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

	// 敵キャラ
	Enemy* enemy_ = nullptr;

	// 天球
	Skydome* skydome_ = nullptr;

	// 3Dモデル
	Model* modelSkydome_ = nullptr;
	Model* model_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelBullet_ = nullptr;
	
	// レールカメラ
	std::unique_ptr<RailCamera> railcamera_;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;




	

	bool isDebugCameraActive_ = false;

	int32_t Scene = 1;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
