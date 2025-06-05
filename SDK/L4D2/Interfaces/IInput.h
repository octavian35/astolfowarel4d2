#pragma once
//#include "../public/inputsystem/ButtonCode.h"
#include "EngineClient.h"
class bf_write;
class bf_read;
class kbutton_t;
class CUserCmd;
class C_BaseCombatWeapon;

struct CameraThirdData_t
{
    float   m_flPitch;
    float   m_flYaw;
    float   m_flDist;
    float   m_flLag;
    Vector  m_vecHullMin;
    Vector  m_vecHullMax;
};
enum ButtonCode_t
{
	BUTTON_CODE_INVALID = -1,
	BUTTON_CODE_NONE = 0,

	KEY_FIRST = 0,

	KEY_NONE = KEY_FIRST,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,

	KEY_LAST = KEY_SCROLLLOCKTOGGLE,
	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

	// Mouse
	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up 
	MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down

	MOUSE_LAST = MOUSE_WHEEL_DOWN,
	MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1,

	// Joystick
	JOYSTICK_FIRST = MOUSE_LAST + 1,

	JOYSTICK_FIRST_BUTTON = JOYSTICK_FIRST,



	KEY_XBUTTON_A = JOYSTICK_FIRST_BUTTON,		// Buttons
	KEY_XBUTTON_B,
	KEY_XBUTTON_X,
	KEY_XBUTTON_Y,
	KEY_XBUTTON_LEFT_SHOULDER,
	KEY_XBUTTON_RIGHT_SHOULDER,
	KEY_XBUTTON_BACK,
	KEY_XBUTTON_START,
	KEY_XBUTTON_STICK1,
	KEY_XBUTTON_STICK2,
};
struct IInput_t
{
	// Initialization/shutdown of the subsystem
	virtual void        Init_All(void) = 0;
	virtual void        Shutdown_All(void) = 0;
	// Latching button states
	virtual int         GetButtonBits(int) = 0;
	// Create movement command
	virtual void        CreateMove(int sequence_number, float input_sample_frametime, bool active) = 0;
	virtual void        ExtraMouseSample(float frametime, bool active) = 0;
	virtual bool        WriteUsercmdDeltaToBuffer(bf_write* buf, int from, int to, bool isnewcommand) = 0;
	virtual void        EncodeUserCmdToBuffer(bf_write& buf, int slot) = 0;
	virtual void        DecodeUserCmdFromBuffer(bf_read& buf, int slot) = 0;

	virtual CUserCmd* GetUserCmd(int sequence_number) = 0;

	virtual void        MakeWeaponSelection(C_BaseCombatWeapon* weapon) = 0;

	// Retrieve key state
	virtual float       KeyState(kbutton_t* key) = 0;
	// Issue key event
	virtual int         KeyEvent(int eventcode, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;
	// Look for key
	virtual kbutton_t* FindKey(const char* name) = 0;

	// Issue commands from controllers
	virtual void        ControllerCommands(void) = 0;
	// Extra initialization for some joysticks
	virtual void        Joystick_Advanced(void) = 0;
	virtual void        Joystick_SetSampleTime(float frametime) = 0;
	virtual void        IN_SetSampleTime(float frametime) = 0;

	// Accumulate mouse delta
	virtual void        AccumulateMouse(void) = 0;
	// Activate/deactivate mouse
	virtual void        ActivateMouse(void) = 0;
	virtual void        DeactivateMouse(void) = 0;

	// Clear mouse state data
	virtual void        ClearStates(void) = 0;
	// Retrieve lookspring setting
	virtual float       GetLookSpring(void) = 0;

	// Retrieve mouse position
	virtual void        GetFullscreenMousePos(int* mx, int* my, int* unclampedx = 0, int* unclampedy = 0) = 0;
	virtual void        SetFullscreenMousePos(int mx, int my) = 0;
	virtual void        ResetMouse(void) = 0;
	virtual float       GetLastForwardMove(void) = 0;
	virtual float       Joystick_GetForward(void) = 0;
	virtual float       Joystick_GetSide(void) = 0;
	virtual float       Joystick_GetPitch(void) = 0;
	virtual float       Joystick_GetYaw(void) = 0;

	// Third Person camera ( TODO/FIXME:  Move this to a separate interface? )
	virtual void        CAM_Think(void) = 0;
	virtual int         CAM_IsThirdPerson(void) = 0;
	virtual void        CAM_ToThirdPerson(void) = 0;
	virtual void        CAM_ToFirstPerson(void) = 0;
	virtual void        CAM_StartMouseMove(void) = 0;
	virtual void        CAM_EndMouseMove(void) = 0;
	virtual void        CAM_StartDistance(void) = 0;
	virtual void        CAM_EndDistance(void) = 0;
	virtual int         CAM_InterceptingMouse(void) = 0;

	// orthographic camera info ( TODO/FIXME:  Move this to a separate interface? )
	virtual void        CAM_ToOrthographic() = 0;
	virtual bool        CAM_IsOrthographic() const = 0;
	virtual void        CAM_OrthographicSize(float& w, float& h) const = 0;

	virtual void        LevelInit(void) = 0;

	// Causes an input to have to be re-pressed to become active
	virtual void        ClearInputButton(int bits) = 0;

	virtual void        CAM_SetCameraThirdData(CameraThirdData_t* pCameraData, const Vector& vecCameraOffset) = 0;
	virtual void        CAM_CameraThirdThink(void) = 0;

	virtual bool        EnableJoystickMode() = 0;

public:
	// bool m_fCameraInThirdPerson; // 0x00B1
	bool& m_fCameraInThirdPerson() {
		return *reinterpret_cast<bool*>(reinterpret_cast<DWORD>(this) + 0x00B1);
	}

public:
	// Vector ThirdpersonCameraAngle; // 0x00B4
	// Vector m_vecCameraOffset; // 0x00B4
	Vector& m_vecCameraOffset() {
		// return reinterpret_cast<Vector*>(reinterpret_cast<DWORD>(this) + 0x00B4);
		return *reinterpret_cast<Vector*>(reinterpret_cast<DWORD>(this) + 0x00B4);
	}
};

namespace I { inline IInput_t* Input; }