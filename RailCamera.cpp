#include"RailCamera.h"
#include"Procession.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update()
{
	Vector3 CameraSpeed = { 0, 0, 0.1f };

	worldTransform_.translation_ += CameraSpeed;

	if (worldTransform_.translation_.z > -20)
	{
		//�ړ�(�x�N�g�������Z)
		worldTransform_.translation_ -= CameraSpeed;
	}

	//�g�����X�t�H�[�����X�V
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ *= Mat_Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= Mat_Rotation(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Mat_Translation(worldTransform_.translation_);
	//���[���h�s��̕��s�ړ��������擾
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f(���[���J�����̑O���x�N�g��)
	forward = Mat_Velocity(forward, worldTransform_.matWorld_);

	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = forward += viewProjection_.eye;

	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);

	//���[���J�����̉�]�𔽉f (���[���J�����̏���x�N�g��)
	viewProjection_.up = Mat_Velocity(up, worldTransform_.matWorld_);

	//�r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();

	DebugText* debugText_ = DebugText::GetInstance();

	//�f�o�b�O�p�\��
	debugText_->SetPos(20, 100);
	debugText_->Printf("RailCamera Pos(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

}
