// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerator.h"

AMapGenerator::AMapGenerator()
{
}

void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();
}

void AMapGenerator::StartGenerateMap()
{

	for (int32 Floor = 0; Floor < MaxFloor; Floor++)
	{
		FDungeonMap NewDungeonMap(MapSize.Y, MapSize.X);
		DungeonMaps.Add(NewDungeonMap);

		FDungeonRoom NewDungeonRoom;
		DungeonRooms.Add(NewDungeonRoom);

		//DungeonMaps[Floor].Init(FTileArray(MapSize.X), MapSize.Y);
		FillWall(Floor);
		TObjectPtr<URoomNode> Root = NewObject<URoomNode>();
		Root->SetNodeRect(FRectInt(0, 0, MapSize.X - 4, MapSize.Y - 4));
		Divde(Root, 0, Floor);
		GenerateRoom(Root, 0, Floor);
		GenerateLoad(Root, 0, Floor);
		//FillClif2(Root, 0, Floor);

		/*Door,  Ladder 생성 순서를 지켜주자.*/
		GenerateDoor(Floor);
		MaxDepth -= DepthOffset;
		MaxDepth = FMath::Max(1, MaxDepth);
	}

	GenerateFirstFloorLadder();
	for (int Floor = 1; Floor < MaxFloor; Floor++)
	{
		GenerateSecondFloorLadder(Floor);
	}

}

void AMapGenerator::Divde(TObjectPtr<URoomNode> Tree, int32 Depth, int32 Floor)
{
	Tree->NodeRect.RoomNumber = RoomID;
	DungeonRooms[Floor].Rooms.Add(RoomID++, Tree);
	Tree->ThisFloor = Floor;

	if (Depth == MaxDepth)
	{
		Tree->IsReaf = true;
		return;
	}

	//가로 길이와 세로 길이 중 더 긴것을 찾아서 나눠준다.
	//가로 => 세로 나누기.  세로=> 가로 나누기
	int32 MaxLength = FMath::Max(Tree->NodeRect.Width, Tree->NodeRect.Height);
	int32	SplitLength = FMath::RoundToInt(FMath::FRandRange(MaxLength * MinDivideRate, MaxLength * MaxDivideRate));

	//이걸 하면 방 크기를 일정 이상 유지할 수 있지만, 대신 사이즈에 맞지 않게 Depth를 키우면 Range 에러가 생겨난다.
	if (SplitLength < MinRoomSize)
		SplitLength = MinRoomSize;

	//가로가 더 길다면, 세로로 나눈다.
	if (Tree->NodeRect.Width >= Tree->NodeRect.Height)
	{
		//[		|	 		] Tree라는 네모난 상자를 랜덤한 비율로 자르고, 생겨난 두 개의 상자를 왼쪽, 오른쪽 노드에 저장.
		Tree->LeftNode = NewObject<URoomNode>();
		Tree->LeftNode->SetNodeRect(FRectInt(Tree->NodeRect.X, Tree->NodeRect.Y, SplitLength, Tree->NodeRect.Height));
		Tree->RightNode = NewObject<URoomNode>();
		Tree->RightNode->SetNodeRect(FRectInt(Tree->NodeRect.X + SplitLength, Tree->NodeRect.Y, Tree->NodeRect.Width - SplitLength, Tree->NodeRect.Height));

	}
	else
	{
		Tree->LeftNode = NewObject<URoomNode>();
		Tree->LeftNode->SetNodeRect(FRectInt(Tree->NodeRect.X, Tree->NodeRect.Y, Tree->NodeRect.Width, SplitLength));
		Tree->RightNode = NewObject<URoomNode>();
		Tree->RightNode->SetNodeRect(FRectInt(Tree->NodeRect.X, Tree->NodeRect.Y + SplitLength, Tree->NodeRect.Width, Tree->NodeRect.Height - SplitLength));
	}

	Tree->LeftNode->ParentNode = Tree;
	Tree->RightNode->ParentNode = Tree;
	Divde(Tree->RightNode, Depth + 1, Floor);
	Divde(Tree->LeftNode, Depth + 1, Floor);

}

