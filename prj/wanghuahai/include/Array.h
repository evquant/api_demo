#ifndef ITSTATION_DATASERVER_ARRAY_H_
#define ITSTATION_DATASERVER_ARRAY_H_

#include "SpinLock.h"
#include <assert.h>
#include <exception>
#include <new>


#define DEF_EQUAL_FUNC(ope) \
	Array<T>& operator ope(Array<T>& arr) \
	{ \
	(*this)[0] ope arr[0]; \
	return *this; \
}

#define DEF_EQUAL_T_FUNC(ope) \
	Array<T>& operator ope(T val) \
	{ \
		(*this)[0] ope val; \
		return *this; \
	}

#define DEF_OPERATOR_FUNC(ope) T operator ope(Array<T>& arr) { return (*this)[0] ope arr[0]; }

#define DEF_OPERATOR_T_FUNC(ope) T operator ope(T val) { return (*this)[0] ope val; }

#define DEF_OPERATOR_T_ARRAY_FUNC(ope) \
	template <typename T1, typename T2> \
	T2 operator ope(T1 val, Array<T2>& arr) \
	{ \
		return val ope arr[0]; \
	}

template <typename T> class Array;
DEF_OPERATOR_T_ARRAY_FUNC(+)
DEF_OPERATOR_T_ARRAY_FUNC(-)
DEF_OPERATOR_T_ARRAY_FUNC(*)
DEF_OPERATOR_T_ARRAY_FUNC(/)

template <typename T>
class Array
{
	enum {
		DEFAULT_LEN = 240,
		MAX_LEN = 1000000
	};
public:
	static T ZERO_ ;

	Array(int default_capcity = DEFAULT_LEN) : is_reference_(false), cur_pos_(-1), capcity_(default_capcity), zero_idx_flag_(false), zero_idx_(-1)
	{
		data_ = new T[capcity_];
		memset(data_, 0, capcity_ * sizeof(T));
	}
	virtual ~Array() { if (!is_reference_) { delete []data_; data_ = nullptr; } }

	Array(const Array<T> &other) {
		memcpy(this, &other, sizeof(Array<T>));
		is_reference_ = true;		
	}

	operator T() { return operator[](0); }

	int Size() const { return (zero_idx_flag_ ? zero_idx_ : cur_pos_) + 1; }
	void Resize(int s) 
	{
		if (s < 0) { throw std::invalid_argument("链表长度不能小于0"); }
		if (s > MAX_LEN) { throw std::out_of_range("超过允许最大长度1000000"); }
		if (capcity_ < s) { Stretch(s); }
		cur_pos_ = s - 1;
	}
	void Clear() { cur_pos_ = -1; }
	T* Data(int i = 0) 
	{ 
		//if (i >= capcity_) { throw std::runtime_error("访问下标越界"); }
		if (i >= capcity_) { return data_; }
		return data_ + i; 
	}
	T& operator[](int idx) 
	{
		//int pos = cur_pos_-idx;
		int pos = (zero_idx_flag_ ? zero_idx_ : cur_pos_) - idx;
		//if (pos < 0) { throw std::runtime_error("Array访问下标越界"); }
		if (pos < 0) { return ZERO_; }
		return data_[pos];
	}

	int Pos(int idx)
	{
		//int pos = cur_pos_-idx;
		int pos = (zero_idx_flag_ ? zero_idx_ : cur_pos_) - idx;
		//if (pos < 0) { throw std::runtime_error("Array获取Pos下标越界"); }
		if (pos < 0) { return -1; }
		return pos;
	}

	void Update(const T& val)
	{
		if(cur_pos_ < 0) { return; }
		data_[cur_pos_] = val;
	}
	void Append(const T& val)
	{
		cur_pos_++;
		if (cur_pos_ >= capcity_) { Stretch(cur_pos_ + 1); }
		data_[cur_pos_] = val;
	}

	void SetZeroIdxFlag(bool flag) { zero_idx_flag_ = flag; }
	void SetZeroIdx(int idx) { zero_idx_ = idx; }

	//Array<double>& operator=(T val)
	//{
	//	(*this)[0] = val;
	//	return *this;
	//}

	DEF_EQUAL_FUNC(=)
	DEF_EQUAL_FUNC(+=)
	DEF_EQUAL_FUNC(-=)
	DEF_EQUAL_FUNC(*=)
	DEF_EQUAL_FUNC(/=)

	DEF_EQUAL_T_FUNC(=)
	DEF_EQUAL_T_FUNC(+=)
	DEF_EQUAL_T_FUNC(-=)
	DEF_EQUAL_T_FUNC(*=)
	DEF_EQUAL_T_FUNC(/=)

	DEF_OPERATOR_FUNC(+)
	DEF_OPERATOR_FUNC(-)
	DEF_OPERATOR_FUNC(*)
	DEF_OPERATOR_FUNC(/)

	DEF_OPERATOR_T_FUNC(+)
	DEF_OPERATOR_T_FUNC(-)
	DEF_OPERATOR_T_FUNC(*)
	DEF_OPERATOR_T_FUNC(/)


private:
	void Stretch(int s)
	{
		if (s <= capcity_) { return; }
		while (capcity_ < s) { capcity_ *= 2; }
		try {
			T* new_data = new T[capcity_];
			memset(new_data, 0, capcity_ * sizeof(T));
			memcpy(new_data, data_, sizeof(T) * Size());
			delete []data_;
			data_ = new_data;
		}
		catch (...) {
			throw std::out_of_range("分配内存失败");
		}
	}

private:
	bool is_reference_;
	T* data_;
	int cur_pos_;
	int capcity_;

	bool zero_idx_flag_;
	int zero_idx_;
};

template <typename T> T Array<T>::ZERO_ = T();

typedef Array<double> NumericSeries;
typedef double NumericSimple;

#endif
