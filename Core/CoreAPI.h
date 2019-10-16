#pragma once
namespace Core
{
#pragma region MAINAPI

	/**
	* \brief Read Initial Information and Initial the SystemConfiguration
	* \param filename The name of configuration  *.hxj
	* \return 0 Success
	*/
	//int EMCParameterInitialization(char* filename);

	/**
	* \brief Read Initial Information and Initial the SystemConfiguration
	* \param filename The name of configuration  *.xml
	* \return 0 Success
	*/
	int AnalysisConfigurationFile(char* filename);
	int ParseConfiguratoinFile(char* filename);
	

	int DataInitialization();

	/**
	 * \brief The mission centre to control the flow of calculations for matrix and request
	 * \param requestonly true: only calculate request and false: calculate matrix and request
	 * \return 
	 */
	int CalculatedMissionCentre(bool requestonly = false);

	/**
	 * \brief Save Current, Basisfunction and RCS Results into Files
	 * \return 0 Success
	 */
	int SaveResults(bool requestonly = false);
	/**
	* \brief Release the static data in ComponentList
	* \return 0 Success
	*/
	int ReleaseMemory();
#pragma endregion MAINAPI


#pragma region CompositeAPI
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
	int CreatBasisFunction(const bool isCreat = false);
	/**
	* \brief Based on the binary file of BasicFunction.dat construct BasicFunction暂时只有RWG基函数
	* \param loadFileName The filename to be load
	* \n \b PS: It need the mesh construction before
	* \n BasicFunction.dat first and second terms are [BasicFuctionTypte Unknowns]
	* \n RWGFormat is [ID trianglePlusID triangleMinusID nodePlusID nodeMinusID nodeLeftID nodeRightID current]
	* \return 0 is Success; 1 is Fail to read BasicFunction.dat
	*/
	static int LoadBasisFunction(const char* loadFileName);
	/**
	* \brief Write Information of BasicFuntion in to binary File: BasicFunction.dat
	* \param saveFileName The filename to be saved
	* \return 0 is Success; 1 is Fail to save BasicFunction.dat; 2 BasicFunctions are empty
	*/
	int SaveBasisFunction(const char* saveFileName);

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
	* \brief Initial the Solver and Impedance Matrix，有MoM和AIM可选
	* \return 0 Success
	*/
	int InitialSolverAndImpedance(int currentIndex = 0);
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
	int Solve(int currentIndex=0);
	
	/**
	 * \brief Generate and Solve Impedance Matrix, including set RightHand
	 * \param configIndex The index of sweep
	 * \return 0 Success
	 */
	int MatrixSolving(int configIndex);

	/**
	 * \brief Initial Current
	 * \param read default=false creat
	 * \return 0 Success
	 */
	int SetCurrent(bool read = false);

	/**
	 * \brief Calculate the far field RCS with specific current
	 * \param zmc 
	 * \return 0 Success
	 */
	int CalculateRequest(int zmc);
#pragma endregion

#pragma region OTHERAPI	
	
#pragma endregion OTHERAPI
}

