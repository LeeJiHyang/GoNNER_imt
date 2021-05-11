#pragma once

class CTimeManager
{
	DECLARE_SINGLETON(CTimeManager)
	

private:
	//FPS:�ʴ� ������ ��(frames per second)

	bool _isHardware;				//���� Ÿ�̸Ӹ� �����ϳ�?
	__int64 _curTime;				//����ð�
	__int64 _lastTime;				//�������ð�

	float _FPStimeElapsed;			//FPS �������ð��� ���� �ð��� �����
	float _timeScale;				//�ð� �����
	DWORD _FPSFrameCount;			//FPS ī��Ʈ

	//���ϰ�
	DWORD _frameRate;				//FPS
	float _timeElapsed;				//�� �����Ӵ� �����
	float _worldTime;				//��ü �ð� �����


public:
	CTimeManager();
	~CTimeManager();

	//�ʱ�ȭ ���� ������ �������� 60���������� ����
	void SetFrame(UINT lock = 60);

	//�� ������ ����ð� �������� 1/60
	float GetElapsedTime() const { return _timeElapsed; }
	//��ü �ð� ����� ��������
	float GetWorldTime() const { return _worldTime; }
	//���� FPS ��������
	unsigned long GetFrameRate() { return _frameRate; }
	void Render();
};