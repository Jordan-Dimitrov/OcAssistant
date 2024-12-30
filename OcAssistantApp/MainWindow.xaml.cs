using System.IO;
using System.IO.Pipes;
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

        private NamedPipeClientStream _PipeClient;
        private bool _IsPipeClientOpen = false;

        public MainWindow()
        {
            InitializeComponent();
            var info = GetGpuInfo();
            GpuName.Content += $" {info.Name}";
            CoreCount.Content += $" {info.coreCount.ToString()}";
            IsVrReady.Content += $" {info.isVrReady.ToString()}";
            MemorySize.Content += $" {info.memorySizeMB.ToString()} MB";
            GpuClock.Content += $" {info.gpuClockKHz.ToString()} MHz";
            VramClock.Content += $" {info.vramClockKHz.ToString()} MHz";

            _PipeClient = new NamedPipeClientStream(".", "BenchmarkPipe", PipeDirection.Out);
            _PipeClient.Connect();
            _IsPipeClientOpen = true;
        }

        private void StartBenchmarkClick(object sender, RoutedEventArgs e)
        {
            SendCommand("start");
        }

        private void StopBenchmarkClick(object sender, RoutedEventArgs e)
        {
            SendCommand("stop");
        }

        private void SendCommand(string command)
        {
            if (_IsPipeClientOpen)
            {
                try
                {
                    using (StreamWriter writer = new StreamWriter(_PipeClient, leaveOpen: true))
                    {
                        writer.Write(command);
                        writer.Flush();
                    }
                }
                catch (IOException ex)
                {
                    MessageBox.Show($"Failed to send command: {ex.Message}");
                }
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            if (_IsPipeClientOpen)
            {
                _PipeClient.Close();
                _PipeClient.Dispose();
                _IsPipeClientOpen = false;
            }
        }
    }
}