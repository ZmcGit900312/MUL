using System.IO;
using System.Windows.Forms.DataVisualization.Charting;

namespace Configuration
{
    //Card Select
    /*
        Geometry Card
    BC	specifies the outer boundaries of the simulation region
    BL	creates a line
    BP	creates a parallelogram
    BQ	creates a quadrangle
    BT	creates a triangle
    CB	changes already assigned labels
    CL	creates a circular line using segments
    CN	changes the direction of the normal vector
    DD	utilises domain decomposition to solve a MoM model more efficiently
    DK	creates a dielectric or magnetic eighth of a sphere
    DP	defines a node point
    DZ	creates a cylindrical dielectric shell
    EG	defines the end of the geometry
    EL	creates a segment of an ellipsoid
    FA	defines an antenna array analysis
    FM	set parameters related to the MLFMM
    FO	defines a Fock region
    FP	set parameters related to the FEM
    HC	creates a cylinder with a hyperbolic border
    HE	creates a coil from wire segments
    HP	creates a plate with a hyperbolic border
    HY	creates a hyperboloid section
    IN	reads an external include file containing mesh information
    IP	sets the parameter that defines the degree of meshing
    KA	defines the border of the PO area
    KK	creates a elliptical conical segment
    KL	sets the wedges in the PO area
    KR	creates a planar elliptical element
    KU	creates a spherical element
    LA	specifies the label for segments, triangles, polygonal plates, etc.
    ME	defines the medium
    MB	defines a modal port boundary condition
    NC	defines the name for the next configuration
    NU	creates a NURBS surface from specified control points
    PB	creates a paraboloid
    PE	specifies the unit cell that will be used in periodic boundary condition calculations
    PH	creates a flat plate with an elliptic hole
    PM	creates a polygonal shape that is meshed into triangles
    PO	applies the physical optics approximation
    PY	creates a polygonal plate for use with UTD
    QT	creates a dielectric or magnetic cuboid (meshed into tetrahedral elements)
    QU	creates a dielectric or magnetic cuboid (meshed into cuboidal elements)
    RM	remeshing and adaptive mesh refinement
    SF	enters a scaling factor, with which all dimensions are multiplied
    SY	utilises symmetry in the construction of the geometry
    TG	transformation (i.e. translation and rotation) of the geometric structures
    TO	creates a toroid
    TP	transforms a point
    UT	parameters for the uniform theory of diffraction (UTD)
    UZ	creates a cylinder for use in the UTD region
    VS	specifies known visibility information (required when using physical optics with multiple reflections)
    WA	define all active windscreen antenna elements
    WG	creates a parallelogram consisting of a wire grid
    WR	defines the dielectric windscreen reference plane
    ZY	creates a cylindrical element

	    Control Card
    **	characters used to indicate a comment line
    Ax	type of excitation (e.g. an incident plane wave or a voltage source)
    AB	creates a modal port excitation
    BO	through the use of the reflection factor coefficient a ground plane can be inserted
    CA	defines a cable path section for the cable irradiation computation.
    CD	defines a specific cable cross section (single, coaxial, ribbon and bundle/multi-cable)
    CF	set the type of integral equation for perfectly conducting metallic surfaces
    CG	the algorithm used to solve the matrix equation is selected
    CI	defines a cable interconnect and termination
    CM	field calculation for CableMod and CRIPTE (coupling into transmission lines) or PCBMod (coupling into a PCB)
    CO	inserts a dielectric and/or magnetic surface on the elements
    CS	defines a cable path section and the centre/reference location to which a cable cross section is applied
    DA	creates additional files for the results
    DI	enters the properties of the dielectric medium
    DL	defines a layered medium
    EE	calculates the error estimates
    EN	indicates the end of the input file
    FD	defines the FDTD solver settings
    FE	calculates the near fields
    FF	calculates the far fields
    FR	sets the frequencies at which the calculations are to be carried out
    GF	sets the Green's functions
    KC	transfer the signal names in CADFEKO to POSTFEKO
    KS	transfer the connector names in CADFEKO to POSTFEKO
    L2	defines a complex load on a vertex
    L4	adds a load between a metallic triangle and the ground plane for the planar multilayer Green's function
    LC	defines a cable load
    LD	defines a distributed load, consisting of resistance, inductance and capacitance
    LE	defines a load on the edge between surface triangles
    LF	impress a complex impedance between two points inside a FEM mesh
    LN	defines a complex load to any non-radiating network port that is not connected to geometry
    LP	defines a parallel circuit (resistance, inductance and capacitance) load
    LS	defines a series circuit (resistance, inductance and capacitance) load
    LT	defines a series circuit (resistance, inductance or capacitance) load to a voxel mesh to be used in conjunction with the FDTD method
    LZ	defines a complex load on a segment
    NW	define a linear non-radiating network
    OF	offset i.e. displacement of the origin when calculating the near fields
    OM	calculates the weighted set of orthogonal current-modes that are supported on a conducting surface.
    OS	saves the surface currents in a file
    PP	defines the phase for periodic boundary condition calculation
    PR	defines a current/voltage probe
    PS	sets general control parameters
    PW	defines the radiating power of a transmitting antenna
    RA	defines an ideal receiving antenna
    SA	used to calculate SAR in dielectric media
    SH	used to define solid and braided cable shields
    SK	takes a finite conductivity into account through the skin effect of ohmic losses; also for thin dielectric layers
    SP	calculates the S-parameters for the active sources
    TL	specifies a non-radiating transmission line
    TR	calculate reflection and transmission coefficients for an incident plane wave on a planar structure
    WD	defines the dielectric properties of the windscreen glass layers
*/
    public enum Card
    {
        //Geometry Card
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
        void Read(string[] buffer);
    }
    /// <inheritdoc />
    /// File Parameter
    public class Cin : ICard
    {
        public Card Type => Card.IN;
        public int FileType { get; set; } = 3;//Nas=3

