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

// ACTION class ***************************

dll::ACTION::ACTION(moveables _who, float _x, float _y) :PROTON(_x, _y)
{
	type = _who;

	switch (type)
	{
	case moveables::shot:
		new_dims(11.0f, 11.0f);
		_speed = 5.0f;
		break;

	case moveables::flyer:
		new_dims(41.0f, 65.0f);
		_speed = 1.1f;
		max_frames = 10;
		frame_delay = 6;
		damage = 5;
		max_attack_delay = 80;
		lifes = 100;
		break;

	case moveables::girl:
		new_dims(48.0f, 65.0f);
		_speed = 0.8f;
		max_frames = 25;
		frame_delay = 3;
		damage = 9;
		max_attack_delay = 100;
		lifes = 110;
		break;

	case moveables::soul:
		new_dims(28.0f, 50.0f);
		_speed = 1.0f;
		max_frames = 119;
		frame_delay = 1;
		damage = 6;
		max_attack_delay = 105;
		lifes = 90;
		break;

	case moveables::zombie:
		new_dims(64.0f, 75.0f);
		_speed = 0.4f;
		max_frames = 3;
		frame_delay = 22;
		damage = 10;
		max_attack_delay = 120;
		lifes = 150;
		break;

	case moveables::hero:
		new_dims(80.0f, 80.0f);
		_speed = 1.5f;
		max_frames = 31;
		frame_delay = 2;
		damage = 20;
		lifes = 150;
		break;
	}

	max_attack_delay = attack_delay;
	max_frame_delay = frame_delay;
}

void dll::ACTION::set_path(float target_x, float target_y)
{
	ver_dir = false;
	hor_dir = false;

	move_sx = start.x;
	move_sy = start.y;

	move_ex = target_x;
	move_ey = target_y;

	if (move_sx == move_ex || (move_ex > move_sx && move_ex <= end.x))
	{
		ver_dir = true;
		return;
	}
	if (move_sy == move_ey || (move_ey > move_sy && move_ey <= end.y))
	{
		hor_dir = true;
		return;
	}

	slope = (move_ey - move_sy) / (move_ex - move_sx);
	intercept = start.y - slope * start.x;
}

float dll::ACTION::get_target_x() const
{
	return move_ex;
}
float dll::ACTION::get_target_y() const
{
	return move_ey;
}

bool dll::ACTION::move(float gear)
{
	float my_speed = _speed + gear / 10.0f;

	if (hor_dir)
	{
		if (move_ex < move_sx)
		{
			start.x -= my_speed;
			set_edges();
			if (end.x <= 0)return false;
		}
		else if (move_ex > move_sx)
		{
			start.x += my_speed;
			set_edges();
			if (start.x >= scr_width)return false;
		}
		else return false;
	}
	else if (ver_dir)
	{
		if (move_ey < move_sy)
		{
			start.y -= my_speed;
			set_edges();
			if (end.y <= sky)return false;
		}
		else if (move_ey > move_sy)
		{
			start.y += my_speed;
			set_edges();
			if (start.y >= ground)return false;
		}
		else return false;
	}
	else
	{
		if (move_ex < move_sx)
		{
			start.x -= my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (end.x <= 0 || start.x >= scr_width || end.y <= sky || start.y >= ground)return false;
		}
		else if (move_ex > move_sx)
		{
			start.x += my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (end.x <= 0 || start.x >= scr_width || end.y <= sky || start.y >= ground)return false;
		}
		else return false;
	}

	return true;
}

///////////////////////////////////////////

// SHOT class *****************************

dll::SHOT::SHOT(float _sx, float _sy, float _ex, float _ey, int _power) :ACTION(moveables::shot, _sx, _sy)
{
	damage = _power;

	set_path(_ex, _ey);
}
int dll::SHOT::get_frame()
{
	return 0;
}
int dll::SHOT::attack()
{
	return damage;
}

void dll::SHOT::Release()
{
	delete this;
}

dll::SHOT* dll::SHOT::create(float sx, float sy, float ex, float ey, int power)
{
	SHOT* ret{ nullptr };
	
	ret = new SHOT(sx, sy, ex, ey, power);

	return ret;
}

//////////////////////////////////////////

// EVIL class *****************************

dll::EVIL::EVIL(moveables _who, float _sx, float _sy, float _ex, float _ey) :ACTION(_who, _sx, _sy)
{
	set_path(_ex, _ey);

	if (move_ex > move_sx)dir = dirs::right;
	else dir = dirs::left;
}

int dll::EVIL::get_frame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++frame;
		if (frame > max_frames)frame = 0;
	}

	return frame;
}
int dll::EVIL::attack()
{
	--attack_delay;
	if (attack_delay <= 0)
	{
		attack_delay = max_attack_delay;
		return damage;
	}

	return 0;
}
void dll::EVIL::Release()
{
	delete this;
}

dll::EVIL* dll::EVIL::create(moveables who, float sx, float sy, float ex, float ey)
{
	EVIL* ret{ nullptr };

	ret = new EVIL(who, sx, sy, ex, ey);

	return ret;
}

////////////////////////////////////////////











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