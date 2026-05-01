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