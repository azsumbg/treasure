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