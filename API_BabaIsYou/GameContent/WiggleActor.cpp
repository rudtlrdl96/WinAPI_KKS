#include "WiggleActor.h"
#include "ContentConst.h"

size_t WiggleActor::WiggleIndex = 0;

WiggleActor::WiggleActor()
{
}

WiggleActor::~WiggleActor()
{
}

void WiggleActor::Wiggle()
{
	++WiggleIndex;

	if (ContentConst::WIGGLE_MAXTICK <= WiggleIndex)
	{
		WiggleIndex = 0;
	}
}
