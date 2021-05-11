#pragma once
template <typename T>
class SingletonBase {

	//접근자 프로텍티드(상속되는 자식에게 공개되는 멤버)
protected:
	//상속받은 자식들마다 고유의 인스턴스 멤버를 갖게 된다.
	static T* Instance;
	//생성자 소멸자 역할은 없음
	SingletonBase() {};
	~SingletonBase() {};

public:
	//스태틱 멤버 받아오는 함수
	static T* GetInstance();
	//객체 해제해주는 함수
	void DestroyInstance();
};

//스태틱 멤버는 클래스 밖에서 초기화
template<typename T>
T* SingletonBase<T>::Instance = nullptr;


template<typename T>
T * SingletonBase<T>::GetInstance()
{
	//생성되어 있지 않다면 동적할당을 통해 생성
	if (!Instance) Instance = new T;
	//인스턴스 값 반환
	return Instance;
}

template<typename T>
void SingletonBase<T>::DestroyInstance()
{
	//생성된 값이 존재한다면
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

