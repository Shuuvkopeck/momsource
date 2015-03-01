// game.cpp

#include "CvGameCoreDLL.h"
#include "CvInitCore.h"
#include "CvDLLPythonIFaceBase.h"
#include "CyArgsList.h"
#include "CvInfos.h"
#include "CvGlobals.h"
#include "CvDLLUtilityIFaceBase.h"
#include "CvGameAI.h"
#include "CvGameCoreUtils.h"
/*************************************************************************************************/
/**	True Modular							10/22/08									Xienwolf	**/
/**																								**/
/**														**/
/*************************************************************************************************/
#include "CvXMLLoadUtility.h"
/*************************************************************************************************/
/**	CivSelector									END												**/
/*************************************************************************************************/

// Public Functions...

CvInitCore::CvInitCore()
{
	m_abOptions = new bool[NUM_GAMEOPTION_TYPES];
	m_abMPOptions = new bool[NUM_MPOPTION_TYPES];
	m_abForceControls = new bool[NUM_FORCECONTROL_TYPES];

	m_aszLeaderName = new CvWString[MAX_PLAYERS];
	m_aszCivDescription = new CvWString[MAX_PLAYERS];
	m_aszCivShortDesc = new CvWString[MAX_PLAYERS];
	m_aszCivAdjective = new CvWString[MAX_PLAYERS];
	m_aszCivPassword = new CvWString[MAX_PLAYERS];
	m_aszEmail = new CvString[MAX_PLAYERS];
	m_aszSmtpHost = new CvString[MAX_PLAYERS];

	m_abWhiteFlag = new bool[MAX_PLAYERS];
	m_aszFlagDecal = new CvWString[MAX_PLAYERS];

	m_aeCiv = new CivilizationTypes[MAX_PLAYERS];
	m_aeLeader = new LeaderHeadTypes[MAX_PLAYERS];
	m_aeTeam = new TeamTypes[MAX_PLAYERS];
	m_aeHandicap = new HandicapTypes[MAX_PLAYERS];
	m_aeColor = new PlayerColorTypes[MAX_PLAYERS];
	m_aeArtStyle = new ArtStyleTypes[MAX_PLAYERS];

	// Slot data
	m_aeSlotStatus = new SlotStatus[MAX_PLAYERS];
	m_aeSlotClaim = new SlotClaim[MAX_PLAYERS];

	// Civ flags
	m_abPlayableCiv = new bool[MAX_PLAYERS];
	m_abMinorNationCiv = new bool[MAX_PLAYERS];

	// Unsaved player data
	m_aiNetID = new int[MAX_PLAYERS];
	m_abReady = new bool[MAX_PLAYERS];

	m_aszPythonCheck = new CvString[MAX_PLAYERS];
	m_aszXMLCheck = new CvString[MAX_PLAYERS];

	m_aeCustomMapOptions = NULL;
	m_abVictories = NULL;

	reset(NO_GAMEMODE);
}


CvInitCore::~CvInitCore()
{
	uninit();

	SAFE_DELETE_ARRAY(m_abOptions);
	SAFE_DELETE_ARRAY(m_abMPOptions);
	SAFE_DELETE_ARRAY(m_abForceControls);
	SAFE_DELETE_ARRAY(m_aszLeaderName);
	SAFE_DELETE_ARRAY(m_aszCivDescription);
	SAFE_DELETE_ARRAY(m_aszCivShortDesc);
	SAFE_DELETE_ARRAY(m_aszCivAdjective);
	SAFE_DELETE_ARRAY(m_aszCivPassword);
	SAFE_DELETE_ARRAY(m_aszEmail);
	SAFE_DELETE_ARRAY(m_aszSmtpHost);
	SAFE_DELETE_ARRAY(m_abWhiteFlag);
	SAFE_DELETE_ARRAY(m_aszFlagDecal);
	SAFE_DELETE_ARRAY(m_aeCiv);
	SAFE_DELETE_ARRAY(m_aeLeader);
	SAFE_DELETE_ARRAY(m_aeTeam);
	SAFE_DELETE_ARRAY(m_aeHandicap);
	SAFE_DELETE_ARRAY(m_aeColor);
	SAFE_DELETE_ARRAY(m_aeArtStyle);
	SAFE_DELETE_ARRAY(m_aeSlotStatus);
	SAFE_DELETE_ARRAY(m_aeSlotClaim);
	SAFE_DELETE_ARRAY(m_abPlayableCiv);
	SAFE_DELETE_ARRAY(m_abMinorNationCiv);
	SAFE_DELETE_ARRAY(m_aiNetID);
	SAFE_DELETE_ARRAY(m_abReady);
	SAFE_DELETE_ARRAY(m_aszPythonCheck);
	SAFE_DELETE_ARRAY(m_aszXMLCheck);
/*************************************************************************************************/
/**	TrueModular								05/26/09	Written: Mr. Genie	Imported: Xienwolf	**/
/**																								**/
/**	Properly links Modular modifications to previous elements, and allows partial overwriting	**/
/*************************************************************************************************/
	m_aszSaveGameVector.clear();
/*************************************************************************************************/
/**	TrueModular								END													**/
/*************************************************************************************************/
}

void CvInitCore::init(GameMode eMode)
{
	//--------------------------------
	// Init saved data
	reset(eMode);
}

void CvInitCore::uninit()
{
	clearCustomMapOptions();
	clearVictories();
}


// FUNCTION: reset()
// Initializes data members that are serialized.
void CvInitCore::reset(GameMode eMode)
{
	//--------------------------------
	// Uninit class
	uninit();

	resetGame();
	resetPlayers();

	setMode(eMode);

	if (getMode() != NO_GAMEMODE)
	{
		setDefaults();
	}
/*************************************************************************************************/
/**	LoadedTGA								05/26/09	Written: Mr. Genie	Imported: Xienwolf	**/
/**	Modifies how the TGA is handled to allow many more religions/corporations/resources easily	**/
/** deleting bogus Corporations and Religions elements from the vectors                          */
/*************************************************************************************************/
	CvXMLLoadUtility* pXMLLoadUtility = new CvXMLLoadUtility;
	pXMLLoadUtility->cleanTGA();
	SAFE_DELETE(pXMLLoadUtility);
/*************************************************************************************************/
/**	LoadedTGA								END													**/
/*************************************************************************************************/
}

void CvInitCore::setDefaults()
{
	for (int i = 0; i < NUM_GAMEOPTION_TYPES; ++i)
	{
		m_abOptions[i] = GC.getGameOptionInfo((GameOptionTypes)i).getDefault();
	}
	for (int i = 0; i < NUM_MPOPTION_TYPES; ++i)
	{
		m_abMPOptions[i] = GC.getMPOptionInfo((MultiplayerOptionTypes)i).getDefault();
	}
}


bool CvInitCore::getHuman(PlayerTypes eID) const
{
	if (getSlotStatus(eID) == SS_TAKEN)
	{
		return true;
	}
	else if (getSlotStatus(eID) == SS_OPEN)
	{
		return ( gDLL->isGameActive() || getHotseat() || getPitboss() || getPbem());
	}
	else
	{
		return false;
	}
}

int CvInitCore::getNumHumans() const
{
	int iNumHumans = 0;
	for (int i = 0; i < MAX_CIV_PLAYERS; ++i)
	{
		if (getHuman((PlayerTypes)i))
		{
			++iNumHumans;
		}
	}
	return iNumHumans;
}

int CvInitCore::getNumDefinedPlayers() const
{
	int iCount = 0;
	for (int i = 0; i < MAX_CIV_PLAYERS; ++i)
	{
		if ((getCiv((PlayerTypes)i) != NO_CIVILIZATION) && (getLeader((PlayerTypes)i) != NO_LEADER))
		{
			iCount++;
		}
	}
	return iCount;
}

bool CvInitCore::getMultiplayer() const
{
	switch (getType())
	{
	case GAME_MP_NEW:
	case GAME_MP_LOAD:
	case GAME_MP_SCENARIO:
		return true;
	default:
		return false;
	}
}

bool CvInitCore::getNewGame() const
{
	switch (getType())
	{
	case GAME_SP_NEW:
	case GAME_SP_SCENARIO:
	case GAME_MP_NEW:
	case GAME_MP_SCENARIO:
	case GAME_HOTSEAT_NEW:
	case GAME_HOTSEAT_SCENARIO:
	case GAME_PBEM_NEW:
	case GAME_PBEM_SCENARIO:
		return true;
	default:
		return false;
	}
}

bool CvInitCore::getSavedGame() const
{
	switch (getType())
	{
	case GAME_SP_LOAD:
	case GAME_MP_LOAD:
	case GAME_HOTSEAT_LOAD:
	case GAME_PBEM_LOAD:
		return true;
	default:
		return false;
	}
}

bool CvInitCore::getPitboss() const
{
	return (getMode() == GAMEMODE_PITBOSS);
}

bool CvInitCore::getHotseat() const
{
	return ( (getType() == GAME_HOTSEAT_NEW) || (getType() == GAME_HOTSEAT_SCENARIO) || (getType() == GAME_HOTSEAT_LOAD) );
}


bool CvInitCore::getPbem() const
{
	return ( (getType() == GAME_PBEM_NEW) || (getType() == GAME_PBEM_SCENARIO) || (getType() == GAME_PBEM_LOAD) );
}


bool CvInitCore::checkBounds( int iValue, int iLower, int iUpper ) const
{
	if (iValue >= iLower)
	{
		if (iValue < iUpper)
		{
			return true;
		}
	}
	return false;
}


