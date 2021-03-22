// Fill out your copyright notice in the Description page of Project Settings.

#include "VPmame.h"
#include "UE4_VPinMAME.h"
#include <string>

DEFINE_LOG_CATEGORY(LogVPinball);

static HICON m_hIcon = 0;
static IConnectionPointContainer* GPControllerConnectionPointContainer = nullptr;
static IConnectionPoint* GPControllerConnectionPoint = nullptr;
static DWORD dwControllerCookie = 0;
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
		UE_LOG(LogVPinball, Log, TEXT("Controller Interface successfully interrogated - good to go!."));
	}

	if (SUCCEEDED(Hr))
		Hr = GPControllerConnectionPointContainer->FindConnectionPoint(__uuidof(_IControllerEvents), &GPControllerConnectionPoint);

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
void UVPmame:: put_Switch (int nSwitchNo,bool pVal )
{
	VARIANT_BOOL tBoolVal = pVal ? -1 : 0;
	if (GPController == nullptr)
	{
		return;
	}
	
	GPController->put_Switch(nSwitchNo, tBoolVal);
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