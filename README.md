# jDialogs
 Dialoges builder from JSON for Windows (C++)
 
# Uses

Add two files from include to the project. At the beginning, insert this code
```cpp
namespace jDialogs = perchik71::jDialogs;
```

To load the dialog in ANSI, you need to select the class with the prefix A
```cpp
jDialogs::jDialogA* lpDialogs = new jDialogs::jDialogA();
```
And for Wide with the W prefix
```cpp
jDialogs::jDialogW* lpDialogs = new jDialogs::jDialogW();
```

**Warning**: Wide creates a dialog by calling the WinAPI Wide function. Your JSON file must be in UTF-8 encoding, all names will be decoded in Wide encoding. For ANSI, the your file must be in ANSI encoding. Also, Wide allows you to load files not only in the English-language file path.

Load a file

(ANSI)
```cpp
lpDialogs->LoadFromFile("test.json");
```
(WIDE)
```cpp
lpDialogs->LoadFromFile(L"test.json");
```

Display the dialog in normal mode
```cpp
lpDialogs->Show(hWnd, DlgProc, 0);
```
Display the dialog in modal mode
```cpp
lpDialogs->ShowModal(hWnd, DlgProc, 0); 
```

Sample file json
```json
{"ExStyle": 0,
 "Style": ["WS_SYSMENU", "WS_THICKFRAME", "WS_MINIMIZEBOX"],
 "Title": "Dialog for testing",
 "ClassName": "",
 "x": 0,
 "y": 0,
 "Width": 520,
 "Height": 365,
 "FontName": "Microsoft Sans Serif",
 "FontSize": 12,
 "FontWeight": 400,
 "FontItalic": 0,
 "Controls": [
  {"Type": "GROUPBOX", "Title": "Hello World", "x": 10, "y": 10, "Width": 400, "Height": 300},
  {"Type": "LTEXT", "Title": "Hello World", "x": 20, "y": 28, "Width": 230, "Height": 12},
  {"Type": "CTEXT", "Title": "Hello World", "x": 20, "y": 40, "Width": 230, "Height": 12},
  {"Type": "RTEXT", "Title": "Hello World", "x": 20, "y": 52, "Width": 230, "Height": 12},
  {"Type": "EDITTEXT", "Title": "Hello World", "Style": ["ES_AUTOHSCROLL"], "x": 20, "y": 64, "Width": 230, "Height": 14},
  {"Type": "DEFPUSHBUTTON", "Title": "Hello World", "x": 20, "y": 80, "Width": 120, "Height": 12},
  {"Type": "LISTBOX", "x": 60, "y": 100, "Width": 120, "Height": 80},
  {"Type": "COMBOBOX", "x": 200, "y": 100, "Width": 120, "Height": 80},
  {"Type": "AUTORADIOBUTTON", "Title": "Hello World", "x": 200, "y": 200, "Width": 120, "Height": 14},
  {"Type": "AUTOCHECKBUTTON", "Title": "Hello World", "x": 200, "y": 220, "Width": 120, "Height": 14},
  {"Type": "PROGRESSBAR", "x": 60, "y": 240, "Width": 120, "Height": 20}
  ]
}
```
**Warning**: By default, Windows uses the so-called Dialog Unit for the location and size of dialogs and their controls. I also implemented a mechanism for accessing these magic values, so specify all `x`, `y`, `Width`, `Height` only in `pixels`. In [msdn](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-mapdialogrect), it is written that half of the size in height and width of the specified font is taken. 

Styles must be typed either in `[]`, or `0`, or not specified at all. Some styles are already embedded and can be specified as a string, while others can be typed in hex, where `0x` must be specified at the beginning (hex should be string in `""`).

# Dependencies
The project depends on https://github.com/nlohmann/json used for working with JSON data.
 
# Supported classes

| Класс | Описание |
|----:|:----:|
| TEXT | `String` element |
| LTEXT | Left-aligned `string` control |
| CTEXT | Center-aligned `string` control |
| RTEXT | Right-aligned `string` control |
| EDITTEXT | `Entry` control |
| DEFPUSHBUTTON | `Button` control with tabstop |
| PUSHBUTTON | `Button` control |
| LISTBOX | `ListBox` control |
| COMBOBOX | `ComboBox` control |
| AUTORADIOBUTTON | `RadioButton` control |
| AUTOCHECKBUTTON | `CheckButton` control |
| GROUPBOX | `GroupBox` control |
| LISTVIEW | `ListView` uses CommCtrl 6.0 |
| TREEVIEW | `TreeView` uses CommCtrl 6.0 |
| TABVIEW | `TabControl` uses CommCtrl 6.0 |
| BITMAP | `Static` control for Bitmap (Title = indetificator resource bitmap) |
| ICON | `Static` control for Icon (Title = indetificator resource icon) |
| BITMAPEX | Center-aligned `Static` control for Bitmap (Title = indetificator resource bitmap) |
| ICONEX | Center-aligned `Static` control for Icon (Title = indetificator resource icon) |
| SCROLLBAR | `ScrollBar` control |
| REBAR | `ReBarControl` uses CommCtrl 6.0 |
| HOTKEY | `HotKey` uses CommCtrl 6.0 |
| IPADDRESS | `IpAddress` control uses CommCtrl 6.0 |
| MONTHCAL | `MonthCalendar` control uses CommCtrl 6.0 |
| STATUSBAR | `StatusBar` control |
| PROGRESSBAR | `ProgressBar` control uses CommCtrl 6.0 |
| ANIMATE | `SysAnimate` control uses CommCtrl 6.0 |
| HEADER | `SysHeader` control uses CommCtrl 6.0 |
| RICHEDIT | `RichEdit20` control (Ansi = RichEdit20A or Wide = RichEdit20W) |
| DATETIMEPICKER | `DateTimePick` control uses CommCtrl 6.0 |

# License
![MIT License](https://camo.githubusercontent.com/20666e1b72ed1ea8f0a7c1d1e0ea35769a7c24f879ecc27ac16641b46f225a01/68747470733a2f2f6f70656e736f757263652e6f72672f74726164656d61726b732f6f70656e736f757263652f4f53492d417070726f7665642d4c6963656e73652d313030783133372e706e67)

The classes is licensed under the [MIT License](https://opensource.org/licenses/MIT):

Copyright © 2021 Alex (Perchik71) 

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

_____

The class contains the JSON Parser from [Niels Lohmann](https://nlohmann.me/) which is licensed under the MIT License (see above). Copyright © 2013-2021 [Niels Lohmann](https://nlohmann.me/)