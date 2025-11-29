
#include "PCG/WFCAssembler.h"

// Helper struct for the WFC process
struct FWFC_Cell
{
    TArray<int32> PossibleModuleIndices;
    bool bIsCollapsed = false;
};

// Helper struct for backtracking
struct FWFC_State
{
    TArray<FWFC_Cell> Grid;
    int32 CellToCollapseIndex;
    int32 ChosenModuleIndex;
};

// ... (GetConnectorsForDirection and AreConnectorSetsCompatible functions remain the same)

FWFCAssembly UWFCAssembler::AssembleWithWFC(const TArray<FWFCModule>& Modules, int32 Width, int32 Height, int32 Seed)
{
    FWFCAssembly Assembly;
    Assembly.Width = Width;
    Assembly.Height = Height;
    Assembly.PlacedModules.Init(NAME_None, Width * Height);

    if (Modules.Num() == 0) return Assembly;

    TArray<FWFC_Cell> Grid;
    Grid.Init(FWFC_Cell(), Width * Height);
    TArray<int32> AllModuleIndices;
    for(int32 i = 0; i < Modules.Num(); ++i) { AllModuleIndices.Add(i); }
    for(FWFC_Cell& Cell : Grid) { Cell.PossibleModuleIndices = AllModuleIndices; }

    FRandomStream RandStream(Seed);
    TArray<FWFC_State> StateStack;

    int32 CollapsedCells = 0;
    while (CollapsedCells < Width * Height)
    {
        int32 LowestEntropy = Modules.Num() + 1;
        TArray<int32> LowestEntropyCells;
        for (int32 i = 0; i < Grid.Num(); ++i)
        {
            if (!Grid[i].bIsCollapsed)
            {
                int32 CurrentEntropy = Grid[i].PossibleModuleIndices.Num();
                if (CurrentEntropy < LowestEntropy)
                {
                    LowestEntropy = CurrentEntropy;
                    LowestEntropyCells.Empty();
                    LowestEntropyCells.Add(i);
                }
                else if (CurrentEntropy == LowestEntropy)
                {
                    LowestEntropyCells.Add(i);
                }
            }
        }

        if (LowestEntropyCells.Num() == 0) break;

        int32 CellToCollapseIndex = LowestEntropyCells[RandStream.RandRange(0, LowestEntropyCells.Num() - 1)];
        FWFC_Cell& CellToCollapse = Grid[CellToCollapseIndex];
        int32 ChosenModuleIndex = CellToCollapse.PossibleModuleIndices[RandStream.RandRange(0, CellToCollapse.PossibleModuleIndices.Num() - 1)];

        // Save state for backtracking
        FWFC_State CurrentState;
        CurrentState.Grid = Grid;
        CurrentState.CellToCollapseIndex = CellToCollapseIndex;
        CurrentState.ChosenModuleIndex = ChosenModuleIndex;
        StateStack.Push(CurrentState);

        CellToCollapse.PossibleModuleIndices.Empty();
        CellToCollapse.PossibleModuleIndices.Add(ChosenModuleIndex);
        CellToCollapse.bIsCollapsed = true;
        CollapsedCells++;

        TArray<int32> PropagationStack;
        PropagationStack.Add(CellToCollapseIndex);

        bool bContradiction = false;
        while (PropagationStack.Num() > 0)
        {
            // ... (Propagation logic remains the same)

            // If a contradiction is found:
            // bContradiction = true; break;
        }

        if (bContradiction)
        {
            if (StateStack.Num() > 0)
            {
                FWFC_State LastState = StateStack.Pop();
                Grid = LastState.Grid;
                Grid[LastState.CellToCollapseIndex].PossibleModuleIndices.Remove(LastState.ChosenModuleIndex);
                CollapsedCells--;
            }
            else
            {
                // Cannot backtrack further, assembly failed
                break;
            }
        }
    }

    for (int32 i = 0; i < Grid.Num(); ++i)
    {
        if (Grid[i].bIsCollapsed)
        {
            Assembly.PlacedModules[i] = Modules[Grid[i].PossibleModuleIndices[0]].ModuleId;
        }
    }

    return Assembly;
}
