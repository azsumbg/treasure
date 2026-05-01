#pragma once

#ifdef TREASURE_EXPORTS
#define TREASURE_API __declspec(dllexport)
#else 
#define TREASURE_API __declspec(dllimport)
#endif

#include <random>
#include <d2d1.h>

#pragma comment(lib, "d2d1.lib")

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 800.0f };

constexpr float sky{ 50.0f };
constexpr float ground{ 750.0f };

constexpr int ERR_PTR{ 5001 };
constexpr int ERR_INDEX{ 5002 };
constexpr int ERR_PARAM{ 5003 };
constexpr int ERR_UNK{ 5004 };

enum class dirs { up = 0, down = 1, left = 2, right = 3, stop = 4 };

enum class nature { tree1 = 0, tree2 = 1, tree3 = 2, mountain1 = 3, mountain2 = 3 };
enum class evils { soul = 0, flyer = 1, zombie = 2, girl = 3 };
enum class assets { gold = 0, life = 1, gun = 2, armor = 3, map = 4 };

struct TREASURE_API FPOINT
{
	float x{ 0 };
	float y{ 0 };
};

struct TREASURE_API FADE
{
	assets type{ assets::gold };

	FPOINT position{};

	int delay{ 80 };

	float opacity = 1.0f;

	float get_opacity()
	{
		--delay;
		if (delay == 0)
		{
			delay = 80;
			opacity -= 0.1f;
		}
		return opacity;
	}
};

struct TREASURE_API TILE
{
	bool is_water = false;
	D2D1_RECT_F rect{};
};

namespace dll
{
	class TREASURE_API RANDIT
	{
	private:
		std::mt19937* twister{ nullptr };

	public:
		RANDIT();
		~RANDIT();

		int operator()(int min, int max);
		float operator()(float min, float max);
	};

	class TREASURE_API EXCEPTION
	{
	private:
		int _what_happened = 0;

		EXCEPTION(int what_happened);

		const wchar_t* getErr()const;
	};

	template<typename T>class BAG
	{
	private:
		size_t max_size{ 1 };
		T* m_ptr{ nullptr };
		size_t next_pos{ 0 };

	public:
		BAG() :m_ptr{ reinterpret_cast<T*>(calloc(max_size, sizeof(T))) } {};
		BAG(size_t size)
		{
			max_size = size;
			m_ptr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
		}
		BAG(BAG& other)
		{
			if (!other.m_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				max_size = other.max_size;
				next_pos = other.next_pos;

				m_ptr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));

				if (other.next_pos > 0) for (size_t i = 0; i < other.next_pos; ++i)m_ptr[i] = other.m_ptr[i];
			}
		}
		BAG(BAG&& other)
		{
			if (!other.m_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				m_ptr = other.m_ptr;
				max_size = other.max_size;
				next_pos = other.next_pos;

				other.m_ptr = nullptr;
			}
		}

		~BAG()
		{
			free(m_ptr);
		}

		BAG& operator=(BAG& other)
		{
			free(m_ptr);
			m_ptr = nullptr;

			if (!other.m_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				max_size = other.max_size;
				next_pos = other.next_pos;

				m_ptr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));

				if (other.next_pos > 0) for (size_t i = 0; i < other.next_pos; ++i)m_ptr[i] = other.m_ptr[i];
			}

			return *this;
		}
		BAG& operator=(BAG&& other)
		{
			free(m_ptr);
			m_ptr = nullptr;

			if (!other.m_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				m_ptr = other.m_ptr;
				max_size = other.max_size;
				next_pos = other.next_pos;

				other.m_ptr = nullptr;
			}

			return *this;
		}

		T& operator[](size_t index)
		{
			if (!m_ptr)throw EXCEPTION(ERR_PTR);
			if (index < 0 || index >= next_pos)throw EXCEPTION(ERR_INDEX);

			return m_ptr[index];
		}

		bool empty()const
		{
			return(next_pos == 0);
		}
		size_t size()const
		{
			return next_pos;
		}
		size_t capacity()const
		{
			return max_size;
		}

		void clear()
		{
			free(m_ptr);
			max_size = 1;
			next_pos = 0;

			m_ptr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
		}

		void push_back(T element)
		{
			if (!m_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					m_ptr[next_pos] = element;
					++next_pos;
				}
				else
				{
					++max_size;
					
					T* dummy = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
					
					if (!dummy)throw EXCEPTION(ERR_PTR);
					else
					{
						m_ptr = dummy;
						dummy = nullptr;

						m_ptr[next_pos] = element;
						++next_pos;
					}
				}
			}
		}
		void push_back(T* element)
		{
			if (!m_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					m_ptr[next_pos] = *element;
					++next_pos;
				}
				else
				{
					++max_size;

					T* dummy = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));

					if (!dummy)throw EXCEPTION(ERR_PTR);
					else
					{
						m_ptr = dummy;
						dummy = nullptr;

						m_ptr[next_pos] = *element;
						++next_pos;
					}
				}
			}
		}

		void erase(size_t index)
		{
			if (!m_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (index < 0 || index >= next_pos)throw EXCEPTION(ERR_INDEX);
				
				if (next_pos - 1 == index)--next_pos;
				else
				{
					for (size_t i = index; i < next_pos - 1; ++i)m_ptr[i] = m_ptr[i + 1];
					--next_pos;
				}
			}
		}

		void push_front(T element)
		{
			if (!m_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (next_pos == 0)
				{
					*m_ptr = element;
					++next_pos;
				}
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t i = next_pos; i > 0; --i)m_ptr[i] = m_ptr[i - 1];
						*m_ptr = element;
						++next_pos;
					}
					else
					{
						++max_size;
						T* dummy = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
						if (!dummy)throw EXCEPTION(ERR_PTR);
						else
						{
							m_ptr = dummy;
							dummy = nullptr;

							for (size_t i = next_pos; i > 0; --i)m_ptr[i] = m_ptr[i - 1];
							*m_ptr = element;
							++next_pos;
						}
					}
				}
			}
		}
		void push_front(T* element)
		{
			if (!m_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (next_pos == 0)
				{
					*m_ptr = *element;
					++next_pos;
				}
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t i = next_pos; i > 0; --i)m_ptr[i] = m_ptr[i - 1];
						*m_ptr = *element;
						++next_pos;
					}
					else
					{
						++max_size;
						T* dummy = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
						if (!dummy)throw EXCEPTION(ERR_PTR);
						else
						{
							m_ptr = dummy;
							dummy = nullptr;

							for (size_t i = next_pos; i > 0; --i)m_ptr[i] = m_ptr[i - 1];
							*m_ptr = *element;
							++next_pos;
						}
					}
				}
			}
		}

		void insert(T element, size_t index)
		{
			if (!m_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (index < 0 || index > next_pos)throw EXCEPTION(ERR_INDEX);

				if (next_pos == 0)
				{
					*m_ptr = element;
					++next_pos;
				}
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t i = next_pos; i > index; --i)m_ptr[i] = m_ptr[i - 1];
						m_ptr[index] = element;
						++next_pos;
					}
					else
					{
						++max_size;
						T* dummy = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
						if (!dummy)throw EXCEPTION(ERR_PTR);
						else
						{
							m_ptr = dummy;
							dummy = nullptr;

							for (size_t i = next_pos; i > index; --i)m_ptr[i] = m_ptr[i - 1];
							m_ptr[index] = element;
							++next_pos;
						}
					}
				}
			}
		}
		void insert(T* element, size_t index)
		{
			if (!m_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (index < 0 || index > next_pos)throw EXCEPTION(ERR_INDEX);

				if (next_pos == 0)
				{
					*m_ptr = *element;
					++next_pos;
				}
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t i = next_pos; i > index; --i)m_ptr[i] = m_ptr[i - 1];
						m_ptr[index] = *element;
						++next_pos;
					}
					else
					{
						++max_size;
						T* dummy = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
						if (!dummy)throw EXCEPTION(ERR_PTR);
						else
						{
							m_ptr = dummy;
							dummy = nullptr;

							for (size_t i = next_pos; i > index; --i)m_ptr[i] = m_ptr[i - 1];
							m_ptr[index] = *element;
							++next_pos;
						}
					}
				}
			}
		}

	};











}