        public int Patch { get; set; } = 0;//Patch Information
        public string MeshFile { get; set; }
        public string ProjectDir { get; set; }
        public string ProjectName { get; set; }
        public void Write(StreamWriter sw) => sw.WriteLine("{0}\t{1}\t{2}\t{3}\t{4}\t{5}",
            (int)Type, FileType, Patch, MeshFile, ProjectDir, ProjectName);

        public void Read(string[] buffer)
        {
            FileType = int.Parse(buffer[1]);
            Patch = int.Parse(buffer[2]);
            MeshFile = buffer[3];
            ProjectDir = buffer[4];
            ProjectName = buffer[5];
        }
    }

    /// <inheritdoc />
    /// Frequency Set
    public class Cfr : ICard
    {
        public Card Type => Card.FR;
        public double Frequency { get; set; } = 3.0e8;

        public void Write(StreamWriter sw) => sw.WriteLine("{0}\t{1:E}", (int)Type, Frequency);
        public void Read(string[] buffer) => Frequency = double.Parse(buffer[1]);
        
    }

    /// <inheritdoc />
    /// BiCG-STAB Parameter
    public class Ccg : ICard
    {
        public Card Type => Card.CG;
        public int SolutionType { get; set; } = 0;//BiCG-Stab=0
        public int PreConditionType { get; set; } = 2;//Jacobi=1,ILU=2
        public int Maxiteration { get; set; } = 0;
        public double Tolerance { get; set; } = 1.0e-4;
        public double MaxStopTolerance { get; set; } = 10;

        public void Write(StreamWriter sw) => sw.WriteLine("{0}\t{1}\t{2}\t{3}\t{4:E}\t{5:E}",
            (int)Type, SolutionType, PreConditionType, Maxiteration, Tolerance, MaxStopTolerance);

        public void Read(string[] buffer)
        {
            SolutionType = int.Parse(buffer[1]);
            PreConditionType = int.Parse(buffer[2]);
            Maxiteration = int.Parse(buffer[3]);
            Tolerance = double.Parse(buffer[4]);
            MaxStopTolerance = double.Parse(buffer[5]);        
        }
    }



    //FarField
    public class Cff : ICard
    {
        public Cff() { }
        public Cff(string[] buffer) { Read(buffer); }
        public Card Type => Card.FF;
        public uint FieldType { get; set; } = 0;//RCS
        public int ThetaNum { get; set; } = 1;
        public int PhiNum { get; set; } = 1;
        public double ThetaStart { get; set; } = 0;
        public double PhiStart { get; set; } = 0;
        public double ThetaIncrement { get; set; } = 1;
        public double PhiIncrement { get; set; } = 1;
        public string FarFileName { get; set; }
        
        public void Write(StreamWriter sw) => sw.WriteLine("{0}\t{1}\t{2}\t{3}\t{4:f}\t{5:f}\t{6:f}\t{7:f}\t{8}",
            (int)Type, FieldType, ThetaNum, PhiNum, ThetaStart, PhiStart, ThetaIncrement, PhiIncrement, FarFileName);

        public void Read(string[] buffer)
        {
            FieldType = uint.Parse(buffer[1]);
            ThetaNum = int.Parse(buffer[2]);
            PhiNum = int.Parse(buffer[3]);
            ThetaStart = double.Parse(buffer[4]);
            PhiStart = double.Parse(buffer[5]);
            ThetaIncrement = double.Parse(buffer[6]);
            PhiIncrement = double.Parse(buffer[7]);
            FarFileName = buffer[8];     
        }
    }

    //BasicFunction with Current
    public class Cos : ICard
    {
        public Card Type => Card.OS;
        public int BasicFunctionType { get; set; } = -1;
        public string BasicFunctionName { get; set; } = @"";

        public void Write(StreamWriter sw) => sw.WriteLine("{0}\t{1}\t{2}",
            (int) Type, BasicFunctionType, BasicFunctionName);

        public void Read(string[] buffer)
        {
            BasicFunctionType = int.Parse(buffer[1]);
            BasicFunctionName = buffer[2];
        }
    }

    //Plane wave
    public class Ca0 : ICard
    {
        public Card Type => Card.A0;
        //Direction
        public uint ThetaNum { get; set; } = 1;
        public uint PhiNum { get; set; } = 1;
        public double ThetaStart { get; set; } = 0;
        public double PhiStart { get; set; } = 0;
        public double ThetaIncrement { get; set; } = 1;
        public double PhiIncrement { get; set; } = 1;
        //E polarisation
        public int Polarisation { get; set; } = 0;//-1:left,0:linear,1:right
        public double Magnitude { get; set; } = 1;
        public double Phase { get; set; } = 0;
        public double Eta { get; set; } = 0;
        public double RotationX { get; set; } = 0;
        public double RotationY { get; set; } = 0;
        public double RotationZ { get; set; } = 0;

        //FileName
        public string ExcitationName { get; set; } = "Planewave";
        public void Write(StreamWriter sw) => sw.WriteLine("{0}\t{1}\t{2}\t{3:f}\t{4:f}\t{5:f}\t{6:f}\t{7}\t{8:f}\t{9:f}\t{10:f}\t{11:f}\t{12:f}\t{13:f}\t{14}",
                    (int)Type, ThetaNum, PhiNum, ThetaStart, PhiStart, ThetaIncrement, PhiIncrement,
                    Polarisation, Magnitude, Phase, Eta, RotationX, RotationY, RotationZ, ExcitationName);

        public void Read(string[] buffer)
        {
            ThetaNum = uint.Parse(buffer[1]);
            PhiNum = uint.Parse(buffer[2]);
            ThetaStart = double.Parse(buffer[3]);
            PhiStart = double.Parse(buffer[4]);
            ThetaIncrement = double.Parse(buffer[5]);
            PhiIncrement = double.Parse(buffer[6]);
            Polarisation = int.Parse(buffer[7]);
            Magnitude = double.Parse(buffer[8]);
            Phase = double.Parse(buffer[9]);
            Eta = double.Parse(buffer[10]);
            RotationX = double.Parse(buffer[11]);
            RotationY = double.Parse(buffer[12]);
            RotationZ = double.Parse(buffer[13]);
            ExcitationName = buffer[14];
        }
    }
    //AIM
    public class Cam : ICard
    {
        
        
        public Card Type => Card.AM;
        public int Order { get; set; } = 3;
        public double Interval { get; set; } = 0.05;
        public double Threshold { get; set; } = 0.3;
        public double NearCorrectionEps { get; set; } = 1.0e-3;
        public int Dimension { get; set; } = 3;
        public int FillingStrategy { get; set; } = 1;
        public void Write(StreamWriter sw) => sw.WriteLine("{0}\t{1}\t{2:f}\t{3:f}\t{4:E}\t{5}\t{6}",
            (int)Type, Order, Interval, Threshold, NearCorrectionEps, Dimension, FillingStrategy);

        public void Read(string[] buffer)
        {
            Order = int.Parse(buffer[1]);
            Interval = double.Parse(buffer[2]);
            Threshold = double.Parse(buffer[3]);
            NearCorrectionEps = double.Parse(buffer[4]);
            Dimension = int.Parse(buffer[5]);
            FillingStrategy = int.Parse(buffer[6]);
        }
    }
}
