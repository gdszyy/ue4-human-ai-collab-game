#include "PCG/ShapeGenerator.h"
#include "SimplexNoise.h"

FShapeData UShapeGenerator::GenerateShapeWithCellularAutomata(int32 Width, int32 Height, int32 Seed, int32 Iterations, int32 BirthThreshold, int32 SurvivalThreshold)
{
    FShapeData Shape;
    Shape.Width = Width;
    Shape.Height = Height;
    Shape.Grid.Init(false, Width * Height);

    // 1. Random Initialization
    FRandomStream RandStream(Seed);
    for (int32 y = 0; y < Height; ++y)
    {
        for (int32 x = 0; x < Width; ++x)
        {
            Shape.Grid[y * Width + x] = RandStream.FRand() < 0.5f;
        }
    }

    // 2. Simulation
    for (int32 i = 0; i < Iterations; ++i)
    {
        TArray<bool> NextGrid = Shape.Grid;
        for (int32 y = 0; y < Height; ++y)
        {
            for (int32 x = 0; x < Width; ++x)
            {
                int32 Neighbors = 0;
                for (int32 ny = -1; ny <= 1; ++ny)
                {
                    for (int32 nx = -1; nx <= 1; ++nx)
                    {
                        if (nx == 0 && ny == 0) continue;
                        int32 CheckX = x + nx;
                        int32 CheckY = y + ny;
                        if (CheckX >= 0 && CheckX < Width && CheckY >= 0 && CheckY < Height && Shape.Grid[CheckY * Width + CheckX])
                        {
                            Neighbors++;
                        }
                    }
                }

                int32 CurrentIndex = y * Width + x;
                if (Shape.Grid[CurrentIndex] && Neighbors < SurvivalThreshold)
                {
                    NextGrid[CurrentIndex] = false;
                }
                else if (!Shape.Grid[CurrentIndex] && Neighbors > BirthThreshold)
                {
                    NextGrid[CurrentIndex] = true;
                }
            }
        }
        Shape.Grid = NextGrid;
    }

    return Shape;
}

FShapeData UShapeGenerator::GenerateShapeWithSimplexNoise(int32 Width, int32 Height, int32 Seed, float Scale, float Threshold)
{
    FShapeData Shape;
    Shape.Width = Width;
    Shape.Height = Height;
    Shape.Grid.Init(false, Width * Height);

    USimplexNoise::setSeed(Seed);

    for (int32 y = 0; y < Height; ++y)
    {
        for (int32 x = 0; x < Width; ++x)
        {
            float NoiseValue = USimplexNoise::noise2D(x / Scale, y / Scale);
            Shape.Grid[y * Width + x] = NoiseValue > Threshold;
        }
    }

    return Shape;
}
