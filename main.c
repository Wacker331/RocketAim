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
    if (InputFile == NULL)
    {
        printf("%s file not found.\n", Filename);
        return -1;
    }

    AllTargets = calloc(sizeof(struct Point), 1);
    while (1)
    {
        tmp = fscanf(InputFile, "%d, %d", &AllTargets[AllTargetsNum].x, &AllTargets[AllTargetsNum].y);
        if (tmp == EOF)
            break;
        AllTargetsNum++;
        AllTargets = realloc(AllTargets, sizeof(struct Point) * (AllTargetsNum + 1));
    }
    return AllTargetsNum;
}

//Distance in square to avoid sqrt() function
int DistanceSquared(struct Point First, struct Point Second)
{
    return (First.x - Second.x) * (First.x - Second.x) + (First.y - Second.y) * (First.y - Second.y);
}

//Find reachable points from Targets in MaxDistance
int ReachableTargets(struct Point AimedTarget, struct Point* Targets, int TargetsNum, struct Point** Reachable, int MaxDistance)
{
    int i, tmpDistance, ReachableNum = 0;
    for (i = 0; i < TargetsNum; i++)
    {
        if (((tmpDistance = DistanceSquared(AimedTarget, Targets[i])) <= MaxDistance * MaxDistance) && tmpDistance != 0) //Different points
        {
            AddPointToArray(AllTargets[i], Reachable, &ReachableNum);
        }
    }
    return ReachableNum;
}

//Main algorithm for aiming
void Aim()
{
    int i, ReachableNum;
    struct Point *Reachable = calloc(1, sizeof(struct Point));
    struct Point IdealShot;
    int MaxShotedTargets = 0;
    for (i = 0; i < AllTargetsNum; i++)
    {
        ReachableNum = ReachableTargets(AllTargets[i], AllTargets, AllTargetsNum, &Reachable, RocketRadius);

        if (ReachableNum + 1 > MaxShotedTargets)
        {
            MaxShotedTargets = ReachableNum + 1;
            IdealShot.x = AllTargets[i].x;
            IdealShot.y = AllTargets[i].y;
        }
    }
    printf("Shoot in (%d, %d) to destroy %d targets\n", IdealShot.x, IdealShot.y, MaxShotedTargets);
    free(Reachable);
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
