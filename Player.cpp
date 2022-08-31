#include"Player.h"
#include <cassert>
#include"Procession.h"

void Player::Initialize(Model* model, uint32_t textuerHandle)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textuerHandle;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = { 0,0,20 }; 

	worldTransform_.Initialize();
}

void Player::Update()
{
	// デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		}
	);


	// キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	const float kPlayerSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kPlayerSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kPlayerSpeed;
	}
	else if (input_->PushKey(DIK_UP))
	{
		move.y += kPlayerSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		move.y -= kPlayerSpeed;
	}

	//　座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	// 移動限界座標
	const float kMoveLimitX = 13.0f;
	const float kMoveLimitY = 7.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 行列更新
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	worldTransform_.TransferMatrix();

	// デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	// キャラクター攻撃処理
	Attack();

	// 弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = Mat_Velocity(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		std::unique_ptr < PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.matWorld_, velocity);

		// 弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::OnCollision()
{
}

Vector3 Player::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

float Player::Radius()
{
	return radius_;
}
