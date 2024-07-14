#include <stdlib.h>
#include <stdio.h>

//Simple structure for point with x and y
struct Point
{
    int x, y;
};

int RocketRadius = 0, AllTargetsNum = 0;
struct Point* AllTargets = NULL;

void AddPointToArray(struct Point Input, struct Point** Array, int* ArraySize)
{
    *Array = realloc(*Array, sizeof(struct Point) * (*ArraySize + 1));
    (*Array)[*ArraySize].x = Input.x;
    (*Array)[*ArraySize].y = Input.y;
    (*ArraySize)++;
    return;
}

//Parsing input file and creates array of targets on map
int ParseInputFile(char* Filename)
{
    FILE* InputFile = fopen(Filename, "r");
    char tmp;
    struct Point tmpPoint;
    if (InputFile == NULL)
    {
        printf("%s file not found.\n", Filename);
        return -1;
    }

    AllTargets = calloc(sizeof(struct Point), 1);
    while (1)
    {
        AllTargets = realloc(AllTargets, sizeof(struct Point) * (AllTargetsNum + 1));
        tmp = fscanf(InputFile, "%d, %d", &tmpPoint.x, &tmpPoint.y);
        if (tmp == EOF)
            break;
        AllTargets[AllTargetsNum].x = tmpPoint.x;
        AllTargets[AllTargetsNum].y = tmpPoint.y;
        AllTargetsNum++;
    }
    return AllTargetsNum;
}

//Distance in square to avoid sqrt() function
int DistanceSquared(struct Point First, struct Point Second)
{
    return (First.x - Second.x) * (First.x - Second.x) + (First.y - Second.y) * (First.y - Second.y);
}

//Find reachable points from Targets in MaxDistance. 
//If NULL provided for "Reachable", it returns only amount of reachable targets
int ReachableTargets(struct Point AimedTarget, struct Point* Targets, int TargetsNum, struct Point** Reachable, int MaxDistance)
{
    int i, tmpDistance, ReachableNum = 0;
    for (i = 0; i < TargetsNum; i++)
    {
        if (DistanceSquared(AimedTarget, Targets[i]) <= (MaxDistance * MaxDistance)) //Different points
        {
            if (Reachable != NULL)
                AddPointToArray(AllTargets[i], Reachable, &ReachableNum);
            else
                ReachableNum++;
        }
    }
    return ReachableNum;
}

//Calculates middle point of "Targets" by average of coordinates
struct Point CalculateMiddlePoint(struct Point* Targets, int TargetsNum, int* Set, int SetSize)
{
    struct Point Middle;
    Middle.x = 0;
    Middle.y = 0;

    printf("Middle point of");

    for (int i = 0; i < SetSize; i++)
    {
        Middle.x += Targets[Set[i] - 1].x;
        Middle.y += Targets[Set[i] - 1].y;
        printf(" (%d, %d)", Targets[Set[i] - 1].x, Targets[Set[i] - 1].y);
    }
    Middle.x /= SetSize;
    Middle.y /= SetSize;

    printf(" elements: (%d, %d)\n", Middle.x, Middle.y);

    return Middle;
}

//Generates different sets of combinations that is used for indexes in "Targets" to enumerate variants of shot
int GenerateCombinations(int MaxIndex, int End, int Index, int* Set, int SetSize, struct Point* Targets, int TargetsNum, struct Point* IdealShot) {
    int MaxHits = 0, tmp;
    struct Point tmpShot;
    if (Index == SetSize) 
    {
        *IdealShot = CalculateMiddlePoint(Targets, TargetsNum, Set, SetSize);
        return ReachableTargets(*IdealShot, Targets, TargetsNum, NULL, RocketRadius);
    }

    for (int i = End; i >= 1; i--) 
    {
        Set[Index] = i;
        tmp = GenerateCombinations(MaxIndex, i - 1, Index + 1, Set, SetSize, Targets, TargetsNum, &tmpShot);

        if (tmp > MaxHits)
        {
            MaxHits = tmp;
            IdealShot -> x = tmpShot.x;
            IdealShot -> y = tmpShot.y;
        }
    }
    return MaxHits;
}

//Enumerate all opportunities of shooting "MainTarget"
int ScanTarget(struct Point MainTarget, struct Point* Reachable, int ReachableNum, struct Point* IdealShot)
{
    int IndexesSet[AllTargetsNum];
    int i, SetSize, tmp, MaxHits = 0;
    struct Point tmpShot;
    for (SetSize = ReachableNum; SetSize > 0; SetSize--)
    {
        tmp = GenerateCombinations(ReachableNum, ReachableNum, 0, IndexesSet, SetSize, Reachable, ReachableNum, &tmpShot);
        if (tmp > MaxHits)
        {
            MaxHits = tmp;
            IdealShot -> x = tmpShot.x;
            IdealShot -> y = tmpShot.y;
        }
        if (tmp == SetSize)
            break; //Can't get more targets
    }
    return MaxHits;
}

//Main algorithm for aiming
void Aim()
{
    int i, ReachableNum;
    struct Point *Reachable;
    struct Point IdealShot, tmpShot;
    int MaxShotedTargets = 0, tmp;
    for (i = 0; i < AllTargetsNum; i++)
    {
        Reachable = calloc(1, sizeof(struct Point));
        ReachableNum = ReachableTargets(AllTargets[i], AllTargets, AllTargetsNum, &Reachable, 2 * RocketRadius);

        tmp = ScanTarget(AllTargets[i], Reachable, ReachableNum, &tmpShot);
        if (tmp > MaxShotedTargets)
        {
            MaxShotedTargets = tmp;
            IdealShot.x = tmpShot.x;
            IdealShot.y = tmpShot.y;
        }

        free(Reachable);
    }
    printf("Shoot in (%d, %d) to destroy %d targets\n", IdealShot.x, IdealShot.y, MaxShotedTargets);
    return;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        //Not enough args
        printf("Usage: %s <filename> <radius>\n", argv[0]);
        return -1;
    }
    else
    {
        AllTargetsNum = ParseInputFile(argv[1]);
        if (AllTargetsNum == 0)
        {
            //If input is empty
            printf("%s is empty\n", argv[1]);
            return -1;
        }
        RocketRadius = atoi(argv[2]);
        Aim();
    }
    free(AllTargets);
}