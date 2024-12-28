using System.Runtime.InteropServices;
using System.Windows;

namespace OcAssistantApp
{
    public struct GpuInfo
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string Name;
        public uint coreCount;
        public bool isVrReady;
        public uint powerUtilization;
        public ulong memorySizeMB;
        public uint gpuClockKHz;
        public uint vramClockKHz;
    }

    public partial class MainWindow : Window
    {
        const string DLL_PATH = @"..\..\..\..\bin\Debug\GpuDetails.dll";

        [DllImport(DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern GpuInfo GetGpuInfo();

        public MainWindow()
        {
            InitializeComponent();
            var info = GetGpuInfo();
            Test.Content = info.Name;
        }
    }
}