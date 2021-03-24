// Fill out your copyright notice in the Description page of Project Settings.

#include "VPmame.h"

//#include <RenderMeshAsset.h>

#include "UE4_VPinMAME.h"
#include <string>

DEFINE_LOG_CATEGORY(LogVPinball);

//static HICON m_hIcon = 0;
static IConnectionPointContainer* GPControllerConnectionPointContainer = nullptr;
static IConnectionPoint* GPControllerConnectionPoint = nullptr;
//static DWORD dwControllerCookie = 0;
static BSTR GPGameName;
static BSTR GPVersion;

UVPmame::UVPmame() {
	CLSID ClsID;
	HRESULT Hr;
	GPController = nullptr;
	Hr = CLSIDFromProgID(OLESTR("VPinMAME.Controller"), &ClsID); // Get class ID from program ID
	if (FAILED(Hr))
	{
		UE_LOG(LogVPinball, Error, TEXT("Could not create VPinMAME.Controller Class. VPinMAME must be installed properly so that registry vars are initialized."));
		return;
	}
	else
	{
		UE_LOG(LogVPinball, Log, TEXT("VPinMAME.Controller Class found."));
	}

	Hr = CoCreateInstance(ClsID, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IController), reinterpret_cast<void**>(&GPController)); // Create COM object
	if (FAILED(Hr))
	{
		UE_LOG(LogVPinball, Error, TEXT("Can't create the Controller instance! \nRegistry settings from VPinMAME installation may be pointing at an incorrect VPinMAME64.dll or folder."));
		GPController = nullptr;
		return;
	}
	else
	{
		UE_LOG(LogVPinball, Log, TEXT("Controller instance created."));
	}

	Hr = GPController->QueryInterface(IID_IConnectionPointContainer, reinterpret_cast<void**>(&GPControllerConnectionPointContainer)); // Get pointer to COM interfaces
	if (FAILED(Hr) || GPControllerConnectionPointContainer == nullptr)
	{
		UE_LOG(LogVPinball, Error, TEXT("QueryInterface Failed! VPinMAME COM object interface invalid - incompatible version or bad VPinMAME64.dll?"));
		return;
	}
	else
	{
		UE_LOG(LogVPinball, Log, TEXT("Controller Interface successfully interrogated."));
	}

	Hr = GPControllerConnectionPointContainer->FindConnectionPoint(__uuidof(_IControllerEvents), &GPControllerConnectionPoint);
	if (FAILED(Hr) || GPControllerConnectionPoint == nullptr)
	{
		UE_LOG(LogVPinball, Error, TEXT("FindConnecitoNPoint Failed! Controller Connection Point invalid "));
		return;
	}
	else
	{
		UE_LOG(LogVPinball, Log, TEXT("FindConnectionPoint Successful."));
	}

	FUE4_VPinMAMEModule::Get().SetPinMAME(this);
};

void UVPmame::put_GameName (const FString& RomName )
{
	HRESULT Hr;
	
	if (GPController == nullptr)
	{
		return;
	}
	
	/* Set romname of emulated pinball machine */
	char* GameName = TCHAR_TO_ANSI(*RomName);
	GPGameName = SysAllocStringLen(NULL, (UINT)strlen(GameName));
	MultiByteToWideChar(CP_ACP, 0, GameName, -1, GPGameName, (int)strlen(GameName));

	Hr = GPController->put_GameName(GPGameName);
	if (FAILED(Hr))
	{
		UE_LOG(LogVPinball, Error, TEXT("Can't Set Gamename !"));
		SysFreeString(GPGameName);
		return;
	}
	else
	{
		UE_LOG(LogVPinball, Log, TEXT("Gamename Set."));
		SysFreeString(GPGameName);
	}
}

FString UVPmame::get_GameName ()
{
	HRESULT Hr;
	
	if (GPController == nullptr)
	{
		return FString("");
	}
	
	/* Get romname of emulated pinball machine */
	Hr = GPController->get_GameName(&GPGameName);
	if (FAILED(Hr)) 
	{
		UE_LOG(LogVPinball, Error, TEXT("Error: get_GameName(RomName) failed."));
		return FString("");
	}
	
	int length = WideCharToMultiByte(CP_ACP, 0, &GPGameName[0], -1, NULL, 0, NULL, NULL);
	std::string s( length-1, '\0' );
	WideCharToMultiByte(CP_ACP, 0, &GPGameName[0], -1, &s[0], length, NULL, NULL);
	return FString(s.c_str());
}

