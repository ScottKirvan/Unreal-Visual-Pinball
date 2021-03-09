// Fill out your copyright notice in the Description page of Project Settings.

#include "VPmame.h"
#include "UE4_VPinMAME.h"

static HICON m_hIcon = 0;
static IConnectionPointContainer* GPControllerConnectionPointContainer = NULL;
static IConnectionPoint* GPControllerConnectionPoint = NULL;
static DWORD dwControllerCookie = 0;
//static HWND hWnd = NULL;
static BSTR GPGameName;

UVPmame::UVPmame() {
	CLSID ClsID;
	HRESULT hr;
	GPController = NULL;
	hr = CLSIDFromProgID(OLESTR("VPinMAME.Controller"), &ClsID); // Get class ID from program ID
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Warning, TEXT("Class couldn't be found. Maybe it isn't registered"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Class found."));
	}

	hr = CoCreateInstance(ClsID, NULL, CLSCTX_INPROC_SERVER, __uuidof(IController), (void**)&GPController); // Create COM object
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't create the Controller class! \nPlease check that you have installed Visual PinMAME properly!"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Controller class Created."));
	}

	hr = GPController->QueryInterface(IID_IConnectionPointContainer, (void**)&GPControllerConnectionPointContainer); // Get pointer to COM interfaces
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Log, TEXT("QueryInterface Failed!"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("QueryInterface succeeded."));
	}

	if (SUCCEEDED(hr))
		hr = GPControllerConnectionPointContainer->FindConnectionPoint(__uuidof(_IControllerEvents), &GPControllerConnectionPoint);

	FUE4_VPinMAMEModule::Get().SetPinMAME(this);
};

void UVPmame::VPStart(const FString& RomName) // Get romname from blueprint and start Pinmame emulator
{
	/* Pinmame COM object creation */
	//CLSID ClsID;
	HRESULT hr;
	/*
	hr = CLSIDFromProgID(OLESTR("VPinMAME.Controller"), &ClsID); // Get class ID from program ID
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Warning, TEXT("Class couldn't be found. Maybe it isn't registered"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Class found."));
	}

	hr = CoCreateInstance(ClsID, NULL, CLSCTX_INPROC_SERVER, __uuidof(IController), (void**)&GPController); // Create COM object
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't create the Controller class! \nPlease check that you have installed Visual PinMAME properly!"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Controller class Created."));
	}

	hr = GPController->QueryInterface(IID_IConnectionPointContainer, (void**)&GPControllerConnectionPointContainer); // Get pointer to COM interfaces
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Log, TEXT("QueryInterface Failed!"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("QueryInterface succeeded."));
	}

	if (SUCCEEDED(hr))
		hr = GPControllerConnectionPointContainer->FindConnectionPoint(__uuidof(_IControllerEvents), &GPControllerConnectionPoint);
	*/
 

	/* Emulator settings :Used for testing */
	/*
	hr = GPController->put_HandleKeyboard(VARIANT_TRUE); // Allow switch input by keyboard
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't Set HandleKeyboard !"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("HandleKeyboard Set to true."));
	}

	GPController->put_ShowDMDOnly(VARIANT_FALSE); // Show all PinMame UI info, not only the DMD
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't Set ShowDMDOnly !"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("ShowDMDOnly Set to false."));
	}

	GPController->put_ShowWinDMD(VARIANT_TRUE); // Show UI in resizable window
	*/
 
	/* Set romname of emulated pinball machine */
	char* GameName = TCHAR_TO_ANSI(*RomName);
	GPGameName = SysAllocStringLen(NULL, (UINT)strlen(GameName));
	MultiByteToWideChar(CP_ACP, 0, GameName, -1, GPGameName, (int)strlen(GameName));

	hr = GPController->put_GameName(GPGameName);
	if (FAILED(hr)) {
		UE_LOG(LogTemp, Log, TEXT("Can't Set Gamename !"));
		SysFreeString(GPGameName);
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Gamename Set."));
		SysFreeString(GPGameName);
	}

	/* Start emulator */
	GPController->Run(0, 0);     // TODO - probably should have a nMinVersion number here?
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
	GPController->Stop();
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

	hr = GPController->get_RawDmdPixels(&varDmdPixels);
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

	hr = GPController->get_Lamps(&varLamps);
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
