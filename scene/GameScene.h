﻿#pragma once

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
	DebugText* debugText_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 3Dモデル
	Model* model_ = nullptr;
	// パーツID
	enum PartId {
		kRoot,	//	大元
		kSpine,	//	脊椎
		kChest,	//	胸
		kHead,	//	頭
		kArmL,	//	左腕
		kArmR,	//	右腕
		kHip,	//	尻
		kLegL,	//	左足
		kLegR,	//	右足

		kNumPartId
	};

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// デバッグカメラの生成
	//DebugCamera* debugCamera_ = nullptr;

	// カメラ上方向の角度
	float viewAngle = 0.0f;

	// 自キャラ
	Player* player_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
