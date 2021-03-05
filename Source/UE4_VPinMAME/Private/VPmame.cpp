// Fill out your copyright notice in the Description page of Project Settings.


#include "VPmame.h"
#include "Windows/AllowWindowsPlatformTypes.h" // Add vanilla C++ header :Start

extern "C"  // this end-runs a compiler error - but I think it's still functional
{
	long __cdecl _InterlockedIncrement(long volatile* pn);
	long __cdecl _InterlockedDecrement(long volatile* pn);
}
inline long InterlockedIncrement(long volatile* pn) { return _InterlockedIncrement(pn); }
inline long InterlockedDecrement(long volatile* pn) { return _InterlockedDecrement(pn); }

#include "com.h"
#include "Windows/HideWindowsPlatformTypes.h" // Add vanilla C++ header :End


static IController* pController = NULL;
static HICON m_hIcon = 0;
static IConnectionPointContainer* pControllerConnectionPointContainer = NULL;
static IConnectionPoint* pControllerConnectionPoint = NULL;
static DWORD dwControllerCookie = 0;
static HWND hWnd = NULL;
static BSTR sGameName;



void UVPmame::VPStart(const FString& RomName) // Get romname from blueprint and start Pinmame emulator
{
	/* Pinmame COM object creation */
	CLSID ClsID;
	HRESULT hr;
	hr = CLSIDFromProgID(OLESTR("VPinMAME.Controller"), &ClsID); // Get class ID from program ID
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Warning, TEXT("Class couldn't be found. Maybe it isn't registered"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Class found."));
	}

	hr = CoCreateInstance(ClsID, NULL, CLSCTX_INPROC_SERVER, __uuidof(IController), (void**)&pController); // Create COM object
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't create the Controller class! \nPlease check that you have installed Visual PinMAME properly!"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Controller class Created."));
	}

	hr = pController->QueryInterface(IID_IConnectionPointContainer, (void**)&pControllerConnectionPointContainer); // Get pointer to COM interfaces
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Log, TEXT("QueryInterface Failed!"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("QueryInterface succeeded."));
	}

	if (SUCCEEDED(hr))
		hr = pControllerConnectionPointContainer->FindConnectionPoint(__uuidof(_IControllerEvents), &pControllerConnectionPoint);
 



	/* Emulator settings :Used for testing */
	pController->put_HandleKeyboard(VARIANT_TRUE); // Allow switch input by keyboard
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't Set HandleKeyboard !"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("HandleKeyboard Set to true."));
	}

	pController->put_ShowDMDOnly(VARIANT_FALSE); // Show all PinMame UI info, not only the DMD
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't Set ShowDMDOnly !"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("ShowDMDOnly Set to false."));
	}

	pController->put_ShowWinDMD(VARIANT_TRUE); // Show UI in resizable window
 

 
	/* Set romname of emulated pinball machine */
	char* GameName = TCHAR_TO_ANSI(*RomName);
	sGameName = SysAllocStringLen(NULL, (UINT)strlen(GameName));
	MultiByteToWideChar(CP_ACP, 0, GameName, -1, sGameName, (int)strlen(GameName));


	hr = pController->put_GameName(sGameName);
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't Set Gamename !"));
		SysFreeString(sGameName);
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Gamename Set."));
		SysFreeString(sGameName);
	}

	/* Start emulator */
	pController->Run(0, 0);
	if (FAILED(hr)) {
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
	pController->Stop();
	UE_LOG(LogTemp, Log, TEXT("Stopping Emulator."));
	return;
}

/* Get array of changed DMD pixels in unsigned integer format*/
/* Call will fail if there are no changed pixels, Exit and do not process in this case */
void UVPmame::VPGetDMD(TArray<uint8>& Dots)
{
	HRESULT hr;
	VARIANT varDmdPixels;
	LONG lstart, lend;
	VARIANT HUGEP* pbstr;
	SAFEARRAY* psa = new SAFEARRAY;

	hr = pController->get_RawDmdPixels(&varDmdPixels);
	if (hr == 1) // Return value of 1 means no changed pixels and no valid data: Exit
	{
		return;
	}
	else if (hr == 0) // Return value of 0 means changed pixels : Process
	{
		psa = varDmdPixels.parray; //Get pointer to safeArray


		hr = SafeArrayAccessData(psa, (void HUGEP**) & pbstr); // Get pointer to arraydata : Data is passed as VARIANT type
		if (FAILED(hr))
		{
			return;
		}
		else
		{
			SafeArrayGetLBound(psa, 1, &lstart); // Get Array Start : Safearrays can have a starting point other than zero
			SafeArrayGetUBound(psa, 1, &lend); // Get Array End
			for (long idx = lstart; idx <= lend; idx++) // Loop to fill Blueprint array with updated pixeldata
			{
				BYTE pixel;
				pixel = pbstr[idx].uintVal; // Pixeldata in VARIANT is stored as Unsigned Integer
				pixel = (pixel - 20) * 3;
				Dots.EmplaceAt(idx, pixel);
			}

			SafeArrayUnaccessData(psa); // Release safearray for updating
			SafeArrayDestroy(psa);	// Destroy safearray object
			return;

		}
	}

}


/* Get array of changed lamps in unsigned integer format*/
/* Call will fail if there are no changed lamps, Exit and do not process in this case */
void UVPmame::VPGetLamps(TArray<uint8>& Lamps)
{
	HRESULT hr;
	VARIANT varLamps;
	LONG lstart, lend;
	VARIANT HUGEP* pbstr;
	SAFEARRAY* psa = new SAFEARRAY;

	hr = pController->get_Lamps(&varLamps);
	if (hr == 1) // Return value of 1 means no changed lamps and no valid data: Exit
	{
		return;
	}
	else if (hr == 0) // Return value of 0 means changed lamps : Process
	{
		psa = varLamps.parray;

		hr = SafeArrayAccessData(psa, (void HUGEP**) & pbstr); // Get pointer to arraydata : Data is passed as VARIANT type
		if (FAILED(hr))
		{
			return;
		}
		else
		{
			SafeArrayGetLBound(psa, 1, &lstart); // Get array start : Safearrays can have a starting point other than zero
			SafeArrayGetUBound(psa, 1, &lend); // Get Array End

			for (long idx = lstart; idx <= lend; idx++) // Loop to fill Blueprint array with updated lampdata
			{
				BOOL lamp;
				lamp = pbstr[idx].boolVal; // lampdata in VARIANT is stored as bool
				Lamps.EmplaceAt(idx, lamp);
			}


			SafeArrayUnaccessData(psa); // Release safearray for updating
			SafeArrayDestroy(psa); // Destroy safearray object

			return;
		}
	}

return;


}

