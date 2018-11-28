#pragma once

namespace epicyclism
{
	template <typename I1, typename I2, typename F> constexpr I2 ctransform(I1 b1, I1 e1, I2 b2, F f)
	{
		while (b1 != e1)
		{
			*b2 = f(*b1);
			++b1, ++b2;
		}
		return b2;
	}

	template <typename I1, typename I2, typename F> constexpr I2 ctransform(I1 i1, I1 e1, I1 i2, I2 o, F f)
	{
		while (i1 != e1)
		{
			*o = f(*i1, *i2);
			++i1, ++i2, ++o;
		}
		return o;
	}

	template <typename I1, typename F> constexpr void cfor_each(I1 b, I1 e, F f)
	{
		while (b != e)
		{
			f(*b);
			++b;
		}
	}

	template<typename I> constexpr void qsort(I b, I e)
	{
		auto swap = [&](I l, I r)
		{
			auto tmp = *l;
			*l = *r;
			*r = tmp;
		};
		auto partition = [&](I l, I r)
		{
			--r;
			auto v = *r;
			auto i = l;
			while (l < r)
			{
				if (*l < v)
				{
					swap(i, l);
					++i;
				}
				++l;
			}
			swap(i, r);
			return i;
		};
		if (b < e)
		{
			I sp = partition(b, e);
			qsort(b, sp);
			qsort(sp + 1, e);
		}
	}

	template<typename I, typename F> constexpr void qsort(I b, I e, F f)
	{
		auto const swap = [&](I l, I r)
		{
			auto tmp = *l;
			*l = *r;
			*r = tmp;
		};
		auto const partition = [&](I l, I r)
		{
			--r;
			auto v = *r;
			auto i = l;
			while (l < r)
			{
				if (f(*l,  v))
				{
					swap(i, l);
					++i;
				}
				++l;
			}
			swap(i, r);
			return i;
		};
		if (b < e)
		{
			I sp = partition(b, e);
			qsort(b, sp, f);
			qsort(sp + 1, e, f);
		}
	}
}