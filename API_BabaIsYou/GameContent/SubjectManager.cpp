#include "SubjectManager.h"
#include "ContentConst.h"
#include "GridActor.h"

SubjectManager::SubjectManager()
{
}

SubjectManager::~SubjectManager()
{
	GridActor::DeleteGrid();
}

void SubjectManager::InitGrid()
{
	GridActor::InitGrid(ContentConst::GridSize, ContentConst::PuzzleActorSize);
}