void UVPmame::VpStart(const FString& RomName) // Get romname from blueprint and start Pinmame emulator
{
	if (GPController == nullptr)
	{
		return;
	}
	
	put_GameName (RomName);
	Run(0); 
}

/* Stop emulator */
void UVPmame::VpStop()
{
	if (GPController == nullptr)
	{
		return;
	}
	
	GPController->Stop();
	UE_LOG(LogVPinball, Log, TEXT("Stopping Emulator."));
	return;
}

/* Get array of changed DMD pixels in unsigned integer format*/
/* Call will fail if there are no changed pixels, Exit and do not process in this case */
void UVPmame::VpGetDMD(TArray<uint8>& Dots)
{
	get_RawDmdPixels(Dots);
}


void UVPmame::get_RawDmdPixels(TArray<uint8>& Pixels)
{
	VARIANT VarDmdPixels;
	LONG lstart, lend;
	VARIANT HUGEP* Pbstr;

	if (GPController == nullptr)
	{
		return;
	}
	
	HRESULT Hr = GPController->get_RawDmdPixels(&VarDmdPixels);
	if (Hr == 1) // Return value of 1 means no changed pixels and no valid data: Exit
	{
		return;
	}
	else if (Hr == 0) // Return value of 0 means changed pixels : Process
	{
		SAFEARRAY* psa = VarDmdPixels.parray; //Get pointer to safeArray

		Hr = SafeArrayAccessData(psa, reinterpret_cast<void **>(& Pbstr)); // Get pointer to arraydata : Data is passed as VARIANT type
		if (FAILED(Hr))
		{
			return;
		}
		else
		{
			SafeArrayGetLBound(psa, 1, &lstart); // Get Array Start : Safearrays can have a starting point other than zero
			SafeArrayGetUBound(psa, 1, &lend); // Get Array End
			for (long Idx = lstart; Idx <= lend; Idx++) // Loop to fill Blueprint array with updated pixeldata  -- Idx may not start at 0 
			{
				uint8 Pixel;
				Pixel = Pbstr[Idx].uintVal; // Pixeldata in VARIANT is stored as Unsigned Integer
				Pixel = (Pixel - 20) * 3;
				Pixels.EmplaceAt(Idx, Pixel);
			}

			SafeArrayUnaccessData(psa); // Release safearray for updating
			SafeArrayDestroy(psa);	// Destroy safearray object
			return;

		}
	}
}

void UVPmame::get_DmdPixel (int x,int y,  uint8 & PixelValue )
{
	int Dot;
	
	if (GPController == nullptr)
	{
		return;
	}
	
	GPController->get_DmdPixel(x, y, &Dot);  // I think Dot may have a return value that indicates an error - not sure how to check it
	PixelValue = Dot;
	PixelValue = (PixelValue - 21) * 25;
	//PixelValue = (PixelValue - 20) * 3;  // I'm manipulating this based on what Data Sung did above - no sure I follow and the results seem odd
}


/* Get array of changed lamps in unsigned integer format*/
/* Call will fail if there are no changed lamps, Exit and do not process in this case */
void UVPmame::VpGetLamps (TArray<uint8>& Lamps)
{
	get_Lamps(Lamps);
}

