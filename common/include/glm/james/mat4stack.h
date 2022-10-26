#pragma once
#include "../mat4x4.hpp"
#include <vector>

namespace glm
{
	// a simple helper class to represent a matrix stack
	class mat4stack
	{
	public:
		mat4stack() : mCurrent(1.f) {}
		~mat4stack() {};
		// push a new transformation onto the stack
		inline void push(const mat4& mat)
		{
			mStack.push_back(mCurrent);
			mCurrent = mCurrent * mat;
		}
		// pop one transformation back
		void pop()
		{
			if (mStack.empty()) return;
			mCurrent = mStack.back();
			mStack.pop_back();
		}
		// clear everything. back to the identity
		void clear()
		{
			mCurrent = mat4(1.f);
			mStack.clear();
		}
		// get the current transformation
		inline const mat4& getCurrent()
		{
			return mCurrent;
		}
	private:
		std::vector<mat4> mStack;
		mat4 mCurrent;
	};
}