FRectInt AMapGenerator::GenerateRoom(TObjectPtr<URoomNode> Tree, int32 Depth, int32 Floor)
{
	FRectInt RoomRect;
	if (Depth == MaxDepth)
	{
		//리프 노드에 도달하면 방을 만들어준다.
		RoomRect = Tree->NodeRect;
		//최소값은 노드 길이의 절반. 
		int32 Width = FMath::RandRange(RoomRect.Width / MinNodeSize, RoomRect.Width);
		int32 Height = FMath::RandRange(RoomRect.Height / MinNodeSize, RoomRect.Height);


		//if (Width < MinRoomSize)
		//	Width = MinRoomSize;
		//if (Height < MinRoomSize)
		//	Height = MinRoomSize;

		int32 X = RoomRect.X + FMath::RandRange(1, RoomRect.Width - Width);
		int32 Y = RoomRect.Y + FMath::RandRange(1, RoomRect.Height - Height);

		RoomRect.X = X;
		RoomRect.Y = Y;
		RoomRect.Width = Width;
		RoomRect.Height = Height;
		RoomRect.RoomNumber = Tree->NodeRect.RoomNumber;
		RoomCenterPoints.Add(FVector((X + Width) / 2, (Y + Height) / 2, Floor));
		FillRoom(RoomRect, Floor);
	}
	else
	{
		Tree->LeftNode->RoomRect = GenerateRoom(Tree->LeftNode, Depth + 1, Floor);
		Tree->RightNode->RoomRect = GenerateRoom(Tree->RightNode, Depth + 1, Floor);
		RoomRect = Tree->LeftNode->RoomRect;
	}

	return RoomRect;
}

void AMapGenerator::GenerateLoad(TObjectPtr<URoomNode> Tree, int32 Depth, int32 Floor)
{
	if (Depth == MaxDepth)
		return;


	FVector2D LeftNodeCenter = Tree->LeftNode->GetCenter();
	FVector2D RightNodeCenter = Tree->RightNode->GetCenter();

	FVector ObjectPoint;

	if (LeftNodeCenter.X < RightNodeCenter.X)
	{
		for (int32 i = LeftNodeCenter.X; i <= RightNodeCenter.X; i++)
		{
			if (DungeonMaps[Floor].Dungeons[LeftNodeCenter.Y].TileArry[i] == ETileType::Room)
				continue;

			int32 ret = SearchTile(LeftNodeCenter.Y, i, ETileType::Room, Floor);
			if (ret >= 3)
				continue;

			DungeonMaps[Floor].Dungeons[LeftNodeCenter.Y].TileArry[i] = ETileType::Road;
		}


		ObjectPoint = FVector(LeftNodeCenter.Y, (LeftNodeCenter.X + RightNodeCenter.X) / 2, Floor);

	}
	else
	{
		for (int32 i = RightNodeCenter.X; i <= LeftNodeCenter.X; i++)
		{
			if (DungeonMaps[Floor].Dungeons[RightNodeCenter.Y].TileArry[i] == ETileType::Room)
				continue;

			int32 ret = SearchTile(RightNodeCenter.Y, i, ETileType::Room, Floor);
			if (ret >=3)
				continue;

			DungeonMaps[Floor].Dungeons[RightNodeCenter.Y].TileArry[i] = ETileType::Road;
		}


		ObjectPoint = FVector(RightNodeCenter.Y, (LeftNodeCenter.X + RightNodeCenter.X) / 2, Floor);
	}

	if (LeftNodeCenter.Y < RightNodeCenter.Y)
	{
		for (int32 i = LeftNodeCenter.Y; i <= RightNodeCenter.Y; i++)
		{
			if (DungeonMaps[Floor].Dungeons[i].TileArry[RightNodeCenter.X] == ETileType::Room)
				continue;

			int32 ret = SearchTile(i, RightNodeCenter.X, ETileType::Room, Floor);
			if (ret >= 3)
				continue;

			DungeonMaps[Floor].Dungeons[i].TileArry[RightNodeCenter.X] = ETileType::Road;
		}

		ObjectPoint = FVector((RightNodeCenter.Y + LeftNodeCenter.Y) / 2, RightNodeCenter.X, Floor);
	}
	else
	{
		for (int32 i = RightNodeCenter.Y; i <= LeftNodeCenter.Y; i++)
		{
			if (DungeonMaps[Floor].Dungeons[i].TileArry[LeftNodeCenter.X] == ETileType::Room)
				continue;

			int32 ret = SearchTile(i, LeftNodeCenter.X, ETileType::Room, Floor);
			if (ret >= 3)
				continue;
			DungeonMaps[Floor].Dungeons[i].TileArry[LeftNodeCenter.X] = ETileType::Road;
		}


		ObjectPoint = FVector((RightNodeCenter.Y + LeftNodeCenter.Y) / 2, LeftNodeCenter.X, Floor);
	}

	RoadObjectPoints.Add(ObjectPoint);
	GenerateLoad(Tree->LeftNode, Depth + 1, Floor);
	GenerateLoad(Tree->RightNode, Depth + 1, Floor);

}

void AMapGenerator::GenerateDoor(int32 Floor)
{
	//맵 전체를 순회해서 앞뒤 룸. 양옆 벽.	Forward
	//										양옆 룸. 앞뒤 벽	Right
	//										

	for (int32 Y = 1; Y < MapSize.Y - 1; Y++)
	{
		for (int32 X = 1; X < MapSize.X - 1; X++)
		{

			if (DungeonMaps[Floor].Dungeons[Y].TileArry[X] != ETileType::Road)
				continue;;
				
			if (DungeonMaps[Floor].Dungeons[Y + 1].TileArry[X] == ETileType::Room
				&& DungeonMaps[Floor].Dungeons[Y - 1].TileArry[X] == ETileType::Room
				&& DungeonMaps[Floor].Dungeons[Y].TileArry[X + 1] == ETileType::Wall
				&& DungeonMaps[Floor].Dungeons[Y].TileArry[X - 1] == ETileType::Wall)
				DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::ForwardDoor;

			if (DungeonMaps[Floor].Dungeons[Y + 1].TileArry[X] == ETileType::Room
				&& DungeonMaps[Floor].Dungeons[Y - 1].TileArry[X] == ETileType::Road
				&& DungeonMaps[Floor].Dungeons[Y].TileArry[X + 1] == ETileType::Wall
				&& DungeonMaps[Floor].Dungeons[Y].TileArry[X - 1] == ETileType::Wall)
				DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::ForwardDoor;

			if (DungeonMaps[Floor].Dungeons[Y + 1].TileArry[X] == ETileType::Road
				&& DungeonMaps[Floor].Dungeons[Y - 1].TileArry[X] == ETileType::Room
				&& DungeonMaps[Floor].Dungeons[Y].TileArry[X + 1] == ETileType::Wall
				&& DungeonMaps[Floor].Dungeons[Y].TileArry[X - 1] == ETileType::Wall)
				DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::ForwardDoor;

			if (DungeonMaps[Floor].Dungeons[Y + 1].TileArry[X] == ETileType::Wall
				&& DungeonMaps[Floor].Dungeons[Y - 1].TileArry[X] == ETileType::Wall
				&& DungeonMaps[Floor].Dungeons[Y].TileArry[X + 1] == ETileType::Room
				&& DungeonMaps[Floor].Dungeons[Y].TileArry[X - 1] == ETileType::Room)
				DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::RightDoor;

			if (DungeonMaps[Floor].Dungeons[Y + 1].TileArry[X] == ETileType::Wall
				&& DungeonMaps[Floor].Dungeons[Y - 1].TileArry[X] == ETileType::Wall
				&& DungeonMaps[Floor].Dungeons[Y].TileArry[X + 1] == ETileType::Road
				&& DungeonMaps[Floor].Dungeons[Y].TileArry[X - 1] == ETileType::Room)
				DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::RightDoor;

			if (DungeonMaps[Floor].Dungeons[Y + 1].TileArry[X] == ETileType::Wall
				&& DungeonMaps[Floor].Dungeons[Y - 1].TileArry[X] == ETileType::Wall
				&& DungeonMaps[Floor].Dungeons[Y].TileArry[X + 1] == ETileType::Room
				&& DungeonMaps[Floor].Dungeons[Y].TileArry[X - 1] == ETileType::Road)
				DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::RightDoor;
		}
	}
}

