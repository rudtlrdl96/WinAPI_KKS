#include "TextActor.h"

std::vector<std::vector<TextActor*>> vecTextGrid;

TextActor::TextActor()
{
}

TextActor::~TextActor()
{
}


void TextActor::Init()
{

}

void TextActor::Input()
{

}

void TextActor::Update()
{

}

void TextActor::Render()
{

}


void TextActor::InitTextGrid(const float4& _Size)
{
	vecTextGrid.resize(_Size.iy());

	for (size_t y = 0; y < vecTextGrid.size(); y++)
	{
		vecTextGrid[y].resize(_Size.ix());

		for (size_t x = 0; x < vecTextGrid[y].size(); x++)
		{
			vecTextGrid[y][x] = nullptr;
		}
	}
}

void TextActor::DeleteTextGrid()
{
	for (size_t y = 0; y < vecTextGrid.size(); y++)
	{
		vecTextGrid[y].clear();
	}

	vecTextGrid.clear();
}

bool TextActor::Move(const float4& _Dir)
{
	if (0 == vecTextGrid.size())
	{
		MsgAssert("vecTextGrid를 초기화 하지 않고 이동하려 했습니다.");
		return false;
	}

	float4 NextPos = _Dir + GetPos();

	if (NextPos.iy() > vecTextGrid.size() || NextPos.ix() > vecTextGrid[0].size())
	{
		return false;
	}

	if (nullptr != vecTextGrid[NextPos.iy()][NextPos.ix()] &&
		false == vecTextGrid[NextPos.iy()][NextPos.ix()]->Move(_Dir))
	{
		return false;
	}
	
	// 이동...

	return true;
}

void TextActor::Interaction()
{
}
