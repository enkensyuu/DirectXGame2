#include"Enemy.h"
#include <cassert>
#include"Procession.h"

void Enemy::Approach()
{
	Vector3 ApproachSpeed = { 0,0,-0.2f };

	// �ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += ApproachSpeed;
	// �K��̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_=Phase::Leave;
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
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = { 0,3,100 };
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
}

void Enemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
