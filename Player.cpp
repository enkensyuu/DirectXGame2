#include"Player.h"
#include <cassert>
#include"Procession.h"

void Player::Initialize(Model* model, uint32_t textuerHandle)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textuerHandle;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = { 0,0,20 }; 

	worldTransform_.Initialize();
}

void Player::Update()
{
	// �f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		}
	);


	// �L�����N�^�[�̈ړ��x�N�g��
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

	//�@���W�ړ�(�x�N�g���̉��Z)
	worldTransform_.translation_ += move;

	// �ړ����E���W
	const float kMoveLimitX = 13.0f;
	const float kMoveLimitY = 7.0f;

	// �͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// �s��X�V
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	worldTransform_.TransferMatrix();

	// �f�o�b�O�p�\��
	debugText_->SetPos(50, 50);
	debugText_->Printf("Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	// �L�����N�^�[�U������
	Attack();

	// �e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// �e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		// �e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = Mat_Velocity(velocity, worldTransform_.matWorld_);

		// �e�𐶐����A������
		std::unique_ptr < PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.matWorld_, velocity);

		// �e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::OnCollision()
{
}

Vector3 Player::GetWorldPosition()
{
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

float Player::Radius()
{
	return radius_;
}
