#include"Player.h"
#include <cassert>

Matrix4 Mat_Identity()
{
	//�P�ʍs��
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	return matIdentity;
}

Matrix4 Mat_Translation(Vector3 translation)
{
	//���s�ړ��s���錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;
	matTrans.m[3][3] = 1;

	return matTrans;
}

void Player::Initialize(Model* model, uint32_t textuerhandle)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textuerhandle;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update()
{
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
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 19;

	// �͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// �s��X�V
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ *= Mat_Translation(worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	// �f�o�b�O�p�\��
	debugText_->SetPos(50, 50);
	debugText_->Printf("Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
