// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using WinRT.Interop;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace CustomTitleBar
{
    /// <summary>
    /// An empty window that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainWindow : Window
    {
        public static AppWindow m_AppWindow;
        public MainWindow()
        {
            this.InitializeComponent();

            m_AppWindow = GetAppWindowForCurrentWindow();

            AppTitleBar.SetWindows(this, m_AppWindow);

            // Check to see if customization is supported.
            // The method returns true on Windows 10 since Windows App SDK 1.2, and on all versions of
            // Windows App SDK on Windows 11.
            var titleBar = m_AppWindow.TitleBar;
            titleBar.ExtendsContentIntoTitleBar = true;
        }

        public AppWindow GetAppWindowForCurrentWindow()
        {
            IntPtr hWnd = WindowNative.GetWindowHandle(this);
            WindowId wndId = Microsoft.UI.Win32Interop.GetWindowIdFromWindow(hWnd);
            return AppWindow.GetFromWindowId(wndId);
        }


    }
}
