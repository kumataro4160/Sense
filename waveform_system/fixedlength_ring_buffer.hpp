#pragma once

#include <array>


namespace sense
{
	template <class Signal, size_t N>
	struct FixedlengthRingBuffer
	{
	private:
		std::array<Signal, N> samples;
		int readPosition;

	public:
		constexpr FixedlengthRingBuffer()noexcept :
			readPosition(0)
		{

		}
		template <class T>
		constexpr FixedlengthRingBuffer(const T &t = 0.0)noexcept :
			readPosition(0)
		{
			std::fill(samples.begin(), samples.end(), Signal(t));
		}
		constexpr explicit FixedlengthRingBuffer(const std::array<Signal, N> &samples)noexcept :
			samples(samples), readPosition(0)
		{

		}
		template <class Converted>
		constexpr explicit operator FixedlengthRingBuffer<Converted, N>()const noexcept
		{
			FixedlengthRingBuffer<Converted, N> ret;
#pragma omp for
			for(size_t i = 0; i < N; ++i)
			{
				ret[i] = static_cast<Converted>(samples[i]);
			}
			return ret;
		}
		constexpr size_t getLength()const noexcept
		{
			return N;
		}
		constexpr longlong getFirstTime()const noexcept
		{
			return readPosition;
		}
		constexpr longlong getLastTime()const noexcept
		{
			return getLength() + getFirstTime();
		}
		constexpr Signal &operator[](int n)noexcept
		{
			return samples[(n % N + N + readPosition) % N];
		}
		constexpr const Signal &operator[](int n)const noexcept
		{
			return samples[(n % N + N + readPosition) % N];
		}
		constexpr auto begin()noexcept
		{
			return samples.begin();
		}
		constexpr auto begin()const noexcept
		{
			return samples.begin();
		}
		constexpr auto end()noexcept
		{
			return samples.end();
		}
		constexpr auto end()const noexcept
		{
			return samples.end();
		}
		constexpr FixedlengthRingBuffer &operator+=(const FixedlengthRingBuffer &multi)noexcept
		{
#pragma omp for
			for(size_t i = 0; i < N; ++i)
			{
				(*this)[i] += multi[i];
			}
			return *this;
		}
		constexpr FixedlengthRingBuffer &operator-=(const FixedlengthRingBuffer &multi)noexcept
		{
#pragma omp for
			for(size_t i = 0; i < N; ++i)
			{
				(*this)[i] -= multi[i];
			}
			return *this;
		}
		constexpr FixedlengthRingBuffer &operator*=(const FixedlengthRingBuffer &multi)noexcept
		{
#pragma omp for
			for(size_t i = 0; i < N; ++i)
			{
				(*this)[i] *= multi[i];
			}
			return *this;
		}
		constexpr FixedlengthRingBuffer &operator/=(const FixedlengthRingBuffer &multi)noexcept
		{
#pragma omp for
			for(size_t i = 0; i < N; ++i)
			{
				(*this)[i] /= multi[i];
			}
			return *this;
		}
		template <class T>
		constexpr FixedlengthRingBuffer &operator+=(const T &t)noexcept
		{
#pragma omp for
			for(size_t i = 0; i < N; ++i)
			{
				samples[i] += t;
			}
			return *this;
		}
		template <class T>
		constexpr FixedlengthRingBuffer &operator-=(const T &t)noexcept
		{
#pragma omp for
			for(size_t i = 0; i < N; ++i)
			{
				samples[i] -= t;
			}
			return *this;
		}
		template <class T>
		constexpr FixedlengthRingBuffer &operator*=(const T &t)noexcept
		{
#pragma omp for
			for(size_t i = 0; i < N; ++i)
			{
				samples[i] *= t;
			}
			return *this;
		}
		template <class T>
		constexpr FixedlengthRingBuffer &operator/=(const T &t)noexcept
		{
#pragma omp for
			for(size_t i = 0; i < N; ++i)
			{
				samples[i] /= t;
			}
			return *this;
		}
		constexpr FixedlengthRingBuffer operator-()const noexcept
		{
			return FixedlengthRingBuffer(*this) *= -1.0;
		}
		FixedlengthRingBuffer &pushFirstOnce()noexcept
		{
			readPosition = (readPosition + N - 1) % N;
			return *this;
		}
		FixedlengthRingBuffer &pushLastOnce()noexcept
		{
			readPosition = ++readPosition % N;
			return *this;
		}
		FixedlengthRingBuffer &operator<<=(int n)noexcept
		{
			readPosition = (readPosition + N + n % N) % N;
			return *this;
		}
		FixedlengthRingBuffer &operator>>=(int n)noexcept
		{
			return (*this) <<= -n;
		}
		FixedlengthRingBuffer &pushFirstOnce(const Signal &signal)noexcept
		{
			pushFirstOnce();
			(*this)[0] = signal;
			return *this;
		}
		FixedlengthRingBuffer &pushLastOnce(const Signal &signal)noexcept
		{
			pushLastOnce();
			(*this)[-1] = signal;
			return *this;
		}
	};

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator+(const FixedlengthRingBuffer<Signal, N> &left, const FixedlengthRingBuffer<Signal, N> &right)noexcept
	{
		return FixedlengthRingBuffer(left) += right;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator+(FixedlengthRingBuffer<Signal, N> &&left, const FixedlengthRingBuffer<Signal, N> &right)noexcept
	{
		return left += right;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator+(const FixedlengthRingBuffer<Signal, N> &left, FixedlengthRingBuffer<Signal, N> &&right)noexcept
	{
		return right += left;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator+(FixedlengthRingBuffer<Signal, N> &&left, FixedlengthRingBuffer<Signal, N> &&right)noexcept
	{
		return left += right;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator-(const FixedlengthRingBuffer<Signal, N> &left, const FixedlengthRingBuffer<Signal, N> &right)noexcept
	{
		return FixedlengthRingBuffer(left) -= right;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator-(FixedlengthRingBuffer<Signal, N> &&left, const FixedlengthRingBuffer<Signal, N> &right)noexcept
	{
		return left -= right;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator*(const FixedlengthRingBuffer<Signal, N> &left, const FixedlengthRingBuffer<Signal, N> &right)noexcept
	{
		return FixedlengthRingBuffer(left) *= right;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator*(FixedlengthRingBuffer<Signal, N> &&left, const FixedlengthRingBuffer<Signal, N> &right)noexcept
	{
		return left *= right;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator*(const FixedlengthRingBuffer<Signal, N> &left, FixedlengthRingBuffer<Signal, N> &&right)noexcept
	{
		return right *= left;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator*(FixedlengthRingBuffer<Signal, N> &&left, FixedlengthRingBuffer<Signal, N> &&right)noexcept
	{
		return left *= right;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator/(const FixedlengthRingBuffer<Signal, N> &left, const FixedlengthRingBuffer<Signal, N> &right)noexcept
	{
		return FixedlengthRingBuffer(left) /= right;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator/(FixedlengthRingBuffer<Signal, N> &&left, const FixedlengthRingBuffer<Signal, N> &right)noexcept
	{
		return left /= right;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator+(const FixedlengthRingBuffer<Signal, N> &left, const T &right)noexcept
	{
		return FixedlengthRingBuffer(left) += right;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator+(FixedlengthRingBuffer<Signal, N> &&left, const T &right)noexcept
	{
		return left += right;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator+(const T &left, const FixedlengthRingBuffer<Signal, N> &right)noexcept
	{
		return FixedlengthRingBuffer(right) += left;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator+(const T &left, FixedlengthRingBuffer<Signal, N> &&right)noexcept
	{
		return right += left;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator-(const FixedlengthRingBuffer<Signal, N> &left, const T &right)noexcept
	{
		return FixedlengthRingBuffer(left) -= right;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator-(FixedlengthRingBuffer<Signal, N> &&left, const T &right)noexcept
	{
		return left -= right;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator-(const T &left, const FixedlengthRingBuffer<Signal, N> &right)noexcept
	{
		return (-right) += left;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator*(const FixedlengthRingBuffer<Signal, N> &left, const T &right)noexcept
	{
		return FixedlengthRingBuffer(left) *= right;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator*(FixedlengthRingBuffer<Signal, N> &&left, const T &right)noexcept
	{
		return left *= right;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator*(const T &left, const FixedlengthRingBuffer<Signal, N> &right)noexcept
	{
		return FixedlengthRingBuffer(right) *= left;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator*(const T &left, FixedlengthRingBuffer<Signal, N> &&right)noexcept
	{
		return right *= left;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator/(const FixedlengthRingBuffer<Signal, N> &left, const T &right)noexcept
	{
		return FixedlengthRingBuffer(left) /= right;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator/(FixedlengthRingBuffer<Signal, N> &&left, const T &right)noexcept
	{
		return left /= right;
	}

	template <class Signal, size_t N, class T>
	constexpr FixedlengthRingBuffer<Signal, N> operator/(const T &left, const FixedlengthRingBuffer<Signal, N> &right)noexcept
	{
		FixedlengthRingBuffer<Signal, N> ret;
#pragma omp for
		for(size_t i = 0; i < N; ++i)
		{
			ret[i] = left / right[i];
		}
		return ret;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator<<(const FixedlengthRingBuffer<Signal, N> &left, int right)noexcept
	{
		return FixedlengthRingBuffer(left) <<= right;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator<<(FixedlengthRingBuffer<Signal, N> &&left, int right)noexcept
	{
		return left <<= right;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator>>(const FixedlengthRingBuffer<Signal, N> &left, int right)noexcept
	{
		return FixedlengthRingBuffer(left) >>= right;
	}

	template <class Signal, size_t N>
	constexpr FixedlengthRingBuffer<Signal, N> operator>>(FixedlengthRingBuffer<Signal, N> &&left, int right)noexcept
	{
		return left >>= right;
	}
}