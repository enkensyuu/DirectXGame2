#include"Enemy.h"
#include <cassert>
#include"Procession.h"
#include"Player.h"

void Enemy::Approach()
{
	Vector3 ApproachSpeed = { 0,0,-0.2f };

	// �ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += ApproachSpeed;
	// �K��̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}

	// ���˃^�C�}�[�J�E���g�_�E��
	FireTimer--;
	// �w�莞�ԂɒB����
	if (FireTimer <= 0)
	{
		// �e�𔭎�
		Fire();
		// ���˃^�C�}�[��������
		FireTimer = kFireInterval;
	}
}

void Enemy::Leave()
{
	Vector3 LeaveSpeed = { 0.2f,0,0.2f };

	// �ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += LeaveSpeed;
}

void Enemy::Initialize(Model* model, uint32_t textuerHandle)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("lui-zi.jpg");

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = { 0,3,100 };

	// �L�����N�^�[�U������
	ApproachInitialize();
}

void Enemy::Update()
{
	switch (phase_)
	{
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}

	// �f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		}
	);

	// �L�����N�^�[�̈ړ��x�N�g��
	//Vector3 move = { 0,0,0 };
	//
	//const float kEnemySpeed = 0.2f;
	//
	//move.z -= kEnemySpeed;
	//
	////�@���W�ړ�(�x�N�g���̉��Z)
	//worldTransform_.translation_ += move;

	// �s��X�V
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// �e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

void Enemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	// �e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

void Enemy::Fire()
{
	assert(player_);
	// �e�̑��x
	const float kBulletSpeed = 1.0f;

	Vector3 Playervelocity = player_->GetWorldPosition();
	Vector3 Enemyvelocity = GetWorldPosition();
	Vector3 velocity = Playervelocity -= Enemyvelocity;

	// �e�𐶐����A������
	std::unique_ptr < EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// �e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachInitialize()
{
	// ���˃^�C�}�[��������
	FireTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition()
{
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

float Enemy::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}

Vector3 Enemy::Normalize()
{
	float len = length();
	if (len != 0)
	{
		return*this /= len;
	}
	return*this;
}

