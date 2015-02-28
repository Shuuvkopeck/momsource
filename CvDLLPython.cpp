#include "CvGameCoreDLL.h"
#include "CyMap.h"
#include "CyPlayer.h"
#include "CySelectionGroup.h"
#include "CyPlot.h"
#include "CyGame.h"
#include "CyUnit.h"
#include "CyGlobalContext.h"
#include "CyCity.h"

void CyCityPythonInterface1(python::class_<CyCity>& x);
void CyCityPythonInterface2(python::class_<CyCity>& x);
void CyPlotPythonInterface1(python::class_<CyPlot>& x);
void CyPlayerPythonInterface1(python::class_<CyPlayer>& x);
void CyPlayerPythonInterface2(python::class_<CyPlayer>& x);
void CyUnitPythonInterface1(python::class_<CyUnit>& x);
void CyGlobalContextPythonInterface1(python::class_<CyGlobalContext>& x);
void CyGlobalContextPythonInterface2(python::class_<CyGlobalContext>& x);
void CyGlobalContextPythonInterface3(python::class_<CyGlobalContext>& x);
void CyGlobalContextPythonInterface4(python::class_<CyGlobalContext>& x);
void CyGamePythonInterface();
void CyRandomPythonInterface();
void CyEnumsPythonInterface();
void CyTeamPythonInterface();
void CyAreaPythonInterface();
void CyStructsPythonInterface1();
void CyMapPythonInterface();
void CyMapGeneratorPythonInterface();
void CyInfoPythonInterface1();
void CyInfoPythonInterface2();
void CyInfoPythonInterface3();
void CyInfoPythonInterface4();
void CySelectionGroupInterface(python::class_<CySelectionGroup>& x);
void CySelectionGroupInterface2(python::class_<CySelectionGroup>& x);
void CyArtFileMgrPythonInterface();
void CyGameTextMgrInterface();
void CyHallOfFameInterface();
void CyGameCoreUtilsPythonInterface();
void CyMessageControlInterface();

/*************************************************************************************************/
/**	ADDON (WINAMP) merged Sephi																	**/
/*************************************************************************************************/
void CyWinampPythonInterface();
/*************************************************************************************************/
/**	ADDON (WINAMP)						END														**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	ADDON (Combatauras) Sephi																	**/
/*************************************************************************************************/
void CyCombatAuraPythonInterface();
/*************************************************************************************************/
/**	ADDON (Combatauras)						END													**/
/*************************************************************************************************/

//
//
//
DllExport void DLLPublishToPython()
{
	CyEnumsPythonInterface();
	CyGamePythonInterface();
	CyRandomPythonInterface();
	CyTeamPythonInterface();
	CyAreaPythonInterface();
	CyStructsPythonInterface1();
	CyMapPythonInterface();
	CyMapGeneratorPythonInterface();
	CyArtFileMgrPythonInterface();
	CyGameTextMgrInterface();
	CyInfoPythonInterface1();
	CyInfoPythonInterface2();
	CyInfoPythonInterface3();
	CyInfoPythonInterface4();
	CyHallOfFameInterface();
	CyGameCoreUtilsPythonInterface();
	CyMessageControlInterface();

/*************************************************************************************************/
/**	ADDON (WINAMP) merged Sephi																	**/
/*************************************************************************************************/
	CyWinampPythonInterface();
/*************************************************************************************************/
/**	ADDON (WINAMP)						END														**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	ADDON (Combatauras) Sephi																	**/
/*************************************************************************************************/
    CyCombatAuraPythonInterface();
/*************************************************************************************************/
/**	ADDON (Combatauras)						END													**/
/*************************************************************************************************/

	//
	// large interfaces which can be split across files if need be
	//
	python::class_<CyCity> city ("CyCity");		// define city class
	CyCityPythonInterface1(city);				// publish it's methods
	CyCityPythonInterface2(city);				// publish it's methods

	python::class_<CyPlayer> player ("CyPlayer");	// define player class
	CyPlayerPythonInterface1(player);				// publish it's methods
	CyPlayerPythonInterface2(player);				// publish it's methods

	python::class_<CyUnit> unit ("CyUnit");		// define unit class
	CyUnitPythonInterface1(unit);				// publish it's methods

	python::class_<CyPlot> plot ("CyPlot");		// define plot class
	CyPlotPythonInterface1(plot);				// publish it's methods

	python::class_<CyGlobalContext> gc ("CyGlobalContext");	// define globals class
	CyGlobalContextPythonInterface1(gc);					// publish it's methods
	CyGlobalContextPythonInterface2(gc);					// publish it's methods
	CyGlobalContextPythonInterface3(gc);					// publish it's methods
	CyGlobalContextPythonInterface4(gc);					// publish it's methods

	python::class_<CySelectionGroup> selectiongroup ("CySelectionGroup");	// define SelectionGroup class
	CySelectionGroupInterface(selectiongroup);				// publish it's methods
	CySelectionGroupInterface2(selectiongroup);				// publish it's methods
}
