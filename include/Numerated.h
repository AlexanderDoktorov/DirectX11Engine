#pragma once

template<class T>
class Numerated
{
public:
	Numerated()
	{
		++numInstances;
	}
	~Numerated()
	{
		--numInstances;
	}
	static size_t GetCount()
	{
		return numInstances;
	}
private:
	static size_t numInstances;
};

template<class T>
size_t Numerated<T>::numInstances = 0U;