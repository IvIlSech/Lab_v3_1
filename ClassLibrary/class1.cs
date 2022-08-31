using System.Collections.ObjectModel;
using System.Runtime.InteropServices;

namespace ClassLibrary
{
    public enum VMf
    {
        vmsLn,
        vmsLGamma,
        vmdLn,
        vmdLGamma
    }
    public class VMGrid
    {
        public int N { get; set; }
        public float Min { get; set; }
        public float Max { get; set; }
        public float Step
        {
            get { return (Max - Min) / (N - 1); }
        }
        public VMf vmf { get; set; }
        public VMGrid(int n , float Max, float Min, VMf vmf)
        {
            this.N = n;
            this.Max = Max;
            this.Min = Min;
            this.vmf = vmf;
        }
        public VMGrid()
        {
            N = 100;
            Min = 0;
            Max = 100;
            vmf = VMf.vmdLGamma;
        }
    }
    public struct VMTime
    {
        public VMGrid Grid { get; set; }
        public int TimeHA { get; set; }
        public int TimeEP { get; set; }
        public float TimeRatio
        {
            get
            {
                if (TimeHA != 0)
                {
                    return TimeEP / TimeHA;
                }
                else
                {
                    return -1;
                }
            }
        }
        public VMTime(VMGrid grid, int time_ha, int time_ep)
        {
            Grid = grid;
            TimeHA = time_ha;
            TimeEP = time_ep;
        }
        public override string ToString()
        {
            return $"N = {Grid.N}, GridMax = {Grid.Max}, GridMin = {Grid.Min} step = {Grid.Step}, " +
                $"function = {Grid.vmf}, TimeHA = {TimeHA}, TimeEP = {TimeEP}, " +
                $"TimeRatio = {TimeRatio}\n";
        }
    }
    public struct VMAccuracy
    {
        public VMGrid Grid { get; set; }
        public float MaxDiffModule { get; set; }
        public float FuncArg { get; set; }
        public float FuncResHA { get; set; }
        public float FuncResEP { get; set; }
        public VMAccuracy(VMGrid grid, float maxdifmod, float funcarg, float funcresha, float funcresep)
        {
            this.Grid = grid;
            this.MaxDiffModule = maxdifmod;
            this.FuncArg = funcarg;
            this.FuncResHA = funcresha;
            this.FuncResEP = funcresep;
        }
        public override string ToString()
        {
            return $" Grid = {Grid}, MaxDiffModule = {MaxDiffModule}, FuncArg = {FuncArg}," +
                $" FuncResHa = {FuncResHA}, FuncResEP = {FuncResEP}\n";
        }
    }

    public class VMBenchmark
    {
        [DllImport(@"C:\Users\pulsyena-laptop\source\repos\lab_v3\x64\Debug\Dll.dll")]
        static extern void GlF(int n, float Max, float Min, int func_id, int[] time, double[] max_value);
        public ObservableCollection<VMTime> TimeComp { get; set; }
        public ObservableCollection<VMAccuracy> FuncValueComp { get; set; }
        public VMBenchmark()
        {
            TimeComp = new ObservableCollection<VMTime>();
            FuncValueComp = new ObservableCollection<VMAccuracy>();
        }
        public void AddVMTime(VMGrid New_grid)
        {
            try
            {
                VMGrid copyGrid = new VMGrid(New_grid.N, New_grid.Max, New_grid.Min, New_grid.vmf);
                int[] time = new int[3];
                double[] max_value = new double[3];
                GlF(copyGrid.N, copyGrid.Max, copyGrid.Min, (int)copyGrid.vmf, time, max_value);
                var new_time = new VMTime(copyGrid, time[0], time[1]);
                TimeComp.Add(new_time);
            }
            catch (Exception ex)
            {
                throw;
            }
        }
        public void AddVMAccuracy(VMGrid New_grid)
        {
            try
            {
                VMGrid copyGrid = new VMGrid(New_grid.N, New_grid.Max, New_grid.Min, New_grid.vmf);
                int[] time = new int[2];
                double[] max_value = new double[4];
                GlF(copyGrid.N, copyGrid.Max, copyGrid.Min, (int)copyGrid.vmf, time, max_value);
                var new_accur = new VMAccuracy(copyGrid, (float)max_value[0],
                                               (float)max_value[1], (float)max_value[2], (float)max_value[3]);
                FuncValueComp.Add(new_accur);
            }
            catch (Exception ex)
            {
                throw;
            }
        }
        public float MaxRatio
        {
            get
            {
                float max;
                var TimeItem = from i in TimeComp
                               where i.TimeRatio > 0
                               select i.TimeRatio;
                max = TimeItem.Max();
                return max;
            }
        }
        public float MinRatio
        {
            get
            {
                float min;
                var TimeItem = from i in TimeComp
                               where i.TimeRatio > 0
                               select i.TimeRatio;
                min = TimeItem.Min();
                return min;
            }
        }
    }
}