#pragma once
template <typename T>
class SingletonBase {

	//������ ������Ƽ��(��ӵǴ� �ڽĿ��� �����Ǵ� ���)
protected:
	//��ӹ��� �ڽĵ鸶�� ������ �ν��Ͻ� ����� ���� �ȴ�.
	static T* Instance;
	//������ �Ҹ��� ������ ����
	SingletonBase() {};
	~SingletonBase() {};

public:
	//����ƽ ��� �޾ƿ��� �Լ�
	static T* GetInstance();
	//��ü �������ִ� �Լ�
	void DestroyInstance();
};

//����ƽ ����� Ŭ���� �ۿ��� �ʱ�ȭ
template<typename T>
T* SingletonBase<T>::Instance = nullptr;


template<typename T>
T * SingletonBase<T>::GetInstance()
{
	//�����Ǿ� ���� �ʴٸ� �����Ҵ��� ���� ����
	if (!Instance) Instance = new T;
	//�ν��Ͻ� �� ��ȯ
	return Instance;
}

template<typename T>
void SingletonBase<T>::DestroyInstance()
{
	//������ ���� �����Ѵٸ�
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

