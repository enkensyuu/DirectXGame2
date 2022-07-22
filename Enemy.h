#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include"ViewProjection.h"
#include "DebugText.h"

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

private:
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
	Phase phase_= Phase::Approach;
};