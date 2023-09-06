﻿#include "Othello.h"
#include <DxLib.h>

// ファイルの読み込み
#include <fstream>
#include "../File/LoadCSV.h"

Othello::Othello() :
	cell{},
	initCell{},
	width(8),
	height(8)
{
}

void Othello::Init()
{
	cell.reserve(static_cast<size_t>(width * height));
	initCell.reserve(cell.size());
}

void Othello::Draw(int offsetX, int offsetY)
{
	int circleOffsetX = offsetX + circleSize / 2;
	int circleOffsetY = offsetY + circleSize / 2;

	for (int i = 0; i < width * height; i++)
	{
		if (cell[i] == Color::HOLE)
		{
			continue;
		}

		int x = i % width;
		int y = i / width;

		unsigned int color = GetColor(0x00, 0xF0, 0x00);
		if ((x + y) % 2) color = GetColor(0x00, 0xC0, 0x00);

		DrawBox(circleSize * x + offsetX, circleSize * y + offsetY,
				circleSize * (x + 1) + offsetX, circleSize * (y + 1) + offsetY, color, true);

		if (cell[i] == Color::BLACK)
		{
			color = GetColor(0, 0, 0);
		}
		else if (cell[i] == Color::WHITE)
		{
			color = GetColor(0xFF, 0xFF, 0xFF);
		}
		else
		{
			continue;
		}

		DrawCircle(circleSize * x + circleOffsetX, circleSize * y + circleOffsetY, circleSize / 2, color, true);
	}
}

void Othello::Reset()
{
	for (size_t i = 0; i < cell.size(); i++)
	{
		cell[i] = initCell[i];
	}
}

int Othello::Put(int x, int y, Color color)
{
	int index = y * width + x;
	if (cell[index] == Color::HOLE)
	{
		return 0;
	}

	int count = 0;
	Color other = Color::EMPTY; //相手の色
	if (color == Color::BLACK)
	{
		other = Color::WHITE;
	}
	else if (color == Color::WHITE)
	{
		other = Color::BLACK;
	}

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
			{
				continue;
			}
			if (x + i < 0 || y + j < 0 || x + i >= width || y + j >= height)
			{
				continue;
			}
			index = (y + j) * width + (x + i);
			if (cell[index] != other)
			{
				continue;
			}

			const int size = 8;
			for (int s = 2; s < size; s++)
			{
				index += j * width + i;
				if (index >= 0 && index < cell.size())
				{
					if (cell[index] == Color::EMPTY)
					{
						break;
					}

					if (cell[index] == color)
					{
						index = y * width + x;
						cell[index] = color;
						count += s;

						for (int n = 1; n < s; n++)
						{
							index += j * width + i;
							cell[index] = color;
						}

						break;
					}
				}
			}
		}
	}

	return count;
}

int Othello::Load(const std::string& filePath)
{
	if (filePath.empty())
	{
		return -1;
	}

	cell.clear();

	std::ifstream ifs(filePath);
	std::string str;

	if (ifs.fail())
	{
		OutputDebugStringA("ファイルが開けません。\n");
		return -1;
	}

	getline(ifs, str);
	int size[] = { 0, 0 };
	int i = 0;
	for (auto s : str)
	{
		if (s == ',' || s == '\n')
		{
			i++;
			continue;
		}
		else if (s >= '0' && s <= '9')
		{
			size[i] *= 10;
			size[i] += s - '0';
		}
	}

	width = size[0];
	height = size[1];

	int* cellArray = new int[(width * height)];
	File::LoadMapChip(ifs, cellArray, width * height);

	for (int i = 0; i < width * height; i++)
	{
		cell.push_back(static_cast<Color>(cellArray[i]));
	}

	initCell = cell;

	delete[] cellArray;
	ifs.close();
	return 0;
}

Color Othello::GetCell(const size_t& index)
{
	if (index < 0 || index >= cell.size())
	{
		return Color::NONE;
	}

	return cell[index];
}
