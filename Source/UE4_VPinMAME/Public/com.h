// Created by Microsoft (R) C/C++ Compiler Version 14.21.27702.2 (ab812787).
//
// VPINMAME\SOURCE\SAMbuild-master\x64\Release\vpinmame.tlh
//
// C++ source equivalent of type library VPinMAME.tlb
// compiler-generated file created 07/09/19 at 18:22:40 - DO NOT EDIT!

#pragma once
#pragma pack(push, 8)

#include <comdef.h>

//
// Forward references and typedefs
//

struct __declspec(uuid("57270b76-c846-4b1e-88d4-53c8337a0623"))
/* LIBID */ __VPinMAMELib;
struct /* coclass */ Game;
struct __declspec(uuid("bd262bee-902b-47d2-8cc4-54890bb7dc1c"))
/* dual interface */ IGame;
struct __declspec(uuid("fdad01ba-ce74-46aa-a1bc-6a176ab22902"))
/* dual interface */ IRoms;
struct __declspec(uuid("af29eada-63c3-4cb9-89ab-1a8d2f280710"))
/* dual interface */ IRom;
struct __declspec(uuid("693c0a27-a067-4895-b948-65912551e7c3"))
/* dual interface */ IGameSettings;
struct /* coclass */ GameSettings;
struct /* coclass */ Games;
struct __declspec(uuid("0f7b0ec8-343f-48d3-b3b8-98ba4ac621e9"))
/* dual interface */ IGames;
struct /* coclass */ EnumGames;
struct __declspec(uuid("6a4dae2f-2fee-4e6f-86ba-5a93742e584c"))
/* dual interface */ IEnumGames;
struct /* coclass */ Rom;
struct /* coclass */ Roms;
struct /* coclass */ EnumRoms;
struct __declspec(uuid("821a5916-a71f-4ae0-b48e-da6ffc0af6cd"))
/* dual interface */ IEnumRoms;
struct __declspec(uuid("2ce80ec7-ac05-4087-83e7-414de4f0f545"))
/* dispinterface */ _IControllerEvents;
struct /* coclass */ ControllerSettings;
struct __declspec(uuid("1bfe619e-a76e-465b-b11e-4a2853891a45"))
/* dual interface */ IControllerSettings;
struct /* coclass */ Controller;
struct __declspec(uuid("ce9ecc7c-960f-407e-b27b-62e39ab1e30f"))
/* dual interface */ IController;
struct /* coclass */ WSHDlgCtrl;
struct __declspec(uuid("18b280d8-b768-4709-86f4-6675f3fe601b"))
/* dual interface */ IWSHDlgCtrl;
struct /* coclass */ WSHDlgCtrls;
struct __declspec(uuid("80936d79-b8ca-45b1-93b5-224458c8ad5a"))
/* dual interface */ IWSHDlgCtrls;
struct /* coclass */ EnumWSHDlgCtrls;
struct __declspec(uuid("d52e5842-7a8d-4ab0-a8ae-e8ffc9ec9fb7"))
/* dual interface */ IEnumWSHDlgCtrls;
struct /* coclass */ WSHDlg;
struct __declspec(uuid("c0bc59ef-ed67-4fc0-aee3-23ba1028f65d"))
/* dual interface */ IWSHDlg;
#if !defined(_WIN64)
typedef __w64 long LONG_PTR;
#else
typedef __int64 LONG_PTR;
#endif

//
// Smart pointer typedef declarations
//

_COM_SMARTPTR_TYPEDEF(IRom, __uuidof(IRom));
_COM_SMARTPTR_TYPEDEF(IRoms, __uuidof(IRoms));
_COM_SMARTPTR_TYPEDEF(IGameSettings, __uuidof(IGameSettings));
_COM_SMARTPTR_TYPEDEF(IGame, __uuidof(IGame));
_COM_SMARTPTR_TYPEDEF(IGames, __uuidof(IGames));
_COM_SMARTPTR_TYPEDEF(IEnumGames, __uuidof(IEnumGames));
_COM_SMARTPTR_TYPEDEF(IEnumRoms, __uuidof(IEnumRoms));
_COM_SMARTPTR_TYPEDEF(_IControllerEvents, __uuidof(_IControllerEvents));
_COM_SMARTPTR_TYPEDEF(IControllerSettings, __uuidof(IControllerSettings));
_COM_SMARTPTR_TYPEDEF(IController, __uuidof(IController));
_COM_SMARTPTR_TYPEDEF(IWSHDlgCtrl, __uuidof(IWSHDlgCtrl));
_COM_SMARTPTR_TYPEDEF(IWSHDlgCtrls, __uuidof(IWSHDlgCtrls));
_COM_SMARTPTR_TYPEDEF(IEnumWSHDlgCtrls, __uuidof(IEnumWSHDlgCtrls));
_COM_SMARTPTR_TYPEDEF(IWSHDlg, __uuidof(IWSHDlg));

//
// Type library items
//

struct __declspec(uuid("75d6fff0-21e5-4b71-90b4-59409a27297c"))
Game;
    // [ default ] interface IGame

struct __declspec(uuid("af29eada-63c3-4cb9-89ab-1a8d2f280710"))
IRom : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall get_Name (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_State (
        /*[out,retval]*/ long * pVal ) = 0;
      virtual HRESULT __stdcall get_StateDescription (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall Audit (
        /*[in]*/ VARIANT_BOOL fStrict ) = 0;
      virtual HRESULT __stdcall get_Length (
        /*[out,retval]*/ long * pVal ) = 0;
      virtual HRESULT __stdcall get_ExpLength (
        /*[out,retval]*/ long * pVal ) = 0;
      virtual HRESULT __stdcall get_Checksum (
        /*[out,retval]*/ long * pVal ) = 0;
      virtual HRESULT __stdcall get_ExpChecksum (
        /*[out,retval]*/ long * pVal ) = 0;
      virtual HRESULT __stdcall get_Flags (
        /*[out,retval]*/ long * pVal ) = 0;
};

struct __declspec(uuid("fdad01ba-ce74-46aa-a1bc-6a176ab22902"))
IRoms : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ VARIANT * pKey,
        /*[out,retval]*/ struct IRom * * ppGame ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppunkEnum ) = 0;
      virtual HRESULT __stdcall get_Available (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall get_State (
        /*[out,retval]*/ long * pVal ) = 0;
      virtual HRESULT __stdcall get_StateDescription (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall Audit (
        /*[in]*/ VARIANT_BOOL fStrict ) = 0;
};

struct __declspec(uuid("693c0a27-a067-4895-b948-65912551e7c3"))
IGameSettings : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall ShowSettingsDlg (
        /*[in]*/ LONG_PTR hParentWnd ) = 0;
      virtual HRESULT __stdcall Clear ( ) = 0;
      virtual HRESULT __stdcall get_Value (
        /*[in]*/ BSTR sName,
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall put_Value (
        /*[in]*/ BSTR sName,
        /*[in]*/ VARIANT pVal ) = 0;
      virtual HRESULT __stdcall SetDisplayPosition (
        /*[in]*/ VARIANT newValX,
        /*[in]*/ VARIANT newValY,
        /*[in]*/ LONG_PTR hWnd ) = 0;
};

struct __declspec(uuid("bd262bee-902b-47d2-8cc4-54890bb7dc1c"))
IGame : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall get_Name (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_Description (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_Year (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_Manufacturer (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_CloneOf (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_Roms (
        /*[out,retval]*/ struct IRoms * * pVal ) = 0;
      virtual HRESULT __stdcall get_Settings (
        /*[out,retval]*/ struct IGameSettings * * pVal ) = 0;
      virtual HRESULT __stdcall ShowInfoDlg (
        /*[in]*/ int nShowOptions,
        /*[in]*/ LONG_PTR hParentWnd,
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_IsSupported (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
};

struct __declspec(uuid("b9af5401-086e-4359-b998-8b5fa0510fbc"))
GameSettings;
    // [ default ] interface IGameSettings

struct __declspec(uuid("b31ad191-c0c5-4e44-af24-bd0459eda52f"))
Games;
    // [ default ] interface IGames

struct __declspec(uuid("0f7b0ec8-343f-48d3-b3b8-98ba4ac621e9"))
IGames : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ VARIANT * pKey,
        /*[out,retval]*/ struct IGame * * ppGame ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppunkEnum ) = 0;
};

struct __declspec(uuid("d1fab4a1-f9e6-4fef-a02f-bd218ec5b792"))
EnumGames;
    // [ default ] interface IEnumGames

struct __declspec(uuid("6a4dae2f-2fee-4e6f-86ba-5a93742e584c"))
IEnumGames : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall Next (
        /*[in]*/ unsigned long celt,
        /*[out]*/ VARIANT * rgVar,
        /*[out]*/ unsigned long * pCeltFetched ) = 0;
      virtual HRESULT __stdcall Skip (
        /*[in]*/ unsigned long celt ) = 0;
      virtual HRESULT __stdcall Reset ( ) = 0;
      virtual HRESULT __stdcall Clone (
        /*[out]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("ccbad9cc-cc5a-4dd3-ba51-391f3744d815"))
Rom;
    // [ default ] interface IRom

struct __declspec(uuid("c983a522-7dc2-4153-bf78-5dd5a2dade01"))
Roms;
    // [ default ] interface IRoms

struct __declspec(uuid("ef1f6336-7983-4cc4-8fa2-1ce9edfc964f"))
EnumRoms;
    // [ default ] interface IEnumRoms

struct __declspec(uuid("821a5916-a71f-4ae0-b48e-da6ffc0af6cd"))
IEnumRoms : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall Next (
        /*[in]*/ unsigned long celt,
        /*[out]*/ VARIANT * rgVar,
        /*[out]*/ unsigned long * pCeltFetched ) = 0;
      virtual HRESULT __stdcall Skip (
        /*[in]*/ unsigned long celt ) = 0;
      virtual HRESULT __stdcall Reset ( ) = 0;
      virtual HRESULT __stdcall Clone (
        /*[out]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("2ce80ec7-ac05-4087-83e7-414de4f0f545"))
_IControllerEvents : IDispatch
{};

struct __declspec(uuid("dc6936cd-13be-4091-98e3-071acc38ccb8"))
ControllerSettings;
    // [ default ] interface IControllerSettings

struct __declspec(uuid("1bfe619e-a76e-465b-b11e-4a2853891a45"))
IControllerSettings : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall ShowSettingsDlg (
        LONG_PTR hParentWnd ) = 0;
      virtual HRESULT __stdcall Clear ( ) = 0;
      virtual HRESULT __stdcall get_Value (
        /*[in]*/ BSTR sName,
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall put_Value (
        /*[in]*/ BSTR sName,
        /*[in]*/ VARIANT pVal ) = 0;
      virtual HRESULT __stdcall get_InstallDir (
        /*[out,retval]*/ BSTR * pVal ) = 0;
};

struct __declspec(uuid("f389c8b7-144f-4c63-a2e3-246d168f9d39"))
Controller;
    // [ default ] interface IController
    // [ default, source ] dispinterface _IControllerEvents

struct __declspec(uuid("ce9ecc7c-960f-407e-b27b-62e39ab1e30f"))
IController : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall Run (
        /*[in]*/ LONG_PTR hParentWnd,
        /*[in]*/ int nMinVersion ) = 0;
      virtual HRESULT __stdcall Stop ( ) = 0;
      virtual HRESULT __stdcall ShowOptsDialog (
        /*[in]*/ LONG_PTR hParentWnd ) = 0;
      virtual HRESULT __stdcall ShowAboutDialog (
        /*[in]*/ LONG_PTR hParentWnd ) = 0;
      virtual HRESULT __stdcall get_Lamp (
        /*[in]*/ int nLamp,
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall get_Solenoid (
        /*[in]*/ int nSolenoid,
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall get_Switch (
        /*[in]*/ int nSwitchNo,
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_Switch (
        /*[in]*/ int nSwitchNo,
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_RomDirs (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_RomDirs (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_NVRamDir (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_NVRamDir (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_SamplesDir (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_SamplesDir (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_CfgDir (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_CfgDir (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_WPCNumbering (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall get_Lamps (
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_ChangedLamps (
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_Switches (
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall put_Switches (
        /*[in]*/ VARIANT pVal ) = 0;
      virtual HRESULT __stdcall get_ChangedSolenoids (
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_GameName (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_GameName (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_InstallDir (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_HandleKeyboard (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_HandleKeyboard (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_UseSamples (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_UseSamples (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_ShowTitle (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_ShowTitle (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_Pause (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_Pause (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_Machines (
        /*[in]*/ BSTR sMachine,
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_Running (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall CheckROMS (
        /*[in]*/ int nShowOptions,
        /*[in]*/ LONG_PTR hParentWnd,
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall get_HandleMechanics (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall put_HandleMechanics (
        /*[in]*/ int pVal ) = 0;
      virtual HRESULT __stdcall get_GIString (
        int nString,
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_ChangedGIStrings (
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_ShowDMDOnly (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_ShowDMDOnly (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_SplashInfoLine (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_SplashInfoLine (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_Solenoids (
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_Dip (
        /*[in]*/ int nNo,
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall put_Dip (
        /*[in]*/ int nNo,
        /*[in]*/ int pVal ) = 0;
      virtual HRESULT __stdcall get_GIStrings (
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_GetMech (
        /*[in]*/ int mechNo,
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_ShowFrame (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_ShowFrame (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_BorderSizeX (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall put_BorderSizeX (
        /*[in]*/ int pVal ) = 0;
      virtual HRESULT __stdcall get_BorderSizeY (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall put_BorderSizeY (
        /*[in]*/ int pVal ) = 0;
      virtual HRESULT __stdcall get_WindowPosX (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall put_WindowPosX (
        /*[in]*/ int pVal ) = 0;
      virtual HRESULT __stdcall get_WindowPosY (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall put_WindowPosY (
        /*[in]*/ int pVal ) = 0;
      virtual HRESULT __stdcall get_SampleRate (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall put_SampleRate (
        /*[in]*/ int pVal ) = 0;
      virtual HRESULT __stdcall get_DoubleSize (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_DoubleSize (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall SetDisplayPosition (
        /*[in]*/ int x,
        /*[in]*/ int y,
        /*[in]*/ LONG_PTR hParentWindow ) = 0;
      virtual HRESULT __stdcall get_LockDisplay (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_LockDisplay (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_Antialias (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_Antialias (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_SolMask (
        /*[in]*/ int nLow,
        /*[out,retval]*/ long * pVal ) = 0;
      virtual HRESULT __stdcall put_SolMask (
        /*[in]*/ int nLow,
        /*[in]*/ long pVal ) = 0;
      virtual HRESULT __stdcall ShowPathesDialog (
        /*[in]*/ LONG_PTR hParentWnd ) = 0;
      virtual HRESULT __stdcall get_ImgDir (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_ImgDir (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_Version (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_Mech (
        /*[in]*/ int param,
        /*[in]*/ int _arg2 ) = 0;
      virtual HRESULT __stdcall get_NewSoundCommands (
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_Games (
        /*[out,retval]*/ struct IGames * * pVal ) = 0;
      virtual HRESULT __stdcall get_Settings (
        /*[out,retval]*/ struct IControllerSettings * * pVal ) = 0;
      virtual HRESULT __stdcall get_ChangedLEDs (
        /*[in]*/ int nHigh,
        /*[in]*/ int nLow,
        /*[in]*/ int nnHigh,
        /*[in]*/ int nnLow,
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_Hidden (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_Hidden (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_Game (
        /*[out,retval]*/ struct IGame * * pVal ) = 0;
      virtual HRESULT __stdcall get_MechSamples (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_MechSamples (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall GetWindowRect (
        /*[in]*/ LONG_PTR hWnd,
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall GetClientRect (
        /*[in]*/ LONG_PTR hWnd,
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_RawDmdWidth (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_RawDmdHeight (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_RawDmdPixels (
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_DmdWidth (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_DmdHeight (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_DmdPixel (
        /*[in]*/ int x,
        /*[in]*/ int y,
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_updateDmdPixels (
        /*[in]*/ int * * buf,
        /*[in]*/ int width,
        /*[in]*/ int height,
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_ChangedLampsState (
        /*[in]*/ int * * buf,
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_LampsState (
        /*[in]*/ int * * buf,
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_ChangedSolenoidsState (
        /*[in]*/ int * * buf,
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_SolenoidsState (
        /*[in]*/ int * * buf,
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_ChangedGIsState (
        /*[in]*/ int * * buf,
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_MasterVolume (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall put_MasterVolume (
        /*[in]*/ int pVal ) = 0;
      virtual HRESULT __stdcall get_EnumAudioDevices (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_AudioDevicesCount (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_AudioDeviceDescription (
        /*[in]*/ int num,
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_AudioDeviceModule (
        /*[in]*/ int num,
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_CurrentAudioDevice (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall put_CurrentAudioDevice (
        /*[in]*/ int pVal ) = 0;
      virtual HRESULT __stdcall get_FastFrames (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall put_FastFrames (
        /*[in]*/ int pVal ) = 0;
      virtual HRESULT __stdcall get_IgnoreRomCrc (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_IgnoreRomCrc (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_CabinetMode (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_CabinetMode (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_ShowPinDMD (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_ShowPinDMD (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_ShowWinDMD (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_ShowWinDMD (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_ChangedLEDsState (
        /*[in]*/ int nHigh,
        /*[in]*/ int nLow,
        /*[in]*/ int nnHigh,
        /*[in]*/ int nnLow,
        /*[in]*/ int * * buf,
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall get_RawDmdColoredPixels (
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_NVRAM (
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_ChangedNVRAM (
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall get_SoundMode (
        /*[out,retval]*/ int * pVal ) = 0;
      virtual HRESULT __stdcall put_SoundMode (
        /*[in]*/ int pVal ) = 0;
};

struct __declspec(uuid("d3ba6587-82a7-46a7-aefe-7a75cad51f31"))
WSHDlgCtrl;
    // [ default ] interface IWSHDlgCtrl

struct __declspec(uuid("18b280d8-b768-4709-86f4-6675f3fe601b"))
IWSHDlgCtrl : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall get_Type (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_Value (
        /*[out,retval]*/ VARIANT * pVal ) = 0;
      virtual HRESULT __stdcall put_Value (
        /*[in]*/ VARIANT pVal ) = 0;
      virtual HRESULT __stdcall get_Title (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_Title (
        /*[in]*/ BSTR pVal ) = 0;
};

struct __declspec(uuid("73c1a53e-2c2b-42a3-bceb-e9ee0539b0ff"))
WSHDlgCtrls;
    // [ default ] interface IWSHDlgCtrls

struct __declspec(uuid("80936d79-b8ca-45b1-93b5-224458c8ad5a"))
IWSHDlgCtrls : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ VARIANT * pKey,
        /*[out,retval]*/ struct IWSHDlgCtrl * * ppWSHDlgCtrl ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppunkEnum ) = 0;
};

struct __declspec(uuid("587ed49f-b712-48c7-a580-61c25257eb3e"))
EnumWSHDlgCtrls;
    // [ default ] interface IEnumWSHDlgCtrls

struct __declspec(uuid("d52e5842-7a8d-4ab0-a8ae-e8ffc9ec9fb7"))
IEnumWSHDlgCtrls : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall Next (
        /*[in]*/ unsigned long celt,
        /*[out]*/ VARIANT * rgVar,
        /*[out]*/ unsigned long * pCeltFetched ) = 0;
      virtual HRESULT __stdcall Skip (
        /*[in]*/ unsigned long celt ) = 0;
      virtual HRESULT __stdcall Reset ( ) = 0;
      virtual HRESULT __stdcall Clone (
        /*[out]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("1dcaffa5-978d-4cc0-9576-73bc0293e67c"))
WSHDlg;
    // [ default ] interface IWSHDlg

struct __declspec(uuid("c0bc59ef-ed67-4fc0-aee3-23ba1028f65d"))
IWSHDlg : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall get_x (
        /*[out,retval]*/ long * pVal ) = 0;
      virtual HRESULT __stdcall put_x (
        /*[in]*/ long pVal ) = 0;
      virtual HRESULT __stdcall get_y (
        /*[out,retval]*/ long * pVal ) = 0;
      virtual HRESULT __stdcall put_y (
        /*[in]*/ long pVal ) = 0;
      virtual HRESULT __stdcall get_w (
        /*[out,retval]*/ long * pVal ) = 0;
      virtual HRESULT __stdcall put_w (
        /*[in]*/ long pVal ) = 0;
      virtual HRESULT __stdcall get_h (
        /*[out,retval]*/ long * pVal ) = 0;
      virtual HRESULT __stdcall put_h (
        /*[in]*/ long pVal ) = 0;
      virtual HRESULT __stdcall get_Title (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_Title (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall AddCtrl (
        /*[in]*/ BSTR sType,
        /*[in]*/ long x,
        /*[in]*/ long y,
        /*[in]*/ long w,
        /*[in]*/ long h,
        /*[in]*/ BSTR sTitle,
        /*[out,retval]*/ IUnknown * * pRetVal ) = 0;
      virtual HRESULT __stdcall Show (
        /*[in]*/ LONG_PTR hParentWnd,
        /*[out,retval]*/ VARIANT * RetVal ) = 0;
      virtual HRESULT __stdcall get_Ctrls (
        /*[out,retval]*/ struct IWSHDlgCtrls * * ppVal ) = 0;
};

#pragma pack(pop)
