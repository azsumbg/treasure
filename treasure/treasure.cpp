#include "pch.h"
#include "treasure.h"

// RANDIT class *****************************

dll::RANDIT::RANDIT()
{
	std::random_device rd{};
	std::seed_seq sq{ rd(),rd(), rd(), rd(), rd(), rd(), rd(), rd() };

	twister = new std::mt19937(sq);
}
dll::RANDIT::~RANDIT()
{
	if (twister)delete twister;
}

int dll::RANDIT::operator()(int min, int max)
{
	if (max <= min)return 0;

	std::uniform_int_distribution distrib(min, max);

	return distrib(*twister);
}
float dll::RANDIT::operator()(float min, float max)
{
	if (max <= min)return 0;

	std::uniform_real_distribution<float> distrib(min, max);

	return distrib(*twister);
}

/////////////////////////////////////////////

// EXCEPTION class *************************

dll::EXCEPTION::EXCEPTION(int what_happened) :_what_happened{ what_happened } {};

const wchar_t* dll::EXCEPTION::getErr()const
{
	switch (_what_happened)
	{
	case ERR_INDEX:
		return L"Bad index passed to BAG object function !";
		
	case ERR_PTR:
		return L"Bad pointer passed to BAG object function !";

	case ERR_PARAM:
		return L"Bad parameter passed to BAG object function !";
	}

	return L"Unknown error occurred in BAG object !";
}

////////////////////////////////////////////

// PROTON class ***************************

dll::PROTON::PROTON()
{
	_width = 1.0f;
	_height = 1.0f;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	_my_rect.left = start.x;
	_my_rect.right = end.x;
	_my_rect.top = start.y;
	_my_rect.bottom = end.y;
}
dll::PROTON::PROTON(float _sx, float _sy)
{
	start.x = _sx;
	start.y = _sy;

	_width = 1.0f;
	_height = 1.0f;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	_my_rect.left = start.x;
	_my_rect.right = end.x;
	_my_rect.top = start.y;
	_my_rect.bottom = end.y;
}
dll::PROTON::PROTON(float _sx, float _sy, float _first_width, float _first_height)
{
	start.x = _sx;
	start.y = _sy;

	_width = _first_width;
	_height = _first_height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	_my_rect.left = start.x;
	_my_rect.right = end.x;
	_my_rect.top = start.y;
	_my_rect.bottom = end.y;
}

void dll::PROTON::set_edges()
{
	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	_my_rect.left = start.x;
	_my_rect.right = end.x;
	_my_rect.top = start.y;
	_my_rect.bottom = end.y;
}
void dll::PROTON::new_dims(float _new_width, float _new_height)
{
	_width = _new_width;
	_height = _new_height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	_my_rect.left = start.x;
	_my_rect.right = end.x;
	_my_rect.top = start.y;
	_my_rect.bottom = end.y;
}

void dll::PROTON::set_width(float _new_width)
{
	_width = _new_width;
	
	x_rad = _width / 2.0f;
	
	end.x = start.x + _width;
	
	center.x = start.x + x_rad;
	
	_my_rect.left = start.x;
	_my_rect.right = end.x;
}
void dll::PROTON::set_height(float _new_height)
{
	_height = 1.0f;

	y_rad = _height / 2.0f;

	end.y = start.y + _height;

	center.y = start.y + y_rad;

	_my_rect.top = start.y;
	_my_rect.bottom = end.y;
}

float dll::PROTON::get_width()const
{
	return _width;
}
float dll::PROTON::get_height()const
{
	return _height;
}
D2D1_RECT_F dll::PROTON::get_rect()const
{
	return _my_rect;
}

///////////////////////////////////////////

// FIELD class ****************************

dll::FIELD::FIELD(float _where_x, float _where_y)
{
	RANDIT _rand{};

	float tx = 0;
	float ty = 50.0f;

	for (int row = 0; row < FIELD_ROWS; ++row)
	{
		for (int col = 0; col < FIELD_COLS; ++col)
		{
			FieldArray[row][col].rect.left = tx;
			FieldArray[row][col].rect.right = tx + 50.0f;
			FieldArray[row][col].rect.top = ty;
			FieldArray[row][col].rect.bottom = ty + 50.0f;

			tx += 50.0f;
		}

		tx = 0;
		ty += 50.0f;
	}

	for (int row = 0; row < FIELD_ROWS - 2; ++row)
	{
		for (int col = 0; col < FIELD_COLS - 3; ++col)
		{
			if (_rand(0, 5) == 2)
			{
				FieldArray[row][col].is_water = true;
				FieldArray[row][col + 1].is_water = true;
				FieldArray[row][col + 2].is_water = true;

				FieldArray[row + 1][col].is_water = true;
				FieldArray[row + 1][col + 1].is_water = true;
				FieldArray[row + 1][col + 2].is_water = true;
			}
		}
	}
}

D2D1_RECT_F dll::FIELD::get_tile_rect(int row, int col)const
{
	return FieldArray[row][col].rect;
}

bool dll::FIELD::is_water_tile(int row, int col)const
{
	return FieldArray[row][col].is_water;
}

///////////////////////////////////////////

// NATURE class ***************************

dll::NATURE::NATURE(nature _what, float _sx_, float _sy_) :PROTON(_sx_, _sy_)
{
	type = _what;

	switch (type)
	{
	case nature::tree1:
		new_dims(90.0f, 90.0f);
		break;

	case nature::tree2:
		new_dims(90.0f, 90.0f);
		break;

	case nature::tree3:
		new_dims(90.0f, 90.0f);
		break;

	case nature::mountain1:
		new_dims(200.0f, 134.0f);
		break;

	case nature::mountain2:
		new_dims(250.0f, 112.0f);
		break;
	}
}

D2D1_RECT_F dll::NATURE::get_rect()const
{
	return _my_rect;
}

void dll::NATURE::Release()
{
	delete this;
}

dll::NATURE* dll::NATURE::create(nature what, float sx_, float sy_)
{
	NATURE* ret{ nullptr };

	ret = new NATURE(what, sx_, sy_);
	
	return ret;
}

///////////////////////////////////////////






















// FUNCTIONS *******************************

float dll::Distance(FPOINT first, FPOINT second)
{
	float a = (float)(pow(abs(second.x - first.x), 2.0f));
	float b = (float)(pow(abs(second.y - first.y), 2.0f));

	return (float)(sqrt(a + b));
}
bool dll::Intersect(D2D1_RECT_F first, D2D1_RECT_F second)
{
	if (!(first.left >= second.right || first.right <= second.left || first.top >= second.bottom || first.bottom <= second.top))
		return true;
	
	return false;
}
bool dll::Intersect(FPOINT first_center, FPOINT second_center, float first_xrad, float second_xrad,
	float first_yrad, float second_yrad)
{
	if (Distance(first_center, second_center) <= first_xrad + second_xrad
		&& Distance(first_center, second_center) <= first_yrad + second_yrad)return true;

	return false;
}
void dll::Sort(BAG<FPOINT>& bag, FPOINT ref)
{
	if (bag.size() < 2)return;
	else
	{
		bool ok = false;

		while (!ok)
		{
			ok = true;

			for (size_t i = 0; i < bag.size() - 1; ++i)
			{
				if (Distance(bag[i], ref) > Distance(bag[i + 1], ref))
				{
					FPOINT temp{ bag[i] };
					bag[i] = bag[i + 1];
					bag[i + 1] = temp;
					ok = false;
				}
			}
		}
	}
}