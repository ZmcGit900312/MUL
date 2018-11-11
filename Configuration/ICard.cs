using System.IO;

namespace Configuration
{
    //Card Select
    public enum Card
    {
        BC = 0, BL, BP, BQ, BT, CB = 5, CL, CN, DD, DK,
        DP = 10, DZ, EG, EL, FA, AM = 15, FO, FP, HC, HE,
        HP = 20, HY, IN, IP, KA, KK = 25, KL, KR, KU, LA,
        MB = 30, ME, NC, NU, PB, PE = 35, PH, PM, PO, PY,
        QT = 40, QU, RM, SF, SY, TG = 45, TO, TP, UT, UZ,
        VS = 50, WA, WG, WR, ZY,

        A0 = 60, A1, A2, A3, A4, A5 = 65, A6, A7, AB, AC,
        AE = 70, AF, AI, AK, AN, AP = 75, AR, AS, AT, AV,
        AW = 80, BO, CA, CD, CF, CG = 85, CM, CO, CS, CI,
        DA = 90, DI, DL, EE, EN, FD = 95, FE, FF, FR, GF,
        KC = 100, KS, L2, L4, LC, LD = 105, LE, LF, LN, LP,
        LS = 110, LT, LZ, NW, OF, OM = 115, OS, PP, PR, PS,
        PW = 120, RA, SA, SH, SK, SP = 125, TL, TR, WD
    }

    public interface ICard
    {
        Card Type { get; }
        void Write(StreamWriter sw);
    }
    /// <inheritdoc />
    /// Mesh File Parameter
    public class Cin : ICard
    {
        public Card Type => Card.IN;
        public uint FileType { get; set; } = 3;//Nas=3

        public uint Patch { get; set; } = 0;//Patch Information
        public string MeshFile { get; set; }
        public string ProjectDir { get; set; }
        public string ProjectName { get; set; }
        public void Write(StreamWriter sw)=> sw.WriteLine("{0}\t{1}\t{2}\t{3}\t{4}\t{5}",
            (int)Type,FileType,Patch,MeshFile,ProjectDir,ProjectName);
    }

    /// <inheritdoc />
    /// Frequency Set
    public class Cfr : ICard
    {
        public Card Type => Card.FR;
        public double Frequency { get; set; } = 3.0e8;

        public void Write(StreamWriter sw) => sw.WriteLine("{0}\t{1:E}", (int)Type, Frequency);
        
    }

    /// <inheritdoc />
    /// BiCG-STAB Parameter
    public class Ccg : ICard
    {
        public Card Type => Card.CG;
        public uint SolutionType { get; set; } = 0;//BiCG-Stab=10
        public uint PreConditionType { get; set; } = 0;//Jacobi=64,ILU=128
        public uint Maxiteration { get; set; } = 0;
        public double Tolerance { get; set; } = 1.0e-4;
        public double MaxStopTolerance { get; set; } = 10;

        public void Write(StreamWriter sw) => sw.WriteLine("{0}\t{1}\t{2}\t{3}\t{4:E}\t{5:E}", 
            (int)Type,SolutionType,PreConditionType,Maxiteration,Tolerance,MaxStopTolerance);
    }

    

    //FarField
    public class  Cff : ICard
    {
        public uint FieldType { get; set; } = 0;//RCS
        public uint ThetaNum { get; set; } = 1;
        public uint PhiNum { get; set; }= 1;
        public double ThetaStart { get; set; } = 0;
        public double PhiStart { get; set; }= 0;
        public double ThetaIncrement { get; set; }= 1;
        public double PhiIncrement { get; set; } = 1;
        public string FarFileName { get; set; }
        public Card Type => Card.FF;
        public void Write(StreamWriter sw) => sw.WriteLine("{0}\t{1}\t{2}\t{3}\t{4:f}\t{5:f}\t{6:f}\t{7:f}\t{8}",
            (int)Type, FieldType, ThetaNum,PhiNum,ThetaStart,PhiStart,ThetaIncrement,PhiIncrement,FarFileName);
    }

    //Plane wave
    public class Ca0 : ICard
    {
        public Card Type => Card.A0;
        //Direction
        public uint ThetaNum { get; set; }= 1;
        public uint PhiNum { get; set; } = 1;
        public double ThetaStart { get; set; } = 0;
        public double PhiStart { get; set; } = 0;
        public double ThetaIncrement { get; set; } = 1;
        public double PhiIncrement { get; set; } = 1;
        //E polarisation
        public int Polarisation { get; set; }= 0;//-1:left,0:linear,1:right
        public double Magnitude { get; set; }=1;
        public double Phase { get; set; }=0;
        public double Eta { get; set; }=0;
        public double RotationX { get; set; } = 0;
        public double RotationY { get; set; }=0;
        public double RotationZ { get; set; }=0;
        
        //FileName
        public string ExcitationName { get; set; }
        public void Write(StreamWriter sw) => sw.WriteLine("{0}\t{1}\t{2}\t{3:f}\t{4:f}\t{5:f}\t{6:f}\t{7}\t{8:f}\t{9:f}\t{10:f}\t{11:f}\t{12:f}\t{13:f}\t{14}",
                    (int)Type, ThetaNum, PhiNum, ThetaStart, PhiStart, ThetaIncrement, PhiIncrement,
                    Polarisation,Magnitude,Phase,Eta,RotationX,RotationY,RotationZ, ExcitationName);
    }
    //AIM
    public class Cam : ICard
    {
        public Card Type => Card.AM;
        public int Order { get; set; }=3;
        public double Interval { get; set; } = 0.05;
        public double Threshold { get; set; }= 0.3;
        public double NearCorrectionEps { get; set; }= 1.0e-3;
        public uint Dimension { get; set; } = 3;
        public int FillingStrategy { get; set; } = 1;
        public void Write(StreamWriter sw) => sw.WriteLine("{0}\t{1}\t{2:f}\t{3:f}\t{4:E}\t{5}\t{6}", 
            (int)Type,Order,Interval,Threshold,NearCorrectionEps,Dimension,FillingStrategy);
    }
}