bool CvInitCore::getSlotVacant(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_CIV_PLAYERS, eID, "CvInitCore::getSlotVacant");

	bool bRetVal = false;

	if ( checkBounds(eID, 0, MAX_CIV_PLAYERS) )
	{
		bool bTakeoverAI = getMPOption(MPOPTION_TAKEOVER_AI);
		SlotStatus eStatus = getSlotStatus(eID);

		// Check the status of this slot
		if ( (eStatus == SS_OPEN) || (bTakeoverAI && (eStatus == SS_COMPUTER)) )
		{
			bRetVal = ( getSlotClaim(eID) != SLOTCLAIM_ASSIGNED );
		}
	}

	return bRetVal;
}

PlayerTypes CvInitCore::getAvailableSlot()
{
	int i;

	// Get the next ID available ID
	// First check for open slots only
	for (i = 0; i < MAX_CIV_PLAYERS; ++i)
	{
		PlayerTypes eID = (PlayerTypes)i;
		if ( (getSlotClaim(eID) == SLOTCLAIM_UNASSIGNED) && (getSlotStatus(eID) == SS_OPEN) )
		{
			setSlotClaim(eID, SLOTCLAIM_ASSIGNED);
			return eID;
		}
	}

	// That didn't work, check to see if we can assign computer slots
	if (getMPOption(MPOPTION_TAKEOVER_AI))
	{
		for (i = 0; i < MAX_CIV_PLAYERS; ++i)
		{
			PlayerTypes eID = (PlayerTypes)i;
			if ( (getSlotClaim(eID) == SLOTCLAIM_UNASSIGNED) && (getSlotStatus(eID) == SS_COMPUTER) )
			{
				setSlotClaim(eID, SLOTCLAIM_ASSIGNED);
				return eID;
			}
		}
	}

	// None available at all...
	return NO_PLAYER;
}

void CvInitCore::reassignPlayer(PlayerTypes eOldID, PlayerTypes eNewID)
{
	FASSERT_BOUNDS(0, MAX_CIV_PLAYERS, eOldID, "CvInitCore::reassignPlayer (eOldID)");
	FASSERT_BOUNDS(0, MAX_CIV_PLAYERS, eNewID, "CvInitCore::reassignPlayer (eNewID)");

	if ( checkBounds(eOldID, 0, MAX_CIV_PLAYERS) && checkBounds(eNewID, 0, MAX_CIV_PLAYERS) )
	{
		// *** SAVE TARGET SLOT DETAILS TEMPORARILY
		// Temp civ details
		CvWString szLeaderName = m_aszLeaderName[eNewID];
		CvWString szCivDescription = m_aszCivDescription[eNewID];
		CvWString szCivShortDesc = m_aszCivShortDesc[eNewID];
		CvWString szCivAdjective = m_aszCivAdjective[eNewID];
		CvWString szCivPassword = m_aszCivPassword[eNewID];
		CvString szEmail = m_aszEmail[eNewID];
		CvString szSmtpHost = m_aszSmtpHost[eNewID];
		bool bWhiteFlag = m_abWhiteFlag[eNewID];
		CvWString szFlagDecal = m_aszFlagDecal[eNewID];
		CivilizationTypes eCiv = m_aeCiv[eNewID];
		LeaderHeadTypes eLeader = m_aeLeader[eNewID];
		TeamTypes eTeam = m_aeTeam[eNewID];
		HandicapTypes eHandicap = m_aeHandicap[eNewID];
		PlayerColorTypes eColor = m_aeColor[eNewID];
		ArtStyleTypes eArtStyle = m_aeArtStyle[eNewID];
		// Temp slot data
		SlotStatus eSlotStatus = m_aeSlotStatus[eNewID];
		SlotClaim eSlotClaim = m_aeSlotClaim[eNewID];
		// Temp civ flags
		bool bPlayableCiv = m_abPlayableCiv[eNewID];
		bool bMinorNationCiv = m_abMinorNationCiv[eNewID];
		// Temp unsaved player data
		int iNetID = m_aiNetID[eNewID];
		bool bReady = m_abReady[eNewID];
		CvString szPythonCheck = m_aszPythonCheck[eNewID];
		CvString szXMLCheck = m_aszXMLCheck[eNewID];

		// *** SAVE OLD SLOT DETAILS IN NEW SLOT
		// New civ details
		m_aszLeaderName[eNewID] = m_aszLeaderName[eOldID];
		m_aszCivDescription[eNewID] = m_aszCivDescription[eOldID];
		m_aszCivShortDesc[eNewID] = m_aszCivShortDesc[eOldID];
		m_aszCivAdjective[eNewID] = m_aszCivAdjective[eOldID];
		m_aszCivPassword[eNewID] = m_aszCivPassword[eOldID];
		m_aszEmail[eNewID] = m_aszEmail[eOldID];
		m_aszSmtpHost[eNewID] = m_aszSmtpHost[eOldID];
		m_abWhiteFlag[eNewID] = m_abWhiteFlag[eOldID];
		m_aszFlagDecal[eNewID] = m_aszFlagDecal[eOldID];
		m_aeCiv[eNewID] = m_aeCiv[eOldID];
		m_aeLeader[eNewID] = m_aeLeader[eOldID];
		m_aeTeam[eNewID] = m_aeTeam[eOldID];
		m_aeHandicap[eNewID] = m_aeHandicap[eOldID];
		m_aeColor[eNewID] = m_aeColor[eOldID];
		m_aeArtStyle[eNewID] = m_aeArtStyle[eOldID];
		// New slot data
		m_aeSlotStatus[eNewID] = m_aeSlotStatus[eOldID];
		m_aeSlotClaim[eNewID] = m_aeSlotClaim[eOldID];
		// New civ flags
		m_abPlayableCiv[eNewID] = m_abPlayableCiv[eOldID];
		m_abMinorNationCiv[eNewID] = m_abMinorNationCiv[eOldID];
		// New unsaved player data
		m_aiNetID[eNewID] = m_aiNetID[eOldID];
		m_abReady[eNewID] = m_abReady[eOldID];
		m_aszPythonCheck[eNewID] = m_aszPythonCheck[eOldID];
		m_aszXMLCheck[eNewID] = m_aszXMLCheck[eOldID];

		// *** SAVE TEMP DETAILS IN OLD SLOT
		// New civ details
		m_aszLeaderName[eOldID] = szLeaderName;
		m_aszCivDescription[eOldID] = szCivDescription;
		m_aszCivShortDesc[eOldID] = szCivShortDesc;
		m_aszCivAdjective[eOldID] = szCivAdjective;
		m_aszCivPassword[eOldID] = szCivPassword;
		m_aszEmail[eOldID] = szEmail;
		m_aszSmtpHost[eOldID] = szSmtpHost;
		m_abWhiteFlag[eOldID] = bWhiteFlag;
		m_aszFlagDecal[eOldID] = szFlagDecal;
		m_aeCiv[eOldID] = eCiv;
		m_aeLeader[eOldID] = eLeader;
		m_aeTeam[eOldID] = eTeam;
		m_aeHandicap[eOldID] = eHandicap;
		m_aeColor[eOldID] = eColor;
		m_aeArtStyle[eOldID] = eArtStyle;
		// New slot data
		m_aeSlotStatus[eOldID] = eSlotStatus;
		m_aeSlotClaim[eOldID] = eSlotClaim;
		// New civ flags
		m_abPlayableCiv[eOldID] = bPlayableCiv;
		m_abMinorNationCiv[eOldID] = bMinorNationCiv;
		// New unsaved player data
		m_aiNetID[eOldID] = iNetID;
		m_abReady[eOldID] = bReady;
		m_aszPythonCheck[eOldID] = szPythonCheck;
		m_aszXMLCheck[eOldID] = szXMLCheck;

		// We may have a new active player id...
		if (getActivePlayer() == eOldID)
		{
			setActivePlayer(eNewID);
		}
		else if (getActivePlayer() == eNewID)
		{
			setActivePlayer(eOldID);
		}

		if(CvPlayerAI::areStaticsInitialized())
		{
			GET_PLAYER(eOldID).updateTeamType();
			GET_PLAYER(eNewID).updateTeamType();
			GET_PLAYER(eOldID).updateHuman();
			GET_PLAYER(eNewID).updateHuman();
		}
	}
}

//FfH: Added by Kael 04/08/2008
void CvInitCore::reassignPlayerAdvanced(PlayerTypes eOldID, PlayerTypes eNewID, int iTimer)
{
	if ( checkBounds(eOldID, 0, MAX_CIV_PLAYERS) && checkBounds(eNewID, 0, MAX_CIV_PLAYERS) )
	{
		// *** SAVE TARGET SLOT DETAILS TEMPORARILY
		// Temp civ details
		CvString szEmail = m_aszEmail[eNewID];
		CvString szSmtpHost = m_aszSmtpHost[eNewID];
		HandicapTypes eHandicap = m_aeHandicap[eNewID];
		SlotStatus eSlotStatus = m_aeSlotStatus[eNewID];
		SlotClaim eSlotClaim = m_aeSlotClaim[eNewID];
		bool bPlayableCiv = m_abPlayableCiv[eNewID];
		bool bMinorNationCiv = m_abMinorNationCiv[eNewID];
		int iNetID = m_aiNetID[eNewID];
		bool bReady = m_abReady[eNewID];
		CvString szPythonCheck = m_aszPythonCheck[eNewID];
		CvString szXMLCheck = m_aszXMLCheck[eNewID];

		m_aszEmail[eNewID] = m_aszEmail[eOldID];
		m_aszSmtpHost[eNewID] = m_aszSmtpHost[eOldID];
		m_aeHandicap[eNewID] = m_aeHandicap[eOldID];
        setSlotStatus(eNewID, m_aeSlotStatus[eOldID]);
		m_aeSlotClaim[eNewID] = m_aeSlotClaim[eOldID];
		m_abPlayableCiv[eNewID] = m_abPlayableCiv[eOldID];
		m_abMinorNationCiv[eNewID] = m_abMinorNationCiv[eOldID];
		m_aiNetID[eNewID] = m_aiNetID[eOldID];
		m_abReady[eNewID] = m_abReady[eOldID];
		m_aszPythonCheck[eNewID] = m_aszPythonCheck[eOldID];
		m_aszXMLCheck[eNewID] = m_aszXMLCheck[eOldID];

		m_aszEmail[eOldID] = szEmail;
		m_aszSmtpHost[eOldID] = szSmtpHost;
		m_aeHandicap[eOldID] = eHandicap;
        setSlotStatus(eOldID, eSlotStatus);
		m_aeSlotClaim[eOldID] = eSlotClaim;
		m_abPlayableCiv[eOldID] = bPlayableCiv;
		m_abMinorNationCiv[eOldID] = bMinorNationCiv;
		m_aiNetID[eOldID] = iNetID;
		m_abReady[eOldID] = bReady;
		m_aszPythonCheck[eOldID] = szPythonCheck;
		m_aszXMLCheck[eOldID] = szXMLCheck;

        for (int iI = 0; iI < NUM_PLAYEROPTION_TYPES; iI++)
        {
            GET_PLAYER(eNewID).setOption((PlayerOptionTypes)iI,GET_PLAYER(eOldID).isOption((PlayerOptionTypes)iI));
            GET_PLAYER(eOldID).setOption((PlayerOptionTypes)iI,false);
        }

		// We may have a new active player id...
		if (getActivePlayer() == eOldID)
		{
		    GC.getGameINLINE().setActivePlayer(eNewID);
		}
		else if (getActivePlayer() == eNewID)
		{
			setActivePlayer(eOldID);
		}

		if(CvPlayerAI::areStaticsInitialized())
		{
			GET_PLAYER(eOldID).updateTeamType();
			GET_PLAYER(eNewID).updateTeamType();
			GET_PLAYER(eOldID).updateHuman();
			GET_PLAYER(eNewID).updateHuman();
		}

		if (iTimer != -1)
		{
		    GET_PLAYER(eNewID).changeTempPlayerTimer(iTimer);
		    GET_PLAYER(eNewID).setRealPlayer(eOldID);
		}
	}
}
//FfH: End Add

void CvInitCore::closeInactiveSlots()
{
	// Open inactive slots mean different things to different game modes and types...
	// Let's figure out what they mean for us

	for (int i = 0; i < MAX_CIV_PLAYERS; i++)
	{
		PlayerTypes eID = (PlayerTypes)i;
		if (getSlotStatus(eID) == SS_OPEN)
		{
			if (getPitboss() || getHotseat() || getPbem())
			{
				// Pitboss & hotseat - all "open" slots are non-present human players
				setSlotStatus(eID, SS_TAKEN);
			}
			else if (getType() == GAME_MP_SCENARIO)
			{
				// Multiplayer scenario - all "open" slots should be filled with an AI player
				setSlotStatus(eID, SS_COMPUTER);
			}
			else
			{
				// If it's a normal game, all "open" slots should be closed.
				setSlotStatus(eID, SS_CLOSED);
			}
			setSlotClaim(eID, SLOTCLAIM_UNASSIGNED);

			gDLL->sendPlayerInfo(eID);
		}
	}
}

void CvInitCore::reopenInactiveSlots()
{
	// "Inactive" open slots will only be in Pitboss and Hotseat
	if (getPitboss() || getHotseat() || getPbem())
	{
		for (int i = 0; i < MAX_CIV_PLAYERS; ++i)
		{
			PlayerTypes eID = (PlayerTypes)i;
			// Reopen all slots that don't have active connections
			if (getSlotStatus(eID) == SS_TAKEN)
			{
				if ( getSlotClaim(eID) != SLOTCLAIM_ASSIGNED )
				{
					setSlotStatus(eID, SS_OPEN);
				}
			}
		}
	}
}

void CvInitCore::resetGame()
{
	// Descriptive strings about game and map
	m_eType = GAME_NONE;
	m_szGameName.clear();
	m_szGamePassword.clear();
	m_szAdminPassword.clear();
	m_szMapScriptName.clear();

	m_bWBMapNoPlayers = false;

	// Standard game parameters
	m_eWorldSize = NO_WORLDSIZE;											// STANDARD_ option?
	m_eClimate = (ClimateTypes)GC.getDefineINT("STANDARD_CLIMATE");			// NO_ option?
	m_eSeaLevel = (SeaLevelTypes)GC.getDefineINT("STANDARD_SEALEVEL");		// NO_ option?
	m_eEra = (EraTypes)GC.getDefineINT("STANDARD_ERA");						// NO_ option?
	m_eGameSpeed = (GameSpeedTypes)GC.getDefineINT("STANDARD_GAMESPEED");	// NO_ option?
	m_eTurnTimer = (TurnTimerTypes)GC.getDefineINT("STANDARD_TURNTIMER");	// NO_ option?
	m_eCalendar = (CalendarTypes)GC.getDefineINT("STANDARD_CALENDAR");		// NO_ option?

	// Map-specific custom parameters
	clearCustomMapOptions();

	// Data-defined victory conditions
	refreshVictories();


	// Standard game options
	int i;
	for (i = 0; i < NUM_GAMEOPTION_TYPES; ++i)
	{
		m_abOptions[i] = false;
	}
	for (i = 0; i < NUM_MPOPTION_TYPES; ++i)
	{
		m_abMPOptions[i] = false;
	}
	m_bStatReporting = false;

	for (i = 0; i < NUM_FORCECONTROL_TYPES; ++i)
	{
		m_abForceControls[i] = false;
	}

	// Game turn mgmt
	m_iGameTurn = 0;
	m_iMaxTurns = 0;
	m_iPitbossTurnTime = 0;
	m_iTargetScore = 0;

	// City Elimination
	m_iMaxCityElimination = 0;

	m_iNumAdvancedStartPoints = 0;

	// Unsaved game data
	m_uiSyncRandSeed = 0;
	m_uiMapRandSeed = 0;
	m_eActivePlayer = NO_PLAYER;

	// Temp vars
	m_szTemp.clear();
}

void CvInitCore::resetGame(CvInitCore * pSource, bool bClear, bool bSaveGameType)
{
	FAssertMsg(pSource, "Passed null pointer to CvInitCore::resetGame");
	FAssertMsg(!bClear || !bSaveGameType, "Should not be clearing data while trying to preserve gametype info in CvInitCore::resetGame");

	if (bClear || !pSource)
	{
		resetGame();
	}
	if (pSource)
	{
/*************************************************************************************************/
/**	TrueModular								05/26/09	Written: Mr. Genie	Imported: Xienwolf	**/
/**	LoadedTGA								05/26/09	Written: Mr. Genie	Imported: Xienwolf	**/
/**	Haven't looked over all of the savegame data bits so far, I assume nothing should change	**/
/**	Properly links Modular modifications to previous elements, and allows partial overwriting	**/
/*************************************************************************************************/
		if (!gDLL->isModularXMLLoading())
		{
			if (bSaveGameType)
			{
				// if the size is not the same(easy check) we gotta reload 100% anyway
				if (pSource->getNumSaveGameVector() != GC.getModLoadControlVectorSize() )
				{
					for ( int i = 0; i < pSource->getNumSaveGameVector(); i++ )
					{
						m_aszSaveGameVector.push_back(pSource->getSaveGameVector(i));
					}
					doReloadInfoClasses();
				}
				// if the size is the same, check if the strings equal
				if (pSource->getNumSaveGameVector() == GC.getModLoadControlVectorSize() )
				{
					for ( int i = 0; i < pSource->getNumSaveGameVector(); i++ )
					{
						//if the strings don't equal, we gotta reload(obviously)
						if ( pSource->getSaveGameVector(i) != GC.getModLoadControlVector(i) )
						{
							m_aszSaveGameVector.clear();
							for ( int i = 0; i < pSource->getNumSaveGameVector(); i++ )
							{
								m_aszSaveGameVector.push_back(pSource->getSaveGameVector(i));
							}
							doReloadInfoClasses();
						}
						break;
					}
				}
			}
		}
/*************************************************************************************************/
/**	TrueModular								END													**/
/*************************************************************************************************/
		// Only copy over saved data

		// Descriptive strings about game and map
		if (!bSaveGameType || getGameMultiplayer() != pSource->getGameMultiplayer())
		{
			setType(pSource->getType());
		}
		setGameName(pSource->getGameName());
		setGamePassword(pSource->getGamePassword());
		setAdminPassword(pSource->getAdminPassword(), false);
		setMapScriptName(pSource->getMapScriptName());

		setWBMapNoPlayers(pSource->getWBMapNoPlayers());

		// Standard game parameters
		setWorldSize(pSource->getWorldSize());
		setClimate(pSource->getClimate());
		setSeaLevel(pSource->getSeaLevel());
		setEra(pSource->getEra());
		setGameSpeed(pSource->getGameSpeed());
		setTurnTimer(pSource->getTurnTimer());
		setCalendar(pSource->getCalendar());

		// Map-specific custom parameters
		setCustomMapOptions(pSource->getNumCustomMapOptions(), pSource->getCustomMapOptions());
		m_iNumHiddenCustomMapOptions = pSource->getNumHiddenCustomMapOptions();
		setVictories(pSource->getNumVictories(), pSource->getVictories());

		// Standard game options
		int i;
		for (i = 0; i < NUM_GAMEOPTION_TYPES; ++i)
		{
			setOption((GameOptionTypes)i, pSource->getOption((GameOptionTypes)i));
		}

		for (i = 0; i < NUM_MPOPTION_TYPES; ++i)
		{
			setMPOption((MultiplayerOptionTypes)i, pSource->getMPOption((MultiplayerOptionTypes)i));
		}
		setStatReporting(pSource->getStatReporting());

		// Game turn mgmt
		setGameTurn(pSource->getGameTurn());
		setMaxTurns(pSource->getMaxTurns());
		setPitbossTurnTime(pSource->getPitbossTurnTime());
		setTargetScore(pSource->getTargetScore());

		// City Elimination
		setMaxCityElimination(pSource->getMaxCityElimination());

		setNumAdvancedStartPoints(pSource->getNumAdvancedStartPoints());

		setSyncRandSeed(pSource->getSyncRandSeed());
		setMapRandSeed(pSource->getMapRandSeed());
	}
}

void CvInitCore::resetPlayers()
{
	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		resetPlayer((PlayerTypes)i);
	}
}

void CvInitCore::resetPlayers(CvInitCore * pSource, bool bClear, bool bSaveSlotInfo)
{
	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		resetPlayer((PlayerTypes)i, pSource, bClear, bSaveSlotInfo);
	}
}

void CvInitCore::resetPlayer(PlayerTypes eID)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::resetPlayer");

	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		// Only copy over saved data

		// Civ details
		m_aszLeaderName[eID].clear();
		m_aszCivDescription[eID].clear();
		m_aszCivShortDesc[eID].clear();
		m_aszCivAdjective[eID].clear();
		m_aszCivPassword[eID].clear();
		m_aszEmail[eID].clear();
		m_aszSmtpHost[eID].clear();

		m_abWhiteFlag[eID] = false;
		m_aszFlagDecal[eID].clear();

		m_aeCiv[eID] = NO_CIVILIZATION;
		m_aeLeader[eID] = NO_LEADER;
		m_aeTeam[eID] = (TeamTypes)eID;
		m_aeHandicap[eID] = (HandicapTypes)GC.getDefineINT("STANDARD_HANDICAP");
		m_aeColor[eID] = NO_PLAYERCOLOR;
		m_aeArtStyle[eID] = NO_ARTSTYLE;


		// Slot data
		m_aeSlotStatus[eID] = SS_CLOSED;
		m_aeSlotClaim[eID] = SLOTCLAIM_UNASSIGNED;

		// Civ flags
		m_abPlayableCiv[eID] = false;
		m_abMinorNationCiv[eID] = false;

		// Unsaved player data
		m_aiNetID[eID] = -1;
		m_abReady[eID] = false;
		m_aszPythonCheck[eID].clear();
		m_aszXMLCheck[eID].clear();

		if(CvPlayerAI::areStaticsInitialized())
		{
			GET_PLAYER(eID).updateTeamType();
			GET_PLAYER(eID).updateHuman();
		}
	}
}

void CvInitCore::resetPlayer(PlayerTypes eID, CvInitCore * pSource, bool bClear, bool bSaveSlotInfo)
{
	FAssertMsg(pSource, "Passed null pointer to CvInitCore::resetPlayer");
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::resetPlayer");

	FAssertMsg(!bClear || !bSaveSlotInfo, "Should not be clearing data while trying to preserve slot info in CvInitCore::resetPlayer");

	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		if (bClear || !pSource)
		{
			resetPlayer(eID);
		}
		if (pSource)
		{
			// Civ details
			setCivDescription(eID, pSource->getCivDescription(eID));
			setCivShortDesc(eID, pSource->getCivShortDesc(eID));
			setCivAdjective(eID, pSource->getCivAdjective(eID));

			setCivPassword(eID, pSource->getCivPassword(eID), false);
			setEmail(eID, pSource->getEmail(eID));
			setSmtpHost(eID, pSource->getSmtpHost(eID));
			setFlagDecal(eID, pSource->getFlagDecal(eID));
			setWhiteFlag(eID, pSource->getWhiteFlag(eID));

			setHandicap(eID, pSource->getHandicap(eID));
			setCiv(eID, pSource->getCiv(eID));
			setTeam(eID, pSource->getTeam(eID));
			setLeader(eID, pSource->getLeader(eID));
			setColor(eID, pSource->getColor(eID));
			setArtStyle(eID, pSource->getArtStyle(eID));

			setPlayableCiv(eID, pSource->getPlayableCiv(eID));
			setMinorNationCiv(eID, pSource->getMinorNationCiv(eID));

			// Slot data
			if (!bSaveSlotInfo)
			{
				// We don't wanna reset the slot data if we are loading a game
				// from init - we want to keep the changes we made during init
				setLeaderName(eID, pSource->getLeaderName(eID));
				setSlotStatus(eID, pSource->getSlotStatus(eID));
				setSlotClaim(eID, pSource->getSlotClaim(eID));
			}
		}
	}
}


CvWString CvInitCore::getMapScriptName() const
{
	if (gDLL->getTransferredMap())
	{
		if (!getWBMapScript())
		{
			// If it's a transferred Python file, we have to hack in the transferred extension
			return ( m_szMapScriptName + CvWString(MAP_TRANSFER_EXT) );
		}
	}
	return m_szMapScriptName;
}

void CvInitCore::setMapScriptName(const CvWString & szMapScriptName)
{
	m_szMapScriptName = szMapScriptName;
	refreshCustomMapOptions();
}

bool CvInitCore::getWBMapScript() const
{
	return (gDLL->isDescFileName( CvString(m_szMapScriptName).GetCString() ));
}


void CvInitCore::setWorldSize(const CvWString & szWorldSize)
{
	for (int i = 0; i < GC.getNumWorldInfos(); i++)
	{
		if ( wcsicmp(szWorldSize.GetCString(), CvWString(GC.getWorldInfo((WorldSizeTypes)i).getType()).GetCString()) == 0 )
		{
			setWorldSize((WorldSizeTypes)i);
		}
	}
}

const CvWString & CvInitCore::getWorldSizeKey(CvWString & szBuffer) const
{
	if ( checkBounds(getWorldSize(), 0, GC.getNumWorldInfos()) )
	{
		szBuffer = GC.getWorldInfo(getWorldSize()).getType();
		return szBuffer;
	}
	else
	{
		szBuffer = L"NO_WORLDSIZE";
		return szBuffer;
	}
}

void CvInitCore::setClimate(const CvWString & szClimate)
{
	for (int i = 0; i < GC.getNumClimateInfos(); i++)
	{
		if ( wcsicmp(szClimate.GetCString(), CvWString(GC.getClimateInfo((ClimateTypes)i).getType()).GetCString()) == 0 )
		{
			setClimate((ClimateTypes)i);
		}
	}
}

const CvWString & CvInitCore::getClimateKey(CvWString & szBuffer) const
{
	if ( checkBounds(getClimate(), 0, GC.getNumClimateInfos()) )
	{
		szBuffer = GC.getClimateInfo(getClimate()).getType();
		return szBuffer;
	}
	else
	{
		szBuffer = L"NO_CLIMATE";
		return szBuffer;
	}
}

void CvInitCore::setSeaLevel(const CvWString & szSeaLevel)
{
	for (int i = 0; i < GC.getNumSeaLevelInfos(); i++)
	{
		if ( wcsicmp(szSeaLevel.GetCString(), CvWString(GC.getSeaLevelInfo((SeaLevelTypes)i).getType()).GetCString()) == 0 )
		{
			setSeaLevel((SeaLevelTypes)i);
		}
	}
}

const CvWString & CvInitCore::getSeaLevelKey(CvWString & szBuffer) const
{
	if ( checkBounds(getSeaLevel(), 0, GC.getNumSeaLevelInfos()) )
	{
		szBuffer = GC.getSeaLevelInfo(getSeaLevel()).getType();
		return szBuffer;
	}
	else
	{
		szBuffer = L"NO_SEALEVEL";
		return szBuffer;
	}
}

void CvInitCore::setEra(const CvWString & szEra)
{
	for (int i = 0; i < GC.getNumEraInfos(); i++)
	{
		if ( wcsicmp(szEra.GetCString(), CvWString(GC.getEraInfo((EraTypes)i).getType()).GetCString()) == 0 )
		{
			setEra((EraTypes)i);
		}
	}
}

const CvWString & CvInitCore::getEraKey(CvWString & szBuffer) const
{
	if ( checkBounds(getEra(), 0, GC.getNumEraInfos()) )
	{
		szBuffer = GC.getEraInfo(getEra()).getType();
		return szBuffer;
	}
	else
	{
		szBuffer = L"NO_ERA";
		return szBuffer;
	}
}

void CvInitCore::setGameSpeed(const CvWString & szGameSpeed)
{
	for (int i = 0; i < GC.getNumGameSpeedInfos(); i++)
	{
		if ( wcsicmp(szGameSpeed.GetCString(), CvWString(GC.getGameSpeedInfo((GameSpeedTypes)i).getType()).GetCString()) == 0 )
		{
			setGameSpeed((GameSpeedTypes)i);
		}
	}
}

const CvWString & CvInitCore::getGameSpeedKey(CvWString & szBuffer) const
{
	if ( checkBounds(getGameSpeed(), 0, GC.getNumGameSpeedInfos()) )
	{
		szBuffer = GC.getGameSpeedInfo(getGameSpeed()).getType();
		return szBuffer;
	}
	else
	{
		szBuffer = L"NO_GAMESPEED";
		return szBuffer;
	}
}

void CvInitCore::setTurnTimer(const CvWString & szTurnTimer)
{
	for (int i = 0; i < GC.getNumTurnTimerInfos(); i++)
	{
		if ( wcsicmp(szTurnTimer.GetCString(), CvWString(GC.getTurnTimerInfo((TurnTimerTypes)i).getType()).GetCString()) == 0 )
		{
			setTurnTimer((TurnTimerTypes)i);
		}
	}
}

const CvWString & CvInitCore::getTurnTimerKey(CvWString & szBuffer) const
{
	if ( checkBounds(getTurnTimer(), 0, GC.getNumTurnTimerInfos()) )
	{
		szBuffer = GC.getTurnTimerInfo(getTurnTimer()).getType();
		return szBuffer;
	}
	else
	{
		szBuffer = L"NO_TURNTIMER";
		return szBuffer;
	}
}

void CvInitCore::setCalendar(const CvWString & szCalendar)
{
	for (int i = 0; i < GC.getNumCalendarInfos(); i++)
	{
		if ( wcsicmp(szCalendar.GetCString(), CvWString(GC.getCalendarInfo((CalendarTypes)i).getType()).GetCString()) == 0 )
		{
			setCalendar((CalendarTypes)i);
		}
	}
}

const CvWString & CvInitCore::getCalendarKey(CvWString & szBuffer) const
{
	if ( checkBounds(getCalendar(), 0, GC.getNumCalendarInfos()) )
	{
		szBuffer = GC.getCalendarInfo(getCalendar()).getType();
		return szBuffer;
	}
	else
	{
		szBuffer = L"NO_CALENDAR";
		return szBuffer;
	}
}


void CvInitCore::clearCustomMapOptions()
{
	SAFE_DELETE_ARRAY(m_aeCustomMapOptions);
	m_iNumCustomMapOptions = 0;
}

void CvInitCore::refreshCustomMapOptions()
{
	clearCustomMapOptions();

	if ( !getWBMapScript() )
	{
		if ( gDLL->pythonMapExists(CvString(getMapScriptName()).GetCString()) )
		{
			bool bOK;
			long iNumOptions = 0;

			gDLL->getPythonIFace()->callFunction(CvString(getMapScriptName()).GetCString(), "getNumHiddenCustomMapOptions", NULL, &iNumOptions);
			m_iNumHiddenCustomMapOptions = iNumOptions;

			bOK = gDLL->getPythonIFace()->callFunction(CvString(getMapScriptName()).GetCString(), "getNumCustomMapOptions", NULL, &iNumOptions);
			if (bOK)
			{
				// Got number of custom map options - now get the option defaults
				CustomMapOptionTypes *aeMapOptions = new CustomMapOptionTypes[iNumOptions];
				for (int i = 0; i < iNumOptions; ++i)
				{
					long iOptionDefault = 0;
					CyArgsList argsList;
					argsList.add(i);

					bOK = gDLL->getPythonIFace()->callFunction(CvString(getMapScriptName()).GetCString(), "getCustomMapOptionDefault", argsList.makeFunctionArgs(), &iOptionDefault);
					FAssertMsg(bOK, "Call to python fn \"getCustomMapOptionDefault\" failed in CvInitCore::refreshCustomMapOptions");
					if (bOK)
					{
						aeMapOptions[i] = (CustomMapOptionTypes)iOptionDefault;
					}
					else
					{
						aeMapOptions[i] = NO_CUSTOM_MAPOPTION;
					}
				}

				setCustomMapOptions(iNumOptions, aeMapOptions);
				SAFE_DELETE_ARRAY(aeMapOptions);
			}
		}
	}
}


void CvInitCore::clearVictories()
{
	SAFE_DELETE_ARRAY(m_abVictories);
	m_iNumVictories = 0;
}

void CvInitCore::refreshVictories()
{
	clearVictories();

	m_iNumVictories = GC.getNumVictoryInfos();
	if (m_iNumVictories > 0)
	{
		m_abVictories = new bool[m_iNumVictories];
		for (int i = 0; i < m_iNumVictories; ++i)
		{
			m_abVictories[i] = true;
		}
	}
}

void CvInitCore::setCustomMapOptions(int iNumCustomMapOptions, const CustomMapOptionTypes * aeCustomMapOptions)
{
	clearCustomMapOptions();
	if (iNumCustomMapOptions)
	{
		FAssertMsg(aeCustomMapOptions, "CustomMap Num/Pointer mismatch in CvInitCore::setCustomMapOptions");
		m_iNumCustomMapOptions = iNumCustomMapOptions;

		m_aeCustomMapOptions = new CustomMapOptionTypes[m_iNumCustomMapOptions];
		for (int i = 0; i < m_iNumCustomMapOptions; ++i)
		{
			m_aeCustomMapOptions[i] = aeCustomMapOptions[i];
		}
	}
}

CustomMapOptionTypes CvInitCore::getCustomMapOption(int iOptionID) const
{
	FASSERT_BOUNDS(0, m_iNumCustomMapOptions, iOptionID, "CvInitCore::getCustomMapOptions");
	if ( checkBounds(iOptionID, 0, m_iNumCustomMapOptions) )
	{
		return m_aeCustomMapOptions[iOptionID];
	}
	else
	{
		return NO_CUSTOM_MAPOPTION;
	}
}

void CvInitCore::setCustomMapOption(int iOptionID, CustomMapOptionTypes eCustomMapOption)
{
	FASSERT_BOUNDS(0, m_iNumCustomMapOptions, iOptionID, "CvInitCore::setCustomMapOption");
	if ( checkBounds(iOptionID, 0, m_iNumCustomMapOptions) )
	{
		m_aeCustomMapOptions[iOptionID] = eCustomMapOption;
	}
}


void CvInitCore::setVictories(int iNumVictories, const bool * abVictories)
{
	clearVictories();
	if (iNumVictories)
	{
		FAssertMsg(abVictories, "Victory Num/Pointer mismatch in CvInitCore::setVictories");
		m_iNumVictories = iNumVictories;

		m_abVictories = new bool[m_iNumVictories];
		for (int i = 0; i < m_iNumVictories; ++i)
		{
			m_abVictories[i] = abVictories[i];
		}
	}
}

bool CvInitCore::getVictory(VictoryTypes eVictoryID) const
{
	FASSERT_BOUNDS(0, m_iNumVictories, eVictoryID, "CvInitCore::getVictory");
	if ( checkBounds(eVictoryID, 0, m_iNumVictories) )
	{
		return m_abVictories[eVictoryID];
	}
	else
	{
		return false;
	}
}

void CvInitCore::setVictory(VictoryTypes eVictoryID, bool bVictory)
{
	FASSERT_BOUNDS(0, m_iNumVictories, eVictoryID, "CvInitCore::setVictory");
	if ( checkBounds(eVictoryID, 0, m_iNumVictories) )
	{
		m_abVictories[eVictoryID] = bVictory;
	}
}


bool CvInitCore::getOption(GameOptionTypes eIndex) const
{
	FASSERT_BOUNDS(0, NUM_GAMEOPTION_TYPES, eIndex, "CvInitCore::getOption");
	if ( checkBounds(eIndex, 0, NUM_GAMEOPTION_TYPES) )
	{
		return m_abOptions[eIndex];
	}
	else
	{
		return false;
	}
}

void CvInitCore::setOption(GameOptionTypes eIndex, bool bOption)
{
	FASSERT_BOUNDS(0, NUM_GAMEOPTION_TYPES, eIndex, "CvInitCore::setOption");
	if ( checkBounds(eIndex, 0, NUM_GAMEOPTION_TYPES) )
	{
		m_abOptions[eIndex] = bOption;
	}
}

bool CvInitCore::getMPOption(MultiplayerOptionTypes eIndex) const
{
	FASSERT_BOUNDS(0, NUM_MPOPTION_TYPES, eIndex, "CvInitCore::getMPOption");
	if ( checkBounds(eIndex, 0, NUM_MPOPTION_TYPES) )
	{
		return m_abMPOptions[eIndex];
	}
	else
	{
		return false;
	}
}

void CvInitCore::setMPOption(MultiplayerOptionTypes eIndex, bool bOption)
{
	FASSERT_BOUNDS(0, NUM_MPOPTION_TYPES, eIndex, "CvInitCore::setMPOption");
	if ( checkBounds(eIndex, 0, NUM_MPOPTION_TYPES) )
	{
		m_abMPOptions[eIndex] = bOption;
	}
}

bool CvInitCore::getForceControl(ForceControlTypes eIndex) const
{
	FASSERT_BOUNDS(0, NUM_FORCECONTROL_TYPES, eIndex, "CvInitCore::getForceControl");
	if ( checkBounds(eIndex, 0, NUM_FORCECONTROL_TYPES) )
	{
		return m_abForceControls[eIndex];
	}
	else
	{
		return false;
	}
}

void CvInitCore::setForceControl(ForceControlTypes eIndex, bool bOption)
{
	FASSERT_BOUNDS(0, NUM_FORCECONTROL_TYPES, eIndex, "CvInitCore::setForceControl");
	if ( checkBounds(eIndex, 0, NUM_FORCECONTROL_TYPES) )
	{
		m_abForceControls[eIndex] = bOption;
	}
}


void CvInitCore::setActivePlayer(PlayerTypes eActivePlayer)
{
	m_eActivePlayer = eActivePlayer;

	if (m_eActivePlayer != NO_PLAYER)
	{
		// Automatically claim this slot
		setSlotClaim(m_eActivePlayer, SLOTCLAIM_ASSIGNED);
	}
}

void CvInitCore::setType(GameType eType)
{
	if (getType() != eType)
	{
		m_eType = eType;

		if(CvPlayerAI::areStaticsInitialized())
		{
			for (int i = 0; i < MAX_PLAYERS; ++i)
			{
				GET_PLAYER((PlayerTypes)i).updateHuman();
			}
		}
	}
}

void CvInitCore::setType(const CvWString & szType)
{
	if (wcsicmp(szType.GetCString(), L"singleplayer") == 0)
	{
		setType(GAME_SP_NEW);
	}
	else if (wcsicmp(szType.GetCString(), L"spload") == 0)
	{
		setType(GAME_SP_LOAD);
	}
	else
	{
		//FAssertMsg(false, "Invalid game type in ini file!");
		setType(GAME_NONE);
	}
}

void CvInitCore::setMode(GameMode eMode)
{
	if (getMode() != eMode)
	{
		m_eMode = eMode;

		if(CvPlayerAI::areStaticsInitialized())
		{
			for (int i = 0; i < MAX_PLAYERS; ++i)
			{
				GET_PLAYER((PlayerTypes)i).updateHuman();
			}
		}
	}
}


const CvWString & CvInitCore::getLeaderName(PlayerTypes eID, uint uiForm) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getLeaderName");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_szTemp = gDLL->getObjectText(CvString(m_aszLeaderName[eID]).GetCString(), uiForm, true);
	}
	else
	{
		m_szTemp = "";
	}

	return m_szTemp;
}

void CvInitCore::setLeaderName(PlayerTypes eID, const CvWString & szLeaderName)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setLeaderName");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		CvWString szName = szLeaderName;
		gDLL->stripSpecialCharacters(szName);

		m_aszLeaderName[eID] = szName;
	}
}

const CvWString & CvInitCore::getLeaderNameKey(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getLeaderNameKey");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aszLeaderName[eID];
	}
	else
	{
		m_szTemp = "";
		return m_szTemp;
	}
}

const CvWString & CvInitCore::getCivDescription(PlayerTypes eID, uint uiForm) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getCivDescription");

	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_szTemp = gDLL->getObjectText(CvString(m_aszCivDescription[eID]).GetCString(), uiForm, true);
	}
	else
	{
		m_szTemp = "";
	}

	return m_szTemp;
}

void CvInitCore::setCivDescription(PlayerTypes eID, const CvWString & szCivDescription)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setCivDescription");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		CvWString szName = szCivDescription;
		gDLL->stripSpecialCharacters(szName);
		m_aszCivDescription[eID] = szName;
	}
}

const CvWString & CvInitCore::getCivDescriptionKey(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getCivDescriptionKey");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aszCivDescription[eID];
	}
	else
	{
		m_szTemp = "";
		return m_szTemp;
	}
}

const CvWString & CvInitCore::getCivShortDesc(PlayerTypes eID, uint uiForm) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getCivShortDesc");

	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		// Assume we have stored the key
		m_szTemp = gDLL->getObjectText(CvString(m_aszCivShortDesc[eID]).GetCString(), uiForm, true);
	}
	else
	{
		m_szTemp = "";
	}

	return m_szTemp;
}

void CvInitCore::setCivShortDesc(PlayerTypes eID, const CvWString & szCivShortDesc)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setCivShortDesc");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		CvWString szName = szCivShortDesc;
		gDLL->stripSpecialCharacters(szName);
		m_aszCivShortDesc[eID] = szName;
	}
}

const CvWString & CvInitCore::getCivShortDescKey(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getCivShortDescKey");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aszCivShortDesc[eID];
	}
	else
	{
		m_szTemp = "";
		return m_szTemp;
	}
}

const CvWString & CvInitCore::getCivAdjective(PlayerTypes eID, uint uiForm) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getCivAdjective");

	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		// Assume we have stored the key
		m_szTemp = gDLL->getObjectText(CvString(m_aszCivAdjective[eID]).GetCString(), uiForm, true);
	}
	else
	{
		m_szTemp = "";
	}

	return m_szTemp;
}

void CvInitCore::setCivAdjective(PlayerTypes eID, const CvWString & szCivAdjective)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setCivAdjective");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		CvWString szName = szCivAdjective;
		gDLL->stripSpecialCharacters(szName);
		m_aszCivAdjective[eID] = szName;
	}
}

const CvWString & CvInitCore::getCivAdjectiveKey(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getCivAdjectiveKey");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aszCivAdjective[eID];
	}
	else
	{
		m_szTemp = "";
		return m_szTemp;
	}
}

const CvWString & CvInitCore::getCivPassword(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getCivPassword");

	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aszCivPassword[eID];
	}
	else
	{
		m_szTemp = "";
		return m_szTemp;
	}
}

void CvInitCore::setCivPassword(PlayerTypes eID, const CvWString & szCivPassword, bool bEncrypt)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setCivPassword");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		if (szCivPassword.empty() || !bEncrypt)
		{
			m_aszCivPassword[eID] = szCivPassword;
		}
		else
		{
			m_aszCivPassword[eID] = CvWString(gDLL->md5String((char*)CvString(szCivPassword).GetCString()));
		}
	}
}


const CvString & CvInitCore::getEmail(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getEmail");

	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aszEmail[eID];
	}
	else
	{
		m_szTempA = "";
		return m_szTempA;
	}
}

void CvInitCore::setEmail(PlayerTypes eID, const CvString & szEmail)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setEmail");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_aszEmail[eID] = szEmail;
	}
}

const CvString & CvInitCore::getSmtpHost(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getSmtpHost");

	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aszSmtpHost[eID];
	}
	else
	{
		m_szTempA = "";
		return m_szTempA;
	}
}

void CvInitCore::setSmtpHost(PlayerTypes eID, const CvString & szHost)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setSmtpHost");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_aszSmtpHost[eID] = szHost;
	}
}


bool CvInitCore::getWhiteFlag(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getWhiteFlag");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_abWhiteFlag[eID];
	}
	else
	{
		return false;
	}
}

void CvInitCore::setWhiteFlag(PlayerTypes eID, bool bWhiteFlag)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setWhiteFlag");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_abWhiteFlag[eID] = bWhiteFlag;
	}
}

const CvWString & CvInitCore::getFlagDecal(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getFlagDecal");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aszFlagDecal[eID];
	}
	else
	{
		m_szTemp = "";
		return m_szTemp;
	}
}

void CvInitCore::setFlagDecal(PlayerTypes eID, const CvWString & szFlagDecal)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setFlagDecal");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_aszFlagDecal[eID] = szFlagDecal;
	}
}


CivilizationTypes CvInitCore::getCiv(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getCiv");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aeCiv[eID];
	}
	else
	{
		return NO_CIVILIZATION;
	}
}

void CvInitCore::setCiv(PlayerTypes eID, CivilizationTypes eCiv)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setCiv");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_aeCiv[eID] = eCiv;
/*************************************************************************************************/
/**	MultiBarb								12/23/08								Xienwolf	**/
/**	adjusted for more barb teams																**/
/**				Workaround required until source of this call for Barbarians is located			**/
/*************************************************************************************************/
		if (eID == BARBARIAN_PLAYER && eCiv == (CivilizationTypes)(GC.getDefineINT("BARBARIAN_CIVILIZATION")))
		{
            setCiv(ANIMAL_PLAYER, (CivilizationTypes)(GC.getDefineINT("ANIMAL_CIVILIZATION")));
            setCiv(WILDMANA_PLAYER, (CivilizationTypes)(GC.getDefineINT("WILDMANA_CIVILIZATION")));
            setCiv(PIRATES_PLAYER, (CivilizationTypes)(GC.getDefineINT("PIRATES_CIVILIZATION")));
            setCiv(WHITEHAND_PLAYER, (CivilizationTypes)(GC.getDefineINT("WHITEHAND_CIVILIZATION")));
            setCiv(DEVIL_PLAYER, (CivilizationTypes)(GC.getDefineINT("DEVIL_CIVILIZATION")));
            setCiv(ORC_PLAYER, (CivilizationTypes)(GC.getDefineINT("BARBARIAN_CIVILIZATION")));
            setCiv(MERCENARIES_PLAYER, (CivilizationTypes)(GC.getDefineINT("MERCENARIES_CIVILIZATION")));
		}
/*************************************************************************************************/
/**	MultiBarb								END													**/
/*************************************************************************************************/
	}
}

LeaderHeadTypes CvInitCore::getLeader(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getLeader");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aeLeader[eID];
	}
	else
	{
		return NO_LEADER;
	}
}

void CvInitCore::setLeader(PlayerTypes eID, LeaderHeadTypes eLeader)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setLeader");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_aeLeader[eID] = eLeader;
/*************************************************************************************************/
/**	ADDON (MultiBarb) merged Sephi  		12/23/08								Xienwolf	**/
/**	adjusted for more barb teams																**/
/**				Workaround required until source of this call for Barbarians is located			**/
/*************************************************************************************************/
		if (eID == BARBARIAN_PLAYER && eLeader == (LeaderHeadTypes)(GC.getDefineINT("BARBARIAN_LEADER")))
		{
            setLeader(ANIMAL_PLAYER, (LeaderHeadTypes)(GC.getDefineINT("ANIMAL_LEADER")));
            setLeader(WILDMANA_PLAYER, (LeaderHeadTypes)(GC.getDefineINT("WILDMANA_LEADER")));
            setLeader(PIRATES_PLAYER, (LeaderHeadTypes)(GC.getDefineINT("PIRATES_LEADER")));
            setLeader(WHITEHAND_PLAYER, (LeaderHeadTypes)(GC.getDefineINT("WHITEHAND_LEADER")));
            setLeader(DEVIL_PLAYER, (LeaderHeadTypes)(GC.getDefineINT("DEVIL_LEADER")));
            setLeader(ORC_PLAYER, (LeaderHeadTypes)(GC.getDefineINT("BARBARIAN_LEADER")));
            setLeader(MERCENARIES_PLAYER, (LeaderHeadTypes)(GC.getDefineINT("MERCENARIES_LEADER")));
        }
/*************************************************************************************************/
/**	MultiBarb								END													**/
/*************************************************************************************************/

	}
}

TeamTypes CvInitCore::getTeam(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getTeam");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aeTeam[eID];
	}
	else
	{
		return NO_TEAM;
	}
}

void CvInitCore::setTeam(PlayerTypes eID, TeamTypes eTeam)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setTeam");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		if (getTeam(eID) != eTeam)
		{
			m_aeTeam[eID] = eTeam;

			if(CvPlayerAI::areStaticsInitialized())
			{
				GET_PLAYER(eID).updateTeamType();
			}
		}
	}
/*************************************************************************************************/
/**	ADDON (multibarb) merged Sephi			12/23/08								Xienwolf	**/
/**	adjusted for more barb teams																**/
/**				Workaround required until source of this call for Barbarians is located			**/
/*************************************************************************************************/
	if (eID == BARBARIAN_PLAYER && eTeam == BARBARIAN_TEAM)
	{
		for(int iI = BARBARIAN_PLAYER+1; iI < NUM_BARBARIAN_PLAYERS; iI++)
		{
			setTeam((PlayerTypes)iI, (TeamTypes)iI);
		}
    }
/*************************************************************************************************/
/**	MultiBarb								END													**/
/*************************************************************************************************/
}

HandicapTypes CvInitCore::getHandicap(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getHandicap");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aeHandicap[eID];
	}
	else
	{
		return NO_HANDICAP;
	}
}

void CvInitCore::setHandicap(PlayerTypes eID, HandicapTypes eHandicap)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setHandicap");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_aeHandicap[eID] = eHandicap;
	}
/*************************************************************************************************/
/**	ADDON (MultiBarb)  merged Sephi			12/23/08								Xienwolf	**/
/**	adjusted for more barb teams																**/
/**				Workaround required until source of this call for Barbarians is located			**/
/*************************************************************************************************/
	if (eID == BARBARIAN_PLAYER)
	{
		for(int iI = BARBARIAN_PLAYER+1; iI < NUM_BARBARIAN_PLAYERS; iI++)
		{
			setHandicap((PlayerTypes)iI, eHandicap);
		}
    }
/*************************************************************************************************/
/**	MultiBarb								END													**/
/*************************************************************************************************/
}

PlayerColorTypes CvInitCore::getColor(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getColor");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aeColor[eID];
	}
	else
	{
		return NO_PLAYERCOLOR;
	}
}

void CvInitCore::setColor(PlayerTypes eID, PlayerColorTypes eColor)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setColor");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_aeColor[eID] = eColor;
	}
/*************************************************************************************************/
/**	ADDON (MultiBarb)  merged Sephi			12/23/08								Xienwolf	**/
/**	adjusted for more barb teams																**/
/**				Workaround required until source of this call for Barbarians is located			**/
/*************************************************************************************************/
	if (eID == BARBARIAN_PLAYER && eColor != -1 && eColor == GC.getCivilizationInfo(GET_PLAYER(BARBARIAN_PLAYER).getCivilizationType()).getDefaultPlayerColor())
	{
        setColor(ANIMAL_PLAYER, (PlayerColorTypes)GC.getCivilizationInfo(GET_PLAYER(ANIMAL_PLAYER).getCivilizationType()).getDefaultPlayerColor());
        setColor(WILDMANA_PLAYER, (PlayerColorTypes)GC.getCivilizationInfo(GET_PLAYER(WILDMANA_PLAYER).getCivilizationType()).getDefaultPlayerColor());
        setColor(PIRATES_PLAYER, (PlayerColorTypes)GC.getCivilizationInfo(GET_PLAYER(PIRATES_PLAYER).getCivilizationType()).getDefaultPlayerColor());
        setColor(WHITEHAND_PLAYER, (PlayerColorTypes)GC.getCivilizationInfo(GET_PLAYER(WHITEHAND_PLAYER).getCivilizationType()).getDefaultPlayerColor());
        setColor(DEVIL_PLAYER, (PlayerColorTypes)GC.getCivilizationInfo(GET_PLAYER(DEVIL_PLAYER).getCivilizationType()).getDefaultPlayerColor());
        setColor(ORC_PLAYER, (PlayerColorTypes)GC.getCivilizationInfo(GET_PLAYER(DEVIL_PLAYER).getCivilizationType()).getDefaultPlayerColor());
        setColor(MERCENARIES_PLAYER, (PlayerColorTypes)GC.getCivilizationInfo(GET_PLAYER(MERCENARIES_PLAYER).getCivilizationType()).getDefaultPlayerColor());
    }
/*************************************************************************************************/
/**	MultiBarb								END													**/
/*************************************************************************************************/
}

ArtStyleTypes CvInitCore::getArtStyle(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getArtStyle");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aeArtStyle[eID];
	}
	else
	{
		return NO_ARTSTYLE;
	}
}

void CvInitCore::setArtStyle(PlayerTypes eID, ArtStyleTypes eArtStyle)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setArtStyle");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_aeArtStyle[eID] = eArtStyle;
	}
}


SlotStatus CvInitCore::getSlotStatus(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getSlotStatus");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aeSlotStatus[eID];
	}
	else
	{
		return SS_CLOSED;
	}
}

void CvInitCore::setSlotStatus(PlayerTypes eID, SlotStatus eSlotStatus)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setSlotStatus");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		if (getSlotStatus(eID) != eSlotStatus)
		{
			m_aeSlotStatus[eID] = eSlotStatus;

			if(CvPlayerAI::areStaticsInitialized())
			{
				GET_PLAYER(eID).updateHuman();
			}
		}
/*************************************************************************************************/
/**	ADDON (MultiBarb)  merged Sephi			12/23/08								Xienwolf	**/
/**	adjusted for more barb teams																**/
/**				Workaround required until source of this call for Barbarians is located			**/
/*************************************************************************************************/
		if (eID == BARBARIAN_PLAYER)
		{
			for(int iI = BARBARIAN_PLAYER+1; iI < NUM_BARBARIAN_PLAYERS; iI++)
			{
				setSlotStatus((PlayerTypes)iI, SS_COMPUTER);
			}
		}
/*************************************************************************************************/
/**	MultiBarb								END													**/
/*************************************************************************************************/
	}
}

SlotClaim CvInitCore::getSlotClaim(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getSlotClaim");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aeSlotClaim[eID];
	}
	else
	{
		return SLOTCLAIM_UNASSIGNED;
	}
}

void CvInitCore::setSlotClaim(PlayerTypes eID, SlotClaim eSlotClaim)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setSlotClaim");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_aeSlotClaim[eID] = eSlotClaim;
	}
}

bool CvInitCore::getReady(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getReady");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_abReady[eID];
	}
	else
	{
		return false;
	}
}

void CvInitCore::setReady(PlayerTypes eID, bool bReady)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setReady");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_abReady[eID] = bReady;
	}
}


bool CvInitCore::getPlayableCiv(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getPlayableCiv");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		if (getWBMapScript() && !getWBMapNoPlayers())
		{
			return m_abPlayableCiv[eID];
		}
		else
		{
			if (getCiv(eID) != NO_CIVILIZATION)
			{

//FfH: Added by Kael 08/10/2007
//				return GC.getCivilizationInfo(getCiv(eID)).isPlayable();
				return true;
//FfH: End Add

			}
			else
			{
				// Don't allow people to play the barb civ
				return (eID < MAX_CIV_PLAYERS);
			}
		}
	}
	else
	{
		return false;
	}
}

void CvInitCore::setPlayableCiv(PlayerTypes eID, bool bPlayableCiv)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setPlayableCiv");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_abPlayableCiv[eID] = bPlayableCiv;
	}
}

bool CvInitCore::getMinorNationCiv(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getMinorNationCiv");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_abMinorNationCiv[eID];
	}
	else
	{
		return false;
	}
}

void CvInitCore::setMinorNationCiv(PlayerTypes eID, bool bMinorNationCiv)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setMinorNationCiv");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_abMinorNationCiv[eID] = bMinorNationCiv;
	}
}

int CvInitCore::getNetID(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getNetID");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aiNetID[eID];
	}
	else
	{
		return -1;
	}
}

void CvInitCore::setNetID(PlayerTypes eID, int iNetID)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setNetID");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_aiNetID[eID] = iNetID;
	}
}

const CvString & CvInitCore::getPythonCheck(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getPythonCheck");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aszPythonCheck[eID];
	}
	else
	{
		m_szTempCheck.clear();
		return m_szTempCheck;
	}
}

void CvInitCore::setPythonCheck(PlayerTypes eID, const CvString & szPythonCheck)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setPythonCheck");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_aszPythonCheck[eID] = szPythonCheck;
	}
}

const CvString & CvInitCore::getXMLCheck(PlayerTypes eID) const
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::getXMLCheck");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		return m_aszXMLCheck[eID];
	}
	else
	{
		m_szTempCheck.clear();
		return m_szTempCheck;
	}
}

void CvInitCore::setXMLCheck(PlayerTypes eID, const CvString & szXMLCheck)
{
	FASSERT_BOUNDS(0, MAX_PLAYERS, eID, "CvInitCore::setXMLCheck");
	if ( checkBounds(eID, 0, MAX_PLAYERS) )
	{
		m_aszXMLCheck[eID] = szXMLCheck;
	}
}

void CvInitCore::setAdminPassword(const CvWString & szAdminPassword, bool bEncrypt)
{
	if (szAdminPassword.empty() || !bEncrypt)
	{
		m_szAdminPassword = szAdminPassword;
	}
	else
	{
		m_szAdminPassword = CvWString(gDLL->md5String((char*)CvString(szAdminPassword).GetCString()));
	}
}

void CvInitCore::resetAdvancedStartPoints()
{
	int iPoints = 0;

	if (NO_ERA != getEra())
	{
		iPoints += GC.getEraInfo(getEra()).getAdvancedStartPoints();
	}

	if (NO_WORLDSIZE != getWorldSize())
	{
		iPoints *= GC.getWorldInfo(getWorldSize()).getAdvancedStartPointsMod();
		iPoints /= 100;
	}

	if (NO_GAMESPEED != getGameSpeed())
	{
		iPoints *= GC.getGameSpeedInfo(getGameSpeed()).getGrowthPercent();
		iPoints /= 100;
	}

	setNumAdvancedStartPoints(iPoints);
}


void CvInitCore::read(FDataStreamBase* pStream)
{
	uint uiSaveFlag=0;
	pStream->Read(&uiSaveFlag);		// flags for expansion (see SaveBits)

/*************************************************************************************************/
/**	TrueModular								05/26/09	Written: Mr. Genie	Imported: Xienwolf	**/
/**																								**/
/**	Properly links Modular modifications to previous elements, and allows partial overwriting	**/
/*************************************************************************************************/
	int iNumSaveGameVector;
	pStream->Read(&iNumSaveGameVector);

	// Empty the Vector
	m_aszSaveGameVector.erase(m_aszSaveGameVector.begin(), m_aszSaveGameVector.end());

	CvString szSaveGameVector;
	for (int uiIndex = 0; uiIndex < iNumSaveGameVector; ++uiIndex)
	{
		pStream->ReadString(szSaveGameVector);
		m_aszSaveGameVector.push_back(szSaveGameVector);
	}
/*************************************************************************************************/
/**	TrueModular								END													**/
/*************************************************************************************************/
	// GAME DATA
	pStream->Read((int*)&m_eType);
	pStream->ReadString(m_szGameName);
	pStream->ReadString(m_szGamePassword);
	pStream->ReadString(m_szAdminPassword);
	pStream->ReadString(m_szMapScriptName);

	pStream->Read(&m_bWBMapNoPlayers);

	pStream->Read((int*)&m_eWorldSize);
	pStream->Read((int*)&m_eClimate);
	pStream->Read((int*)&m_eSeaLevel);
	pStream->Read((int*)&m_eEra);
	pStream->Read((int*)&m_eGameSpeed);
	pStream->Read((int*)&m_eTurnTimer);
	pStream->Read((int*)&m_eCalendar);

	SAFE_DELETE_ARRAY(m_aeCustomMapOptions);
	pStream->Read(&m_iNumCustomMapOptions);
	pStream->Read(&m_iNumHiddenCustomMapOptions);
	if (m_iNumCustomMapOptions > 0)
	{
		m_aeCustomMapOptions = new CustomMapOptionTypes[m_iNumCustomMapOptions];
		pStream->Read(m_iNumCustomMapOptions, (int*)m_aeCustomMapOptions);
	}

	SAFE_DELETE_ARRAY(m_abVictories);
	pStream->Read(&m_iNumVictories);
	if (m_iNumVictories > 0)
	{
		m_abVictories = new bool[m_iNumVictories];
		pStream->Read(m_iNumVictories, m_abVictories);
	}


	if (uiSaveFlag > 0)
	{
		pStream->Read(NUM_GAMEOPTION_TYPES, m_abOptions);
	}
	else
	{
		pStream->Read(NUM_GAMEOPTION_TYPES-1, m_abOptions);
		m_abOptions[NUM_GAMEOPTION_TYPES-1] = false;
	}
	pStream->Read(NUM_MPOPTION_TYPES, m_abMPOptions);

	pStream->Read(&m_bStatReporting);

	pStream->Read(&m_iGameTurn);
	pStream->Read(&m_iMaxTurns);
	pStream->Read(&m_iPitbossTurnTime);
	pStream->Read(&m_iTargetScore);

	pStream->Read(&m_iMaxCityElimination);
	pStream->Read(&m_iNumAdvancedStartPoints);

	// PLAYER DATA
	pStream->ReadString(MAX_PLAYERS, m_aszLeaderName);
	pStream->ReadString(MAX_PLAYERS, m_aszCivDescription);
	pStream->ReadString(MAX_PLAYERS, m_aszCivShortDesc);
	pStream->ReadString(MAX_PLAYERS, m_aszCivAdjective);
	pStream->ReadString(MAX_PLAYERS, m_aszCivPassword);
	pStream->ReadString(MAX_PLAYERS, m_aszEmail);
	pStream->ReadString(MAX_PLAYERS, m_aszSmtpHost);

	pStream->Read(MAX_PLAYERS, m_abWhiteFlag);
	pStream->ReadString(MAX_PLAYERS, m_aszFlagDecal);

	pStream->Read(MAX_PLAYERS, (int*)m_aeCiv);
	pStream->Read(MAX_PLAYERS, (int*)m_aeLeader);
	pStream->Read(MAX_PLAYERS, (int*)m_aeTeam);


	pStream->Read(MAX_PLAYERS, (int*)m_aeHandicap);
	pStream->Read(MAX_PLAYERS, (int*)m_aeColor);
	pStream->Read(MAX_PLAYERS, (int*)m_aeArtStyle);

	pStream->Read(MAX_PLAYERS, (int*)m_aeSlotStatus);
	pStream->Read(MAX_PLAYERS, (int*)m_aeSlotClaim);

	for (int i=0;i<MAX_PLAYERS;i++)
	{
		if (m_aeSlotClaim[i] == SLOTCLAIM_ASSIGNED)
		{
			m_aeSlotClaim[i] = SLOTCLAIM_RESERVED;
		}
	}

	pStream->Read(MAX_PLAYERS, m_abPlayableCiv);
	pStream->Read(MAX_PLAYERS, m_abMinorNationCiv);

	if(CvPlayerAI::areStaticsInitialized())
	{
		for (int i=0;i<MAX_PLAYERS;i++)
		{
			GET_PLAYER((PlayerTypes)i).updateHuman();
			GET_PLAYER((PlayerTypes) i).updateTeamType();
		}
	}
}


void CvInitCore::write(FDataStreamBase* pStream)
{
	uint uiSaveFlag=1;
	pStream->Write(uiSaveFlag);		// flag for expansion, see SaveBits)
/*************************************************************************************************/
/**	TrueModular								05/26/09	Written: Mr. Genie	Imported: Xienwolf	**/
/**																								**/
/**	Properly links Modular modifications to previous elements, and allows partial overwriting	**/
/*************************************************************************************************/
	int iNumModLoadControlVector = GC.getModLoadControlVectorSize();
	pStream->Write(iNumModLoadControlVector);

	for (int uiIndex = 0; uiIndex < iNumModLoadControlVector; ++uiIndex)
	{
		pStream->WriteString(GC.getModLoadControlVector(uiIndex));
	}
/*************************************************************************************************/
/**	TrueModular								END													**/
/*************************************************************************************************/

	// GAME DATA
	pStream->Write(m_eType);
	pStream->WriteString(m_szGameName);
	pStream->WriteString(m_szGamePassword);
	pStream->WriteString(m_szAdminPassword);
	pStream->WriteString(m_szMapScriptName);

	pStream->Write(m_bWBMapNoPlayers);

	pStream->Write(m_eWorldSize);
	pStream->Write(m_eClimate);
	pStream->Write(m_eSeaLevel);
	pStream->Write(m_eEra);
	pStream->Write(m_eGameSpeed);
	pStream->Write(m_eTurnTimer);
	pStream->Write(m_eCalendar);

	pStream->Write(m_iNumCustomMapOptions);
	pStream->Write(m_iNumHiddenCustomMapOptions);
	pStream->Write(m_iNumCustomMapOptions, (int*)m_aeCustomMapOptions);

	pStream->Write(m_iNumVictories);
	pStream->Write(m_iNumVictories, m_abVictories);

	pStream->Write(NUM_GAMEOPTION_TYPES, m_abOptions);
	pStream->Write(NUM_MPOPTION_TYPES, m_abMPOptions);

	pStream->Write(m_bStatReporting);

	pStream->Write(m_iGameTurn);
	pStream->Write(m_iMaxTurns);
	pStream->Write(m_iPitbossTurnTime);
	pStream->Write(m_iTargetScore);

	pStream->Write(m_iMaxCityElimination);
	pStream->Write(m_iNumAdvancedStartPoints);

	// PLAYER DATA
	pStream->WriteString(MAX_PLAYERS, m_aszLeaderName);
	pStream->WriteString(MAX_PLAYERS, m_aszCivDescription);
	pStream->WriteString(MAX_PLAYERS, m_aszCivShortDesc);
	pStream->WriteString(MAX_PLAYERS, m_aszCivAdjective);
	pStream->WriteString(MAX_PLAYERS, m_aszCivPassword);
	pStream->WriteString(MAX_PLAYERS, m_aszEmail);
	pStream->WriteString(MAX_PLAYERS, m_aszSmtpHost);

	pStream->Write(MAX_PLAYERS, m_abWhiteFlag);
	pStream->WriteString(MAX_PLAYERS, m_aszFlagDecal);

	pStream->Write(MAX_PLAYERS, (int*)m_aeCiv);
	pStream->Write(MAX_PLAYERS, (int*)m_aeLeader);
	pStream->Write(MAX_PLAYERS, (int*)m_aeTeam);
	pStream->Write(MAX_PLAYERS, (int*)m_aeHandicap);
	pStream->Write(MAX_PLAYERS, (int*)m_aeColor);
	pStream->Write(MAX_PLAYERS, (int*)m_aeArtStyle);

	pStream->Write(MAX_PLAYERS, (int*)m_aeSlotStatus);
	pStream->Write(MAX_PLAYERS, (int*)m_aeSlotClaim);

	pStream->Write(MAX_PLAYERS, m_abPlayableCiv);
	pStream->Write(MAX_PLAYERS, m_abMinorNationCiv);
}
/*************************************************************************************************/
/**	TrueModular								05/26/09	Written: Mr. Genie	Imported: Xienwolf	**/
/**																								**/
/**	Properly links Modular modifications to previous elements, and allows partial overwriting	**/
/*************************************************************************************************/
int CvInitCore::getNumSaveGameVector()
{
	return m_aszSaveGameVector.size();
}

CvString CvInitCore::getSaveGameVector(int i)
{
	FAssert( i > -1);
	FAssert( i < (int)m_aszSaveGameVector.size() );
	return m_aszSaveGameVector[i];
}

/**	LoadedTGA								05/26/09	Written: Mr. Genie	Imported: Xienwolf	**/
void CvInitCore::doReloadInfoClasses()
{
	GC.doResetInfoClasses((int)m_aszSaveGameVector.size(), m_aszSaveGameVector);
}
/*************************************************************************************************/
/**	TrueModular								END													**/
/*************************************************************************************************/