// Fill out your copyright notice in the Description page of Project Settings.


#include "VPmame.h"
#include "Windows/AllowWindowsPlatformTypes.h" // Add vanilla C++ header :Start
#include "com.h"
#include "Windows/HideWindowsPlatformTypes.h" // Add vanilla C++ header :End


static IController* GPController = nullptr;
static IConnectionPointContainer* GPControllerConnectionPointContainer = nullptr;
static IConnectionPoint* GPControllerConnectionPoint = nullptr;
static BSTR GSGameName;



void UVPmame::VpStart(const FString& RomName) // Get romname from blueprint and start Pinmame emulator
{
	/* Pinmame COM object creation */
	CLSID ClsID;
	HRESULT Hr = CLSIDFromProgID(OLESTR("VPinMAME.Controller"), &ClsID); // Get class ID from program ID
	if (FAILED(Hr)) {
		UE_LOG(LogTemp, Warning, TEXT("Class couldn't be found. Maybe it isn't registered"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Class found."));
	}

	Hr = CoCreateInstance(ClsID, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IController), reinterpret_cast<void**>(&GPController)); // Create COM object
	if (FAILED(Hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't create the Controller class! \nPlease check that you have installed Visual PinMAME properly!"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Controller class Created."));
	}

	Hr = GPController->QueryInterface(IID_IConnectionPointContainer, reinterpret_cast<void**>(&GPControllerConnectionPointContainer)); // Get pointer to COM interfaces
	if (FAILED(Hr)) {
		UE_LOG(LogTemp, Log, TEXT("QueryInterface Failed!"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("QueryInterface succeeded."));
	}

	if (SUCCEEDED(Hr))
		Hr = GPControllerConnectionPointContainer->FindConnectionPoint(__uuidof(_IControllerEvents), &GPControllerConnectionPoint);
 



	/* Emulator settings :Used for testing */
	GPController->put_HandleKeyboard(VARIANT_TRUE); // Allow switch input by keyboard
	if (FAILED(Hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't Set HandleKeyboard !"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("HandleKeyboard Set to true."));
	}

	GPController->put_ShowDMDOnly(VARIANT_FALSE); // Show all PinMame UI info, not only the DMD
	if (FAILED(Hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't Set ShowDMDOnly !"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("ShowDMDOnly Set to false."));
	}

	GPController->put_ShowWinDMD(VARIANT_TRUE); // Show UI in resizable window
 

 
	/* Set romname of emulated pinball machine */
	char* GameName = TCHAR_TO_ANSI(*RomName);
	GSGameName = SysAllocStringLen(NULL, (UINT)strlen(GameName));
	MultiByteToWideChar(CP_ACP, 0, GameName, -1, GSGameName, (int)strlen(GameName));


	Hr = GPController->put_GameName(GSGameName);
	if (FAILED(Hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't Set Gamename !"));
		SysFreeString(GSGameName);
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Gamename Set."));
		SysFreeString(GSGameName);
	}

	/* Start emulator */
	GPController->Run(0, 0);
	if (FAILED(Hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't Run !"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Running."));
	}
	return;


}

/* Stop emulator */
void UVPmame::VPStop()
{
	GPController->Stop();
	UE_LOG(LogTemp, Log, TEXT("Stopping Emulator."));
	return;
}

/* Get array of changed DMD pixels in unsigned integer format*/
/* Call will fail if there are no changed pixels, Exit and do not process in this case */
void UVPmame::VPGetDMD(TArray<uint8>& Dots)
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


		Hr = SafeArrayAccessData(psa, reinterpret_cast<void**>(&Pbstr)); // Get pointer to arraydata : Data is passed as VARIANT type
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
	VARIANT VarLamps;
	LONG lstart, lend;
	VARIANT HUGEP* Pbstr;

	HRESULT Hr = GPController->get_Lamps(&VarLamps);
	if (Hr == 1) // Return value of 1 means no changed pixels and no valid data: Exit
	{
		return;
	}
	else if (Hr == 0)
	{
		SAFEARRAY* Psa = VarLamps.parray;

		Hr = SafeArrayAccessData(Psa, reinterpret_cast<void**>(&Pbstr)); // Get pointer to arraydata : Data is passed as VARIANT type
		if (FAILED(Hr))
		{
			return;
		}
		else
		{
			SafeArrayGetLBound(Psa, 1, &lstart); // Get array start : Safearrays can have a starting point other than zero
			SafeArrayGetUBound(Psa, 1, &lend); // Get Array End

			for (long Idx = lstart; Idx <= lend; Idx++) // Pixeldata in VARIANT is stored as Unsigned Integer
			{
				BOOL Lamp;
				Lamp = Pbstr[Idx].boolVal; // Pixeldata in VARIANT is stored as bool
				Lamps.EmplaceAt(Idx, Lamp);
			}


			SafeArrayUnaccessData(Psa); // Release safearray for updating
			SafeArrayDestroy(Psa); // Destroy safearray object

			return;
		}
	}

return;


}