void UVPmame::get_ChangedLampsState ( TArray<uint8>& lampNumber, TArray<uint8>& lampState, int &lampCount )
{
	get_ChangedLamps ( lampNumber, lampState );
	lampCount = lampNumber.Num();
}
void UVPmame::get_ChangedLamps ( TArray<uint8>& lampNumber, TArray<uint8>& lampState )
{
	HRESULT Hr;
	VARIANT VarLampNum, VarLampState, VarLamps;
	LONG lstart, lend;
	//VARIANT HUGEP* Pbstr;

	if (GPController == nullptr)
	{
		return;
	}
	
	Hr = GPController->get_ChangedLamps(&VarLamps);
	if (Hr == S_FALSE) // VarLamps was null?
	{
		return;
	}
	else if (Hr == S_OK) // Return value of 0 means changed lamps : Process
	{
		if (VarLamps.vt)
		{
			SAFEARRAY* Psa = VarLamps.parray;

			SafeArrayGetLBound(Psa, 1, &lstart); // Get array start : Safearrays can have a starting point other than zero
			SafeArrayGetUBound(Psa, 1, &lend); // Get Array End

			long Idx[2];
			for (Idx[0] = lstart; Idx[0] <= lend; Idx[0]++) // Loop to fill Blueprint array with updated lampdata
			{
				Idx[1] = 0;
				SafeArrayGetElement(Psa, Idx, &VarLampNum);
				lampNumber.EmplaceAt(Idx[0], VarLampNum.lVal);
				Idx[1] = 1;
				SafeArrayGetElement(Psa, Idx, &VarLampState);
				lampState.EmplaceAt(Idx[0], VarLampState.lVal);
			}

			SafeArrayUnaccessData(Psa); // Release safearray for updating
			SafeArrayDestroy(Psa); // Destroy safearray object
		}
	}
}

void UVPmame::get_LampsState (TArray<uint8>& lampState, int &lampCount)
{
	if (GPController == nullptr)
	{
		lampCount = 0;
		return;
	}

	lampCount = 89;
	for (int ix = 0; ix<lampCount; ix++)
	{
		bool bState;
		get_Lamp(ix, bState);
		lampState.EmplaceAt(ix, (uint8)bState);  
	}
}

void UVPmame::get_Lamps(TArray<uint8>& Lamps)
{
	HRESULT Hr;
	VARIANT VarLamps;
	LONG lstart, lend;
	VARIANT HUGEP* Pbstr;

	if (GPController == nullptr)
	{
		return;
	}
	
	Hr = GPController->get_Lamps(&VarLamps);
	if (Hr == 1) // Return value of 1 means no changed lamps and no valid data: Exit
	{
		return;
	}
	else if (Hr == 0) // Return value of 0 means changed lamps : Process
	{
		SAFEARRAY* Psa = VarLamps.parray;

		Hr = SafeArrayAccessData(Psa, reinterpret_cast<void **>(&Pbstr)); // Get pointer to arraydata : Data is passed as VARIANT type
		if (FAILED(Hr))
		{
			return;
		}
		else
		{
			SafeArrayGetLBound(Psa, 1, &lstart); // Get array start : Safearrays can have a starting point other than zero
			SafeArrayGetUBound(Psa, 1, &lend); // Get Array End

			for (long Idx = lstart; Idx <= lend; Idx++) // Loop to fill Blueprint array with updated lampdata
			{
				BOOL Lamp;
				Lamp = Pbstr[Idx].boolVal; // lampdata in VARIANT is stored as bool
				Lamps.EmplaceAt(Idx, Lamp);
			}

			SafeArrayUnaccessData(Psa); // Release safearray for updating
			SafeArrayDestroy(Psa); // Destroy safearray object

			return;
		}
	}
}

void UVPmame::get_RawDmdWidth(int& Width) { if (GPController) { GPController->get_RawDmdWidth(&Width);} }
void UVPmame::get_RawDmdHeight(int& Height) { if (GPController) { GPController->get_RawDmdHeight(&Height);} }
void UVPmame::get_DmdWidth(int& Width) { if (GPController) {GPController->get_DmdWidth(&Width);} }
void UVPmame::get_DmdHeight(int& Height) { if (GPController) {GPController->get_DmdHeight(&Height);} }
void UVPmame::Run(int nMinVersion) 
{ 
	HRESULT Hr;
	if (GPController) 
	{
		Hr = GPController->Run(0,nMinVersion);
		if (FAILED(Hr))
		{
			UE_LOG(LogVPinball, Error, TEXT("Can't Run !"));
			return;
		}
		else
		{
			UE_LOG(LogVPinball, Log, TEXT("Running."));
		}
		return;
	} 
}
void UVPmame::Stop() { if (GPController) {GPController->Stop();} }
void UVPmame::ShowOptsDialog() { if (GPController) {GPController->ShowOptsDialog(0);} }
void UVPmame::ShowAboutDialog() { if (GPController) {GPController->ShowAboutDialog(0);} }
void UVPmame::ShowPathsDialog () { if (GPController) {GPController->ShowPathesDialog(0);} }
void UVPmame::get_Lamp (int nLamp, bool &pVal )
{
	VARIANT_BOOL tBoolVal;
	if (GPController == nullptr)
	{
		return;
	}
	
	GPController->get_Lamp(nLamp, &tBoolVal);
	pVal = (tBoolVal != 0) ? true : false;
}
void UVPmame::get_Solenoid (int nSolenoid, bool &pVal )
{
	VARIANT_BOOL tBoolVal;
	if (GPController == nullptr)
	{
		return;
	}
	
	GPController->get_Solenoid(nSolenoid, &tBoolVal);
	pVal = (tBoolVal != 0) ? true : false;
}
void UVPmame::get_Switch (int nSwitchNo, bool &pVal )
{
	VARIANT_BOOL tBoolVal;
	if (GPController == nullptr)
	{
		return;
	}
	
	GPController->get_Switch(nSwitchNo, &tBoolVal);
	pVal = (tBoolVal != 0) ? true : false;
}
void UVPmame:: put_Switch (int SwitchNumber,bool State )
{
	VARIANT_BOOL tBoolVal = State ? -1 : 0;
	if (GPController == nullptr)
	{
		return;
	}
	
	GPController->put_Switch(SwitchNumber, tBoolVal);
}

void UVPmame::get_Switches ( TArray<uint8>&Switches )
{
	for (int switchNum = 0; switchNum < 128; switchNum++)
	{
		bool tBoolVal;
		get_Switch(switchNum, tBoolVal);
		Switches.EmplaceAt(switchNum, (uint8)tBoolVal);
	}
}

void UVPmame::put_Switches ( TArray<uint8>Switches )
{
	for (int switchNum = 0; switchNum < 128; switchNum++)
	{
		put_Switch(switchNum, Switches[switchNum] ? true : false);
	}
}

bool UVPmame::get_Running ()
{
	VARIANT_BOOL tBoolVal;
	if (GPController == nullptr)
	{
		return false;
	}
	GPController->get_Running(&tBoolVal);
	return (tBoolVal != 0) ? true : false;
}

bool UVPmame::get_Pause ( )
	{
		VARIANT_BOOL tBoolVal;
		if (GPController == nullptr)
		{
			return false;
		}
		GPController->get_Pause(&tBoolVal);
		return (tBoolVal != 0) ? true : false;
	}

void UVPmame::put_Pause (bool pauseGame )
	{
		VARIANT_BOOL tBoolVal = pauseGame ? -1 : 0;
		if (GPController == nullptr)
		{
			return;
		}
		
		GPController->put_Pause(tBoolVal);
	}


FString UVPmame::get_Version ( )
{
	HRESULT Hr;
	
	if (GPController == nullptr)
	{
		return FString("");
	}
	
	Hr = GPController->get_Version(&GPVersion);
	if (FAILED(Hr)) 
	{
		UE_LOG(LogVPinball, Error, TEXT("Error: get_Version() failed."));
		return FString("");
	}
	
	int length = WideCharToMultiByte(CP_ACP, 0, &GPVersion[0], -1, NULL, 0, NULL, NULL);
	std::string s( length-1, '\0' );
	WideCharToMultiByte(CP_ACP, 0, &GPVersion[0], -1, &s[0], length, NULL, NULL);
	return FString(s.c_str());
}

void UVPmame::get_ChangedSolenoids ( TArray<uint8>& solenoidNumber, TArray<uint8>& solenoidState )
{
	HRESULT Hr;
	VARIANT VarSolenoidNum, VarSolenoidState, VarSolenoids;
	LONG lstart, lend;
	//VARIANT HUGEP* Pbstr;

	if (GPController == nullptr)
	{
		return;
	}
	
	Hr = GPController->get_ChangedSolenoids(&VarSolenoids);
	if (Hr == S_FALSE) // VarSolenoids was null?
	{
		return;
	}
	else if (Hr == S_OK) // Return value of 0 means changed solenoids : Process
	{
		if (VarSolenoids.vt)
		{
			SAFEARRAY* Psa = VarSolenoids.parray;

			SafeArrayGetLBound(Psa, 1, &lstart); // Get array start : Safearrays can have a starting point other than zero
			SafeArrayGetUBound(Psa, 1, &lend); // Get Array End

			long Idx[2];
			for (Idx[0] = lstart; Idx[0] <= lend; Idx[0]++) // Loop to fill Blueprint array with updated solenoiddata
			{
				Idx[1] = 0;
				SafeArrayGetElement(Psa, Idx, &VarSolenoidNum);
				solenoidNumber.EmplaceAt(Idx[0], VarSolenoidNum.lVal);
				Idx[1] = 1;
				SafeArrayGetElement(Psa, Idx, &VarSolenoidState);
				solenoidState.EmplaceAt(Idx[0], VarSolenoidState.lVal);
			}

			SafeArrayUnaccessData(Psa); // Release safearray for updating
			SafeArrayDestroy(Psa); // Destroy safearray object
		}
	}
}
void UVPmame::get_Solenoids ( TArray<uint8>& Solenoids)
{
	HRESULT Hr;
	VARIANT VarSolenoids;
	LONG lstart, lend;
	VARIANT HUGEP* Pbstr;

	if (GPController == nullptr)
	{
		return;
	}
	
	Hr = GPController->get_Solenoids(&VarSolenoids);
	if (Hr == 1) // Return value of 1 means no changed solenoids and no valid data: Exit
	{
		return;
	}
	else if (Hr == 0) // Return value of 0 means changed solenoids : Process
	{
		SAFEARRAY* Psa = VarSolenoids.parray;

		Hr = SafeArrayAccessData(Psa, reinterpret_cast<void **>(&Pbstr)); // Get pointer to arraydata : Data is passed as VARIANT type
		if (FAILED(Hr))
		{
			return;
		}
		else
		{
			SafeArrayGetLBound(Psa, 1, &lstart); // Get array start : Safearrays can have a starting point other than zero
			SafeArrayGetUBound(Psa, 1, &lend); // Get Array End

			for (long Idx = lstart; Idx <= lend; Idx++) // Loop to fill Blueprint array with updated solenoiddata
			{
				BOOL Lamp;
				Lamp = Pbstr[Idx].boolVal; // solenoiddata in VARIANT is stored as bool
				Solenoids.EmplaceAt(Idx, Lamp);
			}

			SafeArrayUnaccessData(Psa); // Release safearray for updating
			SafeArrayDestroy(Psa); // Destroy safearray object

			return;
		}
	}
}
void UVPmame::get_ChangedSolenoidsState ( TArray<uint8>& solenoidNumber, TArray<uint8>& solenoidState, int &solenoidCount )
{
	get_ChangedSolenoids ( solenoidNumber, solenoidState );
	solenoidCount = solenoidNumber.Num();
}
void UVPmame::get_SolenoidsState ( TArray<uint8>& solenoidState, int &solenoidCount)
{
	if (GPController == nullptr)
	{
		solenoidCount = 0;
		return;
	}

	solenoidCount = 66;
	for (int ix = 0; ix<solenoidCount; ix++)
	{
		bool bState;
		get_Solenoid(ix, bState);
		solenoidState.EmplaceAt(ix, (uint8)bState);  
	}
}

void UVPmame::get_ChangedLEDs (int nHigh,int nLow,int nnHigh,int nnLow, TArray<uint8>& ledNumber , TArray<uint8>& ledSegments , TArray<uint8>& ledState )
{
	HRESULT Hr;
	VARIANT VarLEDNum, VarLEDSegs, VarLEDState, VarLEDs;
	LONG lstart, lend;

	if (GPController == nullptr)
	{
		return;
	}
	
	Hr = GPController->get_ChangedLEDs(nHigh, nLow, nnHigh, nnLow, &VarLEDs);
	if (Hr == S_FALSE) // VarLEDs was null?
	{
		return;
	}
	else if (Hr == S_OK) // Return value of 0 means changed leds : Process
	{
		if (VarLEDs.vt)
		{
			SAFEARRAY* Psa = VarLEDs.parray;

			SafeArrayGetLBound(Psa, 1, &lstart); // Get array start : Safearrays can have a starting point other than zero
			SafeArrayGetUBound(Psa, 1, &lend); // Get Array End

			long Idx[2];
			for (Idx[0] = lstart; Idx[0] <= lend; Idx[0]++) // Loop to fill Blueprint array with updated leddata
			{
				Idx[1] = 0;
				SafeArrayGetElement(Psa, Idx, &VarLEDNum);
				ledNumber.EmplaceAt(Idx[0], VarLEDNum.lVal);
				Idx[1] = 1;
				SafeArrayGetElement(Psa, Idx, &VarLEDSegs);
				ledSegments.EmplaceAt(Idx[0], VarLEDSegs.lVal);
				Idx[1] = 2;
				SafeArrayGetElement(Psa, Idx, &VarLEDState);
				ledState.EmplaceAt(Idx[0], VarLEDState.lVal);
			}

			SafeArrayUnaccessData(Psa); // Release safearray for updating
			SafeArrayDestroy(Psa); // Destroy safearray object
		}
	}
}
void UVPmame::get_ChangedLEDsState (int nHigh,int nLow,int nnHigh,int nnLow, TArray<uint8>& ledNumber , TArray<uint8>& ledSegments , TArray<uint8>& ledState, int &ledCount )
{
	get_ChangedLEDs (nHigh, nLow, nnHigh, nnLow,  ledNumber, ledSegments, ledState);
	ledCount = ledNumber.Num();
}

void UVPmame::SetVertexColorOverride_LinearColor( UStaticMeshComponent* Target, const TArray<FLinearColor>& VertexColors)
{

	TArray<FColor> Colors;
	if (VertexColors.Num()>0)
	{
		Colors.SetNum(VertexColors.Num());
		for (int32 ColorIdx = 0; ColorIdx < VertexColors.Num(); ColorIdx++)
		{
			Colors[ColorIdx] = VertexColors[ColorIdx].ToFColor(false);
		}
	}
	SetVertexColorOverride( Target, Colors);
}

void UVPmame::SetVertexColorOverride( UStaticMeshComponent* Target, const TArray<FColor>& VertexColors)
{
	UStaticMesh* SM = Target->GetStaticMesh(); 	
	if (!SM) {
		UE_LOG(LogTemp, Log, TEXT("Static mesh not found"));
		return;
	}
	else if (VertexColors.Max() != 0)
	{
		//FPositionVertexBuffer* PositionVertexBuffer = &SM->RenderData->LODResources[0].VertexBuffers.PositionVertexBuffer; 
		Target->SetLODDataCount(1, Target->LODData.Num()); 
		FStaticMeshComponentLODInfo* LODInfo = &Target->LODData[0]; 
		LODInfo->PaintedVertices.Empty(); 	
		LODInfo->OverrideVertexColors = new FColorVertexBuffer(); 
		FStaticMeshLODResources& LodResources = SM->RenderData->LODResources[0]; 
		TArray<FColor> RandomColorArray; 
		RandomColorArray.Reserve(LodResources.GetNumVertices() * 4 - 1); 

		for (int32 i = 0; i < VertexColors.Num(); i++) /// look at SkinnedMechComponent.cpp - maybe this should use the same "ExpectedNumVerts" method
		{
			int c = 0; 
			for (c = 0; c < 4; c++)  // this may be specific to pierre's dmd model, so this may have to change
			{
				//RandomColorArray.Add(FColor(VertexColors[i], 0, 0, 0));
				RandomColorArray.Add(VertexColors[i]);
			}
		}

		LODInfo->OverrideVertexColors->InitFromColorArray(RandomColorArray);
		BeginInitResource(LODInfo->OverrideVertexColors);
		Target->MarkRenderStateDirty();
	}
}


