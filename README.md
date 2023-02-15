## Winui3Windowing

Can be directly launched or File Activated via .jpg, .bmp, .jpeg, and .png.

### CppWinrtComponent
Contains Page2.xaml/idl/h/cpp and WindowService.idl/h/cpp. Page2 is the page navigated to for FileActivation. WindowService.h/cpp saves the opened windows and return the AppWindow/Window handles.

### CppWinrtComponentProject
C# project which creates CppWinrtComponent's projection so Windowing can use it.

### Windowing
Start up project. Contains WindowService.cs and Page1.xaml/.cs. WindowService.cs creates windows on new threads. Page1 is the page navigated to for App Launch.