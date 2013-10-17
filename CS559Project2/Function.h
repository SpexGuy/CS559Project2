#pragma once
#include <glm/glm.hpp>

template<typename P, typename R>
class Function {
public:
	virtual R evaluate(P param) = 0;
};



template<typename R>
class TimeFunction : public Function<int, R> {};



class LinearTimeFunction : public TimeFunction<float> {
private:
	LinearTimeFunction();
protected:
	float m, b;
public:
	LinearTimeFunction(float m, float b) : m(m), b(b) {}
	virtual float evaluate(int time);
};

class ConstantTimeFunction : public TimeFunction<float> {
private:
	ConstantTimeFunction();
protected:
	float value;
public:
	ConstantTimeFunction(float value) : value(value) {}
	inline virtual float evaluate(int time) {
		return value;
	}
};



class Vec3TimeFunction : public TimeFunction<glm::vec3> {
protected:
	TimeFunction<float> *f1, *f2, *f3;
public:
	Vec3TimeFunction(TimeFunction<float> *f1,
					 TimeFunction<float> *f2,
					 TimeFunction<float> *f3) {
		this->f1 = f1;
		this->f2 = f2;
		this->f3 = f3;
	}

	virtual glm::vec3 evaluate(int time);
};

