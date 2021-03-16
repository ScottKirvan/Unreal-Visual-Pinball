// Fill out your copyright notice in the Description page of Project Settings.

#include "VPmame.h"
#include "UE4_VPinMAME.h"

DEFINE_LOG_CATEGORY(LogVPinball);

static HICON m_hIcon = 0;
static IConnectionPointContainer* GPControllerConnectionPointContainer = nullptr;
static IConnectionPoint* GPControllerConnectionPoint = nullptr;
static DWORD dwControllerCookie = 0;
static BSTR GPGameName;

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

void UVPmame::VpStart(const FString& RomName) // Get romname from blueprint and start Pinmame emulator
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

	/* Start emulator */
	Hr = GPController->Run(0, 0);     // TODO - probably should have a nMinVersion number here?
	if (FAILED(Hr)) {
		UE_LOG(LogVPinball, Error, TEXT("Can't Run !"));
		return;
	}
	else {
		UE_LOG(LogVPinball, Log, TEXT("Running."));
	}
	return;
}

/* Stop emulator */
void UVPmame::VpStop()
{
	GPController->Stop();
	UE_LOG(LogVPinball, Log, TEXT("Stopping Emulator."));
	return;
}

/* Get array of changed DMD pixels in unsigned integer format*/
/* Call will fail if there are no changed pixels, Exit and do not process in this case */
void UVPmame::VpGetDMD(TArray<uint8>& Dots)
{
	VARIANT VarDmdPixels;
	LONG lstart, lend;
	VARIANT HUGEP* Pbstr;

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
			for (long Idx = lstart; Idx <= lend; Idx++) // Loop to fill Blueprint array with updated pixeldata
			{
				BYTE Pixel;
				Pixel = Pbstr[Idx].uintVal; // Pixeldata in VARIANT is stored as Unsigned Integer
				Pixel = (Pixel - 20) * 3;
				Dots.EmplaceAt(Idx, Pixel);
			}

			SafeArrayUnaccessData(psa); // Release safearray for updating
			SafeArrayDestroy(psa);	// Destroy safearray object
			return;

		}
	}

}


/* Get array of changed lamps in unsigned integer format*/
/* Call will fail if there are no changed lamps, Exit and do not process in this case */
void UVPmame::VpGetLamps(TArray<uint8>& Lamps)
{
	HRESULT Hr;
	VARIANT VarLamps;
	LONG lstart, lend;
	VARIANT HUGEP* Pbstr;

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

return;
}

void UVPmame::get_RawDmdWidth(int& Width) { GPController->get_RawDmdWidth(&Width); }
void UVPmame::get_DmdWidth(int& Width) { GPController->get_DmdWidth(&Width); }
void UVPmame::Run(int nMinVersion) { GPController->Run(0,nMinVersion); }
void UVPmame::Stop() { GPController->Stop(); }
void UVPmame::ShowOptsDialog() { GPController->ShowOptsDialog(0); }
void UVPmame::ShowAboutDialog() { GPController->ShowAboutDialog(0); }
void UVPmame::ShowPathsDialog () { GPController->ShowPathesDialog(0); }
void UVPmame::get_Lamp (int nLamp, bool &pVal )
{
	VARIANT_BOOL tBoolVal;
	GPController->get_Lamp(nLamp, &tBoolVal);
	pVal = (tBoolVal != 0) ? true : false;
}
void UVPmame::get_Solenoid (int nSolenoid, bool &pVal )
{
	VARIANT_BOOL tBoolVal;
	GPController->get_Solenoid(nSolenoid, &tBoolVal);
	pVal = (tBoolVal != 0) ? true : false;
}
void UVPmame::get_Switch (int nSwitchNo, bool &pVal )
{
	VARIANT_BOOL tBoolVal;
	GPController->get_Switch(nSwitchNo, &tBoolVal);
	pVal = (tBoolVal != 0) ? true : false;
}
void UVPmame:: put_Switch (int nSwitchNo,bool pVal )
{
	VARIANT_BOOL tBoolVal = pVal ? -1 : 0;
	GPController->put_Switch(nSwitchNo, tBoolVal);
}