void AMapGenerator::GenerateFirstFloorLadder()
{
	int32 Floor = 0;

	for (auto RoomNode : DungeonRooms[Floor].Rooms)
	{
		TObjectPtr<URoomNode> Tree = RoomNode.Value;

		
		if (Tree->IsReaf == false)
			continue;
		//이미 방 안에 사다리가 있으면 스킵.
		if (Tree->HasLadder == true)
			continue;

		float RandFloat = FMath::RandRange(0, 100);

		if(RandFloat >= SpawnLadderRate)
			continue;

		int StartY = Tree->RoomRect.Y;
		int EndY = Tree->RoomRect.Y + Tree->RoomRect.Height;

		int StartX = Tree->RoomRect.X;
		int EndX = Tree->RoomRect.X + Tree->RoomRect.Width;

		for (int32 Y = StartY; Y < EndY; Y++)
		{

			if (Tree->HasLadder == true)
				break;
			for (int32 X = StartX; X < EndX; X++)
			{

				int RoomCount = SearchTile(Y, X, ETileType::Room, Floor + 1);
				if (RoomCount <= 2)
					continue;


				/*
						Wall			Room
						Room			Room
				*/

				if (DungeonMaps[Floor].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor].Dungeons[Y + 1].TileArry[X] == ETileType::Wall
					&& DungeonMaps[Floor + 1].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor + 1].Dungeons[Y + 1].TileArry[X] == ETileType::Room)
				{
					DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::ForwardLadder;
					Tree->HasLadder = true;
					LadderObjectPoints.Push(FVector2D(Y, X));
					break;
				}

				/*
		[Wall][Room]	[Room][Room]
				*/

				if (DungeonMaps[Floor].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor].Dungeons[Y].TileArry[X - 1] == ETileType::Wall
					&& DungeonMaps[Floor + 1].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor + 1].Dungeons[Y].TileArry[X - 1] == ETileType::Room)
				{
					DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::LeftLadder;
					Tree->HasLadder = true;
					LadderObjectPoints.Push(FVector2D(Y, X));
					break;
				}
				/*

					[Room]					[Room]
					[Wall]					[Room]
				*/
				if (DungeonMaps[Floor].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor].Dungeons[Y - 1].TileArry[X] == ETileType::Wall
					&& DungeonMaps[Floor + 1].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor + 1].Dungeons[Y - 1].TileArry[X] == ETileType::Room)
				{
					DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::BackwardLadder;
					Tree->HasLadder = true;
					LadderObjectPoints.Push(FVector2D(Y, X));
					break;
				}

				/*
					[Room][Wall]		[Room][Room]
				*/
				if (DungeonMaps[Floor].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor].Dungeons[Y].TileArry[X + 1] == ETileType::Wall
					&& DungeonMaps[Floor + 1].Dungeons[Y].TileArry[X + 1] == ETileType::Room
					&& DungeonMaps[Floor + 1].Dungeons[Y].TileArry[X] == ETileType::Room)
				{
					DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::RightLadder;
					Tree->HasLadder = true;
					LadderObjectPoints.Push(FVector2D(Y, X));
					break;
				}

			}
		}
	}
}

void AMapGenerator::GenerateSecondFloorLadder(int32 Floor)
{
	for (auto RoomNode : DungeonRooms[Floor].Rooms)
	{
		TObjectPtr<URoomNode> Tree = RoomNode.Value;

		if (Tree->IsReaf == false)
			continue;
		//이미 방 안에 사다리가 있으면 스킵.
		if (Tree->HasLadder == true)
			continue;

		float RandFloat = FMath::RandRange(0, 100);

		if (RandFloat >= SpawnLadderRate)
			continue;

		int StartY = Tree->RoomRect.Y;
		int EndY = Tree->RoomRect.Y + Tree->RoomRect.Height;

		int StartX = Tree->RoomRect.X;
		int EndX = Tree->RoomRect.X + Tree->RoomRect.Width;

		for (int32 Y = StartY; Y < EndY; Y++)
		{
			if (Tree->HasLadder == true)
				break;

			for (int32 X = StartX; X < EndX; X++)
			{

				int RoomCount = SearchTile(Y, X, ETileType::Room, Floor + 1);
				if (RoomCount <= 2)
					continue;
				/*

						Wall			Wall
						Wall			Room

				*/

				if (DungeonMaps[Floor].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor].Dungeons[Y + 1].TileArry[X] == ETileType::Wall
					&& DungeonMaps[Floor + 1].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor + 1].Dungeons[Y + 1].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor - 1].Dungeons[Y].TileArry[X] == ETileType::Wall
					)
				{
					DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::ForwardLadder;
					Tree->HasLadder = true;
					LadderObjectPoints.Push(FVector2D(Y, X));
					break;
				}

				/*

		[Wall][Wall]	[Room][Room]

				*/

				if (DungeonMaps[Floor].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor].Dungeons[Y].TileArry[X - 1] == ETileType::Wall
					&& DungeonMaps[Floor + 1].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor + 1].Dungeons[Y].TileArry[X - 1] == ETileType::Room
					&& DungeonMaps[Floor - 1].Dungeons[Y].TileArry[X] == ETileType::Wall)
				{
					DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::LeftLadder;
					Tree->HasLadder = true;
					LadderObjectPoints.Push(FVector2D(Y, X));
					break;
				}
				/*

					[Room]					[Room]
					[Wall]					[Room]
				*/
				if (DungeonMaps[Floor].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor].Dungeons[Y - 1].TileArry[X] == ETileType::Wall
					&& DungeonMaps[Floor + 1].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor + 1].Dungeons[Y - 1].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor - 1].Dungeons[Y].TileArry[X] == ETileType::Wall)
				{
					DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::BackwardLadder;
					Tree->HasLadder = true;
					LadderObjectPoints.Push(FVector2D(Y, X));
					break;
				}

				/*
					[Room][Wall]		[Room][Room]
				*/
				if (DungeonMaps[Floor].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor].Dungeons[Y].TileArry[X + 1] == ETileType::Wall
					&& DungeonMaps[Floor + 1].Dungeons[Y].TileArry[X + 1] == ETileType::Room
					&& DungeonMaps[Floor + 1].Dungeons[Y].TileArry[X] == ETileType::Room
					&& DungeonMaps[Floor - 1].Dungeons[Y].TileArry[X + 1] == ETileType::Wall)
				{
					DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::RightLadder;
					Tree->HasLadder = true;
					LadderObjectPoints.Push(FVector2D(Y, X));
					break;
				}

			}
		}
	}
}

void AMapGenerator::FillClif(int32 Floor)
{
	for (auto RoomNode : DungeonRooms[Floor].Rooms)
	{
		TObjectPtr<URoomNode> Tree = RoomNode.Value;

		if (Tree->GetRoomSize() > CreateClifLessThenThisSize)
			continue;

		 if(Tree->GetRoomSize() < CreateClifGreaterThenThisSize)
			continue;

		if (Tree->IsReaf == false)
			continue;

		Tree->HasClif = true;

		int StartY = Tree->RoomRect.Y;
		int EndY = Tree->RoomRect.Y + Tree->RoomRect.Height;

		int StartX = Tree->RoomRect.X;
		int EndX = Tree->RoomRect.X + Tree->RoomRect.Width;

		//가로보다 세로가 더 길다면,
		if(Tree->RoomRect.Height > Tree->RoomRect.Width)
		{ 
			for (int Y = StartY; Y < EndY; Y++)
			{
				for (int X = StartX; X < EndX; X++)
				{
					if (Tree->GetCenter().Y == Y && Tree->GetCenter().X == X)
						continue;

					//if (ClifAndOr == true)
					//{
					//	if (X % ClifDistanceX == 0 && Y % ClifDistanceY == 0)
					//		continue;
					//}
					//else
					//{
					//	if (X % ClifDistanceX == 0 || Y % ClifDistanceY == 0)
					//		continue;
					//}

					if (Y % ClifDistanceY == 0)
						continue;

					DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::Clif;
				}
			}
		}
		else
		{
			for (int Y = StartY; Y < EndY; Y++)
			{
				for (int X = StartX; X < EndX; X++)
				{
					if (Tree->GetCenter().Y == Y && Tree->GetCenter().X == X)
						continue;

					//if (ClifAndOr == true)
					//{
					//	if (X % ClifDistanceX == 0 && Y % ClifDistanceY == 0)
					//		continue;
					//}
					//else
					//{
					//	if (X % ClifDistanceX == 0 || Y % ClifDistanceY == 0)
					//		continue;
					//}
					if (X % ClifDistanceX == 0)
						continue;

					DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::Clif;
				}
			}

		}
	}
}

void AMapGenerator::FillClif2(URoomNode* Tree, int32 Depth, int32 Floor)
{
	if (Depth == MaxDepth)
		return;

		if (Tree->GetRoomSize() <= CreateClifLessThenThisSize && Tree->GetRoomSize() >= CreateClifGreaterThenThisSize)
		{ 

			int StartY = Tree->RoomRect.Y;
			int EndY = Tree->RoomRect.Y + Tree->RoomRect.Height;

			int StartX = Tree->RoomRect.X;
			int EndX = Tree->RoomRect.X + Tree->RoomRect.Width;

			//가로보다 세로가 더 길다면,
				for (int Y = StartY; Y <= EndY; Y++)
				{
					for (int X = StartX; X <= EndX; X++)
					{
						//if (DungeonMaps[Floor].Dungeons[Y].TileArry[X] == ETileType::Road)
						//	continue;

						DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::Clif;
					}
				}

				FVector2D LeftNodeCenter = Tree->LeftNode->GetCenter();
				FVector2D RightNodeCenter = Tree->RightNode->GetCenter();

				if (LeftNodeCenter.X < RightNodeCenter.X)
				{
					for (int32 i = LeftNodeCenter.X; i <= RightNodeCenter.X; i++)
					{
						DungeonMaps[Floor].Dungeons[LeftNodeCenter.Y].TileArry[i] = ETileType::Room;
					}
				}
				else
				{
					for (int32 i = RightNodeCenter.X; i <= LeftNodeCenter.X; i++)
					{
						DungeonMaps[Floor].Dungeons[RightNodeCenter.Y].TileArry[i] = ETileType::Room;
					}
				}

				if (LeftNodeCenter.Y < RightNodeCenter.Y)
				{
					for (int32 i = LeftNodeCenter.Y; i <= RightNodeCenter.Y; i++)
					{
						DungeonMaps[Floor].Dungeons[i].TileArry[RightNodeCenter.X] = ETileType::Room;
					}

				}
				else
				{
					for (int32 i = RightNodeCenter.Y; i <= LeftNodeCenter.Y; i++)
					{
						DungeonMaps[Floor].Dungeons[i].TileArry[LeftNodeCenter.X] = ETileType::Room;
					}
				}
		}
	FillClif2(Tree->LeftNode, Depth + 1, Floor);
	FillClif2(Tree->RightNode, Depth + 1, Floor);

}


int32 AMapGenerator::SearchTile(int32 Y, int32 X, ETileType TileType, int32 Floor)
{
	if (Floor >= MaxFloor)
		return 0;

	int32 Count = 0;
	for (int32 YY = -1; YY < 1; YY++)
	{
		for (int32 XX = -1; XX < 1; XX++)
		{
			if (YY == 0 && XX == 0)
				continue;
			if (Y + YY < 1 || Y + YY > MapSize.Y || X + XX < 1 || X + XX > MapSize.X)
				continue;

			if (DungeonMaps[Floor].Dungeons[Y + YY].TileArry[X + XX] == TileType)
			{
				Count++;
			}

		}
	}

	return Count;
}

ETileType AMapGenerator::GetTileType(int32 Y, int32 X, int32 Floor)
{
	return DungeonMaps[Floor].Dungeons[Y].TileArry[X];
}

void AMapGenerator::FillRoom(FRectInt rect, int32 Floor)
{
	for (int32 Y = rect.Y; Y < rect.Y + rect.Height - DistanceBetweenRoom; Y++)
	{
		for (int32 X = rect.X; X < rect.X + rect.Width - DistanceBetweenRoom; X++)
		{
			if (DungeonMaps[Floor].Dungeons[Y].TileArry[X] == ETileType::Room)
				DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::Wall;

			DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::Room;
			//DungeonMap[Y][X] =rect.RoomNumber;
		}

	}
}

void AMapGenerator::FillWall(int32 Floor)
{
	for (int32 Y = 0; Y < MapSize.Y; Y++)
	{
		for (int32 X = 0; X < MapSize.X; X++)
		{
			DungeonMaps[Floor].Dungeons[Y].TileArry[X] = ETileType::Wall;
		}
	}
}