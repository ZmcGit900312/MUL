#pragma once
namespace Core
{
#pragma region MAINAPI
	//*****************************************Main API**************************************************
	/**
	* \brief Read the nas file to construct mesh
	* \return 0 is Success; 1 is Fail to read mesh.nas
	*/
	int CreatMesh();
	/**
	* \brief Based on mesh, construct the BasicFunctions，暂时只有RWG基函数
	* \param isCreat Load or Creat BasicFunction. Default is true
	* \return 0 is Success; 1 is Fail to construct BasicFunction.dat
	*/
	int CreatBasicFunction(const bool isCreat=true);
	/**
	* \brief Based on the binary file of BasicFunction.dat construct BasicFunction暂时只有RWG基函数
	* \param loadFileName The filename to be load
	* \n \b PS: It need the mesh construction before 
	* \n BasicFunction.dat first and second terms are [BasicFuctionTypte Unknowns]
	* \n RWGFormat is [ID trianglePlusID triangleMinusID nodePlusID nodeMinusID nodeLeftID nodeRightID current]
	* \return 0 is Success; 1 is Fail to read BasicFunction.dat
	*/
	static int LoadBasicFunction(const char* loadFileName);
	/**
	* \brief Write Information of BasicFuntion in to binary File: BasicFunction.dat
	* \param saveFileName The filename to be saved
	* \return 0 is Success; 1 is Fail to save BasicFunction.dat; 2 BasicFunctions are empty
	*/
	int SaveBasicFunction(const char* saveFileName);

	/**
	* \brief Select the type of Green Function，目前只有自由空间格林函数
	* \return 0 FreeSpace GreenFunction
	*/
	int SetGreenFunction();
	/**
	* \brief Pre Calculate the Self-Impedance of each Triangle
	* \return 0 Success
	*/
	int PreCalculateSelfTriangleImpedance();
	/**
	* \brief Initial the Impedance Matrix，有MoM和AIM可选
	* \return 0 Success
	*/
	int CreatImpedance();
	/**
	* \brief Set Excitation: Only have plane wave
	* \return 0 Success
	*/
	int SetRightHand();
	/**
	* \brief Fill the MoM Impedance
	* \return 0 Success
	*/
	int FillImpedance();
	/**
	* \brief Solving and write to Basic Function
	* \return Solving state
	* \n 0 Success
	* \n 1 Failure
	* \n 2 No-converage
	*/
	int Solve();
	/**
	* \brief Calculate the far field RCS and write in RCS.txt
	* \param IsReadFromFile Whether read the basicfunction from file. Default is false
	* \return State of Calculation
	* \n 0 Success
	*/
	int CalculateRequest(const bool IsReadFromFile = false);

#pragma endregion MAINAPI

#pragma region OTHERAPI
	//**********************************************OtherAPI*******************************************************

	
	/**
	* \brief Read Initial Information and Initial the SystemConfiguration 
	* \param filename The name of configuration  *.hxj
	* \return 0 Success
	*/
	int EMCParameterInitialization(char* filename);
	/**
	* \brief Release the static data in ComponentList
	* \return 0 Success
	*/
	int ReleaseMemory();
#pragma endregion OTHERAPI
}

