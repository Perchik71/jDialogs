#pragma once

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

// System Includes
#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#define NOATOM
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NORASTEROPS
#define NOSCROLL
#define NOSOUND
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOCRYPT
#define NOMCX
#include <Windows.h>
#endif

// set define JDIALOG_NO_MANIFEST_LINKER_COMMCTRL if you don't need to link
// the manifest file for visual themes

// set define JDIALOG_4x4_DIALOG_UINT 
// Microsoft deceives me in its reference book that the width is multiplied by 8, and the height is multiplied by 4.

#ifdef JDIALOG_4x4_DIALOG_UINT
#define JDIALOG_4x4_DIALOG_UINT_SET
#endif

#include <stdint.h>
#include <string>
#include <vector>

#define JDialogPropertyGetter(getf) __declspec(property(get = getf))
#define JDialogProperty(getf, putf) __declspec(property(get = getf, put = putf))

#define JDialogChunkResource			(0xFFFF)
#define JDialogInvalidUId				(0xFFFFFFFF)
#define JDialogIncorrectCheckUId(uid)	(uid >= JDialogInvalidUId)

#define JDialogDefaultControlStyle			(WS_VISIBLE | WS_GROUP)
#define JDialogDefaultTextStyle				(JDialogDefaultControlStyle | SS_NOPREFIX)
#define JDialogDefaultLeftTextStyle			(JDialogDefaultControlStyle | SS_NOPREFIX | SS_LEFT)
#define JDialogDefaultCenterTextStyle		(JDialogDefaultControlStyle | SS_NOPREFIX | SS_CENTER)
#define JDialogDefaultRightTextStyle		(JDialogDefaultControlStyle | SS_NOPREFIX | SS_RIGHT)
#define JDialogDefaultEditTextStyle			(JDialogDefaultControlStyle | WS_BORDER | WS_TABSTOP)
#define JDialogDefaultPushButtonStyle		(JDialogDefaultControlStyle | BS_PUSHBUTTON)
#define JDialogDefaultDefPushButtonStyle	(JDialogDefaultControlStyle | WS_TABSTOP | BS_DEFPUSHBUTTON)
#define JDialogDefaultListBoxStyle			(JDialogDefaultControlStyle | WS_BORDER | WS_TABSTOP)
#define JDialogDefaultComboBoxStyle			(JDialogDefaultControlStyle | WS_TABSTOP | CBS_DROPDOWNLIST)
#define JDialogDefaultComboBoxExStyle		(JDialogDefaultControlStyle | WS_TABSTOP | CBS_DROPDOWN)
#define JDialogDefaultAutoRadioButtonStyle	(JDialogDefaultControlStyle | WS_TABSTOP | BS_RADIOBUTTON)
#define JDialogDefaultAutoCheckButtonStyle	(JDialogDefaultControlStyle | WS_TABSTOP | BS_CHECKBOX)
#define JDialogDefaultGroupBoxStyle			(JDialogDefaultControlStyle | BS_GROUPBOX)
#define JDialogDefaultBitmapStyle			(JDialogDefaultControlStyle | SS_BITMAP)
#define JDialogDefaultIconStyle				(JDialogDefaultControlStyle | SS_ICON)
#define JDialogDefaultBitmapExStyle			(JDialogDefaultBitmapStyle | SS_CENTERIMAGE)
#define JDialogDefaultIconExStyle			(JDialogDefaultIconStyle | SS_CENTERIMAGE)
#define JDialogDefaultListViewStyle			(JDialogDefaultControlStyle | WS_TABSTOP)
#define JDialogDefaultTreeViewStyle			(JDialogDefaultControlStyle | WS_TABSTOP)
#define JDialogDefaultTabViewStyle			(JDialogDefaultControlStyle)
#define JDialogDefaultIPAdressStyle			(JDialogDefaultControlStyle | WS_TABSTOP)
#define JDialogDefaultMonthCalStyle			(JDialogDefaultControlStyle | WS_TABSTOP)
#define JDialogDefaultStatusBarStyle		(JDialogDefaultControlStyle)
#define JDialogDefaultProgressBarStyle		(JDialogDefaultControlStyle | WS_BORDER)
#define JDialogDefaultAnimateStyle			(JDialogDefaultControlStyle)
#define JDialogDefaultHeaderStyle			(JDialogDefaultControlStyle)
#define JDialogDefaultRichEditStyle			(JDialogDefaultEditTextStyle)
#define JDialogDefaultDateTimePickerStyle	(JDialogDefaultControlStyle | WS_TABSTOP)
#define JDialogDefaultReBarStyle			(JDialogDefaultControlStyle)
#define JDialogDefaultHotKeyStyle			(JDialogDefaultControlStyle)
#define JDialogDefaultScrollBarStyle		(JDialogDefaultControlStyle)

namespace perchik71
{
	namespace jDialogs
	{
		using namespace std;

		class jComponent
		{
		protected:
			uint32_t m_ex_style;
			uint32_t m_style;
			int32_t m_x;
			int32_t m_y;
			int32_t m_cx;
			int32_t m_cy;
		protected:
			virtual void doChanged(void) = 0;
		public:
			inline uint32_t GetExStyle(void) const { return m_ex_style; }
			inline uint32_t GetStyle(void)	const { return m_style; }
			inline int32_t GetX(void) const { return m_x; }
			inline int32_t GetY(void) const { return m_y; }
			inline int32_t GetWidth(void) const { return m_cx; }
			inline int32_t GetHeight(void) const { return m_cy; }
			inline void SetExStyle(uint32_t _value) { m_ex_style = _value; doChanged(); }
			inline void SetStyle(uint32_t _value) { m_style = _value; doChanged(); }
			inline void SetX(int32_t _value) { m_x = _value; doChanged(); }
			inline void SetY(int32_t _value) { m_y = _value; doChanged(); }
			inline void SetWidth(int32_t _value) { m_cx = _value; doChanged(); }
			inline void SetHeight(int32_t _value) { m_cy = _value; doChanged(); }
		public:
			JDialogProperty(GetExStyle, SetExStyle) uint32_t ExStyle;
			JDialogProperty(GetStyle, SetStyle) uint32_t Style;
			JDialogProperty(GetX, SetX) int32_t X;
			JDialogProperty(GetY, SetY) int32_t Y;
			JDialogProperty(GetWidth, SetWidth) int32_t Width;
			JDialogProperty(GetHeight, SetHeight) int32_t Height;
		public:
			jComponent(void);
			jComponent(const jComponent& _component);
			virtual ~jComponent(void);
		};

		class jCustomDialog;
		class jCustomControl : public jComponent
		{
		protected:
			jCustomDialog* m_lpDialog;
			uint32_t m_uid;
		protected:
			void doChanged(void);
			virtual bool _lowlevel_LoadFromJSONLayout(LPARAM lParam);
		public:
			jCustomDialog* GetDialog(void) const;
		public:
			inline uint32_t GetUserId(void) const { return m_uid; }
			inline void SetUserId(uint32_t _value) { m_uid = (JDialogIncorrectCheckUId(_value) ? JDialogInvalidUId : _value); doChanged(); }
		public:
			jCustomControl(jCustomDialog* _dialog);
			jCustomControl(const jCustomControl& _control);
		public:
			JDialogPropertyGetter(GetDialog) jCustomDialog* Dialog;
			JDialogProperty(GetUserId, SetUserId) uint32_t UserId;
		};

		class jCustomControlA : public jCustomControl
		{
		private:
			string m_title;
			string m_classname;
		public:
			inline string GetTitle(void) const { return m_title; }
			inline string GetClass(void) const { return m_classname; }
			inline void SetTitle(const string& _value) { m_title = _value; doChanged(); }
			inline void SetClass(const string& _value) { m_classname = _value; doChanged(); }
		public:
			bool ParseJSONObject(const LPVOID _data);
		public:
			jCustomControlA(jCustomDialog* _dialog);
			jCustomControlA(const jCustomControlA& _control);
		public:
			JDialogProperty(GetTitle, SetTitle) string Title;
			JDialogProperty(GetClass, SetClass) string Class;
		};

		class jCustomControlW : public jCustomControl
		{
		private:
			wstring m_title;
			wstring m_classname;
		public:
			inline wstring GetTitle(void) const { return m_title; }
			inline wstring GetClass(void) const { return m_classname; }
			inline void SetTitle(const wstring& _value) { m_title = _value; doChanged(); }
			inline void SetClass(const wstring& _value) { m_classname = _value; doChanged(); }
		public:
			bool ParseJSONObject(const LPVOID _data);
		public:
			jCustomControlW(jCustomDialog* _dialog);
			jCustomControlW(const jCustomControlW& _control);
		public:
			JDialogProperty(GetTitle, SetTitle) wstring Title;
			JDialogProperty(GetClass, SetClass) wstring Class;
		};

		typedef vector<jCustomControl*> jControls;

		class jCustomDialog : public jComponent
		{
		protected:
			uint32_t m_nSize;
			LPVOID m_lpData;
		protected:
			bool m_NeedGenerate;
			uint16_t m_fsize;
			bool m_italic;
			uint16_t m_weight;
			jControls m_items;
		protected:
			inline void doChanged(void) { m_NeedGenerate = true; }
			virtual bool DoGenerateBinary(void) = 0;
			virtual bool _lowlevel_LoadFromJSONLayout(LPARAM lParam);
		public:
			inline uint16_t GetFontSize(void) const { return m_fsize; }
			inline bool GetItalic(void)	const { return m_italic; }
			inline uint16_t GetWeight(void)	const { return m_weight; }
			inline void SetFontSize(uint16_t _value) { m_fsize = _value; doChanged(); }
			inline void SetItalic(bool _value) { m_italic = _value; doChanged(); }
			inline void SetWeight(uint16_t _value) { m_weight = _value;	doChanged(); }
		public:
			inline bool IsNeedGenerate(void) const { return m_NeedGenerate; }
			inline void HappenUpdate(void) { doChanged(); }
			bool AppendControl(jCustomControl* _control);
		public:
			bool GenerateBinary(void);
			// Free binary data
			void Release(void);
			// Save binary data to file
			bool SaveToFile(const string& _fname);
		public:
			virtual HWND Show(HWND _parent, DLGPROC _dlgproc, LPARAM _initparam, HINSTANCE hInst = GetModuleHandle(NULL)) = 0;
			virtual INT_PTR ShowModal(HWND _parent, DLGPROC _dlgproc, LPARAM _initparam, HINSTANCE hInst = GetModuleHandle(NULL)) = 0;
		public:
			jCustomDialog(void);
			jCustomDialog(const jCustomDialog& _dialog);
			virtual ~jCustomDialog(void);
		public:
			JDialogProperty(GetFontSize, SetFontSize) uint16_t FontSize;
			JDialogProperty(GetItalic, SetItalic) bool Italic;
			JDialogProperty(GetWeight, SetWeight) uint16_t Weight;
		};

		/*
		\brief For ANSI support. JSON data, both as a file and as a string, must be in ANSI.
		\string = ANSI
		*/
		class jDialogA : public jCustomDialog
		{
		private:
			string m_title;
			string m_classname;
			string m_face;
		protected:
			bool DoGenerateBinary(void);
			bool LoadFromJSONLayout(LPARAM lParam);
		public:
			inline string GetTitle(void) const { return m_title; }
			inline string GetClass(void) const { return m_classname; }
			inline string GetFontFace(void)	const { return m_face; }
			inline void SetTitle(const string& _value) { m_title = _value; doChanged(); }
			inline void SetClass(const string& _value) { m_classname = _value; doChanged(); }
			inline void SetFontFace(const string& _value) { m_face = _value; doChanged(); }
		public:
			bool ParseJSON(const string& _data);
			bool LoadFromFile(const string& _fname);
		public:
			HWND Show(HWND _parent, DLGPROC _dlgproc, LPARAM _initparam, HINSTANCE hInst = GetModuleHandle(NULL));
			INT_PTR ShowModal(HWND _parent, DLGPROC _dlgproc, LPARAM _initparam, HINSTANCE hInst = GetModuleHandle(NULL));
		public:
			static jDialogA FromFile(const string& _fname);
		public:
			jDialogA(void);
			jDialogA(const string& _json);
			jDialogA(const jDialogA& _dialog);
			~jDialogA(void);
		public:
			JDialogProperty(GetTitle, SetTitle) string Title;
			JDialogProperty(GetClass, SetClass) string Class;
			JDialogProperty(GetFontFace, SetFontFace) string FontFace;
		};

		/*
		\brief For Unicode support. JSON data, both as a file and as a string, must be in Utf8.
		\string = Utf8
		\wstring = Utf16
		*/
		class jDialogW : public jCustomDialog
		{
		private:
			wstring m_title;
			wstring m_classname;
			wstring m_face;
		protected:
			bool DoGenerateBinary(void);
			bool LoadFromJSONLayout(LPARAM lParam);
		public:
			inline wstring GetTitle(void) const { return m_title; }
			inline wstring GetClass(void) const { return m_classname; }
			inline wstring GetFontFace(void) const { return m_face; }
			inline void SetTitle(const wstring& _value) { m_title = _value;	doChanged(); }
			inline void SetClass(const wstring& _value) { m_classname = _value;	doChanged(); }
			inline void SetFontFace(const wstring& _value) { m_face = _value; doChanged(); }
		public:
			bool ParseJSON(const string& _data);
			bool LoadFromFile(const wstring& _fname);
		public:
			HWND Show(HWND _parent, DLGPROC _dlgproc, LPARAM _initparam, HINSTANCE hInst = GetModuleHandle(NULL));
			INT_PTR ShowModal(HWND _parent, DLGPROC _dlgproc, LPARAM _initparam, HINSTANCE hInst = GetModuleHandle(NULL));
		public:
			static jDialogW FromFile(const wstring& _fname);
		public:
			jDialogW(void);
			jDialogW(const string& _json);
			jDialogW(const jDialogW& _dialog);
			~jDialogW(void);
		public:
			JDialogProperty(GetTitle, SetTitle) wstring Title;
			JDialogProperty(GetClass, SetClass) wstring Class;
			JDialogProperty(GetFontFace, SetFontFace) wstring FontFace;
		};

#ifndef JDIALOG_NO_FUNCTIONS_CREATE_CONTROLS

		bool WINAPI CreateControlA(uint32_t _ex_style, const string& _title, const string& _classname,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateControlW(uint32_t _ex_style, const wstring& _title, const wstring& _classname,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

		// TEXT, LTEXT, CTEXT, RTEXT

		bool WINAPI CreateTextA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateTextW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateLTextA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateLTextW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateCTextA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateCTextW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateRTextA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateRTextW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

		// EDITTEXT

		bool WINAPI CreateEditTextA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateEditTextW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

		// DEFPUSHBUTTON

		bool WINAPI CreateDefPushButtonA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateDefPushButtonW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

		// LISTBOX

		bool WINAPI CreateListBoxA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y, 
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateListBoxW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y, 
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

		// COMBOBOX

		bool WINAPI CreateComboBoxA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y, 
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateComboBoxW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y, 
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

		// AUTORADIOBUTTON

		bool WINAPI CreateAutoRadioButtonA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateAutoRadioButtonW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

		// AUTOCHECKBUTTON

		bool WINAPI CreateAutoCheckButtonA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateAutoCheckButtonW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

		// GROUPBOX

		bool WINAPI CreateGroupBoxA(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateGroupBoxW(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

		// LISTVIEW, TREEVIEW, TABVIEW

		bool WINAPI CreateListViewA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateListViewW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateTreeViewA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateTreeViewW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateTabViewA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateTabViewW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

		// BITMAP, ICON, BITMAPEX, ICONEX

		bool WINAPI CreateBitmapA(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateBitmapW(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateIconA(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateIconW(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateBitmapExA(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateBitmapExW(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateIconExA(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateIconExW(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

		// SCROLLBAR

		bool WINAPI CreateScrollBarA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateScrollBarW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

		// REBAR, HOTKEY

		bool WINAPI CreateReBarA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateReBarW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateHotKeyA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateHotKeyW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

		// IPADDRESS, MONTHCAL, STATUSBAR, PROGRESSBAR, ANIMATE, HEADER, RICHEDIT, DATETIMEPICKER

		bool WINAPI CreateIPAdressA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateIPAdressW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateMonthCalA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateMonthCalW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateStatusBarA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateStatusBarW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateProgressBarA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateProgressBarW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateAnimateA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateAnimateW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateHeaderA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateHeaderW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateRichEditA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateRichEditW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);
		bool WINAPI CreateDateTimePickerA(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog);
		bool WINAPI CreateDateTimePickerW(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog);

#endif

#ifdef UNICODE
		typedef jDialogW jDialog;

#ifndef JDIALOG_NO_FUNCTIONS_CREATE_CONTROLS
		inline bool WINAPI CreateControl(uint32_t _ex_style, const wstring& _title, const wstring& _classname,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateControlW(_ex_style, _title, _classname, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateText(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateTextW(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateLText(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateLTextW(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateCText(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateCTextW(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateRText(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateRTextW(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateEditText(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateEditTextW(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateDefPushButton(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateDefPushButtonW(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateListBox(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y, 
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateListBoxW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateComboBox(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y, 
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateComboBoxW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateAutoRadioButton(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateAutoRadioButtonW(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateAutoCheckButton(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateAutoCheckButtonW(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateGroupBox(uint32_t _ex_style, const wstring& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateGroupBoxW(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateListView(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateListViewW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateTreeView(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateTreeViewW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateTabView(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateTabViewW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateBitmap(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateBitmapW(_ex_style, _style, _resource, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateIcon(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateIconW(_ex_style, _style, _resource, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateBitmapEx(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateBitmapExW(_ex_style, _style, _resource, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateIconEx(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateIconExW(_ex_style, _style, _resource, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateScrollBar(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateScrollBarW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateIPAdress(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateIPAdressW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateMonthCal(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateMonthCalW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateStatusBar(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateStatusBarW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateProgressBar(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateProgressBarW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateAnimate(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateAnimateW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateHeader(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateHeaderW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateRichEdit(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateRichEditW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateDateTimePicker(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateDateTimePickerW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateHotKey(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogW* _dialog)
		{
			return CreateHotKeyW(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}
#endif // !JDIALOG_NO_FUNCTIONS_CREATE_CONTROLS
#else
		typedef jDialogA jDialog;

#ifndef JDIALOG_NO_FUNCTIONS_CREATE_CONTROLS

		inline bool WINAPI CreateControl(uint32_t _ex_style, const string& _title, const string& _classname,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateControlA(_ex_style, _title, _classname, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateText(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateTextA(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateLText(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateLTextA(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateCText(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateCTextA(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateRText(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateRTextA(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateEditText(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateEditTextA(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateDefPushButton(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateDefPushButtonA(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateListBox(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateListBoxA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateComboBox(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y, 
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateComboBoxA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateAutoRadioButton(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateAutoRadioButtonA(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateAutoCheckButton(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateAutoCheckButtonA(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateGroupBox(uint32_t _ex_style, const string& _title,
			uint32_t _style, int32_t _x, int32_t _y, int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateGroupBoxA(_ex_style, _title, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateListView(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateListViewA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateTreeView(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateTreeViewA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateTabView(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateTabViewA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateBitmap(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateBitmapA(_ex_style, _style, _resource, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateIcon(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateIconA(_ex_style, _style, _resource, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateBitmapEx(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateBitmapExA(_ex_style, _style, _resource, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateIconEx(uint32_t _ex_style, uint32_t _style, uint32_t _resource, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateIconExA(_ex_style, _style, _resource, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateScrollBar(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateScrollBarA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateIPAdress(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateIPAdressA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateMonthCal(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateMonthCalA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateStatusBar(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateStatusBarA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateProgressBar(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateProgressBarA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateAnimate(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateAnimateA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateHeader(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateHeaderA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateRichEdit(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateRichEditA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateDateTimePicker(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateDateTimePickerA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}

		inline bool WINAPI CreateHotKey(uint32_t _ex_style, uint32_t _style, int32_t _x, int32_t _y,
			int32_t _cx, int32_t _cy, uint32_t _uid, jDialogA* _dialog)
		{
			return CreateHotKeyA(_ex_style, _style, _x, _y, _cx, _cy, _uid, _dialog);
		}
#endif // !JDIALOG_NO_FUNCTIONS_CREATE_CONTROLS
#endif // UNICODE
	}
}