#pragma once
#include "WorldTransform.h"
#include"ViewProjection.h"
#include "DebugText.h"

/// <summary>
/// ���[���J����
/// </summary>
class RailCamera
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotation);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �r���[�v���W�F�N�V����
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	// ���[���h�s����擾
	WorldTransform* GetWorldMatrix() { return &worldTransform_; }

	float Radius();

	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	// ���[���h���W���擾
	Vector3 GetWorldPosition();

	float Hp();

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	// �r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	const float radius_ = 1.0f;

	int32_t hp = 150;

	
};