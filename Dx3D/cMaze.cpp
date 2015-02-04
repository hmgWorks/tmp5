#include "stdafx.h"
#include "cMaze.h"


cMaze::cMaze()
{
}

cMaze::~cMaze()
{
}

void cMaze::Setup()
{
	FILE* pFile;
	fopen_s(&pFile, "data.txt", "r");
	while (!feof(pFile))
	{

	}

	fclose(pFile);
}

void cMaze::Render()
{

}
