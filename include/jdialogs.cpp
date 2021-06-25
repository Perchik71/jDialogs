/*
Generating Windows dialogs in JSON for C++
Version 0.3
https://github.com/Perchik71/jDialogs

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2021 Alex (Perchik71).

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <fstream>
#include <filesystem>
#include <map>

#include "jdialogs.h"

// Need for InitCommonControlsEx
#include <CommCtrl.h>
#pragma comment (lib, "Comctl32.lib")

#ifndef JDIALOG_NO_MANIFEST_LINKER_COMMCTRL
#pragma comment (linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' ""version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif // !JDIALOG_NO_MANIFEST_LINKER_COMMCTRL

#define JSON_USE_IMPLICIT_CONVERSIONS 0

/*
    __ _____ _____ _____
 __|  |   __|     |   | |  JSON for Modern C++
|  |  |__   |  |  | | | |  version 3.9.1
|_____|_____|_____|_|___|  https://github.com/nlohmann/json

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2013-2019 Niels Lohmann <http://nlohmann.me>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "..\json\single_include\nlohmann\json.hpp"

#define STYLE_DEFAULT (WS_DLGFRAME | DS_MODALFRAME | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | DS_SETFONT | DS_SETFOREGROUND | DS_3DLOOK | WS_CAPTION)

#define JDialogCreateCommControlA(_titled, _classd) CreateControlA(_ex_style, _titled, _classd, _style, _x, _y, _cx, _cy, _uid, _dialog)
#define JDialogCreateCommControlW(_titled, _classd) CreateControlW(_ex_style, _titled, _classd, _style, _x, _y, _cx, _cy, _uid, _dialog)

#define USES_STD_LEN

#ifndef USES_STD_LEN
#define JDialogStrLengthByteA(str) ((strlen(str.c_str())) << 1)
#define JDialogStrLengthByteW(str) ((wcslen(str.c_str())) << 1)
#else
#define JDialogStrLengthByteA(str) ((str.length()) << 1)
#define JDialogStrLengthByteW(str) JDialogStrLengthByteA(str)
#endif // !USES_STD_LEN

namespace perchik71
{
	namespace jDialogs
	{
		bool g_comm_init = false;

		using json = nlohmann::json;
		namespace fs = filesystem;

		typedef uint8_t* lpuint8_t;
		typedef uint16_t* lpuint16_t;
		typedef uint32_t* lpuint32_t;

		/*
		If the first element is 0xFFFF, the array has one additional element that specifies the ordinal value of a predefined system class.
		The ordinal can be one of the following atom values.
		*/

		const map<string, uint16_t> mapSysClassA = {
			{ "BUTTON", 0x0080 },
			{ "EDIT", 0x0081 },
			{ "STATIC", 0x0082 },
			{ "LISTBOX", 0x0083 },
			{ "SCROLLBAR", 0x0084 },
			{ "COMBOBOX", 0x0085 }
		};

		const map<wstring, uint16_t> mapSysClassW = {
			{ L"BUTTON", 0x0080 },
			{ L"EDIT", 0x0081 },
			{ L"STATIC", 0x0082 },
			{ L"LISTBOX", 0x0083 },
			{ L"SCROLLBAR", 0x0084 },
			{ L"COMBOBOX", 0x0085 }
		};

		const map<string, uint32_t> mapStdControlType = {
			{ "TEXT", JDialogDefaultTextStyle },
			{ "LTEXT", JDialogDefaultLeftTextStyle },
			{ "CTEXT", JDialogDefaultCenterTextStyle },
			{ "RTEXT", JDialogDefaultRightTextStyle },
			{ "EDITTEXT", JDialogDefaultEditTextStyle },
			{ "DEFPUSHBUTTON", JDialogDefaultDefPushButtonStyle },
			{ "PUSHBUTTON", JDialogDefaultPushButtonStyle },
			{ "LISTBOX", JDialogDefaultListBoxStyle },
			{ "COMBOBOX", JDialogDefaultComboBoxStyle },
			{ "COMBOBOXEX", JDialogDefaultComboBoxStyle },
			{ "AUTORADIOBUTTON", JDialogDefaultAutoRadioButtonStyle },
			{ "AUTOCHECKBUTTON", JDialogDefaultAutoCheckButtonStyle },
			{ "GROUPBOX", JDialogDefaultGroupBoxStyle },
			{ "LISTVIEW", JDialogDefaultListViewStyle },
			{ "TREEVIEW", JDialogDefaultTreeViewStyle },
			{ "TABVIEW", JDialogDefaultTabViewStyle },
			{ "BITMAP", JDialogDefaultBitmapStyle },
			{ "ICON", JDialogDefaultIconStyle },
			{ "BITMAPEX", JDialogDefaultBitmapExStyle },
			{ "ICONEX", JDialogDefaultIconExStyle },
			{ "SCROLLBAR", JDialogDefaultScrollBarStyle },
			{ "REBAR", JDialogDefaultReBarStyle },
			{ "HOTKEY", JDialogDefaultHotKeyStyle },
			{ "IPADDRESS", JDialogDefaultIPAdressStyle },
			{ "MONTHCAL", JDialogDefaultMonthCalStyle },
			{ "STATUSBAR", JDialogDefaultStatusBarStyle },
			{ "PROGRESSBAR", JDialogDefaultProgressBarStyle },
			{ "ANIMATE", JDialogDefaultAnimateStyle },
			{ "HEADER", JDialogDefaultHeaderStyle },
			{ "RICHEDIT", JDialogDefaultRichEditStyle },
			{ "DATETIMEPICKER", JDialogDefaultDateTimePickerStyle }
		};

		const map<string, string> mapStdControlType2A = {
			{ "TEXT", "STATIC" },
			{ "LTEXT", "STATIC" },
			{ "CTEXT", "STATIC" },
			{ "RTEXT", "STATIC" },
			{ "EDITTEXT", "EDIT" },
			{ "DEFPUSHBUTTON", "BUTTON" },
			{ "PUSHBUTTON", "BUTTON" },
			{ "LISTBOX", "LISTBOX" },
			{ "COMBOBOX", "COMBOBOX" },
			{ "AUTORADIOBUTTON", "BUTTON" },
			{ "AUTOCHECKBUTTON", "BUTTON" },
			{ "GROUPBOX", "BUTTON" },
			{ "LISTVIEW", "SysListView32" },
			{ "TREEVIEW", "SysTreeView32" },
			{ "TABVIEW", "SysTabControl32" },
			{ "BITMAP", "STATIC" },
			{ "ICON", "STATIC" },
			{ "BITMAPEX", "STATIC" },
			{ "ICONEX", "STATIC" },
			{ "SCROLLBAR", "SCROLLBAR" },
			{ "REBAR", "ReBarWindow32" },
			{ "HOTKEY", "msctls_hotkey32" },
			{ "IPADDRESS", "SysIPAddress32" },
			{ "MONTHCAL", "SysMonthCal32" },
			{ "STATUSBAR", "msctls_statusbar32" },
			{ "PROGRESSBAR", "msctls_progress32" },
			{ "ANIMATE", "SysAnimate32" },
			{ "HEADER", "SysHeader32" },
			{ "RICHEDIT", "RichEdit20A" },
			{ "DATETIMEPICKER", "SysDateTimePick32" }
		};

		const map<string, wstring> mapStdControlType2W = {
			{ "TEXT", L"STATIC" },
			{ "LTEXT", L"STATIC" },
			{ "CTEXT", L"STATIC" },
			{ "RTEXT", L"STATIC" },
			{ "EDITTEXT", L"EDIT" },
			{ "DEFPUSHBUTTON", L"BUTTON" },
			{ "PUSHBUTTON", L"BUTTON" },
			{ "LISTBOX", L"LISTBOX" },
			{ "COMBOBOX", L"COMBOBOX" },
			{ "AUTORADIOBUTTON", L"BUTTON" },
			{ "AUTOCHECKBUTTON", L"BUTTON" },
			{ "GROUPBOX", L"BUTTON" },
			{ "LISTVIEW", L"SysListView32" },
			{ "TREEVIEW", L"SysTreeView32" },
			{ "TABVIEW", L"SysTabControl32" },
			{ "BITMAP", L"STATIC" },
			{ "ICON", L"STATIC" },
			{ "BITMAPEX", L"STATIC" },
			{ "ICONEX", L"STATIC" },
			{ "SCROLLBAR", L"SCROLLBAR" },
			{ "REBAR", L"ReBarWindow32" },
			{ "HOTKEY", L"msctls_hotkey32" },
			{ "IPADDRESS", L"SysIPAddress32" },
			{ "MONTHCAL", L"SysMonthCal32" },
			{ "STATUSBAR", L"msctls_statusbar32" },
			{ "PROGRESSBAR", L"msctls_progress32" },
			{ "ANIMATE", L"SysAnimate32" },
			{ "HEADER", L"SysHeader32" },
			{ "RICHEDIT", L"RichEdit20W" },
			{ "DATETIMEPICKER", L"SysDateTimePick32" }
		};

		const map<string, uint32_t> mapControlStyle = {
			{ "WS_BORDER", WS_BORDER },
			{ "WS_DISABLED", WS_DISABLED },
			{ "WS_HSCROLL", WS_HSCROLL },
			{ "WS_VSCROLL", WS_VSCROLL },
			{ "WS_GROUP", WS_GROUP },
			{ "WS_VISIBLE", WS_VISIBLE },
			{ "WS_TABSTOP", WS_TABSTOP },
			{ "WS_THICKFRAME", WS_THICKFRAME },
			{ "WS_CLIPCHILDREN", WS_CLIPCHILDREN },
			{ "WS_CLIPSIBLINGS", WS_CLIPSIBLINGS },
			{ "WS_DLGFRAME", WS_DLGFRAME },
			{ "WS_SYSMENU", WS_SYSMENU },
			{ "WS_MAXIMIZEBOX", WS_MAXIMIZEBOX },
			{ "WS_MINIMIZEBOX", WS_MINIMIZEBOX },
			{ "WS_EX_NOPARENTNOTIFY", WS_EX_NOPARENTNOTIFY },
			{ "WS_EX_ACCEPTFILES", WS_EX_ACCEPTFILES },
			{ "WS_EX_CLIENTEDGE", WS_EX_CLIENTEDGE },
			{ "WS_EX_TRANSPARENT", WS_EX_TRANSPARENT },
			{ "WS_EX_STATICEDGE", WS_EX_STATICEDGE },
			{ "WS_EX_WINDOWEDGE", WS_EX_WINDOWEDGE },
			{ "WS_EX_RIGHT", WS_EX_RIGHT },
			{ "WS_EX_RIGHTSCROLLBAR", WS_EX_RIGHTSCROLLBAR },
			{ "WS_EX_LEFT", WS_EX_LEFT },
			{ "WS_EX_LEFTSCROLLBAR", WS_EX_LEFTSCROLLBAR },
			{ "WS_EX_LTRREADING", WS_EX_LTRREADING },
			{ "WS_EX_RTLREADING", WS_EX_RTLREADING },
			{ "WS_EX_NOINHERITLAYOUT", WS_EX_NOINHERITLAYOUT },
			{ "SS_BITMAP", SS_BITMAP },
			{ "SS_BLACKFRAME", SS_BLACKFRAME },
			{ "SS_BLACKRECT", SS_BLACKRECT },
			{ "SS_CENTER", SS_CENTER },
			{ "SS_CENTERIMAGE", SS_CENTERIMAGE },
			{ "SS_ENDELLIPSIS", SS_ENDELLIPSIS },
			{ "SS_ELLIPSISMASK", SS_ELLIPSISMASK },
			{ "SS_ENHMETAFILE", SS_ENHMETAFILE },
			{ "SS_EDITCONTROL", SS_EDITCONTROL },
			{ "SS_GRAYFRAME", SS_GRAYFRAME },
			{ "SS_GRAYRECT", SS_GRAYRECT },
			{ "SS_ICON", SS_ICON },
			{ "SS_LEFT", SS_LEFT },
			{ "SS_RIGHT", SS_RIGHT },
			{ "SS_RIGHTJUST", SS_RIGHTJUST },
			{ "SS_REALSIZEIMAGE", SS_REALSIZEIMAGE },
			{ "SS_NOPREFIX", SS_NOPREFIX },
			{ "SS_OWNERDRAW", SS_OWNERDRAW },
			{ "SS_NOTIFY", SS_NOTIFY },
			{ "SS_REALSIZECONTROL", SS_REALSIZECONTROL },
			{ "SS_SUNKEN", SS_SUNKEN },
			{ "SS_WHITEFRAME", SS_WHITEFRAME },
			{ "SS_WHITERECT", SS_WHITERECT },
			{ "SS_WORDELLIPSIS", SS_WORDELLIPSIS },
			{ "ES_AUTOHSCROLL", ES_AUTOHSCROLL },
			{ "ES_AUTOVSCROLL", ES_AUTOVSCROLL },
			{ "ES_CENTER", ES_CENTER },
			{ "ES_LEFT", ES_LEFT },
			{ "ES_LOWERCASE", ES_LOWERCASE },
			{ "ES_MULTILINE", ES_MULTILINE },
			{ "ES_NOHIDESEL", ES_NOHIDESEL },
			{ "ES_NUMBER", ES_NUMBER },
			{ "ES_OEMCONVERT", ES_OEMCONVERT },
			{ "ES_PASSWORD", ES_PASSWORD },
			{ "ES_READONLY", ES_READONLY },
			{ "ES_RIGHT", ES_RIGHT },
			{ "ES_UPPERCASE", ES_UPPERCASE },
			{ "ES_WANTRETURN", ES_WANTRETURN },
			{ "BS_BITMAP", BS_BITMAP },
			{ "BS_BOTTOM", BS_BOTTOM },
			{ "BS_CENTER", BS_CENTER },
			{ "BS_COMMANDLINK", BS_COMMANDLINK },
			{ "BS_DEFPUSHBUTTON", BS_DEFPUSHBUTTON },
			{ "BS_DEFCOMMANDLINK", BS_DEFCOMMANDLINK },
			{ "BS_DEFSPLITBUTTON", BS_DEFSPLITBUTTON },
			{ "BS_FLAT", BS_FLAT },
			{ "BS_ICON", BS_ICON },
			{ "BS_OWNERDRAW", BS_OWNERDRAW },
			{ "BS_TOP", BS_TOP },
			{ "BS_VCENTER", BS_VCENTER },
			{ "BS_PUSHLIKE", BS_PUSHLIKE },
			{ "BS_NOTIFY", BS_NOTIFY },
			{ "BS_MULTILINE", BS_MULTILINE },
			{ "LVS_EDITLABELS", LVS_EDITLABELS },
			{ "LVS_ICON", LVS_ICON },
			{ "LVS_LIST", LVS_LIST },
			{ "LVS_OWNERDATA", LVS_OWNERDATA },
			{ "LVS_NOCOLUMNHEADER", LVS_NOCOLUMNHEADER },
			{ "LVS_NOSCROLL", LVS_NOSCROLL },
			{ "LVS_REPORT", LVS_REPORT },
			{ "LVS_NOSORTHEADER", LVS_NOSORTHEADER },
			{ "LVS_ALIGNLEFT", LVS_ALIGNLEFT },
			{ "LVS_OWNERDRAWFIXED", LVS_OWNERDRAWFIXED },
			{ "LVS_AUTOARRANGE", LVS_AUTOARRANGE },
			{ "LVS_SMALLICON", LVS_SMALLICON },
			{ "LVS_SINGLESEL", LVS_SINGLESEL },
			{ "LVS_SORTASCENDING", LVS_SORTASCENDING },
			{ "LVS_SORTDESCENDING", LVS_SORTDESCENDING },
			{ "LVS_SHAREIMAGELISTS", LVS_SHAREIMAGELISTS },
			{ "LVS_NOLABELWRAP", LVS_NOLABELWRAP },
			{ "LVS_SHOWSELALWAYS", LVS_SHOWSELALWAYS },
			{ "LVS_EX_CHECKBOXES", LVS_EX_CHECKBOXES },
			{ "LVS_EX_DOUBLEBUFFER", LVS_EX_DOUBLEBUFFER },
			{ "LVS_EX_AUTOSIZECOLUMNS", LVS_EX_AUTOSIZECOLUMNS },
			{ "LVS_EX_FULLROWSELECT", LVS_EX_FULLROWSELECT },
			{ "LVS_EX_TRACKSELECT", LVS_EX_TRACKSELECT },
			{ "LVS_EX_SINGLEROW", LVS_EX_SINGLEROW },
			{ "LVS_EX_AUTOCHECKSELECT", LVS_EX_AUTOCHECKSELECT },
			{ "LVS_EX_AUTOAUTOARRANGE", LVS_EX_AUTOAUTOARRANGE },
			{ "LVS_EX_BORDERSELECT", LVS_EX_BORDERSELECT },
			{ "LVS_EX_COLUMNOVERFLOW", LVS_EX_COLUMNOVERFLOW },
			{ "LVS_EX_INFOTIP", LVS_EX_INFOTIP },
			{ "LVS_EX_GRIDLINES", LVS_EX_GRIDLINES },
			{ "LVS_EX_TWOCLICKACTIVATE", LVS_EX_TWOCLICKACTIVATE },
			{ "LVS_EX_JUSTIFYCOLUMNS", LVS_EX_JUSTIFYCOLUMNS },
			{ "LVS_EX_LABELTIP", LVS_EX_LABELTIP },
			{ "LVS_EX_FLATSB", LVS_EX_FLATSB },
			{ "LVS_EX_SNAPTOGRID", LVS_EX_SNAPTOGRID },
			{ "CBS_AUTOHSCROLL", CBS_AUTOHSCROLL },
			{ "CBS_DISABLENOSCROLL", CBS_DISABLENOSCROLL },
			{ "CBS_DROPDOWN", CBS_DROPDOWN },
			{ "CBS_DROPDOWNLIST", CBS_DROPDOWNLIST },
			{ "CBS_HASSTRINGS", CBS_HASSTRINGS },
			{ "CBS_LOWERCASE", CBS_LOWERCASE },
			{ "CBS_UPPERCASE", CBS_UPPERCASE },
			{ "CBS_OEMCONVERT", CBS_OEMCONVERT },
			{ "CBS_SORT", CBS_SORT },
			{ "CBS_OWNERDRAWFIXED", CBS_OWNERDRAWFIXED },
			{ "CBS_OWNERDRAWVARIABLE", CBS_OWNERDRAWVARIABLE },
			{ "CBS_NOINTEGRALHEIGHT", CBS_NOINTEGRALHEIGHT },
			{ "LBS_NOTIFY", LBS_NOTIFY },
			{ "LBS_NOINTEGRALHEIGHT", LBS_NOINTEGRALHEIGHT },
			{ "LBS_HASSTRINGS", LBS_HASSTRINGS },
			{ "LBS_MULTICOLUMN", LBS_MULTICOLUMN },
			{ "LBS_MULTIPLESEL", LBS_MULTIPLESEL },
			{ "LBS_EXTENDEDSEL", LBS_EXTENDEDSEL },
			{ "LBS_DISABLENOSCROLL", LBS_DISABLENOSCROLL },
			{ "LBS_COMBOBOX", LBS_COMBOBOX },
			{ "LBS_NOREDRAW", LBS_NOREDRAW },
			{ "LBS_NOSEL", LBS_NOSEL },
			{ "LBS_OWNERDRAWFIXED", LBS_OWNERDRAWFIXED },
			{ "LBS_OWNERDRAWVARIABLE", LBS_OWNERDRAWVARIABLE },
			{ "LBS_SORT", LBS_SORT },
			{ "LBS_STANDARD", LBS_STANDARD },
			{ "LBS_WANTKEYBOARDINPUT", LBS_WANTKEYBOARDINPUT },
			{ "LBS_NODATA", LBS_NODATA },
			{ "LBS_USETABSTOPS", LBS_USETABSTOPS },
			{ "TVS_CHECKBOXES", TVS_CHECKBOXES },
			{ "TVS_DISABLEDRAGDROP", TVS_DISABLEDRAGDROP },
			{ "TVS_EDITLABELS", TVS_EDITLABELS },
			{ "TVS_FULLROWSELECT", TVS_FULLROWSELECT },
			{ "TVS_HASBUTTONS", TVS_HASBUTTONS },
			{ "TVS_HASLINES", TVS_HASLINES },
			{ "TVS_INFOTIP", TVS_INFOTIP },
			{ "TVS_LINESATROOT", TVS_LINESATROOT },
			{ "TVS_NOHSCROLL", TVS_NOHSCROLL },
			{ "TVS_NONEVENHEIGHT", TVS_NONEVENHEIGHT },
			{ "TVS_SINGLEEXPAND", TVS_SINGLEEXPAND },
			{ "TVS_SHOWSELALWAYS", TVS_SHOWSELALWAYS },
			{ "TVS_NOSCROLL", TVS_NOSCROLL },
			{ "TVS_RTLREADING", TVS_RTLREADING },
			{ "TVS_NOTOOLTIPS", TVS_NOTOOLTIPS },
			{ "TVS_TRACKSELECT", TVS_TRACKSELECT },
			{ "TVS_EX_AUTOHSCROLL", TVS_EX_AUTOHSCROLL },
			{ "TVS_EX_DOUBLEBUFFER", TVS_EX_DOUBLEBUFFER },
			{ "TVS_EX_DRAWIMAGEASYNC", TVS_EX_DRAWIMAGEASYNC },
			{ "TVS_EX_DIMMEDCHECKBOXES", TVS_EX_DIMMEDCHECKBOXES },
			{ "TVS_EX_EXCLUSIONCHECKBOXES", TVS_EX_EXCLUSIONCHECKBOXES },
			{ "TVS_EX_FADEINOUTEXPANDOS", TVS_EX_FADEINOUTEXPANDOS },
			{ "TVS_EX_MULTISELECT", TVS_EX_MULTISELECT },
			{ "TVS_EX_RICHTOOLTIP", TVS_EX_RICHTOOLTIP },
			{ "TVS_EX_NOSINGLECOLLAPSE", TVS_EX_NOSINGLECOLLAPSE },
			{ "TVS_EX_NOINDENTSTATE", TVS_EX_NOINDENTSTATE },
			{ "TVS_EX_PARTIALCHECKBOXES", TVS_EX_PARTIALCHECKBOXES },
			{ "CBS_AUTOHSCROLL", CBS_AUTOHSCROLL },
			{ "CBS_DISABLENOSCROLL", CBS_DISABLENOSCROLL },
			{ "CBS_DROPDOWN", CBS_DROPDOWN },
			{ "CBS_DROPDOWNLIST", CBS_DROPDOWNLIST },
			{ "CBS_HASSTRINGS", CBS_HASSTRINGS },
			{ "CBS_LOWERCASE", CBS_LOWERCASE },
			{ "CBS_NOINTEGRALHEIGHT", CBS_NOINTEGRALHEIGHT },
			{ "CBS_OEMCONVERT", CBS_OEMCONVERT },
			{ "CBS_OWNERDRAWFIXED", CBS_OWNERDRAWFIXED },
			{ "CBS_OWNERDRAWVARIABLE", CBS_OWNERDRAWVARIABLE },
			{ "CBS_SIMPLE", CBS_SIMPLE },
			{ "CBS_SORT", CBS_SORT },
			{ "CBS_UPPERCASE", CBS_UPPERCASE },
			{ "SBS_BOTTOMALIGN", SBS_BOTTOMALIGN },
			{ "SBS_HORZ", SBS_HORZ },
			{ "SBS_LEFTALIGN", SBS_LEFTALIGN },
			{ "SBS_RIGHTALIGN", SBS_RIGHTALIGN },
			{ "SBS_SIZEBOX", SBS_SIZEBOX },
			{ "SBS_SIZEBOXBOTTOMRIGHTALIGN", SBS_SIZEBOXBOTTOMRIGHTALIGN },
			{ "SBS_SIZEBOXTOPLEFTALIGN", SBS_SIZEBOXTOPLEFTALIGN },
			{ "SBS_SIZEGRIP", SBS_SIZEGRIP },
			{ "SBS_TOPALIGN", SBS_TOPALIGN },
			{ "SBS_VERT", SBS_VERT },
			{ "PBS_SMOOTH", PBS_SMOOTH },
			{ "PBS_SMOOTHREVERSE", PBS_SMOOTHREVERSE },
			{ "PBS_VERTICAL", PBS_VERTICAL },
			{ "PBS_MARQUEE", PBS_MARQUEE }
		};

		DWORD WINAPI jGetStyleFromString(const string& str)
		{
			if (!str.length())
				return 0;

			// hex
			if (size_t pos = str.find_first_of("0x"); pos == 0)
			{
				string shex = str.substr(2);
				return (DWORD)strtoull(shex.c_str(), NULL, 16);
			}

			CHAR szBuf[64] = { 0 };
			strcpy_s(szBuf, str.c_str());
			_strupr_s(szBuf);

			if (auto it = mapControlStyle.find(szBuf); it != mapControlStyle.end())
				return (DWORD)it->second;

			// decimal
			return (DWORD)strtoull(str.c_str(), NULL, 10);
		}

		/*
		Each DLGITEMTEMPLATEEX structure must be aligned on a DWORD boundary.
		The variable-length windowClass and title arrays must be aligned on WORD boundaries.
		The creation data array, if any, must be aligned on a WORD boundary.
		*/

		template<typename T>
		T WINAPI jAlign(T lpIn, int32_t n)
		{
			uint64_t ul = (uint64_t)lpIn;
			ul += n - 1;
			ul &= -n;
			return (T)ul;
		}

		class jMemoryManager
		{
		private:
			static uint32_t roundUp(uint32_t numToRound, uint32_t multiple)
			{
				if (multiple == 0)
					return numToRound;

				uint32_t remainder = numToRound % multiple;
				if (remainder == 0)
					return numToRound;

				return numToRound + multiple - remainder;
			}
		public:
			static LPVOID Alloc(uint32_t _size) {
				return calloc(1, (uint32_t)roundUp(_size, 4));
			}

			static LPVOID Realloc(LPVOID & _ptr, uint32_t _size) {
				_ptr = _recalloc(_ptr, 1, (uint32_t)roundUp(_size, 4));
				return _ptr;
			}

			static void Free(LPVOID _ptr) {
				free(_ptr);
			}
		};

		void WINAPI jGetDialogUnitsA(const string& _face, uint32_t _fsize, BOOL _italic, uint16_t _weight, uint16_t& x, uint16_t& y)
		{
			if (_face.empty() || !_face.length())
			{
			def_uints:
				LONG l = GetDialogBaseUnits();

				x = LOWORD(l);
				y = HIWORD(l);
			}
			else
			{
				HDC hDC = GetDC(GetDesktopWindow());
				HFONT hFont = CreateFontA(
					-MulDiv(_fsize, GetDeviceCaps(hDC, LOGPIXELSY), 72),
					0, 0, 0, _weight, _italic, 0, 0, DEFAULT_CHARSET,
					0, 0, CLEARTYPE_QUALITY, DEFAULT_PITCH,
					_face.c_str());
				if (!hFont)
				{
					ReleaseDC(GetDesktopWindow(), hDC);
					goto def_uints;
				}

				HFONT hOldFont = (HFONT)SelectObject(hDC, (HGDIOBJ)hFont);

				SIZE s;
				if (!GetTextExtentPoint32A(hDC, "A", 1, &s))
				{
					SelectObject(hDC, (HGDIOBJ)hOldFont);
					DeleteObject((HGDIOBJ)hFont);
					ReleaseDC(GetDesktopWindow(), hDC);
					goto def_uints;
				}

				x = jAlign(s.cx, 2) >> 1;
				y = jAlign(s.cy, 2) >> 1;

				SelectObject(hDC, (HGDIOBJ)hOldFont);
				DeleteObject((HGDIOBJ)hFont);
				ReleaseDC(GetDesktopWindow(), hDC);
			}
		}

		void WINAPI jGetDialogUnitsW(const wstring& _face, uint32_t _fsize, BOOL _italic, uint16_t _weight, uint16_t& x, uint16_t& y)
		{
			if (_face.empty() || !_face.length())
			{
			def_uints:
				LONG l = GetDialogBaseUnits();

				x = LOWORD(l);
				y = HIWORD(l);
			}
			else
			{
				HDC hDC = GetDC(GetDesktopWindow());
				HFONT hFont = CreateFontW(
					-MulDiv(_fsize, GetDeviceCaps(hDC, LOGPIXELSY), 72),
					0, 0, 0, _weight, _italic, 0, 0, DEFAULT_CHARSET,
					0, 0, CLEARTYPE_QUALITY, DEFAULT_PITCH,
					_face.c_str());
				if (!hFont)
				{
					ReleaseDC(GetDesktopWindow(), hDC);
					goto def_uints;
				}

				HFONT hOldFont = (HFONT)SelectObject(hDC, (HGDIOBJ)hFont);

				SIZE s;
				if (!GetTextExtentPoint32W(hDC, L"A", 1, &s))
				{
					SelectObject(hDC, (HGDIOBJ)hOldFont);
					DeleteObject((HGDIOBJ)hFont);
					ReleaseDC(GetDesktopWindow(), hDC);
					goto def_uints;
				}

				x = jAlign(s.cx, 2) >> 1;
				y = jAlign(s.cy, 2) >> 1;

				SelectObject(hDC, (HGDIOBJ)hOldFont);
				DeleteObject((HGDIOBJ)hFont);
				ReleaseDC(GetDesktopWindow(), hDC);
			}
		}

		typedef struct DLGTEMPLATEEX
		{
			uint16_t dlgVer;
			uint16_t signature;
			uint32_t helpID;
			uint32_t exStyle;
			uint32_t style;
			uint16_t cDlgItems;
			int16_t x;
			int16_t y;
			int16_t cx;
			int16_t cy;
		} *LPDLGTEMPLATEEX;

		typedef struct DLGITEMTEMPLATEEX {
			uint32_t helpID;
			uint32_t exStyle;
			uint32_t style;
			int16_t x;
			int16_t y;
			int16_t cx;
			int16_t cy;
			uint32_t id;
		} *LPDLGITEMTEMPLATEEX;

		typedef struct DLGUINTS
		{
			uint16_t x;
			uint16_t y;
		} *LPDLGUINTS;

		void WINAPI jAppendStringQuoteA(lpuint16_t& lpAddr, const string& _text)
		{
			LPWSTR lpwsz = (LPWSTR)lpAddr;
			if (_text.length())
			{
				int32_t nSize = MultiByteToWideChar(CP_ACP, 0, _text.c_str(), -1, NULL, 0);
				if (nSize)
				{
					MultiByteToWideChar(CP_ACP, 0, _text.c_str(), -1, lpwsz, nSize);
					lpAddr = (lpuint16_t)(lpwsz + nSize);
				}
				else
					goto empty_add_str;
			}
			else
			{
			empty_add_str:
				*lpwsz = '\0';
				lpAddr = (lpuint16_t)++lpwsz;
			}
		}

		void WINAPI jAppendStringQuoteW(lpuint16_t& lpAddr, const wstring& _text)
		{
			LPWSTR lpwsz = (LPWSTR)lpAddr;
			if (_text.length())
			{
				wcscpy(lpwsz, _text.c_str());
				lpAddr = (lpuint16_t)(lpwsz + _text.length() + 1);
			}
			else
			{
				*lpwsz = '\0';
				lpAddr = (lpuint16_t)++lpwsz;
			}
		}

		uint32_t WINAPI jGetMemSizeForDialogTemplateA(const string& _title, const string& _classname, const string& _face, const jControls* _cntrs)
		{
			uint32_t u32res = 40 + JDialogStrLengthByteA(_title) + JDialogStrLengthByteA(_classname) + JDialogStrLengthByteA(_face);

			CHAR szBuf[256];
			jCustomControlA* control;
			for each (auto cntr in *_cntrs)
			{
				u32res = jAlign(u32res, 4);
				control = (jCustomControlA*)cntr;

				uint32_t title_len = JDialogStrLengthByteA(control->Title);
				uint32_t class_len = JDialogStrLengthByteA(control->Class);

				strcpy(szBuf, control->Class.c_str());
				_strupr_s(szBuf);
				if (mapSysClassA.find(szBuf) != mapSysClassA.end())
					u32res += 32 + title_len;
				else
					u32res += 30 + title_len + class_len;
			}

			return u32res;
		}

		uint32_t WINAPI jGetMemSizeForDialogTemplateW(const wstring& _title, const wstring& _classname, const wstring& _face, const jControls* _cntrs)
		{
			uint32_t u32res = 40 + JDialogStrLengthByteW(_title) + JDialogStrLengthByteW(_classname) + JDialogStrLengthByteW(_face);

			WCHAR szBuf[256];
			jCustomControlW* control;
			for each (auto cntr in *_cntrs)
			{
				u32res = jAlign(u32res, 4);
				control = (jCustomControlW*)cntr;

				uint32_t title_len = JDialogStrLengthByteW(control->Title);
				uint32_t class_len = JDialogStrLengthByteW(control->Class);

				wcscpy(szBuf, control->Class.c_str());
				_wcsupr_s(szBuf);
				if (mapSysClassW.find(szBuf) != mapSysClassW.end())
					u32res += 32 + title_len;
				else
					u32res += 30 + title_len + class_len;					
			}

			return u32res;
		}

		void WINAPI jInitDialogTemplateA(DLGTEMPLATEEX& _template, DLGUINTS& _units, uint32_t _ex_style, const string& _title, const string& _classname,
			const string& _face, uint16_t _fsize, BOOL _italic, uint16_t _weight, uint32_t _style, int32_t x, int32_t y, int32_t cx, int32_t cy, const jControls* _cntrs)
		{
			_template.dlgVer = 1;
			_template.signature = JDialogChunkResource;
			_template.helpID = 0;
			_template.exStyle = _ex_style;
			_template.style = _style | STYLE_DEFAULT;
			_template.cDlgItems = _cntrs->size();

			// position and size window (convert to dialog units)
			jGetDialogUnitsA(_face, _fsize, _italic, _weight, _units.x, _units.y);
			_template.x = (int16_t)MulDiv(x, 4, _units.x);
			_template.y = (int16_t)MulDiv(y, 8, _units.y);
			_template.cx = (int16_t)MulDiv(cx, 4, _units.x);
			_template.cy = (int16_t)MulDiv(cy, 8, _units.y);

			lpuint16_t lpw = (lpuint16_t)(((lpuint8_t)&_template) + 26);
			// no support menu
			*lpw++ = 0;
			// classname
			jAppendStringQuoteA(lpw, _classname);
			// title
			jAppendStringQuoteA(lpw, _title);
			// font size
			*lpw++ = _fsize;
			// font weight
			*lpw++ = _weight;

			lpuint8_t lpb = (lpuint8_t)lpw;
			// font italic
			*lpb++ = (int8_t)_italic;
			// font charset (set default)
			*lpb++ = (int8_t)DEFAULT_CHARSET;

			lpw = (lpuint16_t)lpb;
			// font name
			jAppendStringQuoteA(lpw, _face);

			CHAR szBuf[256];
			jCustomControlA* control;
			LPDLGITEMTEMPLATEEX _itemtemplate;
			for each (auto cntr in *_cntrs)
			{
				lpw = jAlign(lpw, 4);
				control = (jCustomControlA*)cntr;
				_itemtemplate = (LPDLGITEMTEMPLATEEX)lpw;

				_itemtemplate->helpID = 0;
				_itemtemplate->exStyle = control->ExStyle;
				_itemtemplate->style = control->Style | WS_CHILD;
				_itemtemplate->x = (int16_t)MulDiv(control->X, 4, _units.x);
				_itemtemplate->y = (int16_t)MulDiv(control->Y, 8, _units.y);
				_itemtemplate->cx = (int16_t)MulDiv(control->Width, 4, _units.x);
				_itemtemplate->cy = (int16_t)MulDiv(control->Height, 8, _units.y);
				_itemtemplate->id = (JDialogIncorrectCheckUId(control->UserId) ? JDialogInvalidUId : control->UserId);

				lpw += 12;

				// classname
				strcpy(szBuf, control->Class.c_str());
				_strupr_s(szBuf);
				auto it = mapSysClassA.find(szBuf);
				if (it == mapSysClassA.end())
					jAppendStringQuoteA(lpw, control->Class);
				else
				{
					*lpw++ = JDialogChunkResource;
					*lpw++ = it->second;
				}

				// title
				if ((it != mapSysClassA.end()) && (it->second == 0x0082) &&
					(((_itemtemplate->style & SS_BITMAP) == SS_BITMAP) || ((_itemtemplate->style & SS_ICON) == SS_ICON)))
				{
					*lpw++ = JDialogChunkResource;
					*lpw++ = strtoul(control->Title.c_str(), NULL, 10);
				}
				else
					jAppendStringQuoteA(lpw, control->Title);

				*lpw++ = 0;
			}
		}

		void WINAPI jInitDialogTemplateW(DLGTEMPLATEEX& _template, DLGUINTS& _units, uint32_t _ex_style, const wstring& _title, const wstring& _classname,
			const wstring& _face, uint16_t _fsize, BOOL _italic, uint16_t _weight, uint32_t _style, int32_t x, int32_t y, int32_t cx, int32_t cy, const jControls* _cntrs)
		{
			_template.dlgVer = 1;
			_template.signature = JDialogChunkResource;
			_template.helpID = 0;
			_template.exStyle = _ex_style;
			_template.style = _style | STYLE_DEFAULT;
			_template.cDlgItems = _cntrs->size();

			// position and size window (convert to dialog units)
			jGetDialogUnitsW(_face, _fsize, _italic, _weight, _units.x, _units.y);
			_template.x = (int16_t)MulDiv(x, 4, _units.x);
			_template.y = (int16_t)MulDiv(y, 8, _units.y);
			_template.cx = (int16_t)MulDiv(cx, 4, _units.x);
			_template.cy = (int16_t)MulDiv(cy, 8, _units.y);

			lpuint16_t lpw = (lpuint16_t)(((lpuint8_t)&_template) + 26);
			// no support menu
			*lpw++ = 0;
			// classname
			jAppendStringQuoteW(lpw, _classname);
			// title
			jAppendStringQuoteW(lpw, _title);
			// font size
			*lpw++ = _fsize;
			// font weight
			*lpw++ = _weight;

			lpuint8_t lpb = (lpuint8_t)lpw;
			// font italic
			*lpb++ = (int8_t)_italic;
			// font charset (set default)
			*lpb++ = (int8_t)DEFAULT_CHARSET;

			lpw = (lpuint16_t)lpb;
			// font name
			jAppendStringQuoteW(lpw, _face);

			WCHAR szBuf[256];
			jCustomControlW* control;
			LPDLGITEMTEMPLATEEX _itemtemplate;
			for each (auto cntr in *_cntrs)
			{
				lpw = jAlign(lpw, 4);
				control = (jCustomControlW*)cntr;
				_itemtemplate = (LPDLGITEMTEMPLATEEX)lpw;

				_itemtemplate->helpID = 0;
				_itemtemplate->exStyle = control->ExStyle;
				_itemtemplate->style = control->Style | WS_CHILD;
				_itemtemplate->x = (int16_t)MulDiv(control->X, 4, _units.x);
				_itemtemplate->y = (int16_t)MulDiv(control->Y, 8, _units.y);
				_itemtemplate->cx = (int16_t)MulDiv(control->Width, 4, _units.x);
				_itemtemplate->cy = (int16_t)MulDiv(control->Height, 8, _units.y);
				_itemtemplate->id = (JDialogIncorrectCheckUId(control->UserId) ? JDialogInvalidUId : control->UserId);

				lpw += 12;

				// classname
				wcscpy(szBuf, control->Class.c_str());
				_wcsupr_s(szBuf);
				auto it = mapSysClassW.find(szBuf);
				if (it == mapSysClassW.end())
					jAppendStringQuoteW(lpw, control->Class);
				else
				{
					*lpw++ = JDialogChunkResource;
					*lpw++ = it->second;
				}

				// title
				if ((it != mapSysClassW.end()) && (it->second == 0x0082) &&
					(((_itemtemplate->style & SS_BITMAP) == SS_BITMAP) || ((_itemtemplate->style & SS_ICON) == SS_ICON)))
				{
					*lpw++ = JDialogChunkResource;
					*lpw++ = wcstoul(control->Title.c_str(), NULL, 10);
				}
				else
					jAppendStringQuoteW(lpw, control->Title);

				*lpw++ = 0;
			}
		}

		template<typename T>
		void WINAPI jGetValueFromJSON(json* _j, T& _v, const string& _fieldname, const T _defv)
		{
			if (_j->contains(_fieldname))
				_j->at(_fieldname).get_to(_v);
			else
				_v = _defv;
		}

		template<typename T>
		void WINAPI jGetStyleFromJSON(json* _j, T& _v, const string& _fieldname, const T _defv)
		{
			if (_j->contains(_fieldname))
			{
				_v = 0;

				auto j_array = _j->at(_fieldname);
				if (!j_array.is_array())
				{
					j_array.get_to(_v);
					return;
				}

				_v |= _defv;

				for (auto& element : j_array)
				{
					if (element.is_string())
					{
						string str;
						element.get_to(str);

						if (!str.find_first_of("NOT ") || !str.find_first_of("not "))
						{
							_v &= ~jGetStyleFromString(str.substr(4));
						}
						else
							_v |= jGetStyleFromString(str);
					}
					else if(element.is_number_integer())
					{
						int32_t integer;
						_v |= element.get_to(integer);
					}
					else if (element.is_number_unsigned())
					{
						uint32_t _unsigned;
						_v |= element.get_to(_unsigned);
					}
				}
			}
			else
				_v = _defv;
		}

		wstring WINAPI jUtf8ToWide(const string& _t)
		{
			int32_t nSize = MultiByteToWideChar(CP_UTF8, 0, _t.c_str(), -1, NULL, 0);
			if (nSize > 0)
			{
				WCHAR* szBuf = new WCHAR[nSize + 1];

				MultiByteToWideChar(CP_UTF8, 0, _t.c_str(), -1, szBuf, nSize);
				szBuf[nSize] = L'\0';
				wstring ws = szBuf;

				delete[] szBuf;
				return ws;
			}

			return L"";
		}

