// Fill out your copyright notice in the Description page of Project Settings.


#include "ChallengeFunctionLibrary.h"

void UChallengeFunctionLibrary::BRC_Naive(const FString& InInputName)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UChallengeFunctionLibrary::BRC_Naive"));
	// open file
	FArchive* File;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Open file"));
		const FString Path = FPaths::ProjectContentDir() + TEXT("Data/") + InInputName;
		File = IFileManager::Get().CreateFileReader(*Path);
	}
	
	// Prepare RET
	TMap<FString, FBRCStruct_Naive> Data;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Reading data"));
		
		// flag. false == waiting for ;, true == waiting for newline
		bool flag = false;
		// 100 bytes, plus termination symbol
		constexpr int bufferSize = 101;
		ANSICHAR buffer[bufferSize];
		// buffer is null-terminated, so making last character null should work just fine
		buffer[0] = 0;
		int bufferIndex = 0;
	
		FString Station;
		
		while(!File->AtEnd())
		{
			ANSICHAR byte;
			{
				TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Read byte"));
				// this line stream from File to byte. Do not ask.
				*File << byte;
			}
		
			if (!flag && byte == ';')
			{
				buffer[bufferIndex] = 0;
				{
					TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Buffer to string"));
					Station = UTF8_TO_TCHAR(buffer);
				}
				buffer[0] = 0;
				bufferIndex = 0;
				flag = true;
			}
			else if (flag && byte == '\n')
			{
				buffer[bufferIndex] = 0;
				double candidate;
				{
					TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Buffer to number"));
					candidate = FCStringAnsi::Atof(buffer);
				}
				buffer[0] = 0;
				bufferIndex = 0;
				flag = false;
			
				//here add value to TMap
				{
					TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Find row"));
					if (FBRCStruct_Naive* row = Data.Find(Station))
					{
						TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Recalc old value"));
						row->min = row->min < candidate ? row->min : candidate;
						row->max = row->max > candidate ? row->max : candidate;
						row->avg = (row->avg * row->count + candidate) / (row->count + 1);
						row->count = row->count + 1; 
					}
					else
					{
						TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Add new value"));
						Data.Add(Station, FBRCStruct_Naive(candidate));
					}
				}
			}
			else
			{
				TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Add byte to buffer"));
				buffer[bufferIndex] = byte;
				++bufferIndex;
			}
		}
	}
	
	// sort
	TArray<FString> SortedKeys;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Sorting data"));
		Data.GetKeys(SortedKeys);
		SortedKeys.Sort();
	}
	
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Printing data"));
		for (auto key : SortedKeys)
		{
			FBRCStruct_Naive* row = Data.Find(key);
			UE_LOG(LogTemp, Error, TEXT("%s;%.1f;%.1f;%.1f"), *key, row->min, row->avg, row->max);
		}
	}
	
	File->Close();
	delete File;
}

void UChallengeFunctionLibrary::BRC_LFTSA(const FString& InInputName)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UChallengeFunctionLibrary::BRC_LFTSA"));
	
	// Load file to RAM
	TArray<FString> Lines;
	TMap<FString, FBRCStruct_Naive> Data;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_LFTSA::Loading data to RAM"));
		const FString Path = FPaths::ProjectContentDir() + TEXT("Data/") + InInputName;
		FFileHelper::LoadFileToStringArray(OUT Lines, *Path);
	}
	
	FString Separator = TEXT(";");
	FString Station, NumberAsString;
	
	for (int i = 0; i < Lines.Num(); ++i)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Reading data"));
		Lines[i].Split(Separator, &Station, &NumberAsString);
		double candidate = FCString::Atod(*NumberAsString);
		
		//here add value to TMap
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Find row"));
			if (FBRCStruct_Naive* row = Data.Find(Station))
			{
				TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Recalc old value"));
				row->min = row->min < candidate ? row->min : candidate;
				row->max = row->max > candidate ? row->max : candidate;
				row->avg = (row->avg * row->count + candidate) / (row->count + 1);
				row->count = row->count + 1; 
			}
			else
			{
				TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Add new value"));
				Data.Add(Station, FBRCStruct_Naive(candidate));
			}
		}
	}
	
	// sort
	TArray<FString> SortedKeys;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Sorting data"));
		Data.GetKeys(SortedKeys);
		SortedKeys.Sort();
	}
	
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Printing data"));
		for (auto key : SortedKeys)
		{
			FBRCStruct_Naive* row = Data.Find(key);
			UE_LOG(LogTemp, Error, TEXT("%s;%.1f;%.1f;%.1f"), *key, row->min, row->avg, row->max);
		}
	}
}

void UChallengeFunctionLibrary::BRC_TSI(const FString& InInputName)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UChallengeFunctionLibrary::BRC_TSI"));
	// open file
	const FString Path = FPaths::ProjectContentDir() + TEXT("Data/") + InInputName;
	TUniquePtr<FArchive> File(IFileManager::Get().CreateFileReader(*Path));
	
	// Prepare RET
	TMap<FString, int> Indices;
	TArray<FBRCStruct_Naive> Rows;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_TSI::Reading data"));
		
		int soonToBeEnum = 0;
		// 100 bytes, plus termination symbol
		constexpr int bufferSize = 101;
		ANSICHAR buffer[bufferSize];
		// buffer is null-terminated, so making last character null should work just fine
		buffer[0] = 0;
		int bufferIndex = 0;
	
		FString Station;
		
		while(!File->AtEnd())
		{
			ANSICHAR byte;
		
			// this line stream from File to byte. Do not ask.
			*File << byte;
		
			if (soonToBeEnum == 0 && byte == ';')
			{
				buffer[bufferIndex] = 0;
				Station = UTF8_TO_TCHAR(buffer);
				buffer[0] = 0;
				bufferIndex = 0;
				soonToBeEnum = 1;
			}
			else if (soonToBeEnum == 1 && byte == '\n')
			{
				buffer[bufferIndex] = 0;
				double candidate = FCStringAnsi::Atof(buffer);
				buffer[0] = 0;
				bufferIndex = 0;
				soonToBeEnum = 0;
			
				if(int* index = Indices.Find(Station))
				{
					FBRCStruct_Naive& row = Rows[*index];
					row.min = row.min < candidate ? row.min : candidate;
					row.max = row.max > candidate ? row.max : candidate;
					row.avg = (row.avg * row.count + candidate) / (row.count + 1);
				}
				else
				{
					const int currIndex = Rows.Num();
					Rows.Add(FBRCStruct_Naive(candidate));
					Indices.Add(Station, currIndex);
				}
			}
			else
			{
				buffer[bufferIndex] = byte;
				++bufferIndex;
			}
		}
	}
	
	// sort
	TArray<FString> SortedKeys;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_TSI::Sorting data"));
		Indices.GetKeys(SortedKeys);
		SortedKeys.Sort();
	}
	
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_TSI::Printing data"));
		for (auto key : SortedKeys)
		{
			int* index = Indices.Find(key);
			FBRCStruct_Naive& row = Rows[*index];
			UE_LOG(LogTemp, Error, TEXT("%s;%.1f;%.1f;%.1f"), *key, row.min, row.avg, row.max);
		}
	}
	
	File->Close();
}

void UChallengeFunctionLibrary::BRC_TSI_PAS(const FString& InInputName)
{
	constexpr int billion = 1000000000;

	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UChallengeFunctionLibrary::BRC_TSI"));
	// open file
	const FString Path = FPaths::ProjectContentDir() + TEXT("Data/") + InInputName;
	TUniquePtr<FArchive> File(IFileManager::Get().CreateFileReader(*Path));
	
	// Prepare RET
	TMap<FString, int> Indices;
	TArray<FBRCStruct_Naive> Rows;
	Rows.Reserve(billion);
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_TSI::Reading data"));
		
		int soonToBeEnum = 0;
		// 100 bytes, plus termination symbol
		constexpr int bufferSize = 101;
		ANSICHAR buffer[bufferSize];
		// buffer is null-terminated, so making last character null should work just fine
		buffer[0] = 0;
		int bufferIndex = 0;
	
		FString Station;
		
		while(!File->AtEnd())
		{
			ANSICHAR byte;
		
			// this line stream from File to byte. Do not ask.
			*File << byte;
		
			if (soonToBeEnum == 0 && byte == ';')
			{
				buffer[bufferIndex] = 0;
				Station = UTF8_TO_TCHAR(buffer);
				buffer[0] = 0;
				bufferIndex = 0;
				soonToBeEnum = 1;
			}
			else if (soonToBeEnum == 1 && byte == '\n')
			{
				buffer[bufferIndex] = 0;
				double candidate = FCStringAnsi::Atof(buffer);
				buffer[0] = 0;
				bufferIndex = 0;
				soonToBeEnum = 0;
			
				if(int* index = Indices.Find(Station))
				{
					FBRCStruct_Naive& row = Rows[*index];
					row.min = row.min < candidate ? row.min : candidate;
					row.max = row.max > candidate ? row.max : candidate;
					row.avg = (row.avg * row.count + candidate) / (row.count + 1);
				}
				else
				{
					const int currIndex = Rows.Num();
					Rows.Add(FBRCStruct_Naive(candidate));
					Indices.Add(Station, currIndex);
				}
			}
			else
			{
				buffer[bufferIndex] = byte;
				++bufferIndex;
			}
		}
	}
	
	// sort
	TArray<FString> SortedKeys;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_TSI::Sorting data"));
		Indices.GetKeys(SortedKeys);
		SortedKeys.Sort();
	}
	
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_TSI::Printing data"));
		for (auto key : SortedKeys)
		{
			int* index = Indices.Find(key);
			FBRCStruct_Naive& row = Rows[*index];
			UE_LOG(LogTemp, Error, TEXT("%s;%.1f;%.1f;%.1f"), *key, row.min, row.avg, row.max);
		}
	}
	
	File->Close();
}