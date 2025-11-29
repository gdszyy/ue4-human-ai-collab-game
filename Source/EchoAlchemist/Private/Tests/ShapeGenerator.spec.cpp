#include "CoreTypes.h"
#include "Containers/UnrealString.h"
#include "Misc/AutomationTest.h"
#include "PCG/ShapeGenerator.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShapeGeneratorTest, "Project.UnitTests.PCG.ShapeGenerator", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FShapeGeneratorTest::RunTest(const FString& Parameters)
{
    // Test 1: Cellular Automata
    FShapeData CAShape = UShapeGenerator::GenerateShapeWithCellularAutomata(10, 10, 123);
    TestEqual("CA Shape should have correct width", CAShape.Width, 10);
    TestEqual("CA Shape should have correct height", CAShape.Height, 10);
    TestEqual("CA Shape grid should have 100 elements", CAShape.Grid.Num(), 100);

    // Test 2: Simplex Noise
    FShapeData NoiseShape = UShapeGenerator::GenerateShapeWithSimplexNoise(20, 20, 456);
    TestEqual("Noise Shape should have correct width", NoiseShape.Width, 20);
    TestEqual("Noise Shape should have correct height", NoiseShape.Height, 20);
    TestEqual("Noise Shape grid should have 400 elements", NoiseShape.Grid.Num(), 400);

    return true;
}

#endif //WITH_DEV_AUTOMATION_TESTS