#ifndef JDIALOG_NO_FUNCTIONS_CREATE_CONTROLS

		//////////////////////////////////////
		//
		// creates function
		//
		//////////////////////////////////////

		bool WINAPI CreateControlA(uint32_t _ex_style, const string& _title, const string& _classname, uint32_t _style,
			int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			if (!_dialog)
				return false;

			jCustomControlA* control = new jCustomControlA(_dialog);
			if (!control)
				return false;

			control->ExStyle = _ex_style;
			control->Title = _title;
			control->Class = _classname;
			control->Style = _style;
			control->X = _x;
			control->Y = _y;
			control->Width = _cx;
			control->Height = _cy;
			control->UserId = _uid;

			bool bRes = _dialog->AppendControl(control);
			if (!bRes)
				delete control;

			return bRes;
		}

		bool WINAPI CreateControlW(uint32_t _ex_style, const wstring& _title, const wstring& _classname, uint32_t _style,
			int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			if (!_dialog)
				return false;

			jCustomControlW* control = new jCustomControlW(_dialog);
			if (!control)
				return false;

			control->ExStyle = _ex_style;
			control->Title = _title;
			control->Class = _classname;
			control->Style = _style;
			control->X = _x;
			control->Y = _y;
			control->Width = _cx;
			control->Height = _cy;
			control->UserId = _uid;

			bool bRes = _dialog->AppendControl(control);
			if (!bRes)
				delete control;

			return bRes;
		}

		bool WINAPI CreateTextA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA(_title, "STATIC");
		}

		bool WINAPI CreateTextW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(_title, L"STATIC");
		}

		bool WINAPI CreateLTextA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateTextA(_ex_style, _title, _style/* | SS_LEFT*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateLTextW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateTextW(_ex_style, _title, _style/* | SS_LEFT*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateCTextA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateTextA(_ex_style, _title, _style/* | SS_CENTER*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateCTextW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateTextW(_ex_style, _title, _style/* | SS_CENTER*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateRTextA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateTextA(_ex_style, _title, _style/* | SS_RIGHT*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateRTextW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateTextW(_ex_style, _title, _style/* | SS_RIGHT*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateEditTextA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA(_title, "EDIT");
		}

		bool WINAPI CreateEditTextW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(_title, L"EDIT");
		}

		bool WINAPI CreateDefPushButtonA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA(_title, "BUTTON");
		}

		bool WINAPI CreateDefPushButtonW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(_title, L"BUTTON");
		}

		bool WINAPI CreateListBoxA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "LISTBOX");
		}

		bool WINAPI CreateListBoxW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"LISTBOX");
		}

		bool WINAPI CreateComboBoxA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "COMBOBOX");
		}

		bool WINAPI CreateComboBoxW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"COMBOBOX");
		}

		bool WINAPI CreateAutoRadioButtonA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateDefPushButtonA(_ex_style, _title, _style/* | BS_RADIOBUTTON*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateAutoRadioButtonW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateDefPushButtonW(_ex_style, _title, _style/* | BS_RADIOBUTTON*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateAutoCheckButtonA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateDefPushButtonA(_ex_style, _title, _style/* | BS_CHECKBOX*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateAutoCheckButtonW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateDefPushButtonW(_ex_style, _title, _style/* | BS_CHECKBOX*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateGroupBoxA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateDefPushButtonA(_ex_style, _title, _style/* | BS_GROUPBOX*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateGroupBoxW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateDefPushButtonW(_ex_style, _title, _style/* | BS_GROUPBOX*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateListViewA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "SysListView32");
		}

		bool WINAPI CreateListViewW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"SysListView32");
		}

		bool WINAPI CreateTreeViewA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y, 
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "SysTreeView32");
		}

		bool WINAPI CreateTreeViewW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"SysTreeView32");
		}

		bool WINAPI CreateTabViewA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "SysTabControl32");
		}

		bool WINAPI CreateTabViewW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"SysTabControl32");
		}

		bool WINAPI CreateBitmapA(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			CHAR szBuf[64] = { 0 };
			return CreateTextA(_ex_style, ultoa(_resource, szBuf, 10), _style/* | SS_BITMAP*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateBitmapW(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			WCHAR szBuf[64] = { 0 };
			return CreateTextW(_ex_style, _ultow(_resource, szBuf, 10), _style/* | SS_BITMAP*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateIconA(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			CHAR szBuf[64] = { 0 };
			return CreateTextA(_ex_style, ultoa(_resource, szBuf, 10), _style/* | SS_ICON*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateIconW(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			WCHAR szBuf[64] = { 0 };
			return CreateTextW(_ex_style, _ultow(_resource, szBuf, 10), _style/* | SS_ICON*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateBitmapExA(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateBitmapA(_ex_style, _resource, _style/* | SS_CENTERIMAGE*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateBitmapExW(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateBitmapW(_ex_style, _resource, _style/* | SS_CENTERIMAGE*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateIconExA(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateIconA(_ex_style, _resource, _style/* | SS_CENTERIMAGE*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateIconExW(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateIconW(_ex_style, _resource, _style/* | SS_CENTERIMAGE*/, _x, _y, _cx, _cy, _uid, _dialog);
		}

		bool WINAPI CreateScrollBarA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "SCROLLBAR");
		}

		bool WINAPI CreateScrollBarW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"SCROLLBAR");
		}

		bool WINAPI CreateReBarA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "ReBarWindow32");
		}

		bool WINAPI CreateReBarW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"ReBarWindow32");
		}

		bool WINAPI CreateHotKeyA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "msctls_hotkey32");
		}

		bool WINAPI CreateHotKeyW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"msctls_hotkey32");
		}

		bool WINAPI CreateIPAdressA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "SysIPAddress32");
		}

		bool WINAPI CreateIPAdressW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"SysIPAddress32");
		}

		bool WINAPI CreateMonthCalA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "SysMonthCal32");
		}

		bool WINAPI CreateMonthCalW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"SysMonthCal32");
		}

		bool WINAPI CreateStatusBarA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "msctls_statusbar32");
		}

		bool WINAPI CreateStatusBarW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"msctls_statusbar32");
		}

		bool WINAPI CreateProgressBarA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "msctls_progress32");
		}

		bool WINAPI CreateProgressBarW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"msctls_progress32");
		}

		bool WINAPI CreateAnimateA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "SysAnimate32");
		}

		bool WINAPI CreateAnimateW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"SysAnimate32");
		}

		bool WINAPI CreateHeaderA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "SysHeader32");
		}

		bool WINAPI CreateHeaderW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"SysHeader32");
		}

		bool WINAPI CreateRichEditA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "RichEdit20A");
		}

		bool WINAPI CreateRichEditW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"RichEdit20W");
		}

		bool WINAPI CreateDateTimePickerA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return JDialogCreateCommControlA("", "SysDateTimePick32");
		}

		bool WINAPI CreateDateTimePickerW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return JDialogCreateCommControlW(L"", L"SysDateTimePick32");
		}

#endif // !JDIALOG_NO_FUNCTIONS_CREATE_CONTROLS

		//////////////////////////////////////
		//
		// class jComponent
		//
		//////////////////////////////////////

		jComponent::jComponent(void) : m_ex_style(0), m_style(0), m_x(0), m_y(0), m_cx(0), m_cy(0)
		{}

		jComponent::jComponent(const jComponent& _component) : m_ex_style(_component.m_ex_style), m_style(_component.m_style),
			m_x(_component.m_x), m_y(_component.m_y), m_cx(_component.m_cx), m_cy(_component.m_cy)
		{}

		jComponent::~jComponent(void)
		{}

		//////////////////////////////////////
		//
		// class jCustomControl
		//
		//////////////////////////////////////

		void jCustomControl::doChanged(void)
		{
			if (m_lpDialog)
				m_lpDialog->HappenUpdate();
		}

		jCustomDialog* jCustomControl::GetDialog(void) const
		{
			return m_lpDialog;
		}

		bool jCustomControl::_lowlevel_LoadFromJSONLayout(LPARAM lParam)
		{
			json* jData = (json*)lParam;

			if (!jData->size())
				return false;

			string stype;
			jGetValueFromJSON<string>(jData, stype, "Type", "CONTROL");

			CHAR szBuf[64] = { 0 };
			strcpy_s(szBuf, stype.c_str());
			_strupr_s(szBuf);
			if (auto it = mapStdControlType.find(szBuf); it != mapStdControlType.end())
				jGetStyleFromJSON<uint32_t>(jData, m_style, "Style", it->second);
			else
				jGetStyleFromJSON<uint32_t>(jData, m_style, "Style", JDialogDefaultControlStyle);
				
			jGetStyleFromJSON<uint32_t>(jData, m_ex_style, "ExStyle", 0);
			jGetValueFromJSON<int32_t>(jData, m_x, "x", 0);
			jGetValueFromJSON<int32_t>(jData, m_y, "y", 0);
			jGetValueFromJSON<int32_t>(jData, m_cx, "Width", 100);
			jGetValueFromJSON<int32_t>(jData, m_cy, "Height", 100);
			jGetValueFromJSON<uint32_t>(jData, m_uid, "Id", JDialogInvalidUId);

			if (JDialogIncorrectCheckUId(m_uid))
				m_uid = JDialogInvalidUId;

			return true;
		}

		jCustomControl::jCustomControl(jCustomDialog* _dialog) : m_lpDialog(_dialog)
		{}

		jCustomControl::jCustomControl(const jCustomControl& _control) : m_lpDialog(_control.m_lpDialog)
		{}

		//////////////////////////////////////
		//
		// class jCustomControlA
		//
		//////////////////////////////////////

		bool jCustomControlA::ParseJSONObject(const LPVOID _data)
		{
			if (!_lowlevel_LoadFromJSONLayout((LPARAM)_data))
				return false;

			json* jData = (json*)_data;

			jGetValueFromJSON<string>(jData, m_title, "Title", "");

			string stype;
			jGetValueFromJSON<string>(jData, stype, "Type", "CONTROL");

			CHAR szBuf[64] = { 0 };
			strcpy_s(szBuf, stype.c_str());
			_strupr_s(szBuf);
			if (auto it = mapStdControlType2A.find(szBuf); it != mapStdControlType2A.end())
				m_classname = it->second;
			else
				jGetValueFromJSON<string>(jData, m_classname, "ClassName", "");

			return true;
		}

		jCustomControlA::jCustomControlA(jCustomDialog* _dialog) : jCustomControl(_dialog)
		{}

		jCustomControlA::jCustomControlA(const jCustomControlA& _control) : jCustomControl(_control)
		{}

		//////////////////////////////////////
		//
		// class jCustomControlW
		//
		//////////////////////////////////////

		bool jCustomControlW::ParseJSONObject(const LPVOID _data)
		{
			if (!_lowlevel_LoadFromJSONLayout((LPARAM)_data))
				return false;

			json* jData = (json*)_data;
			string tmp;

			jGetValueFromJSON<string>(jData, tmp, "Title", "");
			m_title = jUtf8ToWide(tmp);

			string stype;
			jGetValueFromJSON<string>(jData, stype, "Type", "CONTROL");

			CHAR szBuf[64] = { 0 };
			strcpy_s(szBuf, stype.c_str());
			_strupr_s(szBuf);
			if (auto it = mapStdControlType2W.find(szBuf); it != mapStdControlType2W.end())
				m_classname = it->second;
			else
			{
				jGetValueFromJSON<string>(jData, tmp, "ClassName", "");
				m_classname = jUtf8ToWide(tmp);
			}

			return true;
		}

		jCustomControlW::jCustomControlW(jCustomDialog* _dialog) : jCustomControl(_dialog)
		{}

		jCustomControlW::jCustomControlW(const jCustomControlW& _control) : jCustomControl(_control)
		{}

		//////////////////////////////////////
		//
		// class jCustomDialog
		//
		//////////////////////////////////////

		bool jCustomDialog::GenerateBinary(void)
		{
			if (!IsNeedGenerate())
				return false;

			Release();
			m_NeedGenerate = !DoGenerateBinary();
			return !m_NeedGenerate;
		}

		bool jCustomDialog::AppendControl(jCustomControl* _control)
		{
			if (!_control)
				return false;

			m_items.push_back(_control);
			doChanged();

			return true;
		}

		void jCustomDialog::Release(void)
		{
			if (m_lpData)
			{
				jMemoryManager::Free(m_lpData);
				m_lpData = NULL;
				m_nSize = 0;
				m_NeedGenerate = true;
			}
		}

		bool jCustomDialog::SaveToFile(const string& _fname)
		{
			if (IsNeedGenerate() && !GenerateBinary())
				return false;

			ofstream ofs(_fname, ios::binary | ios::out);
			ofs.write((LPSTR)m_lpData, m_nSize);
			ofs.close();

			return true;
		}

		bool jCustomDialog::_lowlevel_LoadFromJSONLayout(LPARAM lParam)
		{
			json* jData = (json*)lParam;

			if (!jData->size())
				return false;

			jGetStyleFromJSON<uint32_t>(jData, m_ex_style, "ExStyle", 0);
			jGetStyleFromJSON<uint32_t>(jData, m_style, "Style", 0);
			jGetValueFromJSON<uint16_t>(jData, m_fsize, "FontSize", 8);
			jGetValueFromJSON<uint16_t>(jData, m_weight, "FontWeight", FW_NORMAL);
			jGetValueFromJSON<int32_t>(jData, m_x, "x", 0);
			jGetValueFromJSON<int32_t>(jData, m_y, "y", 0);
			jGetValueFromJSON<int32_t>(jData, m_cx, "Width", 100);
			jGetValueFromJSON<int32_t>(jData, m_cy, "Height", 100);

			uint8_t n8temp = 0;
			jGetValueFromJSON<uint8_t>(jData, n8temp, "FontItalic", false);
			m_italic = n8temp > 0;

			return true;
		}

		jCustomDialog::jCustomDialog(void) : jComponent(), m_fsize(0), m_italic(false),
			m_weight(FW_NORMAL), m_NeedGenerate(true), m_lpData(NULL), m_nSize(0)
		{
//#ifndef JDIALOG_NO_MANIFEST_LINKER_COMMCTRL
			if (!g_comm_init)
			{
				INITCOMMONCONTROLSEX InitCtrls;
				InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
				InitCtrls.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES | ICC_TAB_CLASSES |
					ICC_UPDOWN_CLASS | ICC_PROGRESS_CLASS | ICC_HOTKEY_CLASS | ICC_ANIMATE_CLASS | ICC_DATE_CLASSES |
					ICC_USEREX_CLASSES | ICC_COOL_CLASSES | ICC_PAGESCROLLER_CLASS | ICC_NATIVEFNTCTL_CLASS |
					ICC_WIN95_CLASSES | ICC_STANDARD_CLASSES | ICC_LINK_CLASS;
				g_comm_init = InitCommonControlsEx(&InitCtrls);
			}
//#endif // !JDIALOG_NO_MANIFEST_LINKER_COMMCTRL
		}

		jCustomDialog::jCustomDialog(const jCustomDialog& _dialog) : jComponent(_dialog), m_fsize(_dialog.m_fsize),
			m_italic(_dialog.m_italic), m_weight(_dialog.m_weight), m_NeedGenerate(true), m_lpData(NULL), m_nSize(0)
		{}

		jCustomDialog::~jCustomDialog(void)
		{
			Release();
		}

		//////////////////////////////////////
		//
		// class jDialogA
		//
		//////////////////////////////////////

		bool jDialogA::LoadFromJSONLayout(LPARAM lParam)
		{
			if (!_lowlevel_LoadFromJSONLayout(lParam))
				return false;

			json* jData = (json*)lParam;

			jGetValueFromJSON<string>(jData, m_title, "Title", "Dialog");
			jGetValueFromJSON<string>(jData, m_classname, "ClassName", "");
			jGetValueFromJSON<string>(jData, m_face, "FontName", "MS Sans Serif");

			if (jData->contains("Controls"))
			{
				auto j_array = jData->at("Controls");
				if (j_array.is_array())
				{
					for (auto& elem : j_array)
					{
						jCustomControlA* control = new jCustomControlA(this);
						if (!control)
							return false;

						control->ParseJSONObject(&elem);
						bool bRes = AppendControl(control);
						if (!bRes)
						{
							delete control;
							return false;
						}
					}
				}
			}

			return true;
		}

		HWND jDialogA::Show(HWND _parent, DLGPROC _dlgproc, LPARAM _initparam, HINSTANCE hInst)
		{
			if (IsNeedGenerate() && !GenerateBinary())
				return NULL;

			return CreateDialogIndirectParamA(hInst, (LPCDLGTEMPLATEA)m_lpData, _parent, _dlgproc, _initparam);
		}

		INT_PTR jDialogA::ShowModal(HWND _parent, DLGPROC _dlgproc, LPARAM _initparam, HINSTANCE hInst)
		{
			if (IsNeedGenerate() && !GenerateBinary())
				return S_FALSE;

			return DialogBoxIndirectParamA(hInst, (LPCDLGTEMPLATEA)m_lpData, _parent, _dlgproc, _initparam);
		}

		bool jDialogA::DoGenerateBinary(void)
		{
			uint32_t nSize = jGetMemSizeForDialogTemplateA(m_title, m_classname, m_face, &m_items);

			LPVOID lpData = jMemoryManager::Alloc(nSize);
			if (lpData)
			{
				DLGUINTS dlgUnits = { 0 };
				jInitDialogTemplateA(*((LPDLGTEMPLATEEX)lpData), dlgUnits, m_ex_style, m_title, m_classname, m_face, m_fsize,
					m_italic, m_weight, m_style, m_x, m_y, m_cx, m_cy, &m_items);

				m_lpData = lpData;
				m_nSize = nSize;

				return true;
			}

			return false;
		}

		bool jDialogA::ParseJSON(const string& _data)
		{
			json j = json::parse(_data);
			return LoadFromJSONLayout((LPARAM)&j);
		}

		bool jDialogA::LoadFromFile(const string& _fname)
		{
			if (fs::exists(_fname))
			{
				// read a JSON file
				ifstream ios(_fname);
				json j;
				ios >> j;
				ios.close();

				return LoadFromJSONLayout((LPARAM)&j);
			}

			return false;
		}

		jDialogA jDialogA::FromFile(const string& _fname)
		{
			jDialogA dialog;
			dialog.LoadFromFile(_fname);
			return dialog;
		}

		jDialogA::jDialogA(void) : jCustomDialog(), m_title("Dialog"), m_classname(""), m_face("MS Sans Serif")
		{}

		jDialogA::jDialogA(const string& _json) : jDialogA()
		{
			ParseJSON(_json);
		}

		jDialogA::jDialogA(const jDialogA& _dialog) : jCustomDialog(_dialog), m_title(_dialog.m_title),
			m_classname(_dialog.m_classname), m_face(_dialog.m_face)
		{
			jCustomControlA* dublicate;
			for each (auto component in _dialog.m_items)
			{
				dublicate = new jCustomControlA(*(jCustomControlA*)component);
				if (dublicate)
					m_items.push_back(dublicate);
			}
		}

		jDialogA::~jDialogA(void)
		{
			for each (auto component in m_items)
				delete (jCustomControlA*)component;
			m_items.clear();
		}

		//////////////////////////////////////
		//
		// class jDialogW
		//
		//////////////////////////////////////

		bool jDialogW::LoadFromJSONLayout(LPARAM lParam)
		{
			if (!_lowlevel_LoadFromJSONLayout(lParam))
				return false;

			json* jData = (json*)lParam;
			string tmp;

			string stype;
			jGetValueFromJSON<string>(jData, stype, "Type", "CONTROL");

			CHAR szBuf[64] = { 0 };
			strcpy_s(szBuf, stype.c_str());
			_strupr_s(szBuf);
			if (auto it = mapStdControlType.find(szBuf); it != mapStdControlType.end())
				jGetStyleFromJSON<uint32_t>(jData, m_style, "Style", it->second);
			else
				jGetStyleFromJSON<uint32_t>(jData, m_style, "Style", JDialogDefaultControlStyle);

			jGetValueFromJSON<string>(jData, tmp, "Title", "Dialog");
			m_title = jUtf8ToWide(tmp);
			jGetValueFromJSON<string>(jData, tmp, "ClassName", "");
			m_classname = jUtf8ToWide(tmp);
			jGetValueFromJSON<string>(jData, tmp, "FontName", "MS Sans Serif");
			m_face = jUtf8ToWide(tmp);

			if (jData->contains("Controls"))
			{
				auto j_array = jData->at("Controls");
				if (j_array.is_array())
				{
					for (auto& elem : j_array)
					{
						jCustomControlW* control = new jCustomControlW(this);
						if (!control)
							return false;

						control->ParseJSONObject(&elem);
						bool bRes = AppendControl(control);
						if (!bRes)
						{
							delete control;
							return false;
						}
					}
				}
			}

			return true;
		}

		HWND jDialogW::Show(HWND _parent, DLGPROC _dlgproc, LPARAM _initparam, HINSTANCE hInst)
		{
			if (IsNeedGenerate() && !GenerateBinary())
				return NULL;

			return CreateDialogIndirectParamW(hInst, (LPCDLGTEMPLATEW)m_lpData, _parent, _dlgproc, _initparam);
		}

		INT_PTR jDialogW::ShowModal(HWND _parent, DLGPROC _dlgproc, LPARAM _initparam, HINSTANCE hInst)
		{
			if (IsNeedGenerate() && !GenerateBinary())
				return S_FALSE;

			return DialogBoxIndirectParamW(hInst, (LPCDLGTEMPLATEW)m_lpData, _parent, _dlgproc, _initparam);
		}

		bool jDialogW::DoGenerateBinary(void)
		{
			uint32_t nSize = jGetMemSizeForDialogTemplateW(m_title, m_classname, m_face, &m_items);

			LPVOID lpData = jMemoryManager::Alloc(nSize);
			if (lpData)
			{
				DLGUINTS dlgUnits = { 0 };
				jInitDialogTemplateW(*((LPDLGTEMPLATEEX)lpData), dlgUnits, m_ex_style, m_title, m_classname, m_face, m_fsize,
					m_italic, m_weight, m_style, m_x, m_y, m_cx, m_cy, &m_items);

				m_lpData = lpData;
				m_nSize = nSize;

				return true;
			}

			return false;
		}

		bool jDialogW::ParseJSON(const string& _data)
		{
			json j = json::parse(_data);
			return LoadFromJSONLayout((LPARAM)&j);
		}

		bool jDialogW::LoadFromFile(const wstring& _fname)
		{
			if (fs::exists(_fname))
			{
				// read a JSON file
				ifstream ios(_fname);
				json j;
				ios >> j;
				ios.close();

				return LoadFromJSONLayout((LPARAM)&j);
			}

			return false;
		}

		jDialogW jDialogW::FromFile(const wstring& _fname)
		{
			jDialogW dialog;
			dialog.LoadFromFile(_fname);
			return dialog;
		}

		jDialogW::jDialogW(void) : jCustomDialog(), m_title(L"Dialog"), m_classname(L""), m_face(L"MS Sans Serif")
		{}

		jDialogW::jDialogW(const string& _json) : jDialogW()
		{
			ParseJSON(_json);
		}

		jDialogW::jDialogW(const jDialogW& _dialog) : jCustomDialog(_dialog), m_title(_dialog.m_title),
			m_classname(_dialog.m_classname), m_face(_dialog.m_face)
		{
			jCustomControlW* dublicate;
			for each (auto component in _dialog.m_items)
			{
				dublicate = new jCustomControlW(*(jCustomControlW*)component);
				if (dublicate)
					m_items.push_back(dublicate);
			}
		}

		jDialogW::~jDialogW(void)
		{
			for each (auto component in m_items)
				delete (jCustomControlW*)component;
			m_items.clear();
		}
	}
}
