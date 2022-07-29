#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include"ViewProjection.h"
#include "DebugText.h"
#include"EnemyBullet.h"

class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
	/// <summary>
	///初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textuerHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textuerHandle);

	/// <summary>
	///更新
	/// </summary>
	void Update();

	/// <summary>
	///描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection_);

	// 接近フェーズ行動
	void Approach();

	//　離脱フェーズ行動
	void Leave();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	// 発射間隔
	static const int kFireInterval = 60;

	// 接近フェーズ初期化
	void ApproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();
private:
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 行動フェーズ
	enum class Phase
	{
		Approach,	//	接近する
		Leave,		//	離脱する
	};

	// フェーズ
	Phase phase_ = Phase::Approach;

	// 弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	// 発射タイマー
	int32_t FireTimer = 0;

	// 自キャラ
	Player* player_ = nullptr;
};