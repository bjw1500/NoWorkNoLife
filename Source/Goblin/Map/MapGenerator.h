// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapGenerator.generated.h"

/*
	https://sharp2studio.tistory.com/45

*/


UENUM(BlueprintType)
enum class ETileType : uint8
{
	Wall,
	Room,
	Road,
	ForwardDoor,
	RightDoor,
	Stair,
	ForwardLadder,
	RightLadder,
	LeftLadder,
	BackwardLadder,
	Clif,
};



USTRUCT(BlueprintType)
struct FRectInt
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 X;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Y;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Width;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Height;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RoomNumber;

	FRectInt()
	{
		X = 0;
		Y = 0;
		Width = 0;
		Height = 0;
	}

	FRectInt(int x, int y, int width, int height)
	{
		X = x;
		Y = y;
		Width = width;
		Height = height;
	}
};

UCLASS()
class GOBLIN_API URoomNode : public UObject
{
	GENERATED_BODY()
public:

	TObjectPtr<URoomNode>			LeftNode;
	TObjectPtr<URoomNode>			RightNode;
	TObjectPtr<URoomNode>			ParentNode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRectInt		NodeRect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRectInt		RoomRect;			//방 정보

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool				IsReaf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool				HasStair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool				HasLadder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool				HasPortal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool				HasRoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool				HasClif;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32				ThisFloor;


	URoomNode() {}
	URoomNode(FRectInt Rect)
	{
		this->NodeRect = Rect;
	}

	void SetNodeRect(FRectInt Rect) { NodeRect = Rect; };

	UFUNCTION(BlueprintCallable)
	int32 GetRoomSize() { return RoomRect.Height * RoomRect.Width; }

	UFUNCTION(BlueprintCallable)
	FVector2D		GetCenter()
	{
		return FVector2D(RoomRect.X + RoomRect.Width / 2, RoomRect.Y + RoomRect.Height / 2);
	}
};


USTRUCT(BlueprintType)
struct FTileArray
{
	GENERATED_BODY()

	FTileArray()
	{

	}

	FTileArray(int32 Size)
	{
		TileArry.Init(ETileType::Wall, Size);
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<ETileType> TileArry;
};

USTRUCT(BlueprintType)
struct FDungeonMap
{
	GENERATED_BODY()

	FDungeonMap()
	{

	}

	FDungeonMap(int32 Y, int32 X)
	{
		Dungeons.Init(FTileArray(X), Y);
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FTileArray> Dungeons;
};

USTRUCT(BlueprintType)
struct FDungeonRoom
{
	GENERATED_BODY()

	FDungeonRoom()
	{

	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int32, TObjectPtr<URoomNode>> Rooms;
};



UCLASS()
class GOBLIN_API AMapGenerator : public AActor
{
	GENERATED_BODY()

public:

	AMapGenerator();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector		MapSize;							//사이즈
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32			MaxFloor = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDungeonMap> DungeonMaps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDungeonRoom> DungeonRooms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> RoadObjectPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> RoomCenterPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> LadderObjectPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDivideRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDivideRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DepthOffset = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinRoomSize = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinNodeSize = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DistanceBetweenRoom = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnLadderRate = 0.3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CreateClifLessThenThisSize = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CreateClifGreaterThenThisSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ClifDistanceX = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ClifDistanceY = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ClifAndOr = true;

	int32 RoomID = 0;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartGenerateMap();

	void Divde(TObjectPtr<URoomNode>Tree, int32 Depth, int32 Floor);
	FRectInt GenerateRoom(TObjectPtr<URoomNode> Tree, int32 Depth, int32 Floor);
	void GenerateLoad(TObjectPtr<URoomNode>Tree, int32 Depth, int32 Floor);
	void GenerateDoor(int32 Floor);
	void GenerateFirstFloorLadder();
	void GenerateSecondFloorLadder(int32 Floor);

	UFUNCTION(BlueprintCallable)
	void FillClif(int32 Floor);
	UFUNCTION(BlueprintCallable)
	void FillClif2(URoomNode* Tree, int32 Depth, int32 Floor);

	void FillRoom(FRectInt rect, int32 Floor);
	void FillWall(int32 Floor);
	int32	SearchTile(int32 Y, int32 X, ETileType TileType, int32 Floor);

	UFUNCTION(BlueprintCallable)
	ETileType GetTileType(int32 Y, int32 X, int32 Floor);
};