// player.cpp

#include "CvGameCoreDLL.h"
#include "CvGlobals.h"
#include "CvArea.h"
#include "CvGameAI.h"
#include "CvMap.h"
#include "CvPlot.h"
#include "CvRandom.h"
#include "CvTeamAI.h"
#include "CvGameCoreUtils.h"
#include "CvPlayerAI.h"
#include "CvPlayer.h"
#include "CvGameCoreUtils.h"
#include "CvArtFileMgr.h"
#include "CvDiploParameters.h"
#include "CvInitCore.h"
#include "CyArgsList.h"
#include "CvInfos.h"
#include "CvPopupInfo.h"
#include "CvDiploParameters.h"
#include "FProfiler.h"
#include "CvGameTextMgr.h"
#include "CyCity.h"
#include "CyPlot.h"
#include "CyUnit.h"
#include "CvEventReporter.h"

#include "CvDLLInterfaceIFaceBase.h"
#include "CvDLLEntityIFaceBase.h"
#include "CvDLLEngineIFaceBase.h"
#include "CvDLLFAStarIFaceBase.h"
#include "CvDLLPythonIFaceBase.h"

// Public Functions...

CvPlayer::CvPlayer()
{
	m_aiSeaPlotYield = new int[NUM_YIELD_TYPES];
	m_aiYieldRateModifier = new int[NUM_YIELD_TYPES];
	m_aiCapitalYieldRateModifier = new int[NUM_YIELD_TYPES];
	m_aiExtraYieldThreshold = new int[NUM_YIELD_TYPES];
	m_aiTradeYieldModifier = new int[NUM_YIELD_TYPES];
	m_aiFreeCityCommerce = new int[NUM_COMMERCE_TYPES];
	m_aiCommercePercent = new int[NUM_COMMERCE_TYPES];
	m_aiCommerceRate = new int[NUM_COMMERCE_TYPES];
	m_aiCommerceRateModifier = new int[NUM_COMMERCE_TYPES];
	m_aiCapitalCommerceRateModifier = new int[NUM_COMMERCE_TYPES];
	m_aiCommerceChangesPerCultureLevel = new int[NUM_COMMERCE_TYPES];
	m_aiStateReligionBuildingCommerce = new int[NUM_COMMERCE_TYPES];
	m_aiSpecialistExtraCommerce = new int[NUM_COMMERCE_TYPES];
	m_aiCommerceFlexibleCount = new int[NUM_COMMERCE_TYPES];
	m_aiGoldPerTurnByPlayer = new int[MAX_PLAYERS];
	m_aiEspionageSpendingWeightAgainstTeam = new int[MAX_TEAMS];

	m_abFeatAccomplished = new bool[NUM_FEAT_TYPES];
	m_abOptions = new bool[NUM_PLAYEROPTION_TYPES];

	m_paiBonusExport = NULL;
	m_paiBonusImport = NULL;
	m_paiImprovementCount = NULL;
	m_paiFreeBuildingCount = NULL;
	m_paiExtraBuildingHappiness = NULL;
	m_paiExtraBuildingHealth = NULL;
	m_paiFeatureHappiness = NULL;
	m_paiUnitClassCount = NULL;
	m_paiUnitClassMaking = NULL;
	m_paiBuildingClassCount = NULL;
	m_paiBuildingClassMaking = NULL;
	m_paiHurryCount = NULL;
	m_paiSpecialBuildingNotRequiredCount = NULL;
	m_paiHasCivicOptionCount = NULL;
	m_paiNoCivicUpkeepCount = NULL;
	m_paiHasReligionCount = NULL;
	m_paiHasCorporationCount = NULL;
	m_paiUpkeepCount = NULL;
	m_paiSpecialistValidCount = NULL;

	m_pabResearchingTech = NULL;
	m_pabLoyalMember = NULL;

	m_paeCivics = NULL;

	m_ppaaiSpecialistExtraYield = NULL;
	m_ppaaiImprovementYieldChange = NULL;

//FfH: Added by Kael 11/06/2007
	m_pbTraits = NULL;
	m_ppaaiSpecialistTypeExtraCommerce = NULL;
//FfH: End Add
/*************************************************************************************************/
/**	New Gameoption (Passive XP Training) Sephi                                				    **/
/*************************************************************************************************/
	m_paiTrainXPCap = NULL;
	m_paiTrainXPRate = NULL;
/**	ADDON (Houses of Erebus) Sephi			                                 					**/
	m_pabCorporationSupport = NULL;
	m_paiCorporationSupport = NULL;
	m_paiCorporationSupportMod = NULL;
/**	ADDON (Adventures) Sephi                                                      				**/
	m_pabAdventureEnabled = NULL;
	m_pabAdventureFinished = NULL;
	m_ppaaiAdventureCounter = NULL;
/**	ADDON (New Mana) Sephi                                                      				**/
	m_paiSpellDamageModify = NULL;
	m_pabCanEverSpellResearch = NULL;
	m_paiManaSchoolCostModifier = NULL;
	m_paiGlobalYield = NULL;
	m_paiGlobalYieldUpkeep = NULL;
	m_paiGlobalYieldNeededForEquipmentBonus = NULL;
	m_paiNumBonusesConsumed = NULL;
	m_paiNumBonusesConsumedByProduction = NULL;
	m_paiFoundHolyCityCount = NULL;
	m_paiManaFromImprovement = NULL;
	m_paiFaithFromImprovement = NULL;
	m_paiIgnoreBuildingGYCostCount = NULL;
/*************************************************************************************************/
/**	    									END													**/
/*************************************************************************************************/
	reset(NO_PLAYER, true);
}


CvPlayer::~CvPlayer()
{
	uninit();

	SAFE_DELETE_ARRAY(m_aiSeaPlotYield);
	SAFE_DELETE_ARRAY(m_aiYieldRateModifier);
	SAFE_DELETE_ARRAY(m_aiCapitalYieldRateModifier);
	SAFE_DELETE_ARRAY(m_aiExtraYieldThreshold);
	SAFE_DELETE_ARRAY(m_aiTradeYieldModifier);
	SAFE_DELETE_ARRAY(m_aiFreeCityCommerce);
	SAFE_DELETE_ARRAY(m_aiCommercePercent);
	SAFE_DELETE_ARRAY(m_aiCommerceRate);
	SAFE_DELETE_ARRAY(m_aiCommerceRateModifier);
	SAFE_DELETE_ARRAY(m_aiCapitalCommerceRateModifier);
	SAFE_DELETE_ARRAY(m_aiCommerceChangesPerCultureLevel);
	SAFE_DELETE_ARRAY(m_aiStateReligionBuildingCommerce);
	SAFE_DELETE_ARRAY(m_aiSpecialistExtraCommerce);
	SAFE_DELETE_ARRAY(m_aiCommerceFlexibleCount);
	SAFE_DELETE_ARRAY(m_aiGoldPerTurnByPlayer);
	SAFE_DELETE_ARRAY(m_aiEspionageSpendingWeightAgainstTeam);
	SAFE_DELETE_ARRAY(m_abFeatAccomplished);
	SAFE_DELETE_ARRAY(m_abOptions);
}


void CvPlayer::init(PlayerTypes eID)
{
	LeaderHeadTypes eBestPersonality;
	int iValue;
	int iBestValue;
	int iI, iJ;

	//--------------------------------
	// Init saved data
	reset(eID);

	//--------------------------------
	// Init containers
	m_plotGroups.init();

	m_cities.init();

	m_units.init();

	m_selectionGroups.init();

	m_eventsTriggered.init();
/*************************************************************************************************/
/**	ADDON (Combatauras) Sephi																	**/
/*************************************************************************************************/
	m_combatAuras.init();
	m_AIGroups.init();
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
	//--------------------------------
	// Init non-saved data
	setupGraphical();

	//--------------------------------
	// Init other game data
	FAssert(getTeam() != NO_TEAM);
	GET_TEAM(getTeam()).changeNumMembers(1);

	//added block for too many civs
	if (GC.getInitCore().getSlotStatus((PlayerTypes)eID) == SS_COMPUTER && eID < MAX_CIV_PLAYERS)
	{
		if(GC.getInitCore().getHuman((PlayerTypes)eID) == false) {
			int iMax = GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getDefaultPlayers() + GC.getMapINLINE().getWorldSize();

			if(eID > iMax) {
				GC.getInitCore().setSlotStatus((PlayerTypes)eID, SS_CLOSED);
				setAlive(false);
				return;
			}
		}
	}

	if ((GC.getInitCore().getSlotStatus(getID()) == SS_TAKEN) || (GC.getInitCore().getSlotStatus(getID()) == SS_COMPUTER))
	{
		setAlive(true);

		if (GC.getGameINLINE().isOption(GAMEOPTION_RANDOM_PERSONALITIES))
		{
			if (!isBarbarian() && !isMinorCiv())
			{
				iBestValue = 0;
				eBestPersonality = NO_LEADER;

				for (iI = 0; iI < GC.getNumLeaderHeadInfos(); iI++)
				{
/*************************************************************************************************/
/**	ADDON (MultiBarb)  merged Sephi			12/23/08								Xienwolf	**/
/**	adjusted for more barb teams																**/
/**							Adds extra Barbarian Civilizations									**/
/*************************************************************************************************/
/**								---- Start Original Code ----									**
					if (iI != GC.getDefineINT("BARBARIAN_LEADER")) // XXX minor civ???
/**								----  End Original Code  ----									**/
					if (iI != GC.getDefineINT("BARBARIAN_LEADER")
					&& iI != GC.getDefineINT("ANIMAL_LEADER")
					&& iI != GC.getDefineINT("WILDMANA_LEADER")
					&& iI != GC.getDefineINT("PIRATES_LEADER")
					&& iI != GC.getDefineINT("WHITEHAND_LEADER")
					&& iI != GC.getDefineINT("DEVIL_LEADER")
					&& iI != GC.getDefineINT("MERCENARIES_LEADER")
					)
/*************************************************************************************************/
/**	MultiBarb								END													**/
/*************************************************************************************************/
					{
						iValue = (1 + GC.getGameINLINE().getSorenRandNum(10000, "Choosing Personality"));

						for (iJ = 0; iJ < MAX_CIV_PLAYERS; iJ++)
						{
							if (GET_PLAYER((PlayerTypes)iJ).isAlive())
							{
								if (GET_PLAYER((PlayerTypes)iJ).getPersonalityType() == ((LeaderHeadTypes)iI))
								{
									iValue /= 2;
								}
							}
						}

						if (iValue > iBestValue)
						{
							iBestValue = iValue;
							eBestPersonality = ((LeaderHeadTypes)iI);
						}
					}
				}

				if (eBestPersonality != NO_LEADER)
				{
					setPersonalityType(eBestPersonality);
				}
			}
		}

		changeBaseFreeUnits(GC.getDefineINT("INITIAL_BASE_FREE_UNITS"));
		changeBaseFreeMilitaryUnits(GC.getDefineINT("INITIAL_BASE_FREE_MILITARY_UNITS"));
		changeFreeUnitsPopulationPercent(GC.getDefineINT("INITIAL_FREE_UNITS_POPULATION_PERCENT"));
		changeFreeMilitaryUnitsPopulationPercent(GC.getDefineINT("INITIAL_FREE_MILITARY_UNITS_POPULATION_PERCENT"));
		changeGoldPerUnit(GC.getDefineINT("INITIAL_GOLD_PER_UNIT"));
		changeGoldPerMilitaryUnit(GC.getDefineINT("INITIAL_GOLD_PER_MILITARY_UNIT"));
		changeTradeRoutes(GC.getDefineINT("INITIAL_TRADE_ROUTES"));
		changeStateReligionHappiness(GC.getDefineINT("INITIAL_STATE_RELIGION_HAPPINESS"));
		changeNonStateReligionHappiness(GC.getDefineINT("INITIAL_NON_STATE_RELIGION_HAPPINESS"));


		for (iI = 0; iI < NUM_YIELD_TYPES; iI++)
		{
			changeTradeYieldModifier(((YieldTypes)iI), GC.getYieldInfo((YieldTypes)iI).getTradeModifier());
		}

		for (iI = 0; iI < NUM_COMMERCE_TYPES; iI++)
		{
			setCommercePercent(((CommerceTypes)iI), GC.getCommerceInfo((CommerceTypes)iI).getInitialPercent());
		}

		FAssertMsg((GC.getNumTraitInfos() > 0), "GC.getNumTraitInfos() is less than or equal to zero but is expected to be larger than zero in CvPlayer::init");
		for (iI = 0; iI < GC.getNumTraitInfos(); iI++)
		{

//FfH: Modified by Kael 0/06/2007
//			if (hasTrait((TraitTypes)iI))
//			{
//				changeExtraHealth(GC.getTraitInfo((TraitTypes)iI).getHealth());
//				changeExtraHappiness(GC.getTraitInfo((TraitTypes)iI).getHappiness());
//
//				for (iJ = 0; iJ < GC.getNumBuildingInfos(); iJ++)
//				{
//					changeExtraBuildingHappiness((BuildingTypes)iJ, GC.getBuildingInfo((BuildingTypes)iJ).getHappinessTraits(iI));
//				}
//
//				changeUpkeepModifier(GC.getTraitInfo((TraitTypes)iI).getUpkeepModifier());
//				changeLevelExperienceModifier(GC.getTraitInfo((TraitTypes)iI).getLevelExperienceModifier());
//				changeGreatPeopleRateModifier(GC.getTraitInfo((TraitTypes)iI).getGreatPeopleRateModifier());
//				changeGreatGeneralRateModifier(GC.getTraitInfo((TraitTypes)iI).getGreatGeneralRateModifier());
//				changeDomesticGreatGeneralRateModifier(GC.getTraitInfo((TraitTypes)iI).getDomesticGreatGeneralRateModifier());
//
//				changeMaxGlobalBuildingProductionModifier(GC.getTraitInfo((TraitTypes)iI).getMaxGlobalBuildingProductionModifier());
//				changeMaxTeamBuildingProductionModifier(GC.getTraitInfo((TraitTypes)iI).getMaxTeamBuildingProductionModifier());
//				changeMaxPlayerBuildingProductionModifier(GC.getTraitInfo((TraitTypes)iI).getMaxPlayerBuildingProductionModifier());
//
//				for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++)
//				{
//					changeTradeYieldModifier(((YieldTypes)iJ), GC.getTraitInfo((TraitTypes)iI).getTradeYieldModifier(iJ));
//				}
//
//				for (iJ = 0; iJ < NUM_COMMERCE_TYPES; iJ++)
//				{
//					changeFreeCityCommerce(((CommerceTypes)iJ), GC.getTraitInfo((TraitTypes)iI).getCommerceChange(iJ));
//					changeCommerceRateModifier(((CommerceTypes)iJ), GC.getTraitInfo((TraitTypes)iI).getCommerceModifier(iJ));
//				}
//
//				for (iJ = 0; iJ < GC.getNumCivicOptionInfos(); iJ++)
//				{
//					if (GC.getCivicOptionInfo((CivicOptionTypes) iJ).getTraitNoUpkeep(iI))
//					{
//						changeNoCivicUpkeepCount(((CivicOptionTypes)iJ), 1);
//					}
//				}
//			}
            if (GC.getLeaderHeadInfo(getLeaderType()).hasTrait((TraitTypes)iI))
            {
                setHasTrait((TraitTypes)iI, true);
            }
//FfH: End Modify

        }

//FfH: Added by Kael 08/14/2007
        if (GC.getCivilizationInfo(getCivilizationType()).getCivTrait() != NO_TRAIT)
        {
            setHasTrait((TraitTypes)GC.getCivilizationInfo(getCivilizationType()).getCivTrait(), true);
        }
		setAlignment(GC.getLeaderHeadInfo(getLeaderType()).getAlignment());
        GC.getGameINLINE().changeGlobalCounterLimit(GC.getDefineINT("GLOBAL_COUNTER_LIMIT_PER_PLAYER"));
//FfH: End Add
/*************************************************************************************************/
/** ADDON (new CvCivilizationInfo tags) Sephi                                                   **/
/*************************************************************************************************/
        setPopulationLimit(GC.getCivilizationInfo(getCivilizationType()).getPopulationLimit());
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
		updateMaxAnarchyTurns();

		for (iI = 0; iI < NUM_YIELD_TYPES; iI++)
		{
			updateExtraYieldThreshold((YieldTypes)iI);
		}

		for (iI = 0; iI < GC.getNumCivicOptionInfos(); iI++)
		{
			setCivics(((CivicOptionTypes)iI), ((CivicTypes)(GC.getCivilizationInfo(getCivilizationType()).getCivilizationInitialCivics(iI))));
		}

		for (iI = 0; iI < GC.getNumEventInfos(); iI++)
		{
			resetEventOccured((EventTypes)iI, false);
		}

		for (iI = 0; iI < GC.getNumEventTriggerInfos(); iI++)
		{
			resetTriggerFired((EventTriggerTypes)iI);
		}

		for (iI = 0; iI < GC.getNumUnitClassInfos(); ++iI)
		{
			UnitTypes eUnit = ((UnitTypes)(GC.getCivilizationInfo(getCivilizationType()).getCivilizationUnits(iI)));

			if (NO_UNIT != eUnit)
			{
				if (GC.getUnitInfo(eUnit).isFound())
				{
					setUnitExtraCost((UnitClassTypes)iI, getNewCityProductionValue());
				}
			}
		}
	}

	//init Spellresearch
	initCanEverSpellResearch();

	AI_init();
/*************************************************************************************************/
/**	ADDON (MultiBarb)  merged Sephi		12/23/08									Xienwolf	**/
/**	adjusted for new barb teams	        														**/
/**				Workaround required until source of this call for Barbarians is located			**/
/*************************************************************************************************/
	if (eID == BARBARIAN_PLAYER)
	{
		for(int iI = BARBARIAN_PLAYER+1; iI < NUM_BARBARIAN_PLAYERS; iI++)
		{
			GET_PLAYER((PlayerTypes)iI).init((PlayerTypes)iI);
		}
	}
/*************************************************************************************************/
/**	MultiBarb								END													**/
/*************************************************************************************************/
}


void CvPlayer::uninit()
{
	SAFE_DELETE_ARRAY(m_paiBonusExport);
	SAFE_DELETE_ARRAY(m_paiBonusImport);
	SAFE_DELETE_ARRAY(m_paiImprovementCount);
	SAFE_DELETE_ARRAY(m_paiFreeBuildingCount);
	SAFE_DELETE_ARRAY(m_paiExtraBuildingHappiness);
	SAFE_DELETE_ARRAY(m_paiExtraBuildingHealth);
	SAFE_DELETE_ARRAY(m_paiFeatureHappiness);
	SAFE_DELETE_ARRAY(m_paiUnitClassCount);
	SAFE_DELETE_ARRAY(m_paiUnitClassMaking);
	SAFE_DELETE_ARRAY(m_paiBuildingClassCount);
	SAFE_DELETE_ARRAY(m_paiBuildingClassMaking);
	SAFE_DELETE_ARRAY(m_paiHurryCount);
	SAFE_DELETE_ARRAY(m_paiSpecialBuildingNotRequiredCount);
	SAFE_DELETE_ARRAY(m_paiHasCivicOptionCount);
	SAFE_DELETE_ARRAY(m_paiNoCivicUpkeepCount);
	SAFE_DELETE_ARRAY(m_paiHasReligionCount);
	SAFE_DELETE_ARRAY(m_paiHasCorporationCount);
	SAFE_DELETE_ARRAY(m_paiUpkeepCount);
	SAFE_DELETE_ARRAY(m_paiSpecialistValidCount);

	SAFE_DELETE_ARRAY(m_pabResearchingTech);
	SAFE_DELETE_ARRAY(m_pabLoyalMember);

	SAFE_DELETE_ARRAY(m_paeCivics);

	m_triggersFired.clear();
//FfH Scriptable Leader Traits: Added by Kael 08/08/2007

	SAFE_DELETE_ARRAY(m_pbTraits);
	if (m_ppaaiSpecialistTypeExtraCommerce != NULL)
	{
		for (int iI = 0; iI < GC.getNumSpecialistInfos(); iI++)
		{
			SAFE_DELETE_ARRAY(m_ppaaiSpecialistTypeExtraCommerce[iI]);
		}
		SAFE_DELETE_ARRAY(m_ppaaiSpecialistTypeExtraCommerce);
	}
//FfH: End Add

	if (m_ppaaiSpecialistExtraYield != NULL)
	{
		for (int iI = 0; iI < GC.getNumSpecialistInfos(); iI++)
		{
			SAFE_DELETE_ARRAY(m_ppaaiSpecialistExtraYield[iI]);
		}
		SAFE_DELETE_ARRAY(m_ppaaiSpecialistExtraYield);
	}

	if (m_ppaaiImprovementYieldChange != NULL)
	{
		for (int iI = 0; iI < GC.getNumImprovementInfos(); iI++)
		{
			if(m_ppaaiImprovementYieldChange[iI] != NULL)
			{
				SAFE_DELETE_ARRAY(m_ppaaiImprovementYieldChange[iI]);
			}
		}
		SAFE_DELETE_ARRAY(m_ppaaiImprovementYieldChange);
	}
/*************************************************************************************************/
/**	New Gameoption (Passive XP Training) Sephi                                				    **/
/*************************************************************************************************/
	SAFE_DELETE_ARRAY(m_paiTrainXPCap);
	SAFE_DELETE_ARRAY(m_paiTrainXPRate);
/**	ADDON (Houses of Erebus) Sephi			                                 					**/
	SAFE_DELETE_ARRAY(m_pabCorporationSupport);
	SAFE_DELETE_ARRAY(m_paiCorporationSupport);
	SAFE_DELETE_ARRAY(m_paiCorporationSupportMod);
/**	ADDON (Adventures) Sephi                                                      				**/
	SAFE_DELETE_ARRAY(m_pabAdventureEnabled);
	SAFE_DELETE_ARRAY(m_pabAdventureFinished);

	if (m_ppaaiAdventureCounter != NULL)
	{
		for (int iI = 0; iI < GC.getNumAdventureInfos(); iI++)
		{
			SAFE_DELETE_ARRAY(m_ppaaiAdventureCounter[iI]);
		}
		SAFE_DELETE_ARRAY(m_ppaaiAdventureCounter);
	}
/**	ADDON (New Mana) Sephi                                                      				**/
	SAFE_DELETE_ARRAY(m_paiManaSchoolCostModifier);
	SAFE_DELETE_ARRAY(m_paiGlobalYield);
	SAFE_DELETE_ARRAY(m_paiGlobalYieldUpkeep);
	SAFE_DELETE_ARRAY(m_paiGlobalYieldNeededForEquipmentBonus);
	SAFE_DELETE_ARRAY(m_pabCanEverSpellResearch);
	SAFE_DELETE_ARRAY(m_paiSpellDamageModify);
	SAFE_DELETE_ARRAY(m_paiNumBonusesConsumed);
	SAFE_DELETE_ARRAY(m_paiNumBonusesConsumedByProduction);
	SAFE_DELETE_ARRAY(m_paiFoundHolyCityCount);
	SAFE_DELETE_ARRAY(m_paiManaFromImprovement);
	SAFE_DELETE_ARRAY(m_paiFaithFromImprovement);
	SAFE_DELETE_ARRAY(m_paiIgnoreBuildingGYCostCount);
/*************************************************************************************************/
/**	    									END													**/
/*************************************************************************************************/

	m_groupCycle.clear();

	m_researchQueue.clear();

	m_cityNames.clear();


	m_plotGroups.uninit();

	m_cities.uninit();

	m_units.uninit();

	m_selectionGroups.uninit();

	m_eventsTriggered.uninit();
/*************************************************************************************************/
/**	ADDON (Combatauras) Sephi																	**/
/*************************************************************************************************/
	m_combatAuras.uninit();
	m_AIGroups.uninit();
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

	clearMessages();

	clearPopups();

	clearDiplomacy();
}


// FUNCTION: reset()
// Initializes data members that are serialized.
void CvPlayer::reset(PlayerTypes eID, bool bConstructorCall)
{
	int iI, iJ;

	//--------------------------------
	// Uninit class
	uninit();

	m_iStartingX = INVALID_PLOT_COORD;
	m_iStartingY = INVALID_PLOT_COORD;
	m_iTotalPopulation = 0;
	m_iTotalLand = 0;
	m_iTotalLandScored = 0;
	m_iGold = 0;
	m_iGoldPerTurn = 0;
	m_iAdvancedStartPoints = -1;
	m_iGoldenAgeTurns = 0;
	m_iNumUnitGoldenAges = 0;
	m_iStrikeTurns = 0;
	m_iAnarchyTurns = 0;
	m_iMaxAnarchyTurns = 0;
	m_iAnarchyModifier = 0;
	m_iGoldenAgeModifier = 0;
	m_iGlobalHurryModifier = 0;
	m_iGreatPeopleCreated = 0;
	m_iGreatGeneralsCreated = 0;
	m_iGreatPeopleThresholdModifier = 0;
	m_iGreatGeneralsThresholdModifier = 0;
	m_iGreatPeopleRateModifier = 0;
	m_iGreatGeneralRateModifier = 0;
	m_iDomesticGreatGeneralRateModifier = 0;
	m_iStateReligionGreatPeopleRateModifier = 0;
	m_iMaxGlobalBuildingProductionModifier = 0;
	m_iMaxTeamBuildingProductionModifier = 0;
	m_iMaxPlayerBuildingProductionModifier = 0;
	m_iFreeExperience = 0;
	m_iFeatureProductionModifier = 0;
	m_iWorkerSpeedModifier = 0;
	m_iImprovementUpgradeRateModifier = 0;
	m_iMilitaryProductionModifier = 0;
	m_iSpaceProductionModifier = 0;
	m_iCityDefenseModifier = 0;
	m_iNumNukeUnits = 0;
	m_iNumOutsideUnits = 0;
	m_iBaseFreeUnits = 0;
	m_iBaseFreeMilitaryUnits = 0;
	m_iFreeUnitsPopulationPercent = 0;
	m_iFreeMilitaryUnitsPopulationPercent = 0;
	m_iGoldPerUnit = 0;
	m_iGoldPerMilitaryUnit = 0;
	m_iExtraUnitCost = 0;
	m_iNumMilitaryUnits = 0;
	m_iNumSummonUnits = 0;
	m_iNumGarrisionUnits = 0;
	m_iHappyPerMilitaryUnit = 0;
	m_iMilitaryFoodProductionCount = 0;
	m_iConscriptCount = 0;
	m_iMaxConscript = 0;
	m_iHighestUnitLevel = 1;
	m_iOverflowResearch = 0;
	m_iNoUnhealthyPopulationCount = 0;
	m_iExpInBorderModifier = 0;
	m_iBuildingOnlyHealthyCount = 0;
	m_iDistanceMaintenanceModifier = 0;
	m_iNumCitiesMaintenanceModifier = 0;
	m_iCorporationMaintenanceModifier = 0;
	m_iTotalMaintenance = 0;
	m_iUpkeepModifier = 0;
	m_iLevelExperienceModifier = 0;
	m_iExtraHealth = 0;
	m_iBuildingGoodHealth = 0;
	m_iBuildingBadHealth = 0;
	m_iExtraHappiness = 0;
	m_iBuildingHappiness = 0;
	m_iLargestCityHappiness = 0;
	m_iWarWearinessPercentAnger = 0;
	m_iWarWearinessModifier = 0;
	m_iFreeSpecialist = 0;
	m_iNoForeignTradeCount = 0;
	m_iNoCorporationsCount = 0;
	m_iNoForeignCorporationsCount = 0;
	m_iCoastalTradeRoutes = 0;
	m_iTradeRoutes = 0;
	m_iRevolutionTimer = 0;
	m_iConversionTimer = 0;
	m_iStateReligionCount = 0;
	m_iNoNonStateReligionSpreadCount = 0;
	m_iStateReligionHappiness = 0;
	m_iNonStateReligionHappiness = 0;
	m_iStateReligionUnitProductionModifier = 0;
	m_iStateReligionBuildingProductionModifier = 0;
	m_iStateReligionFreeExperience = 0;
	m_iCapitalCityID = FFreeList::INVALID_INDEX;
	m_iCitiesLost = 0;
	m_iWinsVsBarbs = 0;
	m_iAssets = 0;
	m_iPower = 0;
	m_iPopulationScore = 0;
	m_iLandScore = 0;
	m_iTechScore = 0;
	m_iWondersScore = 0;
	m_iCombatExperience = 0;
	m_iPopRushHurryCount = 0;
	m_iInflationModifier = 0;
	m_uiStartTime = 0;
	m_iCrime = 0;
	m_iCaptureCityGoldPercentChange = 0;

	m_bAlive = false;
	m_bEverAlive = false;
	m_bTurnActive = false;
	m_bAutoMoves = false;
	m_bEndTurn = false;
	m_bPbemNewTurn = false;
	m_bExtendedGame = false;
	m_bFoundedFirstCity = false;
	m_bStrike = false;

/*************************************************************************************************/
/**	BETTER AI (New Functions Definition) Sephi                                 					**/
/**																								**/
/**						                                            							**/
/*************************************************************************************************/
    m_bConquestMode =false;
    m_eFavoriteReligion=NO_RELIGION;
    m_iTowerVicFlag =0;

	m_bRangedAttackMode = false;

    m_iFaith = 0;
	m_iFaithIncome = 0;
	m_iReligionsSpread = 0;
    m_iFaithMod = 0;
    m_iManaBonus2 = NO_BONUS;
    m_iManaBonus3 = NO_BONUS;
    m_iManaBonus4 = NO_BONUS;
    m_iBarbarianPower = 100;
    m_iBarbarianPoints = 0;
    m_iBarbarianValue2 = 0;
    m_iBarbarianValue3 = 0;
    m_iMana = 0;
	m_iManaIncome = 0;
    m_iArcane =0;
    m_iArcaneLevel =0;
    m_iFaithUpkeep =0;
    m_iManaUpkeep =0;
    m_iPopulationLimit = -1;
    m_iCityStateType = NO_CITYSTATE;
	m_iCityStateCounter = 0;
    m_iPurityCounter = 0;
    m_iPurityCounterCache1 = 0;
	m_iUnitSupportLimit = 0;
	m_iUnitSupportUsed = 0;
    m_iReducedEquipmentCost = 0;
	m_iImprovementDiscountPercent = 0;
    m_iGreatPersonRatePerCulture = 0;
    m_iHealthRatePerCulture = 0;
	m_iWillpowerChange = 0;
	m_iReducedTerraformCost = 0;
	m_iReducedEnchantmentCost = 0;
	m_iReducedSummonCost = 0;
	m_iIncreaseTerraformTiles = 0;
	m_iIncreaseHostileTerraformTiles = 0;
	m_iIncreaseSummonXP = 0;

	m_iCurrentMagicRitual = -1;
	m_iCurrentMagicRitualManaNeeded = 0;
	m_iMaxMana = 300;
	m_iManaFromSpellResearch = 0;
	m_iManaFromNodes = 0;	
	m_iIncreaseStrengthOfRegularUnits = 0;
    m_iAI_lostCitiesRevolutionMemory = 0;
	m_iManaCreationModifier = 0;
	m_iResistHostileTerraforming = 0;
	m_iTerraformPlan = -1;
	m_iIgnoreUnHealthFromImprovements = 0;
	m_iSpellResearchFocus = -1;
	m_iGlobalCulture = 0;
	m_iTotalCityInfastructureLevel = 0;
	m_iTotalTraitsFromCulture = 0;
	m_iExtraImprovementPerCity = 0;
	m_iExtraManaNodesPercent = 0;
	m_iReducedHurryCostBuilding = 0;
	m_iExtraSupportUnitLimit = 0;
	m_iExtraBuildingCapturedChance = 0;
/**	New Esus                                                                 					**/
    m_iPrestige = 0;

/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
//FfH: Added by Kael 04/11/2008
	m_bAdaptive = false;
	m_bAgnostic = false;
	m_bAssimilation = false;
	m_bDeclaringWar = false;
	m_bDisableHuman = false;
	m_bHideUnits = false;
	m_bIgnoreFood = false;
	m_bInsane = false;
	m_bSeeInvisible = false;
	m_bSprawling = false;
	m_iAlignment = NO_ALIGNMENT;
	m_iDisableProduction = 0;
	m_iDisableResearch = 0;
	m_iDisableSpellcasting = 0;
	m_iDiscoverRandModifier = 0;
    m_iEnslavementChance = 0;
	m_iFreeXPFromCombat = 0;
	m_iHealChange = 0;
	m_iHealChangeEnemy = 0;
	m_iMaxCities = -1;
	m_iNoDiplomacyWithEnemies = 0;
	m_iPillagingGold = 0;
	m_iPlayersKilled = 0;
	m_iRealPlayer = NO_PLAYER;
	m_iResistEnemyModify = 0;
	m_iResistModify = 0;
	m_iSanctuaryTimer = 0;
	m_iStartingGold = 0;
	m_iSummonCostReduction = 0;
	m_iTaxesModifier = 0;
	m_iFoodPerPopulationModifier = 0;
	m_iTempPlayerTimer = 0;
	m_iUpgradeCostModifier = 0;
	m_iProductionCostReductionCapitalBuilding = 0;
	m_iUnitSupportModifier = 0;
//FfH: End Add

/*************************************************************************************************/
/**	New Gameoption (Passive XP Training) Sephi                                				    **/
/*************************************************************************************************/
	if (!bConstructorCall)
	{
		FAssertMsg(m_paiTrainXPCap==NULL, "about to leak memory, CvPlayer::m_paiTrainXPCap");
		FAssertMsg(m_paiTrainXPRate==NULL, "about to leak memory, CvPlayer::m_paiTrainXPRate");
		m_paiTrainXPCap = new int[GC.getNumUnitCombatInfos()];
        m_paiTrainXPRate = new int[GC.getNumUnitCombatInfos()];
	    for (iI = 0; iI < GC.getNumUnitCombatInfos(); iI++)
		{
			m_paiTrainXPCap[iI] = 0;
            m_paiTrainXPRate[iI] = 0;
		}
/**	ADDON (Houses of Erebus) Sephi			                                 					**/

		FAssertMsg(m_pabCorporationSupport==NULL, "about to leak memory, CvPlayer::m_pabCorporationSupport");
		FAssertMsg(m_paiCorporationSupport==NULL, "about to leak memory, CvPlayer::m_paiCorporationSupport");
		FAssertMsg(m_paiCorporationSupportMod==NULL, "about to leak memory, CvPlayer::m_paiCorporationSupportMod");
		m_pabCorporationSupport = new bool[GC.getNumCorporationInfos()];
        m_paiCorporationSupport = new int[GC.getNumCorporationInfos()];
        m_paiCorporationSupportMod = new int[GC.getNumCorporationInfos()];
	    for (iI = 0; iI < GC.getNumCorporationInfos(); iI++)
		{
			m_pabCorporationSupport[iI] = false;
            m_paiCorporationSupport[iI] = 0;
            m_paiCorporationSupportMod[iI] = 0;
		}

/**	ADDON (Adventures) Sephi                                                      				**/
		FAssertMsg(m_pabAdventureEnabled==NULL, "about to leak memory, CvPlayer::m_pabAdventureEnabled");
		FAssertMsg(m_pabAdventureFinished==NULL, "about to leak memory, CvPlayer::m_pabAdventureFinished");
		FAssertMsg(m_ppaaiAdventureCounter==NULL, "about to leak memory, CvPlayer::m_ppaaiAdventureCounter");
		m_pabAdventureEnabled = new bool[GC.getNumAdventureInfos()];
		m_pabAdventureFinished = new bool[GC.getNumAdventureInfos()];

	    for (iI = 0; iI < GC.getNumAdventureInfos(); iI++)
		{
			m_pabAdventureEnabled[iI] = false;
			m_pabAdventureFinished[iI] = false;
		}
		m_ppaaiAdventureCounter = new int*[GC.getNumAdventureInfos()];
	    for (iI = 0; iI < GC.getNumAdventureInfos(); iI++)
		{
			m_ppaaiAdventureCounter[iI] = new int[GC.getMAX_ADVENTURESTEPS()];
		    for(iJ = 0; iJ <GC.getMAX_ADVENTURESTEPS(); iJ++)
		    {
		        m_ppaaiAdventureCounter[iI][iJ] = 0;
		    }
		}

/**	ADDON (New Mana) Sephi **/
		FAssertMsg(m_paiSpellDamageModify==NULL, "about to leak memory, CvPlayer::m_paiSpellDamageModify");
		m_paiSpellDamageModify = new int[GC.getNumDamageTypeInfos()];
		for (iI = 0; iI < GC.getNumDamageTypeInfos(); iI++)
		{
			m_paiSpellDamageModify[iI] = 0;
		}

		FAssertMsg(m_pabCanEverSpellResearch==NULL, "about to leak memory, CvPlayer::m_pabCanEverSpellResearch");
		m_pabCanEverSpellResearch = new bool[GC.getNumTechInfos()];
	    for (iI = 0; iI < GC.getNumTechInfos(); iI++)
		{
			m_pabCanEverSpellResearch[iI] = true;
		}

		FAssertMsg(m_paiManaSchoolCostModifier==NULL, "about to leak memory, CvPlayer::m_paiArcaneschoolsAdvance");
		m_paiManaSchoolCostModifier = new int[GC.getNumManaschoolInfos()];
	    for (iI = 0; iI < GC.getNumManaschoolInfos(); iI++)
		{
			m_paiManaSchoolCostModifier[iI] = 0;
		}

		FAssertMsg(m_paiGlobalYield==NULL, "about to leak memory, CvPlayer::m_paiGlobalYield");
		m_paiGlobalYield = new int[GC.getNUM_YIELD_TYPES()];
	    for (iI = 0; iI < GC.getNUM_YIELD_TYPES(); iI++)
		{
			m_paiGlobalYield[iI] = 0;
		}

		FAssertMsg(m_paiGlobalYieldUpkeep==NULL, "about to leak memory, CvPlayer::m_paiGlobalYieldUpkeep");
		m_paiGlobalYieldUpkeep = new int[GC.getNUM_YIELD_TYPES()];
	    for (iI = 0; iI < GC.getNUM_YIELD_TYPES(); iI++)
		{
			m_paiGlobalYieldUpkeep[iI] = 0;
		}

		FAssertMsg(m_paiGlobalYieldNeededForEquipmentBonus==NULL, "about to leak memory, CvPlayer::m_paiGlobalYieldUpkeep");
		m_paiGlobalYieldNeededForEquipmentBonus = new int[GC.getNUM_YIELD_TYPES()];
	    for (iI = 0; iI < GC.getNUM_YIELD_TYPES(); iI++)
		{
			m_paiGlobalYieldNeededForEquipmentBonus[iI] = 0;
		}

		FAssertMsg(m_paiNumBonusesConsumed==NULL, "about to leak memory, CvPlayer::m_paiNumBonusesConsumed");
		m_paiNumBonusesConsumed = new int[GC.getNumBonusInfos()];
	    for (iI = 0; iI < GC.getNumBonusInfos(); iI++)
		{
			m_paiNumBonusesConsumed[iI] = 0;
		}

		FAssertMsg(m_paiNumBonusesConsumedByProduction==NULL, "about to leak memory, CvPlayer::m_paiNumBonusesConsumedByProduction");
		m_paiNumBonusesConsumedByProduction = new int[GC.getNumBonusInfos()];
	    for (iI = 0; iI < GC.getNumBonusInfos(); iI++)
		{
			m_paiNumBonusesConsumedByProduction[iI] = 0;
		}

		FAssertMsg(m_paiFoundHolyCityCount==NULL, "about to leak memory, CvPlayer::m_paiFoundHolyCityCount");
		m_paiFoundHolyCityCount = new int[GC.getNumReligionInfos()];
	    for (iI = 0; iI < GC.getNumReligionInfos(); iI++)
		{
			m_paiFoundHolyCityCount[iI] = 0;
		}

		FAssertMsg(m_paiManaFromImprovement==NULL, "about to leak memory, CvPlayer::m_paiFoundHolyCityCount");
		m_paiManaFromImprovement = new int[GC.getNumImprovementInfos()];
	    for (iI = 0; iI < GC.getNumImprovementInfos(); iI++)
		{
			m_paiManaFromImprovement[iI] = 0;
		}

		FAssertMsg(m_paiFaithFromImprovement==NULL, "about to leak memory, CvPlayer::m_paiFaithFromImprovement");
		m_paiFaithFromImprovement = new int[GC.getNumImprovementInfos()];
	    for (iI = 0; iI < GC.getNumImprovementInfos(); iI++)
		{
			m_paiFaithFromImprovement[iI] = 0;
		}

		FAssertMsg(m_paiIgnoreBuildingGYCostCount==NULL, "about to leak memory, CvPlayer::m_paiFoundHolyCityCount");
		m_paiIgnoreBuildingGYCostCount = new int[GC.getNumBuildingClassInfos()];
	    for (iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
		{
			m_paiIgnoreBuildingGYCostCount[iI] = 0;
		}

	}
/*************************************************************************************************/
/**	    									END													**/
/*************************************************************************************************/

	m_eID = eID;
	updateTeamType();
	updateHuman();

	if (m_eID != NO_PLAYER)
	{
		m_ePersonalityType = GC.getInitCore().getLeader(m_eID); //??? Is this repeated data???
	}
	else
	{
		m_ePersonalityType = NO_LEADER;
	}
	m_eCurrentEra = ((EraTypes)0);  //??? Is this repeated data???
	m_eLastStateReligion = NO_RELIGION;
	m_eParent = NO_PLAYER;

	for (iI = 0; iI < NUM_YIELD_TYPES; iI++)
	{
		m_aiSeaPlotYield[iI] = 0;
		m_aiYieldRateModifier[iI] = 0;
		m_aiCapitalYieldRateModifier[iI] = 0;
		m_aiExtraYieldThreshold[iI] = 0;
		m_aiTradeYieldModifier[iI] = 0;
	}

	for (iI = 0; iI < NUM_COMMERCE_TYPES; iI++)
	{
		m_aiFreeCityCommerce[iI] = 0;
		m_aiCommercePercent[iI] = 0;
		m_aiCommerceRate[iI] = 0;
		m_aiCommerceRateModifier[iI] = 0;
		m_aiCapitalCommerceRateModifier[iI] = 0;
		m_aiCommerceChangesPerCultureLevel[iI] = 0;
		m_aiStateReligionBuildingCommerce[iI] = 0;
		m_aiSpecialistExtraCommerce[iI] = 0;
		m_aiCommerceFlexibleCount[iI] = 0;
	}

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		m_aiGoldPerTurnByPlayer[iI] = 0;
		if (!bConstructorCall && getID() != NO_PLAYER)
		{
			GET_PLAYER((PlayerTypes) iI).m_aiGoldPerTurnByPlayer[getID()] = 0;
		}
	}

	for (iI = 0; iI < MAX_TEAMS; iI++)
	{
		m_aiEspionageSpendingWeightAgainstTeam[iI] = 0;

		if (!bConstructorCall && getTeam() != NO_TEAM)
		{
			for (iJ = 0; iJ < MAX_PLAYERS; iJ++)
			{
				if (GET_PLAYER((PlayerTypes) iJ).getTeam() == iI)
				{
					GET_PLAYER((PlayerTypes) iJ).setEspionageSpendingWeightAgainstTeam(getTeam(), 0);
				}
			}
		}
	}

	for (iI = 0; iI < NUM_FEAT_TYPES; iI++)
	{
		m_abFeatAccomplished[iI] = false;
	}

	for (iI = 0; iI < NUM_PLAYEROPTION_TYPES; iI++)
	{
		m_abOptions[iI] = false;
	}

	m_szScriptData = "";

	if (!bConstructorCall)
	{
		FAssertMsg(0 < GC.getNumBonusInfos(), "GC.getNumBonusInfos() is not greater than zero but it is used to allocate memory in CvPlayer::reset");
		FAssertMsg(m_paiBonusExport==NULL, "about to leak memory, CvPlayer::m_paiBonusExport");
		m_paiBonusExport = new int [GC.getNumBonusInfos()];
		FAssertMsg(m_paiBonusImport==NULL, "about to leak memory, CvPlayer::m_paiBonusImport");
		m_paiBonusImport = new int [GC.getNumBonusInfos()];
		for (iI = 0; iI < GC.getNumBonusInfos(); iI++)
		{
			m_paiBonusExport[iI] = 0;
			m_paiBonusImport[iI] = 0;
		}

		FAssertMsg(0 < GC.getNumImprovementInfos(), "GC.getNumImprovementInfos() is not greater than zero but it is used to allocate memory in CvPlayer::reset");
		FAssertMsg(m_paiImprovementCount==NULL, "about to leak memory, CvPlayer::m_paiImprovementCount");
		m_paiImprovementCount = new int [GC.getNumImprovementInfos()];
		for (iI = 0; iI < GC.getNumImprovementInfos(); iI++)
		{
			m_paiImprovementCount[iI] = 0;
		}

		FAssertMsg(m_paiFreeBuildingCount==NULL, "about to leak memory, CvPlayer::m_paiFreeBuildingCount");
		m_paiFreeBuildingCount = new int [GC.getNumBuildingInfos()];
		FAssertMsg(m_paiExtraBuildingHappiness==NULL, "about to leak memory, CvPlayer::m_paiExtraBuildingHappiness");
		m_paiExtraBuildingHappiness = new int [GC.getNumBuildingInfos()];
		FAssertMsg(m_paiExtraBuildingHealth==NULL, "about to leak memory, CvPlayer::m_paiExtraBuildingHealth");
		m_paiExtraBuildingHealth = new int [GC.getNumBuildingInfos()];
		for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			m_paiFreeBuildingCount[iI] = 0;
			m_paiExtraBuildingHappiness[iI] = 0;
			m_paiExtraBuildingHealth[iI] = 0;
		}

		FAssertMsg(m_paiFeatureHappiness==NULL, "about to leak memory, CvPlayer::m_paiFeatureHappiness");
		m_paiFeatureHappiness = new int [GC.getNumFeatureInfos()];
		for (iI = 0; iI < GC.getNumFeatureInfos(); iI++)
		{
			m_paiFeatureHappiness[iI] = 0;
		}

		FAssertMsg(m_paiUnitClassCount==NULL, "about to leak memory, CvPlayer::m_paiUnitClassCount");
		m_paiUnitClassCount = new int [GC.getNumUnitClassInfos()];
		FAssertMsg(m_paiUnitClassMaking==NULL, "about to leak memory, CvPlayer::m_paiUnitClassMaking");
		m_paiUnitClassMaking = new int [GC.getNumUnitClassInfos()];
		for (iI = 0; iI < GC.getNumUnitClassInfos(); iI++)
		{
			m_paiUnitClassCount[iI] = 0;
			m_paiUnitClassMaking[iI] = 0;
		}

		FAssertMsg(m_paiBuildingClassCount==NULL, "about to leak memory, CvPlayer::m_paiBuildingClassCount");
		m_paiBuildingClassCount = new int [GC.getNumBuildingClassInfos()];
		FAssertMsg(m_paiBuildingClassMaking==NULL, "about to leak memory, CvPlayer::m_paiBuildingClassMaking");
		m_paiBuildingClassMaking = new int [GC.getNumBuildingClassInfos()];
		for (iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
		{
			m_paiBuildingClassCount[iI] = 0;
			m_paiBuildingClassMaking[iI] = 0;
		}

		FAssertMsg(m_paiHurryCount==NULL, "about to leak memory, CvPlayer::m_paiHurryCount");
		m_paiHurryCount = new int [GC.getNumHurryInfos()];
		for (iI = 0; iI < GC.getNumHurryInfos(); iI++)
		{
			m_paiHurryCount[iI] = 0;
		}

		FAssertMsg(m_paiSpecialBuildingNotRequiredCount==NULL, "about to leak memory, CvPlayer::m_paiSpecialBuildingNotRequiredCount");
		m_paiSpecialBuildingNotRequiredCount = new int [GC.getNumSpecialBuildingInfos()];
		for (iI = 0; iI < GC.getNumSpecialBuildingInfos(); iI++)
		{
			m_paiSpecialBuildingNotRequiredCount[iI] = 0;
		}

		FAssertMsg(m_paiHasCivicOptionCount==NULL, "about to leak memory, CvPlayer::m_paiHasCivicOptionCount");
		m_paiHasCivicOptionCount = new int[GC.getNumCivicOptionInfos()];
		FAssertMsg(m_paiNoCivicUpkeepCount==NULL, "about to leak memory, CvPlayer::m_paiNoCivicUpkeepCount");
		m_paiNoCivicUpkeepCount = new int[GC.getNumCivicOptionInfos()];
		FAssertMsg(m_paeCivics==NULL, "about to leak memory, CvPlayer::m_paeCivics");
		m_paeCivics = new CivicTypes [GC.getNumCivicOptionInfos()];
		for (iI = 0; iI < GC.getNumCivicOptionInfos(); iI++)
		{
			m_paiHasCivicOptionCount[iI] = 0;
			m_paiNoCivicUpkeepCount[iI] = 0;
			m_paeCivics[iI] = NO_CIVIC;
		}

		FAssertMsg(m_paiHasReligionCount==NULL, "about to leak memory, CvPlayer::m_paiHasReligionCount");
		m_paiHasReligionCount = new int[GC.getNumReligionInfos()];
		for (iI = 0;iI < GC.getNumReligionInfos();iI++)
		{
			m_paiHasReligionCount[iI] = 0;
		}

		FAssertMsg(m_paiHasCorporationCount==NULL, "about to leak memory, CvPlayer::m_paiHasReligionCount");
		m_paiHasCorporationCount = new int[GC.getNumCorporationInfos()];
		for (iI = 0;iI < GC.getNumCorporationInfos();iI++)
		{
			m_paiHasCorporationCount[iI] = 0;
		}

		FAssertMsg(m_pabResearchingTech==NULL, "about to leak memory, CvPlayer::m_pabResearchingTech");
		m_pabResearchingTech = new bool[GC.getNumTechInfos()];
		for (iI = 0; iI < GC.getNumTechInfos(); iI++)
		{
			m_pabResearchingTech[iI] = false;
		}

//FfH Scriptable Leader Traits: Added by Kael 08/08/2007
		FAssertMsg(m_pbTraits==NULL, "about to leak memory, CvPlayer::m_pbTraits");
		m_pbTraits = new bool[GC.getNumTraitInfos()];
		for (iI = 0; iI < GC.getNumTraitInfos(); iI++)
		{
			m_pbTraits[iI] = false;
		}
		m_ppaaiSpecialistTypeExtraCommerce = new int*[GC.getNumSpecialistInfos()];
		for (iI = 0; iI < GC.getNumSpecialistInfos(); iI++)
		{
			m_ppaaiSpecialistTypeExtraCommerce[iI] = new int[NUM_COMMERCE_TYPES];
			for (iJ = 0; iJ < NUM_COMMERCE_TYPES; iJ++)
			{
				m_ppaaiSpecialistTypeExtraCommerce[iI][iJ] = 0;
			}
		}
//FfH: End Add

		FAssertMsg(m_pabLoyalMember==NULL, "about to leak memory, CvPlayer::m_pabLoyalMember");
		m_pabLoyalMember = new bool[GC.getNumVoteSourceInfos()];
		for (iI = 0; iI < GC.getNumVoteSourceInfos(); iI++)
		{
			m_pabLoyalMember[iI] = true;
		}

		FAssertMsg(0 < GC.getNumUpkeepInfos(), "GC.getNumUpkeepInfos() is not greater than zero but it is used to allocate memory in CvPlayer::reset");
		FAssertMsg(m_paiUpkeepCount==NULL, "about to leak memory, CvPlayer::m_paiUpkeepCount");
		m_paiUpkeepCount = new int[GC.getNumUpkeepInfos()];
		for (iI = 0; iI < GC.getNumUpkeepInfos(); iI++)
		{
			m_paiUpkeepCount[iI] = 0;
		}

		FAssertMsg(0 < GC.getNumSpecialistInfos(), "GC.getNumSpecialistInfos() is not greater than zero but it is used to allocate memory in CvPlayer::reset");
		FAssertMsg(m_paiSpecialistValidCount==NULL, "about to leak memory, CvPlayer::m_paiSpecialistValidCount");
		m_paiSpecialistValidCount = new int[GC.getNumSpecialistInfos()];
		for (iI = 0; iI < GC.getNumSpecialistInfos(); iI++)
		{
			m_paiSpecialistValidCount[iI] = 0;
		}

		FAssertMsg(0 < GC.getNumSpecialistInfos(), "GC.getNumSpecialistInfos() is not greater than zero but it is used to allocate memory in CvPlayer::reset");
		FAssertMsg(m_ppaaiSpecialistExtraYield==NULL, "about to leak memory, CvPlayer::m_ppaaiSpecialistExtraYield");
		m_ppaaiSpecialistExtraYield = new int*[GC.getNumSpecialistInfos()];
		for (iI = 0; iI < GC.getNumSpecialistInfos(); iI++)
		{
			m_ppaaiSpecialistExtraYield[iI] = new int[NUM_YIELD_TYPES];
			for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++)
			{
				m_ppaaiSpecialistExtraYield[iI][iJ] = 0;
			}
		}

		FAssertMsg(m_ppaaiImprovementYieldChange==NULL, "about to leak memory, CvPlayer::m_ppaaiImprovementYieldChange");
		m_ppaaiImprovementYieldChange = new int*[GC.getNumImprovementInfos()];
		for (iI = 0; iI < GC.getNumImprovementInfos(); iI++)
		{
			m_ppaaiImprovementYieldChange[iI] = new int[NUM_YIELD_TYPES];
			for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++)
			{
				m_ppaaiImprovementYieldChange[iI][iJ] = 0;
			}
		}

		m_mapEventsOccured.clear();
		m_mapEventCountdown.clear();
		m_aFreeUnitCombatPromotions.clear();
		m_aFreeUnitClassPromotions.clear();
		m_aVote.clear();
		m_aUnitExtraCosts.clear();
		m_triggersFired.clear();
	}

	m_plotGroups.removeAll();

	m_cities.removeAll();

	m_units.removeAll();

	m_selectionGroups.removeAll();

	m_eventsTriggered.removeAll();
/*************************************************************************************************/
/**	ADDON (Combatauras) Sephi																	**/
/*************************************************************************************************/
	m_combatAuras.removeAll();
	m_AIGroups.removeAll();
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

	if (!bConstructorCall)
	{
		AI_reset(false);
	}
/*************************************************************************************************/
/**	ADDON (MultiBarb)  merged Sephi		12/23/08									Xienwolf	**/
/**	adjusted for new barb teams	    															**/
/**				Workaround required until source of this call for Barbarians is located			**/
/*************************************************************************************************/
	if (!bConstructorCall)
	{
		if (getID() == BARBARIAN_PLAYER)
		{
			for(int iI = BARBARIAN_PLAYER+1; iI < NUM_BARBARIAN_PLAYERS; iI++)
			{
				GET_PLAYER((PlayerTypes)iI).reset((PlayerTypes)iI, bConstructorCall);
			}
		}
	}
/*************************************************************************************************/
/**	MultiBarb								END													**/
/*************************************************************************************************/
}


//////////////////////////////////////
// graphical only setup
//////////////////////////////////////
void CvPlayer::setupGraphical()
{
	if (!GC.IsGraphicsInitialized())
		return;

	CvCity* pLoopCity;
	CvUnit* pLoopUnit;

	// Setup m_cities
	int iLoop;
	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->setupGraphical();
	}

	// Setup m_units
	for(pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{
		pLoopUnit->setupGraphical();
	}
}


void CvPlayer::initFreeState()
{
	setGold(0);
	changeGold(GC.getHandicapInfo(getHandicapType()).getStartingGold());
	changeGold(GC.getEraInfo(GC.getGameINLINE().getStartEra()).getStartingGold());

//FfH Traits: Added by Kael 08/02/2007
    changeGold(getStartingGold());
//FfH: End Add

	clearResearchQueue();
}


void CvPlayer::initFreeUnits()
{
	UnitTypes eLoopUnit;
	int iFreeCount;
	int iI, iJ;

	if (GC.getGameINLINE().isOption(GAMEOPTION_ADVANCED_START) && !isBarbarian())
	{
		int iPoints = GC.getGameINLINE().getNumAdvancedStartPoints();

		iPoints *= GC.getHandicapInfo(getHandicapType()).getAdvancedStartPointsMod();
		iPoints /= 100;

		if (!isHuman())
		{
			iPoints *= GC.getHandicapInfo(getHandicapType()).getAIAdvancedStartPercent();
			iPoints /= 100;
		}

		setAdvancedStartPoints(iPoints);

		// Starting visibility
		CvPlot* pStartingPlot = getStartingPlot();
		if (NULL != pStartingPlot)
		{
			for (int iPlotLoop = 0; iPlotLoop < GC.getMapINLINE().numPlots(); ++iPlotLoop)
			{
				CvPlot* pPlot = GC.getMapINLINE().plotByIndex(iPlotLoop);

				if (plotDistance(pPlot->getX_INLINE(), pPlot->getY_INLINE(), pStartingPlot->getX_INLINE(), pStartingPlot->getY_INLINE()) <= GC.getDefineINT("ADVANCED_START_SIGHT_RANGE"))
				{
					pPlot->setRevealed(getTeam(), true, false, NO_TEAM, false);
				}
			}
		}
	}
	else
	{
		for (iI = 0; iI < GC.getNumUnitClassInfos(); iI++)
		{
			eLoopUnit = (UnitTypes)GC.getCivilizationInfo(getCivilizationType()).getCivilizationUnits(iI);

			if (eLoopUnit != NO_UNIT)
			{
				iFreeCount = GC.getCivilizationInfo(getCivilizationType()).getCivilizationFreeUnitsClass(iI);

				iFreeCount *= (GC.getEraInfo(GC.getGameINLINE().getStartEra()).getStartingUnitMultiplier() + ((!isHuman()) ? GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIStartingUnitMultiplier() : 0));

				for (iJ = 0; iJ < iFreeCount; iJ++)
				{
					addFreeUnit(eLoopUnit);
				}
			}
		}

		UnitTypes eDefaultUnit = (UnitTypes)GC.getCivilizationInfo(getCivilizationType()).getDefaultUnit();
		if(eDefaultUnit != NO_UNIT) {
			for (iJ = 0; iJ < 5; iJ++)
			{
				addFreeUnit(eDefaultUnit);
			}

			if(GC.getCivilizationInfo(getCivilizationType()).getHero() != NO_UNIT) {
				addFreeUnit((UnitTypes)GC.getCivilizationInfo(getCivilizationType()).getHero());
			}
		}

		iFreeCount = GC.getEraInfo(GC.getGameINLINE().getStartEra()).getStartingDefenseUnits();
		iFreeCount += GC.getHandicapInfo(getHandicapType()).getStartingDefenseUnits();

		if (!isHuman())
		{
			iFreeCount += GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIStartingDefenseUnits();
		}

		if (iFreeCount > 0)
		{
			addFreeUnitAI(UNITAI_CITY_DEFENSE, iFreeCount);
		}

		iFreeCount = GC.getEraInfo(GC.getGameINLINE().getStartEra()).getStartingWorkerUnits();
		iFreeCount += GC.getHandicapInfo(getHandicapType()).getStartingWorkerUnits();

		if (!isHuman())
		{
			iFreeCount += GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIStartingWorkerUnits();
		}

		if (iFreeCount > 0)
		{
			addFreeUnitAI(UNITAI_WORKER, iFreeCount);
		}

		iFreeCount = GC.getEraInfo(GC.getGameINLINE().getStartEra()).getStartingExploreUnits();
		iFreeCount += GC.getHandicapInfo(getHandicapType()).getStartingExploreUnits();

		if (!isHuman())
		{
			iFreeCount += GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIStartingExploreUnits();
		}

		if (iFreeCount > 0)
		{
			addFreeUnitAI(UNITAI_EXPLORE, iFreeCount);
		}
	}
}


void CvPlayer::addFreeUnitAI(UnitAITypes eUnitAI, int iCount)
{
	UnitTypes eLoopUnit;
	UnitTypes eBestUnit;
	bool bValid;
	int iValue;
	int iBestValue;
	int iI, iJ;

	eBestUnit = NO_UNIT;
	iBestValue = 0;

	for (iI = 0; iI < GC.getNumUnitClassInfos(); iI++)
	{
		eLoopUnit = (UnitTypes)GC.getCivilizationInfo(getCivilizationType()).getCivilizationUnits(iI);

		if (eLoopUnit != NO_UNIT)
		{
			if (canTrain(eLoopUnit)

//FfH: Added by Kael 07/30/2009
              && !isWorldUnitClass((UnitClassTypes)iI)
//FfH: End Add

			)
			{
				bValid = true;

				if(GC.getUnitInfo(eLoopUnit).getPrereqCiv()!=NO_CIVILIZATION)
				{
					if(GC.getUnitInfo(eLoopUnit).getPrereqCiv()!=getCivilizationType())
					{
						bValid = false;
					}
				}

				if (GC.getUnitInfo(eLoopUnit).getPrereqAndBonus() != NO_BONUS)
				{
					bValid = false;
				}

				for (iJ = 0; iJ < GC.getNUM_UNIT_PREREQ_OR_BONUSES(); iJ++)
				{
					if (GC.getUnitInfo(eLoopUnit).getPrereqOrBonuses(iJ) != NO_BONUS)
					{
						bValid = false;
					}
				}

				if (bValid)
				{
					iValue = AI_unitValue(eLoopUnit, eUnitAI, NULL);

					if (iValue > iBestValue)
					{
						eBestUnit = eLoopUnit;
						iBestValue = iValue;
					}
				}
			}
		}
	}

	if (eBestUnit != NO_UNIT)
	{
		for (iI = 0; iI < iCount; iI++)
		{
			addFreeUnit(eBestUnit, eUnitAI);
		}
	}
}


void CvPlayer::addFreeUnit(UnitTypes eUnit, UnitAITypes eUnitAI)
{
	CvPlot* pStartingPlot;
	CvPlot* pBestPlot;
	CvPlot* pLoopPlot;
	int iRandOffset;
	int iI;

	if (GC.getGameINLINE().isOption(GAMEOPTION_ONE_CITY_CHALLENGE) && isHuman())
	{
		if ((eUnitAI == UNITAI_SETTLE) || (GC.getUnitInfo(eUnit).getDefaultUnitAIType() == UNITAI_SETTLE))
		{
			if (AI_getNumAIUnits(UNITAI_SETTLE) >= 1)
			{
				return;
			}
		}
	}

	//added Sephi, fix for double Lucian
	if(isWorldUnitClass((UnitClassTypes)GC.getUnitInfo(eUnit).getUnitClassType()))
	{
		if(GC.getGame().isUnitClassMaxedOut((UnitClassTypes)GC.getUnitInfo(eUnit).getUnitClassType()))
		{
			return;
		}
	}

	pStartingPlot = getStartingPlot();

	if (pStartingPlot != NULL)
	{
		pBestPlot = NULL;

		if (isHuman())
		{
			long lResult=0;
			gDLL->getPythonIFace()->callFunction(gDLL->getPythonIFace()->getMapScriptModule(), "startHumansOnSameTile", NULL, &lResult);
			if (lResult == 0)
			{
				if (!(GC.getUnitInfo(eUnit).isFound()))
				{

//FfH: Modified by Kael 09/16/2008
//					iRandOffset = GC.getGameINLINE().getSorenRandNum(NUM_CITY_PLOTS, "Place Units (Player)");
//					for (iI = 0; iI < NUM_CITY_PLOTS; iI++)
					iRandOffset = GC.getGameINLINE().getSorenRandNum(21, "Place Units (Player)");
					for (iI = 0; iI < 21; iI++)
//FfH: End Modify

					{
						pLoopPlot = plotCity(pStartingPlot->getX_INLINE(), pStartingPlot->getY_INLINE(), ((iI + iRandOffset) % NUM_CITY_PLOTS));

						if (pLoopPlot != NULL)
						{
							if (pLoopPlot->getArea() == pStartingPlot->getArea() && !pLoopPlot->isCoastalLand())
							{
								if (!(pLoopPlot->isImpassable()))
								{
									if (!(pLoopPlot->isUnit()))
									{

//FfH: Modified by Kael 09/16/2008
//										if (!(pLoopPlot->isGoody()))
										if (pLoopPlot->getImprovementType() == NO_IMPROVEMENT)
//FfH: End Modify

										{
											pBestPlot = pLoopPlot;
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		if (pBestPlot == NULL)
		{
			pBestPlot = pStartingPlot;
		}

		initUnit(eUnit, pBestPlot->getX_INLINE(), pBestPlot->getY_INLINE(), eUnitAI);

	}
}


int CvPlayer::startingPlotRange() const
{
	int iRange;

	iRange = (GC.getMapINLINE().maxStepDistance() + 10);

	iRange *= GC.getDefineINT("STARTING_DISTANCE_PERCENT");
	iRange /= 100;

	iRange *= (GC.getMapINLINE().getLandPlots() / (GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getTargetNumCities() * GC.getGameINLINE().countCivPlayersAlive()));

//FfH: Modified by Kael 11/18/2007
//	iRange /= NUM_CITY_PLOTS;
	iRange /= 21;
//FfH: End Modify

	iRange += std::min(((GC.getMapINLINE().getNumAreas() + 1) / 2), GC.getGameINLINE().countCivPlayersAlive());

	return std::max(iRange, GC.getDefineINT("MIN_CIV_STARTING_DISTANCE"));
}


bool CvPlayer::startingPlotWithinRange(CvPlot* pPlot, PlayerTypes ePlayer, int iRange, int iPass) const
{
	PROFILE_FUNC();

	//XXX changes to AI_foundValue (which are far more flexible) make this function
	//    redundant but it is still called from Python.
	return false;
}

int CvPlayer::startingPlotDistanceFactor(CvPlot* pPlot, PlayerTypes ePlayer, int iRange) const
{
	PROFILE_FUNC();

	FAssert(ePlayer != getID());

	CvPlot* pStartingPlot;

	int iValue = 1000;

	pStartingPlot = getStartingPlot();

	if (pStartingPlot != NULL)
	{
		if (GC.getGameINLINE().isTeamGame())
		{
			if (GET_PLAYER(ePlayer).getTeam() == getTeam())
			{
				iRange *= GC.getDefineINT("OWN_TEAM_STARTING_MODIFIER");
				iRange /= 100;
			}
			else
			{
				iRange *= GC.getDefineINT("RIVAL_TEAM_STARTING_MODIFIER");
				iRange /= 100;
			}
		}

		int iDistance = stepDistance(pPlot->getX_INLINE(), pPlot->getY_INLINE(), pStartingPlot->getX_INLINE(), pStartingPlot->getY_INLINE());
		if (pStartingPlot->getArea() != pPlot->getArea())
		{
			iDistance *= 4;
			iDistance /= 3;
		}

		iValue *= iDistance;
		iValue /= iRange ;

	}

	return std::max(1, iValue);

}


// Returns the id of the best area, or -1 if it doesn't matter:
int CvPlayer::findStartingArea() const
{
	PROFILE_FUNC();

	long result = -1;
	CyArgsList argsList;
	argsList.add(getID());		// pass in this players ID
	if (gDLL->getPythonIFace()->callFunction(gDLL->getPythonIFace()->getMapScriptModule(), "findStartingArea", argsList.makeFunctionArgs(), &result))
	{
		if (!gDLL->getPythonIFace()->pythonUsingDefaultImpl()) // Python override
		{
			if (result == -1 || GC.getMapINLINE().getArea(result) != NULL)
			{
				return result;
			}
			else
			{
				FAssertMsg(false, "python findStartingArea() must return -1 or the ID of a valid area");
			}
		}
	}

	int iBestValue = 0;
	int iBestArea = -1;
	int iValue;
	int iLoop = 0;

	CvArea *pLoopArea = NULL;

	// find best land area
	for(pLoopArea = GC.getMapINLINE().firstArea(&iLoop); pLoopArea != NULL; pLoopArea = GC.getMapINLINE().nextArea(&iLoop))
	{
		if (!(pLoopArea->isWater())
			&& (!GC.getMap().isPlanes() || pLoopArea->getPlane() == GC.getCivilizationInfo(getCivilizationType()).getNativePlane()))
		{
			// iNumPlayersOnArea is the number of players starting on the area, plus this player
			int iNumPlayersOnArea = (pLoopArea->getNumStartingPlots() + 1);
			int iTileValue = ((pLoopArea->calculateTotalBestNatureYield() + (pLoopArea->countCoastalLand() * 2) + pLoopArea->getNumRiverEdges() + (pLoopArea->getNumTiles())) + 1);
			iValue = iTileValue / iNumPlayersOnArea;

			iValue *= std::min(NUM_CITY_PLOTS + 1, pLoopArea->getNumTiles() + 1);
			iValue /= (NUM_CITY_PLOTS + 1);

			if (iNumPlayersOnArea <= 2)
			{
				iValue *= 4;
				iValue /= 3;
			}

			if (iValue > iBestValue)
			{
				iBestValue = iValue;
				iBestArea = pLoopArea->getID();
			}
		}
	}

	return iBestArea;
}


CvPlot* CvPlayer::findStartingPlot(bool bRandomize)
{
	PROFILE_FUNC();

	long result = -1;
	CyArgsList argsList;
	argsList.add(getID());		// pass in this players ID

	/** disabled for Planes - Sephi 

	if (gDLL->getPythonIFace()->callFunction(gDLL->getPythonIFace()->getMapScriptModule(), "findStartingPlot", argsList.makeFunctionArgs(), &result))
	{
		if (!gDLL->getPythonIFace()->pythonUsingDefaultImpl()) // Python override
		{
			CvPlot *pPlot = GC.getMapINLINE().plotByIndexINLINE(result);
			if (pPlot != NULL)
			{
				return pPlot;
			}
			else
			{
				FAssertMsg(false, "python findStartingPlot() returned an invalid plot index!");
			}
		}
	}

	**/ 

	CvPlot* pLoopPlot;
	int iBestArea = -1;
	int iValue;
	int iI;

	bool bNew = false;
	if (getStartingPlot() != NULL)
	{
		iBestArea = getStartingPlot()->getArea();
		setStartingPlot(NULL, true);
		bNew = true;
	}

	AI_updateFoundValues(true);//this sets all plots found values to -1

	/** Disabled for Planes - Sephi
	if (!bNew)
	{
		iBestArea = findStartingArea();
	}
	**/
//	for(int iPass = 0; iPass < GC.getMapINLINE().maxPlotDistance(); iPass++)
	for(int iPass = 0; iPass < 8; iPass++)
	{
		CvPlot *pBestPlot = NULL;
		int iBestValue = 0;

		for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
		{
			pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

//			if(pLoopPlot->getMinOriginalStartDist() == -1 || pLoopPlot->getMinOriginalStartDist() > 5) {
			if (canFound(pLoopPlot->getX_INLINE(), pLoopPlot->getY_INLINE())) {
				if ((iBestArea == -1) || (pLoopPlot->getArea() == iBestArea)) {
					//added Sephi
					if((iPass > 0) || (!GC.getMap().isPlanes()) ||
						(GC.getCivilizationInfo(getCivilizationType()).getNativePlane() == NO_PLANE 
						  || GC.getCivilizationInfo(getCivilizationType()).getNativePlane() == pLoopPlot->getPlane())) {

						//the distance factor is now done inside foundValue
						/** TODO: redo this. foundvalue shouldn't have to do multiple stuff
						
						iValue = pLoopPlot->getFoundValue(getID());

						if (bRandomize && iValue > 0)
						{
							iValue += GC.getGameINLINE().getSorenRandNum(10000, "Randomize Starting Location");
						}
						**/
						iValue = pLoopPlot->StartingPlotValue(getCivilizationType(), iPass);
						if(iPass == 7) {
							iValue += GC.getGameINLINE().getSorenRandNum(100, "Randomize Starting Location");
						}

						if (iValue > iBestValue)
						{
							iBestValue = iValue;
							pBestPlot = pLoopPlot;
						}
					}  // check for Planes
				}
			}
		}

		if (pBestPlot != NULL)
		{
			return pBestPlot;
		}

		FAssertMsg(iPass != 0, "CvPlayer::findStartingPlot - could not find starting plot in first pass.");
	}

	FAssertMsg(false, "Could not find starting plot.");
	return NULL;
}


CvPlotGroup* CvPlayer::initPlotGroup(CvPlot* pPlot)
{
	CvPlotGroup* pPlotGroup;

	pPlotGroup = addPlotGroup();

	FAssertMsg(pPlotGroup != NULL, "PlotGroup is not assigned a valid value");

	pPlotGroup->init(pPlotGroup->getID(), getID(), pPlot);

	return pPlotGroup;
}


CvCity* CvPlayer::initCity(int iX, int iY, bool bBumpUnits, bool bUpdatePlotGroups)
{
	PROFILE_FUNC();

	CvCity* pCity;

	pCity = addCity();

	FAssertMsg(pCity != NULL, "City is not assigned a valid value");
	FAssertMsg(!(GC.getMapINLINE().plotINLINE(iX, iY)->isCity()), "No city is expected at this plot when initializing new city");

	pCity->init(pCity->getID(), getID(), iX, iY, bBumpUnits, bUpdatePlotGroups);

	return pCity;
}


void CvPlayer::acquireCity(CvCity* pOldCity, bool bConquest, bool bTrade, bool bUpdatePlotGroups)
{
	CLLNode<IDInfo>* pUnitNode;
	CvCity* pNewCity;
	CvUnit* pLoopUnit;
	CvPlot* pCityPlot;
	CvPlot* pLoopPlot;
	bool* pabHasReligion;
	bool* pabHolyCity;
	bool* pabHasCorporation;
	bool* pabHeadquarters;
	int* paiNumRealBuilding;
	int* paiBuildingOriginalOwner;
	int* paiBuildingOriginalTime;
	int* paiCitySpecializationLevel;
	CvWString szBuffer;
	CvWString szName;
	bool abEverOwned[MAX_PLAYERS];
	int aiCulture[MAX_PLAYERS];
	PlayerTypes eOldOwner;
	PlayerTypes eOriginalOwner;
	PlayerTypes eHighestCulturePlayer;
	BuildingTypes eBuilding;
	bool bRecapture;
	bool bRaze;
	bool bGift;
	int iRange;
	int iCaptureGold;
	int iGameTurnFounded;
	int iPopulation;
	int iHighestPopulation;
	int iHurryAngerTimer;
	int iConscriptAngerTimer;
	int iDefyResolutionAngerTimer;
	int iOccupationTimer;
	int iTeamCulturePercent;
	int iDamage;
	int iDX, iDY;
	int iI;
	CLinkList<IDInfo> oldUnits;
	std::vector<int> aeFreeSpecialists;

	pCityPlot = pOldCity->plot();

	pUnitNode = pCityPlot->headUnitNode();

	while (pUnitNode != NULL)
	{
		oldUnits.insertAtEnd(pUnitNode->m_data);
		pUnitNode = pCityPlot->nextUnitNode(pUnitNode);
	}

	pUnitNode = oldUnits.head();

	while (pUnitNode != NULL)
	{
		pLoopUnit = ::getUnit(pUnitNode->m_data);
		pUnitNode = oldUnits.next(pUnitNode);

		if (pLoopUnit && pLoopUnit->getTeam() != getTeam())
		{
			if (pLoopUnit->getDomainType() == DOMAIN_IMMOBILE)
			{
				pLoopUnit->kill(false, getID());
			}
		}
	}

	if (bConquest)
	{
		iRange = pOldCity->getCultureLevel();

		for (iDX = -(iRange); iDX <= iRange; iDX++)
		{
			for (iDY = -(iRange); iDY <= iRange; iDY++)
			{
				if (pOldCity->cultureDistance(iDX, iDY) <= iRange)
				{
					pLoopPlot = plotXY(pOldCity->getX_INLINE(),pOldCity-> getY_INLINE(), iDX, iDY);

					if (pLoopPlot != NULL)
					{
						if (pLoopPlot->getOwnerINLINE() == pOldCity->getOwnerINLINE())
						{
							if (pLoopPlot->getNumCultureRangeCities(pOldCity->getOwnerINLINE()) == 1)
							{
								bool bForceUnowned = false;

								for (iI = 0; iI < MAX_PLAYERS; iI++)
								{
									if (GET_PLAYER((PlayerTypes)iI).isAlive())
									{
										if ((GET_PLAYER((PlayerTypes)iI).getTeam() != getTeam()) && (GET_PLAYER((PlayerTypes)iI).getTeam() != pOldCity->getTeam()))
										{
											if (pLoopPlot->getNumCultureRangeCities((PlayerTypes)iI) > 0)
											{
												bForceUnowned = true;
												break;
											}
										}
									}
								}

								if (bForceUnowned)
								{
									pLoopPlot->setForceUnownedTimer(GC.getDefineINT("FORCE_UNOWNED_CITY_TIMER"));
								}
							}
						}
					}
				}
			}
		}
	}

	if (pOldCity->getOriginalOwner() == pOldCity->getOwnerINLINE())
	{
		GET_PLAYER(pOldCity->getOriginalOwner()).changeCitiesLost(1);
	}
	else if (pOldCity->getOriginalOwner() == getID())
	{
		GET_PLAYER(pOldCity->getOriginalOwner()).changeCitiesLost(-1);
	}

	if (bConquest)
	{
		szBuffer = gDLL->getText("TXT_KEY_MISC_CAPTURED_CITY", pOldCity->getNameKey()).GetCString();
		gDLL->getInterfaceIFace()->addMessage(getID(), true, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_CITYCAPTURE", MESSAGE_TYPE_MAJOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("WORLDBUILDER_CITY_EDIT")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_GREEN"), pOldCity->getX_INLINE(), pOldCity->getY_INLINE(), true, true);

		szName.Format(L"%s (%s)", pOldCity->getName().GetCString(), GET_PLAYER(pOldCity->getOwnerINLINE()).getName());

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (iI != getID())
				{
					if (pOldCity->isRevealed(GET_PLAYER((PlayerTypes)iI).getTeam(), false))
					{
						szBuffer = gDLL->getText("TXT_KEY_MISC_CITY_CAPTURED_BY", szName.GetCString(), getCivilizationDescriptionKey());
						gDLL->getInterfaceIFace()->addMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_CITYCAPTURED", MESSAGE_TYPE_MAJOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("WORLDBUILDER_CITY_EDIT")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_RED"), pOldCity->getX_INLINE(), pOldCity->getY_INLINE(), true, true);
					}
				}
			}
		}

		szBuffer = gDLL->getText("TXT_KEY_MISC_CITY_WAS_CAPTURED_BY", szName.GetCString(), getCivilizationDescriptionKey());
		GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getID(), szBuffer, pOldCity->getX_INLINE(), pOldCity->getY_INLINE(), (ColorTypes)GC.getInfoTypeForString("COLOR_WARNING_TEXT"));
	}

	iCaptureGold = 0;

	if (bConquest)
	{
		long lCaptureGold;
		// Use python to determine city capture gold amounts...
		lCaptureGold = 0;

		CyCity* pyOldCity = new CyCity(pOldCity);

		CyArgsList argsList;
		argsList.add(gDLL->getPythonIFace()->makePythonObject(pyOldCity));	// pass in plot class

		gDLL->getPythonIFace()->callFunction(PYGameModule, "doCityCaptureGold", argsList.makeFunctionArgs(),&lCaptureGold);

		delete pyOldCity;	// python fxn must not hold on to this pointer

		iCaptureGold = (int)lCaptureGold;
		iCaptureGold = iCaptureGold * (100 + getCaptureCityGoldPercentChange()) / 100;
	}

	changeGold(iCaptureGold);
/*************************************************************************************************/
/**	ADDON (Mercenaries) Sephi	        								                    	**/
/*************************************************************************************************/
    if(GC.getGameINLINE().getMercenariesSupportTarget()!=NO_TEAM)
    {
        if(getID()==GC.getMERCENARIES_PLAYER() && GET_PLAYER(pOldCity->getOwnerINLINE()).getTeam()==GC.getGameINLINE().getMercenariesWarTarget())
        {
            for(int iI=0; iI<MAX_CIV_PLAYERS; iI++)
            {
                if(GET_PLAYER((PlayerTypes)iI).isAlive())
                {
                    if(GET_PLAYER((PlayerTypes)iI).getTeam()!=GET_PLAYER(pOldCity->getOwnerINLINE()).getTeam())
                    {
                        if(GET_PLAYER((PlayerTypes)iI).getCivilizationType()==GC.getInfoTypeForString("CIVILIZATION_HIPPUS"))
                        {
                            GET_PLAYER((PlayerTypes)iI).changeGold(iCaptureGold);
                        }
                    }
                }
            }
        }
    }
/*************************************************************************************************/
/**			        						END													**/
/*************************************************************************************************/

	pabHasReligion = new bool[GC.getNumReligionInfos()];
	pabHolyCity = new bool[GC.getNumReligionInfos()];
	pabHasCorporation = new bool[GC.getNumCorporationInfos()];
	pabHeadquarters = new bool[GC.getNumCorporationInfos()];
	paiNumRealBuilding = new int[GC.getNumBuildingInfos()];
	paiBuildingOriginalOwner = new int[GC.getNumBuildingInfos()];
	paiBuildingOriginalTime = new int[GC.getNumBuildingInfos()];

	paiCitySpecializationLevel = new int[NUM_CITYSPECIALIZATION_TYPES];

	for (iI = 0; iI < GC.getNumVoteSourceInfos(); ++iI)
	{
		pOldCity->processVoteSourceBonus((VoteSourceTypes)iI, false);
	}

	eOldOwner = pOldCity->getOwnerINLINE();
	eOriginalOwner = pOldCity->getOriginalOwner();
	eHighestCulturePlayer = pOldCity->findHighestCulture();
	iGameTurnFounded = pOldCity->getGameTurnFounded();
	iPopulation = pOldCity->getPopulation();
	iHighestPopulation = pOldCity->getHighestPopulation();
	iHurryAngerTimer = pOldCity->getHurryAngerTimer();
	iConscriptAngerTimer = pOldCity->getConscriptAngerTimer();
	iDefyResolutionAngerTimer = pOldCity->getDefyResolutionAngerTimer();
	iOccupationTimer = pOldCity->getOccupationTimer();
	szName = pOldCity->getNameKey();
	iDamage = pOldCity->getDefenseDamage();
	int iOldCityId = pOldCity->getID();

//FfH: Added by Kael 07/02/2008
	int iCiv = pOldCity->getTrueCivilizationType();
/*************************************************************************************************/
/**	ADDON (MultiBarb) merged Sephi			12/23/08								Xienwolf	**/
/**	adjusted for more barb teams																**/
/**							Adds extra Barbarian Civilizations									**/
/*************************************************************************************************/
/**								---- Start Original Code ----									**
	if (iCiv == GC.getDefineINT("BARBARIAN_CIVILIZATION"))
/**								----  End Original Code  ----									**/
	if (iCiv == GC.getDefineINT("BARBARIAN_CIVILIZATION")
	|| iCiv == GC.getDefineINT("ANIMAL_CIVILIZATION")
    || iCiv == GC.getDefineINT("WILDMANA_CIVILIZATION")
    || iCiv == GC.getDefineINT("PIRATES_CIVILIZATION")
	|| iCiv == GC.getDefineINT("WHITEHAND_CIVILIZATION")
    || iCiv == GC.getDefineINT("DEVIL_CIVILIZATION")
    || iCiv == GC.getDefineINT("MERCENARIES_CIVILIZATION")
    )
/*************************************************************************************************/
/**	MultiBarb								END													**/
/*************************************************************************************************/
	{
	    iCiv = NO_CIVILIZATION;
	}
//FfH: End Add

	for (iI = 0; iI < GC.getNumSpecialistInfos(); ++iI)
	{
		aeFreeSpecialists.push_back(pOldCity->getAddedFreeSpecialistCount((SpecialistTypes)iI));
	}

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		abEverOwned[iI] = pOldCity->isEverOwned((PlayerTypes)iI);
		aiCulture[iI] = pOldCity->getCultureTimes100((PlayerTypes)iI);
	}

	abEverOwned[getID()] = true;

	for (iI = 0; iI < GC.getNumReligionInfos(); iI++)
	{
		pabHasReligion[iI] = pOldCity->isHasReligion((ReligionTypes)iI);
		pabHolyCity[iI] = pOldCity->isHolyCity((ReligionTypes)iI);
	}

	for (iI = 0; iI < GC.getNumCorporationInfos(); iI++)
	{
		pabHasCorporation[iI] = pOldCity->isHasCorporation((CorporationTypes)iI);
		pabHeadquarters[iI] = pOldCity->isHeadquarters((CorporationTypes)iI);
	}

	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		paiNumRealBuilding[iI] = pOldCity->getNumRealBuilding((BuildingTypes)iI);
		paiBuildingOriginalOwner[iI] = pOldCity->getBuildingOriginalOwner((BuildingTypes)iI);
		paiBuildingOriginalTime[iI] = pOldCity->getBuildingOriginalTime((BuildingTypes)iI);
	}

	std::vector<BuildingYieldChange> aBuildingYieldChange;
	std::vector<BuildingCommerceChange> aBuildingCommerceChange;
	BuildingChangeArray aBuildingHappyChange;
	BuildingChangeArray aBuildingHealthChange;
/*************************************************************************************************/
/**	Change Building Bonuses  Snarko                                   					        **/
/*************************************************************************************************/
	std::vector<BuildingBonusChange> aBuildingBonusChange;
/*************************************************************************************************/
/**	END Change Building Bonuses	                                        						**/
/*************************************************************************************************/

	for (iI = 0; iI < GC.getNumBuildingClassInfos(); ++iI)
	{
		for (int iYield = 0; iYield < NUM_YIELD_TYPES; ++iYield)
		{
			BuildingYieldChange kChange;
			kChange.eBuildingClass = (BuildingClassTypes)iI;
			kChange.eYield = (YieldTypes)iYield;
			kChange.iChange = pOldCity->getBuildingYieldChange((BuildingClassTypes)iI, (YieldTypes)iYield);
			if (0 != kChange.iChange)
			{
				aBuildingYieldChange.push_back(kChange);
			}
		}

		for (int iCommerce = 0; iCommerce < NUM_COMMERCE_TYPES; ++iCommerce)
		{
			BuildingCommerceChange kChange;
			kChange.eBuildingClass = (BuildingClassTypes)iI;
			kChange.eCommerce = (CommerceTypes)iCommerce;
			kChange.iChange = pOldCity->getBuildingCommerceChange((BuildingClassTypes)iI, (CommerceTypes)iCommerce);
			if (0 != kChange.iChange)
			{
				aBuildingCommerceChange.push_back(kChange);
			}
		}

		int iChange = pOldCity->getBuildingHappyChange((BuildingClassTypes)iI);
		if (0 != iChange)
		{
			aBuildingHappyChange.push_back(std::make_pair((BuildingClassTypes)iI, iChange));
		}

		iChange = pOldCity->getBuildingHealthChange((BuildingClassTypes)iI);
		if (0 != iChange)
		{
			aBuildingHealthChange.push_back(std::make_pair((BuildingClassTypes)iI, iChange));
		}

/*************************************************************************************************/
/**	Change Building Bonuses  Snarko                                   					        **/
/*************************************************************************************************/
		for (int iBonus = 0; iBonus < GC.getNumBonusInfos(); ++iBonus)
		{
			BuildingBonusChange kChange;
			kChange.eBuildingClass = (BuildingClassTypes)iI;
			kChange.eBonus = (BonusTypes)iBonus;
			kChange.iChange = pOldCity->getBuildingBonusChange((BuildingClassTypes)iI, (BonusTypes)iBonus);
			if (0 != kChange.iChange)
			{
				aBuildingBonusChange.push_back(kChange);
			}
		}
/*************************************************************************************************/
/**	END Change Building Bonuses	                                        						**/
/*************************************************************************************************/

	}

	for (iI = 0; iI < NUM_CITYSPECIALIZATION_TYPES; ++iI)
	{
		paiCitySpecializationLevel[iI] = pOldCity->getCitySpecializationLevel(iI);
	}

	bRecapture = ((eHighestCulturePlayer != NO_PLAYER) ? (GET_PLAYER(eHighestCulturePlayer).getTeam() == getTeam()) : false);

/*************************************************************************************************/
/**	BUGFIX (add non barbarian Prereq) Sephi                           					        **/
/*************************************************************************************************/

    if (!GET_PLAYER(pOldCity->getOwner()).isBarbarian())
    {
//FfH: Added by Kael 09/21/2008
        if (GET_PLAYER(pOldCity->getOwner()).getNumCities() == 1)
        {
            changePlayersKilled(1);
        }
//FfH: End Add
/*************************************************************************************************/
/**	ADDON (Adventures) Sephi                                                      				**/
/*************************************************************************************************/
        if (GET_PLAYER(pOldCity->getOwner()).getNumCities() == 1)
        {
			for(int iAdvLoop=0;iAdvLoop<GC.getNumAdventureInfos();iAdvLoop++)
			{
				if(isAdventureEnabled(iAdvLoop) && !isAdventureFinished(iAdvLoop))
				{
					CvAdventureInfo &kAdv = GC.getAdventureInfo((AdventureTypes)iAdvLoop);
					for(int iJ=0;iJ<GC.getMAX_ADVENTURESTEPS();iJ++)
					{
						if(kAdv.getAdventureStep(iJ)!=NO_ADVENTURESTEP)
						{
							if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).isCivsDestroyed())
							{
								changeAdventureCounter(iAdvLoop,iJ,1);
							}

							if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).isDestroyCivilization())
							{
								if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getCivilizationType()==GET_PLAYER(eOriginalOwner).getCivilizationType())
								{
									changeAdventureCounter(iAdvLoop,iJ,1);
								}
							}
						}
					}
				}
			}
		}

		GET_TEAM(getTeam()).AI_changeWarValueAccumulated(pOldCity->getTeam(),500);
		GET_TEAM(pOldCity->getTeam()).AI_changeWarValueAccumulated(getTeam(),-500);

    }
/*************************************************************************************************/
/**	END                     	                                        						**/
/*************************************************************************************************/

	pOldCity->kill(false);

	if (bTrade)
	{
		for (iDX = -1; iDX <= 1; iDX++)
		{
			for (iDY = -1; iDY <= 1; iDY++)
			{
				pLoopPlot	= plotXY(pCityPlot->getX_INLINE(), pCityPlot->getY_INLINE(), iDX, iDY);

				if (pLoopPlot != NULL)
				{
					pLoopPlot->setCulture(eOldOwner, 0, false, false);
				}
			}
		}
	}

	pNewCity = initCity(pCityPlot->getX_INLINE(), pCityPlot->getY_INLINE(), !bConquest, false);

	FAssertMsg(pNewCity != NULL, "NewCity is not assigned a valid value");

	pNewCity->setPreviousOwner(eOldOwner);
	pNewCity->setOriginalOwner(eOriginalOwner);
	pNewCity->setGameTurnFounded(iGameTurnFounded);
	pNewCity->setPopulation((bConquest && !bRecapture) ? std::max(1, (iPopulation - 1)) : iPopulation);
	pNewCity->setHighestPopulation(iHighestPopulation);
	pNewCity->setName(szName);
	pNewCity->setNeverLost(false);
	pNewCity->changeDefenseDamage(iDamage);

//FfH: Added by Kael 07/05/2008
    if (iCiv != NO_CIVILIZATION)
    {
        pNewCity->setCivilizationType(iCiv);
    }
//FfH: End Add

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		pNewCity->setEverOwned(((PlayerTypes)iI), abEverOwned[iI]);
		pNewCity->setCultureTimes100(((PlayerTypes)iI), aiCulture[iI], false, false);
	}

	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		int iNum = 0;

		if (paiNumRealBuilding[iI] > 0)
		{
			if(!(GC.getBuildingInfo((BuildingTypes)iI).isNeverCapture() ||
				GC.getBuildingInfo((BuildingTypes)iI).isCapital())) {
				iNum = paiNumRealBuilding[iI];
				eBuilding = (BuildingTypes)iI;
				pNewCity->setNumRealBuildingTimed(eBuilding, std::min(pNewCity->getNumRealBuilding(eBuilding) + iNum, GC.getCITY_MAX_NUM_BUILDINGS()), false, ((PlayerTypes)(paiBuildingOriginalOwner[iI])), paiBuildingOriginalTime[iI]);
			}
		}
	}

	//Destroy some Buildings if City is captured
	pNewCity->removeBuildingsWhenConquered(bConquest && !bRecapture);


	for (std::vector<BuildingYieldChange>::iterator it = aBuildingYieldChange.begin(); it != aBuildingYieldChange.end(); ++it)
	{
		pNewCity->setBuildingYieldChange((*it).eBuildingClass, (*it).eYield, (*it).iChange);
	}

	for (std::vector<BuildingCommerceChange>::iterator it = aBuildingCommerceChange.begin(); it != aBuildingCommerceChange.end(); ++it)
	{
		pNewCity->setBuildingCommerceChange((*it).eBuildingClass, (*it).eCommerce, (*it).iChange);
	}

	for (BuildingChangeArray::iterator it = aBuildingHappyChange.begin(); it != aBuildingHappyChange.end(); ++it)
	{
		pNewCity->setBuildingHappyChange((*it).first, (*it).second);
	}

	for (BuildingChangeArray::iterator it = aBuildingHealthChange.begin(); it != aBuildingHealthChange.end(); ++it)
	{
		pNewCity->setBuildingHealthChange((*it).first, (*it).second);
	}

/*************************************************************************************************/
/**	Change Building Bonuses  Snarko                                   					        **/
/**																								**/
/**						                                            							**/
/*************************************************************************************************/
	for (std::vector<BuildingBonusChange>::iterator it = aBuildingBonusChange.begin(); it != aBuildingBonusChange.end(); ++it)
	{
		pNewCity->setBuildingBonusChange((*it).eBuildingClass, (*it).eBonus, (*it).iChange);
	}
/*************************************************************************************************/
/**	END Change Building Bonuses	                                        						**/
/*************************************************************************************************/

	for (iI = 0; iI < GC.getNumSpecialistInfos(); ++iI)
	{
		pNewCity->changeFreeSpecialistCount((SpecialistTypes)iI, aeFreeSpecialists[iI]);
	}

	for (iI = 0; iI < GC.getNumReligionInfos(); iI++)
	{
		if (pabHasReligion[iI])
		{
			pNewCity->setHasReligion(((ReligionTypes)iI), true, false, true);
		}

		if (pabHolyCity[iI])
		{
			GC.getGameINLINE().setHolyCity(((ReligionTypes)iI), pNewCity, false);
/*************************************************************************************************/
/**	SPEEDTWEAK (CAR MOD) merged Sephi                                         					**/
/**																								**/
/**	                                                                 							**/
/*************************************************************************************************/
			for (int iJ = 0; iJ < GC.getMAX_PLAYERS(); iJ++)
			{
				if (GET_PLAYER((PlayerTypes)iJ).isAlive() && GET_PLAYER((PlayerTypes)iI).getStateReligion() == (ReligionTypes)iI)
				{
					GET_PLAYER(getID()).AI_invalidateAttitudeCache((PlayerTypes)iJ);
					GET_PLAYER((PlayerTypes)iJ).AI_invalidateAttitudeCache(getID());
				}
			}
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
		}
	}

	for (iI = 0; iI < NUM_CITYSPECIALIZATION_TYPES; ++iI)
	{
		pNewCity->changeCitySpecializationLevel(iI,paiCitySpecializationLevel[iI]);
	}

	for (iI = 0; iI < GC.getNumCorporationInfos(); iI++)
	{
		if (pabHasCorporation[iI])
		{
			pNewCity->setHasCorporation(((CorporationTypes)iI), true, false);
		}

		if (pabHeadquarters[iI])
		{
			GC.getGameINLINE().setHeadquarters(((CorporationTypes)iI), pNewCity, false);
		}
	}

	if (bTrade)
	{
		if (isHuman() || (getTeam() == GET_PLAYER(eOldOwner).getTeam()))
		{
			pNewCity->changeHurryAngerTimer(iHurryAngerTimer);
			pNewCity->changeConscriptAngerTimer(iConscriptAngerTimer);
			pNewCity->changeDefyResolutionAngerTimer(iDefyResolutionAngerTimer);
		}

		if (!bRecapture)
		{
			pNewCity->changeOccupationTimer(iOccupationTimer);
		}
	}

	if (bConquest)
	{
		iTeamCulturePercent = pNewCity->calculateTeamCulturePercent(getTeam());

		if (iTeamCulturePercent < GC.getDefineINT("OCCUPATION_CULTURE_PERCENT_THRESHOLD"))
		{
			pNewCity->changeOccupationTimer(((GC.getDefineINT("BASE_OCCUPATION_TURNS") + ((pNewCity->getPopulation() * GC.getDefineINT("OCCUPATION_TURNS_POPULATION_PERCENT")) / 100)) * (100 - iTeamCulturePercent)) / 100);
		}

		GC.getMapINLINE().verifyUnitValidPlot();
	}

	pCityPlot->setRevealed(GET_PLAYER(eOldOwner).getTeam(), true, false, NO_TEAM, false);

	pNewCity->updateEspionageVisibility(false);

	if (bUpdatePlotGroups)
	{
		GC.getGameINLINE().updatePlotGroups();
	}

	CvEventReporter::getInstance().cityAcquired(eOldOwner, getID(), pNewCity, bConquest, bTrade);

	// default value for avoid angry Citizens	
	if(GC.getInfoTypeForString("EMPHASIZE_AVOID_ANGRY_CITIZENS")!=NO_EMPHASIZE)
		pNewCity->AI_setEmphasize((EmphasizeTypes)GC.getInfoTypeForString("EMPHASIZE_AVOID_ANGRY_CITIZENS"),true);


	SAFE_DELETE_ARRAY(pabHasReligion);
	SAFE_DELETE_ARRAY(pabHolyCity);
	SAFE_DELETE_ARRAY(pabHasCorporation);
	SAFE_DELETE_ARRAY(pabHeadquarters);
	SAFE_DELETE_ARRAY(paiNumRealBuilding);
	SAFE_DELETE_ARRAY(paiBuildingOriginalOwner);
	SAFE_DELETE_ARRAY(paiBuildingOriginalTime);
/*************************************************************************************************/
/**	ADDON (multibarb) merged Sephi		                                                        **/
/**																								**/
/**						Force New Barbarians to never control a City							**/
/*************************************************************************************************/
/**								---- Start Original Code ----									**
	if (bConquest)
/**								----  End Original Code  ----									**/
	if (
        getID() == WILDMANA_PLAYER || getID() == ANIMAL_PLAYER || getID() == PIRATES_PLAYER ||
        getID() == WHITEHAND_PLAYER || getID() == DEVIL_PLAYER
	)
	{
		pNewCity->doTask(TASK_RAZE);
	}
	else if (bConquest)
/*************************************************************************************************/
/**	Tweak									END													**/
/*************************************************************************************************/
	{
		CyCity* pyCity = new CyCity(pNewCity);
		CyArgsList argsList;
		argsList.add(getID());	// Player ID
		argsList.add(gDLL->getPythonIFace()->makePythonObject(pyCity));	// pass in city class
		long lResult=0;
		gDLL->getPythonIFace()->callFunction(PYGameModule, "canRazeCity", argsList.makeFunctionArgs(), &lResult);
		delete pyCity;	// python fxn must not hold on to this pointer

		if (lResult == 1)
		{
			//auto raze based on game rules
			if (pNewCity->isAutoRaze())
			{
				if (iCaptureGold > 0)
				{
					szBuffer = gDLL->getText("TXT_KEY_MISC_PILLAGED_CITY", iCaptureGold, pNewCity->getNameKey());
					gDLL->getInterfaceIFace()->addMessage(getID(), true, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_CITYRAZE", MESSAGE_TYPE_MAJOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("WORLDBUILDER_CITY_EDIT")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_GREEN"), pNewCity->getX_INLINE(), pNewCity->getY_INLINE(), true, true);
				}

				pNewCity->doTask(TASK_RAZE);
			}
			else if (!isHuman())
			{
				AI_conquerCity(pNewCity); // could delete the pointer...
			}
			else
			{
				//popup raze option
				eHighestCulturePlayer = pNewCity->getLiberationPlayer(true);
				bRaze = canRaze(pNewCity);
				bGift = ((eHighestCulturePlayer != NO_PLAYER)
						&& (eHighestCulturePlayer != getID())
						&& ((getTeam() == GET_PLAYER(eHighestCulturePlayer).getTeam())
							|| GET_TEAM(getTeam()).isOpenBorders(GET_PLAYER(eHighestCulturePlayer).getTeam())
							|| GET_TEAM(GET_PLAYER(eHighestCulturePlayer).getTeam()).isVassal(getTeam())));

				if (bRaze || bGift)
				{
					CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_RAZECITY);
					pInfo->setData1(pNewCity->getID());
					pInfo->setData2(eHighestCulturePlayer);
					pInfo->setData3(iCaptureGold);
					gDLL->getInterfaceIFace()->addPopup(pInfo, getID());
				}
				else
				{
					pNewCity->chooseProduction();
					CvEventReporter::getInstance().cityAcquiredAndKept(getID(), pNewCity);
				}
			}
		}
	}
	else if (!bTrade)
	{
		if (isHuman())
		{
			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_DISBANDCITY);
			pInfo->setData1(pNewCity->getID());
			gDLL->getInterfaceIFace()->addPopup(pInfo, getID());
		}
		else
		{
			CvEventReporter::getInstance().cityAcquiredAndKept(getID(), pNewCity);
		}
	}

	// Forcing events that deal with the old city not to expire just because we conquered that city
	for (CvEventMap::iterator it = m_mapEventsOccured.begin(); it != m_mapEventsOccured.end(); ++it)
	{
		EventTriggeredData &triggerData = it->second;
		if((triggerData.m_eOtherPlayer == eOldOwner) && (triggerData.m_iOtherPlayerCityId == iOldCityId))
		{
			triggerData.m_iOtherPlayerCityId = -1;
		}
	}

	pNewCity->spawnGarrisonUnits();

/*************************************************************************************************/
/**	ADDON (Mercenaries) Sephi	        								                    	**/
/*************************************************************************************************/
    if(GC.getGameINLINE().getMercenariesSupportTarget()!=NO_TEAM)
    {
        if(getID()==GC.getMERCENARIES_PLAYER() && GET_PLAYER(eOldOwner).getTeam()==GC.getGameINLINE().getMercenariesWarTarget())
        {
            for(int iI=0; iI<MAX_CIV_PLAYERS; iI++)
            {
                if(GET_PLAYER((PlayerTypes)iI).isAlive())
                {
                    if(GET_PLAYER((PlayerTypes)iI).getTeam()==GC.getGameINLINE().getMercenariesSupportTarget())
                    {
                        GET_PLAYER((PlayerTypes)iI).acquireCity(pNewCity, bConquest, bTrade, bUpdatePlotGroups);
                        break;
                    }
                }
            }
        }
    }
/*************************************************************************************************/
/**			        						END													**/
/*************************************************************************************************/
}


void CvPlayer::killCities()
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->kill(false);
	}

	GC.getGameINLINE().updatePlotGroups();
}


CvWString CvPlayer::getNewCityName() const
{
	CLLNode<CvWString>* pNode;
	CvWString szName;
	int iI;

	for (pNode = headCityNameNode(); (pNode != NULL); pNode = nextCityNameNode(pNode))
	{
		szName = gDLL->getText(pNode->m_data);
		if (isCityNameValid(szName, true))
		{
			szName = pNode->m_data;
			break;
		}
	}

	if (szName.empty())
	{
		getCivilizationCityName(szName, getCivilizationType());
	}

	if (szName.empty())
	{
		// Pick a name from another random civ
		int iRandOffset = GC.getGameINLINE().getSorenRandNum(GC.getNumCivilizationInfos(), "Place Units (Player)");
		for (iI = 0; iI < GC.getNumCivilizationInfos(); iI++)
		{
			int iLoopName = ((iI + iRandOffset) % GC.getNumCivilizationInfos());

			getCivilizationCityName(szName, ((CivilizationTypes)iLoopName));

			if (!szName.empty())
			{
				break;
			}
		}
	}

	if (szName.empty())
	{
		szName = "TXT_KEY_CITY";
	}

	return szName;
}


void CvPlayer::getCivilizationCityName(CvWString& szBuffer, CivilizationTypes eCivilization) const
{
	int iRandOffset;
	int iLoopName;
	int iI;

	if (isBarbarian() || isMinorCiv())
	{
		iRandOffset = GC.getGameINLINE().getSorenRandNum(GC.getCivilizationInfo(eCivilization).getNumCityNames(), "Place Units (Player)");
	}
	else
	{
		iRandOffset = 0;
	}

	for (iI = 0; iI < GC.getCivilizationInfo(eCivilization).getNumCityNames(); iI++)
	{
		iLoopName = ((iI + iRandOffset) % GC.getCivilizationInfo(eCivilization).getNumCityNames());

		CvWString szName = gDLL->getText(GC.getCivilizationInfo(eCivilization).getCityNames(iLoopName));

		if (isCityNameValid(szName, true))
		{
			szBuffer = GC.getCivilizationInfo(eCivilization).getCityNames(iLoopName);
			break;
		}
	}
}


bool CvPlayer::isCityNameValid(CvWString& szName, bool bTestDestroyed) const
{
	CvCity* pLoopCity;
	int iLoop;

	if (bTestDestroyed)
	{
		if (GC.getGameINLINE().isDestroyedCityName(szName))
		{
			return false;
		}

		for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);
			for (pLoopCity = kLoopPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kLoopPlayer.nextCity(&iLoop))
			{
				if (pLoopCity->getName() == szName)
				{
					return false;
				}
			}
		}
	}
	else
	{
		for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
		{
			if (pLoopCity->getName() == szName)
			{
				return false;
			}
		}
	}

	return true;
}


CvUnit* CvPlayer::initUnit(UnitTypes eUnit, int iX, int iY, UnitAITypes eUnitAI, DirectionTypes eFacingDirection)
{
	PROFILE_FUNC();

	FAssertMsg(eUnit != NO_UNIT, "Unit is not assigned a valid value");

	CvUnit* pUnit = addUnit();
	FAssertMsg(pUnit != NULL, "Unit is not assigned a valid value");
	if (NULL != pUnit)
	{
		pUnit->init(pUnit->getID(), eUnit, ((eUnitAI == NO_UNITAI) ? ((UnitAITypes)(GC.getUnitInfo(eUnit).getDefaultUnitAIType())) : eUnitAI), getID(), iX, iY, eFacingDirection);
	}

	return pUnit;
}


void CvPlayer::disbandUnit(bool bAnnounce)
{
	CvUnit* pLoopUnit;
	CvUnit* pBestUnit;
	wchar szBuffer[1024];
	int iValue;
	int iBestValue;
	int iLoop;

	iBestValue = MAX_INT;
	pBestUnit = NULL;

	for(pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{
		if (!(pLoopUnit->hasCargo())

//FfH: Added by Kael 11/24/2007
          && pLoopUnit->getUnitInfo().isMilitarySupport()
//FfH: End Add

		  )
		{
			if (!(pLoopUnit->isGoldenAge()))
			{
				if (pLoopUnit->getUnitInfo().getProductionCost() > 0)
				{
					if (!(pLoopUnit->isMilitaryHappiness()) || !(pLoopUnit->plot()->isCity()) || (pLoopUnit->plot()->plotCount(PUF_isMilitaryHappiness, -1, -1, getID()) > 1))
					{
						iValue = (10000 + GC.getGameINLINE().getSorenRandNum(1000, "Disband Unit"));

						iValue += (pLoopUnit->getUnitInfo().getProductionCost() * 5);

						iValue += (pLoopUnit->getExperience() * 20);
						iValue += (pLoopUnit->getLevel() * 100);

						if (pLoopUnit->canDefend() && pLoopUnit->plot()->isCity())
						{
							iValue *= 2;
						}

						if (pLoopUnit->plot()->getTeam() == pLoopUnit->getTeam())
						{
							iValue *= 3;
						}

						switch (pLoopUnit->AI_getUnitAIType())
						{
						case UNITAI_UNKNOWN:
						case UNITAI_ANIMAL:
							break;

						case UNITAI_SETTLE:
/*************************************************************************************************/
/**	BETTER AI (New UNITAI) Sephi                                 					            **/
/**																								**/
/**						                                            							**/
/*************************************************************************************************/
                        case UNITAI_WARWIZARD:
                        case UNITAI_TERRAFORMER:
                        case UNITAI_MANA_UPGRADE:
                        case UNITAI_MAGE:
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
							iValue *= 20;
							break;

						case UNITAI_WORKER:
							iValue *= 10;
							break;

						case UNITAI_ATTACK:
						case UNITAI_ATTACK_CITY:
						case UNITAI_COLLATERAL:
						case UNITAI_PILLAGE:
						case UNITAI_RESERVE:
						case UNITAI_COUNTER:
							iValue *= 2;
							break;

						case UNITAI_CITY_DEFENSE:
						case UNITAI_CITY_COUNTER:
						case UNITAI_CITY_SPECIAL:
						case UNITAI_PARADROP:
							iValue *= 6;
							break;

						case UNITAI_EXPLORE:
							iValue *= 15;
							break;

						case UNITAI_MISSIONARY:
							iValue *= 8;
							break;

						case UNITAI_PROPHET:
						case UNITAI_ARTIST:
						case UNITAI_SCIENTIST:
						case UNITAI_GENERAL:
						case UNITAI_MERCHANT:
						case UNITAI_ENGINEER:
							break;

						case UNITAI_SPY:
							iValue *= 12;
							break;

						case UNITAI_ICBM:
							iValue *= 4;
							break;

						case UNITAI_WORKER_SEA:
							iValue *= 18;
							break;

						case UNITAI_ATTACK_SEA:
						case UNITAI_RESERVE_SEA:
						case UNITAI_ESCORT_SEA:
							break;

						case UNITAI_EXPLORE_SEA:
							iValue *= 25;
							break;

						case UNITAI_ASSAULT_SEA:
						case UNITAI_SETTLER_SEA:
						case UNITAI_MISSIONARY_SEA:
						case UNITAI_SPY_SEA:
						case UNITAI_CARRIER_SEA:
						case UNITAI_MISSILE_CARRIER_SEA:
							iValue *= 5;
							break;

						case UNITAI_PIRATE_SEA:
						case UNITAI_ATTACK_AIR:
							break;

						case UNITAI_DEFENSE_AIR:
						case UNITAI_CARRIER_AIR:
						case UNITAI_MISSILE_AIR:
							iValue *= 3;
							break;

						default:
//							FAssert(false);  new UnitAIs not added yet modified Sephi
							break;
						}

						if (pLoopUnit->getUnitInfo().getExtraCost() > 0)
						{
							iValue /= (pLoopUnit->getUnitInfo().getExtraCost() + 1);
						}

						if(pLoopUnit->getAIGroup()!=NULL && pLoopUnit->getAIGroup()->getGroupType()==AIGROUP_CITY_DEFENSE)
						{
							iValue += 10000;
							iValue *= 100;
						}

						if (iValue < iBestValue)
						{
							iBestValue = iValue;
							pBestUnit = pLoopUnit;
						}
					}
				}
			}
		}
	}

	if (pBestUnit != NULL)
	{
		swprintf(szBuffer, gDLL->getText("TXT_KEY_MISC_UNIT_DISBANDED_NO_MONEY", pBestUnit->getNameKey()).GetCString());
		gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_UNITDISBANDED", MESSAGE_TYPE_MINOR_EVENT, pBestUnit->getButton(), (ColorTypes)GC.getInfoTypeForString("COLOR_RED"), pBestUnit->getX_INLINE(), pBestUnit->getY_INLINE(), true, true);

		FAssert(!(pBestUnit->isGoldenAge()));

		pBestUnit->kill(false);
	}
}


void CvPlayer::killUnits()
{
	CvUnit* pLoopUnit;
	int iLoop;

	for (pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{

//FfH: Modified by Kael 06/13/2008
//		pLoopUnit->kill(false);
//	}
        if (pLoopUnit->isImmortal())
        {
            pLoopUnit->changeImmortal(-1);
        }
        if (pLoopUnit->canScrap())
        {
            pLoopUnit->kill(false);
        }
        else
        {
            pLoopUnit->betray(BARBARIAN_PLAYER);
        }
	}
//FfH: End Modify

}


// XXX should pUnit be a CvSelectionGroup???
// Returns the next unit in the cycle...
CvSelectionGroup* CvPlayer::cycleSelectionGroups(CvUnit* pUnit, bool bForward, bool bWorkers, bool* pbWrap)
{
	CLLNode<int>* pSelectionGroupNode;
	CLLNode<int>* pFirstSelectionGroupNode;
	CvSelectionGroup* pLoopSelectionGroup;

	if (pbWrap != NULL)
	{
		*pbWrap = false;
	}

	pSelectionGroupNode = headGroupCycleNode();

	if (pUnit != NULL)
	{
		while (pSelectionGroupNode != NULL)
		{
			if (getSelectionGroup(pSelectionGroupNode->m_data) == pUnit->getGroup())
			{
				if (bForward)
				{
					pSelectionGroupNode = nextGroupCycleNode(pSelectionGroupNode);
				}
				else
				{
					pSelectionGroupNode = previousGroupCycleNode(pSelectionGroupNode);
				}
				break;
			}

			pSelectionGroupNode = nextGroupCycleNode(pSelectionGroupNode);
		}
	}

	if (pSelectionGroupNode == NULL)
	{
		if (bForward)
		{
			pSelectionGroupNode = headGroupCycleNode();
		}
		else
		{
			pSelectionGroupNode = tailGroupCycleNode();
		}

		if (pbWrap != NULL)
		{
			*pbWrap = true;
		}
	}

	if (pSelectionGroupNode != NULL)
	{
		pFirstSelectionGroupNode = pSelectionGroupNode;

		while (true)
		{
			pLoopSelectionGroup = getSelectionGroup(pSelectionGroupNode->m_data);
			FAssertMsg(pLoopSelectionGroup != NULL, "LoopSelectionGroup is not assigned a valid value");

			if (pLoopSelectionGroup->readyToSelect())
			{
				if (!bWorkers || pLoopSelectionGroup->hasWorker())
				{
					if (pUnit && pLoopSelectionGroup == pUnit->getGroup())
					{
						if (pbWrap != NULL)
						{
							*pbWrap = true;
						}
					}

					return pLoopSelectionGroup;
				}
			}

			if (bForward)
			{
				pSelectionGroupNode = nextGroupCycleNode(pSelectionGroupNode);

				if (pSelectionGroupNode == NULL)
				{
					pSelectionGroupNode = headGroupCycleNode();

					if (pbWrap != NULL)
					{
						*pbWrap = true;
					}
				}
			}
			else
			{
				pSelectionGroupNode = previousGroupCycleNode(pSelectionGroupNode);

				if (pSelectionGroupNode == NULL)
				{
					pSelectionGroupNode = tailGroupCycleNode();

					if (pbWrap != NULL)
					{
						*pbWrap = true;
					}
				}
			}

			if (pSelectionGroupNode == pFirstSelectionGroupNode)
			{
				break;
			}
		}
	}

	return NULL;
}


bool CvPlayer::hasTrait(TraitTypes eTrait) const
{
	FAssertMsg((getLeaderType() >= 0), "getLeaderType() is less than zero");
	FAssertMsg((eTrait >= 0), "eTrait is less than zero");

//FfH: Scriptable Leader Traits: Modified by Kael 08/08/2007
//	return GC.getLeaderHeadInfo(getLeaderType()).hasTrait(eTrait);
	return m_pbTraits[(int)eTrait];
//FfH: End Modify

}

//FfH: Scriptable Leader Traits: Added by Kael 08/08/2007
void CvPlayer::setHasTrait(TraitTypes eTrait, bool bNewValue)
{
	FAssertMsg((eTrait >= 0), "eTrait is less than zero");

	int iI, iChange;
	CvTraitInfo& kTrait = GC.getTraitInfo(eTrait);

    if (bNewValue)
    {
        iChange = 1;
    }
    else
    {
        iChange = -1;
    }

	if (m_pbTraits[(int)eTrait] == bNewValue)
	{
		return;
	}

	m_pbTraits[(int)eTrait] = bNewValue;

	changeExtraHealth(GC.getTraitInfo(eTrait).getHealth() * iChange);
	changeExtraHappiness(GC.getTraitInfo(eTrait).getHappiness() * iChange);

	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		changeExtraBuildingHappiness((BuildingTypes)iI, GC.getBuildingInfo((BuildingTypes)iI).getHappinessTraits((int)eTrait) * iChange);
	}

	changeUpkeepModifier(GC.getTraitInfo(eTrait).getUpkeepModifier() * iChange);
	changeLevelExperienceModifier(GC.getTraitInfo(eTrait).getLevelExperienceModifier() * iChange);
	changeGreatPeopleRateModifier(GC.getTraitInfo(eTrait).getGreatPeopleRateModifier() * iChange);
	changeGreatGeneralRateModifier(GC.getTraitInfo(eTrait).getGreatGeneralRateModifier() * iChange);
	changeDomesticGreatGeneralRateModifier(GC.getTraitInfo(eTrait).getDomesticGreatGeneralRateModifier() * iChange);

	changeMaxGlobalBuildingProductionModifier(GC.getTraitInfo(eTrait).getMaxGlobalBuildingProductionModifier() * iChange);
	changeMaxTeamBuildingProductionModifier(GC.getTraitInfo(eTrait).getMaxTeamBuildingProductionModifier() * iChange);
	changeMaxPlayerBuildingProductionModifier(GC.getTraitInfo(eTrait).getMaxPlayerBuildingProductionModifier() * iChange);

	for (iI = 0; iI < NUM_YIELD_TYPES; iI++)
	{
		changeTradeYieldModifier(((YieldTypes)iI), GC.getTraitInfo(eTrait).getTradeYieldModifier(iI) * iChange);
	}

	for (iI = 0; iI < NUM_COMMERCE_TYPES; iI++)
    {
		changeFreeCityCommerce(((CommerceTypes)iI), GC.getTraitInfo(eTrait).getCommerceChange(iI) * iChange);
		changeCommerceRateModifier(((CommerceTypes)iI), GC.getTraitInfo(eTrait).getCommerceModifier(iI) * iChange);
	}

	for (iI = 0; iI < GC.getNumCivicOptionInfos(); iI++)
	{
		if (GC.getCivicOptionInfo((CivicOptionTypes) iI).getTraitNoUpkeep(int(eTrait)))
		{
			changeNoCivicUpkeepCount(((CivicOptionTypes)iI), iChange);
		}
	}

    if (kTrait.isAdaptive())
    {
        setAdaptive(bNewValue);
    }
	if (kTrait.isIgnoreUnHealthFromImprovements())
		changeIgnoreUnHealthFromImprovements(iChange);
 
	changeImprovementUpgradeRateModifier(kTrait.getImprovementUpgradeRateModifier() * iChange);

	changeExtraImprovementPerCity(kTrait.getExtraImprovementPerCity() * iChange);
	changeExtraManaNodesPercent(kTrait.getExtraManaNodesPercent() * iChange);
	changeReducedHurryCostBuilding(kTrait.getReducedHurryCostBuilding() * iChange);
	changeExtraSupportUnitLimit(kTrait.getExtraSupportUnitLimit() * iChange);
	changeExtraBuildingCapturedChance(kTrait.getExtraBuildingCapturedChance() * iChange);

    if (GC.getTraitInfo(eTrait).isAgnostic())
    {
        setAgnostic(bNewValue);
    }
    if (GC.getTraitInfo(eTrait).isAssimilation())
    {
        setAssimilation(bNewValue);
    }
    if (GC.getTraitInfo(eTrait).isBarbarianAlly())
    {
        GET_TEAM(getTeam()).setBarbarianAlly(bNewValue);
    }
    if (GC.getTraitInfo(eTrait).isIgnoreFood())
    {
        setIgnoreFood(bNewValue);
    }
    if (GC.getTraitInfo(eTrait).isInsane())
    {
        setInsane(bNewValue);
    }
    if (GC.getTraitInfo(eTrait).isSprawling())
    {
        setSprawling(bNewValue);
    }
	changeFreeXPFromCombat(GC.getTraitInfo(eTrait).getFreeXPFromCombat() * iChange);
    if (GC.getTraitInfo(eTrait).getMaxCities() != -1)
    {
        if (iChange == 1)
        {
            setMaxCities(GC.getTraitInfo(eTrait).getMaxCities() + GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getMaxCitiesMod());
        }
        else
        {
            setMaxCities(-1);
        }
    }
	changePillagingGold(GC.getTraitInfo(eTrait).getPillagingGold() * iChange);
	changeStartingGold(GC.getTraitInfo(eTrait).getStartingGold() * iChange);
	changeSummonCostReduction(GC.getTraitInfo(eTrait).getSummonCostReduction() * iChange);
	changeTaxesModifier(GC.getTraitInfo(eTrait).getTaxesModifier() * iChange);
	changeFoodPerPopulationModifier(GC.getTraitInfo(eTrait).getFoodPerPopulationModifier() * iChange);
	changeUpgradeCostModifier(GC.getTraitInfo(eTrait).getUpgradeCostModifier() * iChange);
	changeProductionCostReductionCapitalBuilding(GC.getTraitInfo(eTrait).getProductionCostReductionCapitalBuilding() * iChange);
	changeUnitSupportModifier(GC.getTraitInfo(eTrait).getUnitSupportModifier() * iChange);

	for (iI = 0; iI < NUM_YIELD_TYPES; iI++)
	{
		updateExtraYieldThreshold((YieldTypes)iI);
	}

	updateMaxAnarchyTurns();
/*************************************************************************************************/
/**	ADDON (Houses of Erebus) Sephi			                                 					**/
/*************************************************************************************************/
	for (int i=0;i<GC.getNumCorporationInfos();i++)
	{
		if (GC.getTraitInfo(eTrait).getCorporationSupport((CorporationTypes)i)!=0)
		{
			changeCorporationSupport((CorporationTypes)i, GC.getTraitInfo(eTrait).getCorporationSupport((CorporationTypes)i)  * iChange);
		}
	}


	for (int i=0;i<GC.getNumCorporationInfos();i++)
	{
		if (GC.getTraitInfo(eTrait).getCorporationSupportMultiplier((CorporationTypes)i)!=0)
		{
			changeCorporationSupportMultiplier((CorporationTypes)i, GC.getTraitInfo(eTrait).getCorporationSupportMultiplier((CorporationTypes)i)  * iChange);
		}
	}

	if(GC.getTraitInfo(eTrait).getManaFromImprovementType() != NO_IMPROVEMENT) {
		changeManaFromImprovement(GC.getTraitInfo(eTrait).getManaFromImprovementType(), 
							GC.getTraitInfo(eTrait).getManaFromImprovement() * iChange);
	}

	for(int i = 0; i < GC.getNumBuildingClassInfos(); ++i) {
		if(GC.getTraitInfo(eTrait).isIgnoreBuildingGYCost(i)) {
			changeIgnoreBuildingGYCostCount(i, iChange);
		}
	}
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

}

void CvPlayer::setDisableHuman(bool bNewValue)
{
	m_bDisableHuman = bNewValue;
	updateHuman();
}

bool CvPlayer::getDisableHuman() const
{
	return m_bDisableHuman;
}
//FfH: End Add

bool CvPlayer::isHuman() const
{
	return m_bHuman;
}

void CvPlayer::updateHuman()
{
	if (getID() == NO_PLAYER)
	{
		m_bHuman = false;
	}
	else
	{

//FfH: Modified by Kael for jdog5000's AIAutoPlay 05/29/2008
//		m_bHuman = GC.getInitCore().getHuman(getID());
		if( m_bDisableHuman )
		{
			m_bHuman = false;
		}
		else
		{
            m_bHuman = GC.getInitCore().getHuman(getID());
        }
//FfH: End Modify

	}
}

bool CvPlayer::isBarbarian() const
{
/*************************************************************************************************/
/**	ADDON (MultiBarb) merged Sephi		12/23/08									Xienwolf	**/
/**	count other Barbarian Teams also as barbarian                   							**/
/**							Adds extra Barbarian Civilizations									**/
/*************************************************************************************************/
/**								---- Start Original Code ----	                                **
	return (getID() == BARBARIAN_PLAYER);
/**								----  End Original Code  ----									**/
	return (getID() >= BARBARIAN_PLAYER);
/*************************************************************************************************/
/**	MultiBarb								END													**/
/*************************************************************************************************/
}


const wchar* CvPlayer::getName(uint uiForm) const
{

	if (GC.getInitCore().getLeaderName(getID(), uiForm).empty() || (GC.getGameINLINE().isMPOption(MPOPTION_ANONYMOUS) && isAlive() && GC.getGameINLINE().getGameState() == GAMESTATE_ON))
	{
		return GC.getLeaderHeadInfo(getLeaderType()).getDescription(uiForm);
	}
	else
	{
		return GC.getInitCore().getLeaderName(getID(), uiForm);
	}
}


const wchar* CvPlayer::getNameKey() const
{
	if (GC.getInitCore().getLeaderNameKey(getID()).empty() || (GC.getGameINLINE().isMPOption(MPOPTION_ANONYMOUS) && isAlive()))
	{
		return GC.getLeaderHeadInfo(getLeaderType()).getTextKeyWide();
	}
	else
	{
		return GC.getInitCore().getLeaderNameKey(getID());
	}
}


const wchar* CvPlayer::getCivilizationDescription(uint uiForm) const
{
	if (GC.getInitCore().getCivDescription(getID(), uiForm).empty())
	{
		return GC.getCivilizationInfo(getCivilizationType()).getDescription(uiForm);
	}
	else
	{
		return GC.getInitCore().getCivDescription(getID(), uiForm);
	}
}


const wchar* CvPlayer::getCivilizationDescriptionKey() const
{
	if (GC.getInitCore().getCivDescriptionKey(getID()).empty())
	{
		return GC.getCivilizationInfo(getCivilizationType()).getTextKeyWide();
	}
	else
	{
		return GC.getInitCore().getCivDescriptionKey(getID());
	}
}


const wchar* CvPlayer::getCivilizationShortDescription(uint uiForm) const
{
	if (GC.getInitCore().getCivShortDesc(getID(), uiForm).empty())
	{
		return GC.getCivilizationInfo(getCivilizationType()).getShortDescription(uiForm);
	}
	else
	{
		return GC.getInitCore().getCivShortDesc(getID(), uiForm);
	}
}


const wchar* CvPlayer::getCivilizationShortDescriptionKey() const
{
	if (GC.getInitCore().getCivShortDescKey(getID()).empty())
	{
		return GC.getCivilizationInfo(getCivilizationType()).getShortDescriptionKey();
	}
	else
	{
		return GC.getInitCore().getCivShortDescKey(getID());
	}
}


const wchar* CvPlayer::getCivilizationAdjective(uint uiForm) const
{
	if (GC.getInitCore().getCivAdjective(getID(), uiForm).empty())
	{
		return GC.getCivilizationInfo(getCivilizationType()).getAdjective(uiForm);
	}
	else
	{
		return GC.getInitCore().getCivAdjective(getID(), uiForm);
	}
}

const wchar* CvPlayer::getCivilizationAdjectiveKey() const
{
	if (GC.getInitCore().getCivAdjectiveKey(getID()).empty())
	{
		return GC.getCivilizationInfo(getCivilizationType()).getAdjectiveKey();
	}
	else
	{
		return GC.getInitCore().getCivAdjectiveKey(getID());
	}
}


CvWString CvPlayer::getFlagDecal() const
{
	if (GC.getInitCore().getFlagDecal(getID()).empty())
	{
		return GC.getCivilizationInfo(getCivilizationType()).getFlagTexture();
	}
	else
	{
		return GC.getInitCore().getFlagDecal(getID());
	}
}

bool CvPlayer::isWhiteFlag() const
{
	if (GC.getInitCore().getFlagDecal(getID()).empty())
	{
		return GC.getCivilizationInfo(getCivilizationType()).getArtInfo()->isWhiteFlag();
	}
	else
	{
		return GC.getInitCore().getWhiteFlag(getID());
	}
}


const wchar* CvPlayer::getStateReligionName(uint uiForm) const
{
	return GC.getReligionInfo(getStateReligion()).getDescription(uiForm);
}

const wchar* CvPlayer::getStateReligionKey() const
{
	if (getStateReligion() != NO_RELIGION)
	{
		return GC.getReligionInfo(getStateReligion()).getTextKeyWide();
	}

	return L"TXT_KEY_MISC_NO_STATE_RELIGION";
}


const CvWString CvPlayer::getBestAttackUnitName(uint uiForm) const
{
	return gDLL->getObjectText((CvString)getBestAttackUnitKey(), uiForm, true);
}


const CvWString CvPlayer::getWorstEnemyName() const
{
	TeamTypes eWorstEnemy;

	eWorstEnemy = GET_TEAM(getTeam()).AI_getWorstEnemy();

	if (eWorstEnemy != NO_TEAM)
	{
		return GET_TEAM(eWorstEnemy).getName();
	}

	return "";
}

const wchar* CvPlayer::getBestAttackUnitKey() const
{
	CvCity* pCapitalCity;
	CvCity* pLoopCity;
	UnitTypes eBestUnit;
	int iLoop;

	eBestUnit = NO_UNIT;

	pCapitalCity = getCapitalCity();

	if (pCapitalCity != NULL)
	{
		eBestUnit = pCapitalCity->AI_bestUnitAI(UNITAI_ATTACK, true);
	}

	if (eBestUnit == NO_UNIT)
	{
		for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
		{
			eBestUnit = pLoopCity->AI_bestUnitAI(UNITAI_ATTACK, true);

			if (eBestUnit != NO_UNIT)
			{
				break;
			}
		}
	}

	if (eBestUnit != NO_UNIT)
	{
		return GC.getUnitInfo(eBestUnit).getTextKeyWide();
	}

	return L"TXT_KEY_MISC_NO_UNIT";
}


ArtStyleTypes CvPlayer::getArtStyleType() const
{
	if (GC.getInitCore().getArtStyle(getID()) == NO_ARTSTYLE)
	{
		return ((ArtStyleTypes)(GC.getCivilizationInfo(getCivilizationType()).getArtStyleType()));
	}
	else
	{
		return GC.getInitCore().getArtStyle(getID());
	}
}

const TCHAR* CvPlayer::getUnitButton(UnitTypes eUnit) const
{
//>>>>BUGFfH: Modified by Denev 2009/09/28
//	return GC.getUnitInfo(eUnit).getArtInfo(0, getCurrentEra(), (UnitArtStyleTypes) GC.getCivilizationInfo(getCivilizationType()).getUnitArtStyleType())->getButton();
	return GC.getUnitInfo(eUnit).getArtInfo(0, getCurrentEra(), getUnitArtStyleType())->getButton();
//<<<<BUGFfH: End Modify

}

void CvPlayer::doTurn()
{
	PROFILE_FUNC();

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

/*********************************************************************/
/** DEBUG **/
    if (GC.getDefineINT("USE_PERFORMANCE_LOG_LEVEL1")==1)
    {
        CvString szError="CvPlayer doTurn start";
		gDLL->logMsg("pdebug.log", szError);
		gDLL->logMsg("debug.log", szError);

        TCHAR szOut[1024];
		int CombatType=this->getNumUnits();

        sprintf(szOut, "NumUnits -- %d\n",CombatType);
        gDLL->logMsg("debug.log",szOut);
		CombatType=this->getNumCities();
        sprintf(szOut, "NumCities -- %d\n",CombatType);
        gDLL->logMsg("debug.log",szOut);
		CombatType=this->getNumSelectionGroups();
        sprintf(szOut, "NumSelectionGroups -- %d\n",CombatType);
        gDLL->logMsg("debug.log",szOut);

    }
/** PDEBUG End **/
/*********************************************************************/

	CvCity* pLoopCity;
	int iLoop;

	FAssertMsg(isAlive(), "isAlive is expected to be true");
	FAssertMsg(!hasBusyUnit() || GC.getGameINLINE().isMPOption(MPOPTION_SIMULTANEOUS_TURNS)  || GC.getGameINLINE().isSimultaneousTeamTurns(), "End of turn with busy units in a sequential-turn game");

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn::Python");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
	CvEventReporter::getInstance().beginPlayerTurn( GC.getGameINLINE().getGameTurn(),  getID());
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn::Python");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	doUpdateCacheOnTurn();


	if (isBarbarian()) {
		doTurn_BarbPlayer();
	}
	else {
		updateCultivationRate();
	}


    if (!isHuman())
    {
        AI_doFavoriteReligion();

    }

	for (int iI = 0; iI < GC.getNumTraitInfos(); iI++)
	{
		if (hasTrait((TraitTypes)iI))
		{
            if (!CvString(GC.getTraitInfo((TraitTypes)iI).getPyPerTurn()).empty())
            {
                CyArgsList argsList;
                argsList.add(getID());	// Player ID
                argsList.add(iI);//the trait #
                gDLL->getPythonIFace()->callFunction(PYSpellModule, "trait", argsList.makeFunctionArgs()); //, &lResult
            }
		}
	}

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn::Civic");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	for(int iI = 0; iI < GC.getNumCivicInfos(); iI++)
	{
	    if (!CvString(GC.getCivicInfo((CivicTypes)iI).getPyPerTurn()).empty())
        {
			if(getCivics((CivicOptionTypes)GC.getCivicInfo((CivicTypes)iI).getCivicOptionType())==iI)
			{
                CyArgsList argsList;
                argsList.add(getID());	// Player ID
                argsList.add(iI);//the civic #
                gDLL->getPythonIFace()->callFunction(PYSpellModule, "civic", argsList.makeFunctionArgs()); //, &lResult
            }
		}
		if (GC.getCivicInfo((CivicTypes)iI).isPreservation())
        {
			if(getCivics((CivicOptionTypes)GC.getCivicInfo((CivicTypes)iI).getCivicOptionType())==iI)
			{
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
				GC.getTimeMeasure().Start("Player::doTurn::Civic::Preservation");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
				doCivicPreservation(false);
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
				GC.getTimeMeasure().Stop("Player::doTurn::Civic::Preservation");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

			}
		}
	}

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn::Civic");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

    for(int iAdvLoop=0;iAdvLoop<GC.getNumAdventureInfos();iAdvLoop++)
    {
        if(isAdventureEnabled(iAdvLoop) && !isAdventureFinished(iAdvLoop))
        {
            CvAdventureInfo &kAdv = GC.getAdventureInfo((AdventureTypes)iAdvLoop);
            for(int iJ=0;iJ<GC.getMAX_ADVENTURESTEPS();iJ++)
            {
                if(kAdv.getAdventureStep(iJ)!=NO_ADVENTURESTEP)
                {
                    //CivicDuration
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).isCivicDuration())
                    {
						for(int iI=0;iI < GC.getNumCivicOptionInfos();iI++)
                        {
                            if(getCivics((CivicOptionTypes)iI)==GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getCivicType())
                            {
                                changeAdventureCounter(iAdvLoop,iJ,1);
                                break;
                            }
                        }
                    }

                    //NeverAdoptCivic
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).isNeverAdoptCivic())
                    {
						for(int iI=0;iI < GC.getNumCivicOptionInfos();iI++)
                        {
                            if(getCivics((CivicOptionTypes)iI)==GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getCivicType())
                            {
                                if(getAdventureStatus(iAdvLoop,iJ)>0)
                                {
                                    changeAdventureCounter(iAdvLoop,iJ,-20000);
                                }
                                break;
                            }
                        }
                    }

                    //NumBuildingOwned
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getNumBuildingOwned()>0)
                    {
                        if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getBuildingType()!=NO_BUILDING)
                        {
                            BuildingClassTypes eBuildClass = (BuildingClassTypes)GC.getBuildingInfo((BuildingTypes)GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getBuildingType()).getBuildingClassType();
                            if(eBuildClass != NO_BUILDINGCLASS && GET_TEAM(getTeam()).getBuildingClassCount(eBuildClass)>=GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getNumBuildingOwned())
                            {
                                changeAdventureCounter(iAdvLoop,iJ,1);
                            }
                        }
                    }

                    //NumUnitOwned
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getNumUnitsOwned()>0)
                    {
                        if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getUnitType()!=NO_UNIT)
                        {
                            UnitClassTypes eUnitClass = (UnitClassTypes)GC.getUnitInfo((UnitTypes)GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getUnitType()).getUnitClassType();
                            if(eUnitClass != NO_UNITCLASS && GET_TEAM(getTeam()).getUnitClassCountPlusMaking(eUnitClass)>=GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getNumUnitsOwned())
                            {
                                changeAdventureCounter(iAdvLoop,iJ,1);
                            }
                        }
                    }

                    //NumBonusOwned
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getNumBonusOwned()>0)
                    {
                        if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getBonusType()!=NO_BONUS)
                        {
                            if(getNumAvailableBonuses((BonusTypes)GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getBonusType())>=GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getNumBonusOwned())
                            {
                                changeAdventureCounter(iAdvLoop,iJ,1);
                            }
                        }
                    }

                    //ACabove
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getACabove()>-1)
                    {
                        if(GC.getGameINLINE().getGlobalCounter()>=GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getACabove())
                        {
                            changeAdventureCounter(iAdvLoop,iJ,1);
                        }
                    }

                    //ACbelow
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getACbelow()>-1)
                    {
                        if(GC.getGameINLINE().getGlobalCounter()<=GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getACbelow())
                        {
                            changeAdventureCounter(iAdvLoop,iJ,1);
                        }
                    }

                    //PurityCounter
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getPurityCounter()>0)
                    {
                        if(getPurityCounter()>=GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getPurityCounter())
                        {
                            changeAdventureCounter(iAdvLoop,iJ,1);
                        }
                    }

                    //PaxPatria
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).isPaxPatria())
                    {
                        for(iI = 0; iI < GC.getMAX_CIV_PLAYERS(); iI++)
                        {
                            if(GET_PLAYER((PlayerTypes)iI).isAlive() && GET_PLAYER((PlayerTypes)iI).getTeam()!=getTeam())
                            {
                                if(GET_PLAYER((PlayerTypes)iI).AI_getAttitude(getID())==ATTITUDE_FRIENDLY)
                                {
                                    if(GET_TEAM(getTeam()).isOpenBorders(GET_PLAYER((PlayerTypes)iI).getTeam()))
                                    {
                                        if(GET_TEAM(getTeam()).isHasMet(GET_PLAYER((PlayerTypes)iI).getTeam()))
                                        {
                                            changeAdventureCounter(iAdvLoop,iJ,1);
                                        }
                                    }
                                }
                            }
                        }
                    }

                    //OverCouncilLead
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).isLeadOvercouncil())
                    {
                        VoteSourceTypes eOverCouncil=(VoteSourceTypes)GC.getInfoTypeForString("DIPLOVOTE_OVERCOUNCIL");
                        if (eOverCouncil!=NO_VOTESOURCE && GC.getGameINLINE().getSecretaryGeneral(eOverCouncil)==getTeam())
                        {
                            changeAdventureCounter(iAdvLoop,iJ,1);
                        }
                    }

                    //UnderCouncilLead
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).isLeadUndercouncil())
                    {
                        VoteSourceTypes eUnderCouncil=(VoteSourceTypes)GC.getInfoTypeForString("DIPLOVOTE_UNDERCOUNCIL");
                        if (eUnderCouncil!=NO_VOTESOURCE && GC.getGameINLINE().getSecretaryGeneral(eUnderCouncil)==getTeam())
                        {
                            changeAdventureCounter(iAdvLoop,iJ,1);
                        }
                    }

                    //CapitalPopulation
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getCapitalPopulation()>0)
                    {
						if(getCapitalCity()!=NULL)
						{
							if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getCapitalPopulation()<=getCapitalCity()->getPopulation())
							{
								changeAdventureCounter(iAdvLoop,iJ,1);
							}
						}
                    }

                    //GoldTreasury
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getGoldTreasury()>0)
                    {
                        if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getGoldTreasury()<=getGold())
                        {
                            changeAdventureCounter(iAdvLoop,iJ,1);
                        }
                    }

                    //GoldTreasury
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getUnitCount()>0)
                    {
                        if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getUnitCount()<=getNumUnits())
                        {
                            changeAdventureCounter(iAdvLoop,iJ,1);
                        }
                    }

                    //ConvertAllCivs
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).isConvertAllCivs())
                    {
                        bool bValid=true;
                        for(iI = 0; iI < GC.getMAX_CIV_PLAYERS(); iI++)
                        {
                            if(GET_PLAYER((PlayerTypes)iI).isAlive() && GET_PLAYER((PlayerTypes)iI).getTeam()!=getTeam())
                            {
                                if(GET_PLAYER((PlayerTypes)iI).getStateReligion()!=GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getReligionType())
                                {
                                    bValid=false;
                                    break;
                                }
                            }
                        }

                        if(bValid)
                        {
                            changeAdventureCounter(iAdvLoop,iJ,1);
                        }
                    }

                    //RemoveReligionFromAllCities
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).isRemoveReligionFromAllCities())
                    {
                        int iReligion=GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getReligionType();
                        if (iReligion!=NO_RELIGION)
                        {
                            bool bValid=true;
                            for(iI = 0; iI < GC.getMAX_PLAYERS(); iI++)
                            {
                                CvPlayerAI& kLoopPlayer = GET_PLAYER((PlayerTypes)iI);
                                if (kLoopPlayer.isAlive())
                                {
                                    int iLoop;
                                    for (CvCity* pLoopCity = kLoopPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kLoopPlayer.nextCity(&iLoop))
                                    {
                                        if(pLoopCity->isHasReligion((ReligionTypes)iReligion))
                                        {
                                            bValid=false;
                                            break;
                                        }
                                    }
                                }
                            }

                            if(bValid)
                            {
                                changeAdventureCounter(iAdvLoop,iJ,1);
                            }
                        }
                    }


                    //DiscoverWholeMap
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).isDiscoverWholeMap())
                    {
                        bool bValid=true;
                        for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
                        {
                            CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);
							if(!pLoopPlot->isRevealed(getTeam(),false))
							{
								bValid=false;
								break;
							}
						}

						if (bValid)
						{
							changeAdventureCounter(iAdvLoop,iJ,1);
                        }
                    }
                }
            }
        }
    }

	//automatically cancel global enchantments whose requirements we do not meet
	VerifyGlobalEnchantments();

	GC.getGameINLINE().verifyDeals();
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn::AI_doTurnPre");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	AI_doTurnPre();
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn::AI_doTurnPre");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	if (getRevolutionTimer() > 0)
	{
		changeRevolutionTimer(-1);
	}

	if (getConversionTimer() > 0)
	{
		changeConversionTimer(-1);
	}

	setConscriptCount(0);
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn::AI_assignWorkingPlots");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	AI_assignWorkingPlots();
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn::AI_assignWorkingPlots");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	if (0 == GET_TEAM(getTeam()).getHasMetCivCount(true) || GC.getGameINLINE().isOption(GAMEOPTION_NO_ESPIONAGE))
	{
		setCommercePercent(COMMERCE_ESPIONAGE, 0);
	}

	verifyGoldCommercePercent();

	if(!isBarbarian())
	{
		doGold();
		doMana();
		doFaith();
		doArcane();
		allowedGuild();
		allowedReligion();
		doGlobalCulture();
		doUnitSupportLimit();
	}

	doGlobalYield();
	doResearch();
	doEspionagePoints();
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn::City::doTurn");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->doTurn();
	}
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn::City::doTurn");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	if (getGoldenAgeTurns() > 0 && !(getDisableProduction() > 0 && getDisableResearch() > 0))
	{
		changeGoldenAgeTurns(-1);
	}

	if (getAnarchyTurns() > 0)
	{
		changeAnarchyTurns(-1);
	}
	verifyCivics();

	updateTradeRoutes();
	updateWarWearinessPercentAnger();
//FfH: Added by Kael 11/02/2007
    if (getTempPlayerTimer() > 0)
    {
        changeTempPlayerTimer(-1);
        if (getTempPlayerTimer() == 0)
        {
            GC.getInitCore().reassignPlayerAdvanced(getID(), (PlayerTypes)getRealPlayer(), -1);
        }
    }
    if (getSanctuaryTimer() > 0)
    {
        changeSanctuaryTimer(-1);
    }
    if (getDisableProduction() > 0)
    {
        changeDisableProduction(-1);
        updateMaintenance();
    }
    if (getDisableResearch() > 0)
    {
        changeDisableResearch(-1);
    }
    if (getDisableSpellcasting() > 0)
    {
        changeDisableSpellcasting(-1);
    }
//FfH: End Add
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn::Events");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
	doEvents();
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn::Events");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	updateEconomyHistory(GC.getGameINLINE().getGameTurn(), calculateTotalCommerce());
	updateIndustryHistory(GC.getGameINLINE().getGameTurn(), calculateTotalYield(YIELD_PRODUCTION));
	updateAgricultureHistory(GC.getGameINLINE().getGameTurn(), calculateTotalYield(YIELD_FOOD));
	updatePowerHistory(GC.getGameINLINE().getGameTurn(), getPower());
	updateCultureHistory(GC.getGameINLINE().getGameTurn(), countTotalCulture());

    if (!GC.getGameINLINE().isOption(GAMEOPTION_NO_ESPIONAGE))
    {
        updateEspionageHistory(GC.getGameINLINE().getGameTurn(), GET_TEAM(getTeam()).getEspionagePointsEver());
    }


	expireMessages();  // turn log

	gDLL->getInterfaceIFace()->setDirty(CityInfo_DIRTY_BIT, true);
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("Player::doTurn::AI_doTurnPost");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
	AI_doTurnPost();
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn::AI_doTurnPost");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	/** disabled for Performance
	CvEventReporter::getInstance().endPlayerTurn( GC.getGameINLINE().getGameTurn(),  getID());
	**/

/*********************************************************************/
/** DEBUG **/
    if (GC.getDefineINT("USE_DEBUG_LOG")==1)
    {
        CvString szError="CvPlayer doTurn end";
		gDLL->logMsg("pdebug.log", szError);
    }
/** DEBUG **/
/*********************************************************************/

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("Player::doTurn");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
}


void CvPlayer::doTurnUnits()
{
	PROFILE_FUNC();

	CvSelectionGroup* pLoopSelectionGroup;
	int iLoop;
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("CvPlayer::doTurnUnits::AI_doTurnUnitsPre");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
	AI_doTurnUnitsPre();
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("CvPlayer::doTurnUnits::AI_doTurnUnitsPre");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

/*************************************************************************************************/
/**	ADDON (Combatauras) Sephi																	**/
/*************************************************************************************************/
	doTurnRemoveCombatAuras();
	doTurnApplyCombatAuras();
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("CvPlayer::doTurnUnits::doTurn");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	for(pLoopSelectionGroup = firstSelectionGroup(&iLoop); pLoopSelectionGroup != NULL; pLoopSelectionGroup = nextSelectionGroup(&iLoop))
	{
		pLoopSelectionGroup->doDelayedDeath();
	}

	for (int iPass = 0; iPass < 4; iPass++)
	{
		for(pLoopSelectionGroup = firstSelectionGroup(&iLoop); pLoopSelectionGroup != NULL; pLoopSelectionGroup = nextSelectionGroup(&iLoop))
		{
			switch (pLoopSelectionGroup->getDomainType())
			{
			case DOMAIN_AIR:
				if (iPass == 1)
				{
					pLoopSelectionGroup->doTurn();
				}
				break;
			case DOMAIN_SEA:
				if (iPass == 2)
				{
					pLoopSelectionGroup->doTurn();
				}
				break;
			case DOMAIN_LAND:
				if (iPass == 3)
				{
					pLoopSelectionGroup->doTurn();
				}
				break;
			case DOMAIN_IMMOBILE:
				if (iPass == 0)
				{
					pLoopSelectionGroup->doTurn();
				}
				break;
			case NO_DOMAIN:
				FAssertMsg(NULL == pLoopSelectionGroup->getHeadUnit(), "Unit with no Domain");
			default:
				if (iPass == 3)
				{
					pLoopSelectionGroup->doTurn();
				}
				break;
			}
		}
	}

	if (getID() == GC.getGameINLINE().getActivePlayer())
	{
		gDLL->getFAStarIFace()->ForceReset(&GC.getInterfacePathFinder());

		gDLL->getInterfaceIFace()->setDirty(Waypoints_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(SelectionButtons_DIRTY_BIT, true);
	}

	gDLL->getInterfaceIFace()->setDirty(UnitInfo_DIRTY_BIT, true);

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("CvPlayer::doTurnUnits::doTurn");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("CvPlayer::doTurnUnits::AI_doTurnUnitsPost");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	AI_doTurnUnitsPost();

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("CvPlayer::doTurnUnits::AI_doTurnUnitsPost");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
}


void CvPlayer::verifyCivics()
{
	int iI, iJ;

	if (!isAnarchy())
	{
		for (iI = 0; iI < GC.getNumCivicOptionInfos(); iI++)
		{
			if (!canDoCivics(getCivics((CivicOptionTypes)iI)))
			{
				for (iJ = 0; iJ < GC.getNumCivicInfos(); iJ++)
				{
					if (GC.getCivicInfo((CivicTypes)iJ).getCivicOptionType() == iI)
					{
						if (canDoCivics((CivicTypes)iJ))
						{
							setCivics(((CivicOptionTypes)iI), ((CivicTypes)iJ));
							break;
						}
					}
				}
			}
		}
	}
}


void CvPlayer::updatePlotGroups()
{
	PROFILE_FUNC();

	CvPlotGroup* pLoopPlotGroup;
	int iLoop;
	int iI;

	if (!(GC.getGameINLINE().isFinalInitialized()))
	{
		return;
	}

	for(pLoopPlotGroup = firstPlotGroup(&iLoop); pLoopPlotGroup != NULL; pLoopPlotGroup = nextPlotGroup(&iLoop))
	{
		pLoopPlotGroup->recalculatePlots();
	}

	for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		GC.getMapINLINE().plotByIndexINLINE(iI)->updatePlotGroup(getID(), false);
	}

	updateTradeRoutes();
}


void CvPlayer::updateYield()
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->updateYield();
	}
}


void CvPlayer::updateMaintenance()
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->updateMaintenance();
	}
}


void CvPlayer::updatePowerHealth()
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->updatePowerHealth();
	}
}


void CvPlayer::updateExtraBuildingHappiness()
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->updateExtraBuildingHappiness();
	}
}


void CvPlayer::updateExtraBuildingHealth()
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->updateExtraBuildingHealth();
	}
}


void CvPlayer::updateFeatureHappiness()
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->updateFeatureHappiness();
	}
}


void CvPlayer::updateReligionHappiness()
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->updateReligionHappiness();
	}
}


void CvPlayer::updateExtraSpecialistYield()
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->updateExtraSpecialistYield();
	}
}


void CvPlayer::updateCommerce(CommerceTypes eCommerce)
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->updateCommerce(eCommerce);
	}
}


void CvPlayer::updateCommerce()
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->updateCommerce();
	}
}


void CvPlayer::updateBuildingCommerce()
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->updateBuildingCommerce();
	}
}


void CvPlayer::updateReligionCommerce()
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->updateReligionCommerce();
	}
}


void CvPlayer::updateCorporation()
{
	int iLoop;
	for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->updateCorporation();
	}
}


void CvPlayer::updateCityPlotYield()
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->plot()->updateYield();
	}
}


void CvPlayer::updateCitySight(bool bIncrement, bool bUpdatePlotGroups)
{
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->plot()->updateSight(bIncrement, bUpdatePlotGroups);
	}
}


void CvPlayer::updateTradeRoutes()
{
	CLLNode<int>* pCityNode;
	CvCity* pLoopCity;
	CvCity* pListCity;
	CLinkList<int> cityList;
	int iTotalTradeModifier;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->clearTradeRoutes();
	}

	cityList.clear();

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iTotalTradeModifier = pLoopCity->totalTradeModifier();

		pCityNode = cityList.head();

		while (pCityNode != NULL)
		{
			pListCity = getCity(pCityNode->m_data);

			if (iTotalTradeModifier > pListCity->totalTradeModifier())
			{
				cityList.insertBefore(pLoopCity->getID(), pCityNode);
				break;
			}
			else
			{
				pCityNode = cityList.next(pCityNode);
			}
		}

		if (pCityNode == NULL)
		{
			cityList.insertAtEnd(pLoopCity->getID());
		}
	}

	pCityNode = cityList.head();

	while (pCityNode != NULL)
	{
		getCity(pCityNode->m_data)->updateTradeRoutes();
		pCityNode = cityList.next(pCityNode);
	}
}

void CvPlayer::updatePlunder(int iChange, bool bUpdatePlotGroups)
{
	int iLoop;
	for (CvUnit* pLoopUnit = firstUnit(&iLoop); NULL != pLoopUnit; pLoopUnit = nextUnit(&iLoop))
	{
		if (pLoopUnit->isBlockading())
		{
			pLoopUnit->updatePlunder(iChange, bUpdatePlotGroups);
		}
	}
}

void CvPlayer::updateTimers()
{
	CvSelectionGroup* pLoopSelectionGroup;
	int iLoop;

	for(pLoopSelectionGroup = firstSelectionGroup(&iLoop); pLoopSelectionGroup; pLoopSelectionGroup = nextSelectionGroup(&iLoop))
	{
		pLoopSelectionGroup->updateTimers(); // could destroy the selection group...
	}

	// if a unit was busy, perhaps it was not quite deleted yet, give it one more try
	if (getNumSelectionGroups() > getNumUnits())
	{
		for(pLoopSelectionGroup = firstSelectionGroup(&iLoop); pLoopSelectionGroup; pLoopSelectionGroup = nextSelectionGroup(&iLoop))
		{
			pLoopSelectionGroup->doDelayedDeath(); // could destroy the selection group...
		}
	}

	FAssertMsg(getNumSelectionGroups() <= getNumUnits(), "The number of Units is expected not to exceed the number of Selection Groups");
}


bool CvPlayer::hasReadyUnit(bool bAny) const
{
	PROFILE_FUNC();

	CvSelectionGroup* pLoopSelectionGroup;
	int iLoop;

	for(pLoopSelectionGroup = firstSelectionGroup(&iLoop); pLoopSelectionGroup; pLoopSelectionGroup = nextSelectionGroup(&iLoop))
	{
		//Only Update Units who are not in AIGroups	added Sephi
		if(pLoopSelectionGroup->isUpdateNecessary())
		{
		//end added
			if (pLoopSelectionGroup->readyToMove(bAny))
			{
				return true;
			}
		}
	}

	return false;
}


bool CvPlayer::hasAutoUnit() const
{
	PROFILE_FUNC();

	CvSelectionGroup* pLoopSelectionGroup;
	int iLoop;

	for(pLoopSelectionGroup = firstSelectionGroup(&iLoop); pLoopSelectionGroup; pLoopSelectionGroup = nextSelectionGroup(&iLoop))
	{
		if (pLoopSelectionGroup->readyToAuto())
		{
			return true;
		}
	}

	return false;
}


bool CvPlayer::hasBusyUnit() const
{
	PROFILE_FUNC();

	CvSelectionGroup* pLoopSelectionGroup;
	int iLoop;

	for(pLoopSelectionGroup = firstSelectionGroup(&iLoop); pLoopSelectionGroup; pLoopSelectionGroup = nextSelectionGroup(&iLoop))
	{
		if (pLoopSelectionGroup->isBusy())
		{
		    if (pLoopSelectionGroup->getNumUnits() == 0)
		    {
		        pLoopSelectionGroup->kill();
		        return false;
		    }

			return true;
		}
	}

	return false;
}


void CvPlayer::chooseTech(int iDiscover, CvWString szText, bool bFront)
{
	CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_CHOOSETECH);
	if (NULL != pInfo)
	{
		//edit Sephi
		//fix bug the dirty way - block ability to gain free techs (not balanced at all)
		//pInfo->setData1(iDiscover);
		pInfo->setData1(0);
		pInfo->setText(szText);
		gDLL->getInterfaceIFace()->addPopup(pInfo, getID(), false, bFront);
	}
}


int CvPlayer::calculateScore(bool bFinal, bool bVictory)
{
	PROFILE_FUNC();

	if (!isAlive())
	{
		return 0;
	}

	if (GET_TEAM(getTeam()).getNumMembers() == 0)
	{
		return 0;
	}

	long lScore = 0;

	CyArgsList argsList;
	argsList.add((int) getID());
	argsList.add(bFinal);
	argsList.add(bVictory);
	gDLL->getPythonIFace()->callFunction(PYGameModule, "calculateScore", argsList.makeFunctionArgs(), &lScore);

	return ((int)lScore);
}


int CvPlayer::findBestFoundValue() const
{
	CvArea* pLoopArea;
	int iValue;
	int iBestValue;
	int iLoop;

	iBestValue = 0;

	for(pLoopArea = GC.getMapINLINE().firstArea(&iLoop); pLoopArea != NULL; pLoopArea = GC.getMapINLINE().nextArea(&iLoop))
	{
		iValue = pLoopArea->getBestFoundValue(getID());

		if (iValue > iBestValue)
		{
			iBestValue = iValue;
		}
	}

	return iBestValue;
}


int CvPlayer::upgradeAllPrice(UnitTypes eUpgradeUnit, UnitTypes eFromUnit)
{
	CvUnit* pLoopUnit;
	int iPrice;
	int iLoop;

	iPrice = 0;

	// Loop through units and determine the total power of this player's military
	for (pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{
		if (pLoopUnit->getUnitType() == eFromUnit)
		{
			if (pLoopUnit->canUpgrade(eUpgradeUnit, true))
			{
				iPrice += pLoopUnit->upgradePrice(eUpgradeUnit);
			}
		}
	}

	return iPrice;
}


int CvPlayer::countReligionSpreadUnits(CvArea* pArea, ReligionTypes eReligion) const
{
	PROFILE_FUNC();

	CvUnit* pLoopUnit;
	int iCount;
	int iLoop;

	iCount = 0;

	for (pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{
		if (pLoopUnit->getArea() == pArea->getID())
		{
			if (pLoopUnit->getUnitInfo().getReligionSpreads(eReligion) > 0)
			{
				iCount++;
			}
		}
	}

	return iCount;
}

int CvPlayer::countCorporationSpreadUnits(CvArea* pArea, CorporationTypes eCorporation) const
{
	PROFILE_FUNC();

	int iCount = 0;
	int iLoop;
	for (CvUnit* pLoopUnit = firstUnit(&iLoop); NULL != pLoopUnit; pLoopUnit = nextUnit(&iLoop))
	{
		if (pLoopUnit->area() == pArea)
		{
			if (pLoopUnit->getUnitInfo().getCorporationSpreads(eCorporation) > 0)
			{
				++iCount;
			}
		}
	}

	return iCount;
}


int CvPlayer::countNumCoastalCities() const
{
	CvCity* pLoopCity;
	int iCount;
	int iLoop;

	iCount = 0;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		if (pLoopCity->isCoastal(GC.getMIN_WATER_SIZE_FOR_OCEAN()))
		{
			iCount++;
		}
	}

	return iCount;
}


int CvPlayer::countNumCoastalCitiesByArea(CvArea* pArea) const
{
	CvCity* pLoopCity;
	int iCount;
	int iLoop;

	iCount = 0;

	int iAreaID = pArea->getID();
	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		if (pLoopCity->isCoastal(GC.getMIN_WATER_SIZE_FOR_OCEAN()))
		{
			if ((pLoopCity->getArea() == iAreaID) || pLoopCity->plot()->isAdjacentToArea(iAreaID))
			{
				iCount++;
			}
		}
	}

	return iCount;
}


int CvPlayer::countTotalCulture() const
{
	CvCity* pLoopCity;
	int iCount;
	int iLoop;

	iCount = 0;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iCount += pLoopCity->getCultureTimes100(getID());
	}

	return iCount/100;
}


int CvPlayer::countOwnedBonuses(BonusTypes eBonus) const
{
	PROFILE("CvPlayer::countOwnedBonuses");
    CvCity* pLoopCity;
	CvPlot* pLoopPlot;
	int iCount;
	int iI;
    int iLoop;

    bool bAdvancedStart = (getAdvancedStartPoints() >= 0) && (getCurrentEra() < 3);

	iCount = 0;

	//count bonuses outside city radius
	for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

		if ((pLoopPlot->getOwnerINLINE() == getID()) && !pLoopPlot->isCityRadius())
		{
			if (pLoopPlot->getBonusType(getTeam()) == eBonus)
			{
				iCount++;
			}
		}
		else if (bAdvancedStart && pLoopPlot->isRevealed(getTeam(), false))
		{
			if (pLoopPlot->getBonusType(getTeam()) == eBonus)
			{
				iCount++;
			}
		}
	}



	//count bonuses inside city radius or easily claimed
	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iCount += pLoopCity->AI_countNumBonuses(eBonus, true, pLoopCity->getCommerceRate(COMMERCE_CULTURE) > 0, -1);
	}

	return iCount;
}


int CvPlayer::countUnimprovedBonuses(CvArea* pArea, CvPlot* pFromPlot) const
{
	PROFILE_FUNC();

	CvPlot* pLoopPlot;
	ImprovementTypes eImprovement;
	BuildTypes eBuild;
	BonusTypes eNonObsoleteBonus;
	int iCount;
	int iI, iJ;

	gDLL->getFAStarIFace()->ForceReset(&GC.getBorderFinder());

	iCount = 0;

	for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

		if (pLoopPlot->area() == pArea)
		{
			if (pLoopPlot->getOwnerINLINE() == getID())
			{
				if (!(pLoopPlot->isCity()))
				{
					eNonObsoleteBonus = pLoopPlot->getNonObsoleteBonusType(getTeam());

					if (eNonObsoleteBonus != NO_BONUS)
					{
						eImprovement = pLoopPlot->getImprovementType();

						if ((eImprovement == NO_IMPROVEMENT) || !(GC.getImprovementInfo(eImprovement).isImprovementBonusTrade(eNonObsoleteBonus)))
						{
							if ((pFromPlot == NULL) || gDLL->getFAStarIFace()->GeneratePath(&GC.getBorderFinder(), pFromPlot->getX_INLINE(), pFromPlot->getY_INLINE(), pLoopPlot->getX_INLINE(), pLoopPlot->getY_INLINE(), false, getID(), true))
							{
								for (iJ = 0; iJ < GC.getNumBuildInfos(); iJ++)
								{
									eBuild = ((BuildTypes)iJ);

									if (GC.getBuildInfo(eBuild).getImprovement() != NO_IMPROVEMENT)
									{
										if (GC.getImprovementInfo((ImprovementTypes)(GC.getBuildInfo(eBuild).getImprovement())).isImprovementBonusTrade(eNonObsoleteBonus))
										{
											if (canBuild(pLoopPlot, eBuild))
											{
												iCount++;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return iCount;
}


int CvPlayer::countCityFeatures(FeatureTypes eFeature) const
{
	PROFILE_FUNC();

	CvCity* pLoopCity;
	CvPlot* pLoopPlot;
	int iCount;
	int iLoop;
	int iI;

	iCount = 0;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		for (iI = 0; iI < NUM_CITY_PLOTS; iI++)
		{
			pLoopPlot = plotCity(pLoopCity->getX_INLINE(), pLoopCity->getY_INLINE(), iI);

			if (pLoopPlot != NULL)
			{
				if (pLoopPlot->getFeatureType() == eFeature)
				{
					iCount++;
				}
			}
		}
	}

	return iCount;
}


int CvPlayer::countNumBuildings(BuildingTypes eBuilding) const
{
	PROFILE_FUNC();

	CvCity* pLoopCity;
	int iCount;
	int iLoop;

	iCount = 0;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		if (pLoopCity->getNumBuilding(eBuilding) > 0)
		{
			iCount += pLoopCity->getNumBuilding(eBuilding);
		}
	}

	return iCount;
}


int CvPlayer::countNumCitiesConnectedToCapital() const
{
	CvCity* pLoopCity;
	int iCount;
	int iLoop;

	iCount = 0;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		if (pLoopCity->isConnectedToCapital())
		{
			iCount++;
		}
	}

	return iCount;
}


int CvPlayer::countPotentialForeignTradeCities(CvArea* pIgnoreArea) const
{
	int iTempValue;
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		if (GET_TEAM((TeamTypes)iI).isAlive())
		{
			if (iI != getTeam())
			{
				if (GET_TEAM(getTeam()).isFreeTrade((TeamTypes)iI))
				{
					iTempValue = GET_TEAM((TeamTypes)iI).getNumCities();

					if (pIgnoreArea != NULL)
					{
						iTempValue -= GET_TEAM((TeamTypes)iI).countNumCitiesByArea(pIgnoreArea);
					}

					iCount += iTempValue;
				}
			}
		}
	}

	return iCount;
}


int CvPlayer::countPotentialForeignTradeCitiesConnected() const
{
	CvCity* pCapitalCity;
	CvCity* pLoopCity;
	int iCount;
	int iLoop;
	int iI;

	iCount = 0;

	pCapitalCity = getCapitalCity();

	if (pCapitalCity != NULL)
	{
		for (iI = 0; iI < MAX_CIV_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() != getTeam())
				{
					if (GET_TEAM(getTeam()).isFreeTrade(GET_PLAYER((PlayerTypes)iI).getTeam()))
					{
						for (pLoopCity = GET_PLAYER((PlayerTypes)iI).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iI).nextCity(&iLoop))
						{
							FAssert(pLoopCity->getOwnerINLINE() != getID());
							FAssert(pLoopCity->getTeam() != getTeam());

							if (pLoopCity->plotGroup(getID()) == pCapitalCity->plotGroup(getID()))
							{
								iCount++;
							}
						}
					}
				}
			}
		}
	}

	return iCount;
}



bool CvPlayer::canContact(PlayerTypes ePlayer) const
{
	if (ePlayer == getID())
	{
		return false;
	}

	if (!isAlive() || !(GET_PLAYER(ePlayer).isAlive()))
	{
		return false;
	}

	if (isBarbarian() || GET_PLAYER(ePlayer).isBarbarian())
	{
		return false;
	}

	if (isMinorCiv() || GET_PLAYER(ePlayer).isMinorCiv())
	{
		return false;
	}

	if (getTeam() != GET_PLAYER(ePlayer).getTeam())
	{
		if (!(GET_TEAM(getTeam()).isHasMet(GET_PLAYER(ePlayer).getTeam())))
		{
			return false;
		}

		if (atWar(getTeam(), GET_PLAYER(ePlayer).getTeam()))
		{

//FfH: Added by Kael 08/14/2007
            if (isNoDiplomacyWithEnemies())
            {
                return false;
            }
//FfH: End Add

			if (!(GET_TEAM(getTeam()).canChangeWarPeace(GET_PLAYER(ePlayer).getTeam())))
			{
				return false;
			}
		}

		if (isHuman() || GET_PLAYER(ePlayer).isHuman())
		{
			if (GC.getGameINLINE().isOption(GAMEOPTION_ALWAYS_WAR))
			{
				return false;
			}
		}
	}

//FfH: Added by Kael 11/06/2007
    if (getTempPlayerTimer() > 0)
    {
        return false;
    }
//FfH: End Add

	return true;
}


void CvPlayer::contact(PlayerTypes ePlayer)
{
	CvDiploParameters* pDiplo;

	if (!canContact(ePlayer) || isTurnDone())
	{
		return;
	}

	if (GET_PLAYER(ePlayer).isHuman())
	{
		if (GC.getGameINLINE().isPbem() || GC.getGameINLINE().isHotSeat() || (GC.getGameINLINE().isPitboss() && !gDLL->isConnected(GET_PLAYER(ePlayer).getNetID())))
		{
			if (gDLL->isMPDiplomacy())
			{
				gDLL->beginMPDiplomacy(ePlayer, false, false);
			}
		}
		else
		{
			if (gDLL->getInterfaceIFace()->isFlashing(ePlayer))
			{
				if (!gDLL->getInterfaceIFace()->isDiplomacyLocked())
				{
					gDLL->getInterfaceIFace()->setDiplomacyLocked(true);
					gDLL->sendContactCiv(NETCONTACT_RESPONSE, ePlayer);
				}
			}
			else
			{
				gDLL->sendContactCiv(NETCONTACT_INITIAL, ePlayer);
			}
		}
	}
	else
	{
		pDiplo = new CvDiploParameters(ePlayer);
		FAssert(pDiplo != NULL);
		if (gDLL->ctrlKey())
		{
			pDiplo->setDiploComment((DiploCommentTypes)GC.getInfoTypeForString("AI_DIPLOCOMMENT_TRADING"));
		}
		gDLL->getInterfaceIFace()->setDiploQueue(pDiplo, GC.getGameINLINE().getActivePlayer());
	}
}


void CvPlayer::handleDiploEvent(DiploEventTypes eDiploEvent, PlayerTypes ePlayer, int iData1, int iData2)
{
	CivicTypes* paeNewCivics;
	CvCity* pCity;
	int iI;

	FAssertMsg(ePlayer != getID(), "shouldn't call this function on ourselves");

	switch (eDiploEvent)
	{
	case DIPLOEVENT_CONTACT:
		AI_setFirstContact(ePlayer, true);
		GET_PLAYER(ePlayer).AI_setFirstContact(getID(), true);
		break;

	case DIPLOEVENT_AI_CONTACT:
		break;

	case DIPLOEVENT_FAILED_CONTACT:
		AI_setFirstContact(ePlayer, true);
		GET_PLAYER(ePlayer).AI_setFirstContact(getID(), true);
		break;

	case DIPLOEVENT_GIVE_HELP:
		AI_changeMemoryCount(ePlayer, MEMORY_GIVE_HELP, 1);
		forcePeace(ePlayer);
		break;

	case DIPLOEVENT_REFUSED_HELP:
		AI_changeMemoryCount(ePlayer, MEMORY_REFUSED_HELP, 1);
		break;

	case DIPLOEVENT_ACCEPT_DEMAND:
		AI_changeMemoryCount(ePlayer, MEMORY_ACCEPT_DEMAND, 1);
		forcePeace(ePlayer);
		break;

	case DIPLOEVENT_REJECTED_DEMAND:
		FAssertMsg(GET_PLAYER(ePlayer).getTeam() != getTeam(), "shouldn't call this function on our own team");

		AI_changeMemoryCount(ePlayer, MEMORY_REJECTED_DEMAND, 1);

		if (AI_demandRebukedSneak(ePlayer))
		{
			GET_TEAM(getTeam()).AI_setWarPlan(GET_PLAYER(ePlayer).getTeam(), WARPLAN_PREPARING_LIMITED);
		}
		break;

	case DIPLOEVENT_DEMAND_WAR:
		FAssertMsg(GET_PLAYER(ePlayer).getTeam() != getTeam(), "shouldn't call this function on our own team");

		GET_TEAM(getTeam()).declareWar(GET_PLAYER(ePlayer).getTeam(), false, WARPLAN_LIMITED);
		break;

	case DIPLOEVENT_CONVERT:
		AI_changeMemoryCount(ePlayer, MEMORY_ACCEPTED_RELIGION, 1);
		GET_PLAYER(ePlayer).convert(getStateReligion());
		break;

	case DIPLOEVENT_NO_CONVERT:
		AI_changeMemoryCount(ePlayer, MEMORY_DENIED_RELIGION, 1);
		break;

	case DIPLOEVENT_REVOLUTION:
		AI_changeMemoryCount(ePlayer, MEMORY_ACCEPTED_CIVIC, 1);

		paeNewCivics = new CivicTypes[GC.getNumCivicOptionInfos()];

		for (iI = 0; iI < GC.getNumCivicOptionInfos(); iI++)
		{
			paeNewCivics[iI] = GET_PLAYER(ePlayer).getCivics((CivicOptionTypes)iI);
		}

		FAssertMsg(GC.getLeaderHeadInfo(getPersonalityType()).getFavoriteCivic() != NO_CIVIC, "getFavoriteCivic() must be valid");

		paeNewCivics[GC.getCivicInfo((CivicTypes)(GC.getLeaderHeadInfo(getPersonalityType())).getFavoriteCivic()).getCivicOptionType()] = ((CivicTypes)(GC.getLeaderHeadInfo(getPersonalityType()).getFavoriteCivic()));

		GET_PLAYER(ePlayer).revolution(paeNewCivics, true);

		SAFE_DELETE_ARRAY(paeNewCivics);
		break;

	case DIPLOEVENT_NO_REVOLUTION:
		AI_changeMemoryCount(ePlayer, MEMORY_DENIED_CIVIC, 1);
		break;

	case DIPLOEVENT_JOIN_WAR:
		AI_changeMemoryCount(ePlayer, MEMORY_ACCEPTED_JOIN_WAR, 1);
		GET_TEAM(GET_PLAYER(ePlayer).getTeam()).declareWar(((TeamTypes)iData1), false, WARPLAN_DOGPILE);

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == ((TeamTypes)iData1))
				{
					GET_PLAYER((PlayerTypes)iI).AI_changeMemoryCount(getID(), MEMORY_HIRED_WAR_ALLY, 1);
				}
			}
		}
		break;

	case DIPLOEVENT_NO_JOIN_WAR:
		AI_changeMemoryCount(ePlayer, MEMORY_DENIED_JOIN_WAR, 1);
		break;

	case DIPLOEVENT_STOP_TRADING:
		AI_changeMemoryCount(ePlayer, MEMORY_ACCEPTED_STOP_TRADING, 1);
		GET_PLAYER(ePlayer).stopTradingWithTeam((TeamTypes)iData1);

		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)iI).getTeam() == ((TeamTypes)iData1))
				{
					GET_PLAYER((PlayerTypes)iI).AI_changeMemoryCount(getID(), MEMORY_HIRED_TRADE_EMBARGO, 1);
				}
			}
		}
		break;

	case DIPLOEVENT_NO_STOP_TRADING:
		AI_changeMemoryCount(ePlayer, MEMORY_DENIED_STOP_TRADING, 1);
		break;

	case DIPLOEVENT_ASK_HELP:
		AI_changeMemoryCount(ePlayer, MEMORY_MADE_DEMAND_RECENT, 1);
		break;

	case DIPLOEVENT_MADE_DEMAND:
		if (AI_getMemoryCount(ePlayer, MEMORY_MADE_DEMAND) < 10)
		{
			AI_changeMemoryCount(ePlayer, MEMORY_MADE_DEMAND, 1);
		}
		AI_changeMemoryCount(ePlayer, MEMORY_MADE_DEMAND_RECENT, 1);
		break;

	case DIPLOEVENT_MADE_DEMAND_VASSAL:
		break;

	case DIPLOEVENT_RESEARCH_TECH:
		pushResearch(((TechTypes)iData1), true);
		break;

	case DIPLOEVENT_TARGET_CITY:
		pCity = GET_PLAYER((PlayerTypes)iData1).getCity(iData2);
		if (pCity != NULL)
		{
			pCity->area()->setTargetCity(getID(), pCity);
		}
		break;

	default:
		FAssert(false);
		break;
	}
}


bool CvPlayer::canTradeWith(PlayerTypes eWhoTo) const
{
	if (atWar(getTeam(), GET_PLAYER(eWhoTo).getTeam()))
	{
		return true;
	}

	if (GET_TEAM(getTeam()).isTechTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isTechTrading())
	{
		return true;
	}

	if (GET_TEAM(getTeam()).isGoldTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isGoldTrading())
	{
		return true;
	}

	if (GET_TEAM(getTeam()).isMapTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isMapTrading())
	{
		return true;
	}

	if (canTradeNetworkWith(eWhoTo))
	{
		return true;
	}

	if (GET_TEAM(getTeam()).isOpenBordersTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isOpenBordersTrading())
	{
		return true;
	}

	if (GET_TEAM(getTeam()).isDefensivePactTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isDefensivePactTrading())
	{
		return true;
	}

	if (GET_TEAM(getTeam()).isPermanentAllianceTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isPermanentAllianceTrading())
	{
		return true;
	}

	if (GET_TEAM(getTeam()).isVassalStateTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isVassalStateTrading())
	{
		return true;
	}

	return false;
}

bool CvPlayer::canReceiveTradeCity() const
{
	if (GC.getGameINLINE().isOption(GAMEOPTION_ONE_CITY_CHALLENGE) && isHuman())
	{
		return false;
	}

	return true;
}

bool CvPlayer::canTradeItem(PlayerTypes eWhoTo, TradeData item, bool bTestDenial) const
{
	CvCity *pOurCapitalCity;

	if (bTestDenial)
	{
		if (getTradeDenial(eWhoTo, item) != NO_DENIAL)
		{
			return false;
		}
	}

//FfH: Added by Kael 11/20/2007
    bool bCanTrade = true;
	for (int iI = 0; iI < GC.getNumVoteSourceInfos(); iI++)
	{
	    if (isFullMember((VoteSourceTypes)iI))
	    {
	        if (GC.getGameINLINE().isNoOutsideTechTrades((VoteSourceTypes)iI))
	        {
	            if (!GET_PLAYER(eWhoTo).isFullMember((VoteSourceTypes)iI))
	            {
	                bCanTrade = false;
	            }
	        }
	    }
	}
//FfH: End Add

	switch (item.m_eItemType)
	{
	case TRADE_TECHNOLOGIES:
		if (!(GC.getGameINLINE().isOption(GAMEOPTION_NO_TECH_TRADING)))
		{
			if (GC.getTechInfo((TechTypes)(item.m_iData)).isTrade())
			{
				if (GET_TEAM(getTeam()).isHasTech((TechTypes)(item.m_iData)) 
					&& GET_TEAM(getTeam()).hasMasteredTech((TechTypes)(item.m_iData))
					&& !(GET_TEAM(getTeam()).isNoTradeTech((TechTypes)(item.m_iData)))

//FfH: Added by Kael 11/20/2007
                  && bCanTrade
//FfH: End Add

				  )
				{
					if (!GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isHasTech((TechTypes)(item.m_iData)))
					{
						//if (GET_PLAYER(eWhoTo).isHuman() || (GET_PLAYER(eWhoTo).getCurrentResearch() != item.m_iData))
						{
							if (GET_TEAM(getTeam()).isTechTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isTechTrading())
							{
								FAssertMsg(item.m_iData >= 0, "item.m_iData is expected to be non-negative (invalid Index)");

								if (GET_PLAYER(eWhoTo).canResearch(((TechTypes)item.m_iData), true))
								{
									return true;
								}
							}
						}
					}
				}
			}
		}
		break;

	case TRADE_RESOURCES:
		FAssertMsg(item.m_iData > -1, "iData is expected to be non-negative");

		if (canTradeNetworkWith(eWhoTo))
		{
			if (!GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isBonusObsolete((BonusTypes) item.m_iData) && !GET_TEAM(getTeam()).isBonusObsolete((BonusTypes) item.m_iData))
			{
				bool bCanTradeAll = (isHuman() || getTeam() == GET_PLAYER(eWhoTo).getTeam() || GET_TEAM(getTeam()).isVassal(GET_PLAYER(eWhoTo).getTeam()));
				if (getNumTradeableBonuses((BonusTypes) item.m_iData) > (bCanTradeAll ? 0 : 1))
				{
					// if (GET_PLAYER(eWhoTo).getNumAvailableBonuses(eBonus) == 0)
					{
						return true;
					}
				}
			}
		}
		break;

	case TRADE_CITIES:
		{
			CvCity* pCityTraded = getCity(item.m_iData);

			if (NULL != pCityTraded && pCityTraded->getLiberationPlayer(false) == eWhoTo)
			{
				return true;
			}

			if (GET_PLAYER(eWhoTo).canReceiveTradeCity())
			{
				if (0 == GC.getGameINLINE().getMaxCityElimination())
				{
					if (!GET_TEAM(getTeam()).isAVassal() && !GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isVassal(getTeam()))
					{
						pOurCapitalCity = getCapitalCity();
						if (pOurCapitalCity != NULL)
						{
							if (pOurCapitalCity->getID() != item.m_iData)
							{
								return true;
							}
						}
					}
				}
			}
		}
		break;

	case TRADE_GOLD:
		if (GET_TEAM(getTeam()).isGoldTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isGoldTrading())
		{
			if (getGold() >= item.m_iData)
			{
				return true;
			}
		}
		break;

	case TRADE_GOLD_PER_TURN:
		if (GET_TEAM(getTeam()).isGoldTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isGoldTrading())
		{
			return true;
		}
		break;

	case TRADE_MAPS:
		if (!(GC.getGameINLINE().isOption(GAMEOPTION_NO_MAP_TRADING)))
		{
            if (getTeam() != GET_PLAYER(eWhoTo).getTeam())
            {
                if (GET_TEAM(getTeam()).isMapTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isMapTrading())
                {
                    return true;
                }
            }
		}
		break;

	case TRADE_VASSAL:
	case TRADE_SURRENDER:
		if (!isHuman() || GET_PLAYER(eWhoTo).isHuman()) //  human can't be vassal of AI
		{
			CvTeam& kVassalTeam = GET_TEAM(getTeam());
			CvTeam& kMasterTeam = GET_TEAM(GET_PLAYER(eWhoTo).getTeam());

/*************************************************************************************************/
/**	ADDON (Houses of Erebus) Sephi			                                 					**/
/**																								**/
/**	block Houses of being Vassals                                      							**/
/*************************************************************************************************/
			if (kVassalTeam.isHouseErebus() || kMasterTeam.isHouseErebus())
			{
				break;
			}
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
			if (kMasterTeam.isVassalStateTrading()) // the master must possess the tech
			{
				if (!kVassalTeam.isAVassal() && !kMasterTeam.isAVassal() && getTeam() != GET_PLAYER(eWhoTo).getTeam())
				{
					if ((kMasterTeam.isAtWar(getTeam()) || item.m_iData == 1) && item.m_eItemType == TRADE_SURRENDER)
					{
						return true;
					}

					if (!kMasterTeam.isAtWar(getTeam()) && item.m_eItemType == TRADE_VASSAL)
					{
						return true;
					}
				}
			}
		}
		break;

	case TRADE_PEACE:
		if (!(GET_TEAM(getTeam()).isHuman()))
		{
			if (!(GET_TEAM(getTeam()).isAVassal()))
			{
				if (GET_TEAM(getTeam()).isHasMet((TeamTypes)(item.m_iData)) && GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isHasMet((TeamTypes)(item.m_iData)))
				{
					if (atWar(getTeam(), ((TeamTypes)(item.m_iData))))
					{
						return true;
					}
				}
			}
		}
		break;

	case TRADE_WAR:
		if (!(GET_TEAM(getTeam()).isHuman()))
		{
			if (!(GET_TEAM(getTeam()).isAVassal()))
			{
				if (!GET_TEAM((TeamTypes)item.m_iData).isAVassal())
				{
					if (GET_TEAM(getTeam()).isHasMet((TeamTypes)(item.m_iData)) && GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isHasMet((TeamTypes)(item.m_iData)))
					{
						if (GET_TEAM(getTeam()).canDeclareWar((TeamTypes)(item.m_iData)))
						{
							return true;
						}
					}
				}
			}
		}
		break;

	case TRADE_EMBARGO:
		if (!(GET_TEAM(getTeam()).isHuman()))
		{
			if (GET_TEAM(getTeam()).isHasMet((TeamTypes)(item.m_iData)) && GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isHasMet((TeamTypes)(item.m_iData)))
			{
				if (canStopTradingWithTeam((TeamTypes)(item.m_iData)))
				{
					return true;
				}
			}
		}
		break;

	case TRADE_CIVIC:
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       10/22/09                          denev & jdog5000    */
/*                                                                                              */
/* Diplomacy                                                                                    */
/************************************************************************************************/
/* original bts code
		if (!(GET_TEAM(getTeam()).isHuman()))
*/
		if (!(GET_TEAM(getTeam()).isHuman()) || (getTeam() == GET_PLAYER(eWhoTo).getTeam()))
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
		{
			if (GET_PLAYER(eWhoTo).isCivic((CivicTypes)(item.m_iData)))
			{
				if (canDoCivics((CivicTypes)(item.m_iData)) && !isCivic((CivicTypes)(item.m_iData)))
				{
					if (canRevolution(NULL))
					{
						return true;
					}
				}
			}
		}
		break;

	case TRADE_RELIGION:
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       10/22/09                          denev & jdog5000    */
/*                                                                                              */
/* Diplomacy                                                                                    */
/************************************************************************************************/
/* original bts code
		if (!(GET_TEAM(getTeam()).isHuman()))
*/
		if (!(GET_TEAM(getTeam()).isHuman()) || (getTeam() == GET_PLAYER(eWhoTo).getTeam()))
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
		{
			if (GET_PLAYER(eWhoTo).getStateReligion() == ((ReligionTypes)(item.m_iData)))
			{
				if (canConvert((ReligionTypes)(item.m_iData)))
				{
					return true;
				}
			}
		}
		break;

	case TRADE_OPEN_BORDERS:
		if (getTeam() != GET_PLAYER(eWhoTo).getTeam())
		{
			if (!atWar(getTeam(), GET_PLAYER(eWhoTo).getTeam()))
			{
				if (!(GET_TEAM(getTeam()).isOpenBorders(GET_PLAYER(eWhoTo).getTeam())))
				{
					if (GET_TEAM(getTeam()).isOpenBordersTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isOpenBordersTrading())
					{
						return true;
					}
				}
			}
		}
		break;

	case TRADE_DEFENSIVE_PACT:
		if (!(GET_TEAM(getTeam()).isAVassal()) && !(GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isAVassal()))
		{
			if (getTeam() != GET_PLAYER(eWhoTo).getTeam() && !GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isVassal(getTeam()))
			{
				if (!atWar(getTeam(), GET_PLAYER(eWhoTo).getTeam()))
				{
					if (!(GET_TEAM(getTeam()).isDefensivePact(GET_PLAYER(eWhoTo).getTeam())))
					{
						if (GET_TEAM(getTeam()).isDefensivePactTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isDefensivePactTrading())
						{
							if ((GET_TEAM(getTeam()).getAtWarCount(true) == 0) && (GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).getAtWarCount(true) == 0))
							{
								if (GET_TEAM(getTeam()).canSignDefensivePact(GET_PLAYER(eWhoTo).getTeam()))
								{
									return true;
								}
							}
						}
					}
				}
			}
		}
		break;

	case TRADE_PERMANENT_ALLIANCE:
		if (!(GET_TEAM(getTeam()).isAVassal()) && !(GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isAVassal()))
		{
			if (getTeam() != GET_PLAYER(eWhoTo).getTeam() && !GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isVassal(getTeam()))
			{
				if (!atWar(getTeam(), GET_PLAYER(eWhoTo).getTeam()))
				{
					if (GET_TEAM(getTeam()).isPermanentAllianceTrading() || GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).isPermanentAllianceTrading())
					{
						if ((GET_TEAM(getTeam()).getNumMembers() == 1) && (GET_TEAM(GET_PLAYER(eWhoTo).getTeam()).getNumMembers() == 1))
						{
							return true;
						}
					}
				}
			}
		}
		break;

	case TRADE_PEACE_TREATY:
		return true;
		break;
	}

	return false;
}


DenialTypes CvPlayer::getTradeDenial(PlayerTypes eWhoTo, TradeData item) const
{
	CvCity* pCity;

	switch (item.m_eItemType)
	{
	case TRADE_TECHNOLOGIES:
		return GET_TEAM(getTeam()).AI_techTrade(((TechTypes)(item.m_iData)), GET_PLAYER(eWhoTo).getTeam());
		break;

	case TRADE_RESOURCES:
		return AI_bonusTrade(((BonusTypes)(item.m_iData)), eWhoTo);
		break;

	case TRADE_CITIES:
		pCity = getCity(item.m_iData);
		if (pCity != NULL)
		{
			return AI_cityTrade(pCity, eWhoTo);
		}
		break;

	case TRADE_GOLD:
	case TRADE_GOLD_PER_TURN:
		break;

	case TRADE_MAPS:
		return GET_TEAM(getTeam()).AI_mapTrade(GET_PLAYER(eWhoTo).getTeam());
		break;

	case TRADE_SURRENDER:
		return GET_TEAM(getTeam()).AI_surrenderTrade(GET_PLAYER(eWhoTo).getTeam(), 140);
		break;

	case TRADE_VASSAL:
		return GET_TEAM(getTeam()).AI_vassalTrade(GET_PLAYER(eWhoTo).getTeam());
		break;

	case TRADE_PEACE:
		return GET_TEAM(getTeam()).AI_makePeaceTrade(((TeamTypes)(item.m_iData)), GET_PLAYER(eWhoTo).getTeam());
		break;

	case TRADE_WAR:
		return GET_TEAM(getTeam()).AI_declareWarTrade(((TeamTypes)(item.m_iData)), GET_PLAYER(eWhoTo).getTeam());
		break;

	case TRADE_EMBARGO:
		return AI_stopTradingTrade(((TeamTypes)(item.m_iData)), eWhoTo);
		break;

	case TRADE_CIVIC:
		return AI_civicTrade(((CivicTypes)(item.m_iData)), eWhoTo);
		break;

	case TRADE_RELIGION:
		return AI_religionTrade(((ReligionTypes)(item.m_iData)), eWhoTo);
		break;

	case TRADE_OPEN_BORDERS:
		return GET_TEAM(getTeam()).AI_openBordersTrade(GET_PLAYER(eWhoTo).getTeam());
		break;

	case TRADE_DEFENSIVE_PACT:
		return GET_TEAM(getTeam()).AI_defensivePactTrade(GET_PLAYER(eWhoTo).getTeam());
		break;

	case TRADE_PERMANENT_ALLIANCE:
		return GET_TEAM(getTeam()).AI_permanentAllianceTrade(GET_PLAYER(eWhoTo).getTeam());
		break;

	case TRADE_PEACE_TREATY:
		break;
	}

	return NO_DENIAL;
}


bool CvPlayer::canTradeNetworkWith(PlayerTypes ePlayer) const
{
	CvCity* pOurCapitalCity;

	pOurCapitalCity = getCapitalCity();

	if (pOurCapitalCity != NULL)
	{
		if (pOurCapitalCity->isConnectedToCapital(ePlayer))
		{
			return true;
		}
	}

	return false;
}


int CvPlayer::getNumAvailableBonuses(BonusTypes eBonus) const
{
	CvPlotGroup* pPlotGroup;

	pPlotGroup = ((getCapitalCity() != NULL) ? getCapitalCity()->plot()->getOwnerPlotGroup() : NULL);

//FfH: Added by Kael 11/14/2007
    if (isFullMember((VoteSourceTypes)0))
    {
        if (GC.getGameINLINE().isNoBonus(eBonus))
        {
            return 0;
        }
    }
//FfH: End Add

	if (pPlotGroup != NULL)
	{
		//edit Sephi
		//return pPlotGroup->getNumBonuses(eBonus);
		return pPlotGroup->getNumBonuses(eBonus)-getNumBonusesConsumed(eBonus);
	}

	return 0;
}


int CvPlayer::getNumTradeableBonuses(BonusTypes eBonus) const
{
/*************************************************************************************************/
/**	ADDON (Mana cannot be traded) Sephi                                                         **/
/*************************************************************************************************/
    if (GC.getBonusInfo(eBonus).isNoTrade())
    {
        return 0;
    }
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
	return (getNumAvailableBonuses(eBonus) - getBonusImport(eBonus));
}

bool CvPlayer::hasBonus(BonusTypes eBonus) const
{
	int iLoop;
	for (CvCity* pLoopCity = firstCity(&iLoop); NULL != pLoopCity; pLoopCity = nextCity(&iLoop))
	{
		if (pLoopCity->hasBonus(eBonus))
		{
			return true;
		}
	}

	return false;
}

int CvPlayer::getNumTradeBonusImports(PlayerTypes ePlayer) const
{
	CLLNode<TradeData>* pNode;
	CvDeal* pLoopDeal;
	int iCount;
	int iLoop;

	FAssert(ePlayer != getID());

	iCount = 0;

	for(pLoopDeal = GC.getGameINLINE().firstDeal(&iLoop); pLoopDeal != NULL; pLoopDeal = GC.getGameINLINE().nextDeal(&iLoop))
	{
		if ((pLoopDeal->getFirstPlayer() == getID()) && (pLoopDeal->getSecondPlayer() == ePlayer))
		{
			for (pNode = pLoopDeal->headSecondTradesNode(); (pNode != NULL); pNode = pLoopDeal->nextSecondTradesNode(pNode))
			{
				if (pNode->m_data.m_eItemType == TRADE_RESOURCES)
				{
					iCount++;
				}
			}
		}

		if ((pLoopDeal->getFirstPlayer() == ePlayer) && (pLoopDeal->getSecondPlayer() == getID()))
		{
			for (pNode = pLoopDeal->headFirstTradesNode(); (pNode != NULL); pNode = pLoopDeal->nextFirstTradesNode(pNode))
			{
				if (pNode->m_data.m_eItemType == TRADE_RESOURCES)
				{
					iCount++;
				}
			}
		}
	}

	return iCount;
}


bool CvPlayer::isTradingWithTeam(TeamTypes eTeam, bool bIncludeCancelable) const
{
	int iLoop;

	if (eTeam == getTeam())
	{
		return false;
	}

	for (CvDeal* pLoopDeal = GC.getGameINLINE().firstDeal(&iLoop); pLoopDeal != NULL; pLoopDeal = GC.getGameINLINE().nextDeal(&iLoop))
	{
		if (bIncludeCancelable || pLoopDeal->isCancelable(getID()))
		{
			if (!pLoopDeal->isPeaceDeal())
			{
				if ((pLoopDeal->getFirstPlayer() == getID()) && (GET_PLAYER(pLoopDeal->getSecondPlayer()).getTeam() == eTeam))
				{
					if (pLoopDeal->getLengthFirstTrades() > 0)
					{
						return true;
					}
				}

				if ((pLoopDeal->getSecondPlayer() == getID()) && (GET_PLAYER(pLoopDeal->getFirstPlayer()).getTeam() == eTeam))
				{
					if (pLoopDeal->getLengthSecondTrades() > 0)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}


bool CvPlayer::canStopTradingWithTeam(TeamTypes eTeam, bool bContinueNotTrading) const
{
	if (eTeam == getTeam())
	{
		return false;
	}

	if (GET_TEAM(getTeam()).isVassal(eTeam))
	{
		return false;
	}

	if (!isTradingWithTeam(eTeam, false))
	{
		if (bContinueNotTrading && !isTradingWithTeam(eTeam, true))
		{
			return true;
		}

		return false;
	}

	return true;
}


void CvPlayer::stopTradingWithTeam(TeamTypes eTeam)
{
	CvDeal* pLoopDeal;
	int iLoop;
	int iI;

	FAssert(eTeam != getTeam());

	for(pLoopDeal = GC.getGameINLINE().firstDeal(&iLoop); pLoopDeal != NULL; pLoopDeal = GC.getGameINLINE().nextDeal(&iLoop))
	{
		if (pLoopDeal->isCancelable(getID()) && !(pLoopDeal->isPeaceDeal()))
		{
			if (((pLoopDeal->getFirstPlayer() == getID()) && (GET_PLAYER(pLoopDeal->getSecondPlayer()).getTeam() == eTeam)) ||
				  ((pLoopDeal->getSecondPlayer() == getID()) && (GET_PLAYER(pLoopDeal->getFirstPlayer()).getTeam() == eTeam)))
			{
				pLoopDeal->kill();
			}
		}
	}

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)
			{
				GET_PLAYER((PlayerTypes)iI).AI_changeMemoryCount(getID(), MEMORY_STOPPED_TRADING, 1);
				GET_PLAYER((PlayerTypes)iI).AI_changeMemoryCount(getID(), MEMORY_STOPPED_TRADING_RECENT, 1);
			}
		}
	}
}


void CvPlayer::killAllDeals()
{
	CvDeal* pLoopDeal;
	int iLoop;

	for(pLoopDeal = GC.getGameINLINE().firstDeal(&iLoop); pLoopDeal != NULL; pLoopDeal = GC.getGameINLINE().nextDeal(&iLoop))
	{
		if ((pLoopDeal->getFirstPlayer() == getID()) || (pLoopDeal->getSecondPlayer() == getID()))
		{
			pLoopDeal->kill();
		}
	}
}


void CvPlayer::findNewCapital()
{
	CvCity* pOldCapital;
	CvCity* pLoopCity;
	CvCity* pBestCity;
	BuildingTypes eCapitalBuilding;
	int iValue;
	int iBestValue;
	int iLoop;

	eCapitalBuilding = ((BuildingTypes)(GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(GC.getDefineINT("CAPITAL_BUILDINGCLASS"))));

	if (eCapitalBuilding == NO_BUILDING)
	{
		return;
	}

	pOldCapital = getCapitalCity();

	iBestValue = 0;
	pBestCity = NULL;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		if (pLoopCity != pOldCapital)
		{
			if (0 == pLoopCity->getNumRealBuilding(eCapitalBuilding))
			{
				iValue = (pLoopCity->getPopulation() * 4);

				iValue += pLoopCity->getYieldRate(YIELD_FOOD);
				iValue += (pLoopCity->getYieldRate(YIELD_PRODUCTION) * 3);
				iValue += (pLoopCity->getYieldRate(YIELD_COMMERCE) * 2);
				iValue += pLoopCity->getCultureLevel();
				iValue += pLoopCity->getReligionCount();
				iValue += pLoopCity->getCorporationCount();
				iValue += (pLoopCity->getNumGreatPeople() * 2);

				iValue *= (pLoopCity->calculateCulturePercent(getID()) + 100);
				iValue /= 100;

				if (iValue > iBestValue)
				{
					iBestValue = iValue;
					pBestCity = pLoopCity;
				}
			}
		}
	}

	if (pBestCity != NULL)
	{
		if (pOldCapital != NULL)
		{
			pOldCapital->setNumRealBuilding(eCapitalBuilding, 0);
		}
		FAssertMsg(!(pBestCity->getNumRealBuilding(eCapitalBuilding)), "(pBestCity->getNumRealBuilding(eCapitalBuilding)) did not return false as expected");
		pBestCity->setNumRealBuilding(eCapitalBuilding, 1);
	}
}


int CvPlayer::getNumGovernmentCenters() const
{
	CvCity* pLoopCity;
	int iCount;
	int iLoop;

	iCount = 0;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		if (pLoopCity->isGovernmentCenter())
		{
			iCount++;
		}
	}

	return iCount;
}


bool CvPlayer::canRaze(CvCity* pCity) const
{
	if (!pCity->isAutoRaze())
	{
		if (GC.getGameINLINE().isOption(GAMEOPTION_NO_CITY_RAZING))
		{
			return false;
		}

		if (pCity->getOwnerINLINE() != getID())
		{
			return false;
		}

		if (pCity->calculateTeamCulturePercent(getTeam()) >= GC.getDefineINT("RAZING_CULTURAL_PERCENT_THRESHOLD"))
		{
			return false;
		}
	}

	CyCity* pyCity = new CyCity(pCity);
	CyArgsList argsList;
	argsList.add(getID());	// Player ID
	argsList.add(gDLL->getPythonIFace()->makePythonObject(pyCity));	// pass in city class
	long lResult=0;
	gDLL->getPythonIFace()->callFunction(PYGameModule, "canRazeCity", argsList.makeFunctionArgs(), &lResult);
	delete pyCity;	// python fxn must not hold on to this pointer
	if (lResult == 0)
	{
		return (false);
	}

	return true;
}


void CvPlayer::raze(CvCity* pCity)
{
	wchar szBuffer[1024];
	PlayerTypes eHighestCulturePlayer;
	int iI, iJ;

	if (!canRaze(pCity))
	{
		return;
	}

	FAssert(pCity->getOwnerINLINE() == getID());

	eHighestCulturePlayer = pCity->findHighestCulture();

	if (eHighestCulturePlayer != NO_PLAYER)
	{
		if (GET_PLAYER(eHighestCulturePlayer).getTeam() != getTeam())
		{
			GET_PLAYER(eHighestCulturePlayer).AI_changeMemoryCount(getID(), MEMORY_RAZED_CITY, 1);
		}
	}

	for (iI = 0; iI < GC.getNumReligionInfos(); iI++)
	{
		if (pCity->isHolyCity((ReligionTypes)iI))
		{
			for (iJ = 0; iJ < MAX_PLAYERS; iJ++)
			{
				if (GET_PLAYER((PlayerTypes)iJ).isAlive())
				{
					if (iJ != getID())
					{
						if (GET_PLAYER((PlayerTypes)iJ).getStateReligion() == ((ReligionTypes)iI))
						{
							GET_PLAYER((PlayerTypes)iJ).AI_changeMemoryCount(getID(), MEMORY_RAZED_HOLY_CITY, 1);
						}
					}
				}
			}
		}
	}

	swprintf(szBuffer, gDLL->getText("TXT_KEY_MISC_DESTROYED_CITY", pCity->getNameKey()).GetCString());
	gDLL->getInterfaceIFace()->addMessage(getID(), true, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_CITYRAZE", MESSAGE_TYPE_MAJOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("WORLDBUILDER_CITY_EDIT")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_GREEN"), pCity->getX_INLINE(), pCity->getY_INLINE(), true, true);

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (iI != getID())
			{
				if (pCity->isRevealed(GET_PLAYER((PlayerTypes)iI).getTeam(), false))
				{
					swprintf(szBuffer, gDLL->getText("TXT_KEY_MISC_CITY_HAS_BEEN_RAZED_BY", pCity->getNameKey(), getCivilizationDescriptionKey()).GetCString());
					gDLL->getInterfaceIFace()->addMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_CITYRAZED", MESSAGE_TYPE_MAJOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("WORLDBUILDER_CITY_EDIT")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_RED"), pCity->getX_INLINE(), pCity->getY_INLINE(), true, true);
				}
			}
		}
	}

	swprintf(szBuffer, gDLL->getText("TXT_KEY_MISC_CITY_RAZED_BY", pCity->getNameKey(), getCivilizationDescriptionKey()).GetCString());
	GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getID(), szBuffer, pCity->getX_INLINE(), pCity->getY_INLINE(), (ColorTypes)GC.getInfoTypeForString("COLOR_WARNING_TEXT"));

//FfH Global Counter: Added by Kael 08/11/2007
    bool bValid = true;
   	for (iI = 0; iI < GC.getNumReligionInfos(); iI++)
	{
		if (pCity->isHasReligion((ReligionTypes)iI))
		{
		    if (GC.getReligionInfo((ReligionTypes)iI).getGlobalCounterSpread() > 0)
		    {
		        bValid = false;
		    }
		}
	}
    if (bValid)
    {
        int iCost = GC.getDefineINT("RAZE_COST_BASE");
        if ((pCity->getPopulation() - GC.getDefineINT("RAZE_COST_PER_POPULATION_OVER")) > 0)
        {
            iCost += (pCity->getPopulation() - GC.getDefineINT("RAZE_COST_PER_POPULATION_OVER")) * GC.getDefineINT("RAZE_COST_PER_POPULATION");
        }
        GC.getGameINLINE().changeGlobalCounter(iCost);
    }
   	for (iI = 0; iI < GC.getNumBuildingInfos(); iI++)
   	{
   	    if (pCity->getNumRealBuilding((BuildingTypes)iI) > 0)
   	    {
   	        if (GC.getBuildingInfo((BuildingTypes)iI).isEquipment())
   	        {
                for (int iJ = 0; iJ < GC.getNumUnitInfos(); iJ++)
                {
                    if (GC.getUnitInfo((UnitTypes)iJ).getBuildings(iI))
                    {
                        initUnit((UnitTypes)iJ, pCity->getX(), pCity->getY());
                    }
                }
   	        }
   	    }
   	}
//FfH: End Add

	// Report this event
	CvEventReporter::getInstance().cityRazed(pCity, getID());

/*************************************************************************************************/
/**	ADDON (Adventures) Sephi                                                      				**/
/** tag RazeHolyCity                                                                            **/
/*************************************************************************************************/
    for(int iK=0;iK<GC.getMAX_CIV_PLAYERS();iK++)
    {
        if(GET_PLAYER((PlayerTypes)iK).isAlive())
        {
            for(int iAdvLoop=0;iAdvLoop<GC.getNumAdventureInfos();iAdvLoop++)
            {
                if(GET_PLAYER((PlayerTypes)iK).isAdventureEnabled(iAdvLoop) && !GET_PLAYER((PlayerTypes)iK).isAdventureFinished(iAdvLoop))
                {
                    CvAdventureInfo &kAdv = GC.getAdventureInfo((AdventureTypes)iAdvLoop);
                    for(int iJ=0;iJ<GC.getMAX_ADVENTURESTEPS();iJ++)
                    {
                        if(kAdv.getAdventureStep(iJ)!=NO_ADVENTURESTEP)
                        {
                            if(!(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getRazeHolyCity()==NO_RELIGION))
                            {
                                if(pCity->isHolyCity((ReligionTypes)GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getRazeHolyCity()))
                                {
                                    GET_PLAYER((PlayerTypes)iK).changeAdventureCounter(iAdvLoop,iJ,1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
/*************************************************************************************************/
/**	END                     	                                        						**/
/*************************************************************************************************/

	disband(pCity);
}


void CvPlayer::disband(CvCity* pCity)
{
	if (getNumCities() == 1)
	{
		setFoundedFirstCity(false);
	}

	GC.getGameINLINE().addDestroyedCityName(pCity->getName());

	pCity->kill(true);
}


bool CvPlayer::canReceiveGoody(CvPlot* pPlot, GoodyTypes eGoody, CvUnit* pUnit) const
{
	CvCity* pCity;
	UnitTypes eUnit;
	bool bTechFound;
	int iI;

	if (GC.getGoodyInfo(eGoody).getExperience() > 0)
	{
		if ((pUnit == NULL) || !(pUnit->canAcquirePromotionAny()) || (GC.getGameINLINE().getElapsedGameTurns() < 10))
		{
			return false;
		}
	}

	if (GC.getGoodyInfo(eGoody).getDamagePrereq() > 0)
	{
		if ((pUnit == NULL) || (pUnit->getDamage() < ((pUnit->maxHitPoints() * GC.getGoodyInfo(eGoody).getDamagePrereq()) / 100)))
		{
			return false;
		}
	}

	if (GC.getGoodyInfo(eGoody).isTech())
	{
		bTechFound = false;

		for (iI = 0; iI < GC.getNumTechInfos(); iI++)
		{
			if (GC.getTechInfo((TechTypes) iI).isGoodyTech())
			{
				if(GET_TEAM(getTeam()).getResearchCost((TechTypes)iI) < 1000) {
					if (canResearch((TechTypes)iI))
					{
						bTechFound = true;
						break;
					}
				}
			}
		}

		if (!bTechFound)
		{
			return false;
		}
	}

	if (GC.getGoodyInfo(eGoody).isBad())
	{
		if ((pUnit == NULL) || pUnit->isNoBadGoodies())
		{
			return false;
		}
	}

	if (GC.getGoodyInfo(eGoody).getUnitClassType() != NO_UNITCLASS)
	{
		eUnit = ((UnitTypes)(GC.getCivilizationInfo(getCivilizationType()).getCivilizationUnits(GC.getGoodyInfo(eGoody).getUnitClassType())));

		if (eUnit == NO_UNIT)
		{
			return false;
		}

		if ((GC.getUnitInfo(eUnit).getCombat() > 0) && !(GC.getUnitInfo(eUnit).isOnlyDefensive()))
		{
			if (GC.getGameINLINE().isGameMultiPlayer() || (GC.getGameINLINE().getElapsedGameTurns() < 20))
			{
				return false;
			}
		}

		if (GC.getGameINLINE().isOption(GAMEOPTION_ONE_CITY_CHALLENGE) && isHuman())
		{
			if (GC.getUnitInfo(eUnit).isFound())
			{
				return false;
			}
		}
	}

	if (GC.getGoodyInfo(eGoody).getBarbarianUnitClass() != NO_UNITCLASS)
	{
		if (GC.getGameINLINE().isOption(GAMEOPTION_NO_BARBARIANS))
		{
			return false;
		}
		if (getNumCities() == 0)
		{
			return false;
		}

		if (getNumCities() == 1)
		{
			pCity = GC.getMapINLINE().findCity(pPlot->getX_INLINE(), pPlot->getY_INLINE(), NO_PLAYER, getTeam());

			if (pCity != NULL)
			{
				if (plotDistance(pPlot->getX_INLINE(), pPlot->getY_INLINE(), pCity->getX_INLINE(), pCity->getY_INLINE()) <= (8 - getNumCities()))
				{
					return false;
				}
			}
		}
	}

	return true;
}


void CvPlayer::receiveGoody(CvPlot* pPlot, GoodyTypes eGoody, CvUnit* pUnit)
{
	CvPlot* pLoopPlot;
	CvPlot* pBestPlot = NULL;
	CvWString szBuffer;
	CvWString szTempBuffer;
	TechTypes eBestTech;
	UnitTypes eUnit;
	int iGold;
	int iOffset;
	int iRange;
	int iBarbCount;
	int iValue;
	int iBestValue;
	int iPass;
	int iDX, iDY;
	int iI;

//FfH: Added by Kael 08/31/2008
    CvUnit* pNewUnit;
//FfH: End Add

//	FAssertMsg(canReceiveGoody(pPlot, eGoody, pUnit), "Instance is expected to be able to recieve goody"); //graveyards can also give goodys Sephi

	szBuffer = GC.getGoodyInfo(eGoody).getDescription();

	iGold = GC.getGoodyInfo(eGoody).getGold() + GC.getGameINLINE().getSorenRandNum(GC.getGoodyInfo(eGoody).getGoldRand1(), "Goody Gold 1") + GC.getGameINLINE().getSorenRandNum(GC.getGoodyInfo(eGoody).getGoldRand2(), "Goody Gold 2");
	iGold  = (iGold * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getGrowthPercent()) / 100;

	if (iGold != 0)
	{
		changeGold(iGold);

		szBuffer += gDLL->getText("TXT_KEY_MISC_RECEIVED_GOLD", iGold);
	}

	if (!szBuffer.empty())
	{
		gDLL->getInterfaceIFace()->addMessage(getID(), true, GC.getEVENT_MESSAGE_TIME(), szBuffer, GC.getGoodyInfo(eGoody).getSound(), MESSAGE_TYPE_MINOR_EVENT, ARTFILEMGR.getImprovementArtInfo("ART_DEF_IMPROVEMENT_GOODY_HUT")->getButton(), (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), pPlot->getX_INLINE(), pPlot->getY_INLINE());
	}

	iRange = GC.getGoodyInfo(eGoody).getMapRange();

	if (iRange > 0)
	{
		iOffset = GC.getGoodyInfo(eGoody).getMapOffset();

		if (iOffset > 0)
		{
			iBestValue = 0;
			pBestPlot = NULL;

			for (iDX = -(iOffset); iDX <= iOffset; iDX++)
			{
				for (iDY = -(iOffset); iDY <= iOffset; iDY++)
				{
					pLoopPlot = plotXY(pPlot->getX_INLINE(), pPlot->getY_INLINE(), iDX, iDY);

					if (pLoopPlot != NULL)
					{
						if (!(pLoopPlot->isRevealed(getTeam(), false)))
						{
							iValue = (1 + GC.getGameINLINE().getSorenRandNum(10000, "Goody Map"));

							iValue *= plotDistance(pPlot->getX_INLINE(), pPlot->getY_INLINE(), pLoopPlot->getX_INLINE(), pLoopPlot->getY_INLINE());

							if (iValue > iBestValue)
							{
								iBestValue = iValue;
								pBestPlot = pLoopPlot;
							}
						}
					}
				}
			}
		}

		if (pBestPlot == NULL)
		{
			pBestPlot = pPlot;
		}

		for (iDX = -(iRange); iDX <= iRange; iDX++)
		{
			for (iDY = -(iRange); iDY <= iRange; iDY++)
			{
				pLoopPlot = plotXY(pBestPlot->getX_INLINE(), pBestPlot->getY_INLINE(), iDX, iDY);

				if (pLoopPlot != NULL)
				{
					if (plotDistance(pBestPlot->getX_INLINE(), pBestPlot->getY_INLINE(), pLoopPlot->getX_INLINE(), pLoopPlot->getY_INLINE()) <= iRange)
					{
						if (GC.getGameINLINE().getSorenRandNum(100, "Goody Map") < GC.getGoodyInfo(eGoody).getMapProb())
						{
							pLoopPlot->setRevealed(getTeam(), true, false, NO_TEAM, true);
						}
					}
				}
			}
		}
	}

	if (pUnit != NULL)
	{

//FfH: Modified by Kael 08/31/2008
//        pUnit->changeExperience(GC.getGoodyInfo(eGoody).getExperience());
        if (GC.getGoodyInfo(eGoody).getUnitClassType() == NO_UNITCLASS && GC.getGoodyInfo(eGoody).getBarbarianUnitClass() == NO_UNITCLASS)
        {
            pUnit->changeExperience(GC.getGoodyInfo(eGoody).getExperience());
        }
//FfH: End Modify

	}

	if (pUnit != NULL)
	{
		pUnit->changeDamage(-(GC.getGoodyInfo(eGoody).getHealing()));
	}

	if (GC.getGoodyInfo(eGoody).isTech())
	{
		iBestValue = 0;
		eBestTech = NO_TECH;

		for (iI = 0; iI < GC.getNumTechInfos(); iI++)
		{
			if (GC.getTechInfo((TechTypes) iI).isGoodyTech())
			{
				if (canResearch((TechTypes)iI))
				{
					if(GET_TEAM(getTeam()).getResearchCost((TechTypes)iI) < 1000) {

						iValue = (1 + GC.getGameINLINE().getSorenRandNum(10000, "Goody Tech"));

						if (iValue > iBestValue)
						{
							iBestValue = iValue;
							eBestTech = ((TechTypes)iI);
						}
					}
				}
			}
		}

		FAssertMsg(eBestTech != NO_TECH, "BestTech is not assigned a valid value");

		GET_TEAM(getTeam()).setHasTech(eBestTech, true, getID(), true, true);
		GET_TEAM(getTeam()).setNoTradeTech(eBestTech, true);
	}

	if (GC.getGoodyInfo(eGoody).getUnitClassType() != NO_UNITCLASS)
	{
		eUnit = (UnitTypes)GC.getCivilizationInfo(getCivilizationType()).getCivilizationUnits(GC.getGoodyInfo(eGoody).getUnitClassType());

		if (eUnit == NO_UNIT)
		{
			eUnit = ((UnitTypes)(GC.getUnitClassInfo((UnitClassTypes)GC.getGoodyInfo(eGoody).getUnitClassType()).getDefaultUnitIndex()));
		}
		if (eUnit != NO_UNIT)
		{
			pNewUnit = initUnit(eUnit, pPlot->getX_INLINE(), pPlot->getY_INLINE());
			pNewUnit->changeExperience(GC.getGoodyInfo(eGoody).getExperience());
		}
	}

	if (GC.getGoodyInfo(eGoody).getBarbarianUnitClass() != NO_UNITCLASS)
	{
		iBarbCount = 0;
		eUnit = (UnitTypes)GC.getCivilizationInfo(GET_PLAYER(BARBARIAN_PLAYER).getCivilizationType()).getCivilizationUnits(GC.getGoodyInfo(eGoody).getBarbarianUnitClass());

		if (eUnit != NO_UNIT)
		{
			for (iPass = 0; iPass < 2; iPass++)
			{
				if (iBarbCount < GC.getGoodyInfo(eGoody).getMinBarbarians())
				{
					for (iI = 0; iI < NUM_DIRECTION_TYPES; iI++)
					{
						pLoopPlot = plotDirection(pPlot->getX_INLINE(), pPlot->getY_INLINE(), ((DirectionTypes)iI));

						if (pLoopPlot != NULL)
						{
							if (pLoopPlot->getArea() == pPlot->getArea())
							{
								if (!(pLoopPlot->isImpassable()))
								{
									if (pLoopPlot->getNumUnits() == 0)
									{
										if ((iPass > 0) || (GC.getGameINLINE().getSorenRandNum(100, "Goody Barbs") < GC.getGoodyInfo(eGoody).getBarbarianUnitProb()))
										{

//FfH: Modified by Kael 08/31/2008
//											GET_PLAYER(BARBARIAN_PLAYER).initUnit(eUnit, pLoopPlot->getX_INLINE(), pLoopPlot->getY_INLINE(), ((pLoopPlot->isWater()) ? UNITAI_ATTACK_SEA : UNITAI_ATTACK));
											pNewUnit = GET_PLAYER(BARBARIAN_PLAYER).initUnit(eUnit, pLoopPlot->getX_INLINE(), pLoopPlot->getY_INLINE(), ((pLoopPlot->isWater()) ? UNITAI_ATTACK_SEA : UNITAI_ATTACK));

											iBarbCount++;

											if ((iPass > 0) && (iBarbCount == GC.getGoodyInfo(eGoody).getMinBarbarians()))
											{
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}


void CvPlayer::doGoody(CvPlot* pPlot, CvUnit* pUnit)
{
	CyPlot kGoodyPlot(pPlot);
	CyUnit kGoodyUnit(pUnit);
	CyArgsList argsList;
	argsList.add(getID());		// pass in this players ID
	argsList.add(gDLL->getPythonIFace()->makePythonObject(&kGoodyPlot));
	argsList.add(gDLL->getPythonIFace()->makePythonObject(&kGoodyUnit));

	long result=0;
	bool ok = gDLL->getPythonIFace()->callFunction(PYGameModule, "doGoody", argsList.makeFunctionArgs(), &result);
	if (ok && result)
	{
		return;
	}

	FAssertMsg(pPlot->isGoody(), "pPlot->isGoody is expected to be true");

	pPlot->removeGoody();
	if (!isBarbarian())
	{
		for (int iI = 0; iI < GC.getDefineINT("NUM_DO_GOODY_ATTEMPTS"); iI++)
		{
			if (GC.getHandicapInfo(getHandicapType()).getNumGoodies() > 0)
			{
				GoodyTypes eGoody = (GoodyTypes)GC.getHandicapInfo(getHandicapType()).getGoodies(GC.getGameINLINE().getSorenRandNum(GC.getHandicapInfo(getHandicapType()).getNumGoodies(), "Goodies"));

				FAssert(eGoody >= 0);
				FAssert(eGoody < GC.getNumGoodyInfos());

				if (canReceiveGoody(pPlot, eGoody, pUnit))
				{
					receiveGoody(pPlot, eGoody, pUnit);

					// Python Event
					CvEventReporter::getInstance().goodyReceived(getID(), pPlot, pUnit, eGoody);
					break;
				}
			}
		}
	}
}


bool CvPlayer::canFound(int iX, int iY, bool bTestVisible) const
{
	CvPlot* pPlot;
	CvPlot* pLoopPlot;
	bool bValid;
	int iRange;
	int iDX, iDY;
/*************************************************************************************************/
/**	ADDON (MultiBarb)  merged Sephi		12/23/08									Xienwolf	**/
/**	blocks some new barb civs from founding cities  											**/
/**							Adds extra Barbarian Civilizations									**/
/*************************************************************************************************/
    if (getID() == WILDMANA_PLAYER || getID() == ANIMAL_PLAYER || getID() == PIRATES_PLAYER ||
        getID() == WHITEHAND_PLAYER || getID() == DEVIL_PLAYER || getID() == MERCENARIES_PLAYER
    )
    {
        return false;
    }
/*************************************************************************************************/
/**	MultiBarb								END													**/
/*************************************************************************************************/

	pPlot = GC.getMapINLINE().plotINLINE(iX, iY);

	long lResult=0;
	if(GC.getUSE_CANNOT_FOUND_CITY_CALLBACK())
	{
		CyArgsList argsList;
		argsList.add((int)getID());
		argsList.add(iX);
		argsList.add(iY);
		gDLL->getPythonIFace()->callFunction(PYGameModule, "cannotFoundCity", argsList.makeFunctionArgs(), &lResult);

		if (lResult == 1)
		{
			return false;
		}
	}

	if (GC.getGameINLINE().isFinalInitialized())
	{
		if (GC.getGameINLINE().isOption(GAMEOPTION_ONE_CITY_CHALLENGE) && isHuman())
		{
			if (getNumCities() > 0)
			{
				return false;
			}
		}
	}

	if (pPlot->isImpassable())
	{
		return false;
	}

	if (pPlot->getFeatureType() != NO_FEATURE)
	{
		if (GC.getFeatureInfo(pPlot->getFeatureType()).isNoCity())
		{
			return false;
		}
	}

	if (pPlot->isOwned() && (pPlot->getOwnerINLINE() != getID()))
	{
		return false;
	}

	bValid = false;

	if (!bValid)
	{
		if (GC.getTerrainInfo(pPlot->getTerrainType()).isFound())
		{
			bValid = true;
		}
	}

	if (!bValid)
	{
		if (GC.getTerrainInfo(pPlot->getTerrainType()).isFoundCoast())
		{
			if (pPlot->isCoastalLand())
			{
				bValid = true;
			}
		}
	}

	if (!bValid)
	{
		if (GC.getTerrainInfo(pPlot->getTerrainType()).isFoundFreshWater())
		{
			if (pPlot->isFreshWater())
			{
				bValid = true;
			}
		}
	}

/************************************************************************************************/
/* UNOFFICIAL_PATCH                       10/04/09                    EmperorFool & jdog5000    */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
	// EF: canFoundCitiesOnWater callback handling was incorrect and ignored isWater() if it returned true
	if (!bValid && pPlot->isWater())
	{
		if(GC.getUSE_CAN_FOUND_CITIES_ON_WATER_CALLBACK())
		{
			CyArgsList argsList2;
			argsList2.add(iX);
			argsList2.add(iY);
			lResult=0;
			gDLL->getPythonIFace()->callFunction(PYGameModule, "canFoundCitiesOnWater", argsList2.makeFunctionArgs(), &lResult);

			if (lResult == 1)
			{
				bValid = true;
			}
		}
	}
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/

	if (!bValid)
	{
		return false;
	}

	if (!bTestVisible)
	{
		iRange = GC.getMIN_CITY_RANGE();

		//added Sephi - Force AI to spread Cities more
		if(!isHuman())
			++iRange;
		//end added

		for (iDX = -(iRange); iDX <= iRange; iDX++)
		{
			for (iDY = -(iRange); iDY <= iRange; iDY++)
			{
				pLoopPlot	= plotXY(pPlot->getX_INLINE(), pPlot->getY_INLINE(), iDX, iDY);

				if (pLoopPlot != NULL)
				{
					if (pLoopPlot->isCity())
					{
						//added Sephi small Cities have smaller block radius
						if(pLoopPlot->getPlotCity()->getPlotRadius()>1 || (abs(iDX)<iRange && abs(iDY)<iRange))
						{
							if (pLoopPlot->area() == pPlot->area())
							{
								return false;
							}
						}
					}
				}
			}
		}
	}

//FfH: Added by Kael 09/28/2007
	if (pPlot->getImprovementType() != NO_IMPROVEMENT)
	{
	    if (GC.getImprovementInfo((ImprovementTypes)pPlot->getImprovementType()).isPermanent())
	    {
	        return false;
	    }
	}

//FfH: End Add
/*************************************************************************************************/
/**	WILDMANA (stop AI to try to settle on Mana nodes) Sephi                    					**/
/**																								**/
/**						                                            							**/
/*************************************************************************************************/

	if (pPlot->getBonusType() != NO_BONUS)
	{
	    if (GC.getBonusInfo((BonusTypes)pPlot->getBonusType()).getBonusClassType()==GC.getDefineINT("BONUSCLASS_MANA"))
	    {
	        return false;
	    }

        if (GC.getBonusInfo((BonusTypes)pPlot->getBonusType()).getBonusClassType()==GC.getDefineINT("BONUSCLASS_MANA_RAW"))
	    {
	        return false;
	    }
	}
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
	return true;
}


void CvPlayer::found(int iX, int iY)
{
	CvCity* pCity;
	BuildingTypes eLoopBuilding;
	UnitTypes eDefenderUnit;
	int iI;

	if (!canFound(iX, iY))
	{
		return;
	}

	pCity = initCity(iX, iY, true, true);
	FAssertMsg(pCity != NULL, "City is not assigned a valid value");

	if (isBarbarian())
	{
		eDefenderUnit = pCity->AI_bestUnitAI(UNITAI_CITY_DEFENSE);

		if (eDefenderUnit == NO_UNIT)
		{
			eDefenderUnit = pCity->AI_bestUnitAI(UNITAI_ATTACK);
		}

		if (eDefenderUnit != NO_UNIT)
		{
			for (iI = 0; iI < GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getBarbarianInitialDefenders(); iI++)
			{
				initUnit(eDefenderUnit, iX, iY, UNITAI_CITY_DEFENSE);
			}
		}
	}

	for (iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		eLoopBuilding = ((BuildingTypes)(GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(iI)));

		if (eLoopBuilding != NO_BUILDING)
		{
			if (GC.getBuildingInfo(eLoopBuilding).getFreeStartEra() != NO_ERA)
			{
				if (GC.getGameINLINE().getStartEra() >= GC.getBuildingInfo(eLoopBuilding).getFreeStartEra())
				{
					if (pCity->canConstruct(eLoopBuilding))
					{
						pCity->setNumRealBuilding(eLoopBuilding, 1);
					}
				}
			}
		}
	}

	if (getAdvancedStartPoints() >= 0)
	{
		// Free border expansion for Creative
		bool bCreative = false;
		for (iI = 0; iI < GC.getNumTraitInfos(); ++iI)
		{
			if (hasTrait((TraitTypes)iI))
			{
				if (GC.getTraitInfo((TraitTypes)iI).getCommerceChange(COMMERCE_CULTURE) > 0)
				{
					bCreative = true;
					break;
				}

			}
		}

		if (bCreative)
		{
			for (iI = 0; iI < GC.getNumCultureLevelInfos(); ++iI)
			{
				int iCulture = GC.getGameINLINE().getCultureThreshold((CultureLevelTypes)iI);
				if (iCulture > 0)
				{
					pCity->setCulture(getID(), iCulture, true, true);
					break;
				}
			}
		}
	}

	// Sephi default value for avoid angry Citizens	
	if(GC.getInfoTypeForString("EMPHASIZE_AVOID_ANGRY_CITIZENS")!=NO_EMPHASIZE)
		pCity->AI_setEmphasize((EmphasizeTypes)GC.getInfoTypeForString("EMPHASIZE_AVOID_ANGRY_CITIZENS"),true);

	if (isHuman() && getAdvancedStartPoints() < 0)
	{
		pCity->chooseProduction();
	}
	else
	{
		pCity->doFoundMessage();
	}

	CvEventReporter::getInstance().cityBuilt(pCity);
}


bool CvPlayer::canTrain(UnitTypes eUnit, bool bContinue, bool bTestVisible, bool bIgnoreCost) const
{
	PROFILE_FUNC();

	UnitClassTypes eUnitClass;
	int iI;

	eUnitClass = ((UnitClassTypes)(GC.getUnitInfo(eUnit).getUnitClassType()));

//	FAssert(GC.getCivilizationInfo(getCivilizationType()).getCivilizationUnits(eUnitClass) == eUnit); //modified Sephi

//FfH: Modified by Kael 05/09/2008
//	if (GC.getCivilizationInfo(getCivilizationType()).getCivilizationUnits(eUnitClass) != eUnit)
//	{
//		return false;
//	}
//FfH: End Modify

	if (!bIgnoreCost)
	{
		if (GC.getUnitInfo(eUnit).getProductionCost() == -1)
		{
			return false;
		}
	}

	if (GC.getGameINLINE().isOption(GAMEOPTION_ONE_CITY_CHALLENGE) && isHuman())
	{
		if (GC.getUnitInfo(eUnit).isFound())
		{
			return false;
		}
	}

	if (!(GET_TEAM(getTeam()).isHasTech((TechTypes)(GC.getUnitInfo(eUnit).getPrereqAndTech()))))
	{
		return false;
	}

	for (iI = 0; iI < GC.getNUM_UNIT_AND_TECH_PREREQS(); iI++)
	{
		if (GC.getUnitInfo(eUnit).getPrereqAndTechs(iI) != NO_TECH)
		{
			if (!(GET_TEAM(getTeam()).isHasTech((TechTypes)(GC.getUnitInfo(eUnit).getPrereqAndTechs(iI)))))
			{
				return false;
			}
		}
	}

	if (GC.getUnitInfo(eUnit).getStateReligion() != NO_RELIGION)
	{
		if (getStateReligion() != GC.getUnitInfo(eUnit).getStateReligion())
		{
			return false;
		}
	}

	if (GC.getGameINLINE().isUnitClassMaxedOut(eUnitClass))
	{
		return false;
	}

	if (GET_TEAM(getTeam()).isUnitClassMaxedOut(eUnitClass))
	{
		return false;
	}

	if (isUnitClassMaxedOut(eUnitClass))
	{
		return false;
	}

	if (!bTestVisible)
	{
		if (GC.getGameINLINE().isUnitClassMaxedOut(eUnitClass, (GET_TEAM(getTeam()).getUnitClassMaking(eUnitClass) + ((bContinue) ? -1 : 0))))
		{
			return false;
		}

		if (GET_TEAM(getTeam()).isUnitClassMaxedOut(eUnitClass, (GET_TEAM(getTeam()).getUnitClassMaking(eUnitClass) + ((bContinue) ? -1 : 0))))
		{
			return false;
		}

		if (isUnitClassMaxedOut(eUnitClass, (getUnitClassMaking(eUnitClass) + ((bContinue) ? -1 : 0))))
		{
			return false;
		}

		if (GC.getGameINLINE().isNoNukes() || !GC.getGameINLINE().isNukesValid())
		{
			if (GC.getUnitInfo(eUnit).getNukeRange() != -1)
			{
				return false;
			}
		}

		if (GC.getUnitInfo(eUnit).getSpecialUnitType() != NO_SPECIALUNIT)
		{
			if (!(GC.getGameINLINE().isSpecialUnitValid((SpecialUnitTypes)(GC.getUnitInfo(eUnit).getSpecialUnitType()))))
			{
				return false;
			}
		}

		if(GC.getUnitInfo(eUnit).getFaithUpkeep()>0)
		{
			if(getFaithIncome()<GC.getUnitInfo(eUnit).getFaithUpkeep())
			{
				return false;
			}
		}
	}

//FfH Units: Added by Kael 08/04/2007
	if (GC.getUnitInfo(eUnit).getPrereqCivic() != NO_CIVIC)
	{
	    bool bValid = false;
        for (iI = 0; iI < GC.getDefineINT("MAX_CIVIC_OPTIONS"); iI++)
        {
            if (getCivics((CivicOptionTypes)iI) == GC.getUnitInfo(eUnit).getPrereqCivic())
            {
                bValid = true;
            }
        }
        if (bValid == false)
        {
            return false;
        }
	}
	if (GC.getUnitInfo(eUnit).getPrereqGlobalCounter() != 0)
	{
        if (GC.getGameINLINE().getGlobalCounter() < GC.getUnitInfo(eUnit).getPrereqGlobalCounter())
        {
            return false;
        }
	}
	if (GC.getUnitInfo(eUnit).getPrereqAlignment() != NO_ALIGNMENT)
	{
        if (getAlignment() != GC.getUnitInfo(eUnit).getPrereqAlignment())
        {
            return false;
        }
	}
//FfH: End Add

	if(isCityState())
	{
		if(isWorldUnitClass((UnitClassTypes)GC.getUnitInfo(eUnit).getUnitClassType()))
		{
			return false;
		}
	}

/*************************************************************************************************/
/**	ADDON (Houses of Erebus) Sephi			                                 					**/
/*************************************************************************************************/
    if (GC.getUnitInfo(eUnit).getCorporationSupportNeeded()!=NO_CORPORATION)
    {
        if(!isHasCorporationSupport((CorporationTypes)GC.getUnitInfo(eUnit).getCorporationSupportNeeded()))
        {
            return false;
        }
    }
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

	return true;
}


bool CvPlayer::canConstruct(BuildingTypes eBuilding, bool bContinue, bool bTestVisible, bool bIgnoreCost) const
{
	BuildingClassTypes eBuildingClass;
	int iI;
	CvTeamAI& currentTeam = GET_TEAM(getTeam());

	eBuildingClass = ((BuildingClassTypes)(GC.getBuildingInfo(eBuilding).getBuildingClassType()));

//	FAssert(GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(eBuildingClass) == eBuilding); //modified Sephi

//FfH: Modified by Kael 07/05/2008
//	if (GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(eBuildingClass) != eBuilding)
//	{
//		return false;
//	}
//FfH: End Modify

	if (!bIgnoreCost)
	{
		if (GC.getBuildingInfo(eBuilding).getProductionCost() == -1)
		{
			return false;
		}
	}

	if (!(currentTeam.isHasTech((TechTypes)(GC.getBuildingInfo(eBuilding).getPrereqAndTech()))))
	{
		return false;
	}

	for (iI = 0; iI < GC.getNUM_BUILDING_AND_TECH_PREREQS(); iI++)
	{
		if (GC.getBuildingInfo(eBuilding).getPrereqAndTechs(iI) != NO_TECH)
		{
			if (!(currentTeam.isHasTech((TechTypes)(GC.getBuildingInfo(eBuilding).getPrereqAndTechs(iI)))))
			{
				return false;
			}
		}
	}

	if (currentTeam.isObsoleteBuilding(eBuilding))
	{
		return false;
	}

	if (GC.getBuildingInfo(eBuilding).getSpecialBuildingType() != NO_SPECIALBUILDING)
	{
		if (!(currentTeam.isHasTech((TechTypes)(GC.getSpecialBuildingInfo((SpecialBuildingTypes) GC.getBuildingInfo(eBuilding).getSpecialBuildingType()).getTechPrereq()))))
		{
			return false;
		}
	}

	if (GC.getBuildingInfo(eBuilding).getStateReligion() != NO_RELIGION)
	{
		if (getStateReligion() != GC.getBuildingInfo(eBuilding).getStateReligion())
		{
			return false;
		}
	}

	if (GC.getGameINLINE().countCivTeamsEverAlive() < GC.getBuildingInfo(eBuilding).getNumTeamsPrereq())
	{
		return false;
	}

	if (GC.getBuildingInfo(eBuilding).getVictoryPrereq() != NO_VICTORY)
	{
		if (!(GC.getGameINLINE().isVictoryValid((VictoryTypes)(GC.getBuildingInfo(eBuilding).getVictoryPrereq()))))
		{
			return false;
		}

		if (isMinorCiv())
		{
			return false;
		}

		if (currentTeam.getVictoryCountdown((VictoryTypes)GC.getBuildingInfo(eBuilding).getVictoryPrereq()) >= 0)
		{
			return false;
		}
	}

	if (GC.getBuildingInfo(eBuilding).getMaxStartEra() != NO_ERA)
	{
		if (GC.getGameINLINE().getStartEra() > GC.getBuildingInfo(eBuilding).getMaxStartEra())
		{
			return false;
		}
	}

	if (GC.getGameINLINE().isBuildingClassMaxedOut(eBuildingClass))
	{
		return false;
	}

	if (currentTeam.isBuildingClassMaxedOut(eBuildingClass))
	{
		return false;
	}

	if (isBuildingClassMaxedOut(eBuildingClass))
	{
		return false;
	}

	CvCivilizationInfo &civilizationInfo = GC.getCivilizationInfo(getCivilizationType());
	int numBuildingClassInfos = GC.getNumBuildingClassInfos();
	for (iI = 0; iI < numBuildingClassInfos; iI++)
	{
		BuildingTypes ePrereqBuilding = (BuildingTypes)civilizationInfo.getCivilizationBuildings(iI);

		if (NO_BUILDING != ePrereqBuilding && currentTeam.isObsoleteBuilding(ePrereqBuilding))
		{
			if (getBuildingClassCount((BuildingClassTypes)iI) < getBuildingClassPrereqBuilding(eBuilding, (BuildingClassTypes)iI, 0))
			{
				return false;
			}
		}
	}

	if(isCityState())
	{
		if(GC.getBuildingClassInfo(eBuildingClass).getMaxGlobalInstances()==1)
		{
			return false;
		}
	}

	if (!bTestVisible)
	{
		if (GC.getGameINLINE().isBuildingClassMaxedOut(eBuildingClass, (currentTeam.getBuildingClassMaking(eBuildingClass) + ((bContinue) ? -1 : 0))))
		{
			return false;
		}

		if (currentTeam.isBuildingClassMaxedOut(eBuildingClass, (currentTeam.getBuildingClassMaking(eBuildingClass) + ((bContinue) ? -1 : 0))))
		{
			return false;
		}

		if (isBuildingClassMaxedOut(eBuildingClass, (getBuildingClassMaking(eBuildingClass) + ((bContinue) ? -1 : 0))))
		{
			return false;
		}

		if (GC.getGameINLINE().isNoNukes())
		{
			if (GC.getBuildingInfo(eBuilding).isAllowsNukes())
			{
				for (iI = 0; iI < GC.getNumUnitInfos(); iI++)
				{
					if (GC.getUnitInfo((UnitTypes)iI).getNukeRange() != -1)
					{
						return false;
					}
				}
			}
		}

		if (GC.getBuildingInfo(eBuilding).getSpecialBuildingType() != NO_SPECIALBUILDING)
		{
			if (!(GC.getGameINLINE().isSpecialBuildingValid((SpecialBuildingTypes)(GC.getBuildingInfo(eBuilding).getSpecialBuildingType()))))
			{
				return false;
			}
		}
/*************************************************************************************************/
/**	Xienwolf Tweak							01/04/09											**/
/**																								**/
/**						Fixes Kuriotates on small maps for certain buildings					**/
/*************************************************************************************************/
/**								---- Start Original Code ----									**
		if (getNumCities() < GC.getBuildingInfo(eBuilding).getNumCitiesPrereq())
/**								----  End Original Code  ----									**/
		if (getNumCities() < GC.getBuildingInfo(eBuilding).getNumCitiesPrereq() || (getMaxCities() != -1 && (getMaxCities() < GC.getBuildingInfo(eBuilding).getNumCitiesPrereq()) && (getNumCities() != getMaxCities())))
/*************************************************************************************************/
/**	Tweak									END													**/
/*************************************************************************************************/
		{
			return false;
		}

		if (getHighestUnitLevel() < GC.getBuildingInfo(eBuilding).getUnitLevelPrereq())
		{
			return false;
		}

		for (iI = 0; iI < numBuildingClassInfos; iI++)
		{
			if (getBuildingClassCount((BuildingClassTypes)iI) < getBuildingClassPrereqBuilding(eBuilding, ((BuildingClassTypes)iI), ((bContinue) ? 0 : getBuildingClassMaking(eBuildingClass))))
			{
				return false;
			}
		}
	}

/*************************************************************************************************/
/**	ADDON (New Tags) Sephi			                                 					**/
/*************************************************************************************************/
    if (GC.getBuildingInfo(eBuilding).getCorporationSupportNeeded()!=NO_CORPORATION)
    {
        if(!isHasCorporationSupport((CorporationTypes)GC.getBuildingInfo(eBuilding).getCorporationSupportNeeded()))
        {
            return false;
        }
    }

	CivicTypes eCivic=(CivicTypes)GC.getBuildingInfo(eBuilding).getPrereqCivic();
	if(eCivic!=NO_CIVIC)
	{
		if(getCivics((CivicOptionTypes)GC.getCivicInfo(eCivic).getCivicOptionType())!=eCivic)
		{
			return false;
		}
	}

	if(GC.getBuildingInfo(eBuilding).getLastTurnPossibleBuild() > 0)
	{
		int iMaxTurns = GC.getBuildingInfo(eBuilding).getLastTurnPossibleBuild();

		iMaxTurns *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getBuildPercent();
		iMaxTurns /= 100;

		if(GC.getGame().getElapsedGameTurns() > iMaxTurns)
		{
			return false;
		}
	}

	if(eBuildingClass == GC.getBUILDINGCLASS_PALACE()) {
		if(eBuilding != GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(eBuildingClass)) {
			return false;
		}
	}

/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

	return true;
}


bool CvPlayer::canCreate(ProjectTypes eProject, bool bContinue, bool bTestVisible) const
{
	int iI;

	if (isBarbarian())
	{
		return false;
	}

	if (GC.getProjectInfo(eProject).getProductionCost() == -1)
	{
		return false;
	}

	if (!(GET_TEAM(getTeam()).isHasTech((TechTypes)(GC.getProjectInfo(eProject).getTechPrereq()))))
	{
		return false;
	}

	if (GC.getProjectInfo(eProject).getVictoryPrereq() != NO_VICTORY)
	{
		if (!(GC.getGameINLINE().isVictoryValid((VictoryTypes)(GC.getProjectInfo(eProject).getVictoryPrereq()))))
		{
			return false;
		}

		if (isMinorCiv())
		{
			return false;
		}

		if (GET_TEAM(getTeam()).getVictoryCountdown((VictoryTypes)GC.getProjectInfo(eProject).getVictoryPrereq()) >= 0)
		{
			return false;
		}
	}

	if (GC.getGameINLINE().isProjectMaxedOut(eProject))
	{
		return false;
	}

	if (GET_TEAM(getTeam()).isProjectMaxedOut(eProject))
	{
		return false;
	}

	if (!bTestVisible)
	{
/*************************************************************************************************/
/**	ADDON (New tag Definitions) Sephi                                          					**/
/*************************************************************************************************/
        if (GC.getProjectInfo(eProject).getManaCost() != 0)
        {
            if (getMana()<GC.getProjectInfo(eProject).getManaCost())
            {
                return false;
            }
        }

        if (GC.getProjectInfo(eProject).getFaithCost() != 0)
        {
            if (getFaith()<GC.getProjectInfo(eProject).getFaithCost())
            {
                return false;
            }
        }
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

		if (GC.getGameINLINE().isProjectMaxedOut(eProject, (GET_TEAM(getTeam()).getProjectMaking(eProject) + ((bContinue) ? -1 : 0))))
		{
			return false;
		}

		if (GET_TEAM(getTeam()).isProjectMaxedOut(eProject, (GET_TEAM(getTeam()).getProjectMaking(eProject) + ((bContinue) ? -1 : 0))))
		{
			return false;
		}

		if (GC.getGameINLINE().isNoNukes())
		{
			if (GC.getProjectInfo(eProject).isAllowsNukes())
			{
				for (iI = 0; iI < GC.getNumUnitInfos(); iI++)
				{
					if (GC.getUnitInfo((UnitTypes)iI).getNukeRange() != -1)
					{
						return false;
					}
				}
			}
		}

		if (GC.getProjectInfo(eProject).getAnyoneProjectPrereq() != NO_PROJECT)
		{
			if (GC.getGameINLINE().getProjectCreatedCount((ProjectTypes)(GC.getProjectInfo(eProject).getAnyoneProjectPrereq())) == 0)
			{
				return false;
			}
		}

		if (GC.getProjectInfo(eProject).getProjectPrereq() != NO_PROJECT)
		{
			if (GET_TEAM(getTeam()).getProjectCount(eProject)==0)
			{
				return false;
			}
		}

		for (iI = 0; iI < GC.getNumProjectInfos(); iI++)
		{
			if (GET_TEAM(getTeam()).getProjectCount((ProjectTypes)iI) < GC.getProjectInfo(eProject).getProjectsNeeded(iI))
			{
				return false;
			}
		}
	}

	if(GC.getProjectInfo(eProject).getLastTurnPossibleBuild() > 0)
	{
		int iMaxTurns = GC.getProjectInfo(eProject).getLastTurnPossibleBuild();

		iMaxTurns *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getBuildPercent();
		iMaxTurns /= 100;

		if(GC.getGame().getElapsedGameTurns() > iMaxTurns)
		{
			return false;
		}
	}

	return true;
}


bool CvPlayer::canMaintain(ProcessTypes eProcess, bool bContinue) const
{
	if (GC.getProcessInfo(eProcess).getTechPrereq()!=NO_TECH && !(GET_TEAM(getTeam()).isHasTech((TechTypes)(GC.getProcessInfo(eProcess).getTechPrereq()))))
	{
		return false;
	}

	return true;
}


bool CvPlayer::isProductionMaxedUnitClass(UnitClassTypes eUnitClass) const
{
	if (eUnitClass == NO_UNITCLASS)
	{
		return false;
	}

	if (GC.getGameINLINE().isUnitClassMaxedOut(eUnitClass))
	{
		return true;
	}

	if (GET_TEAM(getTeam()).isUnitClassMaxedOut(eUnitClass))
	{
		return true;
	}

	if (isUnitClassMaxedOut(eUnitClass))
	{
		return true;
	}

	return false;
}


bool CvPlayer::isProductionMaxedBuildingClass(BuildingClassTypes eBuildingClass, bool bAcquireCity) const
{
	if (eBuildingClass == NO_BUILDINGCLASS)
	{
		return false;
	}

	if (!bAcquireCity)
	{
		if (GC.getGameINLINE().isBuildingClassMaxedOut(eBuildingClass))
		{
			return true;
		}
	}

	if (GET_TEAM(getTeam()).isBuildingClassMaxedOut(eBuildingClass))
	{
		return true;
	}

	if (isBuildingClassMaxedOut(eBuildingClass, ((bAcquireCity) ? GC.getBuildingClassInfo(eBuildingClass).getExtraPlayerInstances() : 0)))
	{
		return true;
	}

	return false;
}


bool CvPlayer::isProductionMaxedProject(ProjectTypes eProject) const
{
	if (eProject == NO_PROJECT)
	{
		return false;
	}

	if (GC.getGameINLINE().isProjectMaxedOut(eProject))
	{
		return true;
	}

	if (GET_TEAM(getTeam()).isProjectMaxedOut(eProject))
	{
		return true;
	}

	return false;
}


int CvPlayer::getProductionNeeded(UnitTypes eUnit) const
{
	UnitClassTypes eUnitClass = (UnitClassTypes)GC.getUnitInfo(eUnit).getUnitClassType();
	FAssert(NO_UNITCLASS != eUnitClass);

	int iProductionNeeded = GC.getUnitInfo(eUnit).getProductionCost();

	if(GC.getUnitInfo(eUnit).isScaleWithTech()) {
		int iStrength = std::max(GC.getUnitInfo(eUnit).getCombat(),
								GC.getUnitInfo(eUnit).getCombatDefense());
		if(iStrength > 0) {
			iProductionNeeded *= 100 + 100 * getIncreaseStrengthOfRegularUnits() / iStrength;
			iProductionNeeded /= 100;
		}
	}

	iProductionNeeded *= 100 + getUnitClassCount(eUnitClass) * GC.getUnitClassInfo(eUnitClass).getInstanceCostModifier();
	iProductionNeeded /= 100;

	iProductionNeeded *= GC.getDefineINT("UNIT_PRODUCTION_PERCENT");
	iProductionNeeded /= 100;

	iProductionNeeded *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getTrainPercent();
	iProductionNeeded /= 100;

	iProductionNeeded *= GC.getEraInfo(GC.getGameINLINE().getStartEra()).getTrainPercent();
	iProductionNeeded /= 100;

	if (!isHuman() && !isBarbarian())
	{
		if (isWorldUnitClass(eUnitClass))
		{
			iProductionNeeded *= GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIWorldTrainPercent();
			iProductionNeeded /= 100;
		}
		else
		{
			iProductionNeeded *= GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAITrainPercent();
			iProductionNeeded /= 100;
		}

		iProductionNeeded *= std::max(0, ((GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIPerEraModifier() * getCurrentEra()) + 100));
		iProductionNeeded /= 100;
	}

//	disabled for now, this is used in BTS to scale costs of Settlers
//	iProductionNeeded += getUnitExtraCost(eUnitClass);

	// Python cost modifier
	if(GC.getUSE_GET_UNIT_COST_MOD_CALLBACK())
	{
		CyArgsList argsList;
		argsList.add(getID());	// Player ID
		argsList.add((int)eUnit);
		long lResult=0;
		gDLL->getPythonIFace()->callFunction(PYGameModule, "getUnitCostMod", argsList.makeFunctionArgs(), &lResult);

		if (lResult > 1)
		{
			iProductionNeeded *= lResult;
			iProductionNeeded /= 100;
		}
	}

	return std::max(1, iProductionNeeded);
}


int CvPlayer::getProductionNeeded(BuildingTypes eBuilding) const
{
	int iProductionNeeded;

	iProductionNeeded = GC.getBuildingInfo(eBuilding).getProductionCost();

	iProductionNeeded *= GC.getDefineINT("BUILDING_PRODUCTION_PERCENT");
	iProductionNeeded /= 100;

	iProductionNeeded *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getConstructPercent();
	iProductionNeeded /= 100;

	iProductionNeeded *= GC.getEraInfo(GC.getGameINLINE().getStartEra()).getConstructPercent();
	iProductionNeeded /= 100;

	if (!isHuman() && !isBarbarian())
	{
		if (isWorldWonderClass((BuildingClassTypes)(GC.getBuildingInfo(eBuilding).getBuildingClassType())))
		{
			iProductionNeeded *= GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIWorldConstructPercent();
			iProductionNeeded /= 100;
		}
		else
		{
			iProductionNeeded *= GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIConstructPercent();
			iProductionNeeded /= 100;
		}

		iProductionNeeded *= std::max(0, ((GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIPerEraModifier() * getCurrentEra()) + 100));
		iProductionNeeded /= 100;
	}

	return std::max(1, iProductionNeeded);
}


int CvPlayer::getProductionNeeded(ProjectTypes eProject) const
{
	int iProductionNeeded;

	iProductionNeeded = GC.getProjectInfo(eProject).getProductionCost();

	iProductionNeeded *= GC.getDefineINT("PROJECT_PRODUCTION_PERCENT");
	iProductionNeeded /= 100;

	iProductionNeeded *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getCreatePercent();
	iProductionNeeded /= 100;

	iProductionNeeded *= GC.getEraInfo(GC.getGameINLINE().getStartEra()).getCreatePercent();
	iProductionNeeded /= 100;

	if (!isHuman() && !isBarbarian())
	{
		if (isWorldProject(eProject))
		{
			iProductionNeeded *= GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIWorldCreatePercent();
			iProductionNeeded /= 100;
		}
		else
		{
			iProductionNeeded *= GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAICreatePercent();
			iProductionNeeded /= 100;
		}

		iProductionNeeded *= std::max(0, ((GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIPerEraModifier() * getCurrentEra()) + 100));
		iProductionNeeded /= 100;
	}

	return std::max(1, iProductionNeeded);
}

int CvPlayer::getProductionModifier(UnitTypes eUnit) const
{
	int iMultiplier = 0;

	if (GC.getUnitInfo(eUnit).isMilitaryProduction())
	{
		iMultiplier += getMilitaryProductionModifier();
	}

	for (int iI = 0; iI < GC.getNumTraitInfos(); iI++)
	{
		if (hasTrait((TraitTypes)iI))
		{
			iMultiplier += GC.getUnitInfo(eUnit).getProductionTraits(iI);

			if (GC.getUnitInfo(eUnit).getSpecialUnitType() != NO_SPECIALUNIT)
			{
				iMultiplier += GC.getSpecialUnitInfo((SpecialUnitTypes) GC.getUnitInfo(eUnit).getSpecialUnitType()).getProductionTraits(iI);
			}
		}
	}

	return iMultiplier;
}

int CvPlayer::getProductionModifier(BuildingTypes eBuilding) const
{
	int iMultiplier = 0;
	for (int iI = 0; iI < GC.getNumTraitInfos(); iI++)
	{
		if (hasTrait((TraitTypes)iI))
		{
			iMultiplier += GC.getBuildingInfo(eBuilding).getProductionTraits(iI);

			if (GC.getBuildingInfo(eBuilding).getSpecialBuildingType() != NO_SPECIALBUILDING)
			{
				iMultiplier += GC.getSpecialBuildingInfo((SpecialBuildingTypes) GC.getBuildingInfo(eBuilding).getSpecialBuildingType()).getProductionTraits(iI);
			}
		}
	}

	if(getProductionCostReductionCapitalBuilding() != 0) {
		if(getCapitalCity() != NULL && getCapitalCity()->getNumRealBuilding(eBuilding) > 0) {
			iMultiplier += getProductionCostReductionCapitalBuilding();
		}
	}

	if (::isWorldWonderClass((BuildingClassTypes)(GC.getBuildingInfo(eBuilding).getBuildingClassType())))
	{
		iMultiplier += getMaxGlobalBuildingProductionModifier();
	}

	if (::isTeamWonderClass((BuildingClassTypes)(GC.getBuildingInfo(eBuilding).getBuildingClassType())))
	{
		iMultiplier += getMaxTeamBuildingProductionModifier();
	}

	if (::isNationalWonderClass((BuildingClassTypes)(GC.getBuildingInfo(eBuilding).getBuildingClassType())))
	{
		iMultiplier += getMaxPlayerBuildingProductionModifier();
	}

	return iMultiplier;
}

int CvPlayer::getProductionModifier(ProjectTypes eProject) const
{
	int iMultiplier = 0;

	if (GC.getProjectInfo(eProject).isSpaceship())
	{
		iMultiplier += getSpaceProductionModifier();
	}

	return iMultiplier;
}

int CvPlayer::getBuildingClassPrereqBuilding(BuildingTypes eBuilding, BuildingClassTypes ePrereqBuildingClass, int iExtra) const
{
	CvBuildingInfo& kBuilding = GC.getBuildingInfo(eBuilding);

	int iPrereqs = kBuilding.getPrereqNumOfBuildingClass(ePrereqBuildingClass);

	// dont bother with the rest of the calcs if we have no prereqs
	if (iPrereqs < 1)
	{
		return 0;
	}

	BuildingClassTypes eBuildingClass = (BuildingClassTypes)kBuilding.getBuildingClassType();

	iPrereqs *= std::max(0, (GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getBuildingClassPrereqModifier() + 100));
	iPrereqs /= 100;

	if (!isLimitedWonderClass(eBuildingClass))
	{
		iPrereqs *= (getBuildingClassCount((BuildingClassTypes)(GC.getBuildingInfo(eBuilding).getBuildingClassType())) + iExtra + 1);
	}

	if (GC.getGameINLINE().isOption(GAMEOPTION_ONE_CITY_CHALLENGE) && isHuman())
	{
		iPrereqs = std::min(1, iPrereqs);
	}

	return iPrereqs;
}


void CvPlayer::removeBuildingClass(BuildingClassTypes eBuildingClass)
{
	CvCity* pLoopCity;
	int iLoop;

//FfH: Modified by Kael 02/26/2009
//	BuildingTypes eBuilding;
//	eBuilding = ((BuildingTypes)(GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(eBuildingClass)));
//	if (eBuilding != NO_BUILDING)
//	{
//		for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
//		{
//			if (pLoopCity->getNumRealBuilding(eBuilding) > 0)
//			{
//				pLoopCity->setNumRealBuilding(eBuilding, 0);
//				break;
//			}
//		}
//	}
	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
        for (int iI = 0; iI < GC.getNumBuildingInfos(); iI++)
        {
            if (GC.getBuildingInfo((BuildingTypes)iI).getBuildingClassType() == eBuildingClass)
            {
                if (pLoopCity->getNumRealBuilding((BuildingTypes)iI) > 0)
                {
                    pLoopCity->setNumRealBuilding((BuildingTypes)iI, 0);
                }
            }
        }
	}
//FfH: End Modify

}


// courtesy of the Gourd Bros...
void CvPlayer::processBuilding(BuildingTypes eBuilding, int iChange, CvArea* pArea)
{
	int iI, iJ;

	CvBuildingInfo &kBuilding = GC.getBuildingInfo(eBuilding);

	if (kBuilding.getFreeBuildingClass() != NO_BUILDINGCLASS)
	{
		BuildingTypes eFreeBuilding = (BuildingTypes)GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(kBuilding.getFreeBuildingClass());
/*************************************************************************************************/
/**	BUGFIX (FreeBuilding from Buildings can be -1)                             				    **/
/*************************************************************************************************/
//		changeFreeBuildingCount(eFreeBuilding, iChange);
		if(eFreeBuilding!=NO_BUILDING)
		{
			changeFreeBuildingCount(eFreeBuilding, iChange);
		}
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
	}

	if (kBuilding.getCivicOption() != NO_CIVICOPTION)
	{
		changeHasCivicOptionCount(((CivicOptionTypes)kBuilding.getCivicOption()), iChange);
	}

	changeGreatPeopleRateModifier(kBuilding.getGlobalGreatPeopleRateModifier() * iChange);
	changeGreatGeneralRateModifier(kBuilding.getGreatGeneralRateModifier() * iChange);
	changeDomesticGreatGeneralRateModifier(kBuilding.getDomesticGreatGeneralRateModifier() * iChange);
	changeAnarchyModifier(kBuilding.getAnarchyModifier() * iChange);
	changeGoldenAgeModifier(kBuilding.getGoldenAgeModifier() * iChange);
	changeHurryModifier(GC.getBuildingInfo(eBuilding).getGlobalHurryModifier() * iChange);
	changeFreeExperience(GC.getBuildingInfo(eBuilding).getGlobalFreeExperience() * iChange);
	changeWarWearinessModifier(GC.getBuildingInfo(eBuilding).getGlobalWarWearinessModifier() * iChange);
	pArea->changeFreeSpecialist(getID(), (GC.getBuildingInfo(eBuilding).getAreaFreeSpecialist() * iChange));
	changeFreeSpecialist(GC.getBuildingInfo(eBuilding).getGlobalFreeSpecialist() * iChange);
	changeCoastalTradeRoutes(GC.getBuildingInfo(eBuilding).getCoastalTradeRoutes() * iChange);
	changeTradeRoutes(GC.getBuildingInfo(eBuilding).getGlobalTradeRoutes() * iChange);
	changeReducedEquipmentCost(GC.getBuildingInfo(eBuilding).getReducedEquipmentCost() * iChange);  //added by Darksavant for Arsenal wonder
	changeImprovementDiscountPercent(GC.getBuildingInfo(eBuilding).getImprovementDiscountPercent() * iChange);  //added by Darksavant for Guild of Endeavors wonder
	changeResistHostileTerraforming(GC.getBuildingInfo(eBuilding).getResistHostileTerraforming() * iChange);  //added by Darksavant for changed Stonehenge wonder
	changeIncreaseHostileTerraformTiles(GC.getBuildingInfo(eBuilding).getIncreaseHostileTerraformTiles() * iChange);  //added by Darksavant for changed Stonehenge wonder
	changeIncreaseTerraformTiles(GC.getBuildingInfo(eBuilding).getIncreaseTerraformTiles() * iChange);  //added by Darksavant for changed Stonehenge wonder
	changeCrime(GC.getBuildingInfo(eBuilding).getPlayerCrime() * iChange);  //added by Darksavant for changed Harmonium wonder

//FfH: Added by Kael 04/11/2008
    bool bChange;
    if (iChange > 0)
    {
        bChange = true;
    }
    if (iChange < 0)
    {
        bChange = false;
    }
    if (GC.getBuildingInfo(eBuilding).isHideUnits())
    {
        setHideUnits(bChange);
    }
    if (GC.getBuildingInfo(eBuilding).isSeeInvisible())
    {
        setSeeInvisible(bChange);
    }
    changeResistEnemyModify(GC.getBuildingInfo(eBuilding).getGlobalResistEnemyModify());
    changeResistModify(GC.getBuildingInfo(eBuilding).getGlobalResistModify());
//FfH: End Add

/*************************************************************************************************/
/**	New XML tags Sephi                                											**/
/*************************************************************************************************/
	for (iI = 0; iI < GC.getNumUnitCombatInfos(); iI++) {
		if (GC.getBuildingInfo(eBuilding).getGlobalTrainXPCap(iI) != 0) changeTrainXPCap(GC.getBuildingInfo(eBuilding).getGlobalTrainXPCap(iI) * iChange, iI);
        if (GC.getBuildingInfo(eBuilding).getGlobalTrainXPRate(iI) != 0) changeTrainXPRate(GC.getBuildingInfo(eBuilding).getGlobalTrainXPRate(iI) * iChange, iI);
    }

	for (iI = 0; iI < GC.getNumImprovementInfos(); iI++) {
		for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++) {
			changeImprovementYieldChange(((ImprovementTypes)iI), ((YieldTypes)iJ), (GC.getBuildingInfo(eBuilding).getImprovementYieldChanges(iI, iJ) * iChange));
		}
	}

	if(kBuilding.getManaFromImprovementType() != NO_IMPROVEMENT)
	{
		changeManaFromImprovement(kBuilding.getManaFromImprovementType(), kBuilding.getManaFromImprovement() * iChange);
		ImprovementTypes eUpgrade = (ImprovementTypes)GC.getImprovementInfo((ImprovementTypes)kBuilding.getManaFromImprovementType()).getImprovementUpgrade();
		while(eUpgrade != NO_IMPROVEMENT) {
			changeManaFromImprovement(eUpgrade, kBuilding.getManaFromImprovement() * iChange);
			eUpgrade = (ImprovementTypes)GC.getImprovementInfo(eUpgrade).getImprovementUpgrade();
		}
	}

	if(kBuilding.getFaithFromImprovementType() != NO_IMPROVEMENT)
	{
		changeFaithFromImprovement(kBuilding.getFaithFromImprovementType(), kBuilding.getFaithFromImprovement() * iChange);
		ImprovementTypes eUpgrade = (ImprovementTypes)GC.getImprovementInfo((ImprovementTypes)kBuilding.getFaithFromImprovementType()).getImprovementUpgrade();
		while(eUpgrade != NO_IMPROVEMENT) {
			changeFaithFromImprovement(eUpgrade, kBuilding.getFaithFromImprovement() * iChange);
			eUpgrade = (ImprovementTypes)GC.getImprovementInfo(eUpgrade).getImprovementUpgrade();
		}
	}

/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/


	if (GC.getBuildingInfo(eBuilding).getAreaHealth() > 0)
	{
		pArea->changeBuildingGoodHealth(getID(), (GC.getBuildingInfo(eBuilding).getAreaHealth() * iChange));
	}
	else
	{
		pArea->changeBuildingBadHealth(getID(), (GC.getBuildingInfo(eBuilding).getAreaHealth() * iChange));
	}
	if (GC.getBuildingInfo(eBuilding).getGlobalHealth() > 0)
	{
		changeBuildingGoodHealth(GC.getBuildingInfo(eBuilding).getGlobalHealth() * iChange);
	}
	else
	{
		changeBuildingBadHealth(GC.getBuildingInfo(eBuilding).getGlobalHealth() * iChange);
	}
	pArea->changeBuildingHappiness(getID(), (GC.getBuildingInfo(eBuilding).getAreaHappiness() * iChange));
	changeBuildingHappiness(GC.getBuildingInfo(eBuilding).getGlobalHappiness() * iChange);
	changeWorkerSpeedModifier(GC.getBuildingInfo(eBuilding).getWorkerSpeedModifier() * iChange);
	changeSpaceProductionModifier(GC.getBuildingInfo(eBuilding).getGlobalSpaceProductionModifier() * iChange);
	changeCityDefenseModifier(GC.getBuildingInfo(eBuilding).getAllCityDefenseModifier() * iChange);
	pArea->changeCleanPowerCount(getTeam(), ((GC.getBuildingInfo(eBuilding).isAreaCleanPower()) ? iChange : 0));
	pArea->changeBorderObstacleCount(getTeam(), ((GC.getBuildingInfo(eBuilding).isAreaBorderObstacle()) ? iChange : 0));

	for (iI = 0; iI < NUM_YIELD_TYPES; iI++)
	{
		changeSeaPlotYield(((YieldTypes)iI), (GC.getBuildingInfo(eBuilding).getGlobalSeaPlotYieldChange(iI) * iChange));
		pArea->changeYieldRateModifier(getID(), ((YieldTypes)iI), (GC.getBuildingInfo(eBuilding).getAreaYieldModifier(iI) * iChange));
		changeYieldRateModifier(((YieldTypes)iI), (GC.getBuildingInfo(eBuilding).getGlobalYieldModifier(iI) * iChange));
	}

	for (iI = 0; iI < NUM_COMMERCE_TYPES; iI++)
	{
		changeCommerceRateModifier(((CommerceTypes)iI), (GC.getBuildingInfo(eBuilding).getGlobalCommerceModifier(iI) * iChange));
		changeSpecialistExtraCommerce(((CommerceTypes)iI), (GC.getBuildingInfo(eBuilding).getSpecialistExtraCommerce(iI) * iChange));
		changeStateReligionBuildingCommerce(((CommerceTypes)iI), (GC.getBuildingInfo(eBuilding).getStateReligionCommerce(iI) * iChange));
		changeCommerceFlexibleCount(((CommerceTypes)iI), (GC.getBuildingInfo(eBuilding).isCommerceFlexible(iI)) ? iChange : 0);
	}

	for (iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		BuildingTypes eOurBuilding = (BuildingTypes)GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(iI);
		if (NO_BUILDING != eOurBuilding)
		{
			changeExtraBuildingHappiness(eOurBuilding, (GC.getBuildingInfo(eBuilding).getBuildingHappinessChanges(iI) * iChange));
		}
	}

	for (iI = 0; iI < GC.getNumSpecialistInfos(); iI++)
	{
		for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++)
		{
			changeSpecialistExtraYield(((SpecialistTypes)iI), ((YieldTypes)iJ), (GC.getBuildingInfo(eBuilding).getSpecialistYieldChange(iI, iJ) * iChange));
		}

//FfH: Added by Kael 11/06/2007
		for (iJ = 0; iJ < NUM_COMMERCE_TYPES; iJ++)
		{
			changeSpecialistTypeExtraCommerce(((SpecialistTypes)iI), ((CommerceTypes)iJ), (GC.getBuildingInfo(eBuilding).getSpecialistCommerceChange(iI, iJ) * iChange));
		}
//FfH: End Add

	}
}


bool CvPlayer::canBuild(CvPlot* pPlot, BuildTypes eBuild, bool bTestEra, bool bTestVisible) const
{
	PROFILE_FUNC();

	if(eBuild == NO_BUILD)
		return false;

	int eImprovement = GC.getBuildInfo(eBuild).getImprovement();

	if (!(pPlot->canBuild(eBuild, getID(), bTestVisible)))
	{
		return false;
	}

	if (GC.getBuildInfo(eBuild).getTechPrereq() != NO_TECH)
	{
		if (!(GET_TEAM(getTeam()).isHasTech((TechTypes)GC.getBuildInfo(eBuild).getTechPrereq())))
		{
			if(!bTestVisible)
//			if ((!bTestEra && !bTestVisible) || ((getCurrentEra() + 1) < GC.getTechInfo((TechTypes) GC.getBuildInfo(eBuild).getTechPrereq()).getEra()))
			{
				return false;
			}
		}
	}

	if (!bTestVisible)
	{
		if (pPlot->getFeatureType() != NO_FEATURE

//FfH: Added by Kael 12/16/2007
          && !GC.getCivilizationInfo(getCivilizationType()).isMaintainFeatures(pPlot->getFeatureType())
//FfH: End Add

		)
		{
			if (!(GET_TEAM(getTeam()).isHasTech((TechTypes)GC.getBuildInfo(eBuild).getFeatureTech(pPlot->getFeatureType()))))
			{
				return false;
			}
		}

		if (std::max(0, getGold()) < getBuildCost(pPlot, eBuild))
		{
			return false;
		}

		if(eImprovement != NO_IMPROVEMENT
			&& GC.getBuildInfo(eBuild).isApplyImprovementCost()) {

			if(getGold() < getImprovementGoldCost(pPlot, eBuild))
				return false;
			for(int i = 0; i < NUM_YIELD_TYPES; ++i) {
				if(getGlobalYield(i) < getImprovementYieldCost(pPlot, eBuild, (YieldTypes)i))
				return false;
			}
		}
	}

	if(eImprovement != NO_IMPROVEMENT) {
		if(!GC.getCivilizationInfo(getCivilizationType()).canBuildImprovement(eImprovement))
			return false;		
	}

/*************************************************************************************************/
/**	BETTER AI (block (AI) from building bad Improvements) Sephi                                 **/
/**																			                    **/
/**	                                                                 							**/
/*************************************************************************************************/
	if(!bTestVisible)
	{
		ImprovementTypes eImprovement=(ImprovementTypes)GC.getBuildInfo(eBuild).getImprovement();
		if(eImprovement!=NO_IMPROVEMENT && GC.getImprovementInfo(eImprovement).getImprovementClassType()>=0)
		{
			CvImprovementInfo &info=GC.getImprovementInfo(eImprovement);
			//block AI from building bad Improvements
			if(!isHuman())
			{
				if(pPlot!=NULL && !pPlot->isWater() && pPlot->getOwnerINLINE()==getID() && pPlot->getWorkingCity()!=NULL)
				{
					CvCityAI* pCity=static_cast<CvCityAI*>(pPlot->getWorkingCity());
					if(pCity->AI_PlotBuildValue(eBuild,pPlot)<=0)
					{
						return false;
					}
				}
			}

			//block if Support Costs too high
			if(pPlot->isImprovementToExpensive(getID(),eImprovement))
			{
				return false;
			}
		}
	}

	return true;
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
}

// Returns the cost
int CvPlayer::getBuildCost(const CvPlot* pPlot, BuildTypes eBuild) const
{
	FAssert(eBuild >= 0 && eBuild < GC.getNumBuildInfos());

	if (pPlot->getBuildProgress(eBuild) > 0)
	{
		return 0;
	}

	return std::max(0, GC.getBuildInfo(eBuild).getCost() * (100 + calculateInflationRate())) / 100;
}

int CvPlayer::getImprovementGoldCost(const CvPlot* pPlot, BuildTypes eBuild) const
{
	if (pPlot->getBuildProgress(eBuild) > 0)
	{
		return 0;
	}
	ImprovementTypes eImprovement = (ImprovementTypes)GC.getBuildInfo(eBuild).getImprovement();

	int iGold = (GC.getImprovementInfo(eImprovement).getGoldCost() * getImprovementCostModifier(eImprovement)) / 100;
    if(!isHuman())
    {
		iGold *= 100 + GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIFreeNobleHouseSupportModifier();
		iGold /= 100;
    }

	return iGold;
}

int CvPlayer::getImprovementYieldCost(const CvPlot* pPlot, BuildTypes eBuild, YieldTypes eYield) const
{
	if (pPlot->getBuildProgress(eBuild) > 0)
	{
		return 0;
	}

	ImprovementTypes eImprovement = (ImprovementTypes)GC.getBuildInfo(eBuild).getImprovement();
	int iYield = (GC.getImprovementInfo(eImprovement).getYieldCost(eYield) * getImprovementCostModifier(eImprovement)) / 100;
    if(!isHuman())
    {
		iYield *= 100 + GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIFreeNobleHouseSupportModifier();
		iYield /= 100;
    }

	return iYield;
}


int CvPlayer::getCultivationRate() const
{
	return m_iCultivationRate;
}

void CvPlayer::updateCultivationRate()
{
	int iNumImprovements = 0;
	int iNumPlots = 0;
	CvPlot* pLoopPlot;
	ImprovementTypes eImprovement;

	for(int i = 0; i < GC.getMapINLINE().numPlots(); ++i) {
		pLoopPlot = GC.getMapINLINE().plotByIndex(i);

		if(pLoopPlot->isOwned() && pLoopPlot->getOwner() == getID()) {
			if(!pLoopPlot->isWater() && !pLoopPlot->isPeak() && !pLoopPlot->isCity()) {
				++iNumPlots;
				eImprovement = pLoopPlot->getImprovementType();
				if(eImprovement != NO_IMPROVEMENT && GC.getImprovementInfo(eImprovement).getGoldCost() > 0) {
					++iNumImprovements;
				}
			}
		}
	}

	if(iNumPlots == 0)
		m_iCultivationRate = 100;
	else 
		m_iCultivationRate = (iNumImprovements * 100) / iNumPlots;
}

//TODO: maybe cache this function
int CvPlayer::getImprovementCostModifier(ImprovementTypes eImprovement) const
{
	int iNumImprovements = 0;
	int iNumPlots = 0;
	CvPlot* pLoopPlot;
	ImprovementTypes eLoopImprovement;

	for(int i = 0; i < GC.getMapINLINE().numPlots(); ++i) {
		pLoopPlot = GC.getMapINLINE().plotByIndex(i);

		if(pLoopPlot->isOwned() && pLoopPlot->getOwner() == getID()) {
			if(!pLoopPlot->isWater() && !pLoopPlot->isPeak() && !pLoopPlot->isCity()) {
				++iNumPlots;
				eLoopImprovement = pLoopPlot->getImprovementType();
				if(eLoopImprovement != NO_IMPROVEMENT && GC.getImprovementInfo(eLoopImprovement).getGoldCost() > 0) {
					++iNumImprovements;
				}
			}
		}
	}

	iNumPlots = std::max(1, iNumPlots);

	int iCivMod = GC.getCivilizationInfo(getCivilizationType()).getImprovementCostIncrease();
	iCivMod = std::max(iCivMod, -10);
	int iFreeImps = (20 * 100) / (100 + iCivMod);
	int iFreeImpsScale = (50 * 100) / (100 + iCivMod);

	int iOffset = 0;
	if(GC.getCivilizationInfo(getCivilizationType()).getImpInfrastructureHalfCost() == eImprovement) {
		iOffset = -50;
	}

	iOffset -= getImprovementDiscountPercent();

	int costMod = 100 + iOffset + (((3 * iNumImprovements * (100 + iCivMod)) / iNumPlots) * std::min(iFreeImpsScale, iNumImprovements)) / iFreeImpsScale;

	costMod = std::max(costMod, 25);

	return costMod;
}



RouteTypes CvPlayer::getBestRoute(CvPlot* pPlot) const
{
	PROFILE_FUNC();

	RouteTypes eRoute;
	RouteTypes eBestRoute;
	int iValue;
	int iBestValue;
	int iI;

	iBestValue = 0;
	eBestRoute = NO_ROUTE;

	for (iI = 0; iI < GC.getNumBuildInfos(); iI++)
	{
		eRoute = ((RouteTypes)(GC.getBuildInfo((BuildTypes)iI).getRoute()));

		if (eRoute != NO_ROUTE)
		{
			if ((pPlot != NULL) ? ((pPlot->getRouteType() == eRoute) || canBuild(pPlot, ((BuildTypes)iI))) : GET_TEAM(getTeam()).isHasTech((TechTypes)(GC.getBuildInfo((BuildTypes)iI).getTechPrereq())))
			{
				iValue = GC.getRouteInfo(eRoute).getValue();

				if (iValue > iBestValue)
				{
					iBestValue = iValue;
					eBestRoute = eRoute;
				}
			}
		}
	}

	return eBestRoute;
}


int CvPlayer::getImprovementUpgradeRate() const
{
	int iRate;

	iRate = 1; // XXX

	iRate *= std::max(0, (getImprovementUpgradeRateModifier() + 100));
	iRate /= 100;

	return iRate;
}


int CvPlayer::calculateTotalYield(YieldTypes eYield) const
{
	CvCity* pLoopCity;
	int iTotalCommerce = 0;
	int iLoop = 0;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iTotalCommerce += pLoopCity->getYieldRate(eYield);
	}

	return iTotalCommerce;
}


int CvPlayer::calculateTotalCityHappiness() const
{
	CvCity* pLoopCity;
	int iTotalHappiness = 0;
	int iLoop = 0;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iTotalHappiness += pLoopCity->happyLevel();
	}

	return iTotalHappiness;
}


int CvPlayer::calculateTotalExports(YieldTypes eYield) const
{
	CvCity* pLoopCity;
	CvCity* pTradeCity;
	int iTotalExports = 0;
	int iLoop = 0, iTradeLoop = 0;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		for (iTradeLoop = 0; iTradeLoop < pLoopCity->getTradeRoutes(); iTradeLoop++)
		{
			pTradeCity = pLoopCity->getTradeCity(iTradeLoop);
			if (pTradeCity != NULL)
			{
				if (pTradeCity->getOwnerINLINE() != getID())
				{
					iTotalExports += pLoopCity->calculateTradeYield(eYield, pLoopCity->calculateTradeProfit(pTradeCity));
				}
			}
		}
	}

	return iTotalExports;
}


int CvPlayer::calculateTotalImports(YieldTypes eYield) const
{
	CvCity* pLoopCity;
	CvCity* pTradeCity;
	int iTotalImports = 0;
	int iPlayerLoop = 0, iLoop = 0, iTradeLoop = 0;

	// Loop through players
	for (iPlayerLoop = 0; iPlayerLoop < MAX_CIV_PLAYERS; iPlayerLoop++)
	{
		if (iPlayerLoop != getID())
		{
			for (pLoopCity = GET_PLAYER((PlayerTypes) iPlayerLoop).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes) iPlayerLoop).nextCity(&iLoop))
			{
				for (iTradeLoop = 0; iTradeLoop < pLoopCity->getTradeRoutes(); iTradeLoop++)
				{
					pTradeCity = pLoopCity->getTradeCity(iTradeLoop);
					if (pTradeCity != NULL)
					{
						if (pTradeCity->getOwnerINLINE() == getID())
						{
							iTotalImports += pLoopCity->calculateTradeYield(eYield, pLoopCity->calculateTradeProfit(pTradeCity));
						}
					}
				}
			}
		}
	}
	return iTotalImports;
}


int CvPlayer::calculateTotalCityUnhappiness() const
{
	CvCity* pLoopCity;
	int iTotalUnhappiness = 0;
	int iLoop = 0;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iTotalUnhappiness += pLoopCity->unhappyLevel();
	}

	return iTotalUnhappiness;
}


int CvPlayer::calculateTotalCityHealthiness() const
{
	CvCity* pLoopCity;
	int iTotalHealthiness = 0;
	int iLoop = 0;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iTotalHealthiness += pLoopCity->goodHealth();
	}

	return iTotalHealthiness;
}

int CvPlayer::calculateTotalCityUnhealthiness() const
{
	CvCity* pLoopCity;
	int iTotalUnhealthiness = 0;
	int iLoop = 0;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iTotalUnhealthiness += pLoopCity->badHealth();
	}

	return iTotalUnhealthiness;
}


int CvPlayer::calculateUnitCost(int& iFreeUnits, int& iFreeMilitaryUnits, int& iPaidUnits, int& iPaidMilitaryUnits, int& iBaseUnitCost, int& iMilitaryCost, int& iExtraCost) const
{
	//disabled Unit Cost Sephi
	return 0;
	//disabled Unit Cost

	int iSupport;

	iFreeUnits = GC.getHandicapInfo(getHandicapType()).getFreeUnits();

	iFreeUnits += getBaseFreeUnits();
	iFreeUnits += ((getTotalPopulation() * getFreeUnitsPopulationPercent()) / 100);

	iFreeMilitaryUnits = getBaseFreeMilitaryUnits();
	iFreeMilitaryUnits += ((getTotalPopulation() * getFreeMilitaryUnitsPopulationPercent()) / 100);

	if (!isHuman())
	{
		if (GET_TEAM(getTeam()).hasMetHuman())
		{
			iFreeUnits += getNumCities(); // XXX
			iFreeMilitaryUnits += getNumCities(); // XXX
		}
	}

	iPaidUnits = std::max(0, getNumUnits() - getNumMilitaryUnits() - getNumSummonUnits() - getNumGarrisionUnits()- iFreeUnits);
	iPaidMilitaryUnits = std::max(0, getNumMilitaryUnits() - getNumGarrisionUnits()/2 - iFreeMilitaryUnits);

	iSupport = 0;

	iBaseUnitCost = iPaidUnits * getGoldPerUnit();
	iMilitaryCost = iPaidMilitaryUnits * getGoldPerMilitaryUnit();
	iExtraCost = getExtraUnitCost();

	iSupport = iMilitaryCost + iBaseUnitCost + iExtraCost;

	iSupport *= GC.getHandicapInfo(getHandicapType()).getUnitCostPercent();
	iSupport /= 100;

	if (!isHuman() && !isBarbarian())
	{
		iSupport *= GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIUnitCostPercent();
		iSupport /= 100;

		iSupport *= std::max(0, ((GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIPerEraModifier() * getCurrentEra()) + 100));
		iSupport /= 100;
	}

	FAssert(iSupport >= 0);

	return std::max(0, iSupport);
}


int CvPlayer::calculateUnitCost() const
{
	if (isAnarchy())
	{
		return 0;
	}

//FfH: Added by Kael 10/03/2008
    if (getDisableProduction() > 0 || GC.getGameINLINE().isOption(GAMEOPTION_NO_MAINTENANCE))
    {
        return 0;
    }
//FfH: End Add

	int iFreeUnits;
	int iFreeMilitaryUnits;
	int iPaidUnits;
	int iPaidMilitaryUnits;
	int iMilitaryCost;
	int iBaseUnitCost;
	int iExtraCost;

	return calculateUnitCost(iFreeUnits, iFreeMilitaryUnits, iPaidUnits, iPaidMilitaryUnits, iBaseUnitCost, iMilitaryCost, iExtraCost);
}

int CvPlayer::calculateUnitSupply() const
{
	int iPaidUnits;
	int iBaseSupplyCost;

	if (isAnarchy())
	{
		return 0;
	}

//FfH: Added by Kael 10/03/2008
    if (getDisableProduction() > 0 || GC.getGameINLINE().isOption(GAMEOPTION_NO_MAINTENANCE))
    {
        return 0;
    }
//FfH: End Add

	return calculateUnitSupply(iPaidUnits, iBaseSupplyCost);
}

int CvPlayer::calculateUnitSupply(int& iPaidUnits, int& iBaseSupplyCost) const
{
	int iSupply;

	//added Sephi - mechanic disabled
	return 0;
	//added Sephi - mechanic disabled

	iPaidUnits = std::max(0, (getNumOutsideUnits() - GC.getDefineINT("INITIAL_FREE_OUTSIDE_UNITS")));

	iBaseSupplyCost = iPaidUnits * GC.getDefineINT("INITIAL_OUTSIDE_UNIT_GOLD_PERCENT");
	iBaseSupplyCost /= 100;

	iSupply = iBaseSupplyCost;

	if (!isHuman() && !isBarbarian())
	{
		iSupply *= GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIUnitSupplyPercent();
		iSupply /= 100;

		iSupply *= std::max(0, ((GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIPerEraModifier() * getCurrentEra()) + 100));
		iSupply /= 100;
	}

	FAssert(iSupply >= 0);

	return iSupply;
}


int CvPlayer::calculatePreInflatedCosts() const
{
	CyArgsList argsList;
	argsList.add(getID());
	long lResult = 0;
//  disabled
//	gDLL->getPythonIFace()->callFunction(PYGameModule, "getExtraCost", argsList.makeFunctionArgs(), &lResult);

	return (calculateUnitCost() + calculateUnitSupply() + getTotalMaintenance() + getCivicUpkeep() + (int)lResult);
}


int CvPlayer::calculateInflationRate() const
{
//added Sephi - No Inflation
	return 0;
//added Sephi - No Inflation

//FfH: Added by Kael 11/18/2008
	if (GC.getGameINLINE().isOption(GAMEOPTION_NO_INFLATION))
	{
	    return 0;
	}
//FfH: End Add

	int iTurns = ((GC.getGameINLINE().getGameTurn() + GC.getGameINLINE().getElapsedGameTurns()) / 2);

	if (GC.getGameINLINE().getMaxTurns() > 0)
	{
		iTurns = std::min(GC.getGameINLINE().getMaxTurns(), iTurns);
	}

	iTurns += GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getInflationOffset();

	if (iTurns <= 0)
	{
		return 0;
	}

	int iInflationPerTurnTimes10000 = GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getInflationPercent();
	iInflationPerTurnTimes10000 *= GC.getHandicapInfo(getHandicapType()).getInflationPercent();
	iInflationPerTurnTimes10000 /= 100;

	int iModifier = m_iInflationModifier;
	if (!isHuman() && !isBarbarian())
	{
		int iAIModifier = GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIInflationPercent();
		iAIModifier *= std::max(0, ((GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIPerEraModifier() * getCurrentEra()) + 100));
		iAIModifier /= 100;

		iModifier += iAIModifier - 100;
	}

	iInflationPerTurnTimes10000 *= std::max(0, 100 + iModifier);
	iInflationPerTurnTimes10000 /= 100;

	// Keep up to second order terms in binomial series
	int iRatePercent = (iTurns * iInflationPerTurnTimes10000) / 100;
	iRatePercent += (iTurns * (iTurns - 1) * iInflationPerTurnTimes10000 * iInflationPerTurnTimes10000) / 2000000;

	FAssert(iRatePercent >= 0);

	return iRatePercent;
}


int CvPlayer::calculateInflatedCosts() const
{
	int iCosts;

	iCosts = calculatePreInflatedCosts();

	iCosts *= std::max(0, (calculateInflationRate() + 100));
	iCosts /= 100;

	return iCosts;
}


int CvPlayer::calculateBaseNetGold() const
{
	int iNetGold;

	iNetGold = (getCommerceRate(COMMERCE_GOLD) + getGoldPerTurn());

	iNetGold -= calculateInflatedCosts();

	return iNetGold;
}

int CvPlayer::calculateResearchModifier(TechTypes eTech) const
{
	int iModifier = 100;

	if (NO_TECH == eTech)
	{
		return iModifier;
	}

	int iKnownCount = 0;
	int iPossibleKnownCount = 0;

	for (int iI = 0; iI < MAX_CIV_TEAMS; iI++)
	{
		if (GET_TEAM((TeamTypes)iI).isAlive())
		{
			if (GET_TEAM(getTeam()).isHasMet((TeamTypes)iI))
			{
				if (GET_TEAM((TeamTypes)iI).isHasTech(eTech))
				{
					iKnownCount++;
				}
			}

			iPossibleKnownCount++;
		}
	}

	if (iPossibleKnownCount > 0)
	{
		iModifier += (GC.getDefineINT("TECH_COST_TOTAL_KNOWN_TEAM_MODIFIER") * iKnownCount) / iPossibleKnownCount;
	}

	int iPossiblePaths = 0;
	int iUnknownPaths = 0;

	for (int iI = 0; iI < GC.getNUM_OR_TECH_PREREQS(); iI++)
	{
		if (GC.getTechInfo(eTech).getPrereqOrTechs(iI) != NO_TECH)
		{
			if (!(GET_TEAM(getTeam()).isHasTech((TechTypes)(GC.getTechInfo(eTech).getPrereqOrTechs(iI)))))
			{
				iUnknownPaths++;
			}

			iPossiblePaths++;
		}
	}

	FAssertMsg(iPossiblePaths >= iUnknownPaths, "The number of possible paths is expected to match or exceed the number of unknown ones");

	iModifier += (iPossiblePaths - iUnknownPaths) * GC.getDefineINT("TECH_COST_KNOWN_PREREQ_MODIFIER");

	return iModifier;
}

int CvPlayer::calculateBaseNetResearch(TechTypes eTech) const
{
	TechTypes eResearchTech;

	if (eTech != NO_TECH)
	{
		eResearchTech = eTech;
	}
	else
	{
		eResearchTech = getCurrentResearch();
	}

	return (((GC.getDefineINT("BASE_RESEARCH_RATE") + getCommerceRate(COMMERCE_RESEARCH)) * calculateResearchModifier(eResearchTech)) / 100);
}


int CvPlayer::calculateGoldRate() const
{
	int iRate = 0;

	if (isCommerceFlexible(COMMERCE_RESEARCH))
	{
		iRate = calculateBaseNetGold();
	}
	else
	{
		iRate = std::min(0, (calculateBaseNetResearch() + calculateBaseNetGold()));
	}

	return iRate;
}


int CvPlayer::calculateResearchRate(TechTypes eTech) const
{
	int iRate = 0;

	if (isCommerceFlexible(COMMERCE_RESEARCH))
	{
		iRate = calculateBaseNetResearch(eTech);
	}
	else
	{
		iRate = std::max(1, (calculateBaseNetResearch(eTech) + calculateBaseNetGold()));
	}

	return iRate;
}

int CvPlayer::calculateTotalCommerce() const
{
	int iTotalCommerce = calculateBaseNetGold() + calculateBaseNetResearch();

	for (int i = 0; i < NUM_COMMERCE_TYPES; ++i)
	{
		if (COMMERCE_GOLD != i && COMMERCE_RESEARCH != i)
		{
			iTotalCommerce += getCommerceRate((CommerceTypes)i);
		}
	}

	return iTotalCommerce;
}

bool CvPlayer::isResearch() const
{
	if(GC.getUSE_IS_PLAYER_RESEARCH_CALLBACK())
	{
		CyArgsList argsList;
		long lResult;
		argsList.add(getID());
		lResult = 1;
		gDLL->getPythonIFace()->callFunction(PYGameModule, "isPlayerResearch", argsList.makeFunctionArgs(), &lResult);
		if (lResult == 0)
		{
			return false;
		}
	}

	if (!isFoundedFirstCity())
	{
		return false;
	}

	return true;
}


bool CvPlayer::canEverResearch(TechTypes eTech) const
{
	if (GC.getTechInfo(eTech).isDisable())
	{
		return false;
	}

	if (GC.getTechInfo(eTech).isHolyKnowledge())
	{
		return false;
	}

	if(GC.getTechInfo(eTech).isCulturalAchievement())
	{
		return false;
	}

	if(GC.getTechInfo(eTech).isReligion())
	{
		return false;
	}

	if(GC.getTechInfo(eTech).getBlockedbyAlignment()!=NO_ALIGNMENT)
	{
		if(getAlignment()==GC.getTechInfo(eTech).getBlockedbyAlignment())
		{
			return false;
		}
	}

	if (GC.getCivilizationInfo(getCivilizationType()).isCivilizationDisableTechs(eTech))
	{
		return false;
	}

	//temporary until Unit Rosters are fully implemented
	if (GC.getCivilizationInfo(getCivilizationType()).getDefaultUnit() != NO_UNIT)
	{
		int iIndex = 0;
		if(GC.getTechInfo(eTech).getFlavorValue(FLAVOR_MELEE) == 100)
			++iIndex;
		if(GC.getTechInfo(eTech).getFlavorValue(FLAVOR_RANGED) == 100)
			++iIndex;
		if(GC.getTechInfo(eTech).getFlavorValue(FLAVOR_RECON) == 100)
			++iIndex;
		if(GC.getTechInfo(eTech).getFlavorValue(FLAVOR_MOUNTED) == 100)
			++iIndex;
		if(iIndex > 0 && iIndex < 4)
			return false;
	}


//FfH Techs: Added by Kael 08/09/2007
    if (GC.getTechInfo(eTech).getPrereqReligion() != NO_RELIGION)
    {
        if (getStateReligion() != GC.getTechInfo(eTech).getPrereqReligion())
        {
            return false;
        }
    }
//FfH: End Add

	if(GC.getUSE_CANNOT_RESEARCH_CALLBACK())
	{
		CyArgsList argsList;
		argsList.add(getID());
		argsList.add(eTech);
		argsList.add(false);
		long lResult=0;
		gDLL->getPythonIFace()->callFunction(PYGameModule, "cannotResearch", argsList.makeFunctionArgs(), &lResult);
		if (lResult == 1)
		{
			return false;
		}
	}

	return true;
}


bool CvPlayer::canResearch(TechTypes eTech, bool bTrade) const
{
	bool bFoundPossible;
	bool bFoundValid;
	int iI;

	if(GC.getUSE_CAN_RESEARCH_CALLBACK())
	{
		CyArgsList argsList;
		argsList.add(getID());
		argsList.add(eTech);
		argsList.add(bTrade);
		long lResult=0;
		gDLL->getPythonIFace()->callFunction(PYGameModule, "canResearch", argsList.makeFunctionArgs(), &lResult);
		if (lResult == 1)
		{
			return true;
		}
	}

	if (!isResearch() && getAdvancedStartPoints() < 0)
	{
		return false;
	}

	if (GET_TEAM(getTeam()).isHasTech(eTech))
	{
		return false;
	}

	bFoundPossible = false;
	bFoundValid = false;

	for (iI = 0; iI < GC.getNUM_OR_TECH_PREREQS(); iI++)
	{
		TechTypes ePrereq = (TechTypes)GC.getTechInfo(eTech).getPrereqOrTechs(iI);
		if (ePrereq != NO_TECH)
		{
			bFoundPossible = true;

			if (GET_TEAM(getTeam()).isHasTech(ePrereq))
			{
				if (!bTrade || GC.getGameINLINE().isOption(GAMEOPTION_NO_TECH_BROKERING) || !GET_TEAM(getTeam()).isNoTradeTech(ePrereq))
				{
					bFoundValid = true;
					break;
				}
			}
		}
	}

	if (bFoundPossible && !bFoundValid)
	{
		return false;
	}

	for (iI = 0; iI < GC.getNUM_AND_TECH_PREREQS(); iI++)
	{
		TechTypes ePrereq = (TechTypes)GC.getTechInfo(eTech).getPrereqAndTechs(iI);
		if (ePrereq != NO_TECH)
		{
			if (!GET_TEAM(getTeam()).isHasTech(ePrereq))
			{
				return false;
			}

			if (bTrade && !GC.getGameINLINE().isOption(GAMEOPTION_NO_TECH_BROKERING) && GET_TEAM(getTeam()).isNoTradeTech(ePrereq))
			{
				return false;
			}
		}
	}

	if (!canEverResearch(eTech))
	{
		return false;
	}

	if (!CvString(GC.getTechInfo(eTech).getPyRequirement()).empty())
    {
        CyArgsList argsList;
        argsList.add(getID());	// pass in unit class
        argsList.add(eTech);//the spell #
        long lResult = 0;
        gDLL->getPythonIFace()->callFunction(PYSpellModule, "canResearch", argsList.makeFunctionArgs(), &lResult);
        if (lResult == 0)
        {
            return false;
        }
    }

	return true;
}


TechTypes CvPlayer::getCurrentResearch() const
{
	CLLNode<TechTypes>* pResearchNode;

	pResearchNode = headResearchQueueNode();

	if (pResearchNode != NULL)
	{
		return pResearchNode->m_data;
	}
	else
	{
		return NO_TECH;
	}
}


bool CvPlayer::isCurrentResearchRepeat() const
{
	TechTypes eCurrentResearch;

	eCurrentResearch = getCurrentResearch();

	if (eCurrentResearch == NO_TECH)
	{
		return false;
	}

	return GC.getTechInfo(eCurrentResearch).isRepeat();
}


bool CvPlayer::isNoResearchAvailable() const
{
	int iI;

	if (getCurrentResearch() != NO_TECH)
	{
		return false;
	}

	for (iI = 0; iI < GC.getNumTechInfos(); iI++)
	{
		if (canResearch((TechTypes)iI))
		{
			return false;
		}
	}

	return true;
}


int CvPlayer::getResearchTurnsLeft(TechTypes eTech, bool bOverflow) const
{
	int iTurnsLeft = getResearchTurnsLeftTimes100(eTech, bOverflow);

	iTurnsLeft = (iTurnsLeft + 99) / 100; // round up

	return std::max(1, iTurnsLeft);
}

int CvPlayer::getResearchTurnsLeftTimes100(TechTypes eTech, bool bOverflow) const
{
    int iResearchRate;
	int iOverflow;
	int iResearchLeft;
	int iTurnsLeft;
	int iI;

	iResearchRate = 0;
	iOverflow = 0;

	for (iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getTeam())
			{
				if ((iI == getID()) || (GET_PLAYER((PlayerTypes)iI).getCurrentResearch() == eTech))
				{
					iResearchRate += GET_PLAYER((PlayerTypes)iI).calculateResearchRate(eTech);
					iOverflow += (GET_PLAYER((PlayerTypes)iI).getOverflowResearch() * calculateResearchModifier(eTech)) / 100;
				}
			}
		}
	}

	if(GC.getTechInfo((eTech)).isSpellResearch())
	{
		iResearchRate=getArcaneIncome();
	}

	if (iResearchRate == 0)
	{
		return MAX_INT;
	}

	iResearchLeft = GET_TEAM(getTeam()).getResearchLeft(eTech);

	if (bOverflow)
	{
		iResearchLeft -= iOverflow;
	}

	iResearchLeft *= 100;

	iTurnsLeft = (iResearchLeft / iResearchRate);

	if (iTurnsLeft * iResearchRate < iResearchLeft)
	{
		++iTurnsLeft;
	}

	return std::max(1, iTurnsLeft);


}


bool CvPlayer::isCivic(CivicTypes eCivic) const
{
	int iI;

	for (iI = 0; iI < GC.getNumCivicOptionInfos(); iI++)
	{
		if (getCivics((CivicOptionTypes)iI) == eCivic)
		{
			return true;
		}
	}

	return false;
}


bool CvPlayer::canDoCivics(CivicTypes eCivic) const
{
	PROFILE_FUNC();


/************************************************************************************************/
/* UNOFFICIAL_PATCH                       10/05/09                                jdog5000      */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
	// Circumvents second crash bug in simultaneous turns MP games
	if( eCivic == NO_CIVIC )
	{
		return 1;
	}
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/

//FfH: Added by Kael 11/23/2007
    if (isFullMember((VoteSourceTypes)0))
    {
//FfH: End Add

        if (GC.getGameINLINE().isForceCivicOption((CivicOptionTypes)(GC.getCivicInfo(eCivic).getCivicOptionType())))
        {
            return GC.getGameINLINE().isForceCivic(eCivic);
        }

//FfH: Added by Kael 11/23/2007
    }
//FfH: End Add

	if (!isHasCivicOption((CivicOptionTypes)(GC.getCivicInfo(eCivic).getCivicOptionType())) && !(GET_TEAM(getTeam()).isHasTech((TechTypes)(GC.getCivicInfo(eCivic).getTechPrereq()))))
	{
		return false;
	}

//FfH Civics: Added by Kael 08/11/2007
    if (GC.getCivicInfo(eCivic).isPrereqWar())
    {
        if (GET_TEAM(getTeam()).getAtWarCount(true) == 0)
        {
            return false;
		}
    }
    if (GC.getCivicInfo(eCivic).getBlockAlignment() != NO_ALIGNMENT)
    {
        if (getAlignment() == GC.getCivicInfo(eCivic).getBlockAlignment())
        {
            return false;
        }
    }
    if (GC.getCivicInfo(eCivic).getPrereqAlignment() != NO_ALIGNMENT)
    {
        if (getAlignment() != GC.getCivicInfo(eCivic).getPrereqAlignment())
        {
            return false;
        }
    }
    if (GC.getCivicInfo(eCivic).getPrereqCivilization() != NO_CIVILIZATION)
    {
        if (getCivilizationType() != GC.getCivicInfo(eCivic).getPrereqCivilization())
        {
            return false;
        }
    }
    if (GC.getCivicInfo(eCivic).getPrereqReligion() != NO_RELIGION)
    {
        if (getStateReligion() != GC.getCivicInfo(eCivic).getPrereqReligion())
        {
            return false;
        }
    }

	if(!GC.getCivilizationInfo(getCivilizationType()).isCivicAllowed(eCivic)) {
		return false;
	}

//FfH: End Add

	if (GC.getLeaderHeadInfo(getLeaderType()).getHatedCivic()==eCivic)
    {
	    return false;    
    }

	if(getCivics((CivicOptionTypes)GC.getCivicInfo(eCivic).getCivicOptionType())!=NO_CIVIC)
	{
		if (GC.getCivicInfo(getCivics((CivicOptionTypes)GC.getCivicInfo(eCivic).getCivicOptionType())).isPermanent() &&  getCivics((CivicOptionTypes)GC.getCivicInfo(eCivic).getCivicOptionType())!=eCivic)
		{
			return false;
		}
	}

	if(GC.getUSE_CANNOT_DO_CIVIC_CALLBACK())
	{
		CyArgsList argsList2; // XXX
		argsList2.add(getID());
		argsList2.add(eCivic);
		long lResult=0;
		gDLL->getPythonIFace()->callFunction(PYGameModule, "cannotDoCivic", argsList2.makeFunctionArgs(), &lResult);
		if (lResult == 1)
		{
			return false;
		}
	}

	return true;
}


bool CvPlayer::canRevolution(CivicTypes* paeNewCivics) const
{
	int iI;

	if (isAnarchy())
	{
		return false;
	}

	if (getRevolutionTimer() > 0)
	{
		return false;
	}

	if (paeNewCivics == NULL)
	{
		// XXX is this necessary?
		for (iI = 0; iI < GC.getNumCivicInfos(); iI++)
		{
			if (canDoCivics((CivicTypes)iI))
			{
				if (getCivics((CivicOptionTypes)GC.getCivicInfo((CivicTypes) iI).getCivicOptionType()) != iI)
				{
					return true;
				}
			}
		}
	}
	else
	{
		for (iI = 0; iI < GC.getNumCivicOptionInfos(); ++iI)
		{

//FfH: Added by Kael 12/30/2007
            if (isFullMember((VoteSourceTypes)0))
            {
//FfH: End Add

			if (GC.getGameINLINE().isForceCivicOption((CivicOptionTypes)iI))
			{
				if (!GC.getGameINLINE().isForceCivic(paeNewCivics[iI]))
				{
					return false;
				}
			}

//FfH: Added by Kael 12/30/2007
            }
//FfH: End Add

			if (getCivics((CivicOptionTypes)iI) != paeNewCivics[iI])
			{
				return true;
			}
		}
	}

	return false;
}


void CvPlayer::revolution(CivicTypes* paeNewCivics, bool bForce)
{
	int iAnarchyLength;
	int iI;

	if (!bForce && !canRevolution(paeNewCivics))
	{
		return;
	}

	iAnarchyLength = getCivicAnarchyLength(paeNewCivics);

	if (iAnarchyLength > 0)
	{
		changeAnarchyTurns(iAnarchyLength);

		for (iI = 0; iI < GC.getNumCivicOptionInfos(); iI++)
		{
			setCivics(((CivicOptionTypes)iI), paeNewCivics[iI]);
		}
	}
	else
	{
		for (iI = 0; iI < GC.getNumCivicOptionInfos(); iI++)
		{
			setCivics(((CivicOptionTypes)iI), paeNewCivics[iI]);
		}
	}

	setRevolutionTimer(std::max(1, ((100 + getAnarchyModifier()) * GC.getDefineINT("MIN_REVOLUTION_TURNS")) / 100) + iAnarchyLength);

	if (getID() == GC.getGameINLINE().getActivePlayer())
	{
		gDLL->getInterfaceIFace()->setDirty(Popup_DIRTY_BIT, true); // to force an update of the civic chooser popup
	}
}


int CvPlayer::getCivicPercentAnger(CivicTypes eCivic, bool bIgnore) const
{
	int iCount;
	int iPossibleCount;
	int iI;

//FfH: Added by Kael 11/25/2007
    if (GET_TEAM(getTeam()).isNoCivicAnger())
    {
        return 0;
    }
//FfH: End Add

	if (GC.getCivicInfo(eCivic).getCivicPercentAnger() == 0)
	{
		return 0;
	}

	if (!bIgnore && (getCivics((CivicOptionTypes)(GC.getCivicInfo(eCivic).getCivicOptionType())) == eCivic))
	{
		return 0;
	}

	iCount = 0;
	iPossibleCount = 0;

	for (iI = 0; iI < MAX_CIV_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() != getTeam())
			{
				if (GET_PLAYER((PlayerTypes)iI).getCivics((CivicOptionTypes)(GC.getCivicInfo(eCivic).getCivicOptionType())) == eCivic)
				{
					iCount += GET_PLAYER((PlayerTypes)iI).getNumCities();
				}

				iPossibleCount += GET_PLAYER((PlayerTypes)iI).getNumCities();
			}
		}
	}

	if (iPossibleCount == 0)
	{
		return 0;
	}

	return ((GC.getCivicInfo(eCivic).getCivicPercentAnger() * iCount) / iPossibleCount);
}


bool CvPlayer::canDoReligion(ReligionTypes eReligion) const
{
	if (GET_TEAM(getTeam()).getHasReligionCount(eReligion) == 0)
	{
		return false;
	}

//FfH Traits: Added by Kael 08/02/2007
    if (isAgnostic())
    {
        return false;
    }
    if (GC.getLeaderHeadInfo(getPersonalityType()).getReligionWeightModifier(eReligion) <= -100)
    {
        return false;
    }
//FfH: End Add
/*************************************************************************************************/
/**	BETTER AI (favorite Religions) Sephi                                                        **/
/**																			                    **/
/**	                                                                 							**/
/*************************************************************************************************/
    if (!isHuman())
    {
        if (GC.getLeaderHeadInfo(getPersonalityType()).getReligionWeightModifier(eReligion) < 0)
        {
            return false;
        }
    }
/**	New Esus                                                                 					**/
/** Esus isn't allowed State Religion Anymore and Empy isn't allowed State Religion for Esus Followers **/

    /*if (eReligion==RELIGION_COUNCIL_OF_ESUS)
    {
        return false;
    }*/
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

	return true;
}


bool CvPlayer::canChangeReligion() const
{
	int iI;

	for (iI = 0; iI < GC.getNumReligionInfos(); iI++)
	{
		if (canConvert((ReligionTypes)iI))
		{
			return true;
		}
	}

	return false;
}


bool CvPlayer::canConvert(ReligionTypes eReligion) const
{
/*************************************************************************************************/
/**	BETTER AI (favorite Religions) Sephi                                                        **/
/**																			                    **/
/**	                                                                 							**/
/*************************************************************************************************/
    if (!isHuman() && eReligion !=NO_RELIGION)
    {
        if (getFavoriteReligion() != NO_RELIGION)
        {
            if (getFavoriteReligion() == getLastStateReligion())
            {
                return false;
            }
        }

        if (GC.getLeaderHeadInfo(getLeaderType()).getReligionWeightModifier(eReligion) < 0)
        {
            return false;
        }

		if (getFavoriteReligion() != eReligion) {
			if(getAlignmentAfterConversion(eReligion) != getAlignment()) {
				return false;
			}
		}
    }

	if(isCityState())
	{
		if(eReligion!=GC.getCityStateInfo((CityStateTypes)getCityStateType()).getReligionType())
		{
			return false;
		}
	}
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
	if (isBarbarian())
	{
		return false;
	}

	if (isAnarchy())
	{
		return false;
	}

	if (getConversionTimer() > 0)
	{
		return false;
	}

	if (!isStateReligion())
	{
		return false;
	}

	if (getLastStateReligion() == eReligion)
	{
		return false;
	}

	if (eReligion != NO_RELIGION)
	{
		if (!canDoReligion(eReligion))
		{
			return false;
		}
	}

	return true;
}


void CvPlayer::convert(ReligionTypes eReligion)
{
	int iAnarchyLength;

	if (!canConvert(eReligion))
	{
		return;
	}

	iAnarchyLength = getReligionAnarchyLength();

	changeAnarchyTurns(iAnarchyLength);

	setLastStateReligion(eReligion);

	setConversionTimer(std::max(1, ((100 + getAnarchyModifier()) * GC.getDefineINT("MIN_CONVERSION_TURNS")) / 100) + iAnarchyLength);
/*************************************************************************************************/
/**	ADDON (New Mana) Sephi                                                   					**/
/*************************************************************************************************/
    changeFaith(-getFaith());
	changeFaithMod(-getFaithMod());
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

	if (eReligion != NO_RELIGION)
	{
		setAlignment(getAlignmentAfterConversion(eReligion));
	}

	//meet Holy City States when convert to Religion
	for(int iI=0;iI<GC.getMAX_CIV_PLAYERS();iI++)
	{
		if(getTeam()!=GET_PLAYER((PlayerTypes)iI).getTeam())
		{
			if(GET_PLAYER((PlayerTypes)iI).isCityState())
			{
				if(GC.getCityStateInfo((CityStateTypes)GET_PLAYER((PlayerTypes)iI).getCityStateType()).getReligionType()==eReligion)
				{
					GET_TEAM(getTeam()).meet(GET_PLAYER((PlayerTypes)iI).getTeam(),true);
				}
			}
		}
	}

//>>>>Unofficial Bug Fix: Added by Denev 2010/02/20
//*** Validate queued reserach requirements because some techs requires certain state religion.
	for (CLLNode<TechTypes>* pResearchNode = headResearchQueueNode(); pResearchNode; pResearchNode = nextResearchQueueNode(pResearchNode))
	{
		if (!canEverResearch(pResearchNode->m_data))
		{
			bool bFinished = false;
			while (!bFinished)
			{
				bFinished = (tailResearchQueueNode()->m_data == pResearchNode->m_data);
				popResearch(tailResearchQueueNode()->m_data);
			}
			break;
		}
	}
//<<<<Unofficial Bug Fix: End Add
}


bool CvPlayer::hasHolyCity(ReligionTypes eReligion) const
{
	CvCity* pHolyCity;

	FAssertMsg(eReligion != NO_RELIGION, "Religion is not assigned a valid value");

	pHolyCity = GC.getGameINLINE().getHolyCity(eReligion);

	if (pHolyCity != NULL)
	{
		return (pHolyCity->getOwnerINLINE() == getID());
	}

	return false;
}


int CvPlayer::countHolyCities() const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < GC.getNumReligionInfos(); iI++)
	{
		if (hasHolyCity((ReligionTypes)iI))
		{
			iCount++;
		}
	}

	return iCount;
}


void CvPlayer::foundReligion(ReligionTypes eReligion, ReligionTypes eSlotReligion, bool bAward)
{
	CvCity* pLoopCity;
	CvCity* pBestCity;
	UnitTypes eFreeUnit;
	bool bStarting;
	int iValue;
	int iBestValue;
	int iLoop;

	if (NO_RELIGION == eReligion)
	{
		return;
	}

//FfH: Added by Kael 10/29/2007
    if (GC.getGameINLINE().isReligionDisabled(eReligion))
    {
        return;
    }
//FfH: End Add

	/**
	if (GC.getGameINLINE().isReligionFounded(eReligion))
	{

		if (isHuman())
		{
			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_FOUND_RELIGION, eSlotReligion);
			if (NULL != pInfo)
			{
				gDLL->getInterfaceIFace()->addPopup(pInfo, getID());
			}
		}
		else
		{
			foundReligion(AI_chooseReligion(), eSlotReligion, bAward);
		}

		return;
	}
	**/
	if(GC.getGameINLINE().getHolyCity(eReligion)!=NULL)
	{
		return;
	}

	GC.getGameINLINE().setReligionSlotTaken(eSlotReligion, true);

	bStarting = ((GC.getReligionInfo(eSlotReligion).getTechPrereq() == NO_TECH) || (GC.getTechInfo((TechTypes) GC.getReligionInfo(eSlotReligion).getTechPrereq()).getEra() < GC.getGameINLINE().getStartEra()));

	iBestValue = 0;
	pBestCity = NULL;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		if (!bStarting || !(pLoopCity->isHolyCity()))
		{
			iValue = 10;
			iValue += pLoopCity->getPopulation();
			iValue += GC.getGameINLINE().getSorenRandNum(GC.getDefineINT("FOUND_RELIGION_CITY_RAND"), "Found Religion");

			iValue /= (pLoopCity->getReligionCount() + 1);

			if (pLoopCity->isCapital())
			{
				iValue /= 8;
			}

//FfH: Added by Kael 11/18/2007
			if (pLoopCity->isSettlement())
			{
				iValue /= 8;
			}
//FfH: End Add

			iValue = std::max(1, iValue);

			if (iValue > iBestValue)
			{
				iBestValue = iValue;
				pBestCity = pLoopCity;
			}
		}
	}

	if (pBestCity != NULL)
	{
		GC.getGameINLINE().setHolyCity(eReligion, pBestCity, true);

		if (bAward)
		{
			if (GC.getReligionInfo(eSlotReligion).getNumFreeUnits() > 0)
			{
				eFreeUnit = ((UnitTypes)(GC.getCivilizationInfo(getCivilizationType()).getCivilizationUnits(GC.getReligionInfo(eReligion).getFreeUnitClass())));

				if (eFreeUnit != NO_UNIT)
				{
					for (int i = 0; i < GC.getReligionInfo(eSlotReligion).getNumFreeUnits(); ++i)
					{
						initUnit(eFreeUnit, pBestCity->getX_INLINE(), pBestCity->getY_INLINE());
					}
				}
			}
		}
	}
}


bool CvPlayer::hasHeadquarters(CorporationTypes eCorporation) const
{
	CvCity* pHeadquarters = GC.getGameINLINE().getHeadquarters(eCorporation);

	FAssert(eCorporation != NO_CORPORATION);

	if (pHeadquarters != NULL)
	{
		return (pHeadquarters->getOwnerINLINE() == getID());
	}

	return false;
}


int CvPlayer::countHeadquarters() const
{
	int iCount = 0;

	for (int iI = 0; iI < GC.getNumCorporationInfos(); iI++)
	{
		if (hasHeadquarters((CorporationTypes)iI))
		{
			iCount++;
		}
	}

	return iCount;
}


int CvPlayer::countCorporations(CorporationTypes eCorporation) const
{
	int iCount = 0;

	int iLoop;
	for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		if (pLoopCity->isHasCorporation(eCorporation))
		{
			++iCount;
		}
	}

	return iCount;
}


void CvPlayer::foundCorporation(CorporationTypes eCorporation)
{
	CvCity* pLoopCity;
	CvCity* pBestCity;
	bool bStarting;
	int iValue;
	int iBestValue;
	int iLoop;

	if (GC.getGameINLINE().isCorporationFounded(eCorporation))
	{
		return;
	}

	bStarting = ((GC.getCorporationInfo(eCorporation).getTechPrereq() == NO_TECH) || (GC.getTechInfo((TechTypes) GC.getCorporationInfo(eCorporation).getTechPrereq()).getEra() < GC.getGameINLINE().getStartEra()));

	iBestValue = 0;
	pBestCity = NULL;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		if (!bStarting || !(pLoopCity->isHeadquarters()))
		{
			iValue = 10;
			iValue += pLoopCity->getPopulation();

			for (int i = 0; i < GC.getNUM_CORPORATION_PREREQ_BONUSES(); ++i)
			{
				if (NO_BONUS != GC.getCorporationInfo(eCorporation).getPrereqBonus(i))
				{
					iValue += 10 * pLoopCity->getNumBonuses((BonusTypes)GC.getCorporationInfo(eCorporation).getPrereqBonus(i));
				}
			}

			iValue += GC.getGameINLINE().getSorenRandNum(GC.getDefineINT("FOUND_CORPORATION_CITY_RAND"), "Found Corporation");

			iValue /= (pLoopCity->getCorporationCount() + 1);

			iValue = std::max(1, iValue);

			if (iValue > iBestValue)
			{
				iBestValue = iValue;
				pBestCity = pLoopCity;
			}
		}
	}

	if (pBestCity != NULL)
	{
		pBestCity->setHeadquarters(eCorporation);
	}
}


int CvPlayer::getCivicAnarchyLength(CivicTypes* paeNewCivics) const
{
	bool bChange;
	int iAnarchyLength;
	int iI;

	if (getMaxAnarchyTurns() == 0)
	{
		return 0;
	}

	if (isGoldenAge())
	{
		return 0;
	}

	iAnarchyLength = 0;

	bChange = false;

	for (iI = 0; iI < GC.getNumCivicOptionInfos(); iI++)
	{
		if (paeNewCivics[iI] != getCivics((CivicOptionTypes)iI))
		{
			iAnarchyLength += GC.getCivicInfo(paeNewCivics[iI]).getAnarchyLength();

			bChange = true;
		}
	}

	if (bChange)
	{
		iAnarchyLength += GC.getDefineINT("BASE_CIVIC_ANARCHY_LENGTH");

		iAnarchyLength += ((getNumCities() * GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getNumCitiesAnarchyPercent()) / 100);
	}

	iAnarchyLength = ((iAnarchyLength * std::max(0, (getAnarchyModifier() + 100))) / 100);

	if (iAnarchyLength == 0)
	{
		return 0;
	}

	iAnarchyLength *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getAnarchyPercent();
	iAnarchyLength /= 100;

	iAnarchyLength *= GC.getEraInfo(GC.getGameINLINE().getStartEra()).getAnarchyPercent();
	iAnarchyLength /= 100;

	return range(iAnarchyLength, 1, getMaxAnarchyTurns());
}


int CvPlayer::getReligionAnarchyLength() const
{
	int iAnarchyLength;

	if (getMaxAnarchyTurns() == 0)
	{
		return 0;
	}

	if (isGoldenAge())
	{
		return 0;
	}

	iAnarchyLength = GC.getDefineINT("BASE_RELIGION_ANARCHY_LENGTH");

	iAnarchyLength += ((getNumCities() * GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getNumCitiesAnarchyPercent()) / 100);

	iAnarchyLength = ((iAnarchyLength * std::max(0, (getAnarchyModifier() + 100))) / 100);

	if (iAnarchyLength == 0)
	{
		return 0;
	}

	iAnarchyLength *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getAnarchyPercent();
	iAnarchyLength /= 100;

	iAnarchyLength *= GC.getEraInfo(GC.getGameINLINE().getStartEra()).getAnarchyPercent();
	iAnarchyLength /= 100;

	return range(iAnarchyLength, 1, getMaxAnarchyTurns());
}



int CvPlayer::unitsRequiredForGoldenAge() const
{
//modified Sephi
//	return (GC.getDefineINT("BASE_GOLDEN_AGE_UNITS") + (getNumUnitGoldenAges() * GC.getDefineINT("GOLDEN_AGE_UNITS_MULTIPLIER")));
	return 1;
//modified
}


int CvPlayer::unitsGoldenAgeCapable() const
{
	CvUnit* pLoopUnit;
	int iCount;
	int iLoop;

	iCount = 0;

	for(pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{
		if (pLoopUnit->isGoldenAge())
		{
			iCount++;
		}
	}

	return iCount;
}


int CvPlayer::unitsGoldenAgeReady() const
{
	PROFILE_FUNC();

	CvUnit* pLoopUnit;
	bool* pabUnitUsed;
	int iCount;
	int iLoop;
	int iI;

	pabUnitUsed = new bool[GC.getNumUnitInfos()];

	for (iI = 0; iI < GC.getNumUnitInfos(); iI++)
	{
		pabUnitUsed[iI] = false;
	}

	iCount = 0;

	for(pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{
		if (!(pabUnitUsed[pLoopUnit->getUnitType()]))
		{
		if (pLoopUnit->isGoldenAge())
		{
				pabUnitUsed[pLoopUnit->getUnitType()] = true;
				iCount++;
			}
		}
	}

	SAFE_DELETE_ARRAY(pabUnitUsed);

	return iCount;
}


void CvPlayer::killGoldenAgeUnits(CvUnit* pUnitAlive)
{
	CvUnit* pLoopUnit;
	CvUnit* pBestUnit;
	bool* pabUnitUsed;
	int iUnitsRequired;
	int iValue;
	int iBestValue;
	int iLoop;
	int iI;

	pabUnitUsed = new bool[GC.getNumUnitInfos()];

	for (iI = 0; iI < GC.getNumUnitInfos(); iI++)
	{
		pabUnitUsed[iI] = false;
	}

	iUnitsRequired = unitsRequiredForGoldenAge();

	if (pUnitAlive != NULL)
	{
		pabUnitUsed[pUnitAlive->getUnitType()] = true;
		iUnitsRequired--;
	}

	for (iI = 0; iI < iUnitsRequired; iI++)
	{
		iBestValue = 0;
		pBestUnit = NULL;

		for(pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
		{
			if (pLoopUnit->isGoldenAge())
			{
				if (!(pabUnitUsed[pLoopUnit->getUnitType()]))
				{
					iValue = 10000;

					iValue /= (plotDistance(pLoopUnit->getX_INLINE(), pLoopUnit->getY_INLINE(), pUnitAlive->getX_INLINE(), pUnitAlive->getY_INLINE()) + 1);

					if (iValue > iBestValue)
					{
						iBestValue = iValue;
						pBestUnit = pLoopUnit;
					}
				}
			}
		}

		FAssert(pBestUnit != NULL);
		if (pBestUnit != NULL)
		{
			pabUnitUsed[pBestUnit->getUnitType()] = true;

			pBestUnit->kill(true);

			//play animations
			if (pBestUnit->plot()->isActiveVisible(false))
			{
				//kill removes bestUnit from any groups
				pBestUnit->getGroup()->pushMission(MISSION_GOLDEN_AGE, 0);
			}
		}
	}

	SAFE_DELETE_ARRAY(pabUnitUsed);
}


int CvPlayer::greatPeopleThreshold(bool bMilitary) const
{
	int iThreshold;

	if (bMilitary)
	{
		iThreshold = ((GC.getDefineINT("GREAT_GENERALS_THRESHOLD") * std::max(0, (getGreatGeneralsThresholdModifier() + 100))) / 100);
	}
	else
	{
		iThreshold = GC.getDefineINT("GREAT_PEOPLE_THRESHOLD") + getGreatPeopleThresholdModifier();
	}

	iThreshold *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getGreatPeoplePercent();
	if (bMilitary)
	{
		iThreshold /= std::max(1, GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getTrainPercent());
	}
	else
	{
		iThreshold /= 100;
	}
/**
	iThreshold *= GC.getEraInfo(GC.getGameINLINE().getStartEra()).getGreatPeoplePercent();
	iThreshold /= 100;
**/

	return std::max(1, iThreshold);
}


int CvPlayer::specialistYield(SpecialistTypes eSpecialist, YieldTypes eYield) const
{
	return (GC.getSpecialistInfo(eSpecialist).getYieldChange(eYield) + getSpecialistExtraYield(eSpecialist, eYield));
}


int CvPlayer::specialistCommerce(SpecialistTypes eSpecialist, CommerceTypes eCommerce) const
{

//FfH: Modified by Kael 11/08/2007
//	return (GC.getSpecialistInfo(eSpecialist).getCommerceChange(eCommerce) + getSpecialistExtraCommerce(eCommerce));
	return (GC.getSpecialistInfo(eSpecialist).getCommerceChange(eCommerce) + getSpecialistExtraCommerce(eCommerce) + getSpecialistTypeExtraCommerce(eSpecialist, eCommerce));
//FfH: End Modify

}


CvPlot* CvPlayer::getStartingPlot() const
{
	return GC.getMapINLINE().plotSorenINLINE(m_iStartingX, m_iStartingY);
}


void CvPlayer::setStartingPlot(CvPlot* pNewValue, bool bUpdateStartDist)
{
	CvPlot* pOldStartingPlot;

	pOldStartingPlot = getStartingPlot();

	if (pOldStartingPlot != pNewValue)
	{
		if (pOldStartingPlot != NULL)
		{
			pOldStartingPlot->area()->changeNumStartingPlots(-1);
			pOldStartingPlot->setStartingPlot(false);

			if (bUpdateStartDist)
			{
				GC.getMapINLINE().updateMinOriginalStartDist(pOldStartingPlot->area());
			}
		}

		if (pNewValue == NULL)
		{
			m_iStartingX = INVALID_PLOT_COORD;
			m_iStartingY = INVALID_PLOT_COORD;
		}
		else
		{
			m_iStartingX = pNewValue->getX_INLINE();
			m_iStartingY = pNewValue->getY_INLINE();

			getStartingPlot()->area()->changeNumStartingPlots(1);
			getStartingPlot()->setStartingPlot(true);

			if (bUpdateStartDist)
			{
				GC.getMapINLINE().updateMinOriginalStartDist(getStartingPlot()->area());
			}
		}
	}
}


int CvPlayer::getTotalPopulation() const
{
	return m_iTotalPopulation;
}


int CvPlayer::getAveragePopulation() const
{
	if (getNumCities() == 0)
	{
		return 0;
	}

	return ((getTotalPopulation() / getNumCities()) + 1);
}


void CvPlayer::changeTotalPopulation(int iChange)
{
	changeAssets(-(getPopulationAsset(getTotalPopulation())));
	changePower(-(getPopulationPower(getTotalPopulation())));
	changePopScore(-(getPopulationScore(getTotalPopulation())));

	m_iTotalPopulation = (m_iTotalPopulation + iChange);
	FAssert(getTotalPopulation() >= 0);

	changeAssets(getPopulationAsset(getTotalPopulation()));
	changePower(getPopulationPower(getTotalPopulation()));
	changePopScore(getPopulationScore(getTotalPopulation()));
}


long CvPlayer::getRealPopulation() const
{
	CvCity* pLoopCity;
	__int64 iTotalPopulation = 0;
	int iLoop = 0;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iTotalPopulation += pLoopCity->getRealPopulation();
	}

	if (iTotalPopulation > MAX_INT)
	{
		iTotalPopulation = MAX_INT;
	}

	return ((long)(iTotalPopulation));
}


int CvPlayer::getTotalLand() const
{
	return m_iTotalLand;
}


void CvPlayer::changeTotalLand(int iChange)
{
	m_iTotalLand = (m_iTotalLand + iChange);
	FAssert(getTotalLand() >= 0);
}


int CvPlayer::getTotalLandScored() const
{
	return m_iTotalLandScored;
}


void CvPlayer::changeTotalLandScored(int iChange)
{
	if (iChange != 0)
	{
		changeAssets(-(getLandPlotsAsset(getTotalLandScored())));
		changeLandScore(-(getLandPlotsScore(getTotalLandScored())));

		m_iTotalLandScored = (m_iTotalLandScored + iChange);
		FAssert(getTotalLandScored() >= 0);

		changeAssets(getLandPlotsAsset(getTotalLandScored()));
		changeLandScore(getLandPlotsScore(getTotalLandScored()));
	}
}


int CvPlayer::getGold() const
{
	return m_iGold;
}


void CvPlayer::setGold(int iNewValue)
{
	if (getGold() != iNewValue)
	{
		m_iGold = iNewValue;

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(SelectionButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

void CvPlayer::changeGold(int iChange)
{
	setGold(getGold() + iChange);
}

int CvPlayer::getGoldPerTurn() const
{
	return m_iGoldPerTurn;
}

int CvPlayer::getAdvancedStartPoints() const
{
	return m_iAdvancedStartPoints;
}


void CvPlayer::setAdvancedStartPoints(int iNewValue)
{
	if (getAdvancedStartPoints() != iNewValue)
	{
		m_iAdvancedStartPoints = iNewValue;

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(SelectionButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

void CvPlayer::changeAdvancedStartPoints(int iChange)
{
	setAdvancedStartPoints(getAdvancedStartPoints() + iChange);
}


int CvPlayer::getGoldenAgeTurns() const
{
	return m_iGoldenAgeTurns;
}


bool CvPlayer::isGoldenAge() const
{
	return (getGoldenAgeTurns() > 0);
}


void CvPlayer::changeGoldenAgeTurns(int iChange)
{
	CvWString szBuffer;
	bool bOldGoldenAge;
	int iI;

	if (iChange != 0)
	{
		bOldGoldenAge = isGoldenAge();

		m_iGoldenAgeTurns = (m_iGoldenAgeTurns + iChange);
		FAssert(getGoldenAgeTurns() >= 0);

		if (bOldGoldenAge != isGoldenAge())
		{
			if (isGoldenAge())
			{
				changeAnarchyTurns(-getAnarchyTurns());
			}

			updateYield();

			if (isGoldenAge())
			{
				szBuffer = gDLL->getText("TXT_KEY_MISC_PLAYER_GOLDEN_AGE_BEGINS", getNameKey());
				GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getID(), szBuffer, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));

				CvEventReporter::getInstance().goldenAge(getID());
			}
			else
			{
				CvEventReporter::getInstance().endGoldenAge(getID());
			}

			for (iI = 0; iI < MAX_PLAYERS; iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive())
				{
					if (GET_TEAM(getTeam()).isHasMet(GET_PLAYER((PlayerTypes)iI).getTeam()))
					{
						if (isGoldenAge())
						{
							szBuffer = gDLL->getText("TXT_KEY_MISC_PLAYER_GOLDEN_AGE_HAS_BEGUN", getNameKey());
							gDLL->getInterfaceIFace()->addMessage(((PlayerTypes)iI), (((PlayerTypes)iI) == getID()), GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_GOLDAGESTART", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
						}
						else
						{
							szBuffer = gDLL->getText("TXT_KEY_MISC_PLAYER_GOLDEN_AGE_ENDED", getNameKey());
							gDLL->getInterfaceIFace()->addMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_GOLDAGEEND", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
						}
					}
				}
			}
		}

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

int CvPlayer::getGoldenAgeLength() const
{
	return (GC.getGameINLINE().goldenAgeLength() * std::max(0, 100 + getGoldenAgeModifier())) / 100;
}

int CvPlayer::getNumUnitGoldenAges() const
{
	return m_iNumUnitGoldenAges;
}


void CvPlayer::changeNumUnitGoldenAges(int iChange)
{
	m_iNumUnitGoldenAges = (m_iNumUnitGoldenAges + iChange);
	FAssert(getNumUnitGoldenAges() >= 0);
}


int CvPlayer::getStrikeTurns() const
{
	return m_iStrikeTurns;
}


void CvPlayer::changeStrikeTurns(int iChange)
{
	m_iStrikeTurns = (m_iStrikeTurns + iChange);
	FAssert(getStrikeTurns() >= 0);
}


int CvPlayer::getAnarchyTurns() const
{
	return m_iAnarchyTurns;
}


bool CvPlayer::isAnarchy() const
{
	return (getAnarchyTurns() > 0);
}


void CvPlayer::changeAnarchyTurns(int iChange)
{
	bool bOldAnarchy;

	if (iChange != 0)
	{
		bOldAnarchy = isAnarchy();

		m_iAnarchyTurns = (m_iAnarchyTurns + iChange);
		FAssert(getAnarchyTurns() >= 0);

		if (bOldAnarchy != isAnarchy())
		{
			updateCommerce();
			updateMaintenance();
			updateTradeRoutes();
			updateCorporation();

			AI_makeAssignWorkDirty();

			if (isAnarchy())
			{
				gDLL->getInterfaceIFace()->addMessage(getID(), true, GC.getEVENT_MESSAGE_TIME(), gDLL->getText("TXT_KEY_MISC_REVOLUTION_HAS_BEGUN").GetCString(), "AS2D_REVOLTSTART", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WARNING_TEXT"));
			}
			else
			{
				gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), gDLL->getText("TXT_KEY_MISC_REVOLUTION_OVER").GetCString(), "AS2D_REVOLTEND", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WARNING_TEXT"));
			}

			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
			}

			if (getTeam() == GC.getGameINLINE().getActiveTeam())
			{
				gDLL->getInterfaceIFace()->setDirty(CityInfo_DIRTY_BIT, true);
			}
		}

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getMaxAnarchyTurns() const
{
	return m_iMaxAnarchyTurns;
}


void CvPlayer::updateMaxAnarchyTurns()
{
	int iBestValue;
	int iI;

	iBestValue = GC.getDefineINT("MAX_ANARCHY_TURNS");

	FAssertMsg((GC.getNumTraitInfos() > 0), "GC.getNumTraitInfos() is less than or equal to zero but is expected to be larger than zero in CvPlayer::updateMaxAnarchyTurns");
	for (iI = 0; iI < GC.getNumTraitInfos(); iI++)
	{
		if (hasTrait((TraitTypes)iI))
		{
			if (GC.getTraitInfo((TraitTypes)iI).getMaxAnarchy() >= 0)
			{
				if (GC.getTraitInfo((TraitTypes)iI).getMaxAnarchy() < iBestValue)
				{
					iBestValue = GC.getTraitInfo((TraitTypes)iI).getMaxAnarchy();
				}
			}
		}
	}

	m_iMaxAnarchyTurns = iBestValue;
	FAssert(getMaxAnarchyTurns() >= 0);
}


int CvPlayer::getAnarchyModifier() const
{
	return m_iAnarchyModifier;
}


void CvPlayer::changeAnarchyModifier(int iChange)
{
	if (0 != iChange)
	{
	m_iAnarchyModifier += iChange;

		setRevolutionTimer(std::max(0, ((100 + iChange) * getRevolutionTimer()) / 100));
		setConversionTimer(std::max(0, ((100 + iChange) * getConversionTimer()) / 100));
	}
}


int CvPlayer::getGoldenAgeModifier() const
{
	return m_iGoldenAgeModifier;
}


void CvPlayer::changeGoldenAgeModifier(int iChange)
{
	m_iGoldenAgeModifier += iChange;
}


int CvPlayer::getHurryModifier() const
{
	return m_iGlobalHurryModifier;
}


void CvPlayer::changeHurryModifier(int iChange)
{
	m_iGlobalHurryModifier = (m_iGlobalHurryModifier + iChange);
}


int CvPlayer::getGreatPeopleCreated() const
{
	return m_iGreatPeopleCreated;
}


void CvPlayer::incrementGreatPeopleCreated()
{
	m_iGreatPeopleCreated++;
}

int CvPlayer::getGreatGeneralsCreated() const
{
	return m_iGreatGeneralsCreated;
}

void CvPlayer::incrementGreatGeneralsCreated()
{
	m_iGreatGeneralsCreated++;
}

int CvPlayer::getGreatPeopleThresholdModifier() const
{
	return m_iGreatPeopleThresholdModifier;
}


void CvPlayer::changeGreatPeopleThresholdModifier(int iChange)
{
	m_iGreatPeopleThresholdModifier = (m_iGreatPeopleThresholdModifier + iChange);
}


int CvPlayer::getGreatGeneralsThresholdModifier() const
{
	return m_iGreatGeneralsThresholdModifier;
}


void CvPlayer::changeGreatGeneralsThresholdModifier(int iChange)
{
	m_iGreatGeneralsThresholdModifier += iChange;
}


int CvPlayer::getGreatPeopleRateModifier() const
{
	return m_iGreatPeopleRateModifier;
}


void CvPlayer::changeGreatPeopleRateModifier(int iChange)
{
	m_iGreatPeopleRateModifier = (m_iGreatPeopleRateModifier + iChange);
}


int CvPlayer::getGreatGeneralRateModifier() const
{
	return m_iGreatGeneralRateModifier;
}


void CvPlayer::changeGreatGeneralRateModifier(int iChange)
{
	m_iGreatGeneralRateModifier += iChange;
}


int CvPlayer::getDomesticGreatGeneralRateModifier() const
{
	return (GC.getDefineINT("COMBAT_EXPERIENCE_IN_BORDERS_PERCENT") + m_iDomesticGreatGeneralRateModifier);
}


void CvPlayer::changeDomesticGreatGeneralRateModifier(int iChange)
{
	m_iDomesticGreatGeneralRateModifier += iChange;
}


int CvPlayer::getStateReligionGreatPeopleRateModifier() const
{
	return m_iStateReligionGreatPeopleRateModifier;
}


void CvPlayer::changeStateReligionGreatPeopleRateModifier(int iChange)
{
	m_iStateReligionGreatPeopleRateModifier = (m_iStateReligionGreatPeopleRateModifier + iChange);
}


int CvPlayer::getMaxGlobalBuildingProductionModifier() const
{
	return m_iMaxGlobalBuildingProductionModifier;
}


void CvPlayer::changeMaxGlobalBuildingProductionModifier(int iChange)
{
	m_iMaxGlobalBuildingProductionModifier = (m_iMaxGlobalBuildingProductionModifier + iChange);
}


int CvPlayer::getMaxTeamBuildingProductionModifier() const
{
	return m_iMaxTeamBuildingProductionModifier;
}


void CvPlayer::changeMaxTeamBuildingProductionModifier(int iChange)
{
	m_iMaxTeamBuildingProductionModifier = (m_iMaxTeamBuildingProductionModifier + iChange);
}


int CvPlayer::getMaxPlayerBuildingProductionModifier() const
{
	return m_iMaxPlayerBuildingProductionModifier;
}


void CvPlayer::changeMaxPlayerBuildingProductionModifier(int iChange)
{
	m_iMaxPlayerBuildingProductionModifier = (m_iMaxPlayerBuildingProductionModifier + iChange);
}


int CvPlayer::getFreeExperience() const
{
	return m_iFreeExperience;
}


void CvPlayer::changeFreeExperience(int iChange)
{
	m_iFreeExperience = (m_iFreeExperience + iChange);
}


int CvPlayer::getFeatureProductionModifier() const
{
	return m_iFeatureProductionModifier;
}


void CvPlayer::changeFeatureProductionModifier(int iChange)
{
	m_iFeatureProductionModifier = (m_iFeatureProductionModifier + iChange);
}


int CvPlayer::getWorkerSpeedModifier() const
{
	return m_iWorkerSpeedModifier;
}


void CvPlayer::changeWorkerSpeedModifier(int iChange)
{
	m_iWorkerSpeedModifier = (m_iWorkerSpeedModifier + iChange);
}


int CvPlayer::getImprovementUpgradeRateModifier() const
{
	return m_iImprovementUpgradeRateModifier;
}


void CvPlayer::changeImprovementUpgradeRateModifier(int iChange)
{
	m_iImprovementUpgradeRateModifier = (m_iImprovementUpgradeRateModifier + iChange);
}


int CvPlayer::getMilitaryProductionModifier() const
{
	return m_iMilitaryProductionModifier;
}


void CvPlayer::changeMilitaryProductionModifier(int iChange)
{
	m_iMilitaryProductionModifier = (m_iMilitaryProductionModifier + iChange);
}


int CvPlayer::getSpaceProductionModifier() const
{
	return m_iSpaceProductionModifier;
}


void CvPlayer::changeSpaceProductionModifier(int iChange)
{
	m_iSpaceProductionModifier = (m_iSpaceProductionModifier + iChange);
}


int CvPlayer::getCityDefenseModifier() const
{
	return m_iCityDefenseModifier;
}


void CvPlayer::changeCityDefenseModifier(int iChange)
{
	m_iCityDefenseModifier = (m_iCityDefenseModifier + iChange);
}


int CvPlayer::getNumNukeUnits() const
{
	return m_iNumNukeUnits;
}


void CvPlayer::changeNumNukeUnits(int iChange)
{
	m_iNumNukeUnits = (m_iNumNukeUnits + iChange);
	FAssert(getNumNukeUnits() >= 0);
}


int CvPlayer::getNumOutsideUnits() const
{
	return m_iNumOutsideUnits;
}


void CvPlayer::changeNumOutsideUnits(int iChange)
{
	if (iChange != 0)
	{
		m_iNumOutsideUnits += iChange;
		FAssert(getNumOutsideUnits() >= 0);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getBaseFreeUnits() const
{
	return m_iBaseFreeUnits;
}


void CvPlayer::changeBaseFreeUnits(int iChange)
{
	if (iChange != 0)
	{
		m_iBaseFreeUnits = (m_iBaseFreeUnits + iChange);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getBaseFreeMilitaryUnits() const
{
	return m_iBaseFreeMilitaryUnits;
}


void CvPlayer::changeBaseFreeMilitaryUnits(int iChange)
{
	if (iChange != 0)
	{
		m_iBaseFreeMilitaryUnits = (m_iBaseFreeMilitaryUnits + iChange);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getFreeUnitsPopulationPercent() const
{
	return m_iFreeUnitsPopulationPercent;
}


void CvPlayer::changeFreeUnitsPopulationPercent(int iChange)
{
	if (iChange != 0)
	{
		m_iFreeUnitsPopulationPercent = (m_iFreeUnitsPopulationPercent + iChange);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getFreeMilitaryUnitsPopulationPercent() const
{
	return m_iFreeMilitaryUnitsPopulationPercent;
}


void CvPlayer::changeFreeMilitaryUnitsPopulationPercent(int iChange)
{
	if (iChange != 0)
	{
		m_iFreeMilitaryUnitsPopulationPercent = (m_iFreeMilitaryUnitsPopulationPercent + iChange);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getGoldPerUnit() const
{
	return m_iGoldPerUnit;
}


void CvPlayer::changeGoldPerUnit(int iChange)
{
	if (iChange != 0)
	{
		m_iGoldPerUnit = (m_iGoldPerUnit + iChange);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getGoldPerMilitaryUnit() const
{
	return m_iGoldPerMilitaryUnit;
}


void CvPlayer::changeGoldPerMilitaryUnit(int iChange)
{
	if (iChange != 0)
	{
		m_iGoldPerMilitaryUnit = (m_iGoldPerMilitaryUnit + iChange);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getExtraUnitCost() const
{
	return m_iExtraUnitCost;
}


void CvPlayer::changeExtraUnitCost(int iChange)
{
	if (iChange != 0)
	{
		m_iExtraUnitCost = (m_iExtraUnitCost + iChange);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getNumMilitaryUnits() const
{
	return m_iNumMilitaryUnits;
}


void CvPlayer::changeNumMilitaryUnits(int iChange)
{
	if (iChange != 0)
	{
		m_iNumMilitaryUnits = (m_iNumMilitaryUnits + iChange);
		FAssert(getNumMilitaryUnits() >= 0);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

int CvPlayer::getNumSummonUnits() const
{
	return m_iNumSummonUnits;
}


void CvPlayer::changeNumSummonUnits(int iChange)
{
	if (iChange != 0)
	{
		m_iNumSummonUnits = (m_iNumSummonUnits + iChange);
		FAssert(getNumSummonUnits() >= 0);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

int CvPlayer::getNumGarrisionUnits() const
{
	return m_iNumGarrisionUnits;
}


void CvPlayer::changeNumGarrisionUnits(int iChange)
{
	if (iChange != 0)
	{
		m_iNumGarrisionUnits = (m_iNumGarrisionUnits + iChange);
		FAssert(getNumGarrisionUnits() >= 0);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

int CvPlayer::getHappyPerMilitaryUnit() const
{
	return m_iHappyPerMilitaryUnit;
}


void CvPlayer::changeHappyPerMilitaryUnit(int iChange)
{
	if (iChange != 0)
	{
		m_iHappyPerMilitaryUnit = (m_iHappyPerMilitaryUnit + iChange);

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getMilitaryFoodProductionCount() const
{
	return m_iMilitaryFoodProductionCount;
}


bool CvPlayer::isMilitaryFoodProduction() const
{
	return (getMilitaryFoodProductionCount() > 0);
}


void CvPlayer::changeMilitaryFoodProductionCount(int iChange)
{
	if (iChange != 0)
	{
		m_iMilitaryFoodProductionCount = (m_iMilitaryFoodProductionCount + iChange);
		FAssert(getMilitaryFoodProductionCount() >= 0);

		if (getTeam() == GC.getGameINLINE().getActiveTeam())
		{
			gDLL->getInterfaceIFace()->setDirty(CityInfo_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getHighestUnitLevel()	const
{
	return m_iHighestUnitLevel;
}


void CvPlayer::setHighestUnitLevel(int iNewValue)
{
	m_iHighestUnitLevel = iNewValue;
	FAssert(getHighestUnitLevel() >= 0);
}


int CvPlayer::getMaxConscript() const
{
	return m_iMaxConscript;
}


void CvPlayer::changeMaxConscript(int iChange)
{
	m_iMaxConscript = (m_iMaxConscript + iChange);
	FAssert(getMaxConscript() >= 0);
}


int CvPlayer::getConscriptCount() const
{
	return m_iConscriptCount;
}


void CvPlayer::setConscriptCount(int iNewValue)
{
	m_iConscriptCount = iNewValue;
	FAssert(getConscriptCount() >= 0);
}


void CvPlayer::changeConscriptCount(int iChange)
{
	setConscriptCount(getConscriptCount() + iChange);
}


int CvPlayer::getOverflowResearch() const
{
	return m_iOverflowResearch;
}


void CvPlayer::setOverflowResearch(int iNewValue)
{
	m_iOverflowResearch = iNewValue;
	FAssert(getOverflowResearch() >= 0);
}


void CvPlayer::changeOverflowResearch(int iChange)
{
	setOverflowResearch(getOverflowResearch() + iChange);
}


int CvPlayer::getNoUnhealthyPopulationCount() const
{
	return m_iNoUnhealthyPopulationCount;
}


bool CvPlayer::isNoUnhealthyPopulation() const
{
	return (getNoUnhealthyPopulationCount() > 0);
}


void CvPlayer::changeNoUnhealthyPopulationCount(int iChange)
{
	if (iChange != 0)
	{
		m_iNoUnhealthyPopulationCount = (m_iNoUnhealthyPopulationCount + iChange);
		FAssert(getNoUnhealthyPopulationCount() >= 0);

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getExpInBorderModifier() const
{
	return m_iExpInBorderModifier;
}


void CvPlayer::changeExpInBorderModifier(int iChange)
{
	if (iChange != 0)
	{
		m_iExpInBorderModifier += iChange;
		FAssert(getExpInBorderModifier() >= 0);
	}
}


int CvPlayer::getBuildingOnlyHealthyCount() const
{
	return m_iBuildingOnlyHealthyCount;
}


bool CvPlayer::isBuildingOnlyHealthy() const
{
	return (getBuildingOnlyHealthyCount() > 0);
}


void CvPlayer::changeBuildingOnlyHealthyCount(int iChange)
{
	if (iChange != 0)
	{
		m_iBuildingOnlyHealthyCount = (m_iBuildingOnlyHealthyCount + iChange);
		FAssert(getBuildingOnlyHealthyCount() >= 0);

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getDistanceMaintenanceModifier() const
{
	return m_iDistanceMaintenanceModifier;
}


void CvPlayer::changeDistanceMaintenanceModifier(int iChange)
{
	if (iChange != 0)
	{
		m_iDistanceMaintenanceModifier += iChange;

		updateMaintenance();
	}
}


int CvPlayer::getNumCitiesMaintenanceModifier() const
{
	return m_iNumCitiesMaintenanceModifier;
}


void CvPlayer::changeNumCitiesMaintenanceModifier(int iChange)
{
	if (iChange != 0)
	{
		m_iNumCitiesMaintenanceModifier += iChange;

		updateMaintenance();
	}
}


int CvPlayer::getCorporationMaintenanceModifier() const
{
	return m_iCorporationMaintenanceModifier;
}


void CvPlayer::changeCorporationMaintenanceModifier(int iChange)
{
	if (iChange != 0)
	{
		m_iCorporationMaintenanceModifier += iChange;

		updateMaintenance();
	}
}


int CvPlayer::getTotalMaintenance() const
{
	return m_iTotalMaintenance / 100;
}


void CvPlayer::changeTotalMaintenance(int iChange)
{
	m_iTotalMaintenance += iChange;
	FAssert(m_iTotalMaintenance >= 0);
}


int CvPlayer::getUpkeepModifier() const
{
	return m_iUpkeepModifier;
}


void CvPlayer::changeUpkeepModifier(int iChange)
{
	m_iUpkeepModifier = (m_iUpkeepModifier + iChange);
}


int CvPlayer::getLevelExperienceModifier() const
{
	return m_iLevelExperienceModifier;
}

void CvPlayer::changeLevelExperienceModifier(int iChange)
{
	m_iLevelExperienceModifier += iChange;
}



int CvPlayer::getExtraHealth() const
{
	return m_iExtraHealth;
}


void CvPlayer::changeExtraHealth(int iChange)
{
	if (iChange != 0)
	{
		m_iExtraHealth = (m_iExtraHealth + iChange);

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getBuildingGoodHealth() const
{
	return m_iBuildingGoodHealth;
}


void CvPlayer::changeBuildingGoodHealth(int iChange)
{
	if (iChange != 0)
	{
		m_iBuildingGoodHealth = (m_iBuildingGoodHealth + iChange);
		FAssert(getBuildingGoodHealth() >= 0);

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getBuildingBadHealth() const
{
	return m_iBuildingBadHealth;
}


void CvPlayer::changeBuildingBadHealth(int iChange)
{
	if (iChange != 0)
	{
		m_iBuildingBadHealth = (m_iBuildingBadHealth + iChange);
		FAssert(getBuildingBadHealth() <= 0);

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getExtraHappiness() const
{
	return m_iExtraHappiness;
}


void CvPlayer::changeExtraHappiness(int iChange)
{
	if (iChange != 0)
	{
		m_iExtraHappiness = (m_iExtraHappiness + iChange);

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getBuildingHappiness() const
{
	return m_iBuildingHappiness;
}


void CvPlayer::changeBuildingHappiness(int iChange)
{
	if (iChange != 0)
	{
		m_iBuildingHappiness = (m_iBuildingHappiness + iChange);

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getLargestCityHappiness() const
{
	return m_iLargestCityHappiness;
}


void CvPlayer::changeLargestCityHappiness(int iChange)
{
	if (iChange != 0)
	{
		m_iLargestCityHappiness = (m_iLargestCityHappiness + iChange);

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getWarWearinessPercentAnger() const
{

//FfH: Added by Kael 11/17/2008
	if (GC.getGameINLINE().isOption(GAMEOPTION_NO_WAR_WEARINESS))
	{
	    return 0;
	}
//FfH: End Add

	return m_iWarWearinessPercentAnger;
}


void CvPlayer::updateWarWearinessPercentAnger()
{
	int iNewWarWearinessPercentAnger;
	int iI;

	iNewWarWearinessPercentAnger = 0;

	if (!isBarbarian() && !isMinorCiv())
	{
		for (iI = 0; iI < MAX_CIV_TEAMS; iI++)
		{
			CvTeam& kTeam = GET_TEAM((TeamTypes)iI);
			if (kTeam.isAlive() && !kTeam.isMinorCiv())
			{
				if (kTeam.isAtWar(getTeam()))
				{
					iNewWarWearinessPercentAnger += (GET_TEAM(getTeam()).getWarWeariness((TeamTypes)iI) * std::max(0, 100 + kTeam.getEnemyWarWearinessModifier())) / 10000;
				}
			}
		}
	}

	iNewWarWearinessPercentAnger = getModifiedWarWearinessPercentAnger(iNewWarWearinessPercentAnger);

	if (getWarWearinessPercentAnger() != iNewWarWearinessPercentAnger)
	{
		m_iWarWearinessPercentAnger = iNewWarWearinessPercentAnger;

		AI_makeAssignWorkDirty();
	}
}

int CvPlayer::getModifiedWarWearinessPercentAnger(int iWarWearinessPercentAnger) const
{
	iWarWearinessPercentAnger *= GC.getDefineINT("BASE_WAR_WEARINESS_MULTIPLIER");

	if (GC.getGameINLINE().isOption(GAMEOPTION_ALWAYS_WAR) || GC.getGameINLINE().isOption(GAMEOPTION_NO_CHANGING_WAR_PEACE))
	{
		iWarWearinessPercentAnger *= std::max(0, (GC.getDefineINT("FORCED_WAR_WAR_WEARINESS_MODIFIER") + 100));
		iWarWearinessPercentAnger /= 100;
	}

	if (GC.getGameINLINE().isGameMultiPlayer())
	{
		iWarWearinessPercentAnger *= std::max(0, (GC.getDefineINT("MULTIPLAYER_WAR_WEARINESS_MODIFIER") + 100));
		iWarWearinessPercentAnger /= 100;
	}

	iWarWearinessPercentAnger *= std::max(0, (GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getWarWearinessModifier() + 100));
	iWarWearinessPercentAnger /= 100;

	if (!isHuman() && !isBarbarian() && !isMinorCiv())
	{
		iWarWearinessPercentAnger *= GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIWarWearinessPercent();
		iWarWearinessPercentAnger /= 100;

		iWarWearinessPercentAnger *= std::max(0, ((GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIPerEraModifier() * getCurrentEra()) + 100));
		iWarWearinessPercentAnger /= 100;
	}

	return iWarWearinessPercentAnger;
}


int CvPlayer::getWarWearinessModifier() const
{
	return m_iWarWearinessModifier;
}


void CvPlayer::changeWarWearinessModifier(int iChange)
{
	if (iChange != 0)
	{
		m_iWarWearinessModifier = (m_iWarWearinessModifier + iChange);

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getFreeSpecialist() const
{
	return m_iFreeSpecialist;
}


void CvPlayer::changeFreeSpecialist(int iChange)
{
	if (iChange != 0)
	{
		m_iFreeSpecialist = (m_iFreeSpecialist + iChange);
		FAssert(getFreeSpecialist() >= 0);

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getNoForeignTradeCount() const
{
	return m_iNoForeignTradeCount;
}


bool CvPlayer::isNoForeignTrade() const
{
	return (getNoForeignTradeCount() > 0);
}


void CvPlayer::changeNoForeignTradeCount(int iChange)
{
	if (iChange != 0)
	{
		m_iNoForeignTradeCount = (m_iNoForeignTradeCount + iChange);
		FAssert(getNoForeignTradeCount() >= 0);

		GC.getGameINLINE().updateTradeRoutes();
	}
}


int CvPlayer::getNoCorporationsCount() const
{
	return m_iNoCorporationsCount;
}


bool CvPlayer::isNoCorporations() const
{
	return (getNoCorporationsCount() > 0);
}


void CvPlayer::changeNoCorporationsCount(int iChange)
{
	if (iChange != 0)
	{
		m_iNoCorporationsCount += iChange;
		FAssert(getNoCorporationsCount() >= 0);

		updateCorporation();
	}
}


int CvPlayer::getNoForeignCorporationsCount() const
{
	return m_iNoForeignCorporationsCount;
}


bool CvPlayer::isNoForeignCorporations() const
{
	return (getNoForeignCorporationsCount() > 0);
}


void CvPlayer::changeNoForeignCorporationsCount(int iChange)
{
	if (iChange != 0)
	{
		m_iNoForeignCorporationsCount += iChange;
		FAssert(getNoForeignCorporationsCount() >= 0);

		updateCorporation();
	}
}


int CvPlayer::getCoastalTradeRoutes() const
{
	return m_iCoastalTradeRoutes;
}


void CvPlayer::changeCoastalTradeRoutes(int iChange)
{
	if (iChange != 0)
	{
		m_iCoastalTradeRoutes = (m_iCoastalTradeRoutes + iChange);
		FAssert(getCoastalTradeRoutes() >= 0);

		updateTradeRoutes();
	}
}


int CvPlayer::getTradeRoutes() const
{
	return m_iTradeRoutes;
}


void CvPlayer::changeTradeRoutes(int iChange)
{
	if (iChange != 0)
	{
		m_iTradeRoutes = (m_iTradeRoutes + iChange);
		FAssert(getTradeRoutes() >= 0);

		updateTradeRoutes();
	}
}


int CvPlayer::getRevolutionTimer() const
{
	return m_iRevolutionTimer;
}


void CvPlayer::setRevolutionTimer(int iNewValue)
{
	if (getRevolutionTimer() != iNewValue)
	{
		m_iRevolutionTimer = iNewValue;
		FAssert(getRevolutionTimer() >= 0);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
		}
	}
}


void CvPlayer::changeRevolutionTimer(int iChange)
{
	setRevolutionTimer(getRevolutionTimer() + iChange);
}


int CvPlayer::getConversionTimer() const
{
	return m_iConversionTimer;
}


void CvPlayer::setConversionTimer(int iNewValue)
{
	if (getConversionTimer() != iNewValue)
	{
		m_iConversionTimer = iNewValue;
		FAssert(getConversionTimer() >= 0);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
		}
	}
}


void CvPlayer::changeConversionTimer(int iChange)
{
	setConversionTimer(getConversionTimer() + iChange);
}


int CvPlayer::getStateReligionCount() const
{
	return m_iStateReligionCount;
}


bool CvPlayer::isStateReligion() const
{
	return (getStateReligionCount() > 0);
}


void CvPlayer::changeStateReligionCount(int iChange)
{
	if (iChange != 0)
	{
		// religion visibility now part of espionage
		//GC.getGameINLINE().updateCitySight(false);

		m_iStateReligionCount = (m_iStateReligionCount + iChange);
		FAssert(getStateReligionCount() >= 0);

		// religion visibility now part of espionage
		//GC.getGameINLINE().updateCitySight(true);

		updateMaintenance();
		updateReligionHappiness();
		updateReligionCommerce();

		GC.getGameINLINE().AI_makeAssignWorkDirty();

		gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
	}
}


int CvPlayer::getNoNonStateReligionSpreadCount() const
{
	return m_iNoNonStateReligionSpreadCount;
}


bool CvPlayer::isNoNonStateReligionSpread() const
{
	return (getNoNonStateReligionSpreadCount() > 0);
}


void CvPlayer::changeNoNonStateReligionSpreadCount(int iChange)
{
	m_iNoNonStateReligionSpreadCount = (m_iNoNonStateReligionSpreadCount + iChange);
	FAssert(getNoNonStateReligionSpreadCount() >= 0);
}


int CvPlayer::getStateReligionHappiness() const
{
	return m_iStateReligionHappiness;
}


void CvPlayer::changeStateReligionHappiness(int iChange)
{
	if (iChange != 0)
	{
		m_iStateReligionHappiness = (m_iStateReligionHappiness + iChange);

		updateReligionHappiness();
	}
}


int CvPlayer::getNonStateReligionHappiness() const
{
	return m_iNonStateReligionHappiness;
}


void CvPlayer::changeNonStateReligionHappiness(int iChange)
{
	if (iChange != 0)
	{
		m_iNonStateReligionHappiness = (m_iNonStateReligionHappiness + iChange);

		updateReligionHappiness();
	}
}


int CvPlayer::getStateReligionUnitProductionModifier() const
{
	return m_iStateReligionUnitProductionModifier;
}


void CvPlayer::changeStateReligionUnitProductionModifier(int iChange)
{
	if (iChange != 0)
	{
		m_iStateReligionUnitProductionModifier = (m_iStateReligionUnitProductionModifier + iChange);

		if (getTeam() == GC.getGameINLINE().getActiveTeam())
		{
			gDLL->getInterfaceIFace()->setDirty(CityInfo_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getStateReligionBuildingProductionModifier() const
{
	return m_iStateReligionBuildingProductionModifier;
}


void CvPlayer::changeStateReligionBuildingProductionModifier(int iChange)
{
	if (iChange != 0)
	{
		m_iStateReligionBuildingProductionModifier = (m_iStateReligionBuildingProductionModifier + iChange);

		if (getTeam() == GC.getGameINLINE().getActiveTeam())
		{
			gDLL->getInterfaceIFace()->setDirty(CityInfo_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getStateReligionFreeExperience() const
{
	return m_iStateReligionFreeExperience;
}


void CvPlayer::changeStateReligionFreeExperience(int iChange)
{
	m_iStateReligionFreeExperience = (m_iStateReligionFreeExperience + iChange);
}


CvCity* CvPlayer::getCapitalCity() const
{
	return getCity(m_iCapitalCityID);
}


void CvPlayer::setCapitalCity(CvCity* pNewCapitalCity)
{
	CvCity* pOldCapitalCity;
	bool bUpdatePlotGroups;

	pOldCapitalCity = getCapitalCity();

	if (pOldCapitalCity != pNewCapitalCity)
	{
		bUpdatePlotGroups = ((pOldCapitalCity == NULL) || (pNewCapitalCity == NULL) || (pOldCapitalCity->plot()->getOwnerPlotGroup() != pNewCapitalCity->plot()->getOwnerPlotGroup()));

		if (bUpdatePlotGroups)
		{
			if (pOldCapitalCity != NULL)
			{
				pOldCapitalCity->plot()->updatePlotGroupBonus(false);
			}
			if (pNewCapitalCity != NULL)
			{
				pNewCapitalCity->plot()->updatePlotGroupBonus(false);
			}
		}

		if (pNewCapitalCity != NULL)
		{
			m_iCapitalCityID = pNewCapitalCity->getID();
		}
		else
		{
			m_iCapitalCityID = FFreeList::INVALID_INDEX;
		}

		if (bUpdatePlotGroups)
		{
			if (pOldCapitalCity != NULL)
			{
				pOldCapitalCity->plot()->updatePlotGroupBonus(true);
			}
			if (pNewCapitalCity != NULL)
			{
				pNewCapitalCity->plot()->updatePlotGroupBonus(true);
			}
		}

		updateMaintenance();
		updateTradeRoutes();

		if (pOldCapitalCity != NULL)
		{
			pOldCapitalCity->updateCommerce();

			pOldCapitalCity->setInfoDirty(true);
		}
		if (pNewCapitalCity != NULL)
		{
			pNewCapitalCity->updateCommerce();

			pNewCapitalCity->setInfoDirty(true);
		}
	}
}


int CvPlayer::getCitiesLost() const
{
	return m_iCitiesLost;
}


void CvPlayer::changeCitiesLost(int iChange)
{
	m_iCitiesLost = (m_iCitiesLost + iChange);
}


int CvPlayer::getWinsVsBarbs() const
{
	return m_iWinsVsBarbs;
}


void CvPlayer::changeWinsVsBarbs(int iChange)
{
	m_iWinsVsBarbs = (m_iWinsVsBarbs + iChange);
	FAssert(getWinsVsBarbs() >= 0);
}


int CvPlayer::getAssets() const
{
	return m_iAssets;
}


void CvPlayer::changeAssets(int iChange)
{
	m_iAssets = (m_iAssets + iChange);
	FAssert(getAssets() >= 0);
}


int CvPlayer::getPower() const
{
	return m_iPower;
}


void CvPlayer::changePower(int iChange)
{
	m_iPower = (m_iPower + iChange);
	FAssert(getPower() >= 0);
}


int CvPlayer::getPopScore(bool bCheckVassal) const
{
	if (bCheckVassal && GET_TEAM(getTeam()).isAVassal())
	{
		return m_iPopulationScore / 2;
	}

	int iVassalScore = 0;

	if (bCheckVassal)
	{
		for (int i = 0; i < MAX_CIV_PLAYERS; i++)
		{
			if (i != getID())
			{
				CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)i);
				if (kLoopPlayer.isAlive() && GET_TEAM(kLoopPlayer.getTeam()).isVassal(getTeam()))
				{
					iVassalScore += kLoopPlayer.getPopScore(false) / 2;
				}
			}
		}
	}

	return (m_iPopulationScore + iVassalScore / std::max(1, GET_TEAM(getTeam()).getNumMembers()));
}

void CvPlayer::changePopScore(int iChange)
{
	if (iChange != 0)
	{
		m_iPopulationScore += iChange;
		FAssert(getPopScore() >= 0);

		GC.getGameINLINE().setScoreDirty(true);
	}
}


int CvPlayer::getLandScore(bool bCheckVassal) const
{
	if (bCheckVassal && GET_TEAM(getTeam()).isAVassal())
	{
		return m_iLandScore / 2;
	}

	int iVassalScore = 0;

	if (bCheckVassal)
	{
		for (int i = 0; i < MAX_CIV_PLAYERS; i++)
		{
			if (i != getID())
			{
				CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)i);
				if (kLoopPlayer.isAlive() && GET_TEAM(kLoopPlayer.getTeam()).isVassal(getTeam()))
				{
					iVassalScore += kLoopPlayer.getLandScore(false) / 2;
				}
			}
		}
	}

	return (m_iLandScore + iVassalScore  / std::max(1, GET_TEAM(getTeam()).getNumMembers()));
}


void CvPlayer::changeLandScore(int iChange)
{
	if (iChange != 0)
	{
		m_iLandScore += iChange;
		FAssert(getLandScore() >= 0);

		GC.getGameINLINE().setScoreDirty(true);
	}
}


int CvPlayer::getWondersScore() const
{
	return m_iWondersScore;
}


void CvPlayer::changeWondersScore(int iChange)
{
	if (iChange != 0)
	{
		m_iWondersScore += iChange;
		FAssert(getWondersScore() >= 0);

		GC.getGameINLINE().setScoreDirty(true);
	}
}


int CvPlayer::getTechScore() const
{
	return m_iTechScore;
}


void CvPlayer::changeTechScore(int iChange)
{
	if (iChange != 0)
	{
		m_iTechScore += iChange;
		FAssert(getTechScore() >= 0);

		GC.getGameINLINE().setScoreDirty(true);
	}
}

int CvPlayer::getCombatExperience() const
{
	return m_iCombatExperience;
}

void CvPlayer::setCombatExperience(int iExperience)
{
	FAssert(iExperience >= 0);

	if (iExperience != getCombatExperience())
	{
		m_iCombatExperience = iExperience;

		if (!isBarbarian())
		{
			int iExperienceThreshold = greatPeopleThreshold(true);
			if (m_iCombatExperience >= iExperienceThreshold && iExperienceThreshold > 0)
			{
				// create great person
				CvCity* pBestCity = NULL;
				int iBestValue = MAX_INT;
				int iLoop;
				for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
				{
					int iValue = 4 * GC.getGameINLINE().getSorenRandNum(getNumCities(), "Warlord City Selection");

					for (int i = 0; i < NUM_YIELD_TYPES; i++)
					{
						iValue += pLoopCity->findYieldRateRank((YieldTypes)i);
					}
					iValue += pLoopCity->findPopulationRank();

					if (iValue < iBestValue)
					{
						pBestCity = pLoopCity;
						iBestValue = iValue;
					}
				}

				if (pBestCity)
				{
					int iRandOffset = GC.getGameINLINE().getSorenRandNum(GC.getNumUnitInfos(), "Warlord Unit Generation");
					for (int iI = 0; iI < GC.getNumUnitInfos(); iI++)
					{
						UnitTypes eLoopUnit = (UnitTypes)((iI + iRandOffset) % GC.getNumUnitInfos());
						if (GC.getUnitClassInfo((UnitClassTypes)GC.getUnitInfo(eLoopUnit).getUnitClassType()).isBattleCaptain())
						{
							eLoopUnit=(UnitTypes)GC.getCivilizationInfo(getCivilizationType()).getCivilizationUnits(GC.getUnitInfo(eLoopUnit).getUnitClassType());
							if(eLoopUnit!=NO_UNIT)
							{
								pBestCity->createGreatPeople(eLoopUnit, false, true);
								setCombatExperience(getCombatExperience() - iExperienceThreshold);
								break;
							}
						}
					}
				}
			}
		}
	}
}

void CvPlayer::changeCombatExperience(int iChange)
{
	setCombatExperience(getCombatExperience() + iChange);
}


bool CvPlayer::isConnected() const
{
	return gDLL->isConnected( getNetID() );
}

int CvPlayer::getNetID() const
{
	return GC.getInitCore().getNetID(getID());
}


void CvPlayer::setNetID(int iNetID)
{
	GC.getInitCore().setNetID(getID(), iNetID);
}

void CvPlayer::sendReminder()
{
	CvWString szYearStr;

	// Only perform this step if we have a valid email address on record,
	// and we have provided information about how to send emails
	if ( !getPbemEmailAddress().empty() &&
		!gDLL->GetPitbossSmtpHost().empty() )
	{
		GAMETEXT.setTimeStr(szYearStr, GC.getGameINLINE().getGameTurn(), true);

		// Generate our arguments
		CyArgsList argsList;
		argsList.add(getPbemEmailAddress());
		argsList.add(gDLL->GetPitbossSmtpHost());
		argsList.add(gDLL->GetPitbossSmtpLogin());
		argsList.add(gDLL->GetPitbossSmtpPassword());
		argsList.add(GC.getGameINLINE().getName());
		argsList.add(GC.getGameINLINE().isMPOption(MPOPTION_TURN_TIMER));
		argsList.add(GC.getGameINLINE().getPitbossTurnTime());
		argsList.add(gDLL->GetPitbossEmail());
		argsList.add(szYearStr);

		// Now send our email via Python
		long iResult;
		bool bOK = gDLL->getPythonIFace()->callFunction(PYPitBossModule, "sendEmail", argsList.makeFunctionArgs(), &iResult);

		FAssertMsg( bOK, "Pitboss Python call to onSendEmail failed!" );
		FAssertMsg( iResult == 0, "Pitboss Python fn onSendEmail encountered an error" );
	}
}

uint CvPlayer::getStartTime() const
{
	return m_uiStartTime;
}


void CvPlayer::setStartTime(uint uiStartTime)
{
	m_uiStartTime = uiStartTime;
}


uint CvPlayer::getTotalTimePlayed() const
{
	return ((timeGetTime() - m_uiStartTime)/1000);
}


bool CvPlayer::isMinorCiv() const
{
	return GC.getInitCore().getMinorNationCiv(m_eID);
}


bool CvPlayer::isAlive() const
{
	return m_bAlive;
}


bool CvPlayer::isEverAlive() const
{
	return m_bEverAlive;
}


void CvPlayer::setAlive(bool bNewValue)
{
	CvWString szBuffer;
	int iI;

	if (isAlive() != bNewValue)
	{
		m_bAlive = bNewValue;

		GET_TEAM(getTeam()).changeAliveCount((isAlive()) ? 1 : -1);

/*************************************************************************************************/
/**	SPEEDTWEAK (CAR MOD) merged Sephi                                         					**/
/**																								**/
/**	                                                                 							**/
/*************************************************************************************************/
		GET_TEAM(getTeam()).setHasPlayerMember(getID(), isAlive() ? true : false);
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

		// Report event to Python
		CvEventReporter::getInstance().setPlayerAlive(getID(), bNewValue);

		if (isAlive())
		{
			if (!isEverAlive())
			{
				m_bEverAlive = true;

				GET_TEAM(getTeam()).changeEverAliveCount(1);
			}

			if (getNumCities() == 0)
			{
				setFoundedFirstCity(false);
			}

			updatePlotGroups();

			if (GC.getGameINLINE().isMPOption(MPOPTION_SIMULTANEOUS_TURNS) || (GC.getGameINLINE().getNumGameTurnActive() == 0) || (GC.getGameINLINE().isSimultaneousTeamTurns() && GET_TEAM(getTeam()).isTurnActive()))
			{
				setTurnActive(true);
			}

			gDLL->openSlot(getID());
		}
		else
		{
			clearResearchQueue();
			killUnits();
			killCities();
			killAllDeals();
			killGlobalEnchantments();  //added Sephi
			setTurnActive(false);

			gDLL->endMPDiplomacy();
			gDLL->endDiplomacy();

			if (!isHuman())
			{
				gDLL->closeSlot(getID());
			}

			if (GC.getGameINLINE().getElapsedGameTurns() > 0)
			{
				if (!isBarbarian())
				{
					szBuffer = gDLL->getText("TXT_KEY_MISC_CIV_DESTROYED", getCivilizationAdjectiveKey());

					for (iI = 0; iI < MAX_PLAYERS; iI++)
					{
						if (GET_PLAYER((PlayerTypes)iI).isAlive())
						{
							gDLL->getInterfaceIFace()->addMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_CIVDESTROYED", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WARNING_TEXT"));
						}
					}

					GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getID(), szBuffer, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_WARNING_TEXT"));
				}
			}
		}

		GC.getGameINLINE().setScoreDirty(true);
	}
}


void CvPlayer::verifyAlive()
{
	bool bKill;

	if (isAlive())
	{
		bKill = false;

		if (!bKill)
		{
			if (!isBarbarian())
			{
				if (getNumCities() == 0 && getAdvancedStartPoints() < 0)
				{
					if ((getNumUnits() == 0) || (!(GC.getGameINLINE().isOption(GAMEOPTION_COMPLETE_KILLS)) && isFoundedFirstCity()))
					{
						bKill = true;
					}
				}
			}
		}

		if (!bKill)
		{
			if (!isBarbarian())
			{
				if (GC.getGameINLINE().getMaxCityElimination() > 0)
				{
					if (getCitiesLost() >= GC.getGameINLINE().getMaxCityElimination())
					{
						bKill = true;
					}
				}
			}
		}

		if (bKill)
		{
			setAlive(false);
		}
	}
	else
	{
		if ((getNumCities() > 0) || (getNumUnits() > 0))
		{
			setAlive(true);
		}
	}
}


bool CvPlayer::isTurnActive() const
{
	return m_bTurnActive;
}

void CvPlayer::setTurnActiveForPbem(bool bActive)
{
	FAssertMsg(GC.getGameINLINE().isPbem(), "You are using setTurnActiveForPbem. Are you sure you know what you're doing?");

	// does nothing more than to set the member variable before saving the game
	// the rest of the turn will be performed upon loading the game
	// This allows the player to browse the game in paused mode after he has generated the save
	if (isTurnActive() != bActive)
	{
		m_bTurnActive = bActive;
		GC.getGameINLINE().changeNumGameTurnActive(isTurnActive() ? 1 : -1);
	}
}


void CvPlayer::setTurnActive(bool bNewValue, bool bDoTurn)
{
	int iI;

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Start("CvPlayer::setTurnActive");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	if (isTurnActive() != bNewValue)
	{
		m_bTurnActive = bNewValue;

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/**																								**/
/**																								**/
/*************************************************************************************************/
		if(getID()==0)
		{
			if(!bNewValue)
				GC.getTimeMeasure().Start("AI Turn");
			else if(bNewValue)
				GC.getTimeMeasure().Stop("AI Turn");
		}
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

		if (isTurnActive())
		{
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Start("CvPlayer::setTurnActive::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

			if (GC.getLogging())
			{
				if (gDLL->getChtLvl() > 0)
				{
					TCHAR szOut[1024];
					sprintf(szOut, "Player %d Turn ON\n", getID());
					gDLL->messageControlLog(szOut);
				}
			}

			FAssertMsg(isAlive(), "isAlive is expected to be true");

			setEndTurn(false);
			GC.getGameINLINE().resetTurnTimer();

			// If we are the Pitboss, send this player an email
			if ( gDLL->IsPitbossHost() )
			{
				// If this guy is not currently connected, try sending him an email
				if ( isHuman() && !isConnected() )
				{
					sendReminder();
				}
			}

			if ((GC.getGameINLINE().isHotSeat() || GC.getGameINLINE().isPbem()) && isHuman() && bDoTurn)
			{
				gDLL->getInterfaceIFace()->clearEventMessages();
				gDLL->getEngineIFace()->setResourceLayer(false);

				GC.getGameINLINE().setActivePlayer(getID());
			}

			GC.getGameINLINE().changeNumGameTurnActive(1);
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Stop("CvPlayer::setTurnActive::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Start("CvPlayer::setTurnActive::stuff3");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

			if (bDoTurn)
			{
				if (isAlive() && !isHuman() && !isBarbarian() && (getAdvancedStartPoints() >= 0))
				{
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Start("CvPlayer::setTurnActive::AI_doAdvancedStart");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
					AI_doAdvancedStart();
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Stop("CvPlayer::setTurnActive::AI_doAdvancedStart");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
				}

				if (GC.getGameINLINE().getElapsedGameTurns() > 0)
				{
					if (isAlive())
					{
						if (GC.getGameINLINE().isMPOption(MPOPTION_SIMULTANEOUS_TURNS))
						{
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Start("CvPlayer::setTurnActive::doTurn");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
							doTurn();
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Stop("CvPlayer::setTurnActive::doTurn");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
						}
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Start("CvPlayer::setTurnActive::doTurnUnits");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
						doTurnUnits();
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Stop("CvPlayer::setTurnActive::doTurnUnits");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
					}
				}

				if ((getID() == GC.getGameINLINE().getActivePlayer()) && (GC.getGameINLINE().getElapsedGameTurns() > 0))
				{
					if (GC.getGameINLINE().isNetworkMultiPlayer())
					{
						gDLL->getInterfaceIFace()->addMessage(getID(), true, GC.getEVENT_MESSAGE_TIME(), gDLL->getText("TXT_KEY_MISC_TURN_BEGINS").GetCString(), "AS2D_NEWTURN", MESSAGE_TYPE_DISPLAY_ONLY);
					}
					else
					{
						gDLL->getInterfaceIFace()->playGeneralSound("AS2D_NEWTURN");
					}
				}
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Start("CvPlayer::setTurnActive::doWarnings");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
				doWarnings();
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Stop("CvPlayer::setTurnActive::doWarnings");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
			}
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Stop("CvPlayer::setTurnActive::stuff3");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Start("CvPlayer::setTurnActive::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				if (gDLL->getInterfaceIFace()->getLengthSelectionList() == 0)
				{
					gDLL->getInterfaceIFace()->setCycleSelectionCounter(1);
				}

				gDLL->getInterfaceIFace()->setDirty(SelectionCamera_DIRTY_BIT, true);
			}
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Stop("CvPlayer::setTurnActive::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

		}
		else
		{
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Start("CvPlayer::setTurnActive::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

			if (GC.getLogging())
			{
				if (gDLL->getChtLvl() > 0)
				{
					TCHAR szOut[1024];
					sprintf(szOut, "Player %d Turn OFF\n", getID());
					gDLL->messageControlLog(szOut);
				}
			}

			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				gDLL->getInterfaceIFace()->setForcePopup(false);
				gDLL->getInterfaceIFace()->clearQueuedPopups();
				gDLL->getInterfaceIFace()->flushTalkingHeadMessages();
			}

			// start profiling DLL if desired
			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				startProfilingDLL();
			}

			GC.getGameINLINE().changeNumGameTurnActive(-1);
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Stop("CvPlayer::setTurnActive::zero");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Start("CvPlayer::setTurnActive::stuff7");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

			if (bDoTurn)
			{
				if (!GC.getGameINLINE().isMPOption(MPOPTION_SIMULTANEOUS_TURNS))
				{
					if (isAlive())
					{
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Start("CvPlayer::setTurnActive::stuff8");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
						doTurn();
/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Stop("CvPlayer::setTurnActive::stuff8");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
					}

					if ((GC.getGameINLINE().isPbem() || GC.getGameINLINE().isHotSeat()) && isHuman() && GC.getGameINLINE().countHumanPlayersAlive() > 1)
					{
						GC.getGameINLINE().setHotPbemBetweenTurns(true);
					}

					if (GC.getGameINLINE().isSimultaneousTeamTurns())
					{
						if (!GET_TEAM(getTeam()).isTurnActive())
						{
							for (iI = (getTeam() + 1); iI < MAX_TEAMS; iI++)
							{
								if (GET_TEAM((TeamTypes)iI).isAlive())
								{
									GET_TEAM((TeamTypes)iI).setTurnActive(true);
									break;
								}
							}
						}
					}
					else
					{
						for (iI = (getID() + 1); iI < MAX_PLAYERS; iI++)
						{
							if (GET_PLAYER((PlayerTypes)iI).isAlive())
							{
								if (GC.getGameINLINE().isPbem() && GET_PLAYER((PlayerTypes)iI).isHuman())
								{
									if (!GC.getGameINLINE().getPbemTurnSent())
									{
										gDLL->sendPbemTurn((PlayerTypes)iI);
									}
								}
								else
								{
									GET_PLAYER((PlayerTypes)iI).setTurnActive(true);
								}
								break;
							}
						}
					}
				}
			}

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Stop("CvPlayer::setTurnActive::stuff7");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
		}

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Start("CvPlayer::setTurnActive::stuff8");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

		gDLL->getInterfaceIFace()->updateCursorType();

		gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
			GC.getTimeMeasure().Stop("CvPlayer::setTurnActive::stuff8");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/

	}

/*************************************************************************************************/
/**	TIME MEASURE																	Sephi		**/
/*************************************************************************************************/
	GC.getTimeMeasure().Stop("CvPlayer::setTurnActive");
/*************************************************************************************************/
/**	TIME MEASURE							END													**/
/*************************************************************************************************/
}


bool CvPlayer::isAutoMoves() const
{
	return m_bAutoMoves;
}


void CvPlayer::setAutoMoves(bool bNewValue)
{
	if (isAutoMoves() != bNewValue)
	{
		m_bAutoMoves = bNewValue;

		if (!isAutoMoves())
		{
			if (isEndTurn() || !isHuman())
			{
				setTurnActive(false);
			}
			else
			{
				if (getID() == GC.getGameINLINE().getActivePlayer())
				{
					gDLL->getInterfaceIFace()->setCycleSelectionCounter(1);
				}
			}
		}
	}
}


bool CvPlayer::isEndTurn() const
{
	return m_bEndTurn;
}


void CvPlayer::setEndTurn(bool bNewValue)
{
	if (isEndTurn() != bNewValue)
	{
		FAssertMsg(isTurnActive(), "isTurnActive is expected to be true");

		m_bEndTurn = bNewValue;

		if (isEndTurn())
		{
			setAutoMoves(true);
		}
	}
}

bool CvPlayer::isTurnDone() const
{
	// if this returns true, popups and diplomacy will wait to appear until next turn
	if (!GC.getGameINLINE().isPbem() && !GC.getGameINLINE().isHotSeat())
	{
		return false;
	}
	if (!isHuman() )
	{
		return true;
	}
	if (!isEndTurn())
	{
		return false;
	}
	return (!isAutoMoves());
}

bool CvPlayer::isExtendedGame() const
{
	return m_bExtendedGame;
}


void CvPlayer::makeExtendedGame()
{
	m_bExtendedGame = true;
}


bool CvPlayer::isFoundedFirstCity() const
{
	return m_bFoundedFirstCity;
}


void CvPlayer::setFoundedFirstCity(bool bNewValue)
{
	if (isFoundedFirstCity() != bNewValue)
	{
		m_bFoundedFirstCity = bNewValue;

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(PercentButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(ResearchButtons_DIRTY_BIT, true);
		}
	}
}


bool CvPlayer::isStrike() const
{
	return m_bStrike;
}


void CvPlayer::setStrike(bool bNewValue)
{
	if (isStrike() != bNewValue)
	{
		m_bStrike = bNewValue;

		if (isStrike())
		{
			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), gDLL->getText("TXT_KEY_MISC_UNITS_ON_STRIKE").GetCString(), "AS2D_STRIKE", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WARNING_TEXT"));

				gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
			}
		}
	}
}

PlayerTypes CvPlayer::getID() const
{
	return m_eID;
}


HandicapTypes CvPlayer::getHandicapType() const
{
	return GC.getInitCore().getHandicap(getID());
}


CivilizationTypes CvPlayer::getCivilizationType() const
{
	return GC.getInitCore().getCiv(getID());
}


LeaderHeadTypes CvPlayer::getLeaderType() const
{
	return GC.getInitCore().getLeader(getID());
}


LeaderHeadTypes CvPlayer::getPersonalityType() const
{
	return m_ePersonalityType;
}


void CvPlayer::setPersonalityType(LeaderHeadTypes eNewValue)
{
	m_ePersonalityType = eNewValue;
}


EraTypes CvPlayer::getCurrentEra() const
{
	return m_eCurrentEra;
}



void CvPlayer::setCurrentEra(EraTypes eNewValue)
{
	CvCity* pLoopCity;
	CvUnit* pLoopUnit;
	CvPlot* pLoopPlot;
	int iLoop;
	int iI;

	if (getCurrentEra() != eNewValue)
	{
		EraTypes eOldEra = m_eCurrentEra;
		m_eCurrentEra = eNewValue;

		if (GC.getGameINLINE().getActiveTeam() != NO_TEAM)
		{
			for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
			{
				pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);
				pLoopPlot->updateGraphicEra();

				if (pLoopPlot->getRevealedImprovementType(GC.getGameINLINE().getActiveTeam(), true) != NO_IMPROVEMENT)
				{
					if ((pLoopPlot->getOwnerINLINE() == getID()) || (!(pLoopPlot->isOwned()) && (getID() == GC.getGameINLINE().getActivePlayer())))
					{
						pLoopPlot->setLayoutDirty(true);
					}
				}
			}
		}

		// dirty all of this player's cities...
		for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
		{
			if (pLoopCity->getOwnerINLINE() == getID())
			{
				pLoopCity->setLayoutDirty(true);
			}
		}

		//update unit eras
		for(pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
		{
			gDLL->getEntityIFace()->updateGraphicEra(pLoopUnit->getUnitEntity(), eOldEra);
		}

		//update flag eras
		gDLL->getInterfaceIFace()->setDirty(Flag_DIRTY_BIT, true);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(Soundtrack_DIRTY_BIT, true);
		}

		if (isHuman() && (getCurrentEra() != GC.getGameINLINE().getStartEra()) && !GC.getGameINLINE().isNetworkMultiPlayer())
		{
			if (GC.getGameINLINE().isFinalInitialized() && !(gDLL->GetWorldBuilderMode()))
			{
				CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_PYTHON_SCREEN);
				if (NULL != pInfo)
				{
					pInfo->setData1(eNewValue);
					pInfo->setText(L"showEraMovie");
					addPopup(pInfo);
				}
			}
		}
	}
}


ReligionTypes CvPlayer::getLastStateReligion() const
{
	return m_eLastStateReligion;
}


ReligionTypes CvPlayer::getStateReligion() const
{
	return ((isStateReligion()) ? getLastStateReligion() : NO_RELIGION);
}


void CvPlayer::setLastStateReligion(ReligionTypes eNewValue)
{
	ReligionTypes eOldReligion;
	CvWString szBuffer;
	int iI;

	if (getLastStateReligion() != eNewValue)
	{
		// religion visibility now part of espionage
		//GC.getGameINLINE().updateCitySight(false, true);

//FfH: Added by Kael 11/03/2007
		GC.getGameINLINE().updateCitySight(false);
//FfH: End Add

		eOldReligion = getLastStateReligion();
		m_eLastStateReligion = eNewValue;

		// religion visibility now part of espionage
		//GC.getGameINLINE().updateCitySight(true, true);

//FfH: Added by Kael 11/03/2007
		GC.getGameINLINE().updateCitySight(true);
//FfH: End Add

		updateMaintenance();
		updateReligionHappiness();
		updateReligionCommerce();

		GC.getGameINLINE().updateSecretaryGeneral();

		GC.getGameINLINE().AI_makeAssignWorkDirty();

		gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);

		if (GC.getGameINLINE().isFinalInitialized())
		{
			if (gDLL->isDiplomacy() && (gDLL->getDiplomacyPlayer() == getID()))
			{
				gDLL->updateDiplomacyAttitude(true);
			}

			if (!isBarbarian())
			{
				if (getLastStateReligion() != NO_RELIGION)
				{
					for (iI = 0; iI < MAX_PLAYERS; iI++)
					{
						if (GET_PLAYER((PlayerTypes)iI).isAlive())
						{
							if (GET_TEAM(getTeam()).isHasMet(GET_PLAYER((PlayerTypes)iI).getTeam()))
							{
								szBuffer = gDLL->getText("TXT_KEY_MISC_PLAYER_CONVERT_RELIGION", getNameKey(), GC.getReligionInfo(getLastStateReligion()).getTextKeyWide());
								gDLL->getInterfaceIFace()->addMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_RELIGION_CONVERT", MESSAGE_TYPE_MAJOR_EVENT);
							}
						}
					}

					szBuffer = gDLL->getText("TXT_KEY_MISC_PLAYER_CONVERT_RELIGION", getNameKey(), GC.getReligionInfo(getLastStateReligion()).getTextKeyWide());
					GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getID(), szBuffer);
				}
			}

			// Python Event
			CvEventReporter::getInstance().playerChangeStateReligion(getID(), eNewValue, eOldReligion);

/*************************************************************************************************/
/**	SPEEDTWEAK (CAR MOD) merged Sephi                                         					**/
/**																								**/
/**	                                                                 							**/
/*************************************************************************************************/
			for (int iI = 0; iI < GC.getMAX_PLAYERS(); iI++)
			{
				if (GET_PLAYER((PlayerTypes)iI).isAlive() && GET_PLAYER((PlayerTypes)iI).getStateReligion() != NO_RELIGION)
				{
					GET_PLAYER(getID()).AI_invalidateAttitudeCache((PlayerTypes)iI);
					GET_PLAYER((PlayerTypes)iI).AI_invalidateAttitudeCache(getID());
				}
			}
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
		}
	}
}

PlayerTypes CvPlayer::getParent() const
{
	return m_eParent;
}

void CvPlayer::setParent(PlayerTypes eParent)
{
/*************************************************************************************************/
/**	SPEEDTWEAK (CAR MOD) merged Sephi                                         					**/
/**																								**/
/**	                                                                 							**/
/*************************************************************************************************/
	if (m_eParent != eParent)
	{
		GET_PLAYER(getID()).AI_invalidateAttitudeCache(eParent);
	}
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
	m_eParent = eParent;
}

TeamTypes CvPlayer::getTeam() const
{
	return m_eTeamType;
}

void CvPlayer::updateTeamType()
{
	if(getID() == NO_PLAYER)
	{
		m_eTeamType = NO_TEAM;
	}
	else
	{
		m_eTeamType = GC.getInitCore().getTeam(getID());
	}
}

void CvPlayer::setTeam(TeamTypes eTeam)
{
	FAssert(eTeam != NO_TEAM);
	FAssert(getTeam() != NO_TEAM);

	GET_TEAM(getTeam()).changeNumMembers(-1);
	if (isAlive())
	{
		GET_TEAM(getTeam()).changeAliveCount(-1);
/*************************************************************************************************/
/**	SPEEDTWEAK (CAR MOD) merged Sephi                                         					**/
/**																								**/
/**	                                                                 							**/
/*************************************************************************************************/
		GET_TEAM(getTeam()).setHasPlayerMember(getID(), false);
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
	}
	if (isEverAlive())
	{
		GET_TEAM(getTeam()).changeEverAliveCount(-1);
	}
	GET_TEAM(getTeam()).changeNumCities(-(getNumCities()));
	GET_TEAM(getTeam()).changeTotalPopulation(-(getTotalPopulation()));
	GET_TEAM(getTeam()).changeTotalLand(-(getTotalLand()));

	GC.getInitCore().setTeam(getID(), eTeam);

	GET_TEAM(getTeam()).changeNumMembers(1);
	if (isAlive())
	{
		GET_TEAM(getTeam()).changeAliveCount(1);
/*************************************************************************************************/
/**	SPEEDTWEAK (CAR MOD) merged Sephi                                         					**/
/**																								**/
/**	                                                                 							**/
/*************************************************************************************************/
		GET_TEAM(getTeam()).setHasPlayerMember(getID(), true);
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
	}
	if (isEverAlive())
	{
		GET_TEAM(getTeam()).changeEverAliveCount(1);
	}
	GET_TEAM(getTeam()).changeNumCities(getNumCities());
	GET_TEAM(getTeam()).changeTotalPopulation(getTotalPopulation());
	GET_TEAM(getTeam()).changeTotalLand(getTotalLand());
}


PlayerColorTypes CvPlayer::getPlayerColor() const
{
	return GC.getInitCore().getColor(getID());
}


int CvPlayer::getPlayerTextColorR() const
{
	FAssertMsg(getPlayerColor() != NO_PLAYERCOLOR, "getPlayerColor() is not expected to be equal with NO_PLAYERCOLOR");
	return ((int)(GC.getColorInfo((ColorTypes) GC.getPlayerColorInfo(getPlayerColor()).getTextColorType()).getColor().r * 255));
}


int CvPlayer::getPlayerTextColorG() const
{
	FAssertMsg(getPlayerColor() != NO_PLAYERCOLOR, "getPlayerColor() is not expected to be equal with NO_PLAYERCOLOR");
	return ((int)(GC.getColorInfo((ColorTypes) GC.getPlayerColorInfo(getPlayerColor()).getTextColorType()).getColor().g * 255));
}


int CvPlayer::getPlayerTextColorB() const
{
	FAssertMsg(getPlayerColor() != NO_PLAYERCOLOR, "getPlayerColor() is not expected to be equal with NO_PLAYERCOLOR");
	return ((int)(GC.getColorInfo((ColorTypes) GC.getPlayerColorInfo(getPlayerColor()).getTextColorType()).getColor().b * 255));
}


int CvPlayer::getPlayerTextColorA() const
{
	FAssertMsg(getPlayerColor() != NO_PLAYERCOLOR, "getPlayerColor() is not expected to be equal with NO_PLAYERCOLOR");
	return ((int)(GC.getColorInfo((ColorTypes) GC.getPlayerColorInfo(getPlayerColor()).getTextColorType()).getColor().a * 255));
}


int CvPlayer::getSeaPlotYield(YieldTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_YIELD_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiSeaPlotYield[eIndex];
}


void CvPlayer::changeSeaPlotYield(YieldTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_YIELD_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiSeaPlotYield[eIndex] = (m_aiSeaPlotYield[eIndex] + iChange);

		updateYield();
	}
}


int CvPlayer::getYieldRateModifier(YieldTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_YIELD_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiYieldRateModifier[eIndex];
}


void CvPlayer::changeYieldRateModifier(YieldTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_YIELD_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiYieldRateModifier[eIndex] = (m_aiYieldRateModifier[eIndex] + iChange);

		invalidateYieldRankCache(eIndex);

		if (eIndex == YIELD_COMMERCE)
		{
			updateCommerce();
		}

		AI_makeAssignWorkDirty();

		if (getTeam() == GC.getGameINLINE().getActiveTeam())
		{
			gDLL->getInterfaceIFace()->setDirty(CityInfo_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getCapitalYieldRateModifier(YieldTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_YIELD_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiCapitalYieldRateModifier[eIndex];
}


void CvPlayer::changeCapitalYieldRateModifier(YieldTypes eIndex, int iChange)
{
	CvCity* pCapitalCity;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_YIELD_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiCapitalYieldRateModifier[eIndex] = (m_aiCapitalYieldRateModifier[eIndex] + iChange);

		invalidateYieldRankCache(eIndex);

		pCapitalCity = getCapitalCity();

		if (pCapitalCity != NULL)
		{
			if (eIndex == YIELD_COMMERCE)
			{
				pCapitalCity->updateCommerce();
			}

			pCapitalCity->AI_setAssignWorkDirty(true);

			if (pCapitalCity->getTeam() == GC.getGameINLINE().getActiveTeam())
			{
				pCapitalCity->setInfoDirty(true);
			}
		}
	}
}


int CvPlayer::getExtraYieldThreshold(YieldTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_YIELD_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiExtraYieldThreshold[eIndex];
}


void CvPlayer::updateExtraYieldThreshold(YieldTypes eIndex)
{
	int iBestValue;
	int iI;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_YIELD_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	iBestValue = 0;

	FAssertMsg((GC.getNumTraitInfos() > 0), "GC.getNumTraitInfos() is less than or equal to zero but is expected to be larger than zero in CvPlayer::updateExtraYieldThreshold");
	for (iI = 0; iI < GC.getNumTraitInfos(); iI++)
	{
		if (hasTrait((TraitTypes)iI))
		{
			if (GC.getTraitInfo((TraitTypes) iI).getExtraYieldThreshold(eIndex) > 0)
			{
				if ((iBestValue == 0) || (GC.getTraitInfo((TraitTypes) iI).getExtraYieldThreshold(eIndex) < iBestValue))
				{
					iBestValue = GC.getTraitInfo((TraitTypes) iI).getExtraYieldThreshold(eIndex);
				}
			}
		}
	}

	if (getExtraYieldThreshold(eIndex) != iBestValue)
	{
		m_aiExtraYieldThreshold[eIndex] = iBestValue;
		FAssert(getExtraYieldThreshold(eIndex) >= 0);

		updateYield();
	}
}


int CvPlayer::getTradeYieldModifier(YieldTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_YIELD_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiTradeYieldModifier[eIndex];
}


void CvPlayer::changeTradeYieldModifier(YieldTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_YIELD_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiTradeYieldModifier[eIndex] = (m_aiTradeYieldModifier[eIndex] + iChange);

		updateTradeRoutes();
	}
}


int CvPlayer::getFreeCityCommerce(CommerceTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiFreeCityCommerce[eIndex];
}


void CvPlayer::changeFreeCityCommerce(CommerceTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiFreeCityCommerce[eIndex] = (m_aiFreeCityCommerce[eIndex] + iChange);
		FAssert(getFreeCityCommerce(eIndex) >= 0);

		updateCommerce(eIndex);
	}
}


int CvPlayer::getCommercePercent(CommerceTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiCommercePercent[eIndex];
}


void CvPlayer::setCommercePercent(CommerceTypes eIndex, int iNewValue)
{
	int iTotalCommercePercent;
	int iOldValue;
	int iI;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	iOldValue = getCommercePercent(eIndex);

	m_aiCommercePercent[eIndex] = range(iNewValue, 0, 100);

	if (iOldValue != getCommercePercent(eIndex))
	{
		iTotalCommercePercent = 0;

		for (iI = 0; iI < NUM_COMMERCE_TYPES; iI++)
		{
			iTotalCommercePercent += getCommercePercent((CommerceTypes)iI);
		}

		for (iI = 0; iI < NUM_COMMERCE_TYPES; iI++)
		{
			if (iI != eIndex)
			{
				if (100 != iTotalCommercePercent)
				{
					int iAdjustment = std::min(m_aiCommercePercent[iI], iTotalCommercePercent - 100);
					m_aiCommercePercent[iI] -= iAdjustment;
					iTotalCommercePercent -= iAdjustment;
				}
				else
				{
					break;
				}
			}
		}

		FAssert(100 == iTotalCommercePercent);

		updateCommerce();

		AI_makeAssignWorkDirty();

		if (getTeam() == GC.getGameINLINE().getActiveTeam())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(CityScreen_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(Financial_Screen_DIRTY_BIT, true);
		}
	}
}


void CvPlayer::changeCommercePercent(CommerceTypes eIndex, int iChange)
{
	int iNew=std::min(GC.getDefineINT("MAX_COMMERCE_PERCENT"),(getCommercePercent(eIndex) + iChange));

	iNew=std::max(0,iNew);
	if(eIndex != COMMERCE_RESEARCH && eIndex != COMMERCE_GOLD)
	{
		iNew=std::min(30,iNew);
	}
	//do we reduce too much?
	int iCount=0;
	for(int iI=0;iI<GC.getNUM_COMMERCE_TYPES();iI++)
	{
		if(iI!=COMMERCE_GOLD && iI!=eIndex)
		{
			iCount+=getCommercePercent((CommerceTypes)iI);
		}
	}

	if(iCount+iNew<100-GC.getDefineINT("MAX_COMMERCE_PERCENT_GOLD"))
	{
		iNew=100-GC.getDefineINT("MAX_COMMERCE_PERCENT_GOLD")-iCount;
	}

	setCommercePercent(eIndex,iNew);
	
//	setCommercePercent(eIndex, (getCommercePercent(eIndex) + iChange));
}


int CvPlayer::getCommerceRate(CommerceTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	int iRate = m_aiCommerceRate[eIndex];
	if (GC.getGameINLINE().isOption(GAMEOPTION_NO_ESPIONAGE))
	{
		if (eIndex == COMMERCE_CULTURE)
		{
			iRate += m_aiCommerceRate[COMMERCE_ESPIONAGE];
		}
		else if (eIndex == COMMERCE_ESPIONAGE)
		{
			iRate = 0;
		}
	}

//FfH: Added by Kael 08/28/2008
    if (getDisableResearch() > 0)
    {
        return 0;
    }
//FfH: End Add

	return iRate / 100;
}


void CvPlayer::changeCommerceRate(CommerceTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiCommerceRate[eIndex] += iChange;
//		FAssert(getCommerceRate(eIndex) >= 0);  some buildings give negative commerce Sephi

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

int CvPlayer::getCommerceRateModifier(CommerceTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiCommerceRateModifier[eIndex];
}


void CvPlayer::changeCommerceRateModifier(CommerceTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiCommerceRateModifier[eIndex] = (m_aiCommerceRateModifier[eIndex] + iChange);

		updateCommerce(eIndex);

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getCapitalCommerceRateModifier(CommerceTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiCapitalCommerceRateModifier[eIndex];
}


void CvPlayer::changeCapitalCommerceRateModifier(CommerceTypes eIndex, int iChange)
{
	CvCity* pCapitalCity;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiCapitalCommerceRateModifier[eIndex] = (m_aiCapitalCommerceRateModifier[eIndex] + iChange);

		pCapitalCity = getCapitalCity();

		if (pCapitalCity != NULL)
		{
			pCapitalCity->updateCommerce();

			pCapitalCity->AI_setAssignWorkDirty(true);
		}
	}
}

int CvPlayer::getCommerceChangesPerCultureLevel(CommerceTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiCommerceChangesPerCultureLevel[eIndex];
}


void CvPlayer::changeCommerceChangesPerCultureLevel(CommerceTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiCommerceChangesPerCultureLevel[eIndex] = (m_aiCommerceChangesPerCultureLevel[eIndex] + iChange);

		updateCommerce(eIndex);

		AI_makeAssignWorkDirty();
	}
}

int CvPlayer::getStateReligionBuildingCommerce(CommerceTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiStateReligionBuildingCommerce[eIndex];
}


void CvPlayer::changeStateReligionBuildingCommerce(CommerceTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiStateReligionBuildingCommerce[eIndex] = (m_aiStateReligionBuildingCommerce[eIndex] + iChange);
		FAssert(getStateReligionBuildingCommerce(eIndex) >= 0);

		updateCommerce(eIndex);
	}
}


int CvPlayer::getSpecialistExtraCommerce(CommerceTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiSpecialistExtraCommerce[eIndex];
}


void CvPlayer::changeSpecialistExtraCommerce(CommerceTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiSpecialistExtraCommerce[eIndex] = (m_aiSpecialistExtraCommerce[eIndex] + iChange);
		FAssert(getSpecialistExtraCommerce(eIndex) >= 0);

		updateCommerce(eIndex);

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getCommerceFlexibleCount(CommerceTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiCommerceFlexibleCount[eIndex];
}


bool CvPlayer::isCommerceFlexible(CommerceTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (!isFoundedFirstCity())
	{
		return false;
	}

	if (eIndex == COMMERCE_ESPIONAGE)
	{
		if (0 == GET_TEAM(getTeam()).getHasMetCivCount(true) || GC.getGameINLINE().isOption(GAMEOPTION_NO_ESPIONAGE))
		{
			return false;
		}
	}

	return (GC.getCommerceInfo(eIndex).isFlexiblePercent() || (getCommerceFlexibleCount(eIndex) > 0) || GET_TEAM(getTeam()).isCommerceFlexible(eIndex));
}


void CvPlayer::changeCommerceFlexibleCount(CommerceTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_COMMERCE_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_aiCommerceFlexibleCount[eIndex] = (m_aiCommerceFlexibleCount[eIndex] + iChange);
		FAssert(getCommerceFlexibleCount(eIndex) >= 0);

		if (!isCommerceFlexible(eIndex))
		{
			setCommercePercent(eIndex, 0);
		}

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(PercentButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getGoldPerTurnByPlayer(PlayerTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_PLAYERS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiGoldPerTurnByPlayer[eIndex];
}


void CvPlayer::changeGoldPerTurnByPlayer(PlayerTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_PLAYERS, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_iGoldPerTurn = (m_iGoldPerTurn + iChange);
		m_aiGoldPerTurnByPlayer[eIndex] = (m_aiGoldPerTurnByPlayer[eIndex] + iChange);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}

		if (!isHuman())
		{
			AI_doCommerce();
		}
	}
}


bool CvPlayer::isFeatAccomplished(FeatTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_FEAT_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_abFeatAccomplished[eIndex];
}


void CvPlayer::setFeatAccomplished(FeatTypes eIndex, bool bNewValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_FEAT_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	m_abFeatAccomplished[eIndex] = bNewValue;
}


bool CvPlayer::isOption(PlayerOptionTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_PLAYEROPTION_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_abOptions[eIndex];
}


void CvPlayer::setOption(PlayerOptionTypes eIndex, bool bNewValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < NUM_PLAYEROPTION_TYPES, "eIndex is expected to be within maximum bounds (invalid Index)");
	m_abOptions[eIndex] = bNewValue;
}

bool CvPlayer::isPlayable() const
{
	return GC.getInitCore().getPlayableCiv(getID());
}

void CvPlayer::setPlayable(bool bNewValue)
{
	GC.getInitCore().setPlayableCiv(getID(), bNewValue);
}


int CvPlayer::getBonusExport(BonusTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBonusInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiBonusExport[eIndex];
}


void CvPlayer::changeBonusExport(BonusTypes eIndex, int iChange)
{
	CvCity* pCapitalCity;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBonusInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		pCapitalCity = getCapitalCity();
		if (pCapitalCity != NULL)
		{
			pCapitalCity->plot()->updatePlotGroupBonus(false);
		}
		m_paiBonusExport[eIndex] = (m_paiBonusExport[eIndex] + iChange);
		FAssert(getBonusExport(eIndex) >= 0);
		if (pCapitalCity != NULL)
		{
			pCapitalCity->plot()->updatePlotGroupBonus(true);
		}
	}
}


int CvPlayer::getBonusImport(BonusTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBonusInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiBonusImport[eIndex];
}


void CvPlayer::changeBonusImport(BonusTypes eIndex, int iChange)
{
	CvCity* pCapitalCity;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBonusInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		pCapitalCity = getCapitalCity();
		if (pCapitalCity != NULL)
		{
			pCapitalCity->plot()->updatePlotGroupBonus(false);
		}
		m_paiBonusImport[eIndex] = (m_paiBonusImport[eIndex] + iChange);
		FAssert(getBonusImport(eIndex) >= 0);
		if (pCapitalCity != NULL)
		{
			pCapitalCity->plot()->updatePlotGroupBonus(true);
		}
	}
}


int CvPlayer::getImprovementCount(ImprovementTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumImprovementInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiImprovementCount[eIndex];
}


void CvPlayer::changeImprovementCount(ImprovementTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumImprovementInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	m_paiImprovementCount[eIndex] = (m_paiImprovementCount[eIndex] + iChange);
	FAssert(getImprovementCount(eIndex) >= 0);
}


int CvPlayer::getFreeBuildingCount(BuildingTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiFreeBuildingCount[eIndex];
}


bool CvPlayer::isBuildingFree(BuildingTypes eIndex)	const
{
	return (getFreeBuildingCount(eIndex) > 0);
}


void CvPlayer::changeFreeBuildingCount(BuildingTypes eIndex, int iChange)
{
	CvCity* pLoopCity;
	int iOldFreeBuildingCount;
	int iLoop;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

/*************************************************************************************************/
/**	BUGFIX (CvBuildinginfo::getFreeBuilding works correct for tolerant cities) Sephi			**/
/*************************************************************************************************/
	BuildingClassTypes eBuildingClass=(BuildingClassTypes)GC.getBuildingInfo(eIndex).getBuildingClassType();
/*************************************************************************************************/
/**	BUGFIX									END													**/
/*************************************************************************************************/

	if (iChange != 0)
	{
		iOldFreeBuildingCount = getFreeBuildingCount(eIndex);

		m_paiFreeBuildingCount[eIndex] = (m_paiFreeBuildingCount[eIndex] + iChange);
		FAssert(getFreeBuildingCount(eIndex) >= 0);

		if (iOldFreeBuildingCount == 0)
		{
			FAssertMsg(getFreeBuildingCount(eIndex) > 0, "getFreeBuildingCount(eIndex) is expected to be greater than 0");

			for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
			{
/*************************************************************************************************/
/**	BUGFIX (CvBuildinginfo::getFreeBuilding works correct for tolerant cities) Sephi			**/
/*************************************************************************************************/
/**								---- Start Original Code ----									**
				pLoopCity->setNumFreeBuilding(eIndex, 1);
/**								----  End Original Code  ----									**/
				BuildingTypes eTrueIndex=(BuildingTypes)GC.getCivilizationInfo(pLoopCity->getCivilizationType()).getCivilizationBuildings(eBuildingClass);
				if (eTrueIndex!=NO_BUILDING)
				{
					pLoopCity->setNumFreeBuilding(eIndex, 1);
				}
/*************************************************************************************************/
/**	BUGFIX									END													**/
/*************************************************************************************************/

				pLoopCity->setNumFreeBuilding(eIndex, 1);
			}
		}
		else if (getFreeBuildingCount(eIndex) == 0)
		{
			FAssertMsg(iOldFreeBuildingCount > 0, "iOldFreeBuildingCount is expected to be greater than 0");

			for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
			{
/*************************************************************************************************/
/**	BUGFIX (CvBuildinginfo::getFreeBuilding works correct for tolerant cities) Sephi			**/
/*************************************************************************************************/
/**								---- Start Original Code ----									**
				pLoopCity->setNumFreeBuilding(eIndex, 0);
/**								----  End Original Code  ----									**/
				BuildingTypes eTrueIndex=(BuildingTypes)GC.getCivilizationInfo(pLoopCity->getCivilizationType()).getCivilizationBuildings(eBuildingClass);
				if (eTrueIndex!=NO_BUILDING)
				{
					pLoopCity->setNumFreeBuilding(eIndex, 0);
				}
/*************************************************************************************************/
/**	BUGFIX									END													**/
/*************************************************************************************************/

			}
		}
	}
}


int CvPlayer::getExtraBuildingHappiness(BuildingTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiExtraBuildingHappiness[eIndex];
}


void CvPlayer::changeExtraBuildingHappiness(BuildingTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_paiExtraBuildingHappiness[eIndex] += iChange;

		updateExtraBuildingHappiness();
	}
}

void CvPlayer::setExtraBuildingHappiness(BuildingTypes eIndex, int iValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	m_paiExtraBuildingHappiness[eIndex] = iValue;

	updateExtraBuildingHappiness();
}

int CvPlayer::getExtraBuildingHealth(BuildingTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiExtraBuildingHealth[eIndex];
}


void CvPlayer::changeExtraBuildingHealth(BuildingTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_paiExtraBuildingHealth[eIndex] += iChange;

		updateExtraBuildingHealth();
	}
}

void CvPlayer::setExtraBuildingHealth(BuildingTypes eIndex, int iValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	m_paiExtraBuildingHealth[eIndex] = iValue;

	updateExtraBuildingHealth();
}


int CvPlayer::getFeatureHappiness(FeatureTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumFeatureInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiFeatureHappiness[eIndex];
}


void CvPlayer::changeFeatureHappiness(FeatureTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumFeatureInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_paiFeatureHappiness[eIndex] = (m_paiFeatureHappiness[eIndex] + iChange);

		updateFeatureHappiness();
	}
}


int CvPlayer::getUnitClassCount(UnitClassTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumUnitClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiUnitClassCount[eIndex];
}


bool CvPlayer::isUnitClassMaxedOut(UnitClassTypes eIndex, int iExtra) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumUnitClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (!isNationalUnitClass(eIndex))
	{
		return false;
	}

	FAssertMsg(getUnitClassCount(eIndex) <= GC.getUnitClassInfo(eIndex).getMaxPlayerInstances(), "getUnitClassCount is expected to be less than maximum bound of MaxPlayerInstances (invalid index)");

	return ((getUnitClassCount(eIndex) + iExtra) >= GC.getUnitClassInfo(eIndex).getMaxPlayerInstances());
}


void CvPlayer::changeUnitClassCount(UnitClassTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumUnitClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	m_paiUnitClassCount[eIndex] = (m_paiUnitClassCount[eIndex] + iChange);
	FAssert(getUnitClassCount(eIndex) >= 0);
}


int CvPlayer::getUnitClassMaking(UnitClassTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumUnitClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiUnitClassMaking[eIndex];
}


void CvPlayer::changeUnitClassMaking(UnitClassTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumUnitClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_paiUnitClassMaking[eIndex] = (m_paiUnitClassMaking[eIndex] + iChange);
		FAssert(getUnitClassMaking(eIndex) >= 0);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(Help_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getUnitClassCountPlusMaking(UnitClassTypes eIndex) const
{
	return (getUnitClassCount(eIndex) + getUnitClassMaking(eIndex));
}


int CvPlayer::getBuildingClassCount(BuildingClassTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiBuildingClassCount[eIndex];
}


bool CvPlayer::isBuildingClassMaxedOut(BuildingClassTypes eIndex, int iExtra) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (!isNationalWonderClass(eIndex))
	{
		return false;
	}

	FAssertMsg(getBuildingClassCount(eIndex) <= (GC.getBuildingClassInfo(eIndex).getMaxPlayerInstances() + GC.getBuildingClassInfo(eIndex).getExtraPlayerInstances()), "BuildingClassCount is expected to be less than or match the number of max player instances plus extra player instances");

	return ((getBuildingClassCount(eIndex) + iExtra) >= (GC.getBuildingClassInfo(eIndex).getMaxPlayerInstances() + GC.getBuildingClassInfo(eIndex).getExtraPlayerInstances()));
}


void CvPlayer::changeBuildingClassCount(BuildingClassTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	m_paiBuildingClassCount[eIndex] = (m_paiBuildingClassCount[eIndex] + iChange);
	FAssert(getBuildingClassCount(eIndex) >= 0);
}


int CvPlayer::getBuildingClassMaking(BuildingClassTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiBuildingClassMaking[eIndex];
}


void CvPlayer::changeBuildingClassMaking(BuildingClassTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumBuildingClassInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_paiBuildingClassMaking[eIndex] = (m_paiBuildingClassMaking[eIndex] + iChange);
		FAssert(getBuildingClassMaking(eIndex) >= 0);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(Help_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getBuildingClassCountPlusMaking(BuildingClassTypes eIndex) const
{
	return (getBuildingClassCount(eIndex) + getBuildingClassMaking(eIndex));
}


int CvPlayer::getHurryCount(HurryTypes eIndex) const
{
	FAssert(eIndex >= 0);
	FAssert(eIndex < GC.getNumHurryInfos());
	return m_paiHurryCount[eIndex];
}


bool CvPlayer::canHurry(HurryTypes eIndex) const
{
	//added Sephi - AI will only hurry with stone if it has a lot of stone to spare
	if(!isHuman()) {
		if(GC.getHurryInfo(eIndex).getStonePerProduction() > 0)
		{
			if(getGlobalYield(YIELD_STONE) < 1500)
				return false;
		}
	}
	//end added
	if(getReducedHurryCostBuilding() > 0) {
		if(GC.getHurryInfo(eIndex).getStonePerProduction() > 0)
		{
			return true;
		}
	}

	return (getHurryCount(eIndex) > 0);
}


bool CvPlayer::canPopRush()
{
	return (m_iPopRushHurryCount > 0);
}

void CvPlayer::changeHurryCount(HurryTypes eIndex, int iChange)
{
	FAssert(eIndex >= 0);
	FAssert(eIndex < GC.getNumHurryInfos());

	int oldHurryCount = m_paiHurryCount[eIndex];
	m_paiHurryCount[eIndex] = (m_paiHurryCount[eIndex] + iChange);
	FAssert(getHurryCount(eIndex) >= 0);

	// if we just went from 0 to 1 (or the reverse)
	if ((oldHurryCount > 0) != (m_paiHurryCount[eIndex] > 0))
	{
		// does this hurry reduce population?
		if (GC.getHurryInfo(eIndex).getProductionPerPopulation() > 0)
		{
			m_iPopRushHurryCount += iChange;
			FAssert(m_iPopRushHurryCount >= 0);
		}
	}
}

int CvPlayer::getSpecialBuildingNotRequiredCount(SpecialBuildingTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumSpecialBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiSpecialBuildingNotRequiredCount[eIndex];
}


bool CvPlayer::isSpecialBuildingNotRequired(SpecialBuildingTypes eIndex) const
{
	return (getSpecialBuildingNotRequiredCount(eIndex) > 0);
}


void CvPlayer::changeSpecialBuildingNotRequiredCount(SpecialBuildingTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumSpecialBuildingInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	m_paiSpecialBuildingNotRequiredCount[eIndex] = (m_paiSpecialBuildingNotRequiredCount[eIndex] + iChange);
	FAssert(getSpecialBuildingNotRequiredCount(eIndex) >= 0);
}


int CvPlayer::getHasCivicOptionCount(CivicOptionTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumCivicOptionInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiHasCivicOptionCount[eIndex];
}


bool CvPlayer::isHasCivicOption(CivicOptionTypes eIndex) const
{
	return (getHasCivicOptionCount(eIndex) > 0);
}


void CvPlayer::changeHasCivicOptionCount(CivicOptionTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumCivicOptionInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	m_paiHasCivicOptionCount[eIndex] = (m_paiHasCivicOptionCount[eIndex] + iChange);
	FAssert(getHasCivicOptionCount(eIndex) >= 0);
}


int CvPlayer::getNoCivicUpkeepCount(CivicOptionTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumCivicOptionInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiNoCivicUpkeepCount[eIndex];
}


bool CvPlayer::isNoCivicUpkeep(CivicOptionTypes eIndex) const
{
	return (getNoCivicUpkeepCount(eIndex) > 0);
}


void CvPlayer::changeNoCivicUpkeepCount(CivicOptionTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumCivicOptionInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_paiNoCivicUpkeepCount[eIndex] = (m_paiNoCivicUpkeepCount[eIndex] + iChange);
		FAssert(getNoCivicUpkeepCount(eIndex) >= 0);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getHasReligionCount(ReligionTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumReligionInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paiHasReligionCount[eIndex];
}


int CvPlayer::countTotalHasReligion() const
{
	int iCount;
	int iI;

	iCount = 0;

	for (iI = 0; iI < GC.getNumReligionInfos(); iI++)
	{
		iCount += getHasReligionCount((ReligionTypes)iI);
	}

	return iCount;
}

int CvPlayer::getHasCorporationCount(CorporationTypes eIndex) const
{
	if (!isActiveCorporation(eIndex))
	{
		return 0;
	}

	return m_paiHasCorporationCount[eIndex];
}


int CvPlayer::countTotalHasCorporation() const
{
	int iCount = 0;

	for (int iI = 0; iI < GC.getNumCorporationInfos(); iI++)
	{
		iCount += getHasCorporationCount((CorporationTypes)iI);
	}

	return iCount;
}

bool CvPlayer::isActiveCorporation(CorporationTypes eIndex) const
{
	if (isNoCorporations())
	{
		return false;
	}

	if (isNoForeignCorporations() && !hasHeadquarters(eIndex))
	{
		return false;
	}

	return true;
}


int CvPlayer::findHighestHasReligionCount() const
{
	int iValue;
	int iBestValue;
	int iI;

	iBestValue = 0;

	for (iI = 0; iI < GC.getNumReligionInfos(); iI++)
	{
		iValue = getHasReligionCount((ReligionTypes)iI);

		if (iValue > iBestValue)
		{
			iBestValue = iValue;
		}
	}

	return iBestValue;
}


void CvPlayer::changeHasReligionCount(ReligionTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumReligionInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_paiHasReligionCount[eIndex] = (m_paiHasReligionCount[eIndex] + iChange);
		FAssert(getHasReligionCount(eIndex) >= 0);

		GC.getGameINLINE().updateBuildingCommerce();

		GC.getGameINLINE().AI_makeAssignWorkDirty();
	}
}


void CvPlayer::changeHasCorporationCount(CorporationTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumCorporationInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_paiHasCorporationCount[eIndex] += iChange;
		FAssert(getHasCorporationCount(eIndex) >= 0);

		GC.getGameINLINE().updateBuildingCommerce();

		GC.getGameINLINE().AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getUpkeepCount(UpkeepTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumUpkeepInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(m_paiUpkeepCount != NULL, "m_paiUpkeepCount is not expected to be equal with NULL");
	return m_paiUpkeepCount[eIndex];
}


void CvPlayer::changeUpkeepCount(UpkeepTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumUpkeepInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		FAssertMsg(m_paiUpkeepCount != NULL, "m_paiUpkeepCount is not expected to be equal with NULL");
		m_paiUpkeepCount[eIndex] = (m_paiUpkeepCount[eIndex] + iChange);
		FAssertMsg(getUpkeepCount(eIndex) >= 0, "getUpkeepCount(eIndex) is expected to be non-negative (invalid Index)");

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getSpecialistValidCount(SpecialistTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumSpecialistInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(m_paiSpecialistValidCount != NULL, "m_paiSpecialistValidCount is not expected to be equal with NULL");
	return m_paiSpecialistValidCount[eIndex];
}


bool CvPlayer::isSpecialistValid(SpecialistTypes eIndex) const
{
	return (getSpecialistValidCount(eIndex) > 0);
}


void CvPlayer::changeSpecialistValidCount(SpecialistTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumSpecialistInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		FAssertMsg(m_paiSpecialistValidCount != NULL, "m_paiSpecialistValidCount is not expected to be equal with NULL");
		m_paiSpecialistValidCount[eIndex] = (m_paiSpecialistValidCount[eIndex] + iChange);
		FAssertMsg(getSpecialistValidCount(eIndex) >= 0, "getSpecialistValidCount(eIndex) is expected to be non-negative (invalid Index)");

		AI_makeAssignWorkDirty();
	}
}


bool CvPlayer::isResearchingTech(TechTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumTechInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_pabResearchingTech[eIndex];
}


void CvPlayer::setResearchingTech(TechTypes eIndex, bool bNewValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumTechInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	if (isResearchingTech(eIndex) != bNewValue)
	{
		m_pabResearchingTech[eIndex] = bNewValue;

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(Popup_DIRTY_BIT, true); // to check whether we still need the tech chooser popup
		}
	}
}

bool CvPlayer::isLoyalMember(VoteSourceTypes eVoteSource) const
{
	FAssertMsg(eVoteSource >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eVoteSource < GC.getNumVoteSourceInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_pabLoyalMember[eVoteSource];
}

void CvPlayer::setLoyalMember(VoteSourceTypes eVoteSource, bool bNewValue)
{
	FAssertMsg(eVoteSource >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eVoteSource < MAX_PLAYERS, "eIndex is expected to be within maximum bounds (invalid Index)");

	if (isLoyalMember(eVoteSource) != bNewValue)
	{
		processVoteSourceBonus(eVoteSource, false);
		m_pabLoyalMember[eVoteSource] = bNewValue;
		processVoteSourceBonus(eVoteSource, true);

		GC.getGameINLINE().updateSecretaryGeneral();
	}
}

CivicTypes CvPlayer::getCivics(CivicOptionTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumCivicOptionInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_paeCivics[eIndex];
}


int CvPlayer::getSingleCivicUpkeep(CivicTypes eCivic, bool bIgnoreAnarchy) const
{
	int iUpkeep;

	if (eCivic == NO_CIVIC)
	{
		return 0;
	}

	if (isNoCivicUpkeep((CivicOptionTypes)(GC.getCivicInfo(eCivic).getCivicOptionType())))
	{
		return 0;
	}

	if (GC.getCivicInfo(eCivic).getUpkeep() == NO_UPKEEP)
	{
		return 0;
	}

	if (!bIgnoreAnarchy)
	{
		if (isAnarchy())
		{
			return 0;
		}

//FfH: Added by Kael 10/03/2008
        if (getDisableProduction() > 0)
        {
            return 0;
        }
//FfH: End Add

	}

//added Sephi - simplified Civic Upkeep
	int iUnmodified = 0;
	int iLoop;
	CvCity* pLoopCity;
	for(pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop)) {
		if(!pLoopCity->isSettlement()) {
			iUnmodified += ((3*GC.getCivicInfo(eCivic).getUpkeep()+1) * pLoopCity->getPopulation()) / 10;	
		}
	}

	iUnmodified *= std::max(0, (getUpkeepModifier() + 100));
	iUnmodified /= 100;

	iUnmodified *= GC.getHandicapInfo(getHandicapType()).getCivicUpkeepPercent();
	iUnmodified /= 100;

	if (!isHuman() && !isBarbarian())
	{
		iUnmodified *= GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAICivicUpkeepPercent();
		iUnmodified /= 100;

		iUnmodified *= std::max(0, ((GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIPerEraModifier() * getCurrentEra()) + 100));
		iUnmodified /= 100;
	}

	return iUnmodified;	

//end added Sephi

	iUpkeep = 0;

	iUpkeep += ((std::max(0, (getTotalPopulation() + GC.getDefineINT("UPKEEP_POPULATION_OFFSET") - GC.getCivicInfo(eCivic).getCivicOptionType())) * GC.getUpkeepInfo((UpkeepTypes)(GC.getCivicInfo(eCivic).getUpkeep())).getPopulationPercent()) / 100);

//FfH: Modified by Kael 11/18/2007
//	iUpkeep += ((std::max(0, (getNumCities() + GC.getDefineINT("UPKEEP_CITY_OFFSET") + GC.getCivicInfo(eCivic).getCivicOptionType() - (GC.getNumCivicOptionInfos() / 2))) * GC.getUpkeepInfo((UpkeepTypes)(GC.getCivicInfo(eCivic).getUpkeep())).getCityPercent()) / 100);
	iUpkeep += ((std::max(0, ((getNumCities() - getNumSettlements()) + GC.getDefineINT("UPKEEP_CITY_OFFSET") + GC.getCivicInfo(eCivic).getCivicOptionType() - (GC.getNumCivicOptionInfos() / 2))) * GC.getUpkeepInfo((UpkeepTypes)(GC.getCivicInfo(eCivic).getUpkeep())).getCityPercent()) / 100);
//FfH: End Modify

	iUpkeep *= std::max(0, (getUpkeepModifier() + 100));
	iUpkeep /= 100;

	iUpkeep *= GC.getHandicapInfo(getHandicapType()).getCivicUpkeepPercent();
	iUpkeep /= 100;

	if (!isHuman() && !isBarbarian())
	{
		iUpkeep *= GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAICivicUpkeepPercent();
		iUpkeep /= 100;

		iUpkeep *= std::max(0, ((GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIPerEraModifier() * getCurrentEra()) + 100));
		iUpkeep /= 100;
	}

	return std::max(0, iUpkeep);
}


int CvPlayer::getCivicUpkeep(CivicTypes* paeCivics, bool bIgnoreAnarchy) const
{
	int iTotalUpkeep;
	int iI;

	if (paeCivics == NULL)
	{
		paeCivics = m_paeCivics;
	}

	iTotalUpkeep = 0;

	for (iI = 0; iI < GC.getNumCivicOptionInfos(); iI++)
	{
		iTotalUpkeep += getSingleCivicUpkeep(paeCivics[iI], bIgnoreAnarchy);
	}

	return iTotalUpkeep;
}


void CvPlayer::setCivics(CivicOptionTypes eIndex, CivicTypes eNewValue)
{
	CvWString szBuffer;
	CivicTypes eOldCivic;
	int iI;

	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < GC.getNumCivicOptionInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eNewValue >= 0, "eNewValue is expected to be non-negative (invalid Index)");
	FAssertMsg(eNewValue < GC.getNumCivicInfos(), "eNewValue is expected to be within maximum bounds (invalid Index)");

	eOldCivic = getCivics(eIndex);

	if (eOldCivic != eNewValue)
	{
		m_paeCivics[eIndex] = eNewValue;

		if (eOldCivic != NO_CIVIC)
		{
			processCivics(eOldCivic, -1);
		}
		if (getCivics(eIndex) != NO_CIVIC)
		{
			processCivics(getCivics(eIndex), 1);
		}

/*************************************************************************************************/
/**	ADDON (Houses of Erebus) Sephi			                                 					**/
/**																								**/
/**						                                            							**/
/*************************************************************************************************/
		if (eNewValue!=NO_CIVIC)
		{
			for (int i=0;i<GC.getNumCorporationInfos();i++)
			{
				if (GC.getCivicInfo(eNewValue).getCorporationSupport((CorporationTypes)i)!=0)
				{
					changeCorporationSupport((CorporationTypes)i,GC.getCivicInfo(eNewValue).getCorporationSupport((CorporationTypes)i));
				}
			}
		}

		if (eOldCivic!=NO_CIVIC)
		{
			for (int i=0;i<GC.getNumCorporationInfos();i++)
			{
                if (GC.getCivicInfo(eOldCivic).getCorporationSupport((CorporationTypes)i)!=0)
                {
                    changeCorporationSupport((CorporationTypes)i,-GC.getCivicInfo(eOldCivic).getCorporationSupport((CorporationTypes)i));
                }
			}
		}

		if (eNewValue!=NO_CIVIC)
		{
			for (int i=0;i<GC.getNumCorporationInfos();i++)
			{
                if (GC.getCivicInfo(eNewValue).getCorporationSupportMultiplier((CorporationTypes)i)!=0)
                {
                    changeCorporationSupportMultiplier((CorporationTypes)i,GC.getCivicInfo(eNewValue).getCorporationSupportMultiplier((CorporationTypes)i));
                }
			}
		}

		if (eOldCivic!=NO_CIVIC)
		{
			for (int i=0;i<GC.getNumCorporationInfos();i++)
			{
                if (GC.getCivicInfo(eOldCivic).getCorporationSupportMultiplier((CorporationTypes)i)!=0)
                {
                    changeCorporationSupportMultiplier((CorporationTypes)i,-GC.getCivicInfo(eOldCivic).getCorporationSupportMultiplier((CorporationTypes)i));
                }
			}
		}
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
		GC.getGameINLINE().updateSecretaryGeneral();

		GC.getGameINLINE().AI_makeAssignWorkDirty();

		if (GC.getGameINLINE().isFinalInitialized())
		{
			if (gDLL->isDiplomacy() && (gDLL->getDiplomacyPlayer() == getID()))
			{
				gDLL->updateDiplomacyAttitude(true);
			}

			if (!isBarbarian())
			{
				if (getCivics(eIndex) != NO_CIVIC)
				{
					if (getCivics(eIndex) != GC.getCivilizationInfo(getCivilizationType()).getCivilizationInitialCivics(eIndex))
					{
						for (iI = 0; iI < MAX_PLAYERS; iI++)
						{
							if (GET_PLAYER((PlayerTypes)iI).isAlive())
							{
								if (GET_TEAM(getTeam()).isHasMet(GET_PLAYER((PlayerTypes)iI).getTeam())

//FfH: Added by Kael 11/15/2007
                                  && GET_PLAYER((PlayerTypes)iI).canSeeCivic(eIndex)
//FfH: End Add

                                  )

								{
									szBuffer = gDLL->getText("TXT_KEY_MISC_PLAYER_ADOPTED_CIVIC", getNameKey(), GC.getCivicInfo(getCivics(eIndex)).getTextKeyWide());
									gDLL->getInterfaceIFace()->addMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_CIVIC_ADOPT", MESSAGE_TYPE_MAJOR_EVENT);
								}
							}
						}

						szBuffer = gDLL->getText("TXT_KEY_MISC_PLAYER_ADOPTED_CIVIC", getNameKey(), GC.getCivicInfo(getCivics(eIndex)).getTextKeyWide());
						GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getID(), szBuffer);
					}
				}
			}
		}
/*************************************************************************************************/
/**	SPEEDTWEAK (CAR MOD) merged Sephi                                         					**/
/**																								**/
/**	                                                                 							**/
/*************************************************************************************************/
		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive() && GET_PLAYER((PlayerTypes)iI).getCivics(eIndex) == eNewValue)
			{
				if (GC.getLeaderHeadInfo(GET_PLAYER((PlayerTypes)iI).getLeaderType()).getFavoriteCivic() == eNewValue)
				{
					GET_PLAYER((PlayerTypes)iI).AI_invalidateAttitudeCache(getID());

					if (GC.getLeaderHeadInfo(getLeaderType()).getFavoriteCivic() == eNewValue)
					{
						GET_PLAYER(getID()).AI_invalidateAttitudeCache((PlayerTypes)iI);
					}
				}
			}
		}
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
	}
}


int CvPlayer::getSpecialistExtraYield(SpecialistTypes eIndex1, YieldTypes eIndex2) const
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 expected to be >= 0");
	FAssertMsg(eIndex1 < GC.getNumSpecialistInfos(), "eIndex1 expected to be < GC.getNumSpecialistInfos()");
	FAssertMsg(eIndex2 >= 0, "eIndex2 expected to be >= 0");
	FAssertMsg(eIndex2 < NUM_YIELD_TYPES, "eIndex2 expected to be < NUM_YIELD_TYPES");
	return m_ppaaiSpecialistExtraYield[eIndex1][eIndex2];
}


void CvPlayer::changeSpecialistExtraYield(SpecialistTypes eIndex1, YieldTypes eIndex2, int iChange)
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 expected to be >= 0");
	FAssertMsg(eIndex1 < GC.getNumSpecialistInfos(), "eIndex1 expected to be < GC.getNumSpecialistInfos()");
	FAssertMsg(eIndex2 >= 0, "eIndex2 expected to be >= 0");
	FAssertMsg(eIndex2 < NUM_YIELD_TYPES, "eIndex2 expected to be < NUM_YIELD_TYPES");

	if (iChange != 0)
	{
		m_ppaaiSpecialistExtraYield[eIndex1][eIndex2] = (m_ppaaiSpecialistExtraYield[eIndex1][eIndex2] + iChange);
		FAssert(getSpecialistExtraYield(eIndex1, eIndex2) >= 0);

		updateExtraSpecialistYield();

		AI_makeAssignWorkDirty();
	}
}


int CvPlayer::getImprovementYieldChange(ImprovementTypes eIndex1, YieldTypes eIndex2) const
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumImprovementInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < NUM_YIELD_TYPES, "eIndex2 is expected to be within maximum bounds (invalid Index)");
	return m_ppaaiImprovementYieldChange[eIndex1][eIndex2];
}


void CvPlayer::changeImprovementYieldChange(ImprovementTypes eIndex1, YieldTypes eIndex2, int iChange)
{
	FAssertMsg(eIndex1 >= 0, "eIndex1 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex1 < GC.getNumImprovementInfos(), "eIndex1 is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eIndex2 >= 0, "eIndex2 is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex2 < NUM_YIELD_TYPES, "eIndex2 is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		m_ppaaiImprovementYieldChange[eIndex1][eIndex2] = (m_ppaaiImprovementYieldChange[eIndex1][eIndex2] + iChange);
//		FAssert(getImprovementYieldChange(eIndex1, eIndex2) >= 0); //workshops give negative yields modified Sephi
		updateYield();
	}
}


// XXX should pUnit be a CvSelectionGroup???
void CvPlayer::updateGroupCycle(CvUnit* pUnit)
{
	CLLNode<IDInfo>* pUnitNode;
	CLLNode<int>* pSelectionGroupNode;
	CLLNode<int>* pBestSelectionGroupNode;
	CvSelectionGroup* pLoopSelectionGroup;
	CvUnit* pHeadUnit;
	CvUnit* pBeforeUnit;
	CvUnit* pAfterUnit;
	CvUnit* pLoopUnit;
	CvPlot* pPlot;
	int iValue;
	int iBestValue;

	if (!(pUnit->onMap()))
	{
		return;
	}

	FAssertMsg(pUnit->getGroup() != NULL, "Unit->getGroup() is not assigned a valid value");

	removeGroupCycle(pUnit->getGroupID());

	pPlot = pUnit->plot();

	pBeforeUnit = NULL;
	pAfterUnit = NULL;

	pUnitNode = pPlot->headUnitNode();

	while (pUnitNode != NULL)
	{
		pLoopUnit = ::getUnit(pUnitNode->m_data);
		pUnitNode = pPlot->nextUnitNode(pUnitNode);

		if (pLoopUnit->isGroupHead())
		{
			if (pLoopUnit != pUnit)
			{
				if (!isBeforeUnitCycle(pLoopUnit, pUnit))
				{
					pBeforeUnit = pLoopUnit;
					break;
				}
				else
				{
					pAfterUnit = pLoopUnit;
				}
			}
		}
	}

	pSelectionGroupNode = headGroupCycleNode();

	iBestValue = MAX_INT;
	pBestSelectionGroupNode = NULL;

	while (pSelectionGroupNode != NULL)
	{
		pLoopSelectionGroup = getSelectionGroup(pSelectionGroupNode->m_data);
		FAssertMsg(pLoopSelectionGroup != NULL, "LoopSelectionGroup is not assigned a valid value");

		pHeadUnit = pLoopSelectionGroup->getHeadUnit();

		if (pHeadUnit != NULL)
		{
			if (pBeforeUnit != NULL)
			{
				if (pBeforeUnit == pHeadUnit)
				{
					pBestSelectionGroupNode = pSelectionGroupNode;
					break;
				}
			}
			else if (pAfterUnit != NULL)
			{
				if (pAfterUnit == pHeadUnit)
				{
					pBestSelectionGroupNode = nextGroupCycleNode(pSelectionGroupNode);
					break;
				}
			}
			else
			{
				iValue = plotDistance(pUnit->getX_INLINE(), pUnit->getY_INLINE(), pHeadUnit->getX_INLINE(), pHeadUnit->getY_INLINE());

				if (iValue < iBestValue)
				{
					iBestValue = iValue;
					pBestSelectionGroupNode = pSelectionGroupNode;
				}
			}
		}

		pSelectionGroupNode = nextGroupCycleNode(pSelectionGroupNode);
	}

	if (pBestSelectionGroupNode != NULL)
	{
		m_groupCycle.insertBefore(pUnit->getGroupID(), pBestSelectionGroupNode);
	}
	else
	{
		m_groupCycle.insertAtEnd(pUnit->getGroupID());
	}
}


void CvPlayer::removeGroupCycle(int iID)
{
	CLLNode<int>* pSelectionGroupNode;

	pSelectionGroupNode = headGroupCycleNode();

	while (pSelectionGroupNode != NULL)
	{
		if (pSelectionGroupNode->m_data == iID)
		{
			pSelectionGroupNode = deleteGroupCycleNode(pSelectionGroupNode);
			break;
		}
		else
		{
			pSelectionGroupNode = nextGroupCycleNode(pSelectionGroupNode);
		}
	}
}


CLLNode<int>* CvPlayer::deleteGroupCycleNode(CLLNode<int>* pNode)
{
	return m_groupCycle.deleteNode(pNode);
}


CLLNode<int>* CvPlayer::nextGroupCycleNode(CLLNode<int>* pNode) const
{
	return m_groupCycle.next(pNode);
}


CLLNode<int>* CvPlayer::previousGroupCycleNode(CLLNode<int>* pNode) const
{
	return m_groupCycle.prev(pNode);
}


CLLNode<int>* CvPlayer::headGroupCycleNode() const
{
	return m_groupCycle.head();
}


CLLNode<int>* CvPlayer::tailGroupCycleNode() const
{
	return m_groupCycle.tail();
}


//	Finds the path length from this tech type to one you already know
int CvPlayer::findPathLength(TechTypes eTech, bool bCost) const
{
	int i;
	int iNumSteps = 0;
	int iShortestPath = 0;
	int iPathLength = 0;
	TechTypes ePreReq;
	TechTypes eShortestOr;

	if (GET_TEAM(getTeam()).isHasTech(eTech) || isResearchingTech(eTech))
	{
		//	We have this tech, no reason to add this to the pre-reqs
		//	Base case return 0, we know it...
		return 0;
	}

	//	Cycle through the and paths and add up their tech lengths
	for (i = 0; i < GC.getNUM_AND_TECH_PREREQS(); i++)
	{
		ePreReq = (TechTypes)GC.getTechInfo(eTech).getPrereqAndTechs(i);

		if (ePreReq != NO_TECH)
		{
			iPathLength += findPathLength(ePreReq, bCost);
		}
	}

	eShortestOr = NO_TECH;
	iShortestPath = MAX_INT;
	//	Find the shortest OR tech
	for (i = 0; i < GC.getNUM_OR_TECH_PREREQS(); i++)
	{
		//	Grab the tech
		ePreReq = (TechTypes)GC.getTechInfo(eTech).getPrereqOrTechs(i);

		//	If this is a valid tech
		if (ePreReq != NO_TECH)
		{
			//	Recursively find the path length (takes into account all ANDs)
			iNumSteps = findPathLength(ePreReq, bCost);

			//	If the prereq is a valid tech and its the current shortest, mark it as such
			if (iNumSteps < iShortestPath)
			{
				eShortestOr = ePreReq;
				iShortestPath = iNumSteps;
			}
		}
	}

	//	If the shortest OR is a valid tech, add the steps to it...
	if (eShortestOr != NO_TECH)
	{
		iPathLength += iShortestPath;
	}

	return (iPathLength + ((bCost) ? GET_TEAM(getTeam()).getResearchCost(eTech) : 1));
}


//	Function specifically for python/tech chooser screen
int CvPlayer::getQueuePosition(TechTypes eTech) const
{
	int i = 1;
	CLLNode<TechTypes>* pResearchNode;

	for (pResearchNode = headResearchQueueNode(); pResearchNode; pResearchNode = nextResearchQueueNode(pResearchNode))
	{
		if (pResearchNode->m_data == eTech)
		{
			return i;
		}
		i++;
	}

	return -1;
}


void CvPlayer::clearResearchQueue()
{
	int iI;

	m_researchQueue.clear();

	for (iI = 0; iI < GC.getNumTechInfos(); iI++)
	{
		setResearchingTech(((TechTypes)iI), false);
	}

	if (getTeam() == GC.getGameINLINE().getActiveTeam())
	{
		gDLL->getInterfaceIFace()->setDirty(ResearchButtons_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
	}
}


//	Pushes research onto the queue.  If it is an append if will put it
//	and its pre-reqs into the queue.  If it is not an append it will change
//	research immediately and should be used with clear.  Clear will clear the entire queue.
bool CvPlayer::pushResearch(TechTypes eTech, bool bClear)
{
	int i;
	int iNumSteps;
	int iShortestPath;
	bool bOrPrereqFound;
	TechTypes ePreReq;
    TechTypes eShortestOr;

	FAssertMsg(eTech != NO_TECH, "Tech is not assigned a valid value");

	if (GET_TEAM(getTeam()).isHasTech(eTech) || isResearchingTech(eTech))
	{
		//	We have this tech, no reason to add this to the pre-reqs
		return true;
	}

	if (!canEverResearch(eTech))
	{
		return false;
	}

	//	Pop the entire queue...
	if (bClear)
	{
		clearResearchQueue();
	}

	//	Add in all the pre-reqs for the and techs...
	for (i = 0; i < GC.getNUM_AND_TECH_PREREQS(); i++)
	{
		ePreReq = (TechTypes)GC.getTechInfo(eTech).getPrereqAndTechs(i);

		if (ePreReq != NO_TECH)
		{
			if (!pushResearch(ePreReq))
			{
				return false;
			}
		}
	}

	// Will return the shortest path of all the or techs.  Tie breaker goes to the first one...
	eShortestOr = NO_TECH;
	iShortestPath = MAX_INT;
	bOrPrereqFound = false;
	//	Cycle through all the OR techs
	for (i = 0; i < GC.getNUM_OR_TECH_PREREQS(); i++)
	{
		ePreReq = (TechTypes)GC.getTechInfo(eTech).getPrereqOrTechs(i);

		if (ePreReq != NO_TECH)
		{
			bOrPrereqFound = true;

			//	If the pre-req exists, and we have it, it is the shortest path, get out, we're done
			if (GET_TEAM(getTeam()).isHasTech(ePreReq))
			{
				eShortestOr = ePreReq;
				break;
			}

			if (canEverResearch(ePreReq))
			{
				//	Find the length of the path to this pre-req
				iNumSteps = findPathLength(ePreReq);

				//	If this pre-req is a valid tech, and its the shortest current path, set it as such
				if (iNumSteps < iShortestPath)
				{
					eShortestOr = ePreReq;
					iShortestPath = iNumSteps;
				}
			}
		}
	}

	//	If the shortest path tech is valid, push it (and its children) on to the research queue recursively
	if (eShortestOr != NO_TECH)
	{
		if (!pushResearch(eShortestOr))
		{
			return false;
		}
	}
	else if (bOrPrereqFound)
	{
		return false;
	}

	//	Insert this tech at the end of the queue
	m_researchQueue.insertAtEnd(eTech);

	setResearchingTech(eTech, true);

	//	Set the dirty bits
	if (getTeam() == GC.getGameINLINE().getActiveTeam())
	{
		gDLL->getInterfaceIFace()->setDirty(ResearchButtons_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
	}

	// ONEVENT - Tech selected (any)
	CvEventReporter::getInstance().techSelected(eTech, getID());

	return true;
}


//	If bHead is true we delete the entire queue...
void CvPlayer::popResearch(TechTypes eTech)
{
	CLLNode<TechTypes>* pResearchNode;

	for (pResearchNode = headResearchQueueNode(); pResearchNode; pResearchNode = nextResearchQueueNode(pResearchNode))
	{
		if (pResearchNode->m_data == eTech)
		{
			m_researchQueue.deleteNode(pResearchNode);
			break;
		}
	}

	setResearchingTech(eTech, false);

	if (getTeam() == GC.getGameINLINE().getActiveTeam())
	{
		gDLL->getInterfaceIFace()->setDirty(ResearchButtons_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
	}
}


int CvPlayer::getLengthResearchQueue() const
{
	return m_researchQueue.getLength();
}


CLLNode<TechTypes>* CvPlayer::nextResearchQueueNode(CLLNode<TechTypes>* pNode) const
{
	return m_researchQueue.next(pNode);
}


CLLNode<TechTypes>* CvPlayer::headResearchQueueNode() const
{
	return m_researchQueue.head();
}


CLLNode<TechTypes>* CvPlayer::tailResearchQueueNode() const
{
	return m_researchQueue.tail();
}


void CvPlayer::addCityName(const CvWString& szName)
{
	m_cityNames.insertAtEnd(szName);
}


int CvPlayer::getNumCityNames() const
{
	return m_cityNames.getLength();
}


CvWString CvPlayer::getCityName(int iIndex) const
{
	CLLNode<CvWString>* pCityNameNode;

	pCityNameNode = m_cityNames.nodeNum(iIndex);

	if (pCityNameNode != NULL)
	{
		return pCityNameNode->m_data;
	}
	else
	{
		return L"";
	}
}


CLLNode<CvWString>* CvPlayer::nextCityNameNode(CLLNode<CvWString>* pNode) const
{
	return m_cityNames.next(pNode);
}


CLLNode<CvWString>* CvPlayer::headCityNameNode() const
{
	return m_cityNames.head();
}


CvPlotGroup* CvPlayer::firstPlotGroup(int *pIterIdx, bool bRev) const
{
	return !bRev ? m_plotGroups.beginIter(pIterIdx) : m_plotGroups.endIter(pIterIdx);
}


CvPlotGroup* CvPlayer::nextPlotGroup(int *pIterIdx, bool bRev) const
{
	return !bRev ? m_plotGroups.nextIter(pIterIdx) : m_plotGroups.prevIter(pIterIdx);
}


int CvPlayer::getNumPlotGroups() const
{
	return m_plotGroups.getCount();
}


CvPlotGroup* CvPlayer::getPlotGroup(int iID) const
{
	return((CvPlotGroup *)(m_plotGroups.getAt(iID)));
}


CvPlotGroup* CvPlayer::addPlotGroup()
{
	return((CvPlotGroup *)(m_plotGroups.add()));
}


void CvPlayer::deletePlotGroup(int iID)
{
	m_plotGroups.removeAt(iID);
}


CvCity* CvPlayer::firstCity(int *pIterIdx, bool bRev) const
{
	return !bRev ? m_cities.beginIter(pIterIdx) : m_cities.endIter(pIterIdx);
}


CvCity* CvPlayer::nextCity(int *pIterIdx, bool bRev) const
{
	return !bRev ? m_cities.nextIter(pIterIdx) : m_cities.prevIter(pIterIdx);
}


int CvPlayer::getNumCities() const
{
	return m_cities.getCount();
}


CvCity* CvPlayer::getCity(int iID) const
{
	return(m_cities.getAt(iID));
}


CvCity* CvPlayer::addCity()
{
	return(m_cities.add());
}


void CvPlayer::deleteCity(int iID)
{
	m_cities.removeAt(iID);
}


CvUnit* CvPlayer::firstUnit(int *pIterIdx, bool bRev) const
{
	return !bRev ? m_units.beginIter(pIterIdx) : m_units.endIter(pIterIdx);
}


CvUnit* CvPlayer::nextUnit(int *pIterIdx, bool bRev) const
{
	return !bRev ? m_units.nextIter(pIterIdx) : m_units.prevIter(pIterIdx);
}


int CvPlayer::getNumUnits() const
{
	return m_units.getCount();
}


CvUnit* CvPlayer::getUnit(int iID) const
{
	return (m_units.getAt(iID));
}


CvUnit* CvPlayer::addUnit()
{
	return (m_units.add());
}


void CvPlayer::deleteUnit(int iID)
{
	m_units.removeAt(iID);
}


CvSelectionGroup* CvPlayer::firstSelectionGroup(int *pIterIdx, bool bRev) const
{
	return !bRev ? m_selectionGroups.beginIter(pIterIdx) : m_selectionGroups.endIter(pIterIdx);
}


CvSelectionGroup* CvPlayer::nextSelectionGroup(int *pIterIdx, bool bRev) const
{
	return !bRev ? m_selectionGroups.nextIter(pIterIdx) : m_selectionGroups.prevIter(pIterIdx);
}


int CvPlayer::getNumSelectionGroups() const
{
	return m_selectionGroups.getCount();
}


CvSelectionGroup* CvPlayer::getSelectionGroup(int iID) const
{
	return ((CvSelectionGroup *)(m_selectionGroups.getAt(iID)));
}


CvSelectionGroup* CvPlayer::addSelectionGroup()
{
	return ((CvSelectionGroup *)(m_selectionGroups.add()));
}


void CvPlayer::deleteSelectionGroup(int iID)
{
	bool bRemoved = m_selectionGroups.removeAt(iID);

	FAssertMsg(bRemoved, "could not find group, delete failed");
}

EventTriggeredData* CvPlayer::firstEventTriggered(int *pIterIdx, bool bRev) const
{
	return !bRev ? m_eventsTriggered.beginIter(pIterIdx) : m_eventsTriggered.endIter(pIterIdx);
}

EventTriggeredData* CvPlayer::nextEventTriggered(int *pIterIdx, bool bRev) const
{
	return !bRev ? m_eventsTriggered.nextIter(pIterIdx) : m_eventsTriggered.prevIter(pIterIdx);
}

int CvPlayer::getNumEventsTriggered() const
{
	return m_eventsTriggered.getCount();
}

EventTriggeredData* CvPlayer::getEventTriggered(int iID) const
{
	return ((EventTriggeredData*)(m_eventsTriggered.getAt(iID)));
}

EventTriggeredData* CvPlayer::addEventTriggered()
{
	return ((EventTriggeredData*)(m_eventsTriggered.add()));
}

void CvPlayer::deleteEventTriggered(int iID)
{
	m_eventsTriggered.removeAt(iID);
}


void CvPlayer::addMessage(const CvTalkingHeadMessage& message)
{
	m_listGameMessages.push_back(message);
}


void CvPlayer::clearMessages()
{
	m_listGameMessages.clear();
}


const CvMessageQueue& CvPlayer::getGameMessages() const
{
	return (m_listGameMessages);
}


void CvPlayer::expireMessages()
{
	CvMessageQueue::iterator it = m_listGameMessages.begin();
	bool bFoundExpired = false;
	while(it != m_listGameMessages.end())
	{
		CvTalkingHeadMessage& message = *it;
		if (GC.getGameINLINE().getGameTurn() >= message.getExpireTurn())
		{
			it = m_listGameMessages.erase(it);
			bFoundExpired = true;
		}
		else
		{
			++it;
		}
	}
	if (bFoundExpired)
	{
		gDLL->getInterfaceIFace()->dirtyTurnLog(getID());
	}
}


void CvPlayer::addPopup(CvPopupInfo* pInfo, bool bFront)
{
	if (isHuman())
	{
		if (bFront)
		{
			m_listPopups.push_front(pInfo);
		}
		else
		{
			m_listPopups.push_back(pInfo);
		}
	}
	else
	{
		SAFE_DELETE(pInfo);
	}
}


void CvPlayer::clearPopups()
{
	CvPopupQueue::iterator it;
	for (it = m_listPopups.begin(); it != m_listPopups.end(); ++it)
	{
		CvPopupInfo* pInfo = *it;
		if (NULL != pInfo)
		{
			delete pInfo;
		}
	}
	m_listPopups.clear();
}


CvPopupInfo* CvPlayer::popFrontPopup()
{
	CvPopupInfo* pInfo = NULL;
	if (!m_listPopups.empty())
	{
		pInfo = m_listPopups.front();
		m_listPopups.pop_front();
	}
	return pInfo;
}


const CvPopupQueue& CvPlayer::getPopups() const
{
	return (m_listPopups);
}


void CvPlayer::addDiplomacy(CvDiploParameters* pDiplo)
{
	if (NULL != pDiplo)
	{
		m_listDiplomacy.push_back(pDiplo);
	}
}


void CvPlayer::clearDiplomacy()
{
	CvDiploQueue::iterator it;
	for (it = m_listDiplomacy.begin(); it != m_listDiplomacy.end(); ++it)
	{
		CvDiploParameters* pDiplo = *it;
		if (NULL != pDiplo)
		{
			delete pDiplo;
		}
	}
	m_listDiplomacy.clear();
}


const CvDiploQueue& CvPlayer::getDiplomacy() const
{
	return (m_listDiplomacy);
}


CvDiploParameters* CvPlayer::popFrontDiplomacy()
{
	CvDiploParameters* pDiplo = NULL;
	if (!m_listDiplomacy.empty())
	{
		pDiplo = m_listDiplomacy.front();
		m_listDiplomacy.pop_front();
	}
	return pDiplo;
}


void CvPlayer::showSpaceShip()
{
	CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_PYTHON_SCREEN);
	pInfo->setData1(-1);
	pInfo->setText(L"showSpaceShip");
	addPopup(pInfo);
}

void CvPlayer::clearSpaceShipPopups()
{
	//clear all spaceship popups
	CvPopupQueue::iterator it;
	for (it = m_listPopups.begin(); it != m_listPopups.end(); )
	{
		CvPopupInfo* pInfo = *it;
		if (NULL != pInfo)
		{
			if(pInfo->getText().compare(L"showSpaceShip") == 0)
			{
				it = m_listPopups.erase(it);
				SAFE_DELETE(pInfo);
			}
			else
			{
				it++;
			}
		}
		else
		{
			it++;
		}
	}
}

int CvPlayer::getScoreHistory(int iTurn) const
{
	CvTurnScoreMap::const_iterator it = m_mapScoreHistory.find(iTurn);
	if (it != m_mapScoreHistory.end())
	{
		return it->second;
	}
	return 0;
}

void CvPlayer::updateScoreHistory(int iTurn, int iBestScore)
{
	m_mapScoreHistory[iTurn] = iBestScore;
}

int CvPlayer::getEconomyHistory(int iTurn) const
{
	CvTurnScoreMap::const_iterator it = m_mapEconomyHistory.find(iTurn);
	if (it != m_mapEconomyHistory.end())
	{
		return it->second;
	}
	return 0;
}

void CvPlayer::updateEconomyHistory(int iTurn, int iBestEconomy)
{
	m_mapEconomyHistory[iTurn] = iBestEconomy;
}

int CvPlayer::getIndustryHistory(int iTurn) const
{
	CvTurnScoreMap::const_iterator it = m_mapIndustryHistory.find(iTurn);
	if (it != m_mapIndustryHistory.end())
	{
		return it->second;
	}
	return 0;
}

void CvPlayer::updateIndustryHistory(int iTurn, int iBestIndustry)
{
	m_mapIndustryHistory[iTurn] = iBestIndustry;
}

int CvPlayer::getAgricultureHistory(int iTurn) const
{
	CvTurnScoreMap::const_iterator it = m_mapAgricultureHistory.find(iTurn);
	if (it != m_mapAgricultureHistory.end())
	{
		return it->second;
	}
	return 0;
}

void CvPlayer::updateAgricultureHistory(int iTurn, int iBestAgriculture)
{
	m_mapAgricultureHistory[iTurn] = iBestAgriculture;
}

int CvPlayer::getPowerHistory(int iTurn) const
{
	CvTurnScoreMap::const_iterator it = m_mapPowerHistory.find(iTurn);
	if (it != m_mapPowerHistory.end())
	{
		return it->second;
	}
	return 0;
}

void CvPlayer::updatePowerHistory(int iTurn, int iBestPower)
{
	m_mapPowerHistory[iTurn] = iBestPower;
}

int CvPlayer::getCultureHistory(int iTurn) const
{
	CvTurnScoreMap::const_iterator it = m_mapCultureHistory.find(iTurn);
	if (it != m_mapCultureHistory.end())
	{
		return it->second;
	}
	return 0;
}

void CvPlayer::updateCultureHistory(int iTurn, int iBestCulture)
{
	m_mapCultureHistory[iTurn] = iBestCulture;
}

int CvPlayer::getEspionageHistory(int iTurn) const
{
	CvTurnScoreMap::const_iterator it = m_mapEspionageHistory.find(iTurn);
	if (it != m_mapEspionageHistory.end())
	{
		return it->second;
	}
	return 0;
}

void CvPlayer::updateEspionageHistory(int iTurn, int iBestEspionage)
{
	m_mapEspionageHistory[iTurn] = iBestEspionage;
}

std::string CvPlayer::getScriptData() const
{
	return m_szScriptData;
}

void CvPlayer::setScriptData(std::string szNewValue)
{
	m_szScriptData = szNewValue;
}

const CvString CvPlayer::getPbemEmailAddress() const
{
	return GC.getInitCore().getEmail(getID());
}

void CvPlayer::setPbemEmailAddress(const char* szAddress)
{
	GC.getInitCore().setEmail(getID(), szAddress);
}

const CvString CvPlayer::getSmtpHost() const
{
	return GC.getInitCore().getSmtpHost(getID());
}

void CvPlayer::setSmtpHost(const char* szHost)
{
	GC.getInitCore().setSmtpHost(getID(), szHost);
}

// Protected Functions...

void CvPlayer::doGold()
{
	bool bStrike;
	int iGoldChange;
//	int iDisbandUnit;
//	int iI;

	CyArgsList argsList;
	argsList.add(getID());
	long lResult=0;
	gDLL->getPythonIFace()->callFunction(PYGameModule, "doGold", argsList.makeFunctionArgs(), &lResult);
	if (lResult == 1)
	{
		return;
	}

	iGoldChange = calculateGoldRate();

	FAssert(isHuman() || isBarbarian() || ((getGold() + iGoldChange) >= 0) || isAnarchy());

	changeGold(iGoldChange);

	bStrike = false;

	if (getGold() < 0)
	{
		setGold(0);

		if (!isBarbarian() && (getNumCities() > 0))
		{
			bStrike = true;
		}
	}

	if (bStrike)
	{
		setStrike(true);
		changeStrikeTurns(1);

		if (getStrikeTurns() > 1)
		{
			//new logic for not enough gold
			/**
			iDisbandUnit = (getStrikeTurns() / 2); // XXX mod?

			for (iI = 0; iI < iDisbandUnit; iI++)
			{
				disbandUnit(true);

				if (calculateGoldRate() >= 0)
				{
					break;
				}
			}
			**/
			if(getStrikeTurns() > 3)
			{
				CvCity* pBestCity=NULL;
				int iLoop;
				int iBestValue=0;
				int iValue;
				for (CvCity* pCity = firstCity(&iLoop); NULL != pCity; pCity = nextCity(&iLoop))
				{
					iValue=pCity->getRevoltValue();
					if(iValue>iBestValue)
					{
						iBestValue=iValue;
						pBestCity=pCity;
					}
				}

				if(pBestCity!=NULL)
				{
					int iCounter=0;

					CLLNode<IDInfo>* pUnitNode = pBestCity->plot()->headUnitNode();
					while (pUnitNode != NULL)
					{
						CvUnit* pLoopUnit = ::getUnit(pUnitNode->m_data);
						pUnitNode = pBestCity->plot()->nextUnitNode(pUnitNode);

						if(pLoopUnit->isHasSecondChance() || iCounter>5 || pLoopUnit->getTeam()!=getTeam())
						{
							pLoopUnit->withdrawlToNearestValidPlot();
						}
						else
						{
							iCounter++;
						}
					}

					CvPlot* pTarget=pBestCity->plot();
					GET_PLAYER((PlayerTypes)GC.getBARBARIAN_PLAYER()).acquireCity(pBestCity,false,true,true);

					pUnitNode = pTarget->headUnitNode();
					while (pUnitNode != NULL)
					{
						CvUnit* pLoopUnit = ::getUnit(pUnitNode->m_data);
						pUnitNode = pTarget->nextUnitNode(pUnitNode);

						if(pLoopUnit!=NULL && pLoopUnit->getOwnerINLINE()!=GC.getBARBARIAN_PLAYER())
						{
							CvUnit* pNewUnit=GET_PLAYER((PlayerTypes)GC.getBARBARIAN_PLAYER()).initUnit(pLoopUnit->getUnitType(),pTarget->getX_INLINE(),pTarget->getY_INLINE());
							pNewUnit->convert(pLoopUnit);
						}
					}

				}
			}
		}
	}
	else
	{
		setStrike(false);
	}
}


void CvPlayer::doResearch()
{
	bool bForceResearchChoice;
	int iOverflowResearch;

	CyArgsList argsList;
	argsList.add(getID());
	long lResult=0;
	gDLL->getPythonIFace()->callFunction(PYGameModule, "doResearch", argsList.makeFunctionArgs(), &lResult);
	if (lResult == 1)
	{
		return;
	}

	if (isResearch())
	{
		bForceResearchChoice = false;

		if (getCurrentResearch() == NO_TECH)
		{
			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				chooseTech();
			}

			if (GC.getGameINLINE().getElapsedGameTurns() > 4)
			{
				AI_chooseResearch();

				bForceResearchChoice = true;
			}
		}

		TechTypes eCurrentTech = getCurrentResearch();
		if (eCurrentTech == NO_TECH)
		{
			int iOverflow = (100 * calculateResearchRate()) / std::max(1, calculateResearchModifier(eCurrentTech));
			changeOverflowResearch(iOverflow);
		}
		else
		{
			iOverflowResearch = (getOverflowResearch() * calculateResearchModifier(eCurrentTech)) / 100;
			setOverflowResearch(0);
			GET_TEAM(getTeam()).changeResearchProgress(eCurrentTech, (calculateResearchRate() + iOverflowResearch), getID());
		}

		if (bForceResearchChoice)
		{
			clearResearchQueue();
		}
	}
}

void CvPlayer::doEspionagePoints()
{
	if (getCommerceRate(COMMERCE_ESPIONAGE) > 0)
	{
		GET_TEAM(getTeam()).changeEspionagePointsEver(getCommerceRate(COMMERCE_ESPIONAGE));

		int iSpending = 0;

		// Divide up Espionage between Teams
		for (int iLoop = 0; iLoop < MAX_CIV_TEAMS; iLoop++)
		{
			if (getTeam() != iLoop)
			{
				if (GET_TEAM((TeamTypes)iLoop).isAlive())
				{
					if (GET_TEAM(getTeam()).isHasMet((TeamTypes)iLoop))
					{
						iSpending = getEspionageSpending((TeamTypes)iLoop);

						if (iSpending > 0)
						{
							GET_TEAM(getTeam()).changeEspionagePointsAgainstTeam((TeamTypes)iLoop, iSpending);
						}
					}
				}
			}
		}
	}
}

int CvPlayer::getEspionageSpending(TeamTypes eAgainstTeam) const
{
	int iSpendingValue = 0;

	int iTotalPoints = getCommerceRate(COMMERCE_ESPIONAGE);
	int iAvailablePoints = iTotalPoints;

	int iTotalWeight = 0;
	int iBestWeight = 0;

	bool bFoundTeam = false;

	int iLoop;

	// Get sum of all weights to be used later on
	for (iLoop = 0; iLoop < MAX_CIV_TEAMS; iLoop++)
	{
		if (getTeam() != iLoop)
		{
			if (GET_TEAM((TeamTypes)iLoop).isAlive())
			{
				if (GET_TEAM(getTeam()).isHasMet((TeamTypes)iLoop))
				{
					if (iLoop == int(eAgainstTeam))
					{
						bFoundTeam = true;
					}

					int iWeight = getEspionageSpendingWeightAgainstTeam((TeamTypes)iLoop);

					if (iWeight > iBestWeight)
					{
						iBestWeight = iWeight;
					}

					iTotalWeight += iWeight;
				}
			}
		}
	}

	// The player requested is not valid
	if (!bFoundTeam)
	{
		return -1;
	}

	// Split up Espionage Point budget based on weights (if any weights have been assigned)
	if (iTotalWeight > 0)
	{
		for (iLoop = 0; iLoop < MAX_CIV_TEAMS; iLoop++)
		{
			if (getTeam() != iLoop)
			{
				if (GET_TEAM((TeamTypes)iLoop).isAlive())
				{
					if (GET_TEAM(getTeam()).isHasMet((TeamTypes)iLoop))
					{
						int iChange = (iTotalPoints * getEspionageSpendingWeightAgainstTeam((TeamTypes)iLoop) / iTotalWeight);
						iAvailablePoints -= iChange;

						if (iLoop == int(eAgainstTeam))
						{
							iSpendingValue += iChange;
						}
					}
				}
			}
		}
	}

	// Divide remainder evenly among top Teams
	while (iAvailablePoints > 0)
	{
		for (iLoop = 0; iLoop < MAX_CIV_TEAMS; iLoop++)
		{
			if (getTeam() != iLoop)
			{
				if (GET_TEAM((TeamTypes)iLoop).isAlive())
				{
					if (GET_TEAM(getTeam()).isHasMet((TeamTypes)iLoop))
					{
						if (getEspionageSpendingWeightAgainstTeam((TeamTypes)iLoop) == iBestWeight)
						{
							if (iLoop == int(eAgainstTeam))
							{
								++iSpendingValue;
							}
							--iAvailablePoints;

							if (iAvailablePoints <= 0)
							{
								break;
							}
						}
					}
				}
			}
		}
	}

	return iSpendingValue;
}

bool CvPlayer::canDoEspionageMission(EspionageMissionTypes eMission, PlayerTypes eTargetPlayer, const CvPlot* pPlot, int iExtraData, const CvUnit* pUnit) const
{
	if (getID() == eTargetPlayer || NO_PLAYER == eTargetPlayer)
	{
		return false;
	}

	if (!GET_PLAYER(eTargetPlayer).isAlive() || !GET_TEAM(getTeam()).isHasMet(GET_PLAYER(eTargetPlayer).getTeam()))
	{
		return false;
	}

	CvEspionageMissionInfo& kMission = GC.getEspionageMissionInfo(eMission);

	// Need Tech Prereq, if applicable
	if (kMission.getTechPrereq() != NO_TECH)
	{
		if (!GET_TEAM(getTeam()).isHasTech((TechTypes)kMission.getTechPrereq()))
		{
			return false;
		}
	}

	int iCost = getEspionageMissionCost(eMission, eTargetPlayer, pPlot, iExtraData, pUnit);
	if (iCost < 0)
	{
		return false;
	}

	if (NO_PLAYER != eTargetPlayer)
	{
		int iEspionagePoints = GET_TEAM(getTeam()).getEspionagePointsAgainstTeam(GET_PLAYER(eTargetPlayer).getTeam());

		if (iEspionagePoints < iCost)
		{
			return false;
		}

		if (iEspionagePoints <= 0)
		{
			return false;
		}
	}

	return true;
}

int CvPlayer::getEspionageMissionCost(EspionageMissionTypes eMission, PlayerTypes eTargetPlayer, const CvPlot* pPlot, int iExtraData, const CvUnit* pSpyUnit) const
{
	int iMissionCost = getEspionageMissionBaseCost(eMission, eTargetPlayer, pPlot, iExtraData, pSpyUnit);

	if (-1 == iMissionCost)
	{
		return -1;
	}

	iMissionCost *= getEspionageMissionCostModifier(eMission, eTargetPlayer, pPlot, iExtraData, pSpyUnit);
	iMissionCost /= 100;

	// Multiply cost of mission * number of team members
	iMissionCost *= GET_TEAM(getTeam()).getNumMembers();

	return std::max(0, iMissionCost);
}

int CvPlayer::getEspionageMissionBaseCost(EspionageMissionTypes eMission, PlayerTypes eTargetPlayer, const CvPlot* pPlot, int iExtraData, const CvUnit* pSpyUnit) const
{
	CvEspionageMissionInfo& kMission = GC.getEspionageMissionInfo(eMission);
	int iBaseMissionCost = kMission.getCost();

	// -1 means this mission is disabled
	if (iBaseMissionCost == -1)
	{
		return -1;
	}

	CvCity* pCity = NULL;
	if (NULL != pPlot)
	{
		pCity = pPlot->getPlotCity();
	}

	if (kMission.isSelectPlot())
	{
		if (NULL == pPlot)
		{
			return -1;
		}

		if (!pPlot->isRevealed(getTeam(), false))
		{
			return -1;
		}
	}

	if (NULL == pCity && kMission.isTargetsCity())
	{
		return -1;
	}

	int iMissionCost = -1;

	if (kMission.getStealTreasuryTypes() > 0)
	{
		// Steal Treasury
		int iNumTotalGold = (GET_PLAYER(eTargetPlayer).getGold() * kMission.getStealTreasuryTypes()) / 100;

		if (NULL != pCity)
		{
			iNumTotalGold *= pCity->getPopulation();
			iNumTotalGold /= std::max(1, GET_PLAYER(eTargetPlayer).getTotalPopulation());
		}

		if (iNumTotalGold > 0)
		{
			iMissionCost = (iBaseMissionCost * iNumTotalGold) / 100;
		}
	}
	else if (kMission.getBuyTechCostFactor() > 0)
	{
		// Buy (Steal) Tech
		TechTypes eTech = (TechTypes)iExtraData;
		int iProdCost = MAX_INT;

		if (NO_TECH == eTech)
		{
			for (int iTech = 0; iTech < GC.getNumTechInfos(); ++iTech)
			{
				if (canStealTech(eTargetPlayer, (TechTypes)iTech))
				{
					int iCost = GET_TEAM(getTeam()).getResearchCost((TechTypes)iTech);
					if (iCost < iProdCost)
					{
						iProdCost = iCost;
						eTech = (TechTypes)iTech;
					}
				}
			}
		}
		else
		{
			iProdCost = GET_TEAM(getTeam()).getResearchCost(eTech);
		}

		if (NO_TECH != eTech)
		{
			if (canStealTech(eTargetPlayer, eTech))
			{
				iMissionCost = iBaseMissionCost + ((100 + kMission.getBuyTechCostFactor()) * iProdCost) / 100;
			}
		}
	}
	else if (kMission.getSwitchCivicCostFactor() > 0)
	{
		// Switch Civics
		CivicTypes eCivic = (CivicTypes)iExtraData;

		if (NO_CIVIC == eCivic)
		{
			for (int iCivic = 0; iCivic < GC.getNumCivicInfos(); ++iCivic)
			{
				if (canForceCivics(eTargetPlayer, (CivicTypes)iCivic))
				{
					eCivic = (CivicTypes)iCivic;
					break;
				}
			}
		}

		if (NO_CIVIC != eCivic)
		{
			if (canForceCivics(eTargetPlayer, eCivic))
			{
				iMissionCost = iBaseMissionCost + (kMission.getSwitchCivicCostFactor() * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getAnarchyPercent()) / 10000;
			}
		}
	}
	else if (kMission.getSwitchReligionCostFactor() > 0)
	{
		// Switch Religions
		ReligionTypes eReligion = (ReligionTypes)iExtraData;

		if (NO_RELIGION == eReligion)
		{
			for (int iReligion = 0; iReligion < GC.getNumReligionInfos(); ++iReligion)
			{
				if (canForceReligion(eTargetPlayer, (ReligionTypes)iReligion))
				{
					eReligion = (ReligionTypes)iReligion;
					break;
				}
			}
		}

		if (NO_RELIGION != eReligion)
		{
			if (canForceReligion(eTargetPlayer, eReligion))
			{
				iMissionCost = iBaseMissionCost + (kMission.getSwitchReligionCostFactor() * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getAnarchyPercent()) / 10000;
			}
		}
	}
	else if (kMission.getDestroyUnitCostFactor() > 0)
	{
		// Destroys Unit
		CvUnit* pUnit = GET_PLAYER(eTargetPlayer).getUnit(iExtraData);
		int iCost = MAX_INT;

		if (NULL == pUnit)
		{
			if (NULL != pPlot)
			{
				CLLNode<IDInfo>* pUnitNode = pPlot->headUnitNode();

				while (pUnitNode != NULL)
				{
					CvUnit* pLoopUnit = ::getUnit(pUnitNode->m_data);
					pUnitNode = pPlot->nextUnitNode(pUnitNode);

					if (canSpyDestroyUnit(eTargetPlayer, *pLoopUnit))
					{
						int iValue = getProductionNeeded(pLoopUnit->getUnitType());
						if (iValue < iCost)
						{
							iCost = iValue;
							pUnit = pLoopUnit;
						}
					}
				}

			}
		}
		else
		{
			iCost = getProductionNeeded(pUnit->getUnitType());
		}

		if (NULL != pUnit)
		{
			if (canSpyDestroyUnit(eTargetPlayer, *pUnit))
			{
				iMissionCost = iBaseMissionCost + ((100 + kMission.getDestroyUnitCostFactor()) * iCost) / 100;
			}
		}
	}
	else if (kMission.getDestroyProjectCostFactor() > 0)
	{
		ProjectTypes eProject = (ProjectTypes) iExtraData;
		int iCost = MAX_INT;

		if (NO_PROJECT == eProject)
		{
			for (int iProject = 0; iProject < GC.getNumProjectInfos(); ++iProject)
			{
				if (canSpyDestroyProject(eTargetPlayer, (ProjectTypes)iProject))
				{
					int iValue = getProductionNeeded((ProjectTypes)iProject);

					if (iValue < iCost)
					{
						iCost = iValue;
						eProject = (ProjectTypes)iProject;
					}
				}
			}
		}
		else
		{
			iCost = getProductionNeeded(eProject);
		}

		if (NO_PROJECT != eProject)
		{
			if (canSpyDestroyProject(eTargetPlayer, eProject))
			{
				iMissionCost = iBaseMissionCost + ((100 + kMission.getDestroyProjectCostFactor()) * iCost) / 100;
			}
		}
	}
	else if (kMission.getDestroyProductionCostFactor() > 0)
	{
		FAssert(NULL != pCity);
		if (NULL != pCity)
		{
			iMissionCost = iBaseMissionCost + ((100 + kMission.getDestroyProductionCostFactor()) * pCity->getProduction()) / 100;
		}
	}
	else if (kMission.getBuyUnitCostFactor() > 0)
	{
		// Buy Unit
		CvUnit* pUnit = GET_PLAYER(eTargetPlayer).getUnit(iExtraData);
		int iCost = MAX_INT;

		if (NULL == pUnit)
		{
			if (NULL != pPlot)
			{
				CLLNode<IDInfo>* pUnitNode = pPlot->headUnitNode();

				while (pUnitNode != NULL)
				{
					CvUnit* pLoopUnit = ::getUnit(pUnitNode->m_data);
					pUnitNode = pPlot->nextUnitNode(pUnitNode);

					if (canSpyBribeUnit(eTargetPlayer, *pLoopUnit))
					{
						int iValue = getProductionNeeded(pLoopUnit->getUnitType());
						if (iValue < iCost)
						{
							iCost = iValue;
							pUnit = pLoopUnit;
						}
					}
				}

			}
		}
		else
		{
			iCost = getProductionNeeded(pUnit->getUnitType());
		}


		if (NULL != pUnit)
		{
			if (canSpyBribeUnit(eTargetPlayer, *pUnit))
			{
				iMissionCost = iBaseMissionCost + ((100 + kMission.getBuyUnitCostFactor()) * iCost) / 100;
			}
		}
	}
	else if (kMission.getDestroyBuildingCostFactor() > 0)
	{
		BuildingTypes eBuilding = (BuildingTypes) iExtraData;
		int iCost = MAX_INT;

		if (NO_BUILDING == eBuilding)
		{
			for (int iBuilding = 0; iBuilding < GC.getNumBuildingInfos(); ++iBuilding)
			{
				if (NULL != pCity && pCity->getNumRealBuilding((BuildingTypes)iBuilding) > 0)
				{
					if (canSpyDestroyBuilding(eTargetPlayer, (BuildingTypes)iBuilding))
					{
						int iValue = getProductionNeeded((BuildingTypes)iBuilding);

						if (iValue < iCost)
						{
							iCost = iValue;
							eBuilding = (BuildingTypes)iBuilding;
						}
					}
				}
			}
		}
		else
		{
			iCost = getProductionNeeded(eBuilding);
		}

		if (NO_BUILDING != eBuilding)
		{
			if (NULL != pCity && pCity->getNumRealBuilding(eBuilding) > 0)
			{
				if (canSpyDestroyBuilding(eTargetPlayer, eBuilding))
				{
					iMissionCost = iBaseMissionCost + ((100 + kMission.getDestroyBuildingCostFactor()) * iCost) / 100;
				}
			}
		}
	}
	else if (kMission.getBuyCityCostFactor() > 0)
	{
		// Buy City
		if (NULL != pCity)
		{
			iMissionCost = iBaseMissionCost + (kMission.getBuyCityCostFactor() * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getGrowthPercent()) / 10000;
		}
	}
	else if (kMission.getCityInsertCultureCostFactor() > 0)
	{
		// Insert Culture into City
		if (NULL != pPlot && pPlot->getCulture(getID()) > 0)
		{
			int iCultureAmount = kMission.getCityInsertCultureAmountFactor() *  pCity->countTotalCultureTimes100();
			iCultureAmount /= 10000;
			iCultureAmount = std::max(1, iCultureAmount);
			iMissionCost = iBaseMissionCost + (kMission.getCityInsertCultureCostFactor() * iCultureAmount) / 100;
		}
	}
	else if (kMission.isDestroyImprovement())
	{
		if (NULL != pPlot && !pPlot->isCity())
		{
			if (pPlot->getImprovementType() != NO_IMPROVEMENT || pPlot->getRouteType() != NO_ROUTE)
			{
				iMissionCost = (iBaseMissionCost * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getBuildPercent()) / 100;
			}
		}
	}
	else if (kMission.getCityPoisonWaterCounter() > 0)
	{
		FAssert(NULL != pCity);
		// Cannot poison a city's water supply if it's already poisoned (value is negative when active)
		if (NULL != pCity && pCity->getEspionageHealthCounter() <= 0)
		{
			iMissionCost = iBaseMissionCost;
		}
	}

	// Make city unhappy
	else if (kMission.getCityUnhappinessCounter() > 0)
	{
		FAssert(NULL != pCity);
		// Cannot make a city unhappy if you've already done it (value is negative when active)
		if (NULL != pCity && pCity->getEspionageHappinessCounter() <= 0)
		{
			iMissionCost = iBaseMissionCost;
		}
	}

	// Make city Revolt
	else if (kMission.getCityRevoltCounter() > 0)
	{
		FAssert(NULL != pCity);
		// Cannot make a city revolt if it's already revolting
		if (NULL != pCity && pCity->getOccupationTimer() == 0)
		{
			iMissionCost = iBaseMissionCost;
		}
	}
	else if (kMission.getCounterespionageMod() > 0)
	{
		if (GET_TEAM(getTeam()).getCounterespionageTurnsLeftAgainstTeam(GET_PLAYER(eTargetPlayer).getTeam()) <= 0)
		{
			iMissionCost = (iBaseMissionCost * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getResearchPercent()) / 100;
		}
	}
	else if (kMission.getPlayerAnarchyCounter() > 0)
	{
		// Player anarchy timer: can't add more turns of anarchy to player already in the midst of it
		if (!GET_PLAYER(eTargetPlayer).isAnarchy())
		{
			iMissionCost = (iBaseMissionCost * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getAnarchyPercent()) / 100;
		}
	}
	else if (kMission.isPassive())
	{
		iMissionCost = (iBaseMissionCost * (100 + GET_TEAM(GET_PLAYER(eTargetPlayer).getTeam()).getEspionagePointsAgainstTeam(getTeam()))) / 100;
	}
	else
	{
		iMissionCost = (iBaseMissionCost * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getResearchPercent()) / 100;
	}

	if (iMissionCost < 0)
	{
		return -1;
	}

	return iMissionCost;
}


int CvPlayer::getEspionageMissionCostModifier(EspionageMissionTypes eMission, PlayerTypes eTargetPlayer, const CvPlot* pPlot, int iExtraData, const CvUnit* pSpyUnit) const
{
	CvEspionageMissionInfo& kMission = GC.getEspionageMissionInfo(eMission);
	int iModifier = 100;

	CvCity* pCity = NULL;
	if (NULL != pPlot)
	{
		pCity = pPlot->getPlotCity();
	}

	if (NO_PLAYER == eTargetPlayer)
	{
		eTargetPlayer = getID();
	}

	if (pCity != NULL && kMission.isTargetsCity())
	{
		// City Population
		iModifier *= 100 + (GC.getDefineINT("ESPIONAGE_CITY_POP_EACH_MOD") * (pCity->getPopulation() - 1));
		iModifier /= 100;

		// Trade Route
		if (pCity->isTradeRoute(getID()))
		{
			iModifier *= 100 + GC.getDefineINT("ESPIONAGE_CITY_TRADE_ROUTE_MOD");
			iModifier /= 100;
		}

		ReligionTypes eReligion = getStateReligion();
		if (NO_RELIGION != eReligion)
		{
			int iReligionModifier = 0;

			// City has Your State Religion
			if (pCity->isHasReligion(eReligion))
			{
				if (GET_PLAYER(eTargetPlayer).getStateReligion() != eReligion)
				{
					iReligionModifier += GC.getDefineINT("ESPIONAGE_CITY_RELIGION_STATE_MOD");
				}

				if (hasHolyCity(eReligion))
				{
					iReligionModifier += GC.getDefineINT("ESPIONAGE_CITY_HOLY_CITY_MOD");;
				}
			}

			iModifier *= 100 + iReligionModifier;
			iModifier /= 100;

		}

		// City's culture affects cost
		iModifier *= 100 - (pCity->getCultureTimes100(getID()) * GC.getDefineINT("ESPIONAGE_CULTURE_MULTIPLIER_MOD")) / std::max(1, pCity->getCultureTimes100(eTargetPlayer) + pCity->getCultureTimes100(getID()));
		iModifier /= 100;

		iModifier *= 100 + pCity->getEspionageDefenseModifier();
		iModifier /= 100;
	}

	// Distance mod
	if (pPlot != NULL)
	{
		int iDistance = GC.getMap().maxPlotDistance();

		CvCity* pOurCapital = getCapitalCity();
		if (NULL != pOurCapital)
		{
			if (kMission.isSelectPlot() || kMission.isTargetsCity())
			{
				iDistance = plotDistance(pOurCapital->getX_INLINE(), pOurCapital->getY_INLINE(), pPlot->getX_INLINE(), pPlot->getY_INLINE());
			}
			else
			{
				CvCity* pTheirCapital = GET_PLAYER(eTargetPlayer).getCapitalCity();
				if (NULL != pTheirCapital)
				{
					iDistance = plotDistance(pOurCapital->getX_INLINE(), pOurCapital->getY_INLINE(), pTheirCapital->getX_INLINE(), pTheirCapital->getY_INLINE());
				}
			}
		}

		iModifier *= (iDistance + GC.getMapINLINE().maxPlotDistance()) * GC.getDefineINT("ESPIONAGE_DISTANCE_MULTIPLIER_MOD") / GC.getMapINLINE().maxPlotDistance();
		iModifier /= 100;
	}

	// Spy presence mission cost alteration
	if (NULL != pSpyUnit)
	{
		iModifier *= 100 - (pSpyUnit->getFortifyTurns() * GC.getDefineINT("ESPIONAGE_EACH_TURN_UNIT_COST_DECREASE"));
		iModifier /= 100;
	}

	// My points VS. Your points to mod cost
	int iTargetPoints = GET_TEAM(GET_PLAYER(eTargetPlayer).getTeam()).getEspionagePointsEver();
	int iOurPoints = GET_TEAM(getTeam()).getEspionagePointsEver();
	iModifier *= (GC.getDefineINT("ESPIONAGE_SPENDING_MULTIPLIER") * (2 * iTargetPoints + iOurPoints)) / std::max(1, iTargetPoints + 2 * iOurPoints);
	iModifier /= 100;

	// Counterespionage Mission Mod
	CvTeam& kTargetTeam = GET_TEAM(GET_PLAYER(eTargetPlayer).getTeam());
	if (kTargetTeam.getCounterespionageModAgainstTeam(getTeam()) > 0)
	{
		iModifier *= kTargetTeam.getCounterespionageModAgainstTeam(getTeam());
		iModifier /= 100;
	}

	return iModifier;
}


bool CvPlayer::doEspionageMission(EspionageMissionTypes eMission, PlayerTypes eTargetPlayer, CvPlot* pPlot, int iExtraData, CvUnit* pSpyUnit)
{
	if (!canDoEspionageMission(eMission, eTargetPlayer, pPlot, iExtraData, pSpyUnit))
	{
		return false;
	}

	TeamTypes eTargetTeam = NO_TEAM;
	if (NO_PLAYER != eTargetPlayer)
	{
		eTargetTeam = GET_PLAYER(eTargetPlayer).getTeam();
	}

	CvEspionageMissionInfo& kMission = GC.getEspionageMissionInfo(eMission);

	bool bSomethingHappened = false;
	bool bShowExplosion = false;
	CvWString szBuffer;
	int iMissionCost = getEspionageMissionCost(eMission, eTargetPlayer, pPlot, iExtraData, pSpyUnit);


	//////////////////////////////
	// Destroy Improvement

	if (kMission.isDestroyImprovement())
	{
		if (NULL != pPlot)
		{
			// Blow it up
			if (pPlot->getImprovementType() != NO_IMPROVEMENT)
			{
				szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_SOMETHING_DESTROYED", GC.getImprovementInfo(pPlot->getImprovementType()).getDescription()).GetCString();
				pPlot->setImprovementType((ImprovementTypes)(GC.getImprovementInfo(pPlot->getImprovementType()).getImprovementPillage()));
				bSomethingHappened = true;
			}
			else if (pPlot->getRouteType() != NO_ROUTE)
			{
				szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_SOMETHING_DESTROYED", GC.getRouteInfo(pPlot->getRouteType()).getDescription()).GetCString();
				pPlot->setRouteType(NO_ROUTE, true);
				bSomethingHappened = true;
			}

			if (bSomethingHappened)
			{
				bShowExplosion = true;
			}
		}
	}

	//////////////////////////////
	// Destroy Building

	if (kMission.getDestroyBuildingCostFactor() > 0)
	{
		BuildingTypes eTargetBuilding = (BuildingTypes)iExtraData;

		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_SOMETHING_DESTROYED_IN", GC.getBuildingInfo(eTargetBuilding).getDescription(), pCity->getNameKey()).GetCString();
				pCity->setNumRealBuilding(eTargetBuilding, pCity->getNumRealBuilding(eTargetBuilding) - 1);

				bSomethingHappened = true;
				bShowExplosion = true;
			}
		}
	}

	//////////////////////////////
	// Destroy Project

	if (kMission.getDestroyProjectCostFactor() > 0)
	{
		ProjectTypes eTargetProject = (ProjectTypes)iExtraData;

		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_SOMETHING_DESTROYED_IN", GC.getProjectInfo(eTargetProject).getDescription(), pCity->getNameKey()).GetCString();
				GET_TEAM(eTargetTeam).changeProjectCount(eTargetProject, -1);

				bSomethingHappened = true;
				bShowExplosion = true;
			}
		}
	}

	//////////////////////////////
	// Destroy Production

	if (kMission.getDestroyProductionCostFactor() > 0)
	{
		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_PRODUCTION_DESTROYED_IN", pCity->getProductionName(), pCity->getNameKey());
				pCity->setProduction(0);

				bSomethingHappened = true;
				bShowExplosion = true;
			}
		}
	}

	//////////////////////////////
	// Destroy Unit

	if (kMission.getDestroyUnitCostFactor() > 0)
	{
		if (NO_PLAYER != eTargetPlayer)
		{
			int iTargetUnitID = iExtraData;

			CvUnit* pUnit = GET_PLAYER(eTargetPlayer).getUnit(iTargetUnitID);

			if (NULL != pUnit)
			{
				FAssert(pUnit->plot() == pPlot);
				szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_SOMETHING_DESTROYED", pUnit->getNameKey()).GetCString();
				pUnit->kill(false, getID());

				bSomethingHappened = true;
				bShowExplosion = true;
			}
		}
	}

	//////////////////////////////
	// Buy Unit

	if (kMission.getBuyUnitCostFactor() > 0)
	{
		if (NO_PLAYER != eTargetPlayer)
		{
			int iTargetUnitID = iExtraData;

			CvUnit* pUnit = GET_PLAYER(eTargetPlayer).getUnit(iTargetUnitID);

			if (NULL != pUnit)
			{
				FAssert(pUnit->plot() == pPlot);

				szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_UNIT_BOUGHT", pUnit->getNameKey()).GetCString();

				UnitTypes eUnitType = pUnit->getUnitType();
				int iX = pUnit->getX_INLINE();
				int iY = pUnit->getY_INLINE();
				pUnit->kill(false, getID());
				initUnit(eUnitType, iX, iY, NO_UNITAI);

				bSomethingHappened = true;
			}
		}
	}

	//////////////////////////////
	// Buy City

	if (kMission.getBuyCityCostFactor() > 0)
	{
		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_CITY_BOUGHT", pCity->getNameKey()).GetCString();
				acquireCity(pCity, false, true, true);

				bSomethingHappened = true;
			}
		}
	}

	//////////////////////////////
	// Insert Culture into City

	if (kMission.getCityInsertCultureCostFactor() > 0)
	{
		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_CITY_CULTURE_INSERTED", pCity->getNameKey()).GetCString();

				int iCultureAmount = kMission.getCityInsertCultureAmountFactor() * pCity->countTotalCultureTimes100();
				iCultureAmount /= 10000;
				iCultureAmount = std::max(1, iCultureAmount);

				int iNumTurnsApplied = (GC.getDefineINT("GREAT_WORKS_CULTURE_TURNS") * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getUnitGreatWorkPercent()) / 100;

				for (int i = 0; i < iNumTurnsApplied; ++i)
				{
					pCity->changeCulture(getID(), iCultureAmount / iNumTurnsApplied, true, true);
				}

				if (iNumTurnsApplied > 0)
				{
					pCity->changeCulture(getID(), iCultureAmount % iNumTurnsApplied, false, true);
				}

				bSomethingHappened = true;
			}

		}
	}

	//////////////////////////////
	// Poison City's Water Supply

	if (kMission.getCityPoisonWaterCounter() > 0)
	{
		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_CITY_POISONED", pCity->getNameKey()).GetCString();
				pCity->changeEspionageHealthCounter(kMission.getCityPoisonWaterCounter());

				bShowExplosion = true;
				bSomethingHappened = true;
			}
		}
	}

	//////////////////////////////
	// Make city Unhappy

	if (kMission.getCityUnhappinessCounter() > 0)
	{
		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_CITY_UNHAPPY", pCity->getNameKey()).GetCString();
				pCity->changeEspionageHappinessCounter(kMission.getCityUnhappinessCounter());

				bShowExplosion = true;
				bSomethingHappened = true;
			}
		}
	}

	//////////////////////////////
	// Make city Revolt

	if (kMission.getCityRevoltCounter() > 0)
	{
		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_CITY_REVOLT", pCity->getNameKey()).GetCString();
				pCity->changeCultureUpdateTimer(kMission.getCityRevoltCounter());
				pCity->changeOccupationTimer(kMission.getCityRevoltCounter());

				bSomethingHappened = true;
				bShowExplosion = true;
			}
		}
	}

	//////////////////////////////
	// Steal Treasury

	if (kMission.getStealTreasuryTypes() > 0)
	{
		if (NO_PLAYER != eTargetPlayer)
		{
			int iNumTotalGold = (GET_PLAYER(eTargetPlayer).getGold() * kMission.getStealTreasuryTypes()) / 100;

			if (NULL != pPlot)
			{
				CvCity* pCity = pPlot->getPlotCity();

				if (NULL != pCity)
				{
					iNumTotalGold *= pCity->getPopulation();
					iNumTotalGold /= std::max(1, GET_PLAYER(eTargetPlayer).getTotalPopulation());
				}
			}

			szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_STEAL_TREASURY").GetCString();
			changeGold(iNumTotalGold);
			if (NO_PLAYER != eTargetPlayer)
			{
				GET_PLAYER(eTargetPlayer).changeGold(-iNumTotalGold);
			}

			bSomethingHappened = true;
		}
	}

	//////////////////////////////
	// Buy (Steal) Tech

	if (kMission.getBuyTechCostFactor() > 0)
	{
		int iTech = iExtraData;

		szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_TECH_BOUGHT", GC.getTechInfo((TechTypes) iTech).getDescription()).GetCString();
		GET_TEAM(getTeam()).setHasTech((TechTypes) iTech, true, getID(), false, true);
		GET_TEAM(getTeam()).setNoTradeTech((TechTypes)iTech, true);

		bSomethingHappened = true;
	}

	//////////////////////////////
	// Switch Civic

	if (kMission.getSwitchCivicCostFactor() > 0)
	{
		if (NO_PLAYER != eTargetPlayer)
		{
			int iCivic = iExtraData;

			szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_SWITCH_CIVIC", GC.getCivicInfo((CivicTypes) iCivic).getDescription()).GetCString();
			GET_PLAYER(eTargetPlayer).setCivics((CivicOptionTypes) GC.getCivicInfo((CivicTypes) iCivic).getCivicOptionType(), (CivicTypes) iCivic);
			GET_PLAYER(eTargetPlayer).setRevolutionTimer(std::max(1, ((100 + GET_PLAYER(eTargetPlayer).getAnarchyModifier()) * GC.getDefineINT("MIN_REVOLUTION_TURNS")) / 100));
			bSomethingHappened = true;
		}
	}

	//////////////////////////////
	// Switch Religion

	if (kMission.getSwitchReligionCostFactor() > 0)
	{
		if (NO_PLAYER != eTargetPlayer)
		{
			int iReligion = iExtraData;

			szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_SWITCH_RELIGION", GC.getReligionInfo((ReligionTypes) iReligion).getDescription()).GetCString();
			GET_PLAYER(eTargetPlayer).setLastStateReligion((ReligionTypes) iReligion);
			GET_PLAYER(eTargetPlayer).setConversionTimer(std::max(1, ((100 + GET_PLAYER(eTargetPlayer).getAnarchyModifier()) * GC.getDefineINT("MIN_CONVERSION_TURNS")) / 100));
			bSomethingHappened = true;
		}
	}

	//////////////////////////////
	// Player Anarchy

	if (kMission.getPlayerAnarchyCounter() > 0)
	{
		if (NO_PLAYER != eTargetPlayer)
		{
			int iTurns = (kMission.getPlayerAnarchyCounter() * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getAnarchyPercent()) / 100;
			szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_PLAYER_ANARCHY").GetCString();
			GET_PLAYER(eTargetPlayer).changeAnarchyTurns(iTurns);

			bSomethingHappened = true;
		}
	}

	//////////////////////////////
	// Counterespionage

	if (kMission.getCounterespionageNumTurns() > 0 && kMission.getCounterespionageMod() > 0)
	{
		szBuffer = gDLL->getText("TXT_KEY_ESPIONAGE_TARGET_COUNTERESPIONAGE").GetCString();

		if (NO_TEAM != eTargetTeam)
		{
			int iTurns = (kMission.getCounterespionageNumTurns() * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getResearchPercent()) / 100;
			GET_TEAM(getTeam()).changeCounterespionageTurnsLeftAgainstTeam(eTargetTeam, iTurns);
			GET_TEAM(getTeam()).changeCounterespionageModAgainstTeam(eTargetTeam, kMission.getCounterespionageMod());

			bSomethingHappened = true;

		}
	}

	int iHave = 0;
	if (NO_TEAM != eTargetTeam)
	{
		iHave = GET_TEAM(getTeam()).getEspionagePointsAgainstTeam(eTargetTeam);

		if (bSomethingHappened)
		{
			GET_TEAM(getTeam()).changeEspionagePointsAgainstTeam(eTargetTeam, -iMissionCost);
		}
	}

	if (bShowExplosion)
	{
		if (pPlot)
		{
			if (pPlot->isVisible(GC.getGame().getActiveTeam(), false))
			{
				EffectTypes eEffect = GC.getEntityEventInfo(GC.getMissionInfo(MISSION_BOMBARD).getEntityEvent()).getEffectType();
				gDLL->getEngineIFace()->TriggerEffect(eEffect, pPlot->getPoint(), (float)(GC.getASyncRand().get(360)));
				gDLL->getInterfaceIFace()->playGeneralSound("AS3D_UN_CITY_EXPLOSION", pPlot->getPoint());
			}
		}
	}

	if (bSomethingHappened)
	{
		int iX = -1;
		int iY = -1;
		if (NULL != pPlot)
		{
			iX = pPlot->getX_INLINE();
			iY = pPlot->getY_INLINE();
		}

		gDLL->getInterfaceIFace()->addMessage(getID(), true, GC.getEVENT_MESSAGE_TIME(), gDLL->getText("TXT_KEY_ESPIONAGE_MISSION_PERFORMED"), "AS2D_POSITIVE_DINK", MESSAGE_TYPE_INFO, ARTFILEMGR.getInterfaceArtInfo("ESPIONAGE_BUTTON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_GREEN"), iX, iY, true, true);
	}
	else if (getID() == GC.getGameINLINE().getActivePlayer())
	{
		CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_TEXT);
		if (iHave < iMissionCost)
		{
			pInfo->setText(gDLL->getText("TXT_KEY_ESPIONAGE_TOO_EXPENSIVE", iMissionCost, iHave));
		}
		else
		{
			pInfo->setText(gDLL->getText("TXT_KEY_ESPIONAGE_CANNOT_DO_MISSION"));
		}

		addPopup(pInfo);
	}

	if (bSomethingHappened && !szBuffer.empty())
	{
		int iX = -1;
		int iY = -1;
		if (NULL != pPlot)
		{
			iX = pPlot->getX_INLINE();
			iY = pPlot->getY_INLINE();
		}

		if (NO_PLAYER != eTargetPlayer)
		{
			gDLL->getInterfaceIFace()->addMessage(eTargetPlayer, true, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DEAL_CANCELLED", MESSAGE_TYPE_INFO, ARTFILEMGR.getInterfaceArtInfo("ESPIONAGE_BUTTON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_RED"), iX, iY, true, true);
		}
	}

	return bSomethingHappened;
}

int CvPlayer::getEspionageSpendingWeightAgainstTeam(TeamTypes eIndex) const
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	return m_aiEspionageSpendingWeightAgainstTeam[eIndex];
}

void CvPlayer::setEspionageSpendingWeightAgainstTeam(TeamTypes eIndex, int iValue)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");
	FAssert(iValue >= 0);

	iValue = std::min(std::max(0, iValue), 99);

	if (iValue != getEspionageSpendingWeightAgainstTeam(eIndex))
	{
		m_aiEspionageSpendingWeightAgainstTeam[eIndex] = iValue;

		gDLL->getInterfaceIFace()->setDirty(Espionage_Advisor_DIRTY_BIT, true);
	}
}

void CvPlayer::changeEspionageSpendingWeightAgainstTeam(TeamTypes eIndex, int iChange)
{
	FAssertMsg(eIndex >= 0, "eIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(eIndex < MAX_TEAMS, "eIndex is expected to be within maximum bounds (invalid Index)");

	setEspionageSpendingWeightAgainstTeam(eIndex, getEspionageSpendingWeightAgainstTeam(eIndex) + iChange);
}

void CvPlayer::doAdvancedStartAction(AdvancedStartActionTypes eAction, int iX, int iY, int iData, bool bAdd)
{
	if (getAdvancedStartPoints() < 0)
	{
		return;
	}

	CvPlot* pPlot = GC.getMap().plot(iX, iY);

	if (0 == getNumCities())
	{
		switch (eAction)
		{
		case ADVANCEDSTARTACTION_EXIT:
			//Try to build this player's empire
			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				gDLL->getInterfaceIFace()->setBusy(true);
			}
			AI_doAdvancedStart(true);
			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				gDLL->getInterfaceIFace()->setBusy(false);
			}
			break;
		case ADVANCEDSTARTACTION_AUTOMATE:
		case ADVANCEDSTARTACTION_CITY:
			break;
		default:
			// The first action must be to place a city
			// so players can lose by spending everything
			return;
		}
	}

	switch (eAction)
	{
	case ADVANCEDSTARTACTION_EXIT:
		changeGold(getAdvancedStartPoints());
		setAdvancedStartPoints(-1);
		if (GC.getGameINLINE().getActivePlayer() == getID())
		{
			gDLL->getInterfaceIFace()->setInAdvancedStart(false);
		}

		if (isHuman())
		{
			int iLoop;
			for (CvCity* pCity = firstCity(&iLoop); NULL != pCity; pCity = nextCity(&iLoop))
			{
				pCity->chooseProduction();
			}

			chooseTech();

			if (canRevolution(NULL))
			{
				CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_CHANGECIVIC);
				if (NULL != pInfo)
				{
					gDLL->getInterfaceIFace()->addPopup(pInfo, getID());
				}
			}
		}
		break;
	case ADVANCEDSTARTACTION_AUTOMATE:
		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setBusy(true);
		}
		AI_doAdvancedStart(true);
		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setBusy(false);
		}
		break;
	case ADVANCEDSTARTACTION_UNIT:
		{
			if(pPlot == NULL)
				return;

			UnitTypes eUnit = (UnitTypes) iData;
			int iCost = getAdvancedStartUnitCost(eUnit, bAdd, pPlot);

			if (bAdd && iCost < 0)
			{
				return;
			}

			// Add unit to the map
			if (bAdd)
			{
				if (getAdvancedStartPoints() >= iCost)
				{
					CvUnit* pUnit = initUnit(eUnit, iX, iY);
					if (NULL != pUnit)
					{
						pUnit->finishMoves();
						changeAdvancedStartPoints(-iCost);
					}
				}
			}

			// Remove unit from the map
			else
			{
				// If cost is -1 we already know this unit isn't present
				if (iCost != -1)
				{
					CLLNode<IDInfo>* pUnitNode = pPlot->headUnitNode();
					while (pUnitNode != NULL)
					{
						CvUnit* pLoopUnit = ::getUnit(pUnitNode->m_data);
						pUnitNode = pPlot->nextUnitNode(pUnitNode);

						if (pLoopUnit->getUnitType() == eUnit)
						{
							pLoopUnit->kill(false);
							changeAdvancedStartPoints(iCost);
							return;
						}
					}
				}

				// Proper unit not found above, delete first found
				CLLNode<IDInfo>* pUnitNode = pPlot->headUnitNode();
				if (pUnitNode != NULL)
				{
					CvUnit* pUnit = ::getUnit(pUnitNode->m_data);

					iCost = getAdvancedStartUnitCost(pUnit->getUnitType(), false);
					FAssertMsg(iCost != -1, "If this is -1 then that means it's going to try to delete a unit which shouldn't exist");
					pUnit->kill(false);
					changeAdvancedStartPoints(iCost);
				}
			}

			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				gDLL->getInterfaceIFace()->setDirty(Advanced_Start_DIRTY_BIT, true);
			}
		}
		break;
	case ADVANCEDSTARTACTION_CITY:
		{
			if(pPlot == NULL)
				return;

			int iCost = getAdvancedStartCityCost(bAdd, pPlot);

			if (iCost < 0)
			{
				return;
			}

			// Add City to the map
			if (bAdd)
			{
				if (0 == getNumCities())
				{
					PlayerTypes eClosestPlayer = NO_PLAYER;
					int iMinDistance = MAX_INT;
					for (int iPlayer = 0; iPlayer < MAX_CIV_PLAYERS; iPlayer++)
					{
						CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iPlayer);
						if (kPlayer.isAlive())
						{
							if (kPlayer.getTeam() == getTeam())
							{
								if (0 == kPlayer.getNumCities())
								{
									FAssert(kPlayer.getStartingPlot() != NULL);
									int iDistance = plotDistance(iX, iY, kPlayer.getStartingPlot()->getX_INLINE(), kPlayer.getStartingPlot()->getY_INLINE());
									if (iDistance < iMinDistance)
									{
										eClosestPlayer = kPlayer.getID();
										iMinDistance = iDistance;
									}
								}
							}
						}
					}
					FAssertMsg(eClosestPlayer != NO_PLAYER, "Self at a minimum should always be valid");
					if (eClosestPlayer != getID())
					{
						CvPlot* pTempPlot = GET_PLAYER(eClosestPlayer).getStartingPlot();
						GET_PLAYER(eClosestPlayer).setStartingPlot(getStartingPlot(), false);
						setStartingPlot(pTempPlot, false);
					}
				}
				if (getAdvancedStartPoints() >= iCost || 0 == getNumCities())
				{
					found(iX, iY);
					changeAdvancedStartPoints(-std::min(iCost, getAdvancedStartPoints()));
					GC.getGameINLINE().updateColoredPlots();
					CvCity* pCity = pPlot->getPlotCity();
					if (pCity != NULL)
					{
						if (pCity->getPopulation() > 1)
						{
							pCity->setFood(pCity->growthThreshold() / 2);
						}
					}
				}
			}

			// Remove City from the map
			else
			{
				pPlot->setRouteType(NO_ROUTE, true);
				pPlot->getPlotCity()->kill(true);
				pPlot->setImprovementType(NO_IMPROVEMENT);
				changeAdvancedStartPoints(iCost);
			}

			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				gDLL->getInterfaceIFace()->setDirty(Advanced_Start_DIRTY_BIT, true);
			}
		}
		break;
	case ADVANCEDSTARTACTION_POP:
		{
			if(pPlot == NULL)
				return;

			CvCity* pCity = pPlot->getPlotCity();

			if (pCity != NULL)
			{
				int iCost = getAdvancedStartPopCost(bAdd, pCity);

				if (iCost < 0)
				{
					return;
				}

				bool bPopChanged = false;
				if (bAdd)
				{
					if (getAdvancedStartPoints() >= iCost)
					{
						pCity->changePopulation(1);
						changeAdvancedStartPoints(-iCost);
						bPopChanged = true;
					}
				}
				else
				{
					pCity->changePopulation(-1);
					changeAdvancedStartPoints(iCost);
					bPopChanged = true;
				}

				if (bPopChanged)
				{
					pCity->setHighestPopulation(pCity->getPopulation());
					if (pCity->getPopulation() == 1)
					{
						pCity->setFood(0);
						pCity->setFoodKept(0);
					}
					else if (pCity->getPopulation() > 1)
					{
						pCity->setFood(pCity->growthThreshold() / 2);
						pCity->setFoodKept((pCity->getFood() * pCity->getMaxFoodKeptPercent()) / 100);
					}
				}
			}
		}
		break;
	case ADVANCEDSTARTACTION_CULTURE:
		{
			if(pPlot == NULL)
				return;

			CvCity* pCity = pPlot->getPlotCity();

			if (pCity != NULL)
			{
				int iCost = getAdvancedStartCultureCost(bAdd, pCity);

				if (iCost < 0)
				{
					return;
				}

				// Add Culture to the City
				if (bAdd)
				{
					if (getAdvancedStartPoints() >= iCost)
					{
						pCity->setCulture(getID(), pCity->getCultureThreshold(), true, true);
						changeAdvancedStartPoints(-iCost);
					}
				}

				// Remove Culture from the city
				else
				{
					CultureLevelTypes eLevel = (CultureLevelTypes)std::max(0, pCity->getCultureLevel() - 1);
					pCity->setCulture(getID(), CvCity::getCultureThreshold(eLevel), true, true);
					changeAdvancedStartPoints(iCost);
				}
			}
		}
		break;
	case ADVANCEDSTARTACTION_BUILDING:
		{
			if(pPlot == NULL)
				return;

			CvCity* pCity = pPlot->getPlotCity();

			if (pCity != NULL)
			{
				BuildingTypes eBuilding = (BuildingTypes) iData;
				int iCost = getAdvancedStartBuildingCost(eBuilding, bAdd, pCity);

				if (iCost < 0)
				{
					return;
				}

				// Add Building to the City
				if (bAdd)
				{
					if (getAdvancedStartPoints() >= iCost)
					{
						pCity->setNumRealBuilding(eBuilding, pCity->getNumRealBuilding(eBuilding)+1);
						changeAdvancedStartPoints(-iCost);
						if (GC.getBuildingInfo(eBuilding).getFoodKept() != 0)
						{
							pCity->setFoodKept((pCity->getFood() * pCity->getMaxFoodKeptPercent()) / 100);
						}
					}
				}

				// Remove Building from the map
				else
				{
					pCity->setNumRealBuilding(eBuilding, pCity->getNumRealBuilding(eBuilding)-1);
					changeAdvancedStartPoints(iCost);
					if (GC.getBuildingInfo(eBuilding).getFoodKept() != 0)
					{
						pCity->setFoodKept((pCity->getFood() * pCity->getMaxFoodKeptPercent()) / 100);
					}
				}
			}

			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				gDLL->getInterfaceIFace()->setDirty(Advanced_Start_DIRTY_BIT, true);
			}
		}
		break;
	case ADVANCEDSTARTACTION_ROUTE:
		{
			if(pPlot == NULL)
				return;

			RouteTypes eRoute = (RouteTypes) iData;
			int iCost = getAdvancedStartRouteCost(eRoute, bAdd, pPlot);

			if (bAdd && iCost < 0)
			{
				return;
			}

			// Add Route to the plot
			if (bAdd)
			{
				if (getAdvancedStartPoints() >= iCost)
				{
					pPlot->setRouteType(eRoute, true);
					changeAdvancedStartPoints(-iCost);
				}
			}

			// Remove Route from the Plot
			else
			{
				if (pPlot->getRouteType() != eRoute)
				{
					eRoute = pPlot->getRouteType();
					iCost = getAdvancedStartRouteCost(eRoute, bAdd);
				}

				if (iCost < 0)
				{
					return;
				}

				pPlot->setRouteType(NO_ROUTE, true);
				changeAdvancedStartPoints(iCost);
			}

			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				gDLL->getInterfaceIFace()->setDirty(Advanced_Start_DIRTY_BIT, true);
			}
		}
		break;
	case ADVANCEDSTARTACTION_IMPROVEMENT:
		{
			if(pPlot == NULL)
				return;

			ImprovementTypes eImprovement = (ImprovementTypes) iData;
			int iCost = getAdvancedStartImprovementCost(eImprovement, bAdd, pPlot);

			if (bAdd && iCost < 0)
			{
				return;
			}

			// Add Improvement to the plot
			if (bAdd)
			{
				if (getAdvancedStartPoints() >= iCost)
				{
					if (pPlot->getFeatureType() != NO_FEATURE)
					{
						for (int iI = 0; iI < GC.getNumBuildInfos(); ++iI)
						{
							ImprovementTypes eLoopImprovement = ((ImprovementTypes)(GC.getBuildInfo((BuildTypes)iI).getImprovement()));

							if (eImprovement == eLoopImprovement)
							{
								if (GC.getBuildInfo((BuildTypes)iI).isFeatureRemove(pPlot->getFeatureType()) && canBuild(pPlot, (BuildTypes)iI)

//FfH: Added by Kael 11/11/2007
                                  && (!GC.getCivilizationInfo(getCivilizationType()).isMaintainFeatures(pPlot->getFeatureType()) || GC.getBuildInfo((BuildTypes)iI).getImprovement() == NO_IMPROVEMENT)
//FfH: End Add

                                  )
								{
									pPlot->setFeatureType(NO_FEATURE);
									break;
								}
							}
						}
					}

					pPlot->setImprovementType(eImprovement);

					changeAdvancedStartPoints(-iCost);
				}
			}

			// Remove Improvement from the Plot
			else
			{
				if (pPlot->getImprovementType() != eImprovement)
				{
					eImprovement = pPlot->getImprovementType();
					iCost = getAdvancedStartImprovementCost(eImprovement, bAdd, pPlot);
				}

				if (iCost < 0)
				{
					return;
				}

				pPlot->setImprovementType(NO_IMPROVEMENT);
				changeAdvancedStartPoints(iCost);
			}

			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				gDLL->getInterfaceIFace()->setDirty(Advanced_Start_DIRTY_BIT, true);
			}
		}
		break;
	case ADVANCEDSTARTACTION_TECH:
		{
			TechTypes eTech = (TechTypes) iData;
			int iCost = getAdvancedStartTechCost(eTech, bAdd);

			if (iCost < 0)
			{
				return;
			}

			// Add Tech to team
			if (bAdd)
			{
				if (getAdvancedStartPoints() >= iCost)
				{
					GET_TEAM(getTeam()).setHasTech(eTech, true, getID(), false, false);
					changeAdvancedStartPoints(-iCost);
				}
			}

			// Remove Tech from the Team
			else
			{
				GET_TEAM(getTeam()).setHasTech(eTech, false, getID(), false, false);
				changeAdvancedStartPoints(iCost);
			}

			if (getID() == GC.getGameINLINE().getActivePlayer())
			{
				gDLL->getInterfaceIFace()->setDirty(Advanced_Start_DIRTY_BIT, true);
			}
		}
		break;
	case ADVANCEDSTARTACTION_VISIBILITY:
		{
			if(pPlot == NULL)
				return;

			int iCost = getAdvancedStartVisibilityCost(bAdd, pPlot);

			if (iCost < 0)
			{
				return;
			}

			// Add Visibility to the plot
			if (bAdd)
			{
				if (getAdvancedStartPoints() >= iCost)
				{
					pPlot->setRevealed(getTeam(), true, true, NO_TEAM, true);
					changeAdvancedStartPoints(-iCost);
				}
			}

			// Remove Visibility from the Plot
			else
			{
				pPlot->setRevealed(getTeam(), false, true, NO_TEAM, true);
				changeAdvancedStartPoints(iCost);
			}
		}
		break;
	default:
		FAssert(false);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Adding or removing a unit
/////////////////////////////////////////////////////////////////////////////////////////////

int CvPlayer::getAdvancedStartUnitCost(UnitTypes eUnit, bool bAdd, CvPlot* pPlot) const
{
	int iLoop;
	int iNumUnitType = 0;

	if (0 == getNumCities())
	{
		return -1;
	}

	int iCost = (getProductionNeeded(eUnit) * GC.getUnitInfo(eUnit).getAdvancedStartCost()) / 100;
	if (iCost < 0)
	{
		return -1;
	}

	if (NULL == pPlot)
	{
		if (bAdd)
		{
			bool bValid = false;
			int iLoop;
			for (CvCity* pLoopCity = firstCity(&iLoop); NULL != pLoopCity; pLoopCity = nextCity(&iLoop))
			{
				if (pLoopCity->canTrain(eUnit))
				{
					bValid = true;
					break;
				}
			}

			if (!bValid)
			{
				return -1;
			}
		}
	}
	else
	{
		CvCity* pCity = NULL;

		if (0 == GC.getDefineINT("ADVANCED_START_ALLOW_UNITS_OUTSIDE_CITIES"))
		{
			pCity = pPlot->getPlotCity();

			if (NULL == pCity || pCity->getOwnerINLINE() != getID())
			{
				return -1;
			}

			iCost *= 100;
			iCost /= std::max(1, 100 + pCity->getProductionModifier(eUnit));
		}
		else
		{
			if (pPlot->getOwnerINLINE() != getID())
			{
				return -1;
			}

			iCost *= 100;
			iCost /= std::max(1, 100 + getProductionModifier(eUnit));
		}


		if (bAdd)
		{
			int iMaxUnitsPerCity = GC.getDefineINT("ADVANCED_START_MAX_UNITS_PER_CITY");
			if (iMaxUnitsPerCity >= 0)
			{
				if (GC.getUnitInfo(eUnit).isMilitarySupport() && getNumMilitaryUnits() >= iMaxUnitsPerCity * getNumCities())
				{
					return -1;
				}
			}

			if (NULL != pCity)
			{
				if (!pCity->canTrain(eUnit))
				{
					return -1;
				}
			}
			else
			{
				if (!pPlot->canTrain(eUnit, false, false))
				{
					return -1;
				}

				if (pPlot->isImpassable() && !GC.getUnitInfo(eUnit).isCanMoveImpassable())
				{
					return -1;
				}

				if (pPlot->getFeatureType() != NO_FEATURE)
				{
					if (GC.getUnitInfo(eUnit).getFeatureImpassable(pPlot->getFeatureType()))
					{
						TechTypes eTech = (TechTypes)GC.getUnitInfo(eUnit).getFeaturePassableTech(pPlot->getFeatureType());
						if (NO_TECH == eTech || !GET_TEAM(getTeam()).isHasTech(eTech))
						{
							return -1;
						}
					}
				}
				else
				{
					if (GC.getUnitInfo(eUnit).getTerrainImpassable(pPlot->getTerrainType()))
					{
						TechTypes eTech = (TechTypes)GC.getUnitInfo(eUnit).getTerrainPassableTech(pPlot->getTerrainType());
						if (NO_TECH == eTech || !GET_TEAM(getTeam()).isHasTech(eTech))
						{
							return -1;
						}
					}
				}
			}
		}
		// Must be this unit at plot in order to remove
		else
		{
			bool bUnitFound = false;

			CLLNode<IDInfo>* pUnitNode = pPlot->headUnitNode();
			while (pUnitNode != NULL)
			{
				CvUnit* pLoopUnit = ::getUnit(pUnitNode->m_data);
				pUnitNode = pPlot->nextUnitNode(pUnitNode);

				if (pLoopUnit->getUnitType() == eUnit)
				{
					bUnitFound = true;
				}
			}

			if (!bUnitFound)
			{
				return -1;
			}
		}
	}

	// Increase cost if the XML defines that additional units will cost more
	if (0 != GC.getUnitInfo(eUnit).getAdvancedStartCostIncrease())
	{
		for (CvUnit* pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
		{
			if (pLoopUnit->getUnitType() == eUnit)
			{
				++iNumUnitType;
			}
		}

		if (!bAdd)
		{
			--iNumUnitType;
		}

		if (iNumUnitType > 0)
		{
			iCost *= 100 + GC.getUnitInfo(eUnit).getAdvancedStartCostIncrease() * iNumUnitType;
			iCost /= 100;
		}
	}

	return iCost;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Adding or removing a City
/////////////////////////////////////////////////////////////////////////////////////////////

int CvPlayer::getAdvancedStartCityCost(bool bAdd, CvPlot* pPlot) const
{
	int iNumCities = getNumCities();

	int iCost = getNewCityProductionValue();

	if (iCost < 0)
	{
		return -1;
	}

	// Valid plot?
	if (pPlot != NULL)
	{
		// Need valid plot to found on if adding
		if (bAdd)
		{
			if (!canFound(pPlot->getX(), pPlot->getY(), false))
			{
				return -1;
			}
		}
		// Need your own city present to remove
		else
		{
			if (pPlot->isCity())
			{
				if (pPlot->getPlotCity()->getOwnerINLINE() != getID())
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}
		}

		// Is there a distance limit on how far a city can be placed from a player's start/another city?
		if (GC.getDefineINT("ADVANCED_START_CITY_PLACEMENT_MAX_RANGE") > 0)
		{
			PlayerTypes eClosestPlayer = NO_PLAYER;
			int iClosestDistance = MAX_INT;

			for (int iPlayer = 0; iPlayer < MAX_CIV_PLAYERS; ++iPlayer)
			{
				CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iPlayer);

				if (kPlayer.isAlive())
				{
					CvPlot* pStartingPlot = kPlayer.getStartingPlot();

					if (NULL != pStartingPlot)
					{
						int iDistance = ::plotDistance(pPlot->getX_INLINE(), pPlot->getY_INLINE(), pStartingPlot->getX_INLINE(), pStartingPlot->getY_INLINE());
						if (iDistance <= GC.getDefineINT("ADVANCED_START_CITY_PLACEMENT_MAX_RANGE"))
						{
							if (iDistance < iClosestDistance || (iDistance == iClosestDistance && getTeam() != kPlayer.getTeam()))
							{
								iClosestDistance = iDistance;
								eClosestPlayer = kPlayer.getID();
							}
						}
					}
				}
			}

			if (NO_PLAYER == eClosestPlayer || GET_PLAYER(eClosestPlayer).getTeam() != getTeam())
			{
				return -1;
			}
			//Only allow founding a city at someone elses start point if
			//We have no cities and they have no cities.
			if ((getID() != eClosestPlayer) && ((getNumCities() > 0) || (GET_PLAYER(eClosestPlayer).getNumCities() > 0)))
			{
				return -1;
			}
		}
	}

	// Increase cost if the XML defines that additional units will cost more
	if (0 != GC.getDefineINT("ADVANCED_START_CITY_COST_INCREASE"))
	{
		if (!bAdd)
		{
			--iNumCities;
		}

		if (iNumCities > 0)
		{
			iCost *= 100 + GC.getDefineINT("ADVANCED_START_CITY_COST_INCREASE") * iNumCities;
			iCost /= 100;
		}
	}

	return iCost;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Adding or removing Population
/////////////////////////////////////////////////////////////////////////////////////////////

int CvPlayer::getAdvancedStartPopCost(bool bAdd, CvCity* pCity) const
{
	if (0 == getNumCities())
	{
		return -1;
	}

	int iCost = (getGrowthThreshold(1) * GC.getDefineINT("ADVANCED_START_POPULATION_COST")) / 100;

	if (NULL != pCity)
	{
		if (pCity->getOwnerINLINE() != getID())
		{
			return -1;
		}

		int iPopulation = pCity->getPopulation();

		// Need to have Population to remove it
		if (!bAdd)
		{
			--iPopulation;

			if (iPopulation < GC.getDefineINT("INITIAL_CITY_POPULATION") + GC.getEraInfo(GC.getGameINLINE().getStartEra()).getFreePopulation())
			{
				return -1;
			}
		}

		iCost = (getGrowthThreshold(iPopulation) * GC.getDefineINT("ADVANCED_START_POPULATION_COST")) / 100;

		// Increase cost if the XML defines that additional Pop will cost more
		if (0 != GC.getDefineINT("ADVANCED_START_POPULATION_COST_INCREASE"))
		{
			--iPopulation;

			if (iPopulation > 0)
			{
				iCost *= 100 + GC.getDefineINT("ADVANCED_START_POPULATION_COST_INCREASE") * iPopulation;
				iCost /= 100;
			}
		}
	}

	return iCost;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Adding or removing Culture
/////////////////////////////////////////////////////////////////////////////////////////////

int CvPlayer::getAdvancedStartCultureCost(bool bAdd, CvCity* pCity) const
{
	if (0 == getNumCities())
	{
		return -1;
	}

	int iCost = GC.getDefineINT("ADVANCED_START_CULTURE_COST");
	if (iCost < 0)
	{
		return -1;
	}

	if (NULL != pCity)
	{
		if (pCity->getOwnerINLINE() != getID())
		{
			return -1;
		}

		// Need to have enough culture to remove it
		if (!bAdd)
		{
			if (pCity->getCultureLevel() <= 0)
			{
				return -1;
			}
		}

		int iCulture;
		if (bAdd)
		{
			iCulture = CvCity::getCultureThreshold((CultureLevelTypes)(pCity->getCultureLevel() + 1)) - pCity->getCulture(getID());
		}
		else
		{
			iCulture = pCity->getCulture(getID()) - CvCity::getCultureThreshold((CultureLevelTypes)(pCity->getCultureLevel() - 1));
		}

		iCost *= iCulture;
		iCost /= std::max(1, GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getHurryPercent());
	}

	return iCost;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Adding or removing a Building from a city
/////////////////////////////////////////////////////////////////////////////////////////////

int CvPlayer::getAdvancedStartBuildingCost(BuildingTypes eBuilding, bool bAdd, CvCity* pCity) const
{
	if (0 == getNumCities())
	{
		return -1;
	}

	int iNumBuildingType = 0;

	int iCost = (getProductionNeeded(eBuilding) * GC.getBuildingInfo(eBuilding).getAdvancedStartCost()) / 100;

	if (iCost < 0)
	{
		return -1;
	}

	if (GC.getBuildingInfo(eBuilding).getFreeStartEra() != NO_ERA && GC.getGameINLINE().getStartEra() >=  GC.getBuildingInfo(eBuilding).getFreeStartEra())
	{
		// you get this building for free
		return -1;
	}

	if (NULL == pCity)
	{
		if (bAdd)
		{
			bool bValid = false;
			int iLoop;
			for (CvCity* pLoopCity = firstCity(&iLoop); NULL != pLoopCity; pLoopCity = nextCity(&iLoop))
			{
				if (pLoopCity->canConstruct(eBuilding))
				{
					bValid = true;
					break;
				}
			}

			if (!bValid)
			{
				return -1;
			}
		}
	}
	if (NULL != pCity)
	{
		if (pCity->getOwnerINLINE() != getID())
		{
			return -1;
		}

		iCost *= 100;
		iCost /= std::max(1, 100 + pCity->getProductionModifier(eBuilding));

		if (bAdd)
		{
			if (!pCity->canConstruct(eBuilding, true, false, false))
			{
				return -1;
			}
		}
		else
		{
			if (pCity->getNumRealBuilding(eBuilding) <= 0)
			{
				return -1;
			}

			// Check other buildings in this city and make sure none of them require this one

			// Loop through Buildings to see which are present
			for (int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
			{
				BuildingTypes eBuildingLoop = (BuildingTypes) iBuildingLoop;

				if (pCity->getNumBuilding(eBuildingLoop) > 0)
				{
					// Loop through present Building's requirements
					for (int iBuildingClassPrereqLoop = 0; iBuildingClassPrereqLoop < GC.getNumBuildingClassInfos(); iBuildingClassPrereqLoop++)
					{
						if (GC.getBuildingInfo(eBuildingLoop).isBuildingClassNeededInCity(iBuildingClassPrereqLoop))
						{
							if ((BuildingTypes)(GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(iBuildingClassPrereqLoop)) == eBuilding)
							{
								return -1;
							}
						}
					}
				}
			}
		}
	}

	// Increase cost if the XML defines that additional Buildings will cost more
	if (0 != GC.getBuildingInfo(eBuilding).getAdvancedStartCostIncrease())
	{
		iNumBuildingType = countNumBuildings(eBuilding);

		if (!bAdd)
		{
			--iNumBuildingType;
		}

		if (iNumBuildingType > 0)
		{
			iCost *= 100 + GC.getBuildingInfo(eBuilding).getAdvancedStartCostIncrease() * std::max(0, iNumBuildingType - getNumCities());
			iCost /= 100;
		}
	}

	return iCost;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Adding or removing Route
/////////////////////////////////////////////////////////////////////////////////////////////

int CvPlayer::getAdvancedStartRouteCost(RouteTypes eRoute, bool bAdd, CvPlot* pPlot) const
{
	if (0 == getNumCities())
	{
		return -1;
	}

	if (eRoute == NO_ROUTE)
	{
		return -1;
	}

	int iNumRoutes = 0;

	int iCost = GC.getRouteInfo(eRoute).getAdvancedStartCost();

	// This denotes cities may not be purchased through Advanced Start
	if (iCost < 0)
	{
		return -1;
	}

	iCost *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getBuildPercent();
	iCost /= 100;

	// No invalid plots!
	if (pPlot != NULL)
	{
		if (pPlot->isCity())
		{
			return -1;
		}

		if (bAdd)
		{
			if (pPlot->isImpassable() || pPlot->isWater())
			{
				return -1;
			}
			// Can't place twice
			if (pPlot->getRouteType() == eRoute)
			{
				return -1;
			}
		}
		else
		{
			// Need Route to remove it
			if (pPlot->getRouteType() != eRoute)
			{
				return -1;
			}
		}

		// Must be owned by me
		if (pPlot->getOwnerINLINE() != getID())
		{
			return -1;
		}
	}

	// Tech requirement
	for (int iBuildLoop = 0; iBuildLoop < GC.getNumBuildInfos(); iBuildLoop++)
	{
		if (GC.getBuildInfo((BuildTypes) iBuildLoop).getRoute() == eRoute)
		{
			if (!(GET_TEAM(getTeam()).isHasTech((TechTypes)GC.getBuildInfo((BuildTypes) iBuildLoop).getTechPrereq())))
			{
				return -1;
			}
		}
	}

	// Increase cost if the XML defines that additional units will cost more
	if (0 != GC.getRouteInfo(eRoute).getAdvancedStartCostIncrease())
	{
		int iPlotLoop = 0;
		CvPlot* pPlot;

		for (iPlotLoop = 0; iPlotLoop < GC.getMapINLINE().numPlots(); iPlotLoop++)
		{
			pPlot = GC.getMapINLINE().plotByIndex(iPlotLoop);

			if (pPlot->getRouteType() == eRoute)
			{
				++iNumRoutes;
			}
		}

		if (!bAdd)
		{
			--iNumRoutes;
		}

		if (iNumRoutes > 0)
		{
			iCost *= 100 + GC.getRouteInfo(eRoute).getAdvancedStartCostIncrease() * iNumRoutes;
			iCost /= 100;
		}
	}

	return iCost;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Adding or removing Improvement
/////////////////////////////////////////////////////////////////////////////////////////////

int CvPlayer::getAdvancedStartImprovementCost(ImprovementTypes eImprovement, bool bAdd, CvPlot* pPlot) const
{
	if (eImprovement == NO_IMPROVEMENT)
	{
		return -1;
	}

	if (0 == getNumCities())
	{
		return -1;
	}

	int iNumImprovements = 0;
	int iCost = GC.getImprovementInfo(eImprovement).getAdvancedStartCost();

		// This denotes cities may not be purchased through Advanced Start
	if (iCost < 0)
	{
		return -1;
	}

	iCost *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getBuildPercent();
	iCost /= 100;

	// Can this Improvement be on our plot?
	if (pPlot != NULL)
	{
		if (bAdd)
		{
			// Valid Plot
			if (!pPlot->canHaveImprovement(eImprovement, getTeam(), false))
			{
				return -1;
			}

			bool bValid = false;

			for (int iI = 0; iI < GC.getNumBuildInfos(); ++iI)
			{
				CvBuildInfo& kBuild = GC.getBuildInfo((BuildTypes)iI);
				ImprovementTypes eLoopImprovement = ((ImprovementTypes)(kBuild.getImprovement()));

				if (eImprovement == eLoopImprovement && canBuild(pPlot, (BuildTypes)iI))
				{
					bValid = true;

					FeatureTypes eFeature = pPlot->getFeatureType();
					if (NO_FEATURE != eFeature && kBuild.isFeatureRemove(eFeature)

//FfH: Added by Kael 11/11/2007
                      && !GC.getCivilizationInfo(getCivilizationType()).isMaintainFeatures(pPlot->getFeatureType())
//FfH: End Add

					  )
					{
						iCost += GC.getFeatureInfo(eFeature).getAdvancedStartRemoveCost();
					}

					break;
				}
			}

			if (!bValid)
			{
				return -1;
			}

			// Can't place twice
			if (pPlot->getImprovementType() == eImprovement)
			{
				return -1;
			}
		}
		else
		{
			// Need this improvement in order to remove it
			if (pPlot->getImprovementType() != eImprovement)
			{
				return -1;
			}
		}

		// Must be owned by me
		if (pPlot->getOwnerINLINE() != getID())
		{
			return -1;
		}
	}

	// Tech requirement
	bool bValid=false;

	for (int iBuildLoop = 0; iBuildLoop < GC.getNumBuildInfos(); iBuildLoop++)
	{
		if (GC.getBuildInfo((BuildTypes) iBuildLoop).getImprovement() == eImprovement)
		{
			if(GC.getBuildInfo((BuildTypes) iBuildLoop).getTechPrereq()!=NO_TECH)
			{
				if ((GET_TEAM(getTeam()).isHasTech((TechTypes)GC.getBuildInfo((BuildTypes) iBuildLoop).getTechPrereq())))
				{
					bValid=true;
				}
			}
		}
	}
	if(!bValid)
	{
		return -1;
	}

	// Increase cost if the XML defines that additional units will cost more
	if (0 != GC.getImprovementInfo(eImprovement).getAdvancedStartCostIncrease())
	{
		int iPlotLoop = 0;
		CvPlot* pPlot;

		for (iPlotLoop = 0; iPlotLoop < GC.getMapINLINE().numPlots(); iPlotLoop++)
		{
			pPlot = GC.getMapINLINE().plotByIndex(iPlotLoop);

			if (pPlot->getImprovementType() == eImprovement)
			{
				++iNumImprovements;
			}
		}

		if (!bAdd)
		{
			--iNumImprovements;
		}

		if (iNumImprovements > 0)
		{
			iCost *= 100 + GC.getImprovementInfo(eImprovement).getAdvancedStartCostIncrease() * iNumImprovements;
			iCost /= 100;
		}
	}

	return iCost;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Adding or removing Tech
/////////////////////////////////////////////////////////////////////////////////////////////

int CvPlayer::getAdvancedStartTechCost(TechTypes eTech, bool bAdd) const
{
	if (eTech == NO_TECH)
	{
		return -1;
	}

	if (0 == getNumCities())
	{
		return -1;
	}

	int iNumTechs = 0;

	int iCost = (GET_TEAM(getTeam()).getResearchCost(eTech) * GC.getTechInfo(eTech).getAdvancedStartCost()) / 100;
	if (iCost < 0)
	{
		return -1;
	}

	if (bAdd)
	{
		if (!canResearch(eTech, false))
		{
			return -1;
		}
	}
	else if (!bAdd)
	{
		if (!GET_TEAM(getTeam()).isHasTech(eTech))
		{
			return -1;
		}

		// Search through all techs to see if any of the currently owned ones requires this tech
		for (int iTechLoop = 0; iTechLoop < GC.getNumTechInfos(); iTechLoop++)
		{
			TechTypes eTechLoop = (TechTypes) iTechLoop;

			if (GET_TEAM(getTeam()).isHasTech(eTechLoop))
			{
				int iPrereqLoop;

				// Or Prereqs
				for (iPrereqLoop = 0; iPrereqLoop < GC.getNUM_OR_TECH_PREREQS(); iPrereqLoop++)
				{
					if (GC.getTechInfo(eTechLoop).getPrereqOrTechs(iPrereqLoop) == eTech)
					{
						return -1;
					}
				}

				// And Prereqs
				for (iPrereqLoop = 0; iPrereqLoop < GC.getNUM_AND_TECH_PREREQS(); iPrereqLoop++)
				{
					if (GC.getTechInfo(eTechLoop).getPrereqAndTechs(iPrereqLoop) == eTech)
					{
						return -1;
					}
				}
			}
		}

		// If player has placed anything on the map which uses this tech then you cannot remove it
		int iLoop;

		// Units
		CvUnit* pLoopUnit;
		for(pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
		{
			if (pLoopUnit->getUnitInfo().getPrereqAndTech() == eTech)
			{
				return -1;
			}

			for (int iI = 0; iI < GC.getNUM_UNIT_AND_TECH_PREREQS(); iI++)
			{
				if (pLoopUnit->getUnitInfo().getPrereqAndTechs(iI) == eTech)
				{
					return -1;
				}
			}
		}

		// Cities
		CvCity* pLoopCity;
		for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
		{
			// All Buildings
			for (int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
			{
				BuildingTypes eBuilding = (BuildingTypes) iBuildingLoop;

				if (pLoopCity->getNumRealBuilding(eBuilding) > 0)
				{
					if (GC.getBuildingInfo(eBuilding).getPrereqAndTech() == eTech)
					{
						return -1;
					}

					for (int iI = 0; iI < GC.getNUM_BUILDING_AND_TECH_PREREQS(); iI++)
					{
						if (GC.getBuildingInfo(eBuilding).getPrereqAndTechs(iI) == eTech)
						{
							return -1;
						}
					}
				}
			}
		}

	}

	// Increase cost if the XML defines that additional units will cost more
	if (0 != GC.getTechInfo(eTech).getAdvancedStartCostIncrease())
	{
		for (int iTechLoop = 0; iTechLoop < GC.getNumTechInfos(); iTechLoop++)
		{
			if (GET_TEAM(getTeam()).isHasTech((TechTypes) iTechLoop))
			{
				++iNumTechs;
			}
		}

		if (!bAdd)
		{
			--iNumTechs;
		}

		if (iNumTechs > 0)
		{
			iCost *= 100 + GC.getTechInfo(eTech).getAdvancedStartCostIncrease() * iNumTechs;
			iCost /= 100;
		}
	}

	return iCost;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Adding or removing Visibility
/////////////////////////////////////////////////////////////////////////////////////////////

int CvPlayer::getAdvancedStartVisibilityCost(bool bAdd, CvPlot* pPlot) const
{
	if (0 == getNumCities())
	{
		return -1;
	}

	int iNumVisiblePlots = 0;
	int iCost = GC.getDefineINT("ADVANCED_START_VISIBILITY_COST");

	// This denotes Visibility may not be purchased through Advanced Start
	if (iCost == -1)
	{
		return -1;
	}

	// Valid Plot?
	if (pPlot != NULL)
	{
		if (bAdd)
		{
			if (pPlot->isRevealed(getTeam(), false))
			{
				return -1;
			}
			if (!pPlot->isAdjacentRevealed(getTeam()))
			{
				return -1;
			}
		}
		else
		{
			if (!pPlot->isRevealed(getTeam(), false))
			{
				return -1;
			}
		}
	}

	// Increase cost if the XML defines that additional units will cost more
	if (0 != GC.getDefineINT("ADVANCED_START_VISIBILITY_COST_INCREASE"))
	{
		int iPlotLoop = 0;
		CvPlot* pPlot;

		for (iPlotLoop = 0; iPlotLoop < GC.getMapINLINE().numPlots(); iPlotLoop++)
		{
			pPlot = GC.getMapINLINE().plotByIndex(iPlotLoop);

			if (pPlot->isRevealed(getTeam(), false))
			{
				++iNumVisiblePlots;
			}
		}

		if (!bAdd)
		{
			--iNumVisiblePlots;
		}

		if (iNumVisiblePlots > 0)
		{
			iCost *= 100 + GC.getDefineINT("ADVANCED_START_VISIBILITY_COST_INCREASE") * iNumVisiblePlots;
			iCost /= 100;
		}
	}

	return iCost;
}

void CvPlayer::doWarnings()
{
	CvCity* pNearestCity;
	CvPlot* pLoopPlot;
	CvUnit* pLoopUnit;
	wchar szBuffer[1024];
	int iMaxCount;
	int iI;

	if(!isHuman())
		return;

	//update enemy unit in your territory glow
	int iLoop;
	for(pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{
		//update glow
		gDLL->getEntityIFace()->updateEnemyGlow(pLoopUnit->getUnitEntity());
	}

	//update enemy units close to your territory
	iMaxCount = range(((getNumCities() + 4) / 7), 2, 5);
	for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		if (iMaxCount == 0)
		{
			break;
		}

		pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

		if (pLoopPlot->isAdjacentPlayer(getID()))
		{
			if (!(pLoopPlot->isCity()))
			{
				if (pLoopPlot->isVisible(getTeam(), false))
				{
					CvUnit *pUnit = pLoopPlot->getVisibleEnemyDefender(getID());
					if (pUnit != NULL)
					{
/*************************************************************************************************/
/**	ADDON (multibarb) Sephi 							                                        **/
/**																								**/
/*************************************************************************************************/
/**								---- Start Original Code ----									**
						if (!pUnit->isAnimal())
/**								----  End Original Code  ----									**/
                        if (pUnit->plot()->getOwnerINLINE() == getID() && !(pUnit->isAnimal() && pUnit->isBarbarian())
                         && !(pUnit->isBarbarian() && pUnit->AI_getGroupflag() == GROUPFLAG_MANADEFENDER_DEF))
/*************************************************************************************************/
/**	Tweak									END													**/
/*************************************************************************************************/
						{
							pNearestCity = GC.getMapINLINE().findCity(pLoopPlot->getX_INLINE(), pLoopPlot->getY_INLINE(), getID(), NO_TEAM, !(pLoopPlot->isWater()));

							if (pNearestCity != NULL)
							{
/*************************************************************************************************/
/**	ADDON (display tweak) merged Sephi 							01/04/09	Xienwolf Tweak		**/
/**																								**/
/**							Displays WHICH enemy is near your territory							**/
/*************************************************************************************************/
/**								---- Start Original Code ----									**
								swprintf(szBuffer, gDLL->getText("TXT_KEY_MISC_ENEMY_TROOPS_SPOTTED", pNearestCity->getNameKey()).GetCString());
/**								----  End Original Code  ----									**/
								swprintf(szBuffer, gDLL->getText("TXT_KEY_MISC_ENEMY_TROOPS_SPOTTED", pNearestCity->getNameKey(), GET_PLAYER(pUnit->getVisualOwner(getTeam())).getCivilizationDescriptionKey()));
/*************************************************************************************************/
/**	Tweak									END													**/
/*************************************************************************************************/
								gDLL->getInterfaceIFace()->addMessage(getID(), true, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_ENEMY_TROOPS", MESSAGE_TYPE_INFO, pUnit->getButton(), (ColorTypes)GC.getInfoTypeForString("COLOR_RED"), pLoopPlot->getX_INLINE(), pLoopPlot->getY_INLINE(), true, true);

								iMaxCount--;
							}
						}
					}
				}
			}
		}
	}
}


void CvPlayer::verifyGoldCommercePercent()
{
	int iLoops = 0;
	while ((getGold() + calculateGoldRate()) < 0)
	{
		changeCommercePercent(COMMERCE_GOLD, GC.getDefineINT("COMMERCE_PERCENT_CHANGE_INCREMENTS"));

		if (getCommercePercent(COMMERCE_GOLD) >= GC.getDefineINT("MAX_COMMERCE_PERCENT"))
		{
			break;
		}
		//added
		if(getCommercePercent(COMMERCE_GOLD) >= GC.getDefineINT("MAX_COMMERCE_PERCENT_GOLD"))
		{
			break;
		}
		//added
		++iLoops;
		if(iLoops > 100)
			break;
	}
}


void CvPlayer::processCivics(CivicTypes eCivic, int iChange)
{
	int iI, iJ;

	changeGreatPeopleRateModifier(GC.getCivicInfo(eCivic).getGreatPeopleRateModifier() * iChange);
	changeGreatGeneralRateModifier(GC.getCivicInfo(eCivic).getGreatGeneralRateModifier() * iChange);
	changeDomesticGreatGeneralRateModifier(GC.getCivicInfo(eCivic).getDomesticGreatGeneralRateModifier() * iChange);
	changeStateReligionGreatPeopleRateModifier(GC.getCivicInfo(eCivic).getStateReligionGreatPeopleRateModifier() * iChange);
	changeDistanceMaintenanceModifier(GC.getCivicInfo(eCivic).getDistanceMaintenanceModifier() * iChange);
	changeNumCitiesMaintenanceModifier(GC.getCivicInfo(eCivic).getNumCitiesMaintenanceModifier() * iChange);
	changeCorporationMaintenanceModifier(GC.getCivicInfo(eCivic).getCorporationMaintenanceModifier() * iChange);
	changeExtraHealth(GC.getCivicInfo(eCivic).getExtraHealth() * iChange);
	changeFreeExperience(GC.getCivicInfo(eCivic).getFreeExperience() * iChange);
	changeWorkerSpeedModifier(GC.getCivicInfo(eCivic).getWorkerSpeedModifier() * iChange);
	changeImprovementUpgradeRateModifier(GC.getCivicInfo(eCivic).getImprovementUpgradeRateModifier() * iChange);
	changeMilitaryProductionModifier(GC.getCivicInfo(eCivic).getMilitaryProductionModifier() * iChange);
	changeBaseFreeUnits(GC.getCivicInfo(eCivic).getBaseFreeUnits() * iChange);
	changeBaseFreeMilitaryUnits(GC.getCivicInfo(eCivic).getBaseFreeMilitaryUnits() * iChange);
	changeFreeUnitsPopulationPercent(GC.getCivicInfo(eCivic).getFreeUnitsPopulationPercent() * iChange);
	changeFreeMilitaryUnitsPopulationPercent(GC.getCivicInfo(eCivic).getFreeMilitaryUnitsPopulationPercent() * iChange);
	changeGoldPerUnit(GC.getCivicInfo(eCivic).getGoldPerUnit() * iChange);
	changeGoldPerMilitaryUnit(GC.getCivicInfo(eCivic).getGoldPerMilitaryUnit() * iChange);
	changeHappyPerMilitaryUnit(GC.getCivicInfo(eCivic).getHappyPerMilitaryUnit() * iChange);
	changeMilitaryFoodProductionCount((GC.getCivicInfo(eCivic).isMilitaryFoodProduction()) ? iChange : 0);
	changeMaxConscript(getWorldSizeMaxConscript(eCivic) * iChange);
	changeNoUnhealthyPopulationCount((GC.getCivicInfo(eCivic).isNoUnhealthyPopulation()) ? iChange : 0);
	changeBuildingOnlyHealthyCount((GC.getCivicInfo(eCivic).isBuildingOnlyHealthy()) ? iChange : 0);
	changeLargestCityHappiness(GC.getCivicInfo(eCivic).getLargestCityHappiness() * iChange);
	changeWarWearinessModifier(GC.getCivicInfo(eCivic).getWarWearinessModifier() * iChange);
	changeFreeSpecialist(GC.getCivicInfo(eCivic).getFreeSpecialist() * iChange);
	changeTradeRoutes(GC.getCivicInfo(eCivic).getTradeRoutes() * iChange);
	changeNoForeignTradeCount(GC.getCivicInfo(eCivic).isNoForeignTrade() * iChange);
	changeNoCorporationsCount(GC.getCivicInfo(eCivic).isNoCorporations() * iChange);
	changeNoForeignCorporationsCount(GC.getCivicInfo(eCivic).isNoForeignCorporations() * iChange);
	changeStateReligionCount((GC.getCivicInfo(eCivic).isStateReligion()) ? iChange : 0);
	changeNoNonStateReligionSpreadCount((GC.getCivicInfo(eCivic).isNoNonStateReligionSpread()) ? iChange : 0);
	changeStateReligionHappiness(GC.getCivicInfo(eCivic).getStateReligionHappiness() * iChange);
	changeNonStateReligionHappiness(GC.getCivicInfo(eCivic).getNonStateReligionHappiness() * iChange);
	changeStateReligionUnitProductionModifier(GC.getCivicInfo(eCivic).getStateReligionUnitProductionModifier() * iChange);
	changeStateReligionBuildingProductionModifier(GC.getCivicInfo(eCivic).getStateReligionBuildingProductionModifier() * iChange);
	changeStateReligionFreeExperience(GC.getCivicInfo(eCivic).getStateReligionFreeExperience() * iChange);
	changeExpInBorderModifier(GC.getCivicInfo(eCivic).getExpInBorderModifier() * iChange);
	changeCaptureCityGoldPercentChange(GC.getCivicInfo(eCivic).getCaptureCityGoldPercentChange() * iChange);
	changeFreeXPFromCombat(GC.getCivicInfo(eCivic).getXPPerCombatChange() * iChange);
	changePillagingGold(GC.getCivicInfo(eCivic).getPillagePercentChange() * iChange);

	if (GC.getCivicInfo(eCivic).getGlobalAttitudeChange() != 0)
	{
		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (!GET_PLAYER((PlayerTypes)iI).isHuman() && GET_PLAYER((PlayerTypes)iI).isAlive() && (iI != getID()))
			{
				GET_PLAYER((PlayerTypes)iI).AI_changeAttitudeExtra(getID(), (GC.getCivicInfo(eCivic).getGlobalAttitudeChange() * iChange));
			}
		}
	}

//FfH Civics: Added by Kael 08/11/2007
    changeCoastalTradeRoutes(GC.getCivicInfo(eCivic).getCoastalTradeRoutes() * iChange);
    changeEnslavementChance(GC.getCivicInfo(eCivic).getEnslavementChance() * iChange);
    changeNoDiplomacyWithEnemies(GC.getCivicInfo(eCivic).isNoDiplomacyWithEnemies() * iChange);
    if (iChange == -1)
    {
        for (int iI = 0; iI < GC.getNumVoteSourceInfos(); ++iI)
        {
            if (GC.getVoteSourceInfo((VoteSourceTypes)iI).getCivic() == eCivic)
            {
                if (GC.getGameINLINE().getSecretaryGeneral((VoteSourceTypes)iI) == getTeam())
                {
                    GC.getGameINLINE().clearSecretaryGeneral((VoteSourceTypes)iI);
                }
            }
        }
    }
//FfH: End Add

	for (iI = 0; iI < NUM_YIELD_TYPES; iI++)
	{
		changeYieldRateModifier(((YieldTypes)iI), (GC.getCivicInfo(eCivic).getYieldModifier(iI) * iChange));
		changeCapitalYieldRateModifier(((YieldTypes)iI), (GC.getCivicInfo(eCivic).getCapitalYieldModifier(iI) * iChange));
		changeTradeYieldModifier(((YieldTypes)iI), (GC.getCivicInfo(eCivic).getTradeYieldModifier(iI) * iChange));
	}

	for (iI = 0; iI < NUM_COMMERCE_TYPES; iI++)
	{
		changeCommerceRateModifier(((CommerceTypes)iI), (GC.getCivicInfo(eCivic).getCommerceModifier(iI) * iChange));
		changeCapitalCommerceRateModifier(((CommerceTypes)iI), (GC.getCivicInfo(eCivic).getCapitalCommerceModifier(iI) * iChange));
		changeCommerceChangesPerCultureLevel(((CommerceTypes)iI), (GC.getCivicInfo(eCivic).getCommerceChangesPerCultureLevel(iI) * iChange));		
		changeSpecialistExtraCommerce(((CommerceTypes)iI), (GC.getCivicInfo(eCivic).getSpecialistExtraCommerce(iI) * iChange));
	}

	for (iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		BuildingTypes eOurBuilding = (BuildingTypes)GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(iI);
		if (NO_BUILDING != eOurBuilding)
		{
			changeExtraBuildingHappiness(eOurBuilding, (GC.getCivicInfo(eCivic).getBuildingHappinessChanges(iI) * iChange));
			changeExtraBuildingHealth(eOurBuilding, (GC.getCivicInfo(eCivic).getBuildingHealthChanges(iI) * iChange));
		}
	}

	for (iI = 0; iI < GC.getNumFeatureInfos(); iI++)
	{
		changeFeatureHappiness(((FeatureTypes)iI), (GC.getCivicInfo(eCivic).getFeatureHappinessChanges(iI) * iChange));
	}

	for (iI = 0; iI < GC.getNumHurryInfos(); iI++)
	{
		changeHurryCount(((HurryTypes)iI), ((GC.getCivicInfo(eCivic).isHurry(iI)) ? iChange : 0));
	}

	for (iI = 0; iI < GC.getNumSpecialBuildingInfos(); iI++)
	{
		changeSpecialBuildingNotRequiredCount(((SpecialBuildingTypes)iI), ((GC.getCivicInfo(eCivic).isSpecialBuildingNotRequired(iI)) ? iChange : 0));
	}

	for (iI = 0; iI < GC.getNumSpecialistInfos(); iI++)
	{
		changeSpecialistValidCount(((SpecialistTypes)iI), ((GC.getCivicInfo(eCivic).isSpecialistValid(iI)) ? iChange : 0));
	}

	for (iI = 0; iI < GC.getNumImprovementInfos(); iI++)
	{
		for (iJ = 0; iJ < NUM_YIELD_TYPES; iJ++)
		{
			changeImprovementYieldChange(((ImprovementTypes)iI), ((YieldTypes)iJ), (GC.getCivicInfo(eCivic).getImprovementYieldChanges(iI, iJ) * iChange));
		}
	}

	changeReducedEquipmentCost(GC.getCivicInfo(eCivic).getReducedEquipmentCost() * iChange);
	changeImprovementDiscountPercent(GC.getCivicInfo(eCivic).getImprovementDiscountPercent() * iChange);
	changeGreatPersonRatePerCulture(GC.getCivicInfo(eCivic).getGreatPersonRatePerCulture() * iChange);
	changeHealthRatePerCulture(GC.getCivicInfo(eCivic).getHealthRatePerCulture() * iChange);
	updateBanditLordsCivic(iChange);
}

void CvPlayer::showMissedMessages()
{
	CvMessageQueue::iterator it = m_listGameMessages.begin();
	while (it != m_listGameMessages.end())
	{
		CvTalkingHeadMessage& msg = *it;
		if (!msg.getShown())
		{
			msg.setShown(true);
			gDLL->getInterfaceIFace()->showMessage(msg);
		}
		++it;
	}
}

bool CvPlayer::isPbemNewTurn() const
{
	return m_bPbemNewTurn;
}

void CvPlayer::setPbemNewTurn(bool bNew)
{
	m_bPbemNewTurn = bNew;
}



//
// read object from a stream
// used during load
//
void CvPlayer::read(FDataStreamBase* pStream)
{
	int iI;

	// Init data before load
	reset();

	uint uiFlag=0;
	pStream->Read(&uiFlag);	// flags for expansion

	pStream->Read(&m_iStartingX);
	pStream->Read(&m_iStartingY);
	pStream->Read(&m_iTotalPopulation);
	pStream->Read(&m_iTotalLand);
	pStream->Read(&m_iTotalLandScored);
	pStream->Read(&m_iGold);
	pStream->Read(&m_iCrime);
	pStream->Read(&m_iCaptureCityGoldPercentChange);
	pStream->Read(&m_iGoldPerTurn);
	pStream->Read(&m_iAdvancedStartPoints);
	pStream->Read(&m_iGoldenAgeTurns);
	pStream->Read(&m_iNumUnitGoldenAges);
	pStream->Read(&m_iStrikeTurns);
	pStream->Read(&m_iAnarchyTurns);
	pStream->Read(&m_iMaxAnarchyTurns);
	pStream->Read(&m_iAnarchyModifier);
	pStream->Read(&m_iGoldenAgeModifier);
	pStream->Read(&m_iGlobalHurryModifier);
	pStream->Read(&m_iGreatPeopleCreated);
	pStream->Read(&m_iGreatGeneralsCreated);
	pStream->Read(&m_iGreatPeopleThresholdModifier);
	pStream->Read(&m_iGreatGeneralsThresholdModifier);
	pStream->Read(&m_iGreatPeopleRateModifier);
	pStream->Read(&m_iGreatGeneralRateModifier);
	pStream->Read(&m_iDomesticGreatGeneralRateModifier);
	pStream->Read(&m_iStateReligionGreatPeopleRateModifier);
	pStream->Read(&m_iMaxGlobalBuildingProductionModifier);
	pStream->Read(&m_iMaxTeamBuildingProductionModifier);
	pStream->Read(&m_iMaxPlayerBuildingProductionModifier);
	pStream->Read(&m_iFreeExperience);
	pStream->Read(&m_iFeatureProductionModifier);
	pStream->Read(&m_iWorkerSpeedModifier);
	pStream->Read(&m_iImprovementUpgradeRateModifier);
	pStream->Read(&m_iMilitaryProductionModifier);
	pStream->Read(&m_iSpaceProductionModifier);
	pStream->Read(&m_iCityDefenseModifier);
	pStream->Read(&m_iNumNukeUnits);
	pStream->Read(&m_iNumOutsideUnits);
	pStream->Read(&m_iBaseFreeUnits);
	pStream->Read(&m_iBaseFreeMilitaryUnits);
	pStream->Read(&m_iFreeUnitsPopulationPercent);
	pStream->Read(&m_iFreeMilitaryUnitsPopulationPercent);
	pStream->Read(&m_iGoldPerUnit);
	pStream->Read(&m_iGoldPerMilitaryUnit);
	pStream->Read(&m_iExtraUnitCost);
	pStream->Read(&m_iNumMilitaryUnits);
	pStream->Read(&m_iNumSummonUnits);
	pStream->Read(&m_iNumGarrisionUnits);
	pStream->Read(&m_iHappyPerMilitaryUnit);
	pStream->Read(&m_iMilitaryFoodProductionCount);
	pStream->Read(&m_iConscriptCount);
	pStream->Read(&m_iMaxConscript);
	pStream->Read(&m_iHighestUnitLevel);
	pStream->Read(&m_iOverflowResearch);
	pStream->Read(&m_iNoUnhealthyPopulationCount);
	pStream->Read(&m_iExpInBorderModifier);
	pStream->Read(&m_iBuildingOnlyHealthyCount);
	pStream->Read(&m_iDistanceMaintenanceModifier);
	pStream->Read(&m_iNumCitiesMaintenanceModifier);
	pStream->Read(&m_iCorporationMaintenanceModifier);
	pStream->Read(&m_iTotalMaintenance);
	pStream->Read(&m_iUpkeepModifier);
	pStream->Read(&m_iLevelExperienceModifier);
	pStream->Read(&m_iExtraHealth);
	pStream->Read(&m_iBuildingGoodHealth);
	pStream->Read(&m_iBuildingBadHealth);
	pStream->Read(&m_iExtraHappiness);
	pStream->Read(&m_iBuildingHappiness);
	pStream->Read(&m_iLargestCityHappiness);
	pStream->Read(&m_iWarWearinessPercentAnger);
	pStream->Read(&m_iWarWearinessModifier);
	pStream->Read(&m_iFreeSpecialist);
	pStream->Read(&m_iNoForeignTradeCount);
	pStream->Read(&m_iNoCorporationsCount);
	pStream->Read(&m_iNoForeignCorporationsCount);
	pStream->Read(&m_iCoastalTradeRoutes);
	pStream->Read(&m_iTradeRoutes);
	pStream->Read(&m_iRevolutionTimer);
	pStream->Read(&m_iConversionTimer);
	pStream->Read(&m_iStateReligionCount);
	pStream->Read(&m_iNoNonStateReligionSpreadCount);
	pStream->Read(&m_iStateReligionHappiness);
	pStream->Read(&m_iNonStateReligionHappiness);
	pStream->Read(&m_iStateReligionUnitProductionModifier);
	pStream->Read(&m_iStateReligionBuildingProductionModifier);
	pStream->Read(&m_iStateReligionFreeExperience);
	pStream->Read(&m_iCapitalCityID);
	pStream->Read(&m_iCitiesLost);
	pStream->Read(&m_iWinsVsBarbs);
	pStream->Read(&m_iAssets);
	pStream->Read(&m_iPower);
	pStream->Read(&m_iPopulationScore);
	pStream->Read(&m_iLandScore);
	pStream->Read(&m_iWondersScore);
	pStream->Read(&m_iTechScore);
	pStream->Read(&m_iCombatExperience);

	pStream->Read(&m_bAlive);
	pStream->Read(&m_bEverAlive);
	pStream->Read(&m_bTurnActive);
	pStream->Read(&m_bAutoMoves);
	pStream->Read(&m_bEndTurn);
	pStream->Read(&m_bPbemNewTurn);
	pStream->Read(&m_bExtendedGame);
	pStream->Read(&m_bFoundedFirstCity);
	pStream->Read(&m_bStrike);
/*************************************************************************************************/
/**	BETTER AI (New Functions Definition) Sephi                                 					**/
/**																								**/
/**						                                            							**/
/*************************************************************************************************/
    pStream->Read(&m_bConquestMode);
/**	New Esus                                                                 					**/
    pStream->Read(&m_iPrestige);
    pStream->Read((int*)&m_eFavoriteReligion);
    pStream->Read(&m_iTowerVicFlag);
	pStream->Read(&m_bRangedAttackMode);
    pStream->Read(&m_iArcane);
    pStream->Read(&m_iArcaneLevel);
    pStream->Read(&m_iFaith);
    pStream->Read(&m_iFaithIncome);
    pStream->Read(&m_iReligionsSpread);	
    pStream->Read(&m_iFaithMod);
    pStream->Read(&m_iFaithUpkeep);
    pStream->Read(&m_iManaBonus2);
    pStream->Read(&m_iManaBonus3);
    pStream->Read(&m_iManaBonus4);
    pStream->Read(&m_iBarbarianPower);
    pStream->Read(&m_iBarbarianPoints);
    pStream->Read(&m_iBarbarianValue2);
    pStream->Read(&m_iBarbarianValue3);
    pStream->Read(&m_iMana);
    pStream->Read(&m_iManaIncome);
    pStream->Read(&m_iManaUpkeep);
    pStream->Read(&m_iPopulationLimit);
    pStream->Read(&m_iCityStateType);
    pStream->Read(&m_iCityStateCounter);
    pStream->Read(&m_iPurityCounter);
    pStream->Read(&m_iPurityCounterCache1);
	pStream->Read(&m_iUnitSupportLimit);
	pStream->Read(&m_iUnitSupportUsed);
    pStream->Read(&m_iReducedEquipmentCost);
	pStream->Read(&m_iImprovementDiscountPercent);
    pStream->Read(&m_iGreatPersonRatePerCulture);
    pStream->Read(&m_iHealthRatePerCulture);
    pStream->Read(&m_iWillpowerChange);
    pStream->Read(&m_iReducedTerraformCost);
    pStream->Read(&m_iReducedEnchantmentCost);
    pStream->Read(&m_iReducedSummonCost);
    pStream->Read(&m_iIncreaseTerraformTiles);
    pStream->Read(&m_iIncreaseHostileTerraformTiles);
    pStream->Read(&m_iIncreaseSummonXP);
    pStream->Read(&m_iCurrentMagicRitual);
    pStream->Read(&m_iCurrentMagicRitualManaNeeded);
	pStream->Read(&m_iMaxMana);
	pStream->Read(&m_iManaFromSpellResearch);
	pStream->Read(&m_iManaFromNodes);
	pStream->Read(&m_iIncreaseStrengthOfRegularUnits);		
	pStream->Read(&m_iSpellResearchFocus);
    pStream->Read(&m_iAI_lostCitiesRevolutionMemory);
    pStream->Read(&m_iManaCreationModifier);
    pStream->Read(&m_iResistHostileTerraforming);
    pStream->Read(&m_iTerraformPlan);
    pStream->Read(&m_iIgnoreUnHealthFromImprovements);
    pStream->Read(&m_iGlobalCulture);	
    pStream->Read(&m_iTotalCityInfastructureLevel);	
    pStream->Read(&m_iTotalTraitsFromCulture);	
    pStream->Read(&m_iExtraImprovementPerCity);	
    pStream->Read(&m_iExtraManaNodesPercent);	
    pStream->Read(&m_iReducedHurryCostBuilding);	
    pStream->Read(&m_iExtraSupportUnitLimit);	
    pStream->Read(&m_iExtraBuildingCapturedChance);	
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
//FfH Traits: Added by Kael 08/02/2007
	pStream->Read(&m_bAdaptive);
	pStream->Read(&m_bAgnostic);
	pStream->Read(&m_bAssimilation);
	pStream->Read(&m_bDeclaringWar);
	pStream->Read(&m_bDisableHuman);
	pStream->Read(&m_bHideUnits);
	pStream->Read(&m_bIgnoreFood);
	pStream->Read(&m_bInsane);
	pStream->Read(&m_bSeeInvisible);
	pStream->Read(&m_bSprawling);
	pStream->Read(&m_iAlignment);
	pStream->Read(&m_iDisableProduction);
	pStream->Read(&m_iDisableResearch);
	pStream->Read(&m_iDisableSpellcasting);
	pStream->Read(&m_iDiscoverRandModifier);
	pStream->Read(&m_iEnslavementChance);
	pStream->Read(&m_iFreeXPFromCombat);
	pStream->Read(&m_iHealChange);
	pStream->Read(&m_iHealChangeEnemy);
	pStream->Read(&m_iMaxCities);
	pStream->Read(&m_iNoDiplomacyWithEnemies);
	pStream->Read(&m_iPillagingGold);
	pStream->Read(&m_iPlayersKilled);
	pStream->Read(&m_iRealPlayer);
	pStream->Read(&m_iResistEnemyModify);
	pStream->Read(&m_iResistModify);
	pStream->Read(&m_iSanctuaryTimer);
	pStream->Read(&m_iStartingGold);
	pStream->Read(&m_iSummonCostReduction);
	pStream->Read(&m_iTaxesModifier);
	pStream->Read(&m_iFoodPerPopulationModifier);
	pStream->Read(&m_iTempPlayerTimer);
	pStream->Read(&m_iUpgradeCostModifier);
	pStream->Read(&m_iProductionCostReductionCapitalBuilding);
	pStream->Read(&m_iUnitSupportModifier);
	pStream->Read(GC.getNumTraitInfos(), m_pbTraits);
	for (iI=0;iI<GC.getNumSpecialistInfos();iI++)
	{
		pStream->Read(NUM_COMMERCE_TYPES, m_ppaaiSpecialistTypeExtraCommerce[iI]);
	}
//FfH: End Add

	pStream->Read((int*)&m_eID);
	pStream->Read((int*)&m_ePersonalityType);
	pStream->Read((int*)&m_eCurrentEra);
	pStream->Read((int*)&m_eLastStateReligion);
	pStream->Read((int*)&m_eParent);
	updateTeamType(); //m_eTeamType not saved
	updateHuman();

	pStream->Read(NUM_YIELD_TYPES, m_aiSeaPlotYield);
	pStream->Read(NUM_YIELD_TYPES, m_aiYieldRateModifier);
	pStream->Read(NUM_YIELD_TYPES, m_aiCapitalYieldRateModifier);
	pStream->Read(NUM_YIELD_TYPES, m_aiExtraYieldThreshold);
	pStream->Read(NUM_YIELD_TYPES, m_aiTradeYieldModifier);
	pStream->Read(NUM_COMMERCE_TYPES, m_aiFreeCityCommerce);
	pStream->Read(NUM_COMMERCE_TYPES, m_aiCommercePercent);
	pStream->Read(NUM_COMMERCE_TYPES, m_aiCommerceRate);
	pStream->Read(NUM_COMMERCE_TYPES, m_aiCommerceRateModifier);
	pStream->Read(NUM_COMMERCE_TYPES, m_aiCapitalCommerceRateModifier);
	pStream->Read(NUM_COMMERCE_TYPES, m_aiCommerceChangesPerCultureLevel);
	pStream->Read(NUM_COMMERCE_TYPES, m_aiStateReligionBuildingCommerce);
	pStream->Read(NUM_COMMERCE_TYPES, m_aiSpecialistExtraCommerce);
	pStream->Read(NUM_COMMERCE_TYPES, m_aiCommerceFlexibleCount);
	pStream->Read(MAX_PLAYERS, m_aiGoldPerTurnByPlayer);
	pStream->Read(MAX_TEAMS, m_aiEspionageSpendingWeightAgainstTeam);

	pStream->Read(NUM_FEAT_TYPES, m_abFeatAccomplished);
	pStream->Read(NUM_PLAYEROPTION_TYPES, m_abOptions);

	pStream->ReadString(m_szScriptData);

	FAssertMsg((0 < GC.getNumBonusInfos()), "GC.getNumBonusInfos() is not greater than zero but it is expected to be in CvPlayer::read");
	pStream->Read(GC.getNumBonusInfos(), m_paiBonusExport);
	pStream->Read(GC.getNumBonusInfos(), m_paiBonusImport);
	pStream->Read(GC.getNumImprovementInfos(), m_paiImprovementCount);
	pStream->Read(GC.getNumBuildingInfos(), m_paiFreeBuildingCount);
	pStream->Read(GC.getNumBuildingInfos(), m_paiExtraBuildingHappiness);
	pStream->Read(GC.getNumBuildingInfos(), m_paiExtraBuildingHealth);
	pStream->Read(GC.getNumFeatureInfos(), m_paiFeatureHappiness);
	pStream->Read(GC.getNumUnitClassInfos(), m_paiUnitClassCount);
	pStream->Read(GC.getNumUnitClassInfos(), m_paiUnitClassMaking);
	pStream->Read(GC.getNumBuildingClassInfos(), m_paiBuildingClassCount);
	pStream->Read(GC.getNumBuildingClassInfos(), m_paiBuildingClassMaking);
	pStream->Read(GC.getNumHurryInfos(), m_paiHurryCount);
	pStream->Read(GC.getNumSpecialBuildingInfos(), m_paiSpecialBuildingNotRequiredCount);
	pStream->Read(GC.getNumCivicOptionInfos(), m_paiHasCivicOptionCount);
	pStream->Read(GC.getNumCivicOptionInfos(), m_paiNoCivicUpkeepCount);
	pStream->Read(GC.getNumReligionInfos(), m_paiHasReligionCount);
	pStream->Read(GC.getNumCorporationInfos(), m_paiHasCorporationCount);
	pStream->Read(GC.getNumUpkeepInfos(), m_paiUpkeepCount);
	pStream->Read(GC.getNumSpecialistInfos(), m_paiSpecialistValidCount);

	FAssertMsg((0 < GC.getNumTechInfos()), "GC.getNumTechInfos() is not greater than zero but it is expected to be in CvPlayer::read");
	pStream->Read(GC.getNumTechInfos(), m_pabResearchingTech);

	pStream->Read(GC.getNumVoteSourceInfos(), m_pabLoyalMember);

	for (iI=0;iI<GC.getNumCivicOptionInfos();iI++)
	{
		pStream->Read((int*)&m_paeCivics[iI]);
	}

	for (iI=0;iI<GC.getNumSpecialistInfos();iI++)
	{
		pStream->Read(NUM_YIELD_TYPES, m_ppaaiSpecialistExtraYield[iI]);
	}

	for (iI=0;iI<GC.getNumImprovementInfos();iI++)
	{
		pStream->Read(NUM_YIELD_TYPES, m_ppaaiImprovementYieldChange[iI]);
	}

	m_groupCycle.Read(pStream);
	m_researchQueue.Read(pStream);

	{
		m_cityNames.clear();
		CvWString szBuffer;
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			pStream->ReadString(szBuffer);
			m_cityNames.insertAtEnd(szBuffer);
		}
	}

	ReadStreamableFFreeListTrashArray(m_plotGroups, pStream);
	ReadStreamableFFreeListTrashArray(m_cities, pStream);
	ReadStreamableFFreeListTrashArray(m_units, pStream);
	ReadStreamableFFreeListTrashArray(m_selectionGroups, pStream);
	ReadStreamableFFreeListTrashArray(m_eventsTriggered, pStream);
/*************************************************************************************************/
/**	ADDON (Combatauras) Sephi																	**/
/*************************************************************************************************/
	ReadStreamableFFreeListTrashArray(m_combatAuras, pStream);
	ReadStreamableFFreeListTrashArray(m_AIGroups, pStream);	
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

	{
		CvMessageQueue::_Alloc::size_type iSize;
		pStream->Read(&iSize);
		for (CvMessageQueue::_Alloc::size_type i = 0; i < iSize; i++)
		{
			CvTalkingHeadMessage message;
			message.read(*pStream);
			m_listGameMessages.push_back(message);
		}
	}

	{
		clearPopups();
		CvPopupQueue::_Alloc::size_type iSize;
		pStream->Read(&iSize);
		for (CvPopupQueue::_Alloc::size_type i = 0; i < iSize; i++)
		{
			CvPopupInfo* pInfo = new CvPopupInfo();
			if (NULL != pInfo)
			{
				pInfo->read(*pStream);
				m_listPopups.push_back(pInfo);
			}
		}
	}

	{
		clearDiplomacy();
		CvDiploQueue::_Alloc::size_type iSize;
		pStream->Read(&iSize);
		for (CvDiploQueue::_Alloc::size_type i = 0; i < iSize; i++)
		{
			CvDiploParameters* pDiplo = new CvDiploParameters(NO_PLAYER);
			if (NULL != pDiplo)
			{
				pDiplo->read(*pStream);
				m_listDiplomacy.push_back(pDiplo);
			}
		}
	}

	{
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			int iTurn;
			int iScore;
			pStream->Read(&iTurn);
			pStream->Read(&iScore);
			m_mapScoreHistory[iTurn] = iScore;
		}
	}

	{
		m_mapEconomyHistory.clear();
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			int iTurn;
			int iScore;
			pStream->Read(&iTurn);
			pStream->Read(&iScore);
			m_mapEconomyHistory[iTurn] = iScore;
		}
	}

	{
		m_mapIndustryHistory.clear();
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			int iTurn;
			int iScore;
			pStream->Read(&iTurn);
			pStream->Read(&iScore);
			m_mapIndustryHistory[iTurn] = iScore;
		}
	}

	{
		m_mapAgricultureHistory.clear();
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			int iTurn;
			int iScore;
			pStream->Read(&iTurn);
			pStream->Read(&iScore);
			m_mapAgricultureHistory[iTurn] = iScore;
		}
	}

	{
		m_mapPowerHistory.clear();
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			int iTurn;
			int iScore;
			pStream->Read(&iTurn);
			pStream->Read(&iScore);
			m_mapPowerHistory[iTurn] = iScore;
		}
	}

	{
		m_mapCultureHistory.clear();
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			int iTurn;
			int iScore;
			pStream->Read(&iTurn);
			pStream->Read(&iScore);
			m_mapCultureHistory[iTurn] = iScore;
		}
	}

	{
		m_mapEspionageHistory.clear();
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			int iTurn;
			int iScore;
			pStream->Read(&iTurn);
			pStream->Read(&iScore);
			m_mapEspionageHistory[iTurn] = iScore;
		}
	}

	{
		m_mapEventsOccured.clear();
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			EventTriggeredData kData;
			EventTypes eEvent;
			pStream->Read((int*)&eEvent);
			kData.read(pStream);
			m_mapEventsOccured[eEvent] = kData;
		}
	}

	{
		m_mapEventCountdown.clear();
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			EventTriggeredData kData;
			EventTypes eEvent;
			pStream->Read((int*)&eEvent);
			kData.read(pStream);
			m_mapEventCountdown[eEvent] = kData;
		}
	}

	{
		m_aFreeUnitCombatPromotions.clear();
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			int iUnitCombat;
			int iPromotion;
			pStream->Read(&iUnitCombat);
			pStream->Read(&iPromotion);
			m_aFreeUnitCombatPromotions.push_back(std::make_pair((UnitCombatTypes)iUnitCombat, (PromotionTypes)iPromotion));
		}
	}

	{
		m_aFreeUnitClassPromotions.clear();
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			int iUnitClass;
			int iPromotion;
			pStream->Read(&iUnitClass);
			pStream->Read(&iPromotion);
			m_aFreeUnitClassPromotions.push_back(std::make_pair((UnitClassTypes)iUnitClass, (PromotionTypes)iPromotion));
		}
	}

	{
		m_aVote.clear();
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			int iId;
			PlayerVoteTypes eVote;
			pStream->Read(&iId);
			pStream->Read((int*)&eVote);
			m_aVote.push_back(std::make_pair(iId, eVote));
		}
	}

	{
		m_aUnitExtraCosts.clear();
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			int iCost;
			UnitClassTypes eUnit;
			pStream->Read((int*)&eUnit);
			pStream->Read(&iCost);
			m_aUnitExtraCosts.push_back(std::make_pair(eUnit, iCost));
		}
	}

	if (uiFlag > 0)
	{
		m_triggersFired.clear();
		uint iSize;
		pStream->Read(&iSize);
		for (uint i = 0; i < iSize; i++)
		{
			int iTrigger;
			pStream->Read(&iTrigger);
			m_triggersFired.push_back((EventTriggerTypes)iTrigger);
		}
	}
	else
	{

//FfH: Modified by Kael 09/18/2008
//		int iNumEventTriggers = std::min(176, GC.getNumEventTriggerInfos()); // yuck, hardcoded number of eventTriggers in the epic game in initial release
//		for (iI=0; iI < iNumEventTriggers; iI++)
		for (iI=0; iI < GC.getNumEventTriggerInfos(); iI++)
//FfH: End Modify

		{
			bool bTriggered;
			pStream->Read(&bTriggered);
			if (bTriggered)
			{
				m_triggersFired.push_back((EventTriggerTypes)iI);
			}
		}
	}

	if (!isBarbarian())
	{
		// Get the NetID from the initialization structure
		setNetID(gDLL->getAssignedNetworkID(getID()));
	}

	pStream->Read(&m_iPopRushHurryCount);
	pStream->Read(&m_iInflationModifier);
/*************************************************************************************************/
/**	New Gameoption (Passive XP Training) Sephi                                				    **/
/*************************************************************************************************/
	pStream->Read(GC.getNumUnitCombatInfos(), m_paiTrainXPCap);
	pStream->Read(GC.getNumUnitCombatInfos(), m_paiTrainXPRate);
/**	ADDON (Houses of Erebus) Sephi			                                 					**/
	pStream->Read(GC.getNumCorporationInfos(), m_pabCorporationSupport);
	pStream->Read(GC.getNumCorporationInfos(), m_paiCorporationSupport);
	pStream->Read(GC.getNumCorporationInfos(), m_paiCorporationSupportMod);
/**	ADDON (Adventures) Sephi                                                      				**/
	pStream->Read(GC.getNumAdventureInfos(), m_pabAdventureEnabled);
	pStream->Read(GC.getNumAdventureInfos(), m_pabAdventureFinished);

	for (iI=0;iI<GC.getNumAdventureInfos();iI++)
	{
		pStream->Read(GC.getMAX_ADVENTURESTEPS(), m_ppaaiAdventureCounter[iI]);
	}
/**	ADDON (New Mana) Sephi                                                      				**/
	pStream->Read(GC.getNumDamageTypeInfos(), m_paiSpellDamageModify);	
	pStream->Read(GC.getNumTechInfos(), m_pabCanEverSpellResearch);	
	pStream->Read(GC.getNumManaschoolInfos(), m_paiManaSchoolCostModifier);
	pStream->Read(GC.getNUM_YIELD_TYPES(), m_paiGlobalYield);
	pStream->Read(GC.getNUM_YIELD_TYPES(), m_paiGlobalYieldUpkeep);
	pStream->Read(GC.getNUM_YIELD_TYPES(), m_paiGlobalYieldNeededForEquipmentBonus);
	pStream->Read(GC.getNumBonusInfos(), m_paiNumBonusesConsumed);
	pStream->Read(GC.getNumBonusInfos(), m_paiNumBonusesConsumedByProduction);
	pStream->Read(GC.getNumReligionInfos(), m_paiFoundHolyCityCount);
	pStream->Read(GC.getNumImprovementInfos(), m_paiManaFromImprovement);
	pStream->Read(GC.getNumImprovementInfos(), m_paiFaithFromImprovement);
	pStream->Read(GC.getNumBuildingClassInfos(), m_paiIgnoreBuildingGYCostCount);
/*************************************************************************************************/
/**	    									END													**/
/*************************************************************************************************/

}

//
// save object to a stream
// used during save
//
void CvPlayer::write(FDataStreamBase* pStream)
{
	int iI;

	uint uiFlag = 1;
	pStream->Write(uiFlag);		// flag for expansion

	pStream->Write(m_iStartingX);
	pStream->Write(m_iStartingY);
	pStream->Write(m_iTotalPopulation);
	pStream->Write(m_iTotalLand);
	pStream->Write(m_iTotalLandScored);
	pStream->Write(m_iGold);
	pStream->Write(m_iCrime);
	pStream->Write(m_iCaptureCityGoldPercentChange);
	pStream->Write(m_iGoldPerTurn);
	pStream->Write(m_iAdvancedStartPoints);
	pStream->Write(m_iGoldenAgeTurns);
	pStream->Write(m_iNumUnitGoldenAges);
	pStream->Write(m_iStrikeTurns);
	pStream->Write(m_iAnarchyTurns);
	pStream->Write(m_iMaxAnarchyTurns);
	pStream->Write(m_iAnarchyModifier);
	pStream->Write(m_iGoldenAgeModifier);
	pStream->Write(m_iGlobalHurryModifier);
	pStream->Write(m_iGreatPeopleCreated);
	pStream->Write(m_iGreatGeneralsCreated);
	pStream->Write(m_iGreatPeopleThresholdModifier);
	pStream->Write(m_iGreatGeneralsThresholdModifier);
	pStream->Write(m_iGreatPeopleRateModifier);
	pStream->Write(m_iGreatGeneralRateModifier);
	pStream->Write(m_iDomesticGreatGeneralRateModifier);
	pStream->Write(m_iStateReligionGreatPeopleRateModifier);
	pStream->Write(m_iMaxGlobalBuildingProductionModifier);
	pStream->Write(m_iMaxTeamBuildingProductionModifier);
	pStream->Write(m_iMaxPlayerBuildingProductionModifier);
	pStream->Write(m_iFreeExperience);
	pStream->Write(m_iFeatureProductionModifier);
	pStream->Write(m_iWorkerSpeedModifier);
	pStream->Write(m_iImprovementUpgradeRateModifier);
	pStream->Write(m_iMilitaryProductionModifier);
	pStream->Write(m_iSpaceProductionModifier);
	pStream->Write(m_iCityDefenseModifier);
	pStream->Write(m_iNumNukeUnits);
	pStream->Write(m_iNumOutsideUnits);
	pStream->Write(m_iBaseFreeUnits);
	pStream->Write(m_iBaseFreeMilitaryUnits);
	pStream->Write(m_iFreeUnitsPopulationPercent);
	pStream->Write(m_iFreeMilitaryUnitsPopulationPercent);
	pStream->Write(m_iGoldPerUnit);
	pStream->Write(m_iGoldPerMilitaryUnit);
	pStream->Write(m_iExtraUnitCost);
	pStream->Write(m_iNumMilitaryUnits);
	pStream->Write(m_iNumSummonUnits);
	pStream->Write(m_iNumGarrisionUnits);
	pStream->Write(m_iHappyPerMilitaryUnit);
	pStream->Write(m_iMilitaryFoodProductionCount);
	pStream->Write(m_iConscriptCount);
	pStream->Write(m_iMaxConscript);
	pStream->Write(m_iHighestUnitLevel);
	pStream->Write(m_iOverflowResearch);
	pStream->Write(m_iNoUnhealthyPopulationCount);
	pStream->Write(m_iExpInBorderModifier);
	pStream->Write(m_iBuildingOnlyHealthyCount);
	pStream->Write(m_iDistanceMaintenanceModifier);
	pStream->Write(m_iNumCitiesMaintenanceModifier);
	pStream->Write(m_iCorporationMaintenanceModifier);
	pStream->Write(m_iTotalMaintenance);
	pStream->Write(m_iUpkeepModifier);
	pStream->Write(m_iLevelExperienceModifier);
	pStream->Write(m_iExtraHealth);
	pStream->Write(m_iBuildingGoodHealth);
	pStream->Write(m_iBuildingBadHealth);
	pStream->Write(m_iExtraHappiness);
	pStream->Write(m_iBuildingHappiness);
	pStream->Write(m_iLargestCityHappiness);
	pStream->Write(m_iWarWearinessPercentAnger);
	pStream->Write(m_iWarWearinessModifier);
	pStream->Write(m_iFreeSpecialist);
	pStream->Write(m_iNoForeignTradeCount);
	pStream->Write(m_iNoCorporationsCount);
	pStream->Write(m_iNoForeignCorporationsCount);
	pStream->Write(m_iCoastalTradeRoutes);
	pStream->Write(m_iTradeRoutes);
	pStream->Write(m_iRevolutionTimer);
	pStream->Write(m_iConversionTimer);
	pStream->Write(m_iStateReligionCount);
	pStream->Write(m_iNoNonStateReligionSpreadCount);
	pStream->Write(m_iStateReligionHappiness);
	pStream->Write(m_iNonStateReligionHappiness);
	pStream->Write(m_iStateReligionUnitProductionModifier);
	pStream->Write(m_iStateReligionBuildingProductionModifier);
	pStream->Write(m_iStateReligionFreeExperience);
	pStream->Write(m_iCapitalCityID);
	pStream->Write(m_iCitiesLost);
	pStream->Write(m_iWinsVsBarbs);
	pStream->Write(m_iAssets);
	pStream->Write(m_iPower);
	pStream->Write(m_iPopulationScore);
	pStream->Write(m_iLandScore);
	pStream->Write(m_iWondersScore);
	pStream->Write(m_iTechScore);
	pStream->Write(m_iCombatExperience);

	pStream->Write(m_bAlive);
	pStream->Write(m_bEverAlive);
	pStream->Write(m_bTurnActive);
	pStream->Write(m_bAutoMoves);
	pStream->Write(m_bEndTurn);
	pStream->Write(m_bPbemNewTurn && GC.getGameINLINE().isPbem());
	pStream->Write(m_bExtendedGame);
	pStream->Write(m_bFoundedFirstCity);
	pStream->Write(m_bStrike);

/*************************************************************************************************/
/**	BETTER AI (New Functions Definition) Sephi                                 					**/
/**																								**/
/**						                                            							**/
/*************************************************************************************************/
    pStream->Write(m_bConquestMode);
/**	New Esus                                                                 					**/
    pStream->Write(m_iPrestige);
    pStream->Write(m_eFavoriteReligion);
    pStream->Write(m_iTowerVicFlag);
	pStream->Write(m_bRangedAttackMode);
    pStream->Write(m_iArcane);
    pStream->Write(m_iArcaneLevel);
    pStream->Write(m_iFaith);
    pStream->Write(m_iFaithIncome);
    pStream->Write(m_iReligionsSpread);
    pStream->Write(m_iFaithMod);
    pStream->Write(m_iFaithUpkeep);
    pStream->Write(m_iManaBonus2);
    pStream->Write(m_iManaBonus3);
    pStream->Write(m_iManaBonus4);
    pStream->Write(m_iBarbarianPower);
    pStream->Write(m_iBarbarianPoints);
    pStream->Write(m_iBarbarianValue2);
    pStream->Write(m_iBarbarianValue3);
    pStream->Write(m_iMana);
    pStream->Write(m_iManaIncome);
    pStream->Write(m_iManaUpkeep);
    pStream->Write(m_iPopulationLimit);
    pStream->Write(m_iCityStateType);
    pStream->Write(m_iCityStateCounter);
    pStream->Write(m_iPurityCounter);
    pStream->Write(m_iPurityCounterCache1);
	pStream->Write(m_iUnitSupportLimit);
	pStream->Write(m_iUnitSupportUsed);
    pStream->Write(m_iReducedEquipmentCost);
	pStream->Write(m_iImprovementDiscountPercent);
    pStream->Write(m_iGreatPersonRatePerCulture);
    pStream->Write(m_iHealthRatePerCulture);
    pStream->Write(m_iWillpowerChange);
    pStream->Write(m_iReducedTerraformCost);
    pStream->Write(m_iReducedEnchantmentCost);
    pStream->Write(m_iReducedSummonCost);
    pStream->Write(m_iIncreaseTerraformTiles);
    pStream->Write(m_iIncreaseHostileTerraformTiles);
    pStream->Write(m_iIncreaseSummonXP);
    pStream->Write(m_iCurrentMagicRitual);
    pStream->Write(m_iCurrentMagicRitualManaNeeded);
	pStream->Write(m_iMaxMana);
	pStream->Write(m_iManaFromSpellResearch);
	pStream->Write(m_iManaFromNodes);	
	pStream->Write(m_iIncreaseStrengthOfRegularUnits);		
	pStream->Write(m_iSpellResearchFocus);
    pStream->Write(m_iAI_lostCitiesRevolutionMemory);
    pStream->Write(m_iManaCreationModifier);
    pStream->Write(m_iResistHostileTerraforming);
    pStream->Write(m_iTerraformPlan);	
    pStream->Write(m_iIgnoreUnHealthFromImprovements);	
    pStream->Write(m_iGlobalCulture);	
    pStream->Write(m_iTotalCityInfastructureLevel);	
    pStream->Write(m_iTotalTraitsFromCulture);	
    pStream->Write(m_iExtraImprovementPerCity);	
    pStream->Write(m_iExtraManaNodesPercent);	
    pStream->Write(m_iReducedHurryCostBuilding);	
    pStream->Write(m_iExtraSupportUnitLimit);	
    pStream->Write(m_iExtraBuildingCapturedChance);	
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
//FfH Traits: Added by Kael 08/02/2007
	pStream->Write(m_bAdaptive);
	pStream->Write(m_bAgnostic);
	pStream->Write(m_bAssimilation);
	pStream->Write(m_bDeclaringWar);
	pStream->Write(m_bDisableHuman);
	pStream->Write(m_bHideUnits);
	pStream->Write(m_bIgnoreFood);
	pStream->Write(m_bInsane);
	pStream->Write(m_bSeeInvisible);
	pStream->Write(m_bSprawling);
	pStream->Write(m_iAlignment);
	pStream->Write(m_iDisableProduction);
	pStream->Write(m_iDisableResearch);
	pStream->Write(m_iDisableSpellcasting);
	pStream->Write(m_iDiscoverRandModifier);
	pStream->Write(m_iEnslavementChance);
	pStream->Write(m_iFreeXPFromCombat);
	pStream->Write(m_iHealChange);
	pStream->Write(m_iHealChangeEnemy);
	pStream->Write(m_iMaxCities);
	pStream->Write(m_iNoDiplomacyWithEnemies);
	pStream->Write(m_iPillagingGold);
	pStream->Write(m_iPlayersKilled);
	pStream->Write(m_iRealPlayer);
	pStream->Write(m_iResistEnemyModify);
	pStream->Write(m_iResistModify);
	pStream->Write(m_iSanctuaryTimer);
	pStream->Write(m_iStartingGold);
	pStream->Write(m_iSummonCostReduction);
	pStream->Write(m_iTaxesModifier);
	pStream->Write(m_iFoodPerPopulationModifier);
	pStream->Write(m_iTempPlayerTimer);
	pStream->Write(m_iUpgradeCostModifier);
	pStream->Write(m_iProductionCostReductionCapitalBuilding);
	pStream->Write(m_iUnitSupportModifier);

	pStream->Write(GC.getNumTraitInfos(), m_pbTraits);
	for (iI=0;iI<GC.getNumSpecialistInfos();iI++)
	{
		pStream->Write(NUM_COMMERCE_TYPES, m_ppaaiSpecialistTypeExtraCommerce[iI]);
	}
//FfH: End Add

	pStream->Write(m_eID);
	pStream->Write(m_ePersonalityType);
	pStream->Write(m_eCurrentEra);
	pStream->Write(m_eLastStateReligion);
	pStream->Write(m_eParent);
	//m_eTeamType not saved

	pStream->Write(NUM_YIELD_TYPES, m_aiSeaPlotYield);
	pStream->Write(NUM_YIELD_TYPES, m_aiYieldRateModifier);
	pStream->Write(NUM_YIELD_TYPES, m_aiCapitalYieldRateModifier);
	pStream->Write(NUM_YIELD_TYPES, m_aiExtraYieldThreshold);
	pStream->Write(NUM_YIELD_TYPES, m_aiTradeYieldModifier);
	pStream->Write(NUM_COMMERCE_TYPES, m_aiFreeCityCommerce);
	pStream->Write(NUM_COMMERCE_TYPES, m_aiCommercePercent);
	pStream->Write(NUM_COMMERCE_TYPES, m_aiCommerceRate);
	pStream->Write(NUM_COMMERCE_TYPES, m_aiCommerceRateModifier);
	pStream->Write(NUM_COMMERCE_TYPES, m_aiCapitalCommerceRateModifier);
	pStream->Write(NUM_COMMERCE_TYPES, m_aiCommerceChangesPerCultureLevel);
	pStream->Write(NUM_COMMERCE_TYPES, m_aiStateReligionBuildingCommerce);
	pStream->Write(NUM_COMMERCE_TYPES, m_aiSpecialistExtraCommerce);
	pStream->Write(NUM_COMMERCE_TYPES, m_aiCommerceFlexibleCount);
	pStream->Write(MAX_PLAYERS, m_aiGoldPerTurnByPlayer);
	pStream->Write(MAX_TEAMS, m_aiEspionageSpendingWeightAgainstTeam);

	pStream->Write(NUM_FEAT_TYPES, m_abFeatAccomplished);
	pStream->Write(NUM_PLAYEROPTION_TYPES, m_abOptions);

	pStream->WriteString(m_szScriptData);

	FAssertMsg((0 < GC.getNumBonusInfos()), "GC.getNumBonusInfos() is not greater than zero but an array is being allocated in CvPlayer::write");
	pStream->Write(GC.getNumBonusInfos(), m_paiBonusExport);
	pStream->Write(GC.getNumBonusInfos(), m_paiBonusImport);
	pStream->Write(GC.getNumImprovementInfos(), m_paiImprovementCount);
	pStream->Write(GC.getNumBuildingInfos(), m_paiFreeBuildingCount);
	pStream->Write(GC.getNumBuildingInfos(), m_paiExtraBuildingHappiness);
	pStream->Write(GC.getNumBuildingInfos(), m_paiExtraBuildingHealth);
	pStream->Write(GC.getNumFeatureInfos(), m_paiFeatureHappiness);
	pStream->Write(GC.getNumUnitClassInfos(), m_paiUnitClassCount);
	pStream->Write(GC.getNumUnitClassInfos(), m_paiUnitClassMaking);
	pStream->Write(GC.getNumBuildingClassInfos(), m_paiBuildingClassCount);
	pStream->Write(GC.getNumBuildingClassInfos(), m_paiBuildingClassMaking);
	pStream->Write(GC.getNumHurryInfos(), m_paiHurryCount);
	pStream->Write(GC.getNumSpecialBuildingInfos(), m_paiSpecialBuildingNotRequiredCount);
	pStream->Write(GC.getNumCivicOptionInfos(), m_paiHasCivicOptionCount);
	pStream->Write(GC.getNumCivicOptionInfos(), m_paiNoCivicUpkeepCount);
	pStream->Write(GC.getNumReligionInfos(), m_paiHasReligionCount);
	pStream->Write(GC.getNumCorporationInfos(), m_paiHasCorporationCount);
	pStream->Write(GC.getNumUpkeepInfos(), m_paiUpkeepCount);
	pStream->Write(GC.getNumSpecialistInfos(), m_paiSpecialistValidCount);

	FAssertMsg((0 < GC.getNumTechInfos()), "GC.getNumTechInfos() is not greater than zero but it is expected to be in CvPlayer::write");
	pStream->Write(GC.getNumTechInfos(), m_pabResearchingTech);

	pStream->Write(GC.getNumVoteSourceInfos(), m_pabLoyalMember);

	for (iI=0;iI<GC.getNumCivicOptionInfos();iI++)
	{
		pStream->Write(m_paeCivics[iI]);
	}

	for (iI=0;iI<GC.getNumSpecialistInfos();iI++)
	{
		pStream->Write(NUM_YIELD_TYPES, m_ppaaiSpecialistExtraYield[iI]);
	}

	for (iI=0;iI<GC.getNumImprovementInfos();iI++)
	{
		pStream->Write(NUM_YIELD_TYPES, m_ppaaiImprovementYieldChange[iI]);
	}

	m_groupCycle.Write(pStream);
	m_researchQueue.Write(pStream);

	{
		CLLNode<CvWString>* pNode;
		uint iSize = m_cityNames.getLength();
		pStream->Write(iSize);
		pNode = m_cityNames.head();
		while (pNode != NULL)
		{
			pStream->WriteString(pNode->m_data);
			pNode = m_cityNames.next(pNode);
		}
	}

	WriteStreamableFFreeListTrashArray(m_plotGroups, pStream);
	WriteStreamableFFreeListTrashArray(m_cities, pStream);
	WriteStreamableFFreeListTrashArray(m_units, pStream);
	WriteStreamableFFreeListTrashArray(m_selectionGroups, pStream);
	WriteStreamableFFreeListTrashArray(m_eventsTriggered, pStream);
/*************************************************************************************************/
/**	ADDON (Combatauras) Sephi																	**/
/*************************************************************************************************/
	WriteStreamableFFreeListTrashArray(m_combatAuras, pStream);
	WriteStreamableFFreeListTrashArray(m_AIGroups, pStream);	
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

	{
		CvMessageQueue::_Alloc::size_type iSize = m_listGameMessages.size();
		pStream->Write(iSize);
		CvMessageQueue::iterator it;
		for (it = m_listGameMessages.begin(); it != m_listGameMessages.end(); ++it)
		{
			CvTalkingHeadMessage& message = *it;
			message.write(*pStream);
		}
	}

	{
		CvPopupQueue currentPopups;
		if (GC.getGameINLINE().isNetworkMultiPlayer())
		{
			// don't save open popups in MP to avoid having different state on different machines
			currentPopups.clear();
		}
		else
		{
			gDLL->getInterfaceIFace()->getDisplayedButtonPopups(currentPopups);
		}
		CvPopupQueue::_Alloc::size_type iSize = m_listPopups.size() + currentPopups.size();
		pStream->Write(iSize);
		CvPopupQueue::iterator it;
		for (it = currentPopups.begin(); it != currentPopups.end(); ++it)
		{
			CvPopupInfo* pInfo = *it;
			if (NULL != pInfo)
			{
				pInfo->write(*pStream);
			}
		}
		for (it = m_listPopups.begin(); it != m_listPopups.end(); ++it)
		{
			CvPopupInfo* pInfo = *it;
			if (NULL != pInfo)
			{
				pInfo->write(*pStream);
			}
		}
	}

	{
		CvDiploQueue::_Alloc::size_type iSize = m_listDiplomacy.size();
		pStream->Write(iSize);
		CvDiploQueue::iterator it;
		for (it = m_listDiplomacy.begin(); it != m_listDiplomacy.end(); ++it)
		{
			CvDiploParameters* pDiplo = *it;
			if (NULL != pDiplo)
			{
				pDiplo->write(*pStream);
			}
		}
	}

	{
		uint iSize = m_mapScoreHistory.size();
		pStream->Write(iSize);
		CvTurnScoreMap::iterator it;
		for (it = m_mapScoreHistory.begin(); it != m_mapScoreHistory.end(); ++it)
		{
			pStream->Write(it->first);
			pStream->Write(it->second);
		}
	}

	{
		uint iSize = m_mapEconomyHistory.size();
		pStream->Write(iSize);
		CvTurnScoreMap::iterator it;
		for (it = m_mapEconomyHistory.begin(); it != m_mapEconomyHistory.end(); ++it)
		{
			pStream->Write(it->first);
			pStream->Write(it->second);
		}
	}

	{
		uint iSize = m_mapIndustryHistory.size();
		pStream->Write(iSize);
		CvTurnScoreMap::iterator it;
		for (it = m_mapIndustryHistory.begin(); it != m_mapIndustryHistory.end(); ++it)
		{
			pStream->Write(it->first);
			pStream->Write(it->second);
		}
	}

	{
		uint iSize = m_mapAgricultureHistory.size();
		pStream->Write(iSize);
		CvTurnScoreMap::iterator it;
		for (it = m_mapAgricultureHistory.begin(); it != m_mapAgricultureHistory.end(); ++it)
		{
			pStream->Write(it->first);
			pStream->Write(it->second);
		}
	}

	{
		uint iSize = m_mapPowerHistory.size();
		pStream->Write(iSize);
		CvTurnScoreMap::iterator it;
		for (it = m_mapPowerHistory.begin(); it != m_mapPowerHistory.end(); ++it)
		{
			pStream->Write(it->first);
			pStream->Write(it->second);
		}
	}

	{
		uint iSize = m_mapCultureHistory.size();
		pStream->Write(iSize);
		CvTurnScoreMap::iterator it;
		for (it = m_mapCultureHistory.begin(); it != m_mapCultureHistory.end(); ++it)
		{
			pStream->Write(it->first);
			pStream->Write(it->second);
		}
	}

	{
		uint iSize = m_mapEspionageHistory.size();
		pStream->Write(iSize);
		CvTurnScoreMap::iterator it;
		for (it = m_mapEspionageHistory.begin(); it != m_mapEspionageHistory.end(); ++it)
		{
			pStream->Write(it->first);
			pStream->Write(it->second);
		}
	}

	{
		uint iSize = m_mapEventsOccured.size();
		pStream->Write(iSize);
		CvEventMap::iterator it;
		for (it = m_mapEventsOccured.begin(); it != m_mapEventsOccured.end(); ++it)
		{
			pStream->Write(it->first);
			it->second.write(pStream);
		}
	}

	{
		uint iSize = m_mapEventCountdown.size();
		pStream->Write(iSize);
		CvEventMap::iterator it;
		for (it = m_mapEventCountdown.begin(); it != m_mapEventCountdown.end(); ++it)
		{
			pStream->Write(it->first);
			it->second.write(pStream);
		}
	}

	{
		uint iSize = m_aFreeUnitCombatPromotions.size();
		pStream->Write(iSize);
		UnitCombatPromotionArray::iterator it;
		for (it = m_aFreeUnitCombatPromotions.begin(); it != m_aFreeUnitCombatPromotions.end(); ++it)
		{
			pStream->Write((*it).first);
			pStream->Write((*it).second);
		}
	}

	{
		uint iSize = m_aFreeUnitClassPromotions.size();
		pStream->Write(iSize);
		UnitClassPromotionArray::iterator it;
		for (it = m_aFreeUnitClassPromotions.begin(); it != m_aFreeUnitClassPromotions.end(); ++it)
		{
			pStream->Write((*it).first);
			pStream->Write((*it).second);
		}
	}

	{
		uint iSize = m_aVote.size();
		pStream->Write(iSize);
		std::vector< std::pair<int, PlayerVoteTypes> >::iterator it;
		for (it = m_aVote.begin(); it != m_aVote.end(); ++it)
		{
			pStream->Write((*it).first);
			pStream->Write((*it).second);
		}
	}

	{
		uint iSize = m_aUnitExtraCosts.size();
		pStream->Write(iSize);
		std::vector< std::pair<UnitClassTypes, int> >::iterator it;
		for (it = m_aUnitExtraCosts.begin(); it != m_aUnitExtraCosts.end(); ++it)
		{
			pStream->Write((*it).first);
			pStream->Write((*it).second);
		}
	}

	{
		uint iSize = m_triggersFired.size();
		pStream->Write(iSize);
		std::vector<EventTriggerTypes>::iterator it;
		for (it = m_triggersFired.begin(); it != m_triggersFired.end(); ++it)
		{
			pStream->Write((*it));
		}
	}

	pStream->Write(m_iPopRushHurryCount);
	pStream->Write(m_iInflationModifier);
/*************************************************************************************************/
/**	New Gameoption (Passive XP Training) Sephi                                				    **/
/*************************************************************************************************/
	pStream->Write(GC.getNumUnitCombatInfos(), m_paiTrainXPCap);
	pStream->Write(GC.getNumUnitCombatInfos(), m_paiTrainXPRate);
/**	ADDON (Houses of Erebus) Sephi			                                 					**/
	pStream->Write(GC.getNumCorporationInfos(), m_pabCorporationSupport);
	pStream->Write(GC.getNumCorporationInfos(), m_paiCorporationSupport);
	pStream->Write(GC.getNumCorporationInfos(), m_paiCorporationSupportMod);
/**	ADDON (Adventures) Sephi                                                      				**/
	pStream->Write(GC.getNumAdventureInfos(), m_pabAdventureEnabled);
	pStream->Write(GC.getNumAdventureInfos(), m_pabAdventureFinished);

	for (iI=0;iI<GC.getNumAdventureInfos();iI++)
	{
		pStream->Write(GC.getMAX_ADVENTURESTEPS(), m_ppaaiAdventureCounter[iI]);
	}
/**	ADDON (New Mana) Sephi                                                      				**/
	pStream->Write(GC.getNumDamageTypeInfos(), m_paiSpellDamageModify);	
	pStream->Write(GC.getNumTechInfos(), m_pabCanEverSpellResearch);	
	pStream->Write(GC.getNumManaschoolInfos(), m_paiManaSchoolCostModifier);
	pStream->Write(GC.getNUM_YIELD_TYPES(), m_paiGlobalYield);
	pStream->Write(GC.getNUM_YIELD_TYPES(), m_paiGlobalYieldUpkeep);
	pStream->Write(GC.getNUM_YIELD_TYPES(), m_paiGlobalYieldNeededForEquipmentBonus);
	pStream->Write(GC.getNumBonusInfos(), m_paiNumBonusesConsumed);
	pStream->Write(GC.getNumBonusInfos(), m_paiNumBonusesConsumedByProduction);
	pStream->Write(GC.getNumReligionInfos(), m_paiFoundHolyCityCount);
	pStream->Write(GC.getNumImprovementInfos(), m_paiManaFromImprovement);
	pStream->Write(GC.getNumImprovementInfos(), m_paiFaithFromImprovement);
	pStream->Write(GC.getNumBuildingClassInfos(), m_paiIgnoreBuildingGYCostCount);
/*************************************************************************************************/
/**	    									END													**/
/*************************************************************************************************/
}

void CvPlayer::createGreatPeople(UnitTypes eGreatPersonUnit, bool bIncrementThreshold, bool bIncrementExperience, int iX, int iY)
{
	CvUnit* pGreatPeopleUnit = initUnit(eGreatPersonUnit, iX, iY);
	if (NULL == pGreatPeopleUnit)
	{
		FAssert(false);
		return;
	}

	if (bIncrementThreshold)
	{
		incrementGreatPeopleCreated();

//		changeGreatPeopleThresholdModifier(GC.getDefineINT("GREAT_PEOPLE_THRESHOLD_INCREASE") * ((getGreatPeopleCreated() / 10) + 1));
		changeGreatPeopleThresholdModifier(GC.getDefineINT("GREAT_PEOPLE_THRESHOLD_INCREASE"));

		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getTeam())
			{
				GET_PLAYER((PlayerTypes)iI).changeGreatPeopleThresholdModifier(GC.getDefineINT("GREAT_PEOPLE_THRESHOLD_INCREASE_TEAM") * ((getGreatPeopleCreated() / 10) + 1));
			}
		}
	}

	if (bIncrementExperience)
	{
		incrementGreatGeneralsCreated();

		changeGreatGeneralsThresholdModifier(GC.getDefineINT("GREAT_GENERALS_THRESHOLD_INCREASE") * ((getGreatGeneralsCreated() / 10) + 1));

		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == getTeam())
			{
				GET_PLAYER((PlayerTypes)iI).changeGreatGeneralsThresholdModifier(GC.getDefineINT("GREAT_GENERALS_THRESHOLD_INCREASE_TEAM") * ((getGreatGeneralsCreated() / 10) + 1));
			}
		}
	}


	CvPlot* pPlot = GC.getMapINLINE().plot(iX, iY);
	CvCity* pCity = pPlot->getPlotCity();
	CvWString szReplayMessage;

	if (pPlot)
	{
		if (pCity)
		{
			CvWString szCity;
			szCity.Format(L"%s (%s)", pCity->getName().GetCString(), GET_PLAYER(pCity->getOwnerINLINE()).getName());
			szReplayMessage = gDLL->getText("TXT_KEY_MISC_GP_BORN", pGreatPeopleUnit->getName().GetCString(), szCity.GetCString());
		}
		else
		{
			szReplayMessage = gDLL->getText("TXT_KEY_MISC_GP_BORN_FIELD", pGreatPeopleUnit->getName().GetCString());
		}
		GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getID(), szReplayMessage, iX, iY, (ColorTypes)GC.getInfoTypeForString("COLOR_UNIT_TEXT"));
	}

	bool bShowMessage = true;

	if(GC.getUnitInfo(eGreatPersonUnit).getProductionCost() != -1) {

		bShowMessage = false;
	}

	if(bShowMessage) {

		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				if (pPlot->isRevealed(GET_PLAYER((PlayerTypes)iI).getTeam(), false))
				{
					gDLL->getInterfaceIFace()->addMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szReplayMessage, "AS2D_UNIT_GREATPEOPLE", MESSAGE_TYPE_MAJOR_EVENT, pGreatPeopleUnit->getButton(), (ColorTypes)GC.getInfoTypeForString("COLOR_UNIT_TEXT"), iX, iY, true, true);
				}
				else
				{
					CvWString szMessage = gDLL->getText("TXT_KEY_MISC_GP_BORN_SOMEWHERE", pGreatPeopleUnit->getName().GetCString());
					gDLL->getInterfaceIFace()->addMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szMessage, "AS2D_UNIT_GREATPEOPLE", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_UNIT_TEXT"));
				}
			}
		}
	}

	// Python Event
	if (pCity)
	{
		CvEventReporter::getInstance().greatPersonBorn(pGreatPeopleUnit, getID(), pCity);
	}

}


const EventTriggeredData* CvPlayer::getEventOccured(EventTypes eEvent) const
{
	FAssert(eEvent >= 0 && eEvent < GC.getNumEventInfos());

	CvEventMap::const_iterator it = m_mapEventsOccured.find(eEvent);

	if (it == m_mapEventsOccured.end())
	{
		return NULL;
	}

	return &(it->second);
}

bool CvPlayer::isTriggerFired(EventTriggerTypes eEventTrigger) const
{
	return (std::find(m_triggersFired.begin(), m_triggersFired.end(), eEventTrigger) != m_triggersFired.end());
}

void CvPlayer::resetEventOccured(EventTypes eEvent, bool bAnnounce)
{
	FAssert(eEvent >= 0 && eEvent < GC.getNumEventInfos());

	CvEventMap::iterator it = m_mapEventsOccured.find(eEvent);

	if (it != m_mapEventsOccured.end())
	{
		expireEvent(it->first, it->second, bAnnounce);
		m_mapEventsOccured.erase(it);
	}
}

void CvPlayer::setEventOccured(EventTypes eEvent, const EventTriggeredData& kEventTriggered, bool bOthers)
{
	FAssert(eEvent >= 0 && eEvent < GC.getNumEventInfos());

	m_mapEventsOccured[eEvent] = kEventTriggered;

	if (GC.getEventInfo(eEvent).isQuest())
	{
		CvWStringBuffer szMessageBuffer;
		szMessageBuffer.append(GC.getEventInfo(eEvent).getDescription());
		GAMETEXT.setEventHelp(szMessageBuffer, eEvent, kEventTriggered.getID(), getID());
		gDLL->getInterfaceIFace()->addQuestMessage(getID(), szMessageBuffer.getCString(), kEventTriggered.getID());
	}

	if (bOthers)
	{
		if (GC.getEventInfo(eEvent).isGlobal())
		{
			for (int i = 0; i < MAX_CIV_PLAYERS; i++)
			{
				if (i != getID())
				{
					GET_PLAYER((PlayerTypes)i).setEventOccured(eEvent, kEventTriggered, false);
				}
			}
		}
		else if (GC.getEventInfo(eEvent).isTeam())
		{
			for (int i = 0; i < MAX_CIV_PLAYERS; i++)
			{
				if (i != getID() && getTeam() == GET_PLAYER((PlayerTypes)i).getTeam())
				{
					GET_PLAYER((PlayerTypes)i).setEventOccured(eEvent, kEventTriggered, false);
				}
			}
		}
	}
}


const EventTriggeredData* CvPlayer::getEventCountdown(EventTypes eEvent) const
{
	FAssert(eEvent >= 0 && eEvent < GC.getNumEventInfos());

	CvEventMap::const_iterator it = m_mapEventCountdown.find(eEvent);

	if (it == m_mapEventCountdown.end())
	{
		return NULL;
	}

	return &(it->second);
}

void CvPlayer::setEventCountdown(EventTypes eEvent, const EventTriggeredData& kEventTriggered)
{
	FAssert(eEvent >= 0 && eEvent < GC.getNumEventInfos());

	m_mapEventCountdown[eEvent] = kEventTriggered;
}

void CvPlayer::resetEventCountdown(EventTypes eEvent)
{
	FAssert(eEvent >= 0 && eEvent < GC.getNumEventInfos());

	CvEventMap::iterator it = m_mapEventCountdown.find(eEvent);

	if (it != m_mapEventCountdown.end())
	{
		m_mapEventCountdown.erase(it);
	}
}


void CvPlayer::resetTriggerFired(EventTriggerTypes eTrigger)
{
	std::vector<EventTriggerTypes>::iterator it = std::find(m_triggersFired.begin(), m_triggersFired.end(), eTrigger);

	if (it != m_triggersFired.end())
	{
		m_triggersFired.erase(it);
	}
}

void CvPlayer::setTriggerFired(const EventTriggeredData& kTriggeredData, bool bOthers, bool bAnnounce)
{
	FAssert(kTriggeredData.m_eTrigger >= 0 && kTriggeredData.m_eTrigger < GC.getNumEventTriggerInfos());

	CvEventTriggerInfo& kTrigger = GC.getEventTriggerInfo(kTriggeredData.m_eTrigger);

	if (!isTriggerFired(kTriggeredData.m_eTrigger))
	{
		m_triggersFired.push_back(kTriggeredData.m_eTrigger);

		if (bOthers)
		{
			if (kTrigger.isGlobal())
			{
				for (int i = 0; i < MAX_CIV_PLAYERS; i++)
				{
					if (i != getID())
					{

//						GET_PLAYER((PlayerTypes)i).setTriggerFired(kTriggeredData, false, false);
                        if (GET_PLAYER((PlayerTypes)i).isAlive())
                        {
                            GET_PLAYER((PlayerTypes)i).setTriggerFired(kTriggeredData, false, false);
                        }

					}
				}
			}
			else if (kTrigger.isTeam())
			{
				for (int i = 0; i < MAX_CIV_PLAYERS; i++)
				{
					if (i != getID() && getTeam() == GET_PLAYER((PlayerTypes)i).getTeam())
					{
						GET_PLAYER((PlayerTypes)i).setTriggerFired(kTriggeredData, false, false);
					}
				}
			}
		}
	}

//FfH: Modified by Kael 09/25/2008
//	if (!isEmpty(kTrigger.getPythonCallback()))
//	{
//		long lResult;
//		CyArgsList argsList;
//		argsList.add(gDLL->getPythonIFace()->makePythonObject(&kTriggeredData));
//		gDLL->getPythonIFace()->callFunction(PYRandomEventModule, kTrigger.getPythonCallback(), argsList.makeFunctionArgs(), &lResult);
//	}
    if (isAlive())
    {
		if (!CvString(kTrigger.getPythonCallback()).empty())
        {
            long lResult;

            CyArgsList argsList;
            argsList.add(gDLL->getPythonIFace()->makePythonObject(&kTriggeredData));
            argsList.add(getID());	// Player ID
            gDLL->getPythonIFace()->callFunction(PYRandomEventModule, kTrigger.getPythonCallback(), argsList.makeFunctionArgs(), &lResult);
        }
    }
    GC.getGameINLINE().setEventTriggered((EventTriggerTypes)kTriggeredData.m_eTrigger, true);
//FfH: End Modify

	if (bAnnounce)
	{
		CvPlot* pPlot = GC.getMapINLINE().plot(kTriggeredData.m_iPlotX, kTriggeredData.m_iPlotY);

		if (!kTriggeredData.m_szGlobalText.empty())
		{
			for (int iPlayer = 0; iPlayer < MAX_CIV_PLAYERS; ++iPlayer)
			{
				CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);

				if (kLoopPlayer.isAlive())
				{
					if (GET_TEAM(kLoopPlayer.getTeam()).isHasMet(getTeam()) && (NO_PLAYER == kTriggeredData.m_eOtherPlayer || GET_TEAM(GET_PLAYER(kTriggeredData.m_eOtherPlayer).getTeam()).isHasMet(getTeam())))
					{
						bool bShowPlot = kTrigger.isShowPlot();

						if (bShowPlot)
						{
							if (kLoopPlayer.getTeam() != getTeam())
							{
								if (NULL == pPlot || !pPlot->isRevealed(kLoopPlayer.getTeam(), false))
								{
									bShowPlot = false;
								}
							}
						}

						if (bShowPlot)
						{
							gDLL->getInterfaceIFace()->addMessage((PlayerTypes)iPlayer, false, GC.getEVENT_MESSAGE_TIME(), kTriggeredData.m_szGlobalText, "AS2D_CIVIC_ADOPT", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), kTriggeredData.m_iPlotX, kTriggeredData.m_iPlotY, true, true);
						}
						else
						{
							gDLL->getInterfaceIFace()->addMessage((PlayerTypes)iPlayer, false, GC.getEVENT_MESSAGE_TIME(), kTriggeredData.m_szGlobalText, "AS2D_CIVIC_ADOPT", MESSAGE_TYPE_MINOR_EVENT);
						}
					}
				}
			}

			GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getID(), kTriggeredData.m_szGlobalText, kTriggeredData.m_iPlotX, kTriggeredData.m_iPlotY, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
		}
		else if (!kTriggeredData.m_szText.empty())
		{
			if (kTrigger.isShowPlot() && NULL != pPlot && pPlot->isRevealed(getTeam(), false))
			{
				gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), kTriggeredData.m_szText, "AS2D_CIVIC_ADOPT", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), kTriggeredData.m_iPlotX, kTriggeredData.m_iPlotY, true, true);
			}
			else
			{
				gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), kTriggeredData.m_szText, "AS2D_CIVIC_ADOPT", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"));
			}
		}
	}
}

EventTriggeredData* CvPlayer::initTriggeredData(EventTriggerTypes eEventTrigger, bool bFire, int iCityId, int iPlotX, int iPlotY, PlayerTypes eOtherPlayer, int iOtherPlayerCityId, ReligionTypes eReligion, CorporationTypes eCorporation, int iUnitId, BuildingTypes eBuilding)
{
	CvEventTriggerInfo& kTrigger = GC.getEventTriggerInfo(eEventTrigger);

	CvCity* pCity = getCity(iCityId);
	CvCity* pOtherPlayerCity = NULL;
	if (NO_PLAYER != eOtherPlayer)
	{
		pOtherPlayerCity = GET_PLAYER(eOtherPlayer).getCity(iOtherPlayerCityId);
	}
	CvPlot* pPlot = GC.getMapINLINE().plot(iPlotX, iPlotY);
	CvUnit* pUnit = getUnit(iUnitId);

	std::vector<CvPlot*> apPlots;
	bool bPickPlot = ::isPlotEventTrigger(eEventTrigger);

	if (kTrigger.isPickCity())
	{
		if (NULL == pCity)
		{
			pCity = pickTriggerCity(eEventTrigger);
		}

		if (NULL != pCity)
		{
			if (bPickPlot)
			{
				for (int iPlot = 0; iPlot < NUM_CITY_PLOTS; ++iPlot)
				{
					if (CITY_HOME_PLOT != iPlot)
					{
						CvPlot* pLoopPlot = pCity->getCityIndexPlot(iPlot);

						if (NULL != pLoopPlot)
						{
							if (pLoopPlot->canTrigger(eEventTrigger, getID()))
							{
								apPlots.push_back(pLoopPlot);
							}
						}
					}
				}
			}
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		if (kTrigger.getNumBuildings() > 0 && kTrigger.getNumBuildingsRequired() > 0)
		{
			int iFoundValid = 0;

			for (int i = 0; i < kTrigger.getNumBuildingsRequired(); ++i)
			{
				if (kTrigger.getBuildingRequired(i) != NO_BUILDINGCLASS)
				{
					iFoundValid += getBuildingClassCount((BuildingClassTypes)kTrigger.getBuildingRequired(i));
				}
			}

			if (iFoundValid < kTrigger.getNumBuildings())
			{
				return NULL;
			}
		}

		if (kTrigger.getNumReligions() > 0)
		{
			int iFoundValid = 0;

			if (kTrigger.getNumReligionsRequired() > 0)
			{
				for (int i = 0; i < kTrigger.getNumReligionsRequired(); ++i)
				{
					if (kTrigger.getReligionRequired(i) != NO_RELIGION)
					{
						if (getHasReligionCount((ReligionTypes)kTrigger.getReligionRequired(i)) > 0)
						{
							++iFoundValid;
						}
					}
				}
			}
			else
			{
				for (int i = 0; i < GC.getNumReligionInfos(); ++i)
				{
					if (getHasReligionCount((ReligionTypes)i) > 0)
					{
						++iFoundValid;
					}
				}
			}

			if (iFoundValid < kTrigger.getNumReligions())
			{
				return NULL;
			}
		}

		if (kTrigger.getNumCorporations() > 0)
		{
			int iFoundValid = 0;

			if (kTrigger.getNumCorporationsRequired() > 0)
			{
				for (int i = 0; i < kTrigger.getNumCorporationsRequired(); ++i)
				{
					if (kTrigger.getCorporationRequired(i) != NO_CORPORATION)
					{
						if (getHasCorporationCount((CorporationTypes)kTrigger.getCorporationRequired(i)) > 0)
						{
							++iFoundValid;
						}
					}
				}
			}
			else
			{
				for (int i = 0; i < GC.getNumCorporationInfos(); ++i)
				{
					if (getHasCorporationCount((CorporationTypes)i) > 0)
					{
						++iFoundValid;
					}
				}
			}

			if (iFoundValid < kTrigger.getNumCorporations())
			{
				return NULL;
			}
		}

		if (kTrigger.getMinPopulation() > 0)
		{
			if (getTotalPopulation() < kTrigger.getMinPopulation())
			{
				return NULL;
			}
		}

		if (kTrigger.getMaxPopulation() > 0)
		{
			if (getTotalPopulation() > kTrigger.getMaxPopulation())
			{
				return NULL;
			}
		}

		if (bPickPlot)
		{
			for (int iPlot = 0; iPlot < GC.getMapINLINE().numPlotsINLINE(); ++iPlot)
			{
				CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iPlot);

				if (pLoopPlot->canTrigger(eEventTrigger, getID()))
				{
					apPlots.push_back(pLoopPlot);
				}
			}
		}
	}

	if (kTrigger.isPickReligion())
	{
		if (NO_RELIGION == eReligion)
		{
			if (kTrigger.isStateReligion())
			{
				ReligionTypes eStateReligion = getStateReligion();
				if (NO_RELIGION != eStateReligion && isValidTriggerReligion(kTrigger, pCity, eStateReligion))
				{
					eReligion = getStateReligion();
				}
			}
			else
			{
				int iOffset = GC.getGameINLINE().getSorenRandNum(GC.getNumReligionInfos(), "Event pick religion");

				for (int i = 0; i < GC.getNumReligionInfos(); ++i)
				{
					int iReligion = (i + iOffset) % GC.getNumReligionInfos();

					if (isValidTriggerReligion(kTrigger, pCity, (ReligionTypes)iReligion))
					{
						eReligion = (ReligionTypes)iReligion;
						break;
					}
				}
			}
		}

		if (NO_RELIGION == eReligion)
		{
			return NULL;
		}
	}

	if (kTrigger.isPickCorporation())
	{
		if (NO_CORPORATION == eCorporation)
		{
			int iOffset = GC.getGameINLINE().getSorenRandNum(GC.getNumCorporationInfos(), "Event pick corporation");

			for (int i = 0; i < GC.getNumCorporationInfos(); ++i)
			{
				int iCorporation = (i + iOffset) % GC.getNumCorporationInfos();

				if (isValidTriggerCorporation(kTrigger, pCity, (CorporationTypes)iCorporation))
				{
					eCorporation = (CorporationTypes)iCorporation;
					break;
				}
			}
		}

		if (NO_CORPORATION == eCorporation)
		{
			return NULL;
		}
	}

	if (NULL == pPlot)
	{
		if (apPlots.size() > 0)
		{
			int iChosen = GC.getGameINLINE().getSorenRandNum(apPlots.size(), "Event pick plot");
			pPlot = apPlots[iChosen];

			if (NULL == pCity)
			{
				pCity = GC.getMapINLINE().findCity(pPlot->getX_INLINE(), pPlot->getY_INLINE(), getID(), NO_TEAM, false);
			}
		}
		else
		{
			if (bPickPlot)
			{
				return NULL;
			}

			if (NULL != pCity)
			{
				pPlot = pCity->plot();
			}
		}
	}

	if (kTrigger.getNumBuildings() > 0)
	{
		if (NULL != pCity && NO_BUILDING == eBuilding)
		{
			std::vector<BuildingTypes> aeBuildings;
			for (int i = 0; i < kTrigger.getNumBuildingsRequired(); ++i)
			{
				if (kTrigger.getBuildingRequired(i) != NO_BUILDINGCLASS)
				{
					BuildingTypes eTestBuilding = (BuildingTypes)GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(kTrigger.getBuildingRequired(i));
					if (NO_BUILDING != eTestBuilding && pCity->getNumRealBuilding(eTestBuilding) > 0)
					{
						aeBuildings.push_back(eTestBuilding);
					}
				}
			}

			if (aeBuildings.size() > 0)
			{
				int iChosen = GC.getGameINLINE().getSorenRandNum(aeBuildings.size(), "Event pick building");
				eBuilding = aeBuildings[iChosen];
			}
			else
			{
				return NULL;
			}
		}
	}

	if (NULL == pUnit)
	{
		pUnit = pickTriggerUnit(eEventTrigger, pPlot, bPickPlot);
	}

	if (NULL == pUnit && kTrigger.getNumUnits() > 0)
	{
		return NULL;
	}

	if (NULL == pPlot && NULL != pUnit)
	{
		pPlot = pUnit->plot();
	}

	if (NULL == pPlot && bPickPlot)
	{
		return NULL;
	}

	if (kTrigger.getNumUnitsGlobal() > 0)
	{
		int iNumUnits = 0;
		for (int iPlayer = 0; iPlayer < MAX_CIV_PLAYERS; ++iPlayer)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);

			if (kLoopPlayer.isAlive())
			{
				int iLoop;
				for (CvUnit* pLoopUnit = kLoopPlayer.firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = kLoopPlayer.nextUnit(&iLoop))
				{
					if (MIN_INT != pLoopUnit->getTriggerValue(eEventTrigger, pPlot, true))
					{
						++iNumUnits;
					}
				}
			}
		}

		if (iNumUnits < kTrigger.getNumUnitsGlobal())
		{
			return NULL;
		}
	}

	if (kTrigger.getNumBuildingsGlobal() > 0)
	{
		int iNumBuildings = 0;
		for (int iPlayer = 0; iPlayer < MAX_CIV_PLAYERS; ++iPlayer)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);

			if (kLoopPlayer.isAlive())
			{
				for (int i = 0; i < kTrigger.getNumBuildingsRequired(); ++i)
				{
					if (kTrigger.getBuildingRequired(i) != NO_BUILDINGCLASS)
					{
						iNumBuildings += getBuildingClassCount((BuildingClassTypes)kTrigger.getBuildingRequired(i));
					}
				}
			}
		}

		if (iNumBuildings < kTrigger.getNumBuildingsGlobal())
		{
			return NULL;
		}
	}

	if (kTrigger.isPickPlayer())
	{
		std::vector<PlayerTypes> aePlayers;
		std::vector<CvCity*> apCities;

		if (NO_PLAYER == eOtherPlayer)
		{
			for (int i = 0; i < MAX_CIV_PLAYERS; i++)
			{
				if (GET_PLAYER((PlayerTypes)i).canTrigger(eEventTrigger, getID(), eReligion))
				{
					if (kTrigger.isPickOtherPlayerCity())
					{
						CvCity* pBestCity = NULL;

						if (NULL != pCity)
						{
							pBestCity = GC.getMapINLINE().findCity(pCity->getX_INLINE(), pCity->getY_INLINE(), (PlayerTypes)i);
						}
						else
						{
							pBestCity = GET_PLAYER((PlayerTypes)i).pickTriggerCity(eEventTrigger);
						}

						if (NULL != pBestCity)
						{
							apCities.push_back(pBestCity);
							aePlayers.push_back((PlayerTypes)i);
						}
					}
					else
					{
						apCities.push_back(NULL);
						aePlayers.push_back((PlayerTypes)i);
					}
				}
			}

			if (aePlayers.size() > 0)
			{
				int iChosen = GC.getGameINLINE().getSorenRandNum(aePlayers.size(), "Event pick player");
				eOtherPlayer = aePlayers[iChosen];
				pOtherPlayerCity = apCities[iChosen];
			}
			else
			{
				return NULL;
			}
		}
	}

	EventTriggeredData* pTriggerData = addEventTriggered();

	if (NULL != pTriggerData)
	{
		pTriggerData->m_eTrigger = eEventTrigger;
		pTriggerData->m_ePlayer = getID();
		pTriggerData->m_iTurn = GC.getGameINLINE().getGameTurn();
		pTriggerData->m_iCityId = (NULL != pCity) ? pCity->getID() : -1;
		pTriggerData->m_iPlotX = (NULL != pPlot) ? pPlot->getX_INLINE() : INVALID_PLOT_COORD;
		pTriggerData->m_iPlotY = (NULL != pPlot) ? pPlot->getY_INLINE() : INVALID_PLOT_COORD;
		pTriggerData->m_eOtherPlayer = eOtherPlayer;
		pTriggerData->m_iOtherPlayerCityId = (NULL != pOtherPlayerCity) ? pOtherPlayerCity->getID() : -1;
		pTriggerData->m_eReligion = eReligion;
		pTriggerData->m_eCorporation = eCorporation;
		pTriggerData->m_iUnitId = (NULL != pUnit) ? pUnit->getID() : -1;
		pTriggerData->m_eBuilding = eBuilding;
	}
	else
	{
		return NULL;
	}

	if (!CvString(kTrigger.getPythonCanDo()).empty())
	{
		long lResult;

		CyArgsList argsList;
		argsList.add(gDLL->getPythonIFace()->makePythonObject(pTriggerData));

		gDLL->getPythonIFace()->callFunction(PYRandomEventModule, kTrigger.getPythonCanDo(), argsList.makeFunctionArgs(), &lResult);

		if (0 == lResult)
		{
			deleteEventTriggered(pTriggerData->getID());
			return NULL;
		}

		// python may change pTriggerData
		pCity = getCity(pTriggerData->m_iCityId);
		pPlot = GC.getMapINLINE().plot(pTriggerData->m_iPlotX, pTriggerData->m_iPlotY);
		pUnit = getUnit(pTriggerData->m_iUnitId);
		eOtherPlayer = pTriggerData->m_eOtherPlayer;
		if (NO_PLAYER != eOtherPlayer)
		{
			pOtherPlayerCity = GET_PLAYER(eOtherPlayer).getCity(pTriggerData->m_iOtherPlayerCityId);
		}
		eReligion = pTriggerData->m_eReligion;
		eCorporation = pTriggerData->m_eCorporation;
		eBuilding = pTriggerData->m_eBuilding;
	}

	std::vector<CvWString> aszTexts;
	for (int i = 0; i < kTrigger.getNumTexts(); ++i)
	{
		if (NO_ERA == kTrigger.getTextEra(i) || kTrigger.getTextEra(i) == getCurrentEra())
		{
			aszTexts.push_back(kTrigger.getText(i));
		}
	}

	if (aszTexts.size() > 0)
	{
		int iText = GC.getGameINLINE().getSorenRandNum(aszTexts.size(), "Event Text choice");

		pTriggerData->m_szText = gDLL->getText(aszTexts[iText].GetCString(),
			eOtherPlayer != NO_PLAYER ? GET_PLAYER(eOtherPlayer).getCivilizationAdjectiveKey() : L"",
			NULL != pCity ? pCity->getNameKey() : L"",
			NULL != pUnit ? pUnit->getNameKey() : L"",
			NO_RELIGION != eReligion ? GC.getReligionInfo(eReligion).getAdjectiveKey() : L"",
			NO_BUILDING != eBuilding ? GC.getBuildingInfo(eBuilding).getTextKeyWide() : L"",
			NULL != pOtherPlayerCity ? pOtherPlayerCity->getNameKey() : L"",
			NULL != pPlot && NO_TERRAIN != pPlot->getTerrainType() ? GC.getTerrainInfo(pPlot->getTerrainType()).getTextKeyWide() : L"",
			NULL != pPlot && NO_IMPROVEMENT != pPlot->getImprovementType() ? GC.getImprovementInfo(pPlot->getImprovementType()).getTextKeyWide() : L"",
			NULL != pPlot && NO_BONUS != pPlot->getBonusType() ? GC.getBonusInfo(pPlot->getBonusType()).getTextKeyWide() : L"",
			NULL != pPlot && NO_ROUTE != pPlot->getRouteType() ? GC.getRouteInfo(pPlot->getRouteType()).getTextKeyWide() : L"",
			NO_CORPORATION != eCorporation ? GC.getCorporationInfo(eCorporation).getTextKeyWide() : L""
			);

	}
	else
	{
		pTriggerData->m_szText = L"";
	}

	if (kTrigger.getNumWorldNews() > 0)
	{
		int iText = GC.getGameINLINE().getSorenRandNum(kTrigger.getNumWorldNews(), "Trigger World News choice");

		pTriggerData->m_szGlobalText = gDLL->getText(kTrigger.getWorldNews(iText).GetCString(),
			getCivilizationAdjectiveKey(),
			NULL != pCity ? pCity->getNameKey() : L"",
			pTriggerData->m_eReligion != NO_RELIGION ? GC.getReligionInfo(pTriggerData->m_eReligion).getAdjectiveKey() : L"",
			eOtherPlayer != NO_PLAYER ? GET_PLAYER(eOtherPlayer).getCivilizationAdjectiveKey() : L"",
			NULL != pOtherPlayerCity ? pOtherPlayerCity->getNameKey() : L"",
			pTriggerData->m_eCorporation != NO_CORPORATION ? GC.getCorporationInfo(pTriggerData->m_eCorporation).getTextKeyWide() : L""
			);
	}
	else
	{
		pTriggerData->m_szGlobalText.clear();
	}

	if (bFire)
	{
		trigger(*pTriggerData);
	}

	return pTriggerData;
}


bool CvPlayer::canDoEvent(EventTypes eEvent, const EventTriggeredData& kTriggeredData) const
{
	if (eEvent == NO_EVENT)
	{
		FAssert(false);
		return false;
	}

	CvEventInfo& kEvent = GC.getEventInfo(eEvent);

	int iGold = std::min(getEventCost(eEvent, kTriggeredData.m_eOtherPlayer, false), getEventCost(eEvent, kTriggeredData.m_eOtherPlayer, true));

	if (iGold != 0)
	{
		if (iGold > 0 && NO_PLAYER != kTriggeredData.m_eOtherPlayer && kEvent.isGoldToPlayer())
		{
			if (GET_PLAYER(kTriggeredData.m_eOtherPlayer).getGold() < iGold)
			{
				return false;
			}
		}
		else if (iGold < 0)
		{
			if (getGold() < -iGold)
			{
				return false;
			}
		}
	}

	if (0 != kEvent.getSpaceProductionModifier())
	{
		bool bValid = false;
		for (int iProject = 0; iProject < GC.getNumProjectInfos(); ++iProject)
		{
			CvProjectInfo& kProject = GC.getProjectInfo((ProjectTypes)iProject);
			if (kProject.isSpaceship())
			{
				if (kProject.getVictoryPrereq() != NO_VICTORY)
				{
					if (GC.getGameINLINE().isVictoryValid((VictoryTypes)(kProject.getVictoryPrereq())))
					{
						bValid = true;
						break;
					}
				}
			}
		}

		if (!bValid)
		{
			return false;
		}
	}

	if (kEvent.getEspionagePoints() > 0 && GC.getGameINLINE().isOption(GAMEOPTION_NO_ESPIONAGE))
	{
		return false;
	}

	if (NO_PLAYER != kTriggeredData.m_eOtherPlayer)
	{
		if (kEvent.getEspionagePoints() + GET_TEAM(getTeam()).getEspionagePointsAgainstTeam(GET_PLAYER(kTriggeredData.m_eOtherPlayer).getTeam()) < 0)
		{
			return false;
		}
	}

	if (0 != kEvent.getTechPercent() || 0 != kEvent.getTechCostPercent())
	{
		if (NO_TECH == getBestEventTech(eEvent, kTriggeredData.m_eOtherPlayer))
		{
			return false;
		}
	}

	if (NO_TECH != kEvent.getPrereqTech())
	{
		if (!GET_TEAM(getTeam()).isHasTech((TechTypes)kEvent.getPrereqTech()))
		{
			return false;
		}
	}

	if (NO_BONUS != kEvent.getBonusGift())
	{
		BonusTypes eBonus = (BonusTypes)kEvent.getBonusGift();
		if (NO_PLAYER == kTriggeredData.m_eOtherPlayer)
		{
			return false;
		}

		if (!canTradeNetworkWith(kTriggeredData.m_eOtherPlayer))
		{
			return false;
		}

		if (GET_PLAYER(kTriggeredData.m_eOtherPlayer).getNumAvailableBonuses(eBonus) > 0)
		{
			return false;
		}

		if (getNumTradeableBonuses(eBonus) <= 1)
		{
			return false;
		}
	}

	if (kEvent.getUnitClass() != NO_UNITCLASS)
	{
		UnitTypes eUnit = (UnitTypes)GC.getCivilizationInfo(getCivilizationType()).getCivilizationUnits(kEvent.getUnitClass());
		if (eUnit == NO_UNIT)
		{
			return false;
		}
	}

	if (kEvent.isCityEffect())
	{
		CvCity* pCity =	getCity(kTriggeredData.m_iCityId);
		if (NULL == pCity || !pCity->canApplyEvent(eEvent, kTriggeredData))
		{
			return false;
		}
	}
	else if (kEvent.isOtherPlayerCityEffect())
	{
		if (NO_PLAYER == kTriggeredData.m_eOtherPlayer)
		{
			return false;
		}

		CvCity* pCity = GET_PLAYER(kTriggeredData.m_eOtherPlayer).getCity(kTriggeredData.m_iOtherPlayerCityId);
		if (NULL == pCity || !pCity->canApplyEvent(eEvent, kTriggeredData))
		{
			return false;
		}
	}

	if (::isPlotEventTrigger(kTriggeredData.m_eTrigger))
	{
		CvPlot* pPlot = GC.getMapINLINE().plotINLINE(kTriggeredData.m_iPlotX, kTriggeredData.m_iPlotY);
		if (NULL != pPlot)
		{
			if (!pPlot->canApplyEvent(eEvent))
			{
				return false;
			}
		}
	}

	CvUnit* pUnit = getUnit(kTriggeredData.m_iUnitId);
	if (NULL != pUnit)
	{
		if (!pUnit->canApplyEvent(eEvent))
		{
			return false;
		}
	}

	if (NO_BONUS != kEvent.getBonusRevealed())
	{
		if (GET_TEAM(getTeam()).isHasTech((TechTypes)GC.getBonusInfo((BonusTypes)kEvent.getBonusRevealed()).getTechReveal()))
		{
			return false;
		}

		if (GET_TEAM(getTeam()).isForceRevealedBonus((BonusTypes)kEvent.getBonusRevealed()))
		{
			return false;
		}
	}

	if (kEvent.getConvertOwnCities() > 0)
	{
		bool bFoundValid = false;

		if (NO_RELIGION != kTriggeredData.m_eReligion)
		{
			int iLoop;
			for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
			{
				if (!pLoopCity->isHasReligion(kTriggeredData.m_eReligion))
				{
					if (-1 == kEvent.getMaxNumReligions() || pLoopCity->getReligionCount() <= kEvent.getMaxNumReligions())
					{
						bFoundValid = true;
						break;
					}
				}
			}
		}

		if (!bFoundValid)
		{
			return false;
		}
	}

	if (kEvent.getConvertOtherCities() > 0)
	{
		bool bFoundValid = false;

		if (NO_RELIGION != kTriggeredData.m_eReligion)
		{
			if (NO_PLAYER != kTriggeredData.m_eOtherPlayer)
			{
				int iLoop;
				for (CvCity* pLoopCity = GET_PLAYER(kTriggeredData.m_eOtherPlayer).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER(kTriggeredData.m_eOtherPlayer).nextCity(&iLoop))
				{
					if (!pLoopCity->isHasReligion(kTriggeredData.m_eReligion))
					{
						if (-1 == kEvent.getMaxNumReligions() || pLoopCity->getReligionCount() <= kEvent.getMaxNumReligions())
						{
							bFoundValid = true;
							break;
						}
					}
				}
			}
		}

		if (!bFoundValid)
		{
			return false;
		}
	}

	if (0 != kEvent.getAttitudeModifier())
	{
		if (NO_PLAYER == kTriggeredData.m_eOtherPlayer)
		{
			return false;
		}

		if (GET_PLAYER(kTriggeredData.m_eOtherPlayer).getTeam() == getTeam())
		{
			return false;
		}

		if (GET_PLAYER(kTriggeredData.m_eOtherPlayer).isHuman())
		{
			if (0 == kEvent.getOurAttitudeModifier())
			{
				return false;
			}
		}
	}

	if (0 != kEvent.getTheirEnemyAttitudeModifier())
	{
		if (NO_PLAYER == kTriggeredData.m_eOtherPlayer)
		{
			return false;
		}

		TeamTypes eWorstEnemy = GET_TEAM(GET_PLAYER(kTriggeredData.m_eOtherPlayer).getTeam()).AI_getWorstEnemy();
		if (NO_TEAM == eWorstEnemy || eWorstEnemy == getTeam())
		{
			return false;
		}

		if (!GET_TEAM(eWorstEnemy).isAlive())
		{
			return false;
		}

		if (eWorstEnemy == getTeam())
		{
			return false;
		}
	}

	if (kEvent.isDeclareWar())
	{
		if (NO_PLAYER == kTriggeredData.m_eOtherPlayer)
		{
			return false;
		}

		if (!GET_TEAM(GET_PLAYER(kTriggeredData.m_eOtherPlayer).getTeam()).canDeclareWar(getTeam()) || !GET_TEAM(getTeam()).canDeclareWar(GET_PLAYER(kTriggeredData.m_eOtherPlayer).getTeam()))
		{
			return false;
		}
	}

	if (kEvent.isQuest())
	{
		for (int iTrigger = 0; iTrigger < GC.getNumEventTriggerInfos(); ++iTrigger)
		{
			CvEventTriggerInfo& kTrigger = GC.getEventTriggerInfo((EventTriggerTypes)iTrigger);
			if (!kTrigger.isRecurring())
			{
				for (int i = 0; i < kTrigger.getNumPrereqEvents(); ++i)
				{
					if (kTrigger.getPrereqEvent(i) == eEvent)
					{
						if (isTriggerFired((EventTriggerTypes)iTrigger))
						{
							return false;
						}
					}
				}
			}
		}
	}

//FfH: Added by Kael 01/21/2008
	if (kEvent.getPrereqAlignment() != NO_ALIGNMENT)
	{
        if (getAlignment() != kEvent.getPrereqAlignment())
        {
            return false;
		}
	}
	if (kEvent.getPrereqBonus() != NO_BONUS)
	{
        if (!hasBonus((BonusTypes)kEvent.getPrereqBonus()))
        {
            return false;
		}
	}
	if (kEvent.getPrereqCivilization() != NO_CIVILIZATION)
	{
        if (getCivilizationType() != kEvent.getPrereqCivilization())
        {
            return false;
		}
	}
	if (kEvent.getPrereqCorporation() != NO_CORPORATION)
	{
		CvCity* pCity =	getCity(kTriggeredData.m_iCityId);
		if (pCity != NULL)
		{
            if (!pCity->isHasCorporation((CorporationTypes)kEvent.getPrereqCorporation()))
           {
                return false;
            }
		}
	}
	if (kEvent.getPrereqReligion() != NO_RELIGION)
	{
		CvCity* pCity =	getCity(kTriggeredData.m_iCityId);
		if (pCity != NULL)
		{
            if (!pCity->isHasReligion((ReligionTypes)kEvent.getPrereqReligion()))
            {
                return false;
            }
		}
	}
	if (kEvent.getPrereqStateReligion() != NO_RELIGION)
	{
        if (getStateReligion() != kEvent.getPrereqStateReligion())
        {
            return false;
		}
	}
//FfH: End Add

	if (!CvString(kEvent.getPythonCanDo()).empty())
	{
		long lResult;

		CyArgsList argsList;
		argsList.add(eEvent);
		argsList.add(gDLL->getPythonIFace()->makePythonObject(&kTriggeredData));

		gDLL->getPythonIFace()->callFunction(PYRandomEventModule, kEvent.getPythonCanDo(), argsList.makeFunctionArgs(), &lResult);

		if (0 == lResult)
		{
			return false;
		}
	}

	return true;
}


void CvPlayer::applyEvent(EventTypes eEvent, int iEventTriggeredId, bool bUpdateTrigger)
{
	FAssert(eEvent != NO_EVENT);

	EventTriggeredData* pTriggeredData = getEventTriggered(iEventTriggeredId);

	if (NULL == pTriggeredData)
	{
		deleteEventTriggered(iEventTriggeredId);
		return;
	}

	if (bUpdateTrigger)
	{
		setTriggerFired(*pTriggeredData, true);
	}

	if (!canDoEvent(eEvent, *pTriggeredData))
	{
		if (bUpdateTrigger)
		{
			deleteEventTriggered(iEventTriggeredId);
		}
		return;
	}

	setEventOccured(eEvent, *pTriggeredData);

	CvEventInfo& kEvent = GC.getEventInfo(eEvent);
	CvCity* pCity =	getCity(pTriggeredData->m_iCityId);
	CvCity* pOtherPlayerCity = NULL;

	if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
	{
		pOtherPlayerCity = GET_PLAYER(pTriggeredData->m_eOtherPlayer).getCity(pTriggeredData->m_iOtherPlayerCityId);
	}

	int iGold = getEventCost(eEvent, pTriggeredData->m_eOtherPlayer, false);
	int iRandomGold = getEventCost(eEvent, pTriggeredData->m_eOtherPlayer, true);

	iGold += GC.getGameINLINE().getSorenRandNum(iRandomGold - iGold + 1, "Event random gold");

	if (iGold != 0)
	{
		changeGold(iGold);

		if (NO_PLAYER != pTriggeredData->m_eOtherPlayer && kEvent.isGoldToPlayer())
		{
			GET_PLAYER(pTriggeredData->m_eOtherPlayer).changeGold(-iGold);
		}
	}

	if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
	{
		if (kEvent.getEspionagePoints() != 0)
		{
			GET_TEAM(getTeam()).changeEspionagePointsAgainstTeam(GET_PLAYER(pTriggeredData->m_eOtherPlayer).getTeam(), kEvent.getEspionagePoints());
		}
	}

	if (0 != kEvent.getTechPercent())
	{
		TechTypes eBestTech = getBestEventTech(eEvent, pTriggeredData->m_eOtherPlayer);

		if (eBestTech != NO_TECH)
		{
			int iBeakers  = GET_TEAM(getTeam()).changeResearchProgressPercent(eBestTech, kEvent.getTechPercent(), getID());

			if (iBeakers > 0)
			{
				for (int iI = 0; iI < MAX_CIV_PLAYERS; iI++)
				{
					if (GET_PLAYER((PlayerTypes)iI).isAlive())
					{
						if (GET_PLAYER((PlayerTypes)iI).getTeam() == getID())
						{
							CvWString szBuffer = gDLL->getText("TXT_KEY_MISC_PROGRESS_TOWARDS_TECH", iBeakers, GC.getTechInfo(eBestTech).getTextKeyWide());

							gDLL->getInterfaceIFace()->addMessage(((PlayerTypes)iI), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, NULL, MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_TECH_TEXT"));
						}
					}
				}
			}
		}
	}

	if (kEvent.isGoldenAge())
	{
		changeGoldenAgeTurns(getGoldenAgeLength());
	}

	if (0 != kEvent.getInflationModifier())
	{
		m_iInflationModifier += kEvent.getInflationModifier();
	}

	if (0 != kEvent.getSpaceProductionModifier())
	{
		changeSpaceProductionModifier(kEvent.getSpaceProductionModifier());
	}

	if (0 != kEvent.getFreeUnitSupport())
	{
		changeBaseFreeUnits(kEvent.getFreeUnitSupport());
	}

	if (kEvent.isDeclareWar())
	{
		if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
		{
			GET_TEAM(GET_PLAYER(pTriggeredData->m_eOtherPlayer).getTeam()).declareWar(getTeam(), false, WARPLAN_LIMITED);
		}
	}

	if (NO_BONUS != kEvent.getBonusGift())
	{
		if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
		{
			CLinkList<TradeData> ourList;
			CLinkList<TradeData> theirList;
			TradeData kTradeData;
			setTradeItem(&kTradeData, TRADE_RESOURCES, kEvent.getBonusGift());
			ourList.insertAtEnd(kTradeData);
			GC.getGameINLINE().implementDeal(getID(), pTriggeredData->m_eOtherPlayer, &ourList, &theirList);
		}
	}

	bool bClear = false;
	for (int iEvent = 0; iEvent < GC.getNumEventInfos(); ++iEvent)
	{
		if (kEvent.getClearEventChance(iEvent) > 0)
		{
			bClear = GC.getGameINLINE().getSorenRandNum(100, "Event Clear") < kEvent.getClearEventChance(iEvent);
			if (bClear)
			{
				if (kEvent.isGlobal())
				{
					for (int j = 0; j < MAX_CIV_PLAYERS; j++)
					{
						GET_PLAYER((PlayerTypes)j).resetEventOccured((EventTypes)iEvent, j != getID());
					}
				}
				else if (kEvent.isTeam())
				{
					for (int j = 0; j < MAX_CIV_PLAYERS; j++)
					{
						if (getTeam() == GET_PLAYER((PlayerTypes)j).getTeam())
						{
							GET_PLAYER((PlayerTypes)j).resetEventOccured((EventTypes)iEvent, j != getID());
						}
					}
				}
				else
				{
					resetEventOccured((EventTypes)iEvent, false);
				}
			}
		}
	}

	if (NULL != pCity && kEvent.isCityEffect())
	{
		pCity->applyEvent(eEvent, *pTriggeredData, bClear);
	}
	else if (NULL != pOtherPlayerCity && kEvent.isOtherPlayerCityEffect())
	{
		pOtherPlayerCity->applyEvent(eEvent, *pTriggeredData, bClear);
	}

	if (!kEvent.isCityEffect() && !kEvent.isOtherPlayerCityEffect())
	{
		if (kEvent.getHappy() != 0)
		{
			changeExtraHappiness(kEvent.getHappy());
		}

		if (kEvent.getHealth() != 0)
		{
			changeExtraHealth(kEvent.getHealth());
		}

		if (kEvent.getNumBuildingYieldChanges() > 0)
		{
			int iLoop;
			for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); ++iBuildingClass)
			{
				for (int iYield = 0; iYield < NUM_YIELD_TYPES; ++iYield)
				{
					for (CvCity* pLoopCity = firstCity(&iLoop); NULL != pLoopCity; pLoopCity = nextCity(&iLoop))
					{
						pLoopCity->changeBuildingYieldChange((BuildingClassTypes)iBuildingClass, (YieldTypes)iYield, kEvent.getBuildingYieldChange(iBuildingClass, iYield));
					}
				}
			}
		}

		if (kEvent.getNumBuildingCommerceChanges() > 0)
		{
			int iLoop;
			for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); ++iBuildingClass)
			{
				for (int iCommerce = 0; iCommerce < NUM_COMMERCE_TYPES; ++iCommerce)
				{
					for (CvCity* pLoopCity = firstCity(&iLoop); NULL != pLoopCity; pLoopCity = nextCity(&iLoop))
					{
						pLoopCity->changeBuildingCommerceChange((BuildingClassTypes)iBuildingClass, (CommerceTypes)iCommerce, kEvent.getBuildingCommerceChange(iBuildingClass, iCommerce));
					}
				}
			}
		}

		if (kEvent.getNumBuildingHappyChanges() > 0)
		{
			for (int i = 0; i < GC.getNumBuildingClassInfos(); ++i)
			{
				if (0 != kEvent.getBuildingHappyChange(i))
				{
					BuildingTypes eBuilding = (BuildingTypes)GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(i);
					if (NO_BUILDING != eBuilding)
					{
						changeExtraBuildingHappiness(eBuilding, kEvent.getBuildingHappyChange(i));
					}
				}
			}
		}

		if (kEvent.getNumBuildingHealthChanges() > 0)
		{
			for (int i = 0; i < GC.getNumBuildingClassInfos(); ++i)
			{
				if (0 != kEvent.getBuildingHealthChange(i))
				{
					BuildingTypes eBuilding = (BuildingTypes)GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(i);
					if (NO_BUILDING != eBuilding)
					{
						changeExtraBuildingHealth(eBuilding, kEvent.getBuildingHealthChange(i));
					}
				}
			}
		}

		if (kEvent.getHurryAnger() != 0)
		{
			int iLoop;

			for (CvCity* pLoopCity = firstCity(&iLoop); NULL != pLoopCity; pLoopCity = nextCity(&iLoop))
			{
				pLoopCity->changeHurryAngerTimer(kEvent.getHurryAnger() * pLoopCity->flatHurryAngerLength());
			}
		}

		if (kEvent.getHappyTurns() > 0)
		{
			int iLoop;

			for (CvCity* pLoopCity = firstCity(&iLoop); NULL != pLoopCity; pLoopCity = nextCity(&iLoop))
			{
				pLoopCity->changeHappinessTimer(kEvent.getHappyTurns());
			}
		}

		if (kEvent.getMaxPillage() > 0)
		{
			FAssert(kEvent.getMaxPillage() >= kEvent.getMinPillage());
			int iNumPillage = kEvent.getMinPillage() + GC.getGameINLINE().getSorenRandNum(kEvent.getMaxPillage() - kEvent.getMinPillage(), "Pick number of event pillaged plots");

			int iNumPillaged = 0;
			for (int i = 0; i < iNumPillage; ++i)
			{
				int iRandOffset = GC.getGameINLINE().getSorenRandNum(GC.getMapINLINE().numPlotsINLINE(), "Pick event pillage plot (any city)");
				for (int j = 0; j < GC.getMapINLINE().numPlotsINLINE(); ++j)
				{
					int iPlot = (j + iRandOffset) % GC.getMapINLINE().numPlotsINLINE();
					CvPlot* pPlot = GC.getMapINLINE().plotByIndexINLINE(iPlot);
					if (NULL != pPlot && pPlot->getOwnerINLINE() == getID() && pPlot->isCity())
					{
						if (NO_IMPROVEMENT != pPlot->getImprovementType() && !GC.getImprovementInfo(pPlot->getImprovementType()).isPermanent())
						{
							CvWString szBuffer = gDLL->getText("TXT_KEY_EVENT_CITY_IMPROVEMENT_DESTROYED", GC.getImprovementInfo(pPlot->getImprovementType()).getTextKeyWide());
							gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_PILLAGED", MESSAGE_TYPE_INFO, GC.getImprovementInfo(pPlot->getImprovementType()).getButton(), (ColorTypes)GC.getInfoTypeForString("COLOR_RED"), pPlot->getX_INLINE(), pPlot->getY_INLINE(), true, true);
							pPlot->setImprovementType(NO_IMPROVEMENT);
							++iNumPillaged;
							break;
						}
					}
				}
			}

			if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
			{
				CvWString szBuffer = gDLL->getText("TXT_KEY_EVENT_NUM_CITY_IMPROVEMENTS_DESTROYED", iNumPillaged, getCivilizationAdjectiveKey());
				gDLL->getInterfaceIFace()->addMessage(pTriggeredData->m_eOtherPlayer, false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_PILLAGED", MESSAGE_TYPE_INFO);
			}
		}

		if (kEvent.getFood() != 0)
		{
			int iLoop;

			for (CvCity* pLoopCity = firstCity(&iLoop); NULL != pLoopCity; pLoopCity = nextCity(&iLoop))
			{
				pLoopCity->changeFood(kEvent.getFood());
			}
		}

		if (kEvent.getFoodPercent() != 0)
		{
			int iLoop;

			for (CvCity* pLoopCity = firstCity(&iLoop); NULL != pLoopCity; pLoopCity = nextCity(&iLoop))
			{
				pLoopCity->changeFood((pLoopCity->getFood() * kEvent.getFoodPercent()) / 100);
			}
		}

		if (kEvent.getPopulationChange() != 0)
		{
			int iLoop;

			for (CvCity* pLoopCity = firstCity(&iLoop); NULL != pLoopCity; pLoopCity = nextCity(&iLoop))
			{
				if (pLoopCity->getPopulation() + kEvent.getPopulationChange() > 0)
				{
					pLoopCity->changePopulation(kEvent.getPopulationChange());
				}
			}
		}

		if (kEvent.getCulture() != 0)
		{
			int iLoop;

			for (CvCity* pLoopCity = firstCity(&iLoop); NULL != pLoopCity; pLoopCity = nextCity(&iLoop))
			{
				if (pLoopCity->getCultureTimes100(pLoopCity->getOwnerINLINE()) + 100 * kEvent.getCulture() > 0)
				{
					pLoopCity->changeCulture(pLoopCity->getOwnerINLINE(), kEvent.getCulture(), true, true);
				}
			}
		}

		if (kEvent.getUnitClass() != NO_UNITCLASS)
		{
			UnitTypes eUnit = (UnitTypes)GC.getCivilizationInfo(getCivilizationType()).getCivilizationUnits(kEvent.getUnitClass());
			if (eUnit != NO_UNIT)
			{
				CvCity* pUnitCity = pCity;

				if (NULL == pUnitCity)
				{
					pUnitCity = getCapitalCity();
				}

				if (NULL != pUnitCity)
				{
					for (int i = 0; i < kEvent.getNumUnits(); ++i)
					{

//FfH: Modified by Kael 10/29/2007
//						initUnit(eUnit, pUnitCity->getX_INLINE(), pUnitCity->getY_INLINE());
                        CvUnit* pUnit = initUnit(eUnit, pUnitCity->getX_INLINE(), pUnitCity->getY_INLINE());
                        if (kEvent.getUnitPromotion() != NO_PROMOTION)
                        {
                            pUnit->setHasPromotion((PromotionTypes)kEvent.getUnitPromotion(), true);
                        }
                        if (kEvent.getUnitExperience() != 0)
                        {
                            pUnit->changeExperience(kEvent.getUnitExperience());
                        }
//FfH: End Modify

					}
				}
			}
		}
	}

	CvPlot* pPlot = GC.getMapINLINE().plotINLINE(pTriggeredData->m_iPlotX, pTriggeredData->m_iPlotY);
	if (NULL != pPlot)
	{
		if (::isPlotEventTrigger(pTriggeredData->m_eTrigger))
		{
			FAssert(pPlot->canApplyEvent(eEvent));
			pPlot->applyEvent(eEvent);
		}
	}

	CvUnit* pUnit = getUnit(pTriggeredData->m_iUnitId);
	if (NULL != pUnit)
	{
		FAssert(pUnit->canApplyEvent(eEvent));
		pUnit->applyEvent(eEvent);   // might kill the unit
	}

	for (int i = 0; i < GC.getNumUnitCombatInfos(); ++i)
	{
		if (NO_PROMOTION != kEvent.getUnitCombatPromotion(i))
		{
			int iLoop;
			for (CvUnit* pLoopUnit = firstUnit(&iLoop); NULL != pLoopUnit; pLoopUnit = nextUnit(&iLoop))
			{
				if (pLoopUnit->getUnitCombatType() == i)
				{
					pLoopUnit->setHasPromotion((PromotionTypes)kEvent.getUnitCombatPromotion(i), true);
				}
			}

			setFreePromotion((UnitCombatTypes)i, (PromotionTypes)kEvent.getUnitCombatPromotion(i), true);
		}
	}

	for (int i = 0; i < GC.getNumUnitClassInfos(); ++i)
	{
		if (NO_PROMOTION != kEvent.getUnitClassPromotion(i))
		{
			int iLoop;
			for (CvUnit* pLoopUnit = firstUnit(&iLoop); NULL != pLoopUnit; pLoopUnit = nextUnit(&iLoop))
			{
				if (pLoopUnit->getUnitClassType() == i)
				{
					pLoopUnit->setHasPromotion((PromotionTypes)kEvent.getUnitClassPromotion(i), true);
				}
			}

			setFreePromotion((UnitClassTypes)i, (PromotionTypes)kEvent.getUnitClassPromotion(i), true);
		}
	}

	if (NO_BONUS != kEvent.getBonusRevealed())
	{
		GET_TEAM(getTeam()).setForceRevealedBonus((BonusTypes)kEvent.getBonusRevealed(), true);
	}

	std::vector<CvCity*> apSpreadReligionCities;

	if (kEvent.getConvertOwnCities() > 0)
	{
		if (NO_RELIGION != pTriggeredData->m_eReligion)
		{
			int iLoop;
			for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
			{
				if (!pLoopCity->isHasReligion(pTriggeredData->m_eReligion))
				{
					if (-1 == kEvent.getMaxNumReligions() || pLoopCity->getReligionCount() <= kEvent.getMaxNumReligions())
					{
						apSpreadReligionCities.push_back(pLoopCity);
					}
				}
			}
		}
	}

	while ((int)apSpreadReligionCities.size() > kEvent.getConvertOwnCities())
	{
		int iChosen = GC.getGameINLINE().getSorenRandNum(apSpreadReligionCities.size(), "Even Spread Religion (own)");

		int i = 0;
		for (std::vector<CvCity*>::iterator it = apSpreadReligionCities.begin(); it != apSpreadReligionCities.end(); ++it)
		{
			if (i == iChosen)
			{
				apSpreadReligionCities.erase(it);
				break;
			}
			++i;
		}
	}

	for (std::vector<CvCity*>::iterator it = apSpreadReligionCities.begin(); it != apSpreadReligionCities.end(); ++it)
	{
		(*it)->setHasReligion(pTriggeredData->m_eReligion, true, true, false);
	}

	apSpreadReligionCities.clear();

	if (kEvent.getConvertOtherCities() > 0)
	{
		if (NO_RELIGION != pTriggeredData->m_eReligion)
		{
			if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
			{
				std::vector<CvCity*> apCities;
				int iLoop;
				for (CvCity* pLoopCity = GET_PLAYER(pTriggeredData->m_eOtherPlayer).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER(pTriggeredData->m_eOtherPlayer).nextCity(&iLoop))
				{
					if (!pLoopCity->isHasReligion(pTriggeredData->m_eReligion))
					{
						if (-1 == kEvent.getMaxNumReligions() || pLoopCity->getReligionCount() <= kEvent.getMaxNumReligions())
						{
							apSpreadReligionCities.push_back(pLoopCity);
						}
					}
				}
			}
		}
	}

	while ((int)apSpreadReligionCities.size() > kEvent.getConvertOtherCities())
	{
		int iChosen = GC.getGameINLINE().getSorenRandNum(apSpreadReligionCities.size(), "Even Spread Religion (other)");

		int i = 0;
		for (std::vector<CvCity*>::iterator it = apSpreadReligionCities.begin(); it != apSpreadReligionCities.end(); ++it)
		{
			if (i == iChosen)
			{
				apSpreadReligionCities.erase(it);
				break;
			}
			++i;
		}
	}

	for (std::vector<CvCity*>::iterator it = apSpreadReligionCities.begin(); it != apSpreadReligionCities.end(); ++it)
	{
		(*it)->setHasReligion(pTriggeredData->m_eReligion, true, true, false);
	}

	if (0 != kEvent.getOurAttitudeModifier())
	{
		if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
		{
			if (kEvent.getOurAttitudeModifier() > 0)
			{
				AI_changeMemoryCount(pTriggeredData->m_eOtherPlayer, MEMORY_EVENT_GOOD_TO_US, kEvent.getOurAttitudeModifier());
			}
			else
			{
				AI_changeMemoryCount(pTriggeredData->m_eOtherPlayer, MEMORY_EVENT_BAD_TO_US, -kEvent.getOurAttitudeModifier());
			}
		}
	}

	if (0 != kEvent.getAttitudeModifier())
	{
		if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
		{
			if (kEvent.getAttitudeModifier() > 0)
			{
				GET_PLAYER(pTriggeredData->m_eOtherPlayer).AI_changeMemoryCount(getID(), MEMORY_EVENT_GOOD_TO_US, kEvent.getAttitudeModifier());
			}
			else
			{
				GET_PLAYER(pTriggeredData->m_eOtherPlayer).AI_changeMemoryCount(getID(), MEMORY_EVENT_BAD_TO_US, -kEvent.getAttitudeModifier());
			}
		}
	}

	if (0 != kEvent.getTheirEnemyAttitudeModifier())
	{
		if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
		{
			TeamTypes eWorstEnemy = GET_TEAM(GET_PLAYER(pTriggeredData->m_eOtherPlayer).getTeam()).AI_getWorstEnemy();
			if (NO_TEAM != eWorstEnemy)
			{
				for (int iPlayer = 0; iPlayer < MAX_CIV_PLAYERS; ++iPlayer)
				{
					CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);
					if (kLoopPlayer.isAlive() && kLoopPlayer.getTeam() == eWorstEnemy)
					{
						if (kEvent.getTheirEnemyAttitudeModifier() > 0)
						{
							kLoopPlayer.AI_changeMemoryCount(getID(), MEMORY_EVENT_GOOD_TO_US, kEvent.getTheirEnemyAttitudeModifier());
							AI_changeMemoryCount((PlayerTypes)iPlayer, MEMORY_EVENT_GOOD_TO_US, kEvent.getTheirEnemyAttitudeModifier());
						}
						else
						{
							kLoopPlayer.AI_changeMemoryCount(getID(), MEMORY_EVENT_BAD_TO_US, -kEvent.getTheirEnemyAttitudeModifier());
							AI_changeMemoryCount((PlayerTypes)iPlayer, MEMORY_EVENT_BAD_TO_US, -kEvent.getTheirEnemyAttitudeModifier());
						}
					}
				}
			}
		}
	}

//FfH: Added by Kael 02/23/2008
	if (kEvent.getGlobalCounter() != 0)
	{
        GC.getGameINLINE().changeGlobalCounter(kEvent.getGlobalCounter());
	}
//FfH: End Add

	if (!CvString(kEvent.getPythonCallback()).empty())
	{
		long lResult;

		CyArgsList argsList;
		argsList.add(eEvent);
		argsList.add(gDLL->getPythonIFace()->makePythonObject(pTriggeredData));

		gDLL->getPythonIFace()->callFunction(PYRandomEventModule, kEvent.getPythonCallback(), argsList.makeFunctionArgs(), &lResult);
	}

	if (kEvent.getNumWorldNews() > 0)
	{
		int iText = GC.getGameINLINE().getSorenRandNum(kEvent.getNumWorldNews(), "Event World News choice");

		CvWString szGlobalText;

		TeamTypes eTheirWorstEnemy = NO_TEAM;
		if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
		{
			eTheirWorstEnemy = GET_TEAM(GET_PLAYER(pTriggeredData->m_eOtherPlayer).getTeam()).AI_getWorstEnemy();
		}

		szGlobalText = gDLL->getText(kEvent.getWorldNews(iText).GetCString(),
			getCivilizationAdjectiveKey(),
			NULL != pCity ? pCity->getNameKey() : L"",
			pTriggeredData->m_eOtherPlayer != NO_PLAYER ? GET_PLAYER(pTriggeredData->m_eOtherPlayer).getCivilizationAdjectiveKey() : L"",
			NULL != pOtherPlayerCity ? pOtherPlayerCity->getNameKey() : L"",
			NO_RELIGION != pTriggeredData->m_eReligion ? GC.getReligionInfo(pTriggeredData->m_eReligion).getAdjectiveKey() : L"",
			NO_TEAM != eTheirWorstEnemy ? GET_TEAM(eTheirWorstEnemy).getName().GetCString() : L"",
			NO_CORPORATION != pTriggeredData->m_eCorporation ? GC.getCorporationInfo(pTriggeredData->m_eCorporation).getTextKeyWide() : L""
			);

		for (int iPlayer = 0; iPlayer < MAX_CIV_PLAYERS; ++iPlayer)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);

			if (kLoopPlayer.isAlive())
			{
				if (GET_TEAM(kLoopPlayer.getTeam()).isHasMet(getTeam()) && (NO_PLAYER == pTriggeredData->m_eOtherPlayer || GET_TEAM(GET_PLAYER(pTriggeredData->m_eOtherPlayer).getTeam()).isHasMet(getTeam())))
				{
					bool bShowPlot = GC.getEventTriggerInfo(pTriggeredData->m_eTrigger).isShowPlot();

					if (bShowPlot)
					{
						if (kLoopPlayer.getTeam() != getTeam())
						{
							if (NULL == pPlot || !pPlot->isRevealed(kLoopPlayer.getTeam(), false))
							{
								bShowPlot = false;
							}
						}
					}

					if (bShowPlot)
					{
						gDLL->getInterfaceIFace()->addMessage((PlayerTypes)iPlayer, false, GC.getEVENT_MESSAGE_TIME(), szGlobalText, "AS2D_CIVIC_ADOPT", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), pTriggeredData->m_iPlotX, pTriggeredData->m_iPlotY, true, true);
					}
					else
					{
						gDLL->getInterfaceIFace()->addMessage((PlayerTypes)iPlayer, false, GC.getEVENT_MESSAGE_TIME(), szGlobalText, "AS2D_CIVIC_ADOPT", MESSAGE_TYPE_MINOR_EVENT);
					}
				}
			}
		}

		GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getID(), szGlobalText, pTriggeredData->m_iPlotX, pTriggeredData->m_iPlotY, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));
	}

	if (!CvWString(kEvent.getLocalInfoTextKey()).empty())
	{
		CvWString szLocalText;

		TeamTypes eTheirWorstEnemy = NO_TEAM;
		if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
		{
			eTheirWorstEnemy = GET_TEAM(GET_PLAYER(pTriggeredData->m_eOtherPlayer).getTeam()).AI_getWorstEnemy();
		}

		szLocalText = gDLL->getText(kEvent.getLocalInfoTextKey(),
			getCivilizationAdjectiveKey(),
			NULL != pCity ? pCity->getNameKey() : L"",
			pTriggeredData->m_eOtherPlayer != NO_PLAYER ? GET_PLAYER(pTriggeredData->m_eOtherPlayer).getCivilizationAdjectiveKey() : L"",
			NULL != pOtherPlayerCity ? pOtherPlayerCity->getNameKey() : L"",
			NO_RELIGION != pTriggeredData->m_eReligion ? GC.getReligionInfo(pTriggeredData->m_eReligion).getAdjectiveKey() : L"",
			NO_TEAM != eTheirWorstEnemy ? GET_TEAM(eTheirWorstEnemy).getName().GetCString() : L"",
			NO_CORPORATION != pTriggeredData->m_eCorporation ? GC.getCorporationInfo(pTriggeredData->m_eCorporation).getTextKeyWide() : L""
			);

			if (GC.getEventTriggerInfo(pTriggeredData->m_eTrigger).isShowPlot())
			{
				gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szLocalText, "AS2D_CIVIC_ADOPT", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), pTriggeredData->m_iPlotX, pTriggeredData->m_iPlotY, true, true);
			}
			else
			{
				gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szLocalText, "AS2D_CIVIC_ADOPT", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"));
			}

	}

	if (!CvWString(kEvent.getOtherPlayerPopup()).empty())
	{
		if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
		{
			CvWString szText = gDLL->getText(kEvent.getOtherPlayerPopup(),
				getCivilizationAdjectiveKey(),
				NULL != pCity ? pCity->getNameKey() : L"",
				pTriggeredData->m_eOtherPlayer != NO_PLAYER ? GET_PLAYER(pTriggeredData->m_eOtherPlayer).getCivilizationAdjectiveKey() : L"",
				NULL != pOtherPlayerCity ? pOtherPlayerCity->getNameKey() : L"",
				NO_RELIGION != pTriggeredData->m_eReligion ? GC.getReligionInfo(pTriggeredData->m_eReligion).getAdjectiveKey() : L"",
				NO_CORPORATION != pTriggeredData->m_eCorporation ? GC.getCorporationInfo(pTriggeredData->m_eCorporation).getTextKeyWide() : L""
				);

			CvPopupInfo* pInfo = new CvPopupInfo();

			if (NULL != pInfo)
			{
				pInfo->setText(szText);

				GET_PLAYER(pTriggeredData->m_eOtherPlayer).addPopup(pInfo);
			}
		}
	}

	bool bDeleteTrigger = bUpdateTrigger;

	for (int iEvent = 0; iEvent < GC.getNumEventInfos(); ++iEvent)
	{
		if (0 == kEvent.getAdditionalEventTime(iEvent))
		{
			if (kEvent.getAdditionalEventChance(iEvent) > 0)
			{
				if (canDoEvent((EventTypes)iEvent, *pTriggeredData))
				{
					if (GC.getGameINLINE().getSorenRandNum(100, "Additional Event") < kEvent.getAdditionalEventChance(iEvent))
					{
						applyEvent((EventTypes)iEvent, iEventTriggeredId, false);
					}
				}
			}
		}
		else
		{
			bool bSetTimer = true;
			if (kEvent.getAdditionalEventChance(iEvent) > 0)
			{
				if (GC.getGameINLINE().getSorenRandNum(100, "Additional Event 2") >= kEvent.getAdditionalEventChance(iEvent))
				{
					bSetTimer = false;
				}
			}

			if (bSetTimer)
			{
				EventTriggeredData kTriggered = *pTriggeredData;
				kTriggered.m_iTurn = (GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getGrowthPercent() * kEvent.getAdditionalEventTime((EventTypes)iEvent)) / 100 + GC.getGameINLINE().getGameTurn();

				const EventTriggeredData* pExistingTriggered = getEventCountdown((EventTypes)iEvent);

				if (NULL != pExistingTriggered)
				{
					kTriggered.m_iTurn = std::min(kTriggered.m_iTurn, pExistingTriggered->m_iTurn);
				}

				setEventCountdown((EventTypes)iEvent, kTriggered);
				bDeleteTrigger = false;
			}
		}
	}

	if (bDeleteTrigger)
	{
		deleteEventTriggered(iEventTriggeredId);
	}
}

bool CvPlayer::isValidEventTech(TechTypes eTech, EventTypes eEvent, PlayerTypes eOtherPlayer) const
{
	CvEventInfo& kEvent = GC.getEventInfo(eEvent);

	if (0 == kEvent.getTechPercent() && 0 == kEvent.getTechCostPercent())
	{
		return false;
	}

	if (kEvent.getTechPercent() < 0 && GET_TEAM(getTeam()).getResearchProgress(eTech) <= 0)
	{
		return false;
	}

	if (!canResearch(eTech))
	{
		return false;
	}

	if (getResearchTurnsLeft(eTech, true) < kEvent.getTechMinTurnsLeft())
	{
		return false;
	}

	if (NO_PLAYER != eOtherPlayer && !GET_TEAM(GET_PLAYER(eOtherPlayer).getTeam()).isHasTech(eTech))
	{
		return false;
	}

	return true;
}


TechTypes CvPlayer::getBestEventTech(EventTypes eEvent, PlayerTypes eOtherPlayer) const
{
	TechTypes eBestTech = NO_TECH;
	CvEventInfo& kEvent = GC.getEventInfo(eEvent);

	if (0 == kEvent.getTechPercent() && 0 == kEvent.getTechCostPercent())
	{
		return NO_TECH;
	}

	if (NO_TECH != kEvent.getTech())
	{
		eBestTech = (TechTypes)kEvent.getTech();
	}
	else
	{
		bool bFoundFlavor = false;
		for (int i = 0; i < GC.getNumFlavorTypes(); ++i)
		{
			if (0 != kEvent.getTechFlavorValue(i))
			{
				bFoundFlavor = true;
				break;
			}
		}

		if (!bFoundFlavor)
		{
			eBestTech = getCurrentResearch();
		}
	}

	if (NO_TECH != eBestTech)
	{
		if (!isValidEventTech(eBestTech, eEvent, eOtherPlayer))
		{
			eBestTech = NO_TECH;
		}
	}
	else
	{
		int iBestValue = 0;
		for (int iTech = 0; iTech < GC.getNumTechInfos(); ++iTech)
		{
			if (isValidEventTech((TechTypes)iTech, eEvent, eOtherPlayer))
			{
				int iValue = 0;
				for (int i = 0; i < GC.getNumFlavorTypes(); ++i)
				{
					iValue += kEvent.getTechFlavorValue(i) * GC.getTechInfo((TechTypes)iTech).getFlavorValue(i);
				}

				if (iValue > iBestValue)
				{
					eBestTech = (TechTypes)iTech;
					iBestValue = iValue;
				}
			}
		}
	}

	return eBestTech;
}

int CvPlayer::getEventCost(EventTypes eEvent, PlayerTypes eOtherPlayer, bool bRandom) const
{
	CvEventInfo& kEvent = GC.getEventInfo(eEvent);

	int iGold = kEvent.getGold();
	if (bRandom)
	{
		iGold += kEvent.getRandomGold();
	}

	iGold *= std::max(0, calculateInflationRate() + 100);
	iGold /= 100;

	TechTypes eBestTech = getBestEventTech(eEvent, eOtherPlayer);

	if (NO_TECH != eBestTech)
	{
		iGold -= (kEvent.getTechCostPercent() * GET_TEAM(getTeam()).getResearchCost(eBestTech)) / 100;
	}

	return iGold;
}


void CvPlayer::doEvents()
{
	if (GC.getGameINLINE().isOption(GAMEOPTION_NO_EVENTS))
	{
		return;
	}

	if (isBarbarian())
	{
		return;
	}

	CvEventMap::iterator it = m_mapEventsOccured.begin();
	while (it != m_mapEventsOccured.end())
	{
		if (checkExpireEvent(it->first, it->second))
		{
			expireEvent(it->first, it->second, true);
			it = m_mapEventsOccured.erase(it);
		}
		else
		{
			++it;
		}
	}


	bool bNewEventEligible = true;
	if (GC.getGameINLINE().getElapsedGameTurns() < GC.getDefineINT("FIRST_EVENT_DELAY_TURNS"))
	{
		bNewEventEligible = false;
	}

	if (bNewEventEligible)
	{

//FfH: Modifed by Kael 09/26/2007
//		if (GC.getGameINLINE().getSorenRandNum(GC.getDefineINT("EVENT_PROBABILITY_ROLL_SIDES"), "Global event check") >= GC.getEraInfo(getCurrentEra()).getEventChancePerTurn())
        int iChance = GC.getEraInfo(getCurrentEra()).getEventChancePerTurn();
        if (GC.getGameINLINE().isOption(GAMEOPTION_DOUBLE_EVENTS))
        {
            iChance *= 2;
        }
		if (GC.getGameINLINE().getSorenRandNum(GC.getDefineINT("EVENT_PROBABILITY_ROLL_SIDES"), "Global event check") >= iChance)
//FfH: End Modify

		{
			bNewEventEligible = false;
		}
	}

	std::vector< std::pair<EventTriggeredData*, int> > aePossibleEventTriggerWeights;
	int iTotalWeight = 0;
	for (int i = 0; i < GC.getNumEventTriggerInfos(); ++i)
	{
		int iWeight = getEventTriggerWeight((EventTriggerTypes)i);
		if (iWeight == -1)
		{
			//DEBUG
			/**
			TCHAR szOut[1024];
			sprintf(szOut, "Event %d: %S   ---   %S \n", i, GC.getEventTriggerInfo((EventTriggerTypes)i).getTextKeyWide()  , GC.getEventTriggerInfo((EventTriggerTypes)i).getDescription());
			gDLL->logMsg("EventTriggered.log",szOut, false, false);
			**/
			trigger((EventTriggerTypes)i);
		}
		else if (iWeight > 0 && bNewEventEligible)
		{
			EventTriggeredData* pTriggerData = initTriggeredData((EventTriggerTypes)i);
			if (NULL != pTriggerData)
			{
				iTotalWeight += iWeight;
				aePossibleEventTriggerWeights.push_back(std::make_pair(pTriggerData, iTotalWeight));
			}
		}
	}

	if (iTotalWeight > 0)
	{
		bool bFired = false;
		int iValue = GC.getGameINLINE().getSorenRandNum(iTotalWeight, "Event trigger");
		for (std::vector< std::pair<EventTriggeredData*, int> >::iterator it = aePossibleEventTriggerWeights.begin(); it != aePossibleEventTriggerWeights.end(); ++it)
		{
			EventTriggeredData* pTriggerData = (*it).first;
			if (NULL != pTriggerData)
			{
				if (iValue < (*it).second && !bFired)
				{
					trigger(*pTriggerData);
					bFired = true;
				}
				else
				{
					deleteEventTriggered(pTriggerData->getID());
				}
			}
		}
	}


	std::vector<int> aCleanup;
	for (int i = 0; i < GC.getNumEventInfos(); ++i)
	{
		const EventTriggeredData* pTriggeredData = getEventCountdown((EventTypes)i);
		if (NULL != pTriggeredData)
		{
			if (GC.getGameINLINE().getGameTurn() >= pTriggeredData->m_iTurn)
			{
				applyEvent((EventTypes)i, pTriggeredData->m_iId);
				resetEventCountdown((EventTypes)i);
				aCleanup.push_back(pTriggeredData->m_iId);
			}
		}
	}

	for (std::vector<int>::iterator it = aCleanup.begin(); it != aCleanup.end(); ++it)
	{
		bool bDelete = true;

		for (int i = 0; i < GC.getNumEventInfos(); ++i)
		{
			const EventTriggeredData* pTriggeredData = getEventCountdown((EventTypes)i);
			if (NULL != pTriggeredData)
			{
				if (pTriggeredData->m_iId == *it)
				{
					bDelete = false;
					break;
				}
			}
		}

		if (bDelete)
		{
			deleteEventTriggered(*it);
		}
	}
}


void CvPlayer::expireEvent(EventTypes eEvent, const EventTriggeredData& kTriggeredData, bool bFail)
{
	FAssert(getEventOccured(eEvent) == &kTriggeredData);
	FAssert(GC.getEventInfo(eEvent).isQuest() || GC.getGameINLINE().getGameTurn() - kTriggeredData.m_iTurn <= 4);

	if (GC.getEventInfo(eEvent).isQuest())
	{
		CvMessageQueue::iterator it;
		for (it = m_listGameMessages.begin(); it != m_listGameMessages.end(); ++it)
		{
			CvTalkingHeadMessage& message = *it;

			// the trigger ID is stored in the otherwise unused length field
			if (message.getLength() == kTriggeredData.getID())
			{
				m_listGameMessages.erase(it);
				gDLL->getInterfaceIFace()->dirtyTurnLog(getID());
				break;
			}
		}

		if (bFail)
		{
			gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), gDLL->getText(GC.getEventInfo(eEvent).getQuestFailTextKey()), "AS2D_CIVIC_ADOPT", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_RED"));
		}
	}
}

bool CvPlayer::checkExpireEvent(EventTypes eEvent, const EventTriggeredData& kTriggeredData) const
{
	CvEventInfo& kEvent = GC.getEventInfo(eEvent);

	if (!CvString(kEvent.getPythonExpireCheck()).empty())
	{
		long lResult;

		CyArgsList argsList;
		argsList.add(eEvent);
		argsList.add(gDLL->getPythonIFace()->makePythonObject(&kTriggeredData));

		gDLL->getPythonIFace()->callFunction(PYRandomEventModule, kEvent.getPythonExpireCheck(), argsList.makeFunctionArgs(), &lResult);

		if (0 != lResult)
		{
			return true;
		}
	}

	if (!kEvent.isQuest())
	{
		if (GC.getGameINLINE().getGameTurn() - kTriggeredData.m_iTurn > 2)
		{
			return true;
		}

		return false;
	}

	CvEventTriggerInfo& kTrigger = GC.getEventTriggerInfo(kTriggeredData.m_eTrigger);

	FAssert(kTriggeredData.m_ePlayer != NO_PLAYER);

	CvPlayer& kPlayer = GET_PLAYER(kTriggeredData.m_ePlayer);

	if (kTrigger.isStateReligion() & kTrigger.isPickReligion())
	{
		if (kPlayer.getStateReligion() != kTriggeredData.m_eReligion)
		{
			return true;
		}
	}

	if (NO_CIVIC != kTrigger.getCivic())
	{
		if (!kPlayer.isCivic((CivicTypes)kTrigger.getCivic()))
		{
			return true;
		}
	}

	if (kTriggeredData.m_iCityId != -1)
	{
		if (NULL == kPlayer.getCity(kTriggeredData.m_iCityId))
		{
			return true;
		}
	}

	if (kTriggeredData.m_iUnitId != -1)
	{
		if (NULL == kPlayer.getUnit(kTriggeredData.m_iUnitId))
		{
			return true;
		}
	}

	if (NO_PLAYER != kTriggeredData.m_eOtherPlayer)
	{
		if (!GET_PLAYER(kTriggeredData.m_eOtherPlayer).isAlive())
		{
			return true;
		}

		if (kTriggeredData.m_iOtherPlayerCityId != -1)
		{
			if (NULL == GET_PLAYER(kTriggeredData.m_eOtherPlayer).getCity(kTriggeredData.m_iOtherPlayerCityId))
			{
				return true;
			}
		}
	}

	if (kTrigger.getNumObsoleteTechs() > 0)
	{
		for (int iI = 0; iI < kTrigger.getNumObsoleteTechs(); iI++)
		{
			if (GET_TEAM(getTeam()).isHasTech((TechTypes)(kTrigger.getObsoleteTech(iI))))
			{
				return true;
			}
		}
	}

	return false;
}


void CvPlayer::trigger(EventTriggerTypes eTrigger)
{
	initTriggeredData(eTrigger, true);
}

void CvPlayer::trigger(const EventTriggeredData& kData)
{
	if (isHuman())
	{
		CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_EVENT, kData.getID());
		addPopup(pInfo);
	}
	else
	{
		EventTypes eEvent = AI_chooseEvent(kData.getID());
		if (NO_EVENT != eEvent)
		{
			applyEvent(eEvent, kData.getID());
		}
	}
}

bool CvPlayer::canTrigger(EventTriggerTypes eTrigger, PlayerTypes ePlayer, ReligionTypes eReligion) const
{
	if (!isAlive())
	{
		return false;
	}

	if (getID() == ePlayer)
	{
		return false;
	}

	CvPlayer& kPlayer = GET_PLAYER(ePlayer);
	CvEventTriggerInfo& kTrigger = GC.getEventTriggerInfo(eTrigger);

	if (getTeam() == kPlayer.getTeam())
	{
		return false;
	}

	if (!kTrigger.isPickPlayer())
	{
		return false;
	}

	if (!GET_TEAM(getTeam()).isHasMet(kPlayer.getTeam()))
	{
		return false;
	}

	if (isHuman() && kTrigger.isOtherPlayerAI())
	{
		return false;
	}

	if (GET_TEAM(getTeam()).isAtWar(kPlayer.getTeam()) != kTrigger.isOtherPlayerWar())
	{
		return false;
	}

	if (NO_TECH != kTrigger.getOtherPlayerHasTech())
	{
		if (!GET_TEAM(getTeam()).isHasTech((TechTypes)kTrigger.getOtherPlayerHasTech()))
		{
			return false;
		}
	}

	if (kTrigger.getOtherPlayerShareBorders() > 0)
	{
		int iCount = 0;

		for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); ++iI)
		{
			CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

			if (!pLoopPlot->isWater())
			{
				if ((pLoopPlot->getOwnerINLINE() == getID()) && pLoopPlot->isAdjacentPlayer(ePlayer, true))
				{
					++iCount;
				}
			}
		}

		if (iCount < kTrigger.getOtherPlayerShareBorders())
		{
			return false;
		}
	}

	if (NO_RELIGION != eReligion)
	{
		bool bHasReligion = kTrigger.isStateReligion() ? (getStateReligion() == eReligion) : (getHasReligionCount(eReligion) > 0);

		if (kTrigger.isOtherPlayerHasReligion())
		{
			if (!bHasReligion)
			{
				return false;
			}
		}

		if (kTrigger.isOtherPlayerHasOtherReligion())
		{
			if (bHasReligion)
			{
				return false;
			}

			if (kTrigger.isStateReligion() && getStateReligion() == NO_RELIGION)
			{
				return false;
			}

		}
	}

	return true;
}

CvCity* CvPlayer::pickTriggerCity(EventTriggerTypes eTrigger) const
{
	CvCity* pCity = NULL;
	std::vector<CvCity*> apCities;
	int iLoop;
	int iBestValue = MIN_INT;
	for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		int iValue = pLoopCity->getTriggerValue(eTrigger);

		if (iValue >= iBestValue && iValue != MIN_INT)
		{
			if (iValue > iBestValue)
			{
				apCities.clear();
				iBestValue = iValue;
			}
			apCities.push_back(pLoopCity);
		}
	}

	if (apCities.size() > 0)
	{
		int iChosen = GC.getGameINLINE().getSorenRandNum(apCities.size(), "Event pick city");
		pCity = apCities[iChosen];
	}

	return pCity;
}

CvUnit* CvPlayer::pickTriggerUnit(EventTriggerTypes eTrigger, CvPlot* pPlot, bool bPickPlot) const
{
	CvUnit* pUnit = NULL;
	std::vector<CvUnit*> apUnits;
	int iLoop;
	int iBestValue = MIN_INT;
	for (CvUnit* pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{
		int iValue = pLoopUnit->getTriggerValue(eTrigger, pPlot, bPickPlot);

		if (iValue >= iBestValue && iValue != MIN_INT)
		{
			if (iValue > iBestValue)
			{
				apUnits.clear();
				iBestValue = iValue;
			}
			apUnits.push_back(pLoopUnit);
		}
	}

	if (apUnits.size() > 0)
	{
		int iChosen = GC.getGameINLINE().getSorenRandNum(apUnits.size(), "Event pick unit");
		pUnit = apUnits[iChosen];
	}

	return pUnit;
}

int CvPlayer::getEventTriggerWeight(EventTriggerTypes eTrigger) const
{
	CvEventTriggerInfo& kTrigger = GC.getEventTriggerInfo(eTrigger);

	if (NO_HANDICAP != kTrigger.getMinDifficulty())
	{
		if (GC.getGameINLINE().getHandicapType() < kTrigger.getMinDifficulty())
		{
			return 0;
		}
	}

	if (kTrigger.isSinglePlayer() && GC.getGameINLINE().isGameMultiPlayer())
	{
		return 0;
	}

	if (!GC.getGameINLINE().isEventActive(eTrigger))
	{
		return 0;
	}

	if (kTrigger.getNumObsoleteTechs() > 0)
	{
		for (int iI = 0; iI < kTrigger.getNumObsoleteTechs(); iI++)
		{
			if (GET_TEAM(getTeam()).isHasTech((TechTypes)(kTrigger.getObsoleteTech(iI))))
			{
				return 0;
			}
		}
	}

	if (!kTrigger.isRecurring())
	{
		if (isTriggerFired(eTrigger))
		{
			return 0;
		}

//FfH: Added by Kael 09/19/2008
        if (kTrigger.isGlobal())
        {
            if (GC.getGameINLINE().isEventTriggered(eTrigger))
            {
                return 0;
            }
        }
//FfH: End Add

	}

	if (kTrigger.getNumPrereqOrTechs() > 0)
	{
		bool bFoundValid = false;

		for (int iI = 0; iI < kTrigger.getNumPrereqOrTechs(); iI++)
		{
			if (GET_TEAM(getTeam()).isHasTech((TechTypes)(kTrigger.getPrereqOrTechs(iI))))
			{
				bFoundValid = true;
				break;
			}
		}

		if (!bFoundValid)
		{
			return 0;
		}
	}


	if (kTrigger.getNumPrereqAndTechs() > 0)
	{
		bool bFoundValid = true;

		for (int iI = 0; iI < kTrigger.getNumPrereqAndTechs(); iI++)
		{
			if (!GET_TEAM(getTeam()).isHasTech((TechTypes)(kTrigger.getPrereqAndTechs(iI))))
			{
				bFoundValid = false;
				break;
			}
		}

		if (!bFoundValid)
		{
			return 0;
		}
	}

	if (kTrigger.getNumPrereqEvents() > 0)
	{
		bool bFoundValid = true;
		for (int iI = 0; iI < kTrigger.getNumPrereqEvents(); iI++)
		{
			if (NULL == getEventOccured((EventTypes)kTrigger.getPrereqEvent(iI)))
			{
				bFoundValid = false;
				break;
			}
		}

		if (!bFoundValid)
		{
			return 0;
		}
	}

	if (NO_CIVIC != kTrigger.getCivic())
	{
		bool bFoundValid = false;

		for (int iI = 0; iI < GC.getNumCivicOptionInfos(); ++iI)
		{
			if (getCivics((CivicOptionTypes)iI) == kTrigger.getCivic())
			{
				bFoundValid = true;
				break;
			}
		}

		if (!bFoundValid)
		{
			return 0;
		}
	}

//FfH Events: Added by Kael 08/08/2007
	if (kTrigger.getPrereqGlobalCounter() != 0)
	{
	    if (GC.getGameINLINE().getGlobalCounter() < kTrigger.getPrereqGlobalCounter())
	    {
	        return 0;
	    }
	}
	if (kTrigger.getPrereqAlignment() != NO_ALIGNMENT)
	{
	    if (getAlignment() != kTrigger.getPrereqAlignment())
	    {
	        return 0;
	    }
	}
	if (kTrigger.getPrereqTrait() != NO_TRAIT)
	{
	    if (!hasTrait((TraitTypes)kTrigger.getPrereqTrait()))
	    {
	        return 0;
	    }
	}
	if (kTrigger.getPrereqCivilizationPleased() != NO_CIVILIZATION)
	{
	    if (kTrigger.getPrereqCivilizationPleased() == getCivilizationType())
	    {
	        return 0;
	    }
	    bool bValid = false;
		for (int iI = 0; iI < MAX_PLAYERS; iI++)
        {
            if (GET_PLAYER((PlayerTypes)iI).isAlive())
            {
                if (GET_PLAYER((PlayerTypes)iI).getCivilizationType() == kTrigger.getPrereqCivilizationPleased())
                {
                    if (GET_TEAM(getTeam()).isHasMet(GET_PLAYER((PlayerTypes)iI).getTeam()))
                    {
                        if (GET_PLAYER((PlayerTypes)iI).AI_getAttitude(getID()) >= ATTITUDE_PLEASED)
                        {
                            bValid = true;
                        }
                    }
                }
            }
        }
        if (bValid == false)
        {
            return 0;
        }
	}
	if (kTrigger.getPrereqLeader() != NO_LEADER)
	{
	    if (getLeaderType() != kTrigger.getPrereqLeader())
	    {
	        return 0;
	    }
	}
//FfH: End Add

	if (kTrigger.getMinTreasury() > 0)
	{
		if (getGold() < kTrigger.getMinTreasury())
		{
			return 0;
		}
	}

	if (GC.getMapINLINE().getNumLandAreas() < kTrigger.getMinMapLandmass())
	{
		return 0;
	}

	if (kTrigger.getMinOurLandmass() > 0 || kTrigger.getMaxOurLandmass() != -1)
	{
		int iNumLandmass = 0;

		int iLoop;
		for (CvArea* pArea = GC.getMapINLINE().firstArea(&iLoop); NULL != pArea; pArea = GC.getMapINLINE().nextArea(&iLoop))
		{
			if (!pArea->isWater())
			{
				if (pArea->getCitiesPerPlayer(getID()) > 0)
				{
					++iNumLandmass;
				}
			}
		}

		if (iNumLandmass < kTrigger.getMinOurLandmass())
		{
			return 0;
		}

		if (kTrigger.getMaxOurLandmass() != -1 && iNumLandmass > kTrigger.getMaxOurLandmass())
		{
			return 0;
		}
	}

	if (kTrigger.getProbability() < 0)
	{
		return kTrigger.getProbability();
	}

	int iProbability = kTrigger.getProbability();

	if (kTrigger.isProbabilityUnitMultiply() && kTrigger.getNumUnits() > 0)
	{
		int iNumUnits = 0;
		int iLoop;
		for (CvUnit* pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
		{
			if (MIN_INT != pLoopUnit->getTriggerValue(eTrigger, NULL, true))
			{
				++iNumUnits;
			}
		}

		iProbability *= iNumUnits;
	}

	if (kTrigger.isProbabilityBuildingMultiply() && kTrigger.getNumBuildings() > 0)
	{
		int iNumBuildings = 0;
		for (int i = 0; i < kTrigger.getNumBuildingsRequired(); ++i)
		{
			if (kTrigger.getBuildingRequired(i) != NO_BUILDINGCLASS)
			{
				iNumBuildings += getBuildingClassCount((BuildingClassTypes)kTrigger.getBuildingRequired(i));
			}
		}

		iProbability *= iNumBuildings;
	}

	return iProbability;
}


PlayerTypes CvPlayer::getSplitEmpirePlayer(int iAreaId) const
{
	// can't create different derivative civs on the same continent
	for (int iPlayer = 0; iPlayer < MAX_CIV_PLAYERS; ++iPlayer)
	{
		CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayer);
		if (kLoopPlayer.isAlive() && kLoopPlayer.getParent() == getID())
		{
			CvCity* pLoopCapital = kLoopPlayer.getCapitalCity();
			if (NULL != pLoopCapital)
			{
				if (pLoopCapital->area()->getID() == iAreaId)
				{
					return NO_PLAYER;
				}
			}
		}
	}

	PlayerTypes eNewPlayer = NO_PLAYER;

	// Try to find a player who's never been in the game before
	for (int i = 0; i < MAX_CIV_PLAYERS; ++i)
	{
		if (!GET_PLAYER((PlayerTypes)i).isEverAlive())
		{
			eNewPlayer = (PlayerTypes)i;
			break;
		}
	}

	if (eNewPlayer == NO_PLAYER)
	{
		// Try to recycle a dead player
		for (int i = 0; i < MAX_CIV_PLAYERS; ++i)
		{
			if (!GET_PLAYER((PlayerTypes)i).isAlive())
			{
				eNewPlayer = (PlayerTypes)i;
				break;
			}
		}
	}

	return eNewPlayer;
}

bool CvPlayer::canSplitEmpire() const
{
	int iLoopArea;

	if (GC.getGameINLINE().isOption(GAMEOPTION_NO_VASSAL_STATES))
	{
		return false;
	}

	if (GET_TEAM(getTeam()).isAVassal())
	{
		return false;
	}

	CivLeaderArray aLeaders;
	if (!getSplitEmpireLeaders(aLeaders))
	{
		return false;
	}

	bool bFoundArea = false;

	for (CvArea* pLoopArea = GC.getMapINLINE().firstArea(&iLoopArea); pLoopArea != NULL; pLoopArea = GC.getMapINLINE().nextArea(&iLoopArea))
	{
		if (canSplitArea(pLoopArea->getID()))
		{
			bFoundArea = true;
			break;
		}
	}

	if (!bFoundArea)
	{
		return false;
	}

	return true;
}

bool CvPlayer::canSplitArea(int iAreaId) const
{
	CvArea* pArea = GC.getMapINLINE().getArea(iAreaId);
	CvCity* pCapital = getCapitalCity();

	if (NULL == pCapital)
	{
		return false;
	}

	if (NULL == pArea || pArea == pCapital->area())
	{
		return false;
	}

	if (0 == pArea->getCitiesPerPlayer(getID()))
	{
		return false;
	}

	PlayerTypes ePlayer = getSplitEmpirePlayer(pArea->getID());
	if (NO_PLAYER == ePlayer)
	{
		return false;
	}

	if (!GET_PLAYER(ePlayer).isAlive())
	{
		if (pArea->getCitiesPerPlayer(getID()) <= 1)
		{
			return false;
		}
	}

	return true;
}

bool CvPlayer::getSplitEmpireLeaders(CivLeaderArray& aLeaders) const
{
	aLeaders.clear();

	for (int i = 0; i < GC.getNumCivilizationInfos(); ++i)
	{
		bool bValid = true;

//FfH: Modified by Kael 01/16/2009
//		if (getCivilizationType() == i)
//		{
//			bValid = false;
//		}
//FfH: End Modify

		if (bValid)
		{
			if (!GC.getCivilizationInfo((CivilizationTypes)i).isPlayable() || !GC.getCivilizationInfo((CivilizationTypes)i).isAIPlayable())
			{
				bValid = false;
			}
		}

		if (bValid)
		{
			for (int j = 0; j < MAX_CIV_PLAYERS; ++j)
			{
				if (getID() != j && GET_PLAYER((PlayerTypes)j).isEverAlive() && GET_PLAYER((PlayerTypes)j).getCivilizationType() == i)
				{
					bValid = false;
					break;
				}
			}
		}

		if (bValid)
		{
			for (int j = 0; j < GC.getNumLeaderHeadInfos(); ++j)
			{
				bool bLeaderValid = true;
				if (!GC.getCivilizationInfo((CivilizationTypes)i).isLeaders(j) && !GC.getGameINLINE().isOption(GAMEOPTION_LEAD_ANY_CIV))
				{
					bLeaderValid = false;
				}

//FfH: Added by Kael 01/16/2008
                if (getCivilizationType() == i)
                {
                    if (GC.getLeaderHeadInfo((LeaderHeadTypes)j).isGraphicalOnly())
                    {
                        if (GC.getLeaderHeadInfo((LeaderHeadTypes)j).getAlignment() == getAlignment())
                        {
                            bLeaderValid = true;
                        }
                    }
                }
                if (GC.getLeaderHeadInfo((LeaderHeadTypes)j).getFavoriteWonder() == NO_BUILDING)
                {
                    bLeaderValid = false;
                }
//FfH: End Add

				if (bLeaderValid)
				{
					for (int k = 0; k < MAX_CIV_PLAYERS; ++k)
					{
						if (GET_PLAYER((PlayerTypes)k).isEverAlive() && GET_PLAYER((PlayerTypes)k).getPersonalityType() == j)
						{
							bLeaderValid = false;
						}
					}
				}

				if (bLeaderValid)
				{
					aLeaders.push_back(std::make_pair((CivilizationTypes)i, (LeaderHeadTypes)j));
				}
			}
		}
	}

	return (aLeaders.size() > 0);
}

bool CvPlayer::splitEmpire(int iAreaId)
{
	PROFILE_FUNC();

	if (!canSplitEmpire())
	{
		return false;
	}

	if (!canSplitArea(iAreaId))
	{
		return false;
	}

	CvArea* pArea = GC.getMapINLINE().getArea(iAreaId);
	if (NULL == pArea)
	{
		return false;
	}

	PlayerTypes eNewPlayer = getSplitEmpirePlayer(iAreaId);
	if (eNewPlayer == NO_PLAYER)
	{
		return false;
	}

	bool bPlayerExists = GET_TEAM(GET_PLAYER(eNewPlayer).getTeam()).isAlive();
	FAssert(!bPlayerExists);
	if (!bPlayerExists)
	{
		int iBestValue = -1;
		LeaderHeadTypes eBestLeader = NO_LEADER;
		CivilizationTypes eBestCiv = NO_CIVILIZATION;

		CivLeaderArray aLeaders;
		if (getSplitEmpireLeaders(aLeaders))
		{
			CivLeaderArray::iterator it;
			for (it = aLeaders.begin(); it != aLeaders.end(); ++it)
			{
				int iValue = (1 + GC.getGameINLINE().getSorenRandNum(100, "Choosing Split Personality"));

//FfH: Added by Kael 01/16/2008
				if (getCivilizationType() == it->first)
				{
                    iValue += 1000;
                }
                if (GC.getLeaderHeadInfo(it->second).isGraphicalOnly())
                {
                    iValue += 1000;
                }
//FfH: End Add

				if (GC.getCivilizationInfo(getCivilizationType()).getDerivativeCiv() == it->first)
				{
					iValue += 1000;
				}

				if (iValue > iBestValue)
				{
					iBestValue = iValue;
					eBestLeader = it->second;
					eBestCiv = it->first;
				}
			}
		}

		if (eBestLeader == NO_LEADER || eBestCiv == NO_CIVILIZATION)
		{
			return false;
		}

		CvWString szMessage = gDLL->getText("TXT_KEY_MISC_EMPIRE_SPLIT", getNameKey(), GC.getCivilizationInfo(eBestCiv).getShortDescriptionKey(), GC.getLeaderHeadInfo(eBestLeader).getTextKeyWide());
		for (int i = 0; i < MAX_CIV_PLAYERS; ++i)
		{
			if (GET_PLAYER((PlayerTypes)i).isAlive())
			{
				if (i == getID() || i == eNewPlayer || GET_TEAM(GET_PLAYER((PlayerTypes)i).getTeam()).isHasMet(getTeam()))
				{
					gDLL->getInterfaceIFace()->addMessage((PlayerTypes)i, false, GC.getEVENT_MESSAGE_TIME(), szMessage, "AS2D_REVOLTEND", MESSAGE_TYPE_MAJOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_CITY_BAR_CAPITAL_TEXTURE")->getPath());
				}
			}
		}
		GC.getGameINLINE().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, getID(), szMessage, -1, -1, (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"));

		// remove leftover culture from old recycled player
		for (int iPlot = 0; iPlot < GC.getMapINLINE().numPlotsINLINE(); ++iPlot)
		{
			CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iPlot);

			pLoopPlot->setCulture(eNewPlayer, 0, false, false);
		}

		GC.getGameINLINE().addPlayer(eNewPlayer, eBestLeader, eBestCiv);
		GET_PLAYER(eNewPlayer).setParent(getID());
		GC.getInitCore().setLeaderName(eNewPlayer, GC.getLeaderHeadInfo(eBestLeader).getTextKeyWide());

		CvTeam& kNewTeam = GET_TEAM(GET_PLAYER(eNewPlayer).getTeam());
		for (int i = 0; i < GC.getNumTechInfos(); ++i)
		{
			if (GET_TEAM(getTeam()).isHasTech((TechTypes)i))
			{
				kNewTeam.setHasTech((TechTypes)i, true, eNewPlayer, false, false);
				if (GET_TEAM(getTeam()).isNoTradeTech((TechTypes)i) || GC.getGameINLINE().isOption(GAMEOPTION_NO_TECH_BROKERING))
				{
					kNewTeam.setNoTradeTech((TechTypes)i, true);
				}
			}
		}

		for (int iTeam = 0; iTeam < MAX_TEAMS; ++iTeam)
		{
			CvTeam& kLoopTeam = GET_TEAM((TeamTypes)iTeam);

			if (kLoopTeam.isAlive())
			{
				kNewTeam.setEspionagePointsAgainstTeam((TeamTypes)iTeam, GET_TEAM(getTeam()).getEspionagePointsAgainstTeam((TeamTypes)iTeam));
				kLoopTeam.setEspionagePointsAgainstTeam(GET_PLAYER(eNewPlayer).getTeam(), kLoopTeam.getEspionagePointsAgainstTeam(getTeam()));
			}
		}
		kNewTeam.setEspionagePointsEver(GET_TEAM(getTeam()).getEspionagePointsEver());

		GET_TEAM(getTeam()).assignVassal(GET_PLAYER(eNewPlayer).getTeam(), false);

		AI_updateBonusValue();
	}

	std::vector< std::pair<int, int> > aCultures;
	for (int iPlot = 0; iPlot < GC.getMapINLINE().numPlotsINLINE(); ++iPlot)
	{
		CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iPlot);

		bool bTranferPlot = false;

		if (!bTranferPlot && pLoopPlot->area() == pArea)
		{
			bTranferPlot = true;
		}

		if (!bTranferPlot)
		{
			CvCity* pWorkingCity = pLoopPlot->getWorkingCity();
			if (NULL != pWorkingCity && pWorkingCity->getOwnerINLINE() == getID() && pWorkingCity->area() == pArea)
			{
				bTranferPlot = true;
			}
		}

		if (!bTranferPlot && pLoopPlot->isWater() && pLoopPlot->isAdjacentToArea(pArea))
		{
			bTranferPlot = true;
		}

		if (bTranferPlot)
		{
			int iCulture = pLoopPlot->getCulture(getID());

			if (bPlayerExists)
			{
				iCulture = std::max(iCulture, pLoopPlot->getCulture(eNewPlayer));
			}

			aCultures.push_back(std::make_pair(iPlot, iCulture));
		}

		if (pLoopPlot->isRevealed(getTeam(), false))
		{
			pLoopPlot->setRevealed(GET_PLAYER(eNewPlayer).getTeam(), true, false, getTeam(), false);
		}
	}

	int iLoop;
	for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		if (pLoopCity->area() == pArea)
		{
			int iCulture = pLoopCity->getCultureTimes100(getID());
			CvPlot* pPlot = pLoopCity->plot();

			GET_PLAYER(eNewPlayer).acquireCity(pLoopCity, false, true, false);

			if (NULL != pPlot)
			{
				CvCity* pCity = pPlot->getPlotCity();
				if (NULL != pCity)
				{
					pCity->setCultureTimes100(eNewPlayer, iCulture, false, false);
				}

//FfH: Added by Kael 01/16/2009 (to prevent CtD's when colony free units are dropped on top of hidden nationaliy units)
                CvUnit* pLoopUnit;
                CLLNode<IDInfo>* pUnitNode;
                pUnitNode = pPlot->headUnitNode();
                while (pUnitNode != NULL)
                {
                    pLoopUnit = ::getUnit(pUnitNode->m_data);
                    pUnitNode = pPlot->nextUnitNode(pUnitNode);
                    if (pLoopUnit->isHiddenNationality())
                    {
                        pLoopUnit->jumpToNearestValidPlot();
                    }
                }
//FfH: End Add

				for (int i = 0; i < GC.getDefineINT("COLONY_NUM_FREE_DEFENDERS"); ++i)
				{
					pCity->initConscriptedUnit();
				}
			}
		}
	}

	for (uint i = 0; i < aCultures.size(); ++i)
	{
		CvPlot* pPlot = GC.getMapINLINE().plotByIndexINLINE(aCultures[i].first);
		pPlot->setCulture(eNewPlayer, aCultures[i].second, true, false);
		pPlot->setCulture(getID(), 0, true, false);

		for (int iTeam = 0; iTeam < MAX_TEAMS; ++iTeam)
		{
			if (pPlot->getRevealedOwner((TeamTypes)iTeam, false) == getID())
			{
				pPlot->setRevealedOwner((TeamTypes)iTeam, eNewPlayer);
			}
		}
	}

	GC.getGameINLINE().updatePlotGroups();

	return true;
}

bool CvPlayer::isValidTriggerReligion(const CvEventTriggerInfo& kTrigger, CvCity* pCity, ReligionTypes eReligion) const
{
	if (kTrigger.getNumReligionsRequired() > 0)
	{
		bool bFound = false;

		for (int i = 0; i < kTrigger.getNumReligionsRequired(); ++i)
		{
			if (eReligion == kTrigger.getReligionRequired(i))
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			return false;
		}
	}

	if (NULL != pCity)
	{
		if (!pCity->isHasReligion(eReligion))
		{
			return false;
		}

		if (kTrigger.isHolyCity())
		{
			if (!pCity->isHolyCity(eReligion))
			{
				return false;
			}
		}
	}
	else
	{
		if (0 == getHasReligionCount(eReligion))
		{
			return false;
		}

		if (kTrigger.isHolyCity())
		{
			CvCity* pHolyCity = GC.getGameINLINE().getHolyCity(eReligion);
			if (NULL == pHolyCity || pHolyCity->getOwnerINLINE() != getID())
			{
				return false;
			}
		}
	}

	return true;
}

bool CvPlayer::isValidTriggerCorporation(const CvEventTriggerInfo& kTrigger, CvCity* pCity, CorporationTypes eCorporation) const
{
	if (kTrigger.getNumCorporationsRequired() > 0)
	{
		bool bFound = false;

		for (int i = 0; i < kTrigger.getNumCorporationsRequired(); ++i)
		{
			if (eCorporation == kTrigger.getCorporationRequired(i))
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			return false;
		}
	}

	if (NULL != pCity)
	{
		if (!pCity->isHasCorporation(eCorporation))
		{
			return false;
		}

		if (kTrigger.isHeadquarters())
		{
			if (!pCity->isHeadquarters(eCorporation))
			{
				return false;
			}
		}
	}
	else
	{
		if (getHasCorporationCount(eCorporation) > 0)
		{
			return true;
		}

		if (kTrigger.isHeadquarters())
		{
			CvCity* pHeadquarters = GC.getGameINLINE().getHeadquarters(eCorporation);
			if (NULL == pHeadquarters || pHeadquarters->getOwnerINLINE() != getID())
			{
				return false;
			}
		}
	}

	return false;
}

void CvPlayer::launch(VictoryTypes eVictory)
{
	CvTeam& kTeam = GET_TEAM(getTeam());

	if (!kTeam.canLaunch(eVictory))
	{
		return;
	}

	kTeam.finalizeProjectArtTypes();
	kTeam.setVictoryCountdown(eVictory, kTeam.getVictoryDelay(eVictory));

	gDLL->getEngineIFace()->AddLaunch(getID());

	kTeam.setCanLaunch(eVictory, false);

	CvCity *capital = getCapitalCity();

	//message
	CvWString szBuffer;
	for(int i = 0; i < MAX_PLAYERS; ++i)
	{
		if (GET_PLAYER((PlayerTypes)i).isAlive())
		{
			int plotX = -1;
			int plotY = -1;
			if((capital != NULL) && capital->isRevealed(GET_PLAYER((PlayerTypes) i).getTeam(), false))
			{
				plotX = capital->getX();
				plotY = capital->getY();
			}

			if (GET_PLAYER((PlayerTypes)i).getTeam() == getTeam())
			{
				szBuffer = gDLL->getText("TXT_KEY_VICTORY_YOU_HAVE_LAUNCHED");
			}
			else
			{
				szBuffer = gDLL->getText("TXT_KEY_VICTORY_TEAM_HAS_LAUNCHED", GET_TEAM(getTeam()).getName().GetCString());
			}

			gDLL->getInterfaceIFace()->addMessage(((PlayerTypes)i), true, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_CULTURELEVEL", MESSAGE_TYPE_MAJOR_EVENT, ARTFILEMGR.getMiscArtInfo("SPACE_SHIP_BUTTON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_HIGHLIGHT_TEXT"), plotX, plotY, true, true);
		}
	}
}

bool CvPlayer::isFreePromotion(UnitCombatTypes eUnitCombat, PromotionTypes ePromotion) const
{
	for (UnitCombatPromotionArray::const_iterator it = m_aFreeUnitCombatPromotions.begin(); it != m_aFreeUnitCombatPromotions.end(); ++it)
	{
		if ((*it).first == eUnitCombat && (*it).second == ePromotion)
		{
			return true;
		}
	}

	return false;
}

void CvPlayer::setFreePromotion(UnitCombatTypes eUnitCombat, PromotionTypes ePromotion, bool bFree)
{
	for (UnitCombatPromotionArray::iterator it = m_aFreeUnitCombatPromotions.begin(); it != m_aFreeUnitCombatPromotions.end(); ++it)
	{
		if ((*it).first == eUnitCombat && (*it).second == ePromotion)
		{
			if (!bFree)
			{
				m_aFreeUnitCombatPromotions.erase(it);
			}
			return;
		}
	}

	if (bFree)
	{
		m_aFreeUnitCombatPromotions.push_back(std::make_pair(eUnitCombat, ePromotion));
	}
}

bool CvPlayer::isFreePromotion(UnitClassTypes eUnitClass, PromotionTypes ePromotion) const
{
	for (UnitClassPromotionArray::const_iterator it = m_aFreeUnitClassPromotions.begin(); it != m_aFreeUnitClassPromotions.end(); ++it)
	{
		if ((*it).first == eUnitClass && (*it).second == ePromotion)
		{
			return true;
		}
	}

	return false;
}

void CvPlayer::setFreePromotion(UnitClassTypes eUnitClass, PromotionTypes ePromotion, bool bFree)
{
	for (UnitClassPromotionArray::iterator it = m_aFreeUnitClassPromotions.begin(); it != m_aFreeUnitClassPromotions.end(); ++it)
	{
		if ((*it).first == eUnitClass && (*it).second == ePromotion)
		{
			if (!bFree)
			{
				m_aFreeUnitClassPromotions.erase(it);
			}
			return;
		}
	}

	if (bFree)
	{
		m_aFreeUnitClassPromotions.push_back(std::make_pair(eUnitClass, ePromotion));
	}
}

PlayerVoteTypes CvPlayer::getVote(int iId) const
{
	for (std::vector< std::pair<int, PlayerVoteTypes> >::const_iterator it = m_aVote.begin(); it != m_aVote.end(); ++it)
	{
		if ((*it).first == iId)
		{
			return ((*it).second);
		}
	}

	return NO_PLAYER_VOTE;
}

void CvPlayer::setVote(int iId, PlayerVoteTypes ePlayerVote)
{
	for (std::vector< std::pair<int, PlayerVoteTypes> >::iterator it = m_aVote.begin(); it != m_aVote.end(); ++it)
	{
		if ((*it).first == iId)
		{
			if (ePlayerVote == NO_PLAYER_VOTE)
			{
				m_aVote.erase(it);
			}
			else
			{
				(*it).second = ePlayerVote;
			}
			return;
		}
	}

	if (ePlayerVote != NO_PLAYER_VOTE)
	{
		m_aVote.push_back(std::make_pair(iId, ePlayerVote));
	}
}

int CvPlayer::getUnitExtraCost(UnitClassTypes eUnitClass) const
{
	for (std::vector< std::pair<UnitClassTypes, int> >::const_iterator it = m_aUnitExtraCosts.begin(); it != m_aUnitExtraCosts.end(); ++it)
	{
		if ((*it).first == eUnitClass)
		{
			return ((*it).second);
		}
	}

	return 0;
}

void CvPlayer::setUnitExtraCost(UnitClassTypes eUnitClass, int iCost)
{
	for (std::vector< std::pair<UnitClassTypes, int> >::iterator it = m_aUnitExtraCosts.begin(); it != m_aUnitExtraCosts.end(); ++it)
	{
		if ((*it).first == eUnitClass)
		{
			if (0 == iCost)
			{
				m_aUnitExtraCosts.erase(it);
			}
			else
			{
				(*it).second = iCost;
			}
			return;
		}
	}

	if (0 != iCost)
	{
		m_aUnitExtraCosts.push_back(std::make_pair(eUnitClass, iCost));
	}
}

// CACHE: cache frequently used values
///////////////////////////////////////
bool CvPlayer::hasShrine(ReligionTypes eReligion)
{
	bool bHasShrine = false;

	if (eReligion != NO_RELIGION)
	{
		CvCity* pHolyCity = GC.getGameINLINE().getHolyCity(eReligion);

		// if the holy city exists, and we own it
		if (pHolyCity != NULL && pHolyCity->getOwnerINLINE() == getID())
			bHasShrine = pHolyCity->hasShrine(eReligion);
	}
	return bHasShrine;
}

void CvPlayer::invalidatePopulationRankCache()
{
	int iLoop;
	CvCity* pLoopCity;
	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->invalidatePopulationRankCache();
	}
}

void CvPlayer::invalidateYieldRankCache(YieldTypes eYield)
{
	int iLoop;
	CvCity* pLoopCity;
	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->invalidateYieldRankCache();
	}
}

void CvPlayer::invalidateCommerceRankCache(CommerceTypes eCommerce)
{
	int iLoop;
	CvCity* pLoopCity;
	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		pLoopCity->invalidateCommerceRankCache();
	}
}


void CvPlayer::doUpdateCacheOnTurn()
{
	// add this back, after testing without it
	// invalidateYieldRankCache();
}

void CvPlayer::processVoteSourceBonus(VoteSourceTypes eVoteSource, bool bActive)
{
	int iLoop;
	for (CvCity* pCity = firstCity(&iLoop); NULL != pCity; pCity = nextCity(&iLoop))
	{
		pCity->processVoteSourceBonus(eVoteSource, bActive);
	}
}

int CvPlayer::getVotes(VoteTypes eVote, VoteSourceTypes eVoteSource) const
{
	int iVotes = 0;

	ReligionTypes eReligion = GC.getGameINLINE().getVoteSourceReligion(eVoteSource);

//FfH: Added by Kael 11/15/2007
    if (GC.getVoteSourceInfo(eVoteSource).getCivic() != NO_CIVIC)
    {
        if (!isCivic((CivicTypes)GC.getVoteSourceInfo(eVoteSource).getCivic()))
        {
            return 0;
        }
    }
//FfH: End Add

	//added Sephi
	return getTotalPopulation();
	//added Sephi

	if (NO_VOTE == eVote)
	{
		if (NO_RELIGION != eReligion)
		{
			iVotes = getReligionPopulation(eReligion);
		}
		else
		{
			iVotes = getTotalPopulation();
		}
	}
	else
	{
		if (!GC.getVoteInfo(eVote).isVoteSourceType(eVoteSource))
		{
			return 0;
		}

		if (GC.getVoteInfo(eVote).isCivVoting())
		{
			if (NO_RELIGION == eReligion || getHasReligionCount(eReligion) > 0)
			{
				iVotes = 1;
			}
		}
		else if (GC.getVoteInfo(eVote).isCityVoting())
		{
			if (NO_RELIGION != eReligion)
			{
				iVotes = getHasReligionCount(eReligion);
			}
			else
			{
				iVotes = getNumCities();
			}
		}
		else
		{
			if (NO_RELIGION == eReligion)
			{
				iVotes = getTotalPopulation();
			}
			else
			{
				iVotes = getReligionPopulation(eReligion);
			}
		}

		if (NO_RELIGION != eReligion && getStateReligion() == eReligion)
		{
			iVotes *= (100 + GC.getVoteInfo(eVote).getStateReligionVotePercent());
			iVotes /= 100;
		}
	}

//FfH: Added by Kael 11/14/2007
	CvUnit* pLoopUnit;
	int iLoop;
	for(pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{
		if (GC.getUnitInfo(pLoopUnit->getUnitType()).getDiploVoteType() != NO_VOTESOURCE)
		{
		    iVotes += 1;
		}
	}
//FfH: End Add

	return iVotes;
}

bool CvPlayer::canDoResolution(VoteSourceTypes eVoteSource, const VoteSelectionSubData& kData) const
{
	CvTeam& kOurTeam = GET_TEAM(getTeam());

	if (NO_PLAYER != kData.ePlayer)
	{
		if (!kOurTeam.isHasMet(GET_PLAYER(kData.ePlayer).getTeam()))
		{
			return false;
		}
	}

	if (GC.getVoteInfo(kData.eVote).isOpenBorders())
	{
		for (int iTeam2 = 0; iTeam2 < MAX_CIV_TEAMS; ++iTeam2)
		{
			if (GET_TEAM((TeamTypes)iTeam2).isVotingMember(eVoteSource))
			{
				if (!kOurTeam.isOpenBordersTrading() && !GET_TEAM((TeamTypes)iTeam2).isOpenBordersTrading())
				{
					return false;
				}

				if (kOurTeam.isAtWar((TeamTypes)iTeam2))
				{
					return false;
				}
			}
		}
	}
	else if (GC.getVoteInfo(kData.eVote).isDefensivePact())
	{
		for (int iTeam2 = 0; iTeam2 < MAX_CIV_TEAMS; ++iTeam2)
		{
			if (GET_TEAM((TeamTypes)iTeam2).isVotingMember(eVoteSource))
			{
				if (!kOurTeam.isDefensivePactTrading() && !GET_TEAM((TeamTypes)iTeam2).isDefensivePactTrading())
				{
					return false;
				}

				if (kOurTeam.getAtWarCount(true) > 0 || GET_TEAM((TeamTypes)iTeam2).getAtWarCount(true) > 0)
				{
					return false;
				}

				if (!kOurTeam.canSignDefensivePact((TeamTypes)iTeam2))
				{
					return false;
				}
			}
		}
	}
	else if (GC.getVoteInfo(kData.eVote).isForcePeace())
	{
		FAssert(NO_PLAYER != kData.ePlayer);
		CvPlayer& kPlayer = GET_PLAYER(kData.ePlayer);

		if (kPlayer.getTeam() != getTeam())
		{
			if (kOurTeam.isAtWar(kPlayer.getTeam()))
			{
				TeamTypes eMaster = getTeam();
				for (int iMaster = 0; iMaster < MAX_CIV_TEAMS; ++iMaster)
				{
					if (iMaster != getID() && kOurTeam.isVassal((TeamTypes)iMaster))
					{
						if (GET_TEAM((TeamTypes)iMaster).isVotingMember(eVoteSource))
						{
							eMaster = (TeamTypes)iMaster;
							break;
						}
					}
				}

				if (!GET_TEAM(eMaster).canContact(kPlayer.getTeam()))
				{
					return false;
				}
			}
		}
	}
	else if (GC.getVoteInfo(kData.eVote).isForceWar())
	{
		FAssert(NO_PLAYER != kData.ePlayer);
		CvPlayer& kPlayer = GET_PLAYER(kData.ePlayer);

		if (!kOurTeam.isAtWar(kPlayer.getTeam()))
		{
			TeamTypes eMaster = getTeam();
			for (int iMaster = 0; iMaster < MAX_CIV_TEAMS; ++iMaster)
			{
				if (iMaster != getID() && kOurTeam.isVassal((TeamTypes)iMaster))
				{
					if (GET_TEAM((TeamTypes)iMaster).isVotingMember(eVoteSource))
					{
						eMaster = (TeamTypes)iMaster;
						break;
					}
				}
			}

			if (!GET_TEAM(eMaster).canDeclareWar(kPlayer.getTeam()))
			{
				return false;
			}
		}
	}
	else if (GC.getVoteInfo(kData.eVote).isForceNoTrade())
	{
		FAssert(NO_PLAYER != kData.ePlayer);
		CvPlayer& kPlayer = GET_PLAYER(kData.ePlayer);

		if (!canStopTradingWithTeam(kPlayer.getTeam(), true))
		{
			return false;
		}

	}
	else if (GC.getVoteInfo(kData.eVote).isAssignCity())
	{
		if (GET_TEAM(GET_PLAYER(kData.eOtherPlayer).getTeam()).isVassal(GET_PLAYER(kData.ePlayer).getTeam()))
		{
			return false;
		}
	}

	return true;
}

bool CvPlayer::canDefyResolution(VoteSourceTypes eVoteSource, const VoteSelectionSubData& kData) const
{
	if (GC.getGameINLINE().getSecretaryGeneral(eVoteSource) == getTeam())
	{
		return false;
	}

	if (GC.getVoteInfo(kData.eVote).isOpenBorders())
	{
		for (int iTeam = 0; iTeam < MAX_CIV_TEAMS; ++iTeam)
		{
			CvTeam& kTeam = GET_TEAM((TeamTypes)iTeam);
			if ((PlayerTypes)iTeam != getTeam())
			{
				if (kTeam.isVotingMember(eVoteSource))
				{
					if (!kTeam.isOpenBorders(getTeam()))
					{
						return true;
					}
				}
			}
		}
	}
	else if (GC.getVoteInfo(kData.eVote).isDefensivePact())
	{
		for (int iTeam = 0; iTeam < MAX_CIV_TEAMS; ++iTeam)
		{
			CvTeam& kTeam = GET_TEAM((TeamTypes)iTeam);
			if ((PlayerTypes)iTeam != getTeam())
			{
				if (kTeam.isVotingMember(eVoteSource))
				{
					if (!kTeam.isDefensivePact(getTeam()))
					{
						return true;
					}
				}
			}
		}
	}
	else if (GC.getVoteInfo(kData.eVote).isForceNoTrade())
	{
		return true;
	}
	else if (GC.getVoteInfo(kData.eVote).isForceWar())
	{
		if (!::atWar(getTeam(), GET_PLAYER(kData.ePlayer).getTeam()))
		{
			return true;
		}
	}
	else if (GC.getVoteInfo(kData.eVote).isForcePeace())
	{
		if (GET_PLAYER(kData.ePlayer).getTeam() == getTeam())
		{
			return true;
		}

		if (::atWar(getTeam(), GET_PLAYER(kData.ePlayer).getTeam()))
		{
			return true;
		}
	}
	else if (GC.getVoteInfo(kData.eVote).isAssignCity())
	{
		if (kData.ePlayer == getID())
		{
			return true;
		}
	}
	else if (!GC.getGameINLINE().isTeamVote(kData.eVote))
	{
		return true;
	}

	return false;
}


void CvPlayer::setDefiedResolution(VoteSourceTypes eVoteSource, const VoteSelectionSubData& kData)
{
	FAssert(canDefyResolution(eVoteSource, kData));

	// cities get unhappiness
	int iLoop;
	for (CvCity* pLoopCity = firstCity(&iLoop); NULL != pLoopCity; pLoopCity = nextCity(&iLoop))
	{
		ReligionTypes eReligion = GC.getGameINLINE().getVoteSourceReligion(eVoteSource);

		if (NO_RELIGION == eReligion || pLoopCity->isHasReligion(eReligion))
		{
			int iAngerLength = pLoopCity->flatDefyResolutionAngerLength();
			if (NO_RELIGION != eReligion && pLoopCity->isHasReligion(eReligion))
			{
				iAngerLength /= std::max(1, pLoopCity->getReligionCount());
			}

			pLoopCity->changeDefyResolutionAngerTimer(iAngerLength);
		}
	}

	setLoyalMember(eVoteSource, false);
}


void CvPlayer::setEndorsedResolution(VoteSourceTypes eVoteSource, const VoteSelectionSubData& kData)
{
	setLoyalMember(eVoteSource, true);
}

bool CvPlayer::isFullMember(VoteSourceTypes eVoteSource) const
{
	if (NO_RELIGION != GC.getGameINLINE().getVoteSourceReligion(eVoteSource))
	{
		if (getStateReligion() != GC.getGameINLINE().getVoteSourceReligion(eVoteSource))
		{
			return false;
		}
	}

	if (NO_CIVIC != GC.getVoteSourceInfo(eVoteSource).getCivic())
	{
		if (!isCivic((CivicTypes)GC.getVoteSourceInfo(eVoteSource).getCivic()))
		{
			return false;
		}
	}

	if (!isLoyalMember(eVoteSource))
	{
		return false;
	}

	return isVotingMember(eVoteSource);
}

bool CvPlayer::isVotingMember(VoteSourceTypes eVoteSource) const
{

//FfH: Added by Kael 11/15/2007
	if (GC.getVoteSourceInfo(eVoteSource).getCivic() != NO_CIVIC)
	{
		if (!isCivic((CivicTypes)GC.getVoteSourceInfo(eVoteSource).getCivic()))
		{
			return false;
		}
	}
//FfH: End Add

	return (getVotes(NO_VOTE, eVoteSource) > 0);
}

PlayerTypes CvPlayer::pickConqueredCityOwner(const CvCity& kCity) const
{
	PlayerTypes eBestPlayer = kCity.getLiberationPlayer(true);

	if (NO_PLAYER != eBestPlayer)
	{
		if (GET_TEAM(getTeam()).isVassal(GET_PLAYER(eBestPlayer).getTeam()))
		{
			return eBestPlayer;
		}
	}

	return getID();
}

bool CvPlayer::canHaveTradeRoutesWith(PlayerTypes ePlayer) const
{
	CvPlayer& kOtherPlayer = GET_PLAYER(ePlayer);

	if (!kOtherPlayer.isAlive())
	{
		return false;
	}

	if (getTeam() == kOtherPlayer.getTeam())
	{
		return true;
	}

	if (GET_TEAM(getTeam()).isFreeTrade(kOtherPlayer.getTeam()))
	{
		if (GET_TEAM(getTeam()).isVassal(kOtherPlayer.getTeam()))
		{
			return true;
		}

		if (GET_TEAM(kOtherPlayer.getTeam()).isVassal(getTeam()))
		{
			return true;
		}

		if (!isNoForeignTrade() && !kOtherPlayer.isNoForeignTrade())
		{
			return true;
		}
	}

	return false;
}

bool CvPlayer::canStealTech(PlayerTypes eTarget, TechTypes eTech) const
{
	if (GET_TEAM(GET_PLAYER(eTarget).getTeam()).isHasTech(eTech))
	{
		if (canResearch(eTech))
		{
			return true;
		}
	}

	return false;
}

bool CvPlayer::canForceCivics(PlayerTypes eTarget, CivicTypes eCivic) const
{
	return (GET_PLAYER(eTarget).canDoCivics(eCivic) && !GET_PLAYER(eTarget).isCivic(eCivic) && isCivic(eCivic));
}

bool CvPlayer::canForceReligion(PlayerTypes eTarget, ReligionTypes eReligion) const
{
	return (GET_PLAYER(eTarget).canDoReligion(eReligion) && GET_PLAYER(eTarget).getStateReligion() != eReligion && getStateReligion() == eReligion);
}

bool CvPlayer::canSpyDestroyUnit(PlayerTypes eTarget, CvUnit& kUnit) const
{
	if (kUnit.getTeam() == getTeam())
	{
		return false;
	}

	if (kUnit.getUnitInfo().getProductionCost() <= 0)
	{
		return false;
	}

	if (!kUnit.plot()->isVisible(getTeam(), false))
	{
		return false;
	}

	return true;
}

bool CvPlayer::canSpyBribeUnit(PlayerTypes eTarget, CvUnit& kUnit) const
{
	if (!canSpyDestroyUnit(eTarget, kUnit))
	{
		return false;
	}

	// Can't buy units when at war
	if (kUnit.isEnemy(getTeam()))
	{
		return false;
	}

	// Can't buy units if they are not in a legal plot
	if (!GET_TEAM(getTeam()).isFriendlyTerritory(GET_PLAYER(eTarget).getTeam()) && !GET_TEAM(getTeam()).isOpenBorders(GET_PLAYER(eTarget).getTeam()))
	{
		return false;
	}

	CLLNode<IDInfo>* pUnitNode = kUnit.plot()->headUnitNode();

	while (pUnitNode != NULL)
	{
		CvUnit* pLoopUnit = ::getUnit(pUnitNode->m_data);
		pUnitNode = kUnit.plot()->nextUnitNode(pUnitNode);

		if (NULL != pLoopUnit && pLoopUnit != &kUnit)
		{
			if (pLoopUnit->isEnemy(getTeam()))
			{
				// If we buy the unit, we will be on the same plot as an enemy unit! Not good.
				return false;
			}
		}
	}

	return true;
}

bool CvPlayer::canSpyDestroyBuilding(PlayerTypes eTarget, BuildingTypes eBuilding) const
{
	CvBuildingInfo& kBuilding = GC.getBuildingInfo(eBuilding);
	if (kBuilding.getProductionCost() <= 0)
	{
		return false;
	}

	if (::isLimitedWonderClass((BuildingClassTypes)kBuilding.getBuildingClassType()))
	{
		return false;
	}

	return true;
}

bool CvPlayer::canSpyDestroyProject(PlayerTypes eTarget, ProjectTypes eProject) const
{
	CvProjectInfo& kProject = GC.getProjectInfo(eProject);
	if (kProject.getProductionCost() <= 0)
	{
		return false;
	}

	if (GET_TEAM(GET_PLAYER(eTarget).getTeam()).getProjectCount(eProject) <= 0)
	{
		return false;
	}

	if (::isWorldProject(eProject))
	{
		return false;
	}

	if (!kProject.isSpaceship())
	{
		return false;
	}
	else
	{
		VictoryTypes eVicotry = (VictoryTypes)kProject.getVictoryPrereq();
		if (NO_VICTORY != eVicotry)
		{
			// Can't destroy spaceship components if we have already launched
			if (GET_TEAM(GET_PLAYER(eTarget).getTeam()).getVictoryCountdown(eVicotry) >= 0)
			{
				return false;
			}
		}
	}

	return true;
}

void CvPlayer::forcePeace(PlayerTypes ePlayer)
{
	if (!GET_TEAM(getTeam()).isAVassal())
	{
		FAssert(GET_TEAM(getTeam()).canChangeWarPeace(GET_PLAYER(ePlayer).getTeam()));

		CLinkList<TradeData> playerList;
		CLinkList<TradeData> loopPlayerList;
		TradeData kTradeData;
		setTradeItem(&kTradeData, TRADE_PEACE_TREATY);
		playerList.insertAtEnd(kTradeData);
		loopPlayerList.insertAtEnd(kTradeData);

		GC.getGameINLINE().implementDeal(getID(), ePlayer, &playerList, &loopPlayerList);
	}
}

bool CvPlayer::canSpiesEnterBorders(PlayerTypes ePlayer) const
{
	for (int iMission = 0; iMission < GC.getNumEspionageMissionInfos(); ++iMission)
	{
		if (GC.getEspionageMissionInfo((EspionageMissionTypes)iMission).isNoActiveMissions() && GC.getEspionageMissionInfo((EspionageMissionTypes)iMission).isPassive())
		{
			if (GET_PLAYER(ePlayer).canDoEspionageMission((EspionageMissionTypes)iMission, getID(), NULL, -1, NULL))
			{
				return false;
			}
		}
	}

	return true;
}

int CvPlayer::getReligionPopulation(ReligionTypes eReligion) const
{
	int iPopulation = 0;
	int iLoop;
	for (CvCity* pCity = firstCity(&iLoop); NULL != pCity; pCity = nextCity(&iLoop))
	{
		if (pCity->isHasReligion(eReligion))
		{
			iPopulation += pCity->getPopulation();
		}
	}

	return iPopulation;
}

int CvPlayer::getNewCityProductionValue() const
{
	int iValue = 0;
	for (int iJ = 0; iJ < GC.getNumBuildingClassInfos(); iJ++)
	{
		BuildingTypes eBuilding = ((BuildingTypes)(GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(iJ)));

		if (NO_BUILDING != eBuilding)
		{
			if (GC.getBuildingInfo(eBuilding).getFreeStartEra() != NO_ERA)
			{
				if (GC.getGameINLINE().getStartEra() >= GC.getBuildingInfo(eBuilding).getFreeStartEra())
				{
					iValue += (100 * getProductionNeeded(eBuilding)) / std::max(1, 100 + getProductionModifier(eBuilding));
				}
			}
		}
	}

	iValue *= 100 + GC.getDefineINT("NEW_CITY_BUILDING_VALUE_MODIFIER");
	iValue /= 100;

	iValue += (GC.getDefineINT("ADVANCED_START_CITY_COST") * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getGrowthPercent()) / 100;

	int iPopulation = GC.getDefineINT("INITIAL_CITY_POPULATION") + GC.getEraInfo(GC.getGameINLINE().getStartEra()).getFreePopulation();
	for (int i = 1; i <= iPopulation; ++i)
	{
		iValue += (getGrowthThreshold(i) * GC.getDefineINT("ADVANCED_START_POPULATION_COST")) / 100;
	}

	return iValue;
}

int CvPlayer::getGrowthThreshold(int iPopulation) const
{
	int iThreshold;

	iThreshold = (GC.getDefineINT("BASE_CITY_GROWTH_THRESHOLD") + (iPopulation * GC.getDefineINT("CITY_GROWTH_MULTIPLIER")));

/** added Sephi **/
	for(int iI=1;iI<=iPopulation;iI++)
	{
		iThreshold+=iI;
	}
/**/
	iThreshold *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getGrowthPercent();
	iThreshold /= 100;

	iThreshold *= GC.getEraInfo(GC.getGameINLINE().getStartEra()).getGrowthPercent();
	iThreshold /= 100;

	for(int iI=0;iI<GC.getNumProjectInfos();iI++)
	{
		if(GC.getProjectInfo((ProjectTypes)iI).isBirth())
		{
			if(GET_TEAM(getTeam()).getProjectCount((ProjectTypes)iI)>0)
			{
				BonusTypes eBonus=GC.getProjectInfo((ProjectTypes)iI).getManaBonusLinked();
				if(eBonus!=NO_BONUS)
				{
					iThreshold *= 100-std::min(getNumAvailableBonuses(eBonus)*10,50);
					iThreshold /= 100;
				}
			}
		}
	}

	if (!isHuman() && !isBarbarian())
	{
		iThreshold *= GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIGrowthPercent();
		iThreshold /= 100;

		iThreshold *= std::max(0, ((GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIPerEraModifier() * getCurrentEra()) + 100));
		iThreshold /= 100;
	}

	return std::max(1, iThreshold);
}

void CvPlayer::verifyUnitStacksValid()
{
	int iLoop;
	for(CvUnit* pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{
		pLoopUnit->verifyStackValid();
	}
}

UnitTypes CvPlayer::getTechFreeUnit(TechTypes eTech) const
{
	UnitClassTypes eUnitClass = (UnitClassTypes) GC.getTechInfo(eTech).getFirstFreeUnitClass();
	if (eUnitClass == NO_UNITCLASS)
	{
		return NO_UNIT;
	}

	UnitTypes eUnit = ((UnitTypes)(GC.getCivilizationInfo(getCivilizationType()).getCivilizationUnits(eUnitClass)));
	if (eUnit == NO_UNIT)
	{
		return NO_UNIT;
	}

	if (GC.getUnitInfo(eUnit).getEspionagePoints() > 0 && GC.getGameINLINE().isOption(GAMEOPTION_NO_ESPIONAGE))
	{
		return NO_UNIT;
	}

	return eUnit;
}


void CvPlayer::buildTradeTable(PlayerTypes eOtherPlayer, CLinkList<TradeData>& ourList) const
{
	TradeData item;
	int iLoop;

	//	Put the gold and maps into the table
	setTradeItem(&item, TRADE_GOLD);
	if (canTradeItem(eOtherPlayer, item))
	{
		ourList.insertAtEnd(item);
	}

	//	Gold per turn
	setTradeItem(&item, TRADE_GOLD_PER_TURN);
	if (canTradeItem(eOtherPlayer, item))
	{
		ourList.insertAtEnd(item);
	}

	//	Maps
	setTradeItem(&item, TRADE_MAPS, 0);
	if (canTradeItem(eOtherPlayer, item))
	{
		ourList.insertAtEnd(item);
	}

	//	Vassal
	setTradeItem(&item, TRADE_VASSAL, 0);
	if (canTradeItem(eOtherPlayer, item))
	{
		ourList.insertAtEnd(item);
	}

	//	Open Borders
	setTradeItem(&item, TRADE_OPEN_BORDERS);
	if (canTradeItem(eOtherPlayer, item))
	{
		ourList.insertAtEnd(item);
	}

	//	Defensive Pact
	setTradeItem(&item, TRADE_DEFENSIVE_PACT);
	if (canTradeItem(eOtherPlayer, item))
	{
		ourList.insertAtEnd(item);
	}

	//	Permanent Alliance
	setTradeItem(&item, TRADE_PERMANENT_ALLIANCE);
	if (canTradeItem(eOtherPlayer, item))
	{
		ourList.insertAtEnd(item);
	}

	if (::atWar(getTeam(), GET_PLAYER(eOtherPlayer).getTeam()))
	{
		//	We are at war, allow a peace treaty option
		setTradeItem(&item, TRADE_PEACE_TREATY);
		ourList.insertAtEnd(item);

		//	Capitulation
		setTradeItem(&item, TRADE_SURRENDER, 0);
		if (canTradeItem(eOtherPlayer, item))
		{
			ourList.insertAtEnd(item);
		}
	}

	//	Initial build of the inventory lists and buttons.
	//	Go through all the possible headings
	for (int i = NUM_BASIC_ITEMS; i < NUM_TRADEABLE_HEADINGS; i++)
	{
		bool bFoundItemUs = false;

		//	Build what we need to build for this item
		switch (i)
		{
		case TRADE_TECHNOLOGIES:
			for (int j = 0; j < GC.getNumTechInfos(); j++)
			{
				setTradeItem(&item, TRADE_TECHNOLOGIES, j);
				if (canTradeItem(eOtherPlayer, item))
				{
					bFoundItemUs = true;
					ourList.insertAtEnd(item);
				}
			}
			break;

		case TRADE_RESOURCES:
			for (int j = 0; j < GC.getNumBonusInfos(); j++)
			{
				setTradeItem(&item, TRADE_RESOURCES, j);
				if (canTradeItem(eOtherPlayer, item))
				{
					bFoundItemUs = true;
					ourList.insertAtEnd(item);
				}
			}
			break;

		case TRADE_CITIES:
			for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
			{
				setTradeItem(&item, TRADE_CITIES, pLoopCity->getID());
				if (canTradeItem(eOtherPlayer, item))
				{
					bFoundItemUs = true;
					ourList.insertAtEnd(item);
				}
			}
			break;

		case TRADE_PEACE:
			if (!isHuman())
			{
				for (int j = 0; j < MAX_CIV_TEAMS; j++)
				{
					if (GET_TEAM((TeamTypes)j).isAlive())
					{
						if (j != getTeam() && j != GET_PLAYER(eOtherPlayer).getTeam())
						{
							setTradeItem(&item, TRADE_PEACE, j);
							if (canTradeItem(eOtherPlayer, item))
							{
								ourList.insertAtEnd(item);
								bFoundItemUs = true;
							}
						}
					}
				}
			}
			break;

		case TRADE_WAR:
			if (!isHuman())
			{
				for (int j = 0; j < MAX_CIV_TEAMS; j++)
				{
					if (GET_TEAM((TeamTypes)j).isAlive())
					{
						if (j != getTeam() && j != GET_PLAYER(eOtherPlayer).getTeam())
						{
							setTradeItem(&item, TRADE_WAR, j);
							if (canTradeItem(eOtherPlayer, item))
							{
								ourList.insertAtEnd(item);
								bFoundItemUs = true;
							}
						}
					}
				}
			}
			break;

		case TRADE_EMBARGO:
			if (!isHuman())
			{
				for (int j = 0; j < MAX_CIV_TEAMS; j++)
				{
					if (GET_TEAM((TeamTypes)j).isAlive())
					{
						if (j != getTeam() && j != GET_PLAYER(eOtherPlayer).getTeam())
						{
							setTradeItem(&item, TRADE_EMBARGO, j);
							if (canTradeItem(eOtherPlayer, item))
							{
								ourList.insertAtEnd(item);
								bFoundItemUs = true;
							}
						}
					}
				}
			}
			break;

		case TRADE_CIVIC:
			for (int j = 0; j < GC.getNumCivicInfos(); j++)
			{
				setTradeItem(&item, TRADE_CIVIC, j);
				if (canTradeItem(eOtherPlayer, item))
				{
					bFoundItemUs = true;
					ourList.insertAtEnd(item);
				}
			}
			break;

		case TRADE_RELIGION:
			for (int j = 0; j < GC.getNumReligionInfos(); j++)
			{
				setTradeItem(&item, TRADE_RELIGION, j);
				if (canTradeItem(eOtherPlayer, item))
				{
					bFoundItemUs = true;
					ourList.insertAtEnd(item);
				}
			}
			break;
		}
	}
}

bool CvPlayer::getHeadingTradeString(PlayerTypes eOtherPlayer, TradeableItems eItem, CvWString& szString, CvString& szIcon) const
{
	szIcon.clear();

	switch ( eItem )
	{
	case TRADE_TECHNOLOGIES:
		szString = gDLL->getText("TXT_KEY_CONCEPT_TECHNOLOGY");
		break;

	case TRADE_RESOURCES:
		szString = gDLL->getText("TXT_KEY_TRADE_RESOURCES");
		break;

	case TRADE_CITIES:
		szString = gDLL->getText("TXT_KEY_TRADE_CITIES");
		break;

	case TRADE_PEACE:
		szString = gDLL->getText("TXT_KEY_TRADE_MAKE_PEACE_WITH");
		break;

	case TRADE_WAR:
		szString = gDLL->getText("TXT_KEY_TRADE_DECLARE_WAR_ON");
		break;

	case TRADE_EMBARGO:
		szString = gDLL->getText("TXT_KEY_TRADE_STOP_TRADING_WITH");
		break;

	case TRADE_CIVIC:
		szString = gDLL->getText("TXT_KEY_TRADE_ADOPT");
		break;

	case TRADE_RELIGION:
		szString = gDLL->getText("TXT_KEY_TRADE_CONVERT");
		break;
	default:
		szString.clear();
		return false;
		break;
	}

	return true;
}


bool CvPlayer::getItemTradeString(PlayerTypes eOtherPlayer, bool bOffer, bool bShowingCurrent, const TradeData& zTradeData, CvWString& szString, CvString& szIcon) const
{
	szIcon.clear();

	switch (zTradeData.m_eItemType)
	{
	case TRADE_GOLD:
		if (bOffer)
		{
			szString = gDLL->getText("TXT_KEY_TRADE_GOLD_NUM", zTradeData.m_iData);
		}
		else
		{
			szString = gDLL->getText("TXT_KEY_TRADE_GOLD_NUM", AI_maxGoldTrade(eOtherPlayer));
		}
		break;
	case TRADE_GOLD_PER_TURN:
		if (bOffer)
		{
			szString = gDLL->getText("TXT_KEY_TRADE_GOLD_PER_TURN_NUM", zTradeData.m_iData);
		}
		else
		{
			szString = gDLL->getText("TXT_KEY_TRADE_GOLD_PER_TURN_NUM", AI_maxGoldPerTurnTrade(eOtherPlayer));
		}
		break;
	case TRADE_MAPS:
		szString = gDLL->getText("TXT_KEY_TRADE_WORLD_MAP_STRING");
		break;
	case TRADE_VASSAL:
		szString = gDLL->getText("TXT_KEY_TRADE_VASSAL_STRING");
		break;
	case TRADE_SURRENDER:
		szString = gDLL->getText("TXT_KEY_TRADE_CAPITULATE_STRING");
		break;
	case TRADE_OPEN_BORDERS:
		szString = gDLL->getText("TXT_KEY_TRADE_OPEN_BORDERS_STRING");
		break;
	case TRADE_DEFENSIVE_PACT:
		szString = gDLL->getText("TXT_KEY_TRADE_DEFENSIVE_PACT_STRING");
		break;
	case TRADE_PERMANENT_ALLIANCE:
		szString = gDLL->getText("TXT_KEY_TRADE_PERMANENT_ALLIANCE_STRING");
		break;
	case TRADE_PEACE_TREATY:
		szString = gDLL->getText("TXT_KEY_TRADE_PEACE_TREATY_STRING", GC.getDefineINT("PEACE_TREATY_LENGTH"));
		break;
	case TRADE_TECHNOLOGIES:
		szString = GC.getTechInfo((TechTypes)zTradeData.m_iData).getDescription();
		szIcon = GC.getTechInfo((TechTypes)zTradeData.m_iData).getButton();
		break;
	case TRADE_RESOURCES:
		if (bOffer)
		{
			int iNumResources = GET_PLAYER(eOtherPlayer).getNumTradeableBonuses((BonusTypes)zTradeData.m_iData);
			if (bShowingCurrent)
			{
				++iNumResources;
			}
			szString = gDLL->getText("TXT_KEY_TRADE_RESOURCE", GC.getBonusInfo((BonusTypes)zTradeData.m_iData).getDescription(), iNumResources);

		}
		else
		{
			szString.Format( L"%s (%d)", GC.getBonusInfo((BonusTypes)zTradeData.m_iData).getDescription(), getNumTradeableBonuses((BonusTypes)zTradeData.m_iData));
		}
		szIcon = GC.getBonusInfo((BonusTypes)zTradeData.m_iData).getButton();
		break;
	case TRADE_CITIES:
		{
			CvCity* pCity = NULL;
			if (bOffer)
			{
				pCity = GET_PLAYER(eOtherPlayer).getCity(zTradeData.m_iData);
			}
			else
			{
				pCity = getCity(zTradeData.m_iData);
			}
			if (NULL != pCity)
			{
				if (pCity->getLiberationPlayer(false) == eOtherPlayer)
				{
					szString.Format(L"%s (%s)", pCity->getName().GetCString(), gDLL->getText("TXT_KEY_LIBERATE_CITY").GetCString());
				}
				else
				{
					szString = gDLL->getText("TXT_KEY_CITY_OF", pCity->getNameKey());
				}
			}
		}
		break;
	case TRADE_PEACE:
		if (bOffer)
		{
			szString = gDLL->getText("TXT_KEY_TRADE_PEACE_WITH");
			szString += GET_TEAM((TeamTypes)zTradeData.m_iData).getName();
		}
		else
		{
			szString = GET_TEAM((TeamTypes)zTradeData.m_iData).getName();
		}
		break;
	case TRADE_WAR:
		if (bOffer)
		{
			szString = gDLL->getText("TXT_KEY_TRADE_WAR_WITH");
			szString += GET_TEAM((TeamTypes)zTradeData.m_iData).getName();
		}
		else
		{
			szString = GET_TEAM((TeamTypes)zTradeData.m_iData).getName();
		}
		break;
	case TRADE_EMBARGO:
		if (bOffer)
		{
			szString = gDLL->getText("TXT_KEY_TRADE_STOP_TRADING_WITH");
			szString += L" " + GET_TEAM((TeamTypes)zTradeData.m_iData).getName();
		}
		else
		{
			szString = GET_TEAM((TeamTypes)zTradeData.m_iData).getName();
		}
		break;
	case TRADE_CIVIC:
		if (bOffer)
		{
			szString = gDLL->getText("TXT_KEY_TRADE_ADOPT");
			szString += GC.getCivicInfo((CivicTypes)zTradeData.m_iData).getDescription();
		}
		else
		{
			szString = GC.getCivicInfo((CivicTypes)zTradeData.m_iData).getDescription();
		}
		szIcon = GC.getCivicInfo((CivicTypes)zTradeData.m_iData).getButton();
		break;
	case TRADE_RELIGION:
		if (bOffer)
		{
			szString = gDLL->getText("TXT_KEY_TRADE_CONVERT");
			szString += GC.getReligionInfo((ReligionTypes)zTradeData.m_iData).getDescription();
		}
		else
		{
			szString = GC.getReligionInfo((ReligionTypes)zTradeData.m_iData).getDescription();
		}
		szIcon = GC.getReligionInfo((ReligionTypes)zTradeData.m_iData).getButton();
		break;
	default:
		szString.clear();
		return false;
	}

	return true;
}

void CvPlayer::updateTradeList(PlayerTypes eOtherPlayer, CLinkList<TradeData>& ourInventory, const CLinkList<TradeData>& ourOffer, const CLinkList<TradeData>& theirOffer) const
{
	for (CLLNode<TradeData>* pNode = ourInventory.head(); pNode != NULL; pNode = ourInventory.next(pNode))
	{
		pNode->m_data.m_bHidden = false;

		// Don't show peace treaties when not at war
		if (!::atWar(getTeam(), GET_PLAYER(eOtherPlayer).getTeam()))
		{
			if (pNode->m_data.m_eItemType == TRADE_PEACE_TREATY || pNode->m_data.m_eItemType == TRADE_SURRENDER)
			{
				pNode->m_data.m_bHidden = true;
			}
		}

		// Don't show technologies with no tech trading game option
		if (GC.getGame().isOption(GAMEOPTION_NO_TECH_TRADING) && pNode->m_data.m_eItemType == TRADE_TECHNOLOGIES)
		{
			pNode->m_data.m_bHidden = true;
		}
	}

	for (CLLNode<TradeData>* pNode = ourInventory.head(); pNode != NULL; pNode = ourInventory.next(pNode))
	{
		switch (pNode->m_data.m_eItemType)
		{
		case TRADE_PEACE_TREATY:
			for (CLLNode<TradeData>* pOfferNode = ourOffer.head(); pOfferNode != NULL; pOfferNode = ourOffer.next(pOfferNode))
			{
				// Don't show vassal deals if peace treaty is already on the table
				if (CvDeal::isVassal(pOfferNode->m_data.m_eItemType))
				{
					pNode->m_data.m_bHidden = true;
					break;
				}
			}
			break;
		case TRADE_VASSAL:
		case TRADE_SURRENDER:
			for (CLLNode<TradeData>* pOfferNode = theirOffer.head(); pOfferNode != NULL; pOfferNode = theirOffer.next(pOfferNode))
			{
				// Don't show vassal deals if another type of vassal deal is on the table
				if (CvDeal::isVassal(pOfferNode->m_data.m_eItemType))
				{
					pNode->m_data.m_bHidden = true;
					break;
				}
			}

			if (!pNode->m_data.m_bHidden)
			{
				for (CLLNode<TradeData>* pOfferNode = ourOffer.head(); pOfferNode != NULL; pOfferNode = ourOffer.next(pOfferNode))
				{
					// Don't show peace deals if the other player is offering to be a vassal
					if (CvDeal::isEndWar(pOfferNode->m_data.m_eItemType))
					{
						pNode->m_data.m_bHidden = true;
						break;
					}
				}
			}
			break;
		default:
			break;
		}
	}

	if (!isHuman() || !GET_PLAYER(eOtherPlayer).isHuman())  // everything allowed in human-human trades
	{
		CLLNode<TradeData>* pFirstOffer = ourOffer.head();
		if (pFirstOffer == NULL)
		{
			pFirstOffer = theirOffer.head();
		}

		if (pFirstOffer != NULL)
		{
			if (!CvDeal::isEndWar(pFirstOffer->m_data.m_eItemType) || !::atWar(getTeam(), GET_PLAYER(eOtherPlayer).getTeam()))
			{
				for (CLLNode<TradeData>* pNode = ourInventory.head(); pNode != NULL; pNode = ourInventory.next(pNode))
				{
					if (pFirstOffer->m_data.m_eItemType == TRADE_CITIES || pNode->m_data.m_eItemType == TRADE_CITIES)
					{
						pNode->m_data.m_bHidden = true;
					}
					else if (CvDeal::isAnnual(pFirstOffer->m_data.m_eItemType) != CvDeal::isAnnual(pNode->m_data.m_eItemType))
					{
						pNode->m_data.m_bHidden = true;
					}
				}
			}
		}
	}
}

int CvPlayer::getIntroMusicScriptId(PlayerTypes eForPlayer) const
{
	CvPlayer& kForPlayer = GET_PLAYER(eForPlayer);
	EraTypes eEra = kForPlayer.getCurrentEra();
	CvLeaderHeadInfo& kLeader = GC.getLeaderHeadInfo(getLeaderType());
	if (GET_TEAM(kForPlayer.getTeam()).isAtWar(getTeam()))
	{
		return kLeader.getDiploWarIntroMusicScriptIds(eEra);
	}
	else
	{
		return kLeader.getDiploPeaceIntroMusicScriptIds(eEra);
	}
}

int CvPlayer::getMusicScriptId(PlayerTypes eForPlayer) const
{
	CvPlayer& kForPlayer = GET_PLAYER(eForPlayer);
	EraTypes eEra = kForPlayer.getCurrentEra();
	CvLeaderHeadInfo& kLeader = GC.getLeaderHeadInfo(getLeaderType());
	if (GET_TEAM(kForPlayer.getTeam()).isAtWar(getTeam()))
	{
		return kLeader.getDiploWarMusicScriptIds(eEra);
	}
	else
	{
		return kLeader.getDiploPeaceMusicScriptIds(eEra);
	}
}


void CvPlayer::getGlobeLayerColors(GlobeLayerTypes eGlobeLayerType, int iOption, std::vector<NiColorA>& aColors, std::vector<CvPlotIndicatorData>& aIndicators) const
{
	switch (eGlobeLayerType)
	{
	case GLOBE_LAYER_TRADE:
		getTradeLayerColors(aColors, aIndicators);
		break;
	case GLOBE_LAYER_UNIT:
		getUnitLayerColors((GlobeLayerUnitOptionTypes) iOption, aColors, aIndicators);
		break;
	case GLOBE_LAYER_RESOURCE:
		getResourceLayerColors((GlobeLayerResourceOptionTypes) iOption, aColors, aIndicators);
		break;
	case GLOBE_LAYER_RELIGION:
		getReligionLayerColors((ReligionTypes) iOption, aColors, aIndicators);
		break;
	case GLOBE_LAYER_CULTURE:
		getCultureLayerColors(aColors, aIndicators);
		break;
	default:
		FAssertMsg(false, "Unknown globe layer type");
		break;
	}
}

void CvPlayer::getTradeLayerColors(std::vector<NiColorA>& aColors, std::vector<CvPlotIndicatorData>& aIndicators) const
{
	aColors.resize(GC.getMapINLINE().numPlotsINLINE(), NiColorA(0, 0, 0, 0));
	aIndicators.clear();

	typedef std::map< int, std::vector<int> > PlotGroupMap;
	PlotGroupMap mapPlotGroups;
	for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); ++iI)
	{
		CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);
		CvPlotGroup* pPlotGroup = pLoopPlot->getPlotGroup(getID());
		if (pPlotGroup != NULL && pLoopPlot->isRevealed(getTeam(), true) && pLoopPlot->getTeam() == getTeam())
		{
			mapPlotGroups[pPlotGroup->getID()].push_back(iI);
		}
	}

	CvRandom kRandom;
	kRandom.init(42);
	for (PlotGroupMap::iterator it = mapPlotGroups.begin(); it != mapPlotGroups.end(); ++it)
	{
		NiColorA kColor(kRandom.getFloat(), kRandom.getFloat(), kRandom.getFloat(), 0.8f);
		std::vector<int>& aPlots = it->second;
		for (size_t i = 0; i < aPlots.size(); ++i)
		{
			aColors[aPlots[i]] = kColor;
		}
	}
}

void CvPlayer::getUnitLayerColors(GlobeLayerUnitOptionTypes eOption, std::vector<NiColorA>& aColors, std::vector<CvPlotIndicatorData>& aIndicators) const
{
	aColors.resize(GC.getMapINLINE().numPlotsINLINE(), NiColorA(0, 0, 0, 0));
	aIndicators.clear();

	std::vector< std::vector<float> > aafPlayerPlotStrength(MAX_PLAYERS);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (GET_PLAYER((PlayerTypes)i).isAlive())
		{
			aafPlayerPlotStrength[i].resize(GC.getMapINLINE().numPlotsINLINE());
		}
	}

	float fMaxPlotStrength = 0.0f;

	// create unit plot indicators...
	// build the trade group texture
	typedef std::map<int, NiColor> GroupMap;
	GroupMap mapColors;

	// Loop through all the players
	CvWStringBuffer szBuffer;
	for (int iPlayer = 0; iPlayer < MAX_PLAYERS; iPlayer++)
	{
		if (GET_PLAYER((PlayerTypes)iPlayer).isAlive())
		{
			for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); ++iI)
			{
				CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);
				int iNumUnits = pLoopPlot->getNumUnits();
				float fPlotStrength = 0.0f;

				if (iNumUnits > 0 && pLoopPlot->isVisible(getTeam(), true))
				{
					bool bShowIndicator = false;

					CLLNode<IDInfo>* pUnitNode = pLoopPlot->headUnitNode();
					while (pUnitNode != NULL)
					{
						CvUnit* pUnit = ::getUnit(pUnitNode->m_data);
						pUnitNode = pLoopPlot->nextUnitNode(pUnitNode);

						if (pUnit->getVisualOwner() == iPlayer && !pUnit->isInvisible(getTeam(), GC.getGameINLINE().isDebugMode()))
						{
							// now, is this unit of interest?
							bool bIsMilitary = pUnit->baseCombatStr() > 0;
							bool bIsEnemy = pUnit->isEnemy(getTeam());
							bool bIsOnOurTeam = pUnit->getTeam() == getTeam();
							bool bOfInterest = false;

							switch (eOption)
							{
							case SHOW_ALL_MILITARY:
								{
									bOfInterest = bIsMilitary;
									if (bOfInterest)
									{
										fPlotStrength += ((float) pUnit->currHitPoints() / (float) pUnit->maxHitPoints() * (float) pUnit->baseCombatStr());
									}
									break;
								}
							case SHOW_TEAM_MILITARY:
								{
									bOfInterest = bIsMilitary && bIsOnOurTeam;
									if (bOfInterest)
										fPlotStrength += ((float) pUnit->currHitPoints() / (float) pUnit->maxHitPoints() * (float) pUnit->baseCombatStr());
									break;
								}
							case SHOW_ENEMIES:
								{
									bOfInterest = bIsMilitary && bIsEnemy;
									if (bOfInterest)
										fPlotStrength += ((float) pUnit->currHitPoints() / (float) pUnit->maxHitPoints() * (float) pUnit->baseCombatStr());
									break;
								}

							case SHOW_ENEMIES_IN_TERRITORY:
								{
									bOfInterest = bIsMilitary;
									break;
								}

							case SHOW_PLAYER_DOMESTICS:
								{
									bOfInterest = !bIsMilitary;// && (pUnit->getVisualOwner() == eCurPlayer);
									break;
								}
							default:
								bOfInterest = false;
								break;
							}

							// create the indicator
							if (bOfInterest)
							{
								bShowIndicator = true;
							}

							fMaxPlotStrength = std::max(fPlotStrength, fMaxPlotStrength);
							aafPlayerPlotStrength[iPlayer][iI] = fPlotStrength;
						}
					}

					if (bShowIndicator)
					{
						CvUnit* pUnit = pLoopPlot->getBestDefender(NO_PLAYER);
						if (pUnit != NULL)
						{
							PlayerColorTypes eUnitColor = GET_PLAYER(pUnit->getVisualOwner()).getPlayerColor();
							const NiColorA& kColor = GC.getColorInfo((ColorTypes) GC.getPlayerColorInfo(eUnitColor).getColorTypePrimary()).getColor();

							szBuffer.clear();
							GAMETEXT.setPlotListHelp(szBuffer, pLoopPlot, true, true);

							CvPlotIndicatorData kIndicator;
							kIndicator.m_pUnit = pUnit;
							kIndicator.m_strLabel = "UNITS";
							kIndicator.m_strIcon = pUnit->getButton();

							if (eOption == SHOW_ENEMIES_IN_TERRITORY)
							{
								kIndicator.m_kColor.r = 1;
								kIndicator.m_kColor.r = 0;
								kIndicator.m_kColor.r = 0;
							}
							else
							{
								kIndicator.m_kColor.r = kColor.r;
								kIndicator.m_kColor.g = kColor.g;
								kIndicator.m_kColor.b = kColor.b;
							}
							kIndicator.m_strHelpText = szBuffer.getCString();

							//setup visibility
							switch (eOption)
							{
							case SHOW_ENEMIES_IN_TERRITORY:
								kIndicator.m_bTestEnemyVisibility = true;
								kIndicator.m_eVisibility = PLOT_INDICATOR_VISIBLE_ALWAYS;
								break;
							case SHOW_ENEMIES:
								kIndicator.m_eVisibility = PLOT_INDICATOR_VISIBLE_ALWAYS;
								break;
							default:
								kIndicator.m_eVisibility = PLOT_INDICATOR_VISIBLE_ONSCREEN_ONLY;
								break;
							}
							aIndicators.push_back(kIndicator);
						}
					}
				}
			}
		}
	}

	if (fMaxPlotStrength > 0)
	{
		for (int iPlayer = 0; iPlayer < MAX_PLAYERS; iPlayer++)
		{
			if (GET_PLAYER((PlayerTypes)iPlayer).isAlive())
			{
				PlayerColorTypes eCurPlayerColor = GET_PLAYER((PlayerTypes) iPlayer).getPlayerColor();
				const NiColorA& kColor = GC.getColorInfo((ColorTypes) GC.getPlayerColorInfo(eCurPlayerColor).getColorTypePrimary()).getColor();

				for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
				{
					CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);
					if (pLoopPlot->isVisible(getTeam(), true))
					{
						float fPlotStrength = aafPlayerPlotStrength[iPlayer][iI];
						if (fPlotStrength > 0)
						{
							float fAlpha = (fPlotStrength / fMaxPlotStrength * 0.75f + 0.25f) * 0.8f;
							if (fAlpha > aColors[iI].a)
							{
								aColors[iI] = kColor;
								aColors[iI].a = fAlpha;
							}
						}
					}
				}
			}
		}
	}
}

void CvPlayer::getResourceLayerColors(GlobeLayerResourceOptionTypes eOption, std::vector<NiColorA>& aColors, std::vector<CvPlotIndicatorData>& aIndicators) const
{
	aColors.clear();
	aIndicators.clear();

	PlayerColorTypes ePlayerColor = getPlayerColor();

	CvWStringBuffer szBuffer;
	for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);
		PlayerTypes eOwner = pLoopPlot->getRevealedOwner(getTeam(), true);

		if (pLoopPlot->isRevealed(getTeam(), true))
		{
			BonusTypes eCurType = pLoopPlot->getBonusType((GC.getGame().isDebugMode()) ? NO_TEAM : getTeam());
			if (eCurType != NO_BONUS)
			{
				CvBonusInfo& kBonusInfo = GC.getBonusInfo(eCurType);

				bool bOfInterest = false;
				switch (eOption)
				{
				case SHOW_ALL_RESOURCES:
					bOfInterest = true;
					break;
				case SHOW_STRATEGIC_RESOURCES:
					bOfInterest = (kBonusInfo.getHappiness() == 0) && (kBonusInfo.getHealth() == 0);
					break;
				case SHOW_HAPPY_RESOURCES:
					bOfInterest = (kBonusInfo.getHappiness() != 0 ) && (kBonusInfo.getHealth() == 0);
					break;
				case SHOW_HEALTH_RESOURCES:
					bOfInterest = (kBonusInfo.getHappiness() == 0) && (kBonusInfo.getHealth() != 0);
					break;

				}

				if (bOfInterest)
				{
					CvPlotIndicatorData kData;
					kData.m_strLabel = "RESOURCES";
					kData.m_eVisibility = PLOT_INDICATOR_VISIBLE_ONSCREEN_ONLY;
					kData.m_strIcon = GC.getBonusInfo(eCurType).getButton();

					int x = pLoopPlot->getX();
					int y = pLoopPlot->getY();
					kData.m_Target = NiPoint2(GC.getMapINLINE().plotXToPointX(x), GC.getMapINLINE().plotYToPointY(y));

					if (eOwner == NO_PLAYER)
					{
						kData.m_kColor.r = 0.8f;
						kData.m_kColor.g = 0.8f;
						kData.m_kColor.b = 0.8f;
					}
					else
					{
						PlayerColorTypes eCurPlayerColor = GET_PLAYER(eOwner).getPlayerColor();
						const NiColorA& kColor = GC.getColorInfo((ColorTypes) GC.getPlayerColorInfo(eCurPlayerColor).getColorTypePrimary()).getColor();
						kData.m_kColor.r = kColor.r;
						kData.m_kColor.g = kColor.g;
						kData.m_kColor.b = kColor.b;
					}

					szBuffer.clear();
					GAMETEXT.setBonusHelp(szBuffer, eCurType, false);
					kData.m_strHelpText = szBuffer.getCString();

					aIndicators.push_back(kData);
				}
			}
		}
	}
}

void CvPlayer::getReligionLayerColors(ReligionTypes eSelectedReligion, std::vector<NiColorA>& aColors, std::vector<CvPlotIndicatorData>& aIndicators) const
{
	aColors.resize(GC.getMapINLINE().numPlotsINLINE(), NiColorA(0, 0, 0, 0));
	aIndicators.clear();

	CvRandom kRandom;
	kRandom.init(42 * eSelectedReligion);
	const NiColorA kBaseColor(kRandom.getFloat(), kRandom.getFloat(), kRandom.getFloat(), 1.0f);

	for (int iI = 0; iI  < MAX_PLAYERS; iI ++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			int iLoop;
			for (CvCity* pLoopCity = GET_PLAYER((PlayerTypes)iI).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iI).nextCity(&iLoop))
			{
				if (pLoopCity->isRevealed(getTeam(), true))
				{
					if (pLoopCity->isHasReligion(eSelectedReligion))
					{
						float fAlpha = 0.8f;
						if (!pLoopCity->isHolyCity(eSelectedReligion))
						{
							fAlpha *= 0.5f;
						}

						// loop through the city's plots
						for (int iJ = 0; iJ < NUM_CITY_PLOTS; iJ++)
						{
							CvPlot* pLoopPlot = plotCity(pLoopCity->getX(), pLoopCity->getY(), iJ);
							if (pLoopPlot != NULL)
							{
								// visibility query
								if (pLoopPlot->isRevealed(getTeam(), true))
								{
									int iIndex = GC.getMapINLINE().plotNumINLINE(pLoopPlot->getX_INLINE(), pLoopPlot->getY_INLINE());
									if (fAlpha > aColors[iIndex].a)
									{
										aColors[iIndex] = kBaseColor;
										aColors[iIndex].a = fAlpha;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void CvPlayer::getCultureLayerColors(std::vector<NiColorA>& aColors, std::vector<CvPlotIndicatorData>& aIndicators) const
{
	const int iColorsPerPlot = 4;
	aColors.resize(GC.getMapINLINE().numPlotsINLINE() * iColorsPerPlot, NiColorA(0, 0, 0, 0));
	aIndicators.clear();

	// find maximum total culture
	int iMaxTotalCulture = INT_MIN;
	int iMinTotalCulture = INT_MAX;
	for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);
		int iTotalCulture = pLoopPlot->countTotalCulture();
		if (iTotalCulture > iMaxTotalCulture)
		{
			iMaxTotalCulture = iTotalCulture;
		}
		if (iTotalCulture < iMinTotalCulture && iTotalCulture > 0)
		{
			iMinTotalCulture = iTotalCulture;
		}
	}
	iMinTotalCulture = 0;

	// find culture percentages
	for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);
		PlayerTypes eOwner = pLoopPlot->getRevealedOwner(getTeam(), true);

		// how many people own this plot?
		std::vector < std::pair<int,int> > plot_owners;
		int iNumNonzeroOwners = 0;
		for (int iPlayer = 0; iPlayer < MAX_CIV_PLAYERS; iPlayer++)
		{
			if (GET_PLAYER((PlayerTypes)iPlayer).isAlive())
			{
				int iCurCultureAmount = pLoopPlot->getCulture((PlayerTypes)iPlayer);
				if (iCurCultureAmount != 0)
				{
					iNumNonzeroOwners ++;
					plot_owners.push_back(std::pair<int,int>(iCurCultureAmount, iPlayer));
				}
			}
		}

		// ensure that it is revealed
		if (!plot_owners.empty() && pLoopPlot->getRevealedOwner(getTeam(), true) != NO_PLAYER)
		{
			for (int i = 0; i < iColorsPerPlot; ++i)
			{
				int iCurOwnerIdx = i % plot_owners.size();
				PlayerTypes eCurOwnerID = (PlayerTypes) plot_owners[iCurOwnerIdx].second;
				int iCurCulture = plot_owners[iCurOwnerIdx].first;
				const NiColorA& kCurColor = GC.getColorInfo((ColorTypes) GC.getPlayerColorInfo(GET_PLAYER(eCurOwnerID).getPlayerColor()).getColorTypePrimary()).getColor();

				// damp the color by the value...
				aColors[iI * iColorsPerPlot + i] = kCurColor;
				float blend_factor = 0.5f * std::min(1.0f, std::max(0.0f, (float)(iCurCulture - iMinTotalCulture) / iMaxTotalCulture));
				aColors[iI * iColorsPerPlot + i].a = std::min(0.8f * blend_factor + 0.5f, 1.0f);
			}
		}
	}
}

void CvPlayer::cheat(bool bCtrl, bool bAlt, bool bShift)
{
	if (gDLL->getChtLvl() > 0)
	{
		GET_TEAM(getTeam()).setHasTech(getCurrentResearch(), true, getID(), true, false);
	}
}

const CvArtInfoUnit* CvPlayer::getUnitArtInfo(UnitTypes eUnit, int iMeshGroup) const
{
	CivilizationTypes eCivilization = getCivilizationType();

	if (eCivilization == NO_CIVILIZATION)
	{
		eCivilization = (CivilizationTypes) GC.getDefineINT("BARBARIAN_CIVILIZATION");
	}
	UnitArtStyleTypes eStyle = (UnitArtStyleTypes) GC.getCivilizationInfo(eCivilization).getUnitArtStyleType();
	EraTypes eEra = getCurrentEra();
	if (eEra == NO_ERA)
	{
		eEra = (EraTypes) 0;
	}
	return GC.getUnitInfo(eUnit).getArtInfo(iMeshGroup, eEra, eStyle);
}

bool CvPlayer::hasSpaceshipArrived() const
{
	VictoryTypes eSpaceVictory = GC.getGameINLINE().getSpaceVictory();
	if (eSpaceVictory != NO_VICTORY)
	{
		int iVictoryCountdown = GET_TEAM(getTeam()).getVictoryCountdown(eSpaceVictory);
		if (((GC.getGameINLINE().getGameState() == GAMESTATE_EXTENDED) && (iVictoryCountdown > 0)) || (iVictoryCountdown == 0))
		{
			return true;
		}
	}

	return false;
}

//FfH Traits: Added by Kael 08/02/2007
bool CvPlayer::canSeeCivic(int iCivic) const
{
    if (GC.getCivicInfo((CivicTypes)iCivic).isHidden())
    {
        if (getCivics((CivicOptionTypes)GC.getCivicInfo((CivicTypes)iCivic).getCivicOptionType()) == iCivic)
        {
            return true;
        }
        return false;
    }
    return true;
}

bool CvPlayer::canSeeReligion(int iReligion, CvCity* pCity) const
{
    if (GC.getReligionInfo((ReligionTypes)iReligion).isHidden())
    {
        if (getStateReligion() == iReligion)
        {
            return true;
        }
        if (pCity != NULL)
        {
            if (pCity->getOwner() == getID())
            {
                return true;
            }
        }
        return false;
    }
    return true;
}

bool CvPlayer::isAdaptive() const
{
    return m_bAdaptive;
}

void CvPlayer::setAdaptive(bool bNewValue)
{
	m_bAdaptive = bNewValue;
}

bool CvPlayer::isAgnostic() const
{
    return m_bAgnostic;
}

void CvPlayer::setAgnostic(bool bNewValue)
{
	m_bAgnostic = bNewValue;
}

bool CvPlayer::isAssimilation() const
{
    return m_bAssimilation;
}

void CvPlayer::setAssimilation(bool bNewValue)
{
	m_bAssimilation = bNewValue;
}

bool CvPlayer::isDeclaringWar() const
{
    return m_bDeclaringWar;
}

void CvPlayer::setDeclaringWar(bool bNewValue)
{
	m_bDeclaringWar = bNewValue;
}

bool CvPlayer::isHideUnits() const
{
    return m_bHideUnits;
}

void CvPlayer::setHideUnits(bool bNewValue)
{
	m_bHideUnits = bNewValue;
}

bool CvPlayer::isIgnoreFood() const
{
    return m_bIgnoreFood;
}

void CvPlayer::setIgnoreFood(bool bNewValue)
{
    if (bNewValue !=m_bIgnoreFood)
    {
        m_bIgnoreFood = bNewValue;

        //update yields of Plots
        for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
        {
            CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);
            if(pLoopPlot->getOwnerINLINE()==getID())
            {
                pLoopPlot->updateYield();
            }
        }
    }
}

bool CvPlayer::isInsane() const
{
    return m_bInsane;
}

void CvPlayer::setInsane(bool bNewValue)
{
	m_bInsane = bNewValue;
}

bool CvPlayer::isNoDiplomacyWithEnemies() const
{
    if (m_iNoDiplomacyWithEnemies == 0)
    {
        return false;
    }
    return true;
}

void CvPlayer::changeNoDiplomacyWithEnemies(int iChange)
{
    if (iChange != 0)
    {
        m_iNoDiplomacyWithEnemies += iChange;
    }
}

bool CvPlayer::isSeeInvisible() const
{
    return m_bSeeInvisible;
}

void CvPlayer::setSeeInvisible(bool bNewValue)
{
	m_bSeeInvisible = bNewValue;
}

bool CvPlayer::isSprawling() const
{
    return m_bSprawling;
}

void CvPlayer::setSprawling(bool bNewValue)
{
	m_bSprawling = bNewValue;
}

bool CvPlayer::isGamblingRing() const
{
   	for (int iI = 0; iI < GC.getNumVoteSourceInfos(); ++iI)
	{
	    if (isFullMember((VoteSourceTypes)iI))
	    {
	        if (GC.getGameINLINE().isGamblingRing((VoteSourceTypes)iI))
	        {
	            return true;
	        }
	    }
	}
    return false;
}

bool CvPlayer::isHasTech(int iTech) const
{
    return GET_TEAM(getTeam()).isHasTech((TechTypes)iTech);
}

bool CvPlayer::isSlaveTrade() const
{
   	for (int iI = 0; iI < GC.getNumVoteSourceInfos(); ++iI)
	{
	    if (isFullMember((VoteSourceTypes)iI))
	    {
	        if (GC.getGameINLINE().isSlaveTrade((VoteSourceTypes)iI))
	        {
	            return true;
	        }
	    }
	}
    return false;
}

bool CvPlayer::isSmugglingRing() const
{
   	for (int iI = 0; iI < GC.getNumVoteSourceInfos(); ++iI)
	{
	    if (isFullMember((VoteSourceTypes)iI))
	    {
	        if (GC.getGameINLINE().isSmugglingRing((VoteSourceTypes)iI))
	        {
	            return true;
	        }
	    }
	}
    return false;
}

int CvPlayer::getAlignment() const
{
    return m_iAlignment;
}

void CvPlayer::setAlignment(int iNewValue)
{
    m_iAlignment = iNewValue;
	gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
}

int CvPlayer::getDisableProduction() const
{
    return m_iDisableProduction;
}

void CvPlayer::changeDisableProduction(int iChange)
{
	if (iChange != 0)
	{
		m_iDisableProduction = (m_iDisableProduction + iChange);
	}
}

int CvPlayer::getDisableResearch() const
{
    return m_iDisableResearch;
}

void CvPlayer::changeDisableResearch(int iChange)
{
	if (iChange != 0)
	{
		m_iDisableResearch += iChange;
	}
}

int CvPlayer::getDisableSpellcasting() const
{
    return m_iDisableSpellcasting;
}

void CvPlayer::changeDisableSpellcasting(int iChange)
{
	if (iChange != 0)
	{
		m_iDisableSpellcasting = (m_iDisableSpellcasting + iChange);
	}
}

int CvPlayer::getEnslavementChance() const
{
    return m_iEnslavementChance;
}

void CvPlayer::changeEnslavementChance(int iChange)
{
	if (iChange != 0)
	{
		m_iEnslavementChance = (m_iEnslavementChance + iChange);
	}
}

int CvPlayer::getFreeXPFromCombat() const
{
    return m_iFreeXPFromCombat;
}

void CvPlayer::changeFreeXPFromCombat(int iChange)
{
	if (iChange != 0)
	{
		m_iFreeXPFromCombat = (m_iFreeXPFromCombat + iChange);
	}
}

int CvPlayer::getNumBuilding(int iBuilding) const
{
   	CvCity* pLoopCity;
	int iLoop;
	int iCount = 0;
	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iCount += pLoopCity->getNumRealBuilding((BuildingTypes)iBuilding);
	}
	return iCount;
}

int CvPlayer::getMaxCities() const
{
    return m_iMaxCities;
}

void CvPlayer::setMaxCities(int iNewValue)
{
    m_iMaxCities = iNewValue;
}

int CvPlayer::getNumSettlements() const
{
	CvCity* pLoopCity;
	int iCount = 0;
	int iLoop;
	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
	    if (pLoopCity->isSettlement())
	    {
	        iCount += 1;
	    }
	}
	return iCount;
}

int CvPlayer::getPillagingGold() const
{
    return m_iPillagingGold;
}

void CvPlayer::changePillagingGold(int iChange)
{
	if (iChange != 0)
	{
		m_iPillagingGold += iChange;
	}
}

int CvPlayer::getPlayersKilled() const
{
    return m_iPlayersKilled;
}

void CvPlayer::changePlayersKilled(int iChange)
{
	if (iChange != 0)
	{
		m_iPlayersKilled += iChange;
	}
}

int CvPlayer::getRealPlayer() const
{
    return m_iRealPlayer;
}

void CvPlayer::setRealPlayer(int iNewValue)
{
    m_iRealPlayer = iNewValue;
}

int CvPlayer::getResistEnemyModify() const
{
    return m_iResistEnemyModify;
}

void CvPlayer::changeResistEnemyModify(int iChange)
{
	if (iChange != 0)
	{
		m_iResistEnemyModify += iChange;
	}
}

int CvPlayer::getResistModify() const
{
    return m_iResistModify;
}

void CvPlayer::changeResistModify(int iChange)
{
	if (iChange != 0)
	{
		m_iResistModify += iChange;
	}
}

int CvPlayer::getSanctuaryTimer() const
{
    return m_iSanctuaryTimer;
}

void CvPlayer::changeSanctuaryTimer(int iChange)
{
    if (iChange > 0)
    {
        gDLL->getEngineIFace()->AddGreatWall(getCapitalCity());
    }
	if (iChange != 0)
	{
		m_iSanctuaryTimer += iChange;
	}
	if (m_iSanctuaryTimer == 0)
	{
        gDLL->getEngineIFace()->RemoveGreatWall(getCapitalCity());
	}
}

int CvPlayer::getTempPlayerTimer() const
{
    return m_iTempPlayerTimer;
}

void CvPlayer::changeTempPlayerTimer(int iChange)
{
	if (iChange != 0)
	{
		m_iTempPlayerTimer = (m_iTempPlayerTimer + iChange);
	}
}

int CvPlayer::getStartingGold() const
{
    return m_iStartingGold;
}

void CvPlayer::changeStartingGold(int iChange)
{
	if (iChange != 0)
	{
		m_iStartingGold = (m_iStartingGold + iChange);
	}
}

int CvPlayer::getSummonCostReduction() const
{
    return m_iSummonCostReduction;
}

void CvPlayer::changeSummonCostReduction(int iChange)
{
	m_iSummonCostReduction += iChange;
}

int CvPlayer::getTaxesModifier() const
{
    return m_iTaxesModifier;
}

void CvPlayer::changeTaxesModifier(int iChange)
{
	m_iTaxesModifier += iChange;
}

int CvPlayer::getFoodPerPopulationModifier() const
{
    return m_iFoodPerPopulationModifier;
}

void CvPlayer::changeFoodPerPopulationModifier(int iChange)
{
	m_iFoodPerPopulationModifier += iChange;
}

int CvPlayer::getUpgradeCostModifier() const
{
    return m_iUpgradeCostModifier;
}

void CvPlayer::changeUpgradeCostModifier(int iChange)
{
	if (iChange != 0)
	{
		m_iUpgradeCostModifier = (m_iUpgradeCostModifier + iChange);
	}
}

int CvPlayer::getProductionCostReductionCapitalBuilding() const
{
    return m_iProductionCostReductionCapitalBuilding;
}

void CvPlayer::changeProductionCostReductionCapitalBuilding(int iChange)
{
	if (iChange != 0)
	{
		m_iProductionCostReductionCapitalBuilding += iChange;
	}
}

int CvPlayer::getUnitSupportModifier() const
{
    return m_iUnitSupportModifier;
}

void CvPlayer::changeUnitSupportModifier(int iChange)
{
	if (iChange != 0)
	{
		m_iUnitSupportModifier += iChange;
	}
}

int CvPlayer::getDiscoverRandModifier() const
{
    return m_iDiscoverRandModifier;
}

void CvPlayer::changeDiscoverRandModifier(int iChange)
{
	if (iChange != 0)
	{
		m_iDiscoverRandModifier = (m_iDiscoverRandModifier + iChange);
	}
}

int CvPlayer::getHealChange() const
{
    return m_iHealChange;
}

void CvPlayer::changeHealChange(int iChange)
{
	if (iChange != 0)
	{
		m_iHealChange = (m_iHealChange + iChange);
	}
}

int CvPlayer::getHealChangeEnemy() const
{
    return m_iHealChangeEnemy;
}

void CvPlayer::changeHealChangeEnemy(int iChange)
{
	if (iChange != 0)
	{
		m_iHealChangeEnemy = (m_iHealChangeEnemy + iChange);
	}
}

void CvPlayer::changeSpecialistTypeExtraCommerce(SpecialistTypes eIndex1, CommerceTypes eIndex2, int iChange)
{
	if (iChange != 0)
	{
		m_ppaaiSpecialistTypeExtraCommerce[eIndex1][eIndex2] = (m_ppaaiSpecialistTypeExtraCommerce[eIndex1][eIndex2] + iChange);
		updateCommerce();
		AI_makeAssignWorkDirty();
	}
}

int CvPlayer::getSpecialistTypeExtraCommerce(SpecialistTypes eIndex1, CommerceTypes eIndex2) const
{
	return m_ppaaiSpecialistTypeExtraCommerce[eIndex1][eIndex2];
}

void CvPlayer::setGreatPeopleCreated(int iNewValue)
{
	m_iGreatPeopleCreated = iNewValue;
}

void CvPlayer::setGreatPeopleThresholdModifier(int iNewValue)
{
	m_iGreatPeopleThresholdModifier = iNewValue;
}
//FfH: End Add

/*************************************************************************************************/
/** Skyre Mod                                                                                   **/
/** BETTER AI (Lanun Pirate Coves) merged Sephi                                                 **/
/**						                                            							**/
/*************************************************************************************************/
bool CvPlayer::isPirate() const
{
    SpellTypes ePirateCoveSpell = (SpellTypes)GC.getDefineINT("PIRATE_COVE_SPELL");

    if (ePirateCoveSpell != NO_SPELL)
    {
        CivilizationTypes ePirateCiv = (CivilizationTypes)GC.getSpellInfo(ePirateCoveSpell).getCivilizationPrereq();

        if (ePirateCiv == NO_CIVILIZATION || ePirateCiv == getCivilizationType())
        {
            return true;
        }
    }

    return false;
}
/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

/*************************************************************************************************/
/**	BETTER AI (New Functions) Sephi                                 					        **/
/**																								**/
/**						                                            							**/
/*************************************************************************************************/
bool CvPlayer::isConquestMode() const
{
    return m_bConquestMode;
}

void CvPlayer::startConquestMode()
{
    m_bConquestMode=true;
}

ReligionTypes CvPlayer::getFavoriteReligion() const
{
    return m_eFavoriteReligion;
}

void CvPlayer::setFavoriteReligion(ReligionTypes newvalue)
{
    m_eFavoriteReligion=newvalue;
}

bool CvPlayer::AI_isRangedAttackMode()
{
    return m_bRangedAttackMode;
}

void CvPlayer::AI_setRangedAttackMode(bool newvalue)
{
    m_bRangedAttackMode=newvalue;
}

int CvPlayer::countGroupFlagUnits(int Groupflag) const
{
    int countunits=0;
    int iLoop;

    for (CvUnit* pUnit = firstUnit(&iLoop); NULL != pUnit; pUnit = nextUnit(&iLoop))
    {
        if(pUnit)
        {
            if(pUnit->AI_getGroupflag()==Groupflag)
            {
                countunits++;
            }
        }
    }
    return countunits;
}

void CvPlayer::AI_doFavoriteReligion()
{
	if (isAgnostic())
    {
        return;
	}

    //Pick a Favorite Religion
    if (getFavoriteReligion()==NO_RELIGION)
    {
        for (int i=0;i<GC.getNumReligionInfos();i++)
        {
            if (GC.getGameINLINE().getSorenRandNum(100,"Choose Religion")<GC.getLeaderHeadInfo(getLeaderType()).getReligionWeightModifier(i))
            {
                setFavoriteReligion((ReligionTypes)i);
                break;
            }
        }
        //If still have none, pick randomly
        int hack=0;
        while (getFavoriteReligion()==NO_RELIGION)
        {
            ReligionTypes newReligion=(ReligionTypes)GC.getGameINLINE().getSorenRandNum(GC.getNumReligionInfos(),"Choose Religion");
            if (GC.getLeaderHeadInfo(getLeaderType()).getReligionWeightModifier(newReligion)>=0)
            {
                setFavoriteReligion(newReligion);
            }
            hack++;
            if (hack==30)
            {
                break;
            }
        }
    }
    //Convert to Favorite Religion if possible
    if (getFavoriteReligion()!=NO_RELIGION)
    {
        if(canConvert(getFavoriteReligion()))
        {
            convert(getFavoriteReligion());
        }
    }
    //Choose new Religion for Opportunists
    if (GC.getLeaderHeadInfo(getLeaderType()).isReligionOpportunist() && getFavoriteReligion()!=NO_RELIGION)
    {
//>>>>Unofficial Bug Fix: Modified by Denev 2010/02/07
//*** Fix frequently flipping of state religion.
/*
		bool valid=true;
		if (GC.getReligionInfo(getFavoriteReligion()).getReligionHero1()!=NO_UNITCLASS)
		{
			if (GC.getGameINLINE().getUnitClassCreatedCount(GC.getReligionInfo(getFavoriteReligion()).getReligionHero1())==0)
			{
				valid=false;
			}
		}

		if (GC.getReligionInfo(getFavoriteReligion()).getReligionHero2()!=NO_UNITCLASS)
		{
			if (GC.getGameINLINE().getUnitClassCreatedCount(GC.getReligionInfo(getFavoriteReligion()).getReligionHero2())==0)
			{
				valid=false;
			}
		}

		if (GC.getReligionInfo(getFavoriteReligion()).getReligionHero1()!=NO_UNITCLASS
			&& getUnitClassCountPlusMaking(GC.getReligionInfo(getFavoriteReligion()).getReligionHero1())>0)
		{
			valid=false;
		}

		if (GC.getReligionInfo(getFavoriteReligion()).getReligionHero2()!=NO_UNITCLASS
			&& getUnitClassCountPlusMaking(GC.getReligionInfo(getFavoriteReligion()).getReligionHero2())>0)
		{
			valid=false;
		}

		if (valid)
		{
			for (int i=0;i<GC.getNumReligionInfos();i++)
			{
				if (((GC.getReligionInfo(getFavoriteReligion()).getReligionHero1()!=NO_UNITCLASS
					&& GC.getGameINLINE().getUnitClassCreatedCount(GC.getReligionInfo((ReligionTypes)i).getReligionHero1())==0))
					||((GC.getReligionInfo(getFavoriteReligion()).getReligionHero2()!=NO_UNITCLASS && GC.getGameINLINE().getUnitClassCreatedCount(GC.getReligionInfo((ReligionTypes)i).getReligionHero2())==0)))
				{
					if (GC.getLeaderHeadInfo(getLeaderType()).getReligionWeightModifier((ReligionTypes)i)>=0)
					{
						setFavoriteReligion((ReligionTypes)i);
					}
					break;
				}
			}
		}
*/
		const UnitClassTypes eReligionHero1 = (UnitClassTypes)GC.getReligionInfo(getFavoriteReligion()).getReligionHero1();
		const UnitClassTypes eReligionHero2 = (UnitClassTypes)GC.getReligionInfo(getFavoriteReligion()).getReligionHero2();

		bool bHoldCurrentFavorite = false;

		if ((eReligionHero1 != NO_UNITCLASS && GC.getGameINLINE().getUnitClassCreatedCount(eReligionHero1) == 0)
		 || (eReligionHero2 != NO_UNITCLASS && GC.getGameINLINE().getUnitClassCreatedCount(eReligionHero2) == 0))
		{
			bHoldCurrentFavorite = true;
		}

		if ((eReligionHero1 != NO_UNITCLASS && getUnitClassCountPlusMaking(eReligionHero1) > 0)
		 || (eReligionHero2 != NO_UNITCLASS && getUnitClassCountPlusMaking(eReligionHero2) > 0))
		{
			bHoldCurrentFavorite = true;
		}

		if (!bHoldCurrentFavorite)
		{
			for (int iReligion = 0; iReligion < GC.getNumReligionInfos(); iReligion++)
			{
				const UnitClassTypes eLoopReligionHero1 = (UnitClassTypes)GC.getReligionInfo((ReligionTypes)iReligion).getReligionHero1();
				const UnitClassTypes eLoopReligionHero2 = (UnitClassTypes)GC.getReligionInfo((ReligionTypes)iReligion).getReligionHero2();

				if ((eLoopReligionHero1 != NO_UNITCLASS && GC.getGameINLINE().getUnitClassCreatedCount(eLoopReligionHero1) == 0)
				 || (eLoopReligionHero2 != NO_UNITCLASS && GC.getGameINLINE().getUnitClassCreatedCount(eLoopReligionHero2) == 0))
				{
					if (GC.getLeaderHeadInfo(getLeaderType()).getReligionWeightModifier((ReligionTypes)iReligion) >= 0)
					{
						setFavoriteReligion((ReligionTypes)iReligion);
					}
					break;
				}
			}
		}
//<<<<Unofficial Bug Fix: End Modify
    }
    return;
}

int CvPlayer::AI_getTerraformRitualValue(ProjectTypes eProject)
{
	if(GC.getProjectInfo(eProject).isHostileTerraform())
	{
		return -doTerraformRitual(eProject,true);
	}
	else
	{
		return doTerraformRitual(eProject,true);
	}
}

int CvPlayer::getConquestUnitClassCount(UnitClassTypes UnitClass, CvArea* pArea) const
{
    int count=0;
    int iLoop;
	for(CvUnit* pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{
	    if (pLoopUnit)
	    {
	        if ((pLoopUnit->getUnitClassType()==UnitClass) && (pArea==pLoopUnit->area()) && (pLoopUnit->AI_getGroupflag()==GROUPFLAG_CONQUEST))
	        {
	            count++;
	        }
	    }
	}

    return count;
}

int CvPlayer::AI_getNeededPillageUnits() const
{
    int iRaiderTrait=GC.getInfoTypeForString("TRAIT_RAIDERS");
    int iLeaderValue=100;     //add Leader tag in the future

    int iNeededPillageUnits;

    iNeededPillageUnits=getNumCities()*(8+2*GC.getGameINLINE().getHandicapType())/30;

    iNeededPillageUnits*=iLeaderValue;
    iNeededPillageUnits/=100;

    if (iRaiderTrait!=NO_TRAIT)
    {
        if(hasTrait((TraitTypes)iRaiderTrait))
        {
            iNeededPillageUnits*=150;
            iNeededPillageUnits/=100;
        }
    }

    return iNeededPillageUnits;
}

bool CvPlayer::isCityState() const
{
    return getCityStateType()!=NO_CITYSTATE;
}

int CvPlayer::getCityStateType() const
{
	return m_iCityStateType;
}

void CvPlayer::setCityStateType(int iNewValue)
{
    m_iCityStateType = iNewValue;
}

int CvPlayer::getCityStateCounter() const
{
	return m_iCityStateCounter;
}

void CvPlayer::changeCityStateCounter(int iChange)
{
	m_iCityStateCounter += iChange;
}

int CvPlayer::getPurityCounter() const
{
    return m_iPurityCounter;
}

void CvPlayer::setPurityCounter(int iNewValue)
{
    m_iPurityCounter=iNewValue;
}

int CvPlayer::getPurityCounterCache1() const
{
    return m_iPurityCounterCache1;
}

void CvPlayer::setPurityCounterCache1(int iNewValue)
{
    m_iPurityCounterCache1=iNewValue;
}

void CvPlayer::doUnitSupportLimit()
{
	int iNewValue = 0;
	int iLoop;

	for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iNewValue += (pLoopCity->getUnitSupport());
	}

	//add Civic Mod
	iNewValue += getFreeMilitaryUnitsPopulationPercent() * getNumCities();

	iNewValue *= 100 + getUnitSupportModifier();
	iNewValue /= 100;

	m_iUnitSupportLimit = iNewValue;
}

int CvPlayer::getUnitSupportLimit() const
{
    return m_iUnitSupportLimit;
}

int CvPlayer::getUnitSupportLimitTotal() const
{
	int iTotal = GC.getDefineINT("BASE_UNIT_SUPPORT");

	iTotal += getUnitSupportLimit();

	//add Trait Mod
	iTotal *= 100 + getExtraSupportUnitLimit();
	iTotal /= 100;

	//add difficulty Mod
	if (!isHuman() && !isBarbarian())
	{
		iTotal+=(iTotal*GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIUnitSupportBonusPercent())/100;
	}

	return iTotal;
}
void CvPlayer::changeUnitSupportLimit(int iChange)
{
    m_iUnitSupportLimit += iChange;
}

int CvPlayer::getUnitSupportUsed() const
{
    return m_iUnitSupportUsed;
}

void CvPlayer::changeUnitSupportUsed(int iChange)
{
    m_iUnitSupportUsed += iChange;
}
int CvPlayer::getReducedEquipmentCost() const
{
    return m_iReducedEquipmentCost;
}

void CvPlayer::changeReducedEquipmentCost(int iChange)
{
    m_iReducedEquipmentCost+=iChange;
}

int CvPlayer::getImprovementDiscountPercent() const
{
	return m_iImprovementDiscountPercent;
}

void CvPlayer::changeImprovementDiscountPercent(int iChange)
{
	m_iImprovementDiscountPercent += iChange;
}

int CvPlayer::getGreatPersonRatePerCulture() const
{
	return m_iGreatPersonRatePerCulture;
}

int CvPlayer::getHealthRatePerCulture() const
{
	return m_iHealthRatePerCulture;
}

void CvPlayer::changeGreatPersonRatePerCulture(int iChange)
{
    m_iGreatPersonRatePerCulture+=iChange;
}

void CvPlayer::changeHealthRatePerCulture(int iChange)
{
	if (iChange != 0)
	{
		m_iHealthRatePerCulture+=iChange;

		AI_makeAssignWorkDirty();
	}
}

int CvPlayer::getWillpowerChange() const
{
	return m_iWillpowerChange;
}

void CvPlayer::changeWillpowerChange(int iChange)
{
	m_iWillpowerChange+=iChange;
}

int CvPlayer::getReducedTerraformCost() const
{
	return m_iReducedTerraformCost;
}

void CvPlayer::changeReducedTerraformCost(int iChange)
{
	m_iReducedTerraformCost+=iChange;
}
int CvPlayer::getReducedEnchantmentCost() const
{
	return m_iReducedEnchantmentCost;
}

void CvPlayer::changeReducedEnchantmentCost(int iChange)
{
	m_iReducedEnchantmentCost+=iChange;
}

int CvPlayer::getReducedSummonCost() const
{
	return m_iReducedSummonCost;
}

void CvPlayer::changeReducedSummonCost(int iChange)
{
	m_iReducedSummonCost+=iChange;
}

int CvPlayer::getIncreaseTerraformTiles() const
{
	return m_iIncreaseTerraformTiles;
}

void CvPlayer::changeIncreaseTerraformTiles(int iChange)
{
	m_iIncreaseTerraformTiles+=iChange;
}

int CvPlayer::getIncreaseHostileTerraformTiles() const
{
	return m_iIncreaseHostileTerraformTiles;
}

void CvPlayer::changeIncreaseHostileTerraformTiles(int iChange)
{
	m_iIncreaseHostileTerraformTiles+=iChange;
}

int CvPlayer::getIncreaseSummonXP() const
{
	return m_iIncreaseSummonXP;
}

void CvPlayer::changeIncreaseSummonXP(int iChange)
{
	m_iIncreaseSummonXP+=iChange;
}

int CvPlayer::getSpellDamageModify(int iDamageType) const
{
	return m_paiSpellDamageModify[iDamageType];
}

void CvPlayer::changeSpellDamageModify(int iDamageType, int iChange)
{
	m_paiSpellDamageModify[iDamageType]+=iChange;
}

int CvPlayer::getCurrentMagicRitual() const
{
    return m_iCurrentMagicRitual;
}

void CvPlayer::setCurrentMagicRitual(int iNewValue)
{
	m_iCurrentMagicRitualManaNeeded=0;
    m_iCurrentMagicRitual=iNewValue;
		
	if(iNewValue != NO_PROJECT)
	{
		changeCurrentMagicRitualManaNeeded(getSpecificMagicRitualCost((ProjectTypes)iNewValue));
		AdvanceMagicRitual();
	}
}

int CvPlayer::getCurrentMagicRitualManaNeeded() const
{
    return m_iCurrentMagicRitualManaNeeded;
}

void CvPlayer::changeCurrentMagicRitualManaNeeded(int iChange)
{
    m_iCurrentMagicRitualManaNeeded += iChange;

	if(m_iCurrentMagicRitualManaNeeded == 0 && getCurrentMagicRitual() != NO_PROJECT)
	{
		CvProjectInfo& kRitual = GC.getProjectInfo((ProjectTypes)getCurrentMagicRitual());
		DoGlobalEnchantment((ProjectTypes)getCurrentMagicRitual());
		doTerraformRitual((ProjectTypes)getCurrentMagicRitual(),false);
		doSummonRitual((ProjectTypes)getCurrentMagicRitual());
		setCurrentMagicRitual(NO_PROJECT);
		CvPlayerAI &kPlayer = GET_PLAYER(getID());
		kPlayer.AI_setMagicTargetProject(NO_PROJECT);
	}
}

int CvPlayer::getMaxMana() const
{
    return m_iMaxMana;
}

void CvPlayer::changeMaxMana(int iChange)
{
    m_iMaxMana+=iChange;
}

int CvPlayer::getManaFromSpellResearch() const
{
    return m_iManaFromSpellResearch;
}

void CvPlayer::changeManaFromSpellResearch(int iChange)
{
	if(iChange!=0)
	{
		m_iManaFromSpellResearch+=iChange;
		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(SelectionButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

int CvPlayer::getManaFromNodes() const
{
    return m_iManaFromNodes;
}

void CvPlayer::changeManaFromNodes(int iChange)
{
	if(iChange != 0)
	{
		m_iManaFromNodes += iChange;
	}
}

int CvPlayer::getIncreaseStrengthOfRegularUnits() const
{
	return m_iIncreaseStrengthOfRegularUnits;
}

void CvPlayer::changeIncreaseStrengthOfRegularUnits(int iChange)
{
	CvUnit* pLoopUnit;
	int iLoop;

	if (iChange != 0) 
	{
		m_iIncreaseStrengthOfRegularUnits += iChange;

		for(pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
		{
			pLoopUnit->updateBaseCombatStrength();
		}
	}
}

int CvPlayer::getAI_lostCitiesRevolutionMemory()
{
	return m_iAI_lostCitiesRevolutionMemory;
}

void CvPlayer::changeAI_lostCitiesRevolutionMemory(int iChange)
{
	m_iAI_lostCitiesRevolutionMemory+=iChange;
}

void CvPlayer::changeManaCreationModifier(int iChange)
{
	if(iChange!=0)
	{
		m_iManaCreationModifier+=iChange;
	}
}
int CvPlayer::getManaCreationModifier() const { return m_iManaCreationModifier; }

void CvPlayer::changeResistHostileTerraforming(int iChange)
{
	if(iChange!=0)
	{
		m_iResistHostileTerraforming+=iChange;
	}
}
int CvPlayer::getResistHostileTerraforming() const { return m_iResistHostileTerraforming; }

int CvPlayer::getPopulationLimit() const
{
    return m_iPopulationLimit;
}

void CvPlayer::setPopulationLimit(int iNewValue)
{
    m_iPopulationLimit=iNewValue;
}

bool CvPlayer::canDoGlobalEnchantment(ProjectTypes eProject, bool bAISaveUpMana)
{
	CvProjectInfo &kProject=GC.getProjectInfo(eProject);
	int iLoop;

	if(!kProject.isGlobalEnchantment())
	{
		return false;
	}

	if(kProject.getPrereqCivilization()!=NO_CIVILIZATION)
	{
		if(kProject.getPrereqCivilization()!=getCivilizationType())
		{
			return false;
		}
	}

	if(kProject.getProjectPrereq()!=NO_PROJECT)
	{
		if(GET_TEAM(getTeam()).getProjectCount((ProjectTypes)kProject.getProjectPrereq())<=0)
		{
			return false;
		}
	}

	if(kProject.getAnyoneProjectPrereq()!=NO_PROJECT)
	{
		bool bValid=false;
		for(int iI=0;iI<GC.getMAX_CIV_TEAMS();iI++)
		{
			if(GET_TEAM((TeamTypes)iI).isAlive() && GET_TEAM((TeamTypes)iI).getProjectCount((ProjectTypes)kProject.getAnyoneProjectPrereq())>0)
			{
				bValid=true;
				break;
			}
		}
		if(!bValid)
		{
			return false;
		}
	}

	if(GET_TEAM(getTeam()).getProjectCount(eProject)!=0)
	{
		return false;
	}

	if(!bAISaveUpMana)
	{
		if((kProject.getManaCost()<0)
			|| (kProject.getManaCost()!=0 && getManaIncome()==0))
		{
			return false;
		}

		if(kProject.getFaithCost()!=0 && kProject.getFaithCost()>getFaith())
		{
			return false;
		}
	}

	if(kProject.getCommerceChangeType()!=NO_COMMERCE)
	{
		if(kProject.getCommerceChangeType()==COMMERCE_RESEARCH)
		{
			if(getCurrentResearch()==NO_TECH)
			{				
				return false;
			}
		}
		if(kProject.getCommerceChangeType()==COMMERCE_FAITH)
		{
			if(getStateReligion()==NO_RELIGION)
			{
				return false;
			}
		}
	}

	if(kProject.getPrereqReligion()!=NO_RELIGION)
	{
		if(getStateReligion()!=kProject.getPrereqReligion())
		{
			return false;
		}
	}

	if(kProject.getPrereqAlignment()!=NO_ALIGNMENT)
	{
		if(getAlignment()!=kProject.getPrereqAlignment())
		{
			return false;
		}
	}

	if(kProject.getBlockedAlignment()!=NO_ALIGNMENT)
	{
		if(getAlignment()==kProject.getBlockedAlignment())
		{
			return false;
		}
	}

	if(kProject.getPrereqGlobalCounter() > GC.getGameINLINE().getGlobalCounter())
	{
		return false;
	}

	if(kProject.getDevilSummon() != NO_UNIT)
	{
		if(GET_TEAM(getTeam()).isAtWar((TeamTypes)GC.getDEVIL_TEAM()) || GET_PLAYER((PlayerTypes)GC.getDEVIL_PLAYER()).getNumUnits()==0)
		{
			return false;
		}
	}

	if(kProject.getDisjunctionPower() > 0) {
		bool bValid = false;
		for(int i = 0; i < GC.getMAX_CIV_TEAMS(); ++i) {
			CvTeam& kTeam = GET_TEAM((TeamTypes)i);
			if(kTeam.isAlive() && i != getTeam() && kTeam.isAtWar(getTeam())) {
				for(int j = 0; j < GC.getNumProjectInfos(); ++j) {
					if(GC.getProjectInfo((ProjectTypes)j).isGlobalEnchantment() 
						&& kTeam.getProjectCount((ProjectTypes)j) > 0) {
						bValid = true;
						break;
					}
				}
			}
		}
		if(!bValid)
			return false;
	}

	if(kProject.isRessurection()) {

		UnitTypes eUnit = (UnitTypes)GC.getCivilizationInfo(getCivilizationType()).getHero();

		if(eUnit == NO_UNIT)
			return false;

		UnitClassTypes eHeroClass=(UnitClassTypes)GC.getUnitInfo(eUnit).getUnitClassType();
		if(!GC.getGame().isUnitClassMaxedOut(eHeroClass))
			return false;

		bool bValid = true;
		for(int iI = 0; iI < GC.getMAX_PLAYERS(); iI++) {
			if(GET_PLAYER((PlayerTypes)iI).getUnitClassCount(eHeroClass) > 0) {
				bValid = false;
				break;
			}
		}
		if(!bValid)
			return false;
	}

	PromotionTypes ePromotion=(PromotionTypes)kProject.getPromotionType();
	if(kProject.getPopulationToExperienceRate() != 0 && ePromotion != NO_PROMOTION)
	{
		int iUnitCount = 0;
		for (CvUnit* pUnit = firstUnit(&iLoop); NULL != pUnit; pUnit = nextUnit(&iLoop))
		{
			if(pUnit->isHasPromotion(ePromotion))
			{
				iUnitCount++;
			}
		}

		if(iUnitCount == 0)
		{
			return false;
		}
	}		

	if(kProject.getCreateUnitFromExperience()>0 && kProject.getUnitType()!=NO_UNIT)
	{
		if(kProject.getCreateUnitFromExperience()*CalculateCivCounter(0,0)<100*CalculateCivCounter(1,0))
		{
			return false;
		}
	}

	if(kProject.getTechPrereq()!=NO_TECH)
	{
		if(!GET_TEAM(getTeam()).isHasTech((TechTypes)kProject.getTechPrereq()))
		{
			return false;
		}
	}

	if(kProject.isHostile())
	{
		bool bValid = false;
		for(int iI = 0; iI < GC.getMAX_CIV_PLAYERS(); iI++)
		{
			if(GET_PLAYER((PlayerTypes)iI).isAlive() && GET_TEAM(getTeam()).isAtWar(GET_PLAYER((PlayerTypes)iI).getTeam()))
			{
				bValid = true;
				break;
			}
		}
		if(!bValid)
		{
			return false;
		}
	}

	//Darksavant
	//Player must have holy city, the associated holy city wonder for their faith, and a matching state religion to cast Divine Judgement
	if (kProject.isDivineJudgement()) {
		if (getStateReligion() == (ReligionTypes)GC.getInfoTypeForString("RELIGION_THE_ASHEN_VEIL"))
		{
			if ((getBuildingClassCount((BuildingClassTypes)GC.getInfoTypeForString("BUILDINGCLASS_STIGMATA_ON_THE_UNBORN")) == 1) && isHasTech((TechTypes)GC.getInfoTypeForString("TECH_INFERNAL_POSSESION")))
			{
				return true;
			}
			return false;
		}
		else if (getStateReligion() == (ReligionTypes)GC.getInfoTypeForString("RELIGION_THE_ORDER"))
		{
			if ((getBuildingClassCount((BuildingClassTypes)GC.getInfoTypeForString("BUILDINGCLASS_CODE_OF_JUNIL")) == 1) && isHasTech((TechTypes)GC.getInfoTypeForString("TECH_JUDGEMENT_OF_JUNIL")))
			{
				return true;
			}
			return false;
		}
		else if (getStateReligion() == (ReligionTypes)GC.getInfoTypeForString("RELIGION_FELLOWSHIP_OF_LEAVES"))
		{
			if ((getBuildingClassCount((BuildingClassTypes)GC.getInfoTypeForString("BUILDINGCLASS_SONG_OF_AUTUMN")) == 1) && isHasTech((TechTypes)GC.getInfoTypeForString("TECH_HIDDEN_PATHS")))
			{
				return true;
			}
			return false;
		}
		else if (getStateReligion() == (ReligionTypes)GC.getInfoTypeForString("RELIGION_RUNES_OF_KILMORPH"))
		{
			if ((getBuildingClassCount((BuildingClassTypes)GC.getInfoTypeForString("BUILDINGCLASS_TABLETS_OF_BAMBUR")) == 1) && isHasTech((TechTypes)GC.getInfoTypeForString("TECH_TRADITION_OF_THE_GODDESS")))
			{
				return true;
			}
			return false;
		}
		else if (getStateReligion() == (ReligionTypes)GC.getInfoTypeForString("RELIGION_OCTOPUS_OVERLORDS"))
		{
			if ((getBuildingClassCount((BuildingClassTypes)GC.getInfoTypeForString("BUILDINGCLASS_THE_NECRONOMICON")) == 1) && isHasTech((TechTypes)GC.getInfoTypeForString("TECH_COMPLACENCY_OO")))
			{
				return true;
			}
			return false;
		}
		else if (getStateReligion() == (ReligionTypes)GC.getInfoTypeForString("RELIGION_THE_EMPYREAN"))
		{
			if ((getBuildingClassCount((BuildingClassTypes)GC.getInfoTypeForString("BUILDINGCLASS_DIES_DIEI")) == 1) && isHasTech((TechTypes)GC.getInfoTypeForString("TECH_DEVOTION_TO_LUGUS")))
			{
				return true;
			}
			return false;
		}
		else if (getStateReligion() == (ReligionTypes)GC.getInfoTypeForString("RELIGION_THE_WHITE_HAND"))
		{
			if ((getBuildingClassCount((BuildingClassTypes)GC.getInfoTypeForString("BUILDINGCLASS_THE_HEART_OF_WINTER")) == 1) && isHasTech((TechTypes)GC.getInfoTypeForString("TECH_MULCARN_REBORN")))
			{
				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}

	return true;
}

void CvPlayer::DoGlobalEnchantment(ProjectTypes eProject)
{
	if(!GC.getProjectInfo(eProject).isGlobalEnchantment())
		return;

	changeFaith(-GC.getProjectInfo(eProject).getFaithCost());

	ApplyGlobalEnchantment(eProject);

    for(int iAdvLoop=0;iAdvLoop<GC.getNumAdventureInfos();iAdvLoop++)
    {
        if(isAdventureEnabled(iAdvLoop) && !isAdventureFinished(iAdvLoop))
        {
            CvAdventureInfo &kAdv = GC.getAdventureInfo((AdventureTypes)iAdvLoop);
            for(int iJ=0;iJ<GC.getMAX_ADVENTURESTEPS();iJ++)
            {
                if(kAdv.getAdventureStep(iJ)!=NO_ADVENTURESTEP)
                {                    
                    if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).isGlobalEnchantment())
                    {
						if(GC.getAdventureStepInfo((AdventureStepTypes)kAdv.getAdventureStep(iJ)).getProjectType()==eProject)
						{
                            changeAdventureCounter(iAdvLoop,iJ,1);
						}
					}
				}
			}
		}
	}
}

void CvPlayer::doSummonRitual(ProjectTypes eProject)
{
	CvWString szBuffer;

	if(GC.getProjectInfo(eProject).getSummonUnitType()==NO_UNIT || getSummonCity() == NULL)
	{
		return;
	}

	changeFaith(-GC.getProjectInfo(eProject).getFaithCost());

	CvUnit* pNewUnit=initUnit((UnitTypes)GC.getProjectInfo(eProject).getSummonUnitType(),getSummonCity()->getX(),getSummonCity()->getY());

	pNewUnit->changeExperience(5 + getIncreaseSummonXP());

	szBuffer = gDLL->getText("TXT_KEY_UNIT_SUMMONED", GC.getUnitInfo((UnitTypes)GC.getProjectInfo(eProject).getSummonUnitType()).getDescription());
	gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_WELOVEKING", MESSAGE_TYPE_MINOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_HAPPY_PERSON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), -1, -1, true, true);
}

bool CvPlayer::canDoTerraformRitual(ProjectTypes eProject, bool bAISaveUpMana)
{
	CvProjectInfo &kProject=GC.getProjectInfo(eProject);

	//this is necessary for interface hack
	if(kProject.isTargetedTerraform())
	{
		//this is necessary for interface hack
		if(getNumUnits()==0)
		{
			return false;
		}

		if(getMana() < getSpecificMagicRitualCost(eProject)) {
			return false;
		}

		if(kProject.getFaithCost() > 0)
			return false;
	}
	
	if(kProject.getTechPrereq() != NO_TECH)
	{
		if(!GET_TEAM(getTeam()).isHasTech((TechTypes)kProject.getTechPrereq()))
		{
			return false;
		}
	}

	if(kProject.getNumTerrainChanges()<=0)
	{
		return false;
	}

	if(!bAISaveUpMana)
	{
		if(kProject.getManaCost()==-1)
		{
			return false;
		}

		if(getManaIncome()==0)
		{
			return false;
		}

		if(kProject.getFaithCost()>getFaith())
		{
			return false;
		}
	}

	if(kProject.isHostileTerraform())
	{
		bool bValid=false;
		for(int iI=0;iI<GC.getMAX_CIV_PLAYERS();iI++)
		{
			if(GET_PLAYER((PlayerTypes)iI).isAlive() && GET_TEAM(getTeam()).isAtWar(GET_PLAYER((PlayerTypes)iI).getTeam()))
			{
				bValid=true;
				break;
			}
		}
		if(!bValid)
		{
			return false;
		}
	}

	CvPlot* pLoopPlot;

	if(!kProject.isTargetedTerraform())
	{
		// returns true if first valid plot found
		for(int iI=0;iI<GC.getMapINLINE().numPlotsINLINE();iI++)
		{
			pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);
			if(pLoopPlot->isOwned())
			{
				if((!kProject.isHostileTerraform() && pLoopPlot->getOwnerINLINE()==getID())
					|| (kProject.isHostileTerraform() && GET_TEAM(getTeam()).isAtWar(pLoopPlot->getTeam())))
				{
					if((!pLoopPlot->isWater() && !kProject.isWater())
						||(pLoopPlot->isWater() && kProject.isWater()))
					{
						TerrainTypes plotTerrain = pLoopPlot->getTerrainType();
						FeatureTypes plotFeature = pLoopPlot->getFeatureType();
						ImprovementTypes plotImprovement = pLoopPlot->getImprovementType();

						if(plotTerrain == kProject.getTerrainType1())
							return true;
						if(plotTerrain == kProject.getTerrainType2())
							return true;
						if(plotTerrain == kProject.getTerrainType3())
							return true;
						if(plotTerrain == kProject.getTerrainType4())
							return true;
						if(plotTerrain == kProject.getTerrainType5())
							return true;
						
						if(pLoopPlot->isPeak() && kProject.isRemovePeak())
							return true;

						if(pLoopPlot->getPlotCounter()>0 && kProject.isRemoveHellTerrain())
							return true;

						if(pLoopPlot->getPlotCounter()==0 && kProject.isCreateHellTerrain())
							return true;

						if(!pLoopPlot->isPeak() && kProject.isCreateRiver())
							return true;

						if(pLoopPlot->isWater() && pLoopPlot->getNumUnits()==0 && kProject.isCreateLand())
							return true;

						if(pLoopPlot->getPlotType()==PLOT_LAND && pLoopPlot->getNumUnits()==0 && !pLoopPlot->isCity() && kProject.isDrownLand())
							return true;

						if(pLoopPlot->isRiverSide() && plotFeature==NO_FEATURE && plotTerrain==GC.getInfoTypeForString("TERRAIN_DESERT")  && !pLoopPlot->isPeak() && kProject.isCreateFloodPlains())
							return true;

						if(kProject.getFeatureTypePre()!=NO_FEATURE || kProject.getFeatureTypePost()!=NO_FEATURE)
						{
							if(!pLoopPlot->isPeak() && kProject.getFeatureTypePre()==plotFeature)
								return true;
						}
						if(kProject.getFeatureType2Pre()!=NO_FEATURE || kProject.getFeatureType2Post()!=NO_FEATURE)
						{
							if(!pLoopPlot->isPeak() && kProject.getFeatureType2Pre()==plotFeature)
								return true;
						}
	
						if(kProject.getImprovementTypePre()!=NO_IMPROVEMENT || kProject.getImprovementTypePost()!=NO_IMPROVEMENT)
						{
							if(kProject.getImprovementTypePre() == NO_IMPROVEMENT || kProject.getImprovementTypePre() == plotImprovement)
								return true;
						}
						if(kProject.getImprovementType2Pre() != NO_IMPROVEMENT || kProject.getImprovementType2Post()!=NO_IMPROVEMENT)
						{
							if(kProject.getImprovementType2Pre() == NO_IMPROVEMENT || kProject.getImprovementType2Pre() == plotImprovement)
								return true;
						}
					}
				}
			}
		}
	}
	else // targeted terraforming
	{
		return true;
	}
	// no valid plot found
	return false;
}

int CvPlayer::getSpecificMagicRitualCost(ProjectTypes eProject)
{
	CvProjectInfo& kProject = GC.getProjectInfo(eProject);

	int iProjectManaCost = kProject.getManaCost();

	if(kProject.getNumTerrainChanges()!=0)
	{
		iProjectManaCost = getTerraformManaCost(eProject);
	}
	else if(kProject.isGlobalEnchantment())
	{
		iProjectManaCost = getGlobalEnchantmentManaCost(eProject);
	}
	else if(kProject.getSummonUnitType()!=NO_UNIT)
	{
		iProjectManaCost = getSummonManaCost(eProject);
	}

	int iMod = 100;

	iMod -= GC.getCivilizationInfo(getCivilizationType()).getReducedManaCostInGoldenAge();
	iMod = std::max(50, iMod);

	return iProjectManaCost;
}

int CvPlayer::getTerraformManaCost(ProjectTypes eProject)
{
	int iBaseManaCost=GC.getProjectInfo(eProject).getManaCost();
	int iMod=100;

	ManaschoolTypes iManaschool=NO_MANASCHOOL;
	if(GC.getProjectInfo(eProject).getTechPrereq()!=NO_TECH)
	{
		iManaschool=(ManaschoolTypes)GC.getTechInfo((TechTypes)GC.getProjectInfo(eProject).getTechPrereq()).getSpellSchool();
	}

	if(iManaschool!=NO_MANASCHOOL)
	{
		if(GC.getManaschoolInfo(iManaschool).getBonusReducedTerraformCost()!=NO_BONUS)
		{
			iMod-=10*getNumAvailableBonuses((BonusTypes)GC.getManaschoolInfo(iManaschool).getBonusReducedTerraformCost());
		}

		iMod+=getManaSchoolCostModifier(iManaschool);
	}

	iMod-=getReducedTerraformCost();

	iMod=std::max(iMod,50);

	return (iBaseManaCost*iMod)/100;
}

int CvPlayer::getGlobalEnchantmentManaCost(ProjectTypes eProject)
{
	int iBaseManaCost=GC.getProjectInfo(eProject).getManaCost();
	int iMod=100;

	ManaschoolTypes iManaschool=NO_MANASCHOOL;
	if(GC.getProjectInfo(eProject).getTechPrereq()!=NO_TECH)
	{
		iManaschool=(ManaschoolTypes)GC.getTechInfo((TechTypes)GC.getProjectInfo(eProject).getTechPrereq()).getSpellSchool();
	}

	if(iManaschool!=NO_MANASCHOOL)
	{
		if(GC.getManaschoolInfo(iManaschool).getBonusReducedGlobalEnchantment()!=NO_BONUS)
		{
			iMod-=10*getNumAvailableBonuses((BonusTypes)GC.getManaschoolInfo(iManaschool).getBonusReducedGlobalEnchantment());
		}

		iMod+=getManaSchoolCostModifier(iManaschool);
	}

	iMod-=getReducedTerraformCost();

	iMod=std::max(iMod,50);

	return (iBaseManaCost*iMod)/100;
}

int CvPlayer::getSummonManaCost(ProjectTypes eProject)
{
	int iBaseManaCost = GC.getProjectInfo(eProject).getManaCost();
	int iMod = 100;

	iMod -= getReducedSummonCost();
	iMod -= getSummonCostReduction();

	iMod = std::max(iMod,50);

	return (iBaseManaCost*iMod)/100;
}

void CvPlayer::ApplyGlobalEnchantment(ProjectTypes eProject, TeamTypes eTeamDeclaredWar, PlayerTypes ePlayerAddedTeam)
{
	int iI,iJ,iK;
	int iLoop;
	CvProjectInfo &kProject=GC.getProjectInfo(eProject);
	CvWString szBuffer;

	if (!kProject.isEffectOnly())
	{
		if(ePlayerAddedTeam==NO_PLAYER && eTeamDeclaredWar==NO_TEAM)
		{
			GET_TEAM(getTeam()).changeProjectCount(eProject,1);
		}
	}

	if(kProject.getDisjunctionPower()>0)
	{
		for(iK = 0; iK < MAX_CIV_TEAMS; iK++)
		{
			CvTeam &kTeam =GET_TEAM((TeamTypes)iK);
			if(kTeam.isAlive() && iK!=getTeam() && kTeam.isAtWar((TeamTypes)iK))
			{
				for(int iL = 0; iL < GC.getNumProjectInfos(); iL++)
				{
					CvProjectInfo &kProject = GC.getProjectInfo((ProjectTypes)iL);
					if(kProject.isGlobalEnchantment())
					{
						if(kTeam.getProjectCount((ProjectTypes)iL)>0)
						{
							if(GC.getGameINLINE().getSorenRandNum(100,"Disjunction")<10+kProject.getDisjunctionPower())
							{
								kTeam.changeProjectCount((ProjectTypes)iL,-1);
							}
						}
					}
				}
			}
		}
	}


	if(kProject.isCallTheVoid())
	{
		CvCity* pBestCity=NULL;
		CvCity* pLoopCity;
		int iValue, iBestValue=0;

		for(iI = 0; iI < MAX_CIV_PLAYERS; iI++)
		{
			CvPlayer &kPlayer =GET_PLAYER((PlayerTypes)iI);
			if(kPlayer.isAlive())
			{
				if(kPlayer.getTeam() && GET_TEAM(kPlayer.getTeam()).isAtWar(getTeam()))
				{
					int iLoop;
					for (pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
					{
						iValue=100+pLoopCity->getNumBuildings()*40+pLoopCity->getPopulation()*10;
						if(iValue>iBestValue)
						{
							iBestValue=iValue;
							pBestCity=pLoopCity;
						}
					}
				}
			}
		}

		if(pBestCity!=NULL)
		{
			int iCounter=0;
			for(iJ=0;iJ<GC.getNumBuildingInfos();iJ++)
			{
				if (pBestCity->getNumRealBuilding((BuildingTypes)iJ)==1)
				{
					if(GC.getBuildingClassInfo((BuildingClassTypes)GC.getBuildingInfo((BuildingTypes)iJ).getBuildingClassType()).getMaxPlayerInstances()==-1)
					{
						if(GC.getGameINLINE().getSorenRandNum(100,"Call the Void")<10)
						{
							pBestCity->setNumRealBuilding((BuildingTypes)iJ,0);
							iCounter++;
						}
					}
				}
			}
			if(iCounter>0)
			{
				szBuffer = gDLL->getText("TXT_KEY_PROJECT_CALL_THE_VOID", pBestCity->getName(),iCounter);
				gDLL->getInterfaceIFace()->addMessage(pBestCity->getOwnerINLINE(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DEAL_CANCELLED", MESSAGE_TYPE_MINOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_HAPPY_PERSON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), -1, -1, true, true);
				gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DEAL_CANCELLED", MESSAGE_TYPE_MINOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_HAPPY_PERSON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), -1, -1, true, true);
			}
		}
	}

	if(kProject.isRessurection())
	{
		UnitTypes eUnit=(UnitTypes)GC.getCivilizationInfo(getCivilizationType()).getHero();

		if(eUnit!=NO_UNIT)
		{
			UnitClassTypes eHeroClass=(UnitClassTypes)GC.getUnitInfo(eUnit).getUnitClassType();
			if(eHeroClass!=NO_UNITCLASS && GC.getGame().isUnitClassMaxedOut(eHeroClass))
			{
				bool bValid=true;
				for(int iI=0;iI<GC.getMAX_PLAYERS();iI++)
				{
					if(GET_PLAYER((PlayerTypes)iI).getUnitClassCount(eHeroClass)>0)
					{
						bValid=false;
						break;
					}
				}

				if(bValid && getCapitalCity()!=NULL)
				{
					CvUnit* pUnit = initUnit(eUnit,getCapitalCity()->getX(),getCapitalCity()->getY());
					gDLL->getInterfaceIFace()->addMessage(getID(), true, GC.getEVENT_MESSAGE_TIME(), gDLL->getText("TXT_KEY_MESSAGE_HERO_RESSURECTED"), "AS2D_CHARM_PERSON", MESSAGE_TYPE_MINOR_EVENT, ", ,Art/Interface/Buttons/Spells/Spells_Atlas1.dds,7,8", (ColorTypes)8, getCapitalCity()->getX(), getCapitalCity()->getY(), true, true);
					for(int iI=0;iI<GC.getNumPromotionInfos();iI++)
					{
						if(pUnit->isHasPromotion((PromotionTypes)iI))
						{
							if(GC.getPromotionInfo((PromotionTypes)iI).isEquipment())
							{
								pUnit->setHasPromotion((PromotionTypes)iI,false);
							}
						}
					}
				}
			}
		}
	}

	if(kProject.isChaosRift())
	{
		CvCity* pBestCity=NULL;
		CvCity* pLoopCity;
		int iValue, iBestValue=0;

		for(iI = 0; iI < MAX_CIV_PLAYERS; iI++)
		{
			int iLoop;
			for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
			{
				iValue=GC.getGameINLINE().getSorenRandNum(100,"Chaos Rift")-pLoopCity->getNumBuildings()*10;
				if(iValue>iBestValue)
				{
					iBestValue=iValue;
					pBestCity=pLoopCity;
				}
			}
		}

		if(pBestCity!=NULL)
		{
			int iCounter=0;
			for(iJ=0;iJ<10;iJ++)
			{
				BuildingTypes eRandBuild=(BuildingTypes)GC.getGameINLINE().getSorenRandNum(GC.getNumBuildingInfos(),"Chaos Rift");
				if (pBestCity->getNumRealBuilding(eRandBuild)==0)
				{
					if((GC.getBuildingClassInfo((BuildingClassTypes)GC.getBuildingInfo(eRandBuild).getBuildingClassType()).getMaxPlayerInstances()==-1)
						&&(GC.getBuildingClassInfo((BuildingClassTypes)GC.getBuildingInfo(eRandBuild).getBuildingClassType()).getMaxTeamInstances()==-1)
						&&(GC.getBuildingClassInfo((BuildingClassTypes)GC.getBuildingInfo(eRandBuild).getBuildingClassType()).getMaxGlobalInstances()==-1)
						&&(GC.getBuildingInfo(eRandBuild).getProductionCost()>0))
					{
						pBestCity->setNumRealBuilding(eRandBuild,1);
						iCounter++;
						if(iCounter>2)
						{
							break;
						}
					}
				}
			}
			if(iCounter>0)
			{
				szBuffer = gDLL->getText("TXT_KEY_PROJECT_CHAOS_RIFT", iCounter, pBestCity->getNameKey());
				gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_WELOVEKING", MESSAGE_TYPE_MINOR_EVENT, NULL ,(ColorTypes)GC.getInfoTypeForString("COLOR_GREEN"), pBestCity->getX_INLINE(), pBestCity->getY_INLINE(), true, true);
			}
		}
		else
		{
			szBuffer = gDLL->getText("TXT_KEY_PROJECT_CHAOS_RIFT_FAILED");
			gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, NULL, MESSAGE_TYPE_MINOR_EVENT, NULL ,(ColorTypes)GC.getInfoTypeForString("COLOR_RED"), NULL, NULL, false, false);
		}
	}

	if(kProject.isEarthQuake())
	{
		CvCity* pBestCity=NULL;
		CvCity* pLoopCity;
		int iValue, iBestValue=0;

		for(iI = 0; iI < MAX_CIV_PLAYERS; iI++)
		{
			CvPlayer &kPlayer =GET_PLAYER((PlayerTypes)iI);
			if(kPlayer.isAlive())
			{
				if(kPlayer.getTeam() && GET_TEAM(kPlayer.getTeam()).isAtWar(getTeam()))
				{
					int iLoop;
					for (pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
					{
						iValue=100+pLoopCity->getNumBuildings()*40+pLoopCity->getPopulation()*10;
						if(iValue>iBestValue)
						{
							iBestValue=iValue;
							pBestCity=pLoopCity;
						}
					}
				}
			}
		}

		if(pBestCity!=NULL)
		{
			int iCounter=0;
			for(iJ=0;iJ<GC.getNumBuildingInfos();iJ++)
			{
				if (pBestCity->getNumRealBuilding((BuildingTypes)iJ)==1)
				{
					if(GC.getBuildingClassInfo((BuildingClassTypes)GC.getBuildingInfo((BuildingTypes)iJ).getBuildingClassType()).getMaxPlayerInstances()==-1)
					{
						if(GC.getGameINLINE().getSorenRandNum(100,"Earth Quake")<7)
						{
							pBestCity->setNumRealBuilding((BuildingTypes)iJ,0);
							iCounter++;
						}
					}
				}
			}
			if(iCounter>0)
			{
				szBuffer = gDLL->getText("TXT_KEY_PROJECT_CALL_THE_VOID", pBestCity->getName(),iCounter);
				gDLL->getInterfaceIFace()->addMessage(pBestCity->getOwnerINLINE(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DEAL_CANCELLED", MESSAGE_TYPE_MINOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_HAPPY_PERSON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), -1, -1, true, true);
				gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DEAL_CANCELLED", MESSAGE_TYPE_MINOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_HAPPY_PERSON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), -1, -1, true, true);
			}
		}
	}

	if(kProject.isNatureWrath())
	{
		GET_TEAM(getTeam()).makePeace((TeamTypes)GC.getANIMAL_TEAM(),false);
	}

	if(kProject.getDevilSummon()!=NO_UNIT)
	{
		if(GET_PLAYER((PlayerTypes)GC.getDEVIL_PLAYER()).isAlive())
		{
			ImprovementTypes iDevilPortal=(ImprovementTypes)GC.getInfoTypeForString("IMPROVEMENT_DEVIL_PORTAL");

			for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
			{
				CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

				if(pLoopPlot!=NULL && pLoopPlot->getImprovementType()==iDevilPortal)
				{
					if(!pLoopPlot->isVisibleEnemyUnit((PlayerTypes)GC.getDEVIL_PLAYER()))
					{
						CvUnit* pNewUnit = GET_PLAYER((PlayerTypes)GC.getDEVIL_PLAYER()).initUnit((UnitTypes)kProject.getDevilSummon(),pLoopPlot->getX_INLINE(),pLoopPlot->getY_INLINE());
						pNewUnit->setOriginPlot(pLoopPlot);
					}
				}
			}
		}
	}

	if(kProject.getCommerceChangeType()!=NO_COMMERCE)
	{
		int iChange=kProject.getCommerceChange();
		if(kProject.getCommerceChangeType()==COMMERCE_GOLD)
		{
			changeGold(iChange);
		}
		else if(kProject.getCommerceChangeType()==COMMERCE_RESEARCH)
		{
			if(getCurrentResearch()!=NO_TECH)
			{				
				GET_TEAM(getTeam()).changeResearchProgress(getCurrentResearch(),iChange,getID());
			}
		}
		else if(kProject.getCommerceChangeType()==COMMERCE_FAITH)
		{
			if(getStateReligion()!=NO_RELIGION)
			{
				changeFaith(iChange);
			}
		}
		else if(kProject.getCommerceChangeType()==COMMERCE_MANA)
		{
			changeMana(iChange);
		}
	}

	if(kProject.getYieldChangeType()!=NO_YIELD)
	{
		int iYieldChange=kProject.getYieldChange();
		if(kProject.getManaBonusLinked()!=NO_BONUS)
		{
			iYieldChange+=kProject.getBonusPerMana()*getNumAvailableBonuses(kProject.getManaBonusLinked());
		}
		changeGlobalYield(kProject.getYieldChangeType(),iYieldChange);
	}

	if(ePlayerAddedTeam==NO_PLAYER && eTeamDeclaredWar==NO_TEAM)
	{
		if(kProject.getAwarenessLevel()==2)
		{
			for (iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
			{
				GC.getMapINLINE().plotByIndexINLINE(iI)->setRevealed(getTeam(), true, false, NO_TEAM, false);
			}
		}

		if(kProject.getAwarenessLevel()==3)
		{
			GET_TEAM(getTeam()).changeSeeEverything(+1);
		}
	}

	PromotionTypes ePromotion=(PromotionTypes)kProject.getPromotionType();
	if(kProject.getPopulationToExperienceRate()!=0 && ePromotion!=NO_PROMOTION)
	{
		int iUnitCount=0;
		for (CvUnit* pUnit = firstUnit(&iLoop); NULL != pUnit; pUnit = nextUnit(&iLoop))
		{
			if(pUnit->isHasPromotion(ePromotion))
			{
				iUnitCount++;
			}
		}

		if(iUnitCount>0)
		{
			int iPopulationSacrified=0;

			for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
			{
				if(pLoopCity->getPopulation()>1)
				{
					pLoopCity->changePopulation(-1);
					iPopulationSacrified++;
				}
			}

			int iExpPerUnit=std::max(5,iPopulationSacrified*kProject.getPopulationToExperienceRate()/std::max(iUnitCount,1));

			for (CvUnit* pUnit = firstUnit(&iLoop); NULL != pUnit; pUnit = nextUnit(&iLoop))
			{
				if(pUnit->isHasPromotion(ePromotion))
				{
					pUnit->changeExperience(iExpPerUnit);
				}
			}
		}
	}

	if (!CvString(kProject.getPyResult()).empty())
	{	
		CyArgsList argsList;
		argsList.add(getID());
		argsList.add(eProject);
		gDLL->getPythonIFace()->callFunction(PYGEModule, "globalenchantment", argsList.makeFunctionArgs()); //, &lResult
	}


	if(kProject.getCreateUnitFromExperience()>0 && kProject.getUnitType()!=NO_UNIT)
	{
		int iXPCount=0;
		int iXPReduced=5;
		for (CvUnit* pUnit = firstUnit(&iLoop); NULL != pUnit; pUnit = nextUnit(&iLoop))
		{
			if(pUnit->getExperience()>20 && GC.getUnitInfo(pUnit->getUnitType()).getTier()>1)
			{
				pUnit->changeExperience(-iXPReduced);
				iXPCount+=iXPReduced;
			}
		}

		int iUnitsCreated=std::max(1,iXPCount/std::max(1,kProject.getCreateUnitFromExperience()));

		for(int iI=0;iI<iUnitsCreated;iI++)
		{
			initUnit((UnitTypes)kProject.getUnitType(),getCapitalCity()->getX_INLINE(),getCapitalCity()->getY_INLINE());
		}
	}

	if(!kProject.isHostile())
	{
		for(iI = 0; iI < MAX_CIV_PLAYERS; iI++)
		{
			CvPlayer &kPlayer =GET_PLAYER((PlayerTypes)iI);
			if(kPlayer.isAlive())
			{
				if(iI==ePlayerAddedTeam || (ePlayerAddedTeam==NO_PLAYER && (kPlayer.getTeam()==getTeam())))
				{
					if(kProject.getResistHostileTerraforming()!=0)
					{
						kPlayer.changeResistHostileTerraforming(kProject.getResistHostileTerraforming());
					}

					if(kProject.getTemporaryTech()!=NO_TECH)
					{
						GET_TEAM(kPlayer.getTeam()).setHasTech((TechTypes)kProject.getTemporaryTech(),true,NO_PLAYER,false,false);
					}

					if(kProject.getPromotionType()!=NO_PROMOTION && kProject.isAddPromotionToUnits())
					{
						for (CvUnit* pUnit = kPlayer.firstUnit(&iLoop); NULL != pUnit; pUnit = kPlayer.nextUnit(&iLoop))
						{
							bool bValid=false;
							if(GC.getPromotionInfo((PromotionTypes)kProject.getPromotionType()).isAnyUnitCombat())
							{
								bValid=true;
							}
							else if(GC.getUnitInfo(pUnit->getUnitType()).getUnitCombatType()!=NO_UNITCOMBAT)
							{												
								if(GC.getPromotionInfo((PromotionTypes)kProject.getPromotionType()).getUnitCombat(GC.getUnitInfo(pUnit->getUnitType()).getUnitCombatType()))
								{
									bValid=true;
								}
							}
							if (bValid)
							{
								pUnit->setHasPromotion((PromotionTypes)kProject.getPromotionType(),true);
							}
						}
					}

					if(kProject.getBuildingType()!=NO_BUILDING && kProject.isAddBuildingToCities())
					{
						for (CvCity* pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
						{
							if(pLoopCity->canConstruct((BuildingTypes)kProject.getBuildingType(), false, false, true))
							{
								pLoopCity->setNumRealBuilding((BuildingTypes)kProject.getBuildingType(),1);
							}
						}
					}
				}
			}
		}
	}

	if(kProject.isHostile())
	{
		for(iI = 0; iI < MAX_CIV_PLAYERS; iI++)
		{
			CvPlayer &kPlayer =GET_PLAYER((PlayerTypes)iI);
			if(kPlayer.isAlive())
			{
				if(kPlayer.getTeam()!=getTeam())
				{
					if(iI==ePlayerAddedTeam || (ePlayerAddedTeam==NO_PLAYER && (kPlayer.getTeam()==eTeamDeclaredWar || (eTeamDeclaredWar==NO_TEAM && GET_TEAM(kPlayer.getTeam()).isAtWar(getTeam())))))
					{
						if(kProject.getReduceManaCreation()!=0)
						{
							kPlayer.changeManaCreationModifier(-kProject.getReduceManaCreation());
						}

						if(kProject.getTemporaryTech()!=NO_TECH)
						{
							GET_TEAM(kPlayer.getTeam()).setHasTech((TechTypes)kProject.getTemporaryTech(),true,NO_PLAYER,false,false);
						}

						if(kProject.getDoubleCostOfOtherManaSchools()!=NO_MANASCHOOL)
						{
							for(iJ=0;iI<GC.getNumManaschoolInfos();iJ++)
							{
								if(iJ!=kProject.getDoubleCostOfOtherManaSchools())
								{
									kPlayer.changeManaSchoolCostModifier(iJ,100);
								}
							}
						}

						if(kProject.getPromotionType()!=NO_PROMOTION && kProject.isAddPromotionToUnits())
						{
							for (CvUnit* pUnit = kPlayer.firstUnit(&iLoop); NULL != pUnit; pUnit = kPlayer.nextUnit(&iLoop))
							{
								pUnit->setHasPromotion((PromotionTypes)kProject.getPromotionType(),true);
							}
						}

						if(kProject.getBuildingType()!=NO_BUILDING && kProject.isAddBuildingToCities())
						{
							for (CvCity* pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
							{
								pLoopCity->setNumRealBuilding((BuildingTypes)kProject.getBuildingType(),1);
							}
						}
					}
				}
			}
		}
	}
}

void CvPlayer::RemoveGlobalEnchantment(ProjectTypes eProject, TeamTypes eTeamDeclaredPeace)
{
	CvProjectInfo &kProject=GC.getProjectInfo(eProject);
	CvWString szBuffer;
	int iI, iJ;
	int iLoop;
	if(kProject.isHostile())
	{
		for(iI = 0; iI < MAX_CIV_PLAYERS; iI++)
		{
			CvPlayer &kPlayer =GET_PLAYER((PlayerTypes)iI);
			if(kPlayer.isAlive())
			{
				if(kPlayer.getTeam()!=getTeam())
				{
					if((GET_TEAM(kPlayer.getTeam()).isAtWar(getTeam()) && eTeamDeclaredPeace==NO_TEAM) || kPlayer.getTeam()==eTeamDeclaredPeace)
					{
						if(kProject.getReduceManaCreation()!=0)
						{
							kPlayer.changeManaCreationModifier(kProject.getReduceManaCreation());
						}

						if(kProject.getTemporaryTech()!=NO_TECH)
						{
							GET_TEAM(kPlayer.getTeam()).setHasTech((TechTypes)kProject.getTemporaryTech(),false,NO_PLAYER,false,false);
						}

						if(kProject.getDoubleCostOfOtherManaSchools()!=NO_MANASCHOOL)
						{
							for(iJ=0;iJ<GC.getNumManaschoolInfos();iJ++)
							{
								if(iJ!=kProject.getDoubleCostOfOtherManaSchools())
								{
									kPlayer.changeManaSchoolCostModifier(iJ,-100);
								}
							}
						}

						if(kProject.getPromotionType()!=NO_PROMOTION)
						{
							for (CvUnit* pUnit = kPlayer.firstUnit(&iLoop); NULL != pUnit; pUnit = kPlayer.nextUnit(&iLoop))
							{
								bool bValid=false;
								if(GC.getPromotionInfo((PromotionTypes)kProject.getPromotionType()).isAnyUnitCombat())
								{
									bValid=true;
								}
								else if(GC.getUnitInfo(pUnit->getUnitType()).getUnitCombatType()!=NO_UNITCOMBAT)
								{												
									if(GC.getPromotionInfo((PromotionTypes)kProject.getPromotionType()).getUnitCombat(GC.getUnitInfo(pUnit->getUnitType()).getUnitCombatType()))
									{
										bValid=true;
									}
								}
								if (bValid)
								{
									pUnit->setHasPromotion((PromotionTypes)kProject.getPromotionType(),false);
								}
							}
						}

						if(kProject.getBuildingType()!=NO_BUILDING && kProject.isAddBuildingToCities())
						{
							for (CvCity* pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
							{
								pLoopCity->setNumRealBuilding((BuildingTypes)kProject.getBuildingType(),0);
							}
						}

					}
				}
			}
		}
	}
	else if(eTeamDeclaredPeace==NO_TEAM)
	{
		if(kProject.isNatureWrath())
		{
			GET_TEAM((TeamTypes)GC.getANIMAL_TEAM()).declareWar(getTeam(),false,WARPLAN_TOTAL);
		}

		if(kProject.getAwarenessLevel()==3)
		{
			GET_TEAM(getTeam()).changeSeeEverything(-1);
		}

		for(iI = 0; iI < MAX_CIV_PLAYERS; iI++)
		{
			CvPlayer &kPlayer =GET_PLAYER((PlayerTypes)iI);
			if(kPlayer.isAlive() && kPlayer.getTeam()==getTeam())
			{
				if(kProject.getResistHostileTerraforming()!=0)
				{
					changeResistHostileTerraforming(-kProject.getResistHostileTerraforming());
				}

				if(kProject.getTemporaryTech()!=NO_TECH)
				{
					GET_TEAM(kPlayer.getTeam()).setHasTech((TechTypes)kProject.getTemporaryTech(),false,NO_PLAYER,false,false);
				}

				if(kProject.getPromotionType()!=NO_PROMOTION)
				{
					for (CvUnit* pUnit = kPlayer.firstUnit(&iLoop); NULL != pUnit; pUnit = kPlayer.nextUnit(&iLoop))
					{
						pUnit->setHasPromotion((PromotionTypes)kProject.getPromotionType(),false);
					}
				}

				if(kProject.getBuildingType()!=NO_BUILDING)
				{
					for (CvCity* pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
					{
						pLoopCity->setNumRealBuilding((BuildingTypes)kProject.getBuildingType(),0);
					}
				}
			}

			//Darksavant
			//Remove Divine Judgement buildings if project is canceled
			//If more than 1 DJ is ongoing this will remove other active ones too, but since they are added every round hopefully it will still work, and at worst should only remove their effects for 1 round
			if (kProject.isDivineJudgement() && kPlayer.isAlive())
			{

				//Resets the extra negative health and happiness on the DJ buildings
				if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_FELLOWSHIP_OF_LEAVES"))
				{
					kPlayer.setExtraBuildingHappiness((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_LEAVES"), 0);
					kPlayer.setExtraBuildingHealth((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_LEAVES"), 0);
				}
				else if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_THE_ORDER"))
				{
					kPlayer.setExtraBuildingHappiness((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_ORDER"), 0);
					kPlayer.setExtraBuildingHealth((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_ORDER"), 0);
				}
				else if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_OCTOPUS_OVERLORDS"))
				{
					kPlayer.setExtraBuildingHappiness((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_OVERLORDS"), 0);
					kPlayer.setExtraBuildingHealth((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_OVERLORDS"), 0);
				}
				else if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_RUNES_OF_KILMORPH"))
				{
					kPlayer.setExtraBuildingHappiness((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_KILMORPH"), 0);
					kPlayer.setExtraBuildingHealth((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_KILMORPH"), 0);
				}
				else if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_THE_ASHEN_VEIL"))
				{
					kPlayer.setExtraBuildingHappiness((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_VEIL"), 0);
					kPlayer.setExtraBuildingHealth((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_VEIL"), 0);
				}
				else if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_THE_EMPYREAN"))
				{
					kPlayer.setExtraBuildingHappiness((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_EMPYREAN"), 0);
					kPlayer.setExtraBuildingHealth((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_EMPYREAN"), 0);
				}
				else if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_THE_WHITE_HAND"))
				{
					kPlayer.setExtraBuildingHappiness((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_HAND"), 0);
					kPlayer.setExtraBuildingHealth((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_HAND"), 0);
				}

				//removes DJ building
				for (CvCity* pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
				{
					if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_FELLOWSHIP_OF_LEAVES"))
					{
						pLoopCity->setNumRealBuilding((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_LEAVES"), 0);
					}
					else if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_THE_ORDER"))
					{
						pLoopCity->setNumRealBuilding((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_ORDER"), 0);
					}
					else if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_OCTOPUS_OVERLORDS"))
					{
						pLoopCity->setNumRealBuilding((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_OVERLORDS"), 0);
					}
					else if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_RUNES_OF_KILMORPH"))
					{
						pLoopCity->setNumRealBuilding((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_KILMORPH"), 0);
					}
					else if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_THE_ASHEN_VEIL"))
					{
						pLoopCity->setNumRealBuilding((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_VEIL"), 0);
					}
					else if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_THE_EMPYREAN"))
					{
						pLoopCity->setNumRealBuilding((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_EMPYREAN"), 0);
					}
					else if (kProject.getPrereqReligion() == GC.getInfoTypeForString("RELIGION_THE_WHITE_HAND"))
					{
						pLoopCity->setNumRealBuilding((BuildingTypes)GC.getInfoTypeForString("BUILDING_DIVINE_JUDGEMENT_HAND"), 0);
					}
				}

				CvWString szBuffer;
				szBuffer.Format(L"The %s holy city has been taken!  The threat of Divine Judgement is over!", GC.getReligionInfo((ReligionTypes)kProject.getPrereqReligion()).getTextKeyWide());
				if (GC.getGameINLINE().getHolyCity((ReligionTypes)kProject.getPrereqReligion()) != NULL)
				{
					gDLL->getInterfaceIFace()->addMessage((PlayerTypes)iI, false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DEAL_CANCELLED", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_GREEN"), GC.getGameINLINE().getHolyCity((ReligionTypes)kProject.getPrereqReligion())->getX(), GC.getGameINLINE().getHolyCity((ReligionTypes)kProject.getPrereqReligion())->getY(), true, true);
				}
				else
				{
					gDLL->getInterfaceIFace()->addMessage((PlayerTypes)iI, false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DEAL_CANCELLED", MESSAGE_TYPE_MAJOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_GREEN"), NULL, NULL, true, true);
				}

			}
		}
	}

	if(eTeamDeclaredPeace==NO_TEAM)
	{
		szBuffer = gDLL->getText("TXT_KEY_GLOBAL_ENCHANTMENT_CANCELLED", kProject.getDescription());
		gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DEAL_CANCELLED", MESSAGE_TYPE_MINOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_HAPPY_PERSON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), -1, -1, true, true);

		GET_TEAM(getTeam()).changeProjectCount(eProject,-1);
	}

	//verify if another Enchantment must be cancelled
	for(int iI=0;iI<GC.getNumProjectInfos();iI++)
	{
		if(GC.getProjectInfo((ProjectTypes)iI).isGlobalEnchantment() && GC.getProjectInfo((ProjectTypes)iI).getProjectPrereq()!=NO_PROJECT)
		{
			if(GET_TEAM(getTeam()).getProjectCount((ProjectTypes)iI)>0)
			{
				if(GET_TEAM(getTeam()).getProjectCount((ProjectTypes)GC.getProjectInfo((ProjectTypes)iI).getProjectPrereq())<=0)
				{
					RemoveGlobalEnchantment((ProjectTypes)iI);
				}
			}
		}
	}
			
}

/**	New Esus                                                                 					**/
int CvPlayer::getPrestige() const
{
    return m_iPrestige;
}

void CvPlayer::changePrestige(int iChange)
{
	m_iPrestige += iChange;
}

/** Mana **/

int CvPlayer::getFaithUpkeep() const
{
    return m_iFaithUpkeep;
}
void CvPlayer::changeFaithUpkeep(int ichange)
{
    m_iFaithUpkeep+=ichange;
}

int CvPlayer::getManaUpkeepTotal() const
{
	int iBase = getManaUpkeep();

	iBase *= 100 + getSummonCostReduction();
	iBase /= 100;

	return iBase;
}

int CvPlayer::getManaUpkeep() const
{
    return m_iManaUpkeep;
}
void CvPlayer::changeManaUpkeep(int ichange)
{
	if(ichange!=0)
	{
		m_iManaUpkeep+=ichange;
		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(SelectionButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

int CvPlayer::getArcane() const
{
    return m_iArcane;
}

void CvPlayer::changeArcane(int change)
{
    m_iArcane+=change;
}

int CvPlayer::getArcaneLevel() const
{
    return m_iArcaneLevel;
}

int CvPlayer::getArcaneNeeded(int iTech) const
{
	int iAmount=GET_TEAM(getTeam()).getResearchCost((TechTypes)iTech);
    return iAmount;
}

int CvPlayer::calculateArcaneByLeftOverMana() const
{
	return (getMana() - getMaxMana()) / 2;
}

int CvPlayer::calculateArcaneByBuildings() const
{
	int iArcaneIncome=0;
	CvCity* pLoopCity;
	int iLoop;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iArcaneIncome+=pLoopCity->getCommerceRate(COMMERCE_ARCANE);
	}

	return iArcaneIncome;
}

int CvPlayer::calculateArcaneByManaSchool(ManaschoolTypes iManaSchool) const
{
	int iIncomeFromSchool=0;
    for(int iJ=0;iJ<GC.getNumBonusInfos();iJ++)
    {
        if(GC.getManaschoolInfo(iManaSchool).isBonusLinked((BonusTypes)iJ))
        {
			int iMod=1;
			if(GC.getManaschoolInfo(iManaSchool).getBonusIncreasedSpellResearch()==iJ)
			{
				iMod=2;
			}
			iIncomeFromSchool+=getNumAvailableBonuses((BonusTypes)iJ)*iMod;
        }
    }

	iIncomeFromSchool*=100+GC.getManaschoolInfo(iManaSchool).getSpellResearchAlignmentMod(getAlignment());
	iIncomeFromSchool/=100;

	return iIncomeFromSchool;
}

bool CvPlayer::canSpellResearchNext(TechTypes eSpellResearch) const
{
	if(GET_TEAM(getTeam()).isHasTech(eSpellResearch))
	{
		return false;
	}

	CvTechInfo& kSpellResearch=GC.getTechInfo(eSpellResearch);
	if(kSpellResearch.getPrereqBonus() != NO_BONUS)
	{
		if(getNumAvailableBonuses((BonusTypes)kSpellResearch.getPrereqBonus()) < 1)
		{
			return false;
		}
	}

	bool bValid = false;
	bool bFirst = false;
	TechTypes ePrereqResearch = NO_TECH;

	for(int iI = 0; iI < GC.getNUM_OR_TECH_PREREQS(); iI++)
	{
		ePrereqResearch = (TechTypes)kSpellResearch.getPrereqOrTechs(iI);
		if(ePrereqResearch != NO_TECH)
		{
			bFirst=true;
			if(GET_TEAM(getTeam()).isHasTech(ePrereqResearch))
			{
				bValid=true;
				break;
			}
		}
	}

	if(bFirst && !bValid)
	{
		return false;
	}

	for(int iI=0;iI<GC.getNUM_AND_TECH_PREREQS();iI++)
	{
		ePrereqResearch = (TechTypes)kSpellResearch.getPrereqAndTechs(iI);
		if(ePrereqResearch != NO_TECH)
		{
			if(!GET_TEAM(getTeam()).isHasTech(ePrereqResearch))
			{
				return false;
			}
		}
	}

	if(kSpellResearch.getPreferredAlignment()!=NO_ALIGNMENT)
	{
		if(getAlignment()!=kSpellResearch.getPreferredAlignment())
		{
			return false;
		}
	}

	if(kSpellResearch.getBlockedbyAlignment()!=NO_ALIGNMENT)
	{
		if(getAlignment()==kSpellResearch.getBlockedbyAlignment())
		{
			return false;
		}
	}

	if(kSpellResearch.getNumPrereqSpellResearchFromSameSchool()>0)
	{
		ManaschoolTypes eManaschool=(ManaschoolTypes)kSpellResearch.getSpellSchool();
		int iCounter=0;
		for(int iI=0;iI<GC.getNumTechInfos();iI++)
		{
			if(GC.getTechInfo((TechTypes)iI).getSpellSchool()==eManaschool)
			{
				if(GET_TEAM(getTeam()).isHasTech((TechTypes)iI))
				{
					iCounter++;
				}
			}
		}
		if(iCounter<kSpellResearch.getNumPrereqSpellResearchFromSameSchool())
		{
			return false;
		}
	}

	if(!isCanEverSpellResearch(eSpellResearch))
	{
		return false;
	}

	return true;
}

bool CvPlayer::canGetTrait(TraitTypes eTrait) const
{
	if(eTrait == NO_TRAIT) {
		return false;
	}

	CvTraitInfo& kTrait = GC.getTraitInfo(eTrait);

	if(hasTrait(eTrait))
		return false;

	if(kTrait.isCultureTrait()) {
		return true;
	}

	if((kTrait.getPrereqTrait() != NO_TRAIT) && hasTrait((TraitTypes)kTrait.getPrereqTrait())) {
		return true;
	}

	return false;
}

bool CvPlayer::canGetCulturalAchievement(TechTypes eCultureAchievement) const
{
	if(eCultureAchievement==NO_TECH)
		return false;

	CvTechInfo& kTech=GC.getTechInfo(eCultureAchievement);

	if(kTech.isGuild() 
		|| (!kTech.isCulturalAchievement())
		|| (GET_TEAM(getTeam()).getResearchCost(eCultureAchievement) > getGlobalCulture())
		|| (GET_TEAM(getTeam()).isHasTech(eCultureAchievement)))
		return false;

	TechTypes ePrereqResearch=NO_TECH;
	for(int iI=0;iI<GC.getNUM_AND_TECH_PREREQS();iI++)
	{
		ePrereqResearch=(TechTypes)kTech.getPrereqAndTechs(iI);
		if(ePrereqResearch!=NO_TECH)
		{
			if(!GET_TEAM(getTeam()).isHasTech(ePrereqResearch))
			{
				return false;
			}
		}
	}

	bool bValid=false;
	bool bFirst=false;
	for(int iI=0;iI<GC.getNUM_OR_TECH_PREREQS();iI++)
	{
		ePrereqResearch=(TechTypes)kTech.getPrereqOrTechs(iI);
		if(ePrereqResearch!=NO_TECH)
		{
			bFirst=true;
			if(GET_TEAM(getTeam()).isHasTech(ePrereqResearch))
			{
				bValid=true;
				break;
			}
		}
	}

	if(bFirst && !bValid)
	{
		return false;
	}

	if (!CvString(kTech.getPyRequirement()).empty())
    {
        CyArgsList argsList;
        argsList.add(getID());	// pass in unit class
        argsList.add(eCultureAchievement);//the spell #
        long lResult = 0;
        gDLL->getPythonIFace()->callFunction(PYSpellModule, "canResearch", argsList.makeFunctionArgs(), &lResult);
        if (lResult == 0)
        {
            return false;
        }
    }

	return true;
}

void CvPlayer::doArcane()
{
    if(isBarbarian())
    {
        return;
    }

	if(isAnarchy() || (getDisableProduction() > 0 ))
	{
		return;
	}

	if(getSpellResearchFocus()==NO_TECH)
	{
		if (isHuman() && getID() == GC.getGameINLINE().getActivePlayer())
		{
			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_CHOOSESPELLRESEARCH);
			if (NULL != pInfo)
			{
				gDLL->getInterfaceIFace()->addPopup(pInfo, getID());
			}
		}
		return;
	}

	if(GET_TEAM(getTeam()).isHasTech((TechTypes)getSpellResearchFocus()))
	{
		setSpellResearchFocus(NO_TECH);
		return;
	}

	changeArcane(getArcaneIncome());

    if(getArcane()>=getArcaneNeeded(getSpellResearchFocus()))
    {
	    GET_TEAM(getTeam()).setHasTech((TechTypes)getSpellResearchFocus(),true,getID(),true,true);
        changeArcane(-getArcaneNeeded(getSpellResearchFocus()));
		setSpellResearchFocus(NO_TECH);
    }

	if(getSpellResearchFocus()==NO_TECH)
	{
		if (isHuman() && getID() == GC.getGameINLINE().getActivePlayer())
		{
			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_CHOOSESPELLRESEARCH);
			if (NULL != pInfo)
			{
				gDLL->getInterfaceIFace()->addPopup(pInfo, getID());
			}
		}
		return;
	}
}

void CvPlayer::allowedReligion()
{
	if(!isAllowedMoreReligions())
	{
		return;
	}	

	if (isHuman() && getID() == GC.getGameINLINE().getActivePlayer())
	{
		CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_CHOOSE_RELIGION);
		if (NULL != pInfo)
		{
			gDLL->getInterfaceIFace()->addPopup(pInfo, getID());
		}
		return;
	}

	if(!isHuman()) {
		int iValue;
		int iBestValue = 0;
		TechTypes eBestReligion = NO_TECH;

		for(int iI = 0; iI < GC.getNumTechInfos(); ++iI) {
			if(GC.getTechInfo((TechTypes)iI).isReligion() && !GET_TEAM(getTeam()).isHasTech((TechTypes)iI)) {
				iValue = GC.getGame().getSorenRandNum(100, "AI pick Religion");
				if(getFavoriteReligion()==GC.getTechInfo((TechTypes)iI).getReligionType())
					iValue += 10000;

				if(iValue > iBestValue) {
					iBestValue = iValue;
					eBestReligion = (TechTypes)iI;
				}
			}
		} // for TechInfos

		if(eBestReligion != NO_TECH)
			GET_TEAM(getTeam()).setHasTech(eBestReligion, true, getID(), true, true);

	} // if !isHuman()
}

void CvPlayer::allowedGuild()
{
	if(!isAllowedMoreGuilds())
	{
		return;
	}	

	if (isHuman() && getID() == GC.getGameINLINE().getActivePlayer())
	{
		CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_CHOOSE_GUILD);
		if (NULL != pInfo)
		{
			gDLL->getInterfaceIFace()->addPopup(pInfo, getID());
		}
		return;
	}

	if(!isHuman()) {
		int iValue;
		int iBestValue = -MAX_INT;
		TechTypes eBestGuild = NO_TECH;

		for(int iI = 0; iI < GC.getNumTechInfos(); ++iI) {
			if(canGetGuild((TechTypes)iI)) {
				iValue = static_cast<CvPlayerAI*>(this)->AI_techValueNew((TechTypes)iI, false);

				if(iValue > iBestValue) {
					iBestValue = iValue;
					eBestGuild = (TechTypes)iI;
				}
			}
		} // for TechInfos

		if(eBestGuild != NO_TECH)
			GET_TEAM(getTeam()).setHasTech(eBestGuild, true, getID(), true, true);

	} // if !isHuman()

}

bool CvPlayer::isAllowedMoreReligions() const
{
	if(isAgnostic())
		return false;

	int iNumReligions = getReligionsSpread();
	int iNumReligionsAllowed = 0;
	for(int iI = 0; iI < GC.getNumTechInfos(); iI++) {

		if(GET_TEAM(getTeam()).isHasTech((TechTypes)iI)) {
			if(GC.getTechInfo((TechTypes)iI).isReligion())
				iNumReligions++;
			if(GC.getTechInfo((TechTypes)iI).isAllowReligion())			
				iNumReligionsAllowed++;			
		} // if ishasTech
	} // for TechInfos

	return iNumReligions < iNumReligionsAllowed;
}

bool CvPlayer::canGetGuild(TechTypes eTech) const
{
	if(eTech == NO_TECH)
		return false;

	CvTechInfo& kTech = GC.getTechInfo(eTech);
	if(!kTech.isGuild())
		return false;

	if(GET_TEAM(getTeam()).isHasTech(eTech))
		return false;
	if(kTech.getBlockedbyAlignment() != NO_ALIGNMENT 
		&& kTech.getBlockedbyAlignment() == getAlignment())
		return false;

	return true;
}

bool CvPlayer::isAllowedMoreGuilds() const
{
	int iNumGuilds=0;
	int iNumGuildsAllowed=0;
	for(int iI=0;iI<GC.getNumTechInfos();iI++)
	{
		if(GET_TEAM(getTeam()).isHasTech((TechTypes)iI))
		{
			if(GC.getTechInfo((TechTypes)iI).isGuild())
			{
				iNumGuilds++;
			}
			if(GC.getTechInfo((TechTypes)iI).isAllowGuild())
			{
				iNumGuildsAllowed++;
			}
		}
	}

	return iNumGuilds < iNumGuildsAllowed;
}

int CvPlayer::getArcaneIncome() const
{
	int iIncome=0;
	int iMod=100;
	iIncome+=calculateArcaneByBuildings();

	if(getSpellResearchFocus()!=NO_TECH)
	{
		ManaschoolTypes eManaschool=(ManaschoolTypes)GC.getTechInfo((TechTypes)getSpellResearchFocus()).getSpellSchool();

		for(int iJ=0;iJ<GC.getNumBonusInfos();iJ++)
		{
			if (GC.getManaschoolInfo(eManaschool).isBonusLinked(iJ))
			{
				if(getNumAvailableBonuses((BonusTypes)iJ)>0)
				{
					iMod+=5*getNumAvailableBonuses((BonusTypes)iJ);
				}
			}
		}
	}

	iIncome*=iMod;
	iIncome/=100;

	return iIncome;
}

int CvPlayer::getSpellResearchTurnsLeft() const
{
	if(getArcaneIncome()<=0 || getSpellResearchFocus()==NO_TECH)
	{
		return -1;
	}
	return getArcaneNeeded(getSpellResearchFocus())/getArcaneIncome();
}


int CvPlayer::getMana() const
{
    return m_iMana;
}

void CvPlayer::changeMana(int change)
{
	if (change != 0)
	{
		m_iMana+=change;
		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(SelectionButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}


int CvPlayer::getManaIncome() const
{
	return m_iManaIncome;
}

int CvPlayer::CalculateManaByBuildings() const
{
	int iTemp=0;
	int iLoop;

	bool bManaFromImprovements = false;

	for(int i = 0; i < GC.getNumImprovementInfos(); ++i) {
		if(getManaFromImprovement(i) > 0) {
			bManaFromImprovements = true;
			break;
		}
	}

	for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop)) {

		int iManaFromCity = pLoopCity->getCommerceRate(COMMERCE_MANA);

		if(bManaFromImprovements) {
			int iManaFromImprovements = 0;
			for(int i = 0; i < pLoopCity->getNumCityPlots(); ++i) {
				CvPlot* pLoopPlot = pLoopCity->getCityIndexPlot(i);
				if(pLoopPlot != NULL)
					if(pLoopPlot->getImprovementType() != NO_IMPROVEMENT && pLoopPlot->getWorkingCity() == pLoopCity) 
						iManaFromImprovements += getManaFromImprovement(pLoopPlot->getImprovementType());
			}

			iManaFromCity += iManaFromImprovements / 100;
		}

		iManaFromCity *= 100 + pLoopCity->getCommerceRateModifier(COMMERCE_MANA);
		iManaFromCity /= 100;

		iTemp += iManaFromCity;
	}

	iTemp*=(100+getManaCreationModifier());
	iTemp/=100;

    if(!isHuman())
    {
		iTemp *= GC.getHandicapInfo(GET_PLAYER((PlayerTypes)0).getHandicapType()).getAIManaModifier();
        iTemp /= 100;
    }

	return iTemp;
}

int CvPlayer::CalculateManaByNodes() const
{
	int iTemp = 0;

	int iMod = getManaFromNodes();

	ImprovementTypes iManaShrine = (ImprovementTypes)GC.getIMPROVEMENT_MANA_SHRINE();

	for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

		if(pLoopPlot->getOwnerINLINE() == getID())
		{
			if(pLoopPlot->getImprovementType() == iManaShrine)
            {
	            iTemp += 10 + iMod;
            }

            if(pLoopPlot->getBonusType(getTeam()) != NO_BONUS)
            {
                if (GC.getBonusInfo((BonusTypes)pLoopPlot->getBonusType(getTeam())).getBonusClassType() == GC.getDefineINT("BONUSCLASS_MANA")
					|| GC.getBonusInfo((BonusTypes)pLoopPlot->getBonusType(getTeam())).getBonusClassType()==GC.getDefineINT("BONUSCLASS_MANA_RAW"))
                {
                    iTemp += 10 + iMod;
                }
            }
		}
	}

	iTemp *= (100 + getManaCreationModifier() + getCommerceRateModifier(COMMERCE_MANA));
	iTemp /= 100;

    if(!isHuman())
    {
		iTemp *= GC.getHandicapInfo(GET_PLAYER((PlayerTypes)0).getHandicapType()).getAIManaModifier();
        iTemp /= 100;
    }

	iTemp *= 100 + getExtraManaNodesPercent();
	iTemp /= 100;

	return iTemp;
}

int CvPlayer::CalculateManaUpkeepGE() const
{
	int iTemp = 0;
	CvTeam& kTeam = GET_TEAM(getTeam());
	for(int iI = 0; iI < GC.getNumProjectInfos(); iI++)
	{
		CvProjectInfo& kProject = GC.getProjectInfo((ProjectTypes)iI);
		if(kProject.isGlobalEnchantment())
		{
			if(kTeam.getProjectCount((ProjectTypes)iI) > 0)
			{
				int iCost = kProject.getManaUpkeep() * 100 + kProject.getManaUpkeepPerCityTimes100() * getNumCities();

				if(kProject.isGlobalEnchantmentManaBonus() 
					&& kProject.getTechPrereq() != NO_TECH && GC.getTechInfo((TechTypes)kProject.getTechPrereq()).getPrereqBonus() != NO_BONUS)
				{
					iCost *= 100 - std::min(30, (getNumAvailableBonuses((BonusTypes)GC.getTechInfo((TechTypes)kProject.getTechPrereq()).getPrereqBonus()) - 1) * 10);
					iCost /= 100;
				}

				iCost = iCost + 50;  //always round up

				iCost /= 100;
				iTemp += iCost;
			}
		}
	}

	return iTemp;
}

int CvPlayer::CalculateFromSpellResearch() const
{
	int iTemp=getManaFromSpellResearch();

	iTemp*=(100+getManaCreationModifier()+getCommerceRateModifier(COMMERCE_MANA));
	iTemp/=100;

    if(!isHuman())
    {
		iTemp *= GC.getHandicapInfo(GET_PLAYER((PlayerTypes)0).getHandicapType()).getAIManaModifier();
        iTemp /= 100;
    }

	return iTemp;
}

void CvPlayer::updateManaIncome()
{
	int iTemp = 0;

    iTemp -= getManaUpkeepTotal();
	iTemp -= CalculateManaUpkeepGE();
	iTemp += CalculateManaByBuildings();
	iTemp += CalculateManaByNodes();
	iTemp += CalculateFromSpellResearch();
	m_iManaIncome = iTemp;
}

void CvPlayer::doMana()
{
	AdvanceMagicRitual();

	updateManaIncome();
	changeMana(getManaIncome());
	if(getMana() > getMaxMana())
	{
		changeArcane(calculateArcaneByLeftOverMana());
		changeMana(getMaxMana() - getMana());
	}

	if(getMana()<0 && getManaIncome()*10+getMana()<0)
	{
		int iManaNeededToSave=std::max(3,-getManaIncome());
		//need to remove either Global Enchantment or Summoned Unit
		for(int iI=0;iI<GC.getNumProjectInfos();iI++)
		{
			CvProjectInfo& kProject=GC.getProjectInfo((ProjectTypes)iI);
			int iProjectUpkeep=kProject.getManaUpkeep();
			if(kProject.isGlobalEnchantment() && iProjectUpkeep>0)
			{
				if(GET_TEAM(getTeam()).getProjectCount((ProjectTypes)iI)>0)
				{
					RemoveGlobalEnchantment((ProjectTypes)iI);
					CvWString szBuffer;
					szBuffer.Format(L"You mana levels have gotten so low your mages have canceled the %s global enchantment.", kProject.getDescription());
					gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DEAL_CANCELLED", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), -1, -1, true, true);
					iManaNeededToSave-=iProjectUpkeep;
					if(iManaNeededToSave<=0)
					{
						break;
					}
				}
			}
		}
		if(iManaNeededToSave>0)
		{
			int iLoop;
			int iManaUpkeep;
			for(CvUnit* pLoopUnit=firstUnit(&iLoop);pLoopUnit!=NULL;pLoopUnit=nextUnit(&iLoop))
			{
				iManaUpkeep=GC.getUnitInfo(pLoopUnit->getUnitType()).getManaUpkeep();
				if(iManaUpkeep>0)
				{
					pLoopUnit->scrap();
					CvWString szBuffer;
					szBuffer.Format(L"Your mana levels have gotten so low your mages have banished your %s summon.", GC.getUnitInfo(pLoopUnit->getUnitType()).getDescription());
					gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DEAL_CANCELLED", MESSAGE_TYPE_MINOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_HAPPY_PERSON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), -1, -1, true, true);
					iManaNeededToSave-=iManaUpkeep;
					if(iManaNeededToSave<=0)
					{
						break;
					}
				}
			}
		}
	}
}

int CvPlayer::getFaith() const
{
    return m_iFaith;
}

void CvPlayer::changeFaith(int change)
{
	if (change != 0)
	{
		m_iFaith+=change;
		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(SelectionButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

int CvPlayer::getFaithMod() const
{
	return 100;
//    return m_iFaithMod;
}

void CvPlayer::changeFaithMod(int change)
{
    m_iFaithMod+=change;
}

int CvPlayer::getManaBonus2() const
{
    return m_iManaBonus2;
}

void CvPlayer::setManaBonus2(int iNewValue)
{
    m_iManaBonus2=iNewValue;
}

int CvPlayer::getManaBonus3() const
{
    return m_iManaBonus3;
}

void CvPlayer::setManaBonus3(int iNewValue)
{
    m_iManaBonus3=iNewValue;
}

int CvPlayer::getManaBonus4() const
{
    return m_iManaBonus4;
}

void CvPlayer::setManaBonus4(int iNewValue)
{
    m_iManaBonus4=iNewValue;
}

int CvPlayer::getBarbarianPower() const
{
    return m_iBarbarianPower;
}

void CvPlayer::setBarbarianPower(int iNewValue)
{
    m_iBarbarianPower = iNewValue;
}

void CvPlayer::changeBarbarianPower(int iChange)
{
    m_iBarbarianPower += iChange;
}

int CvPlayer::getBarbarianPoints() const
{
    return m_iBarbarianPoints;
}

void CvPlayer::setBarbarianPoints(int iNewValue)
{
    m_iBarbarianPoints = iNewValue;
}

void CvPlayer::changeBarbarianPoints(int iChange)
{
    m_iBarbarianPoints += iChange;
}

int CvPlayer::getBarbarianValue2() const
{
    return m_iBarbarianValue2;
}

void CvPlayer::setBarbarianValue2(int iNewValue)
{
    m_iBarbarianValue2 = iNewValue;
}

void CvPlayer::changeBarbarianValue2(int iChange)
{
    m_iBarbarianValue2 += iChange;
}

int CvPlayer::getBarbarianValue3() const
{
    return m_iBarbarianValue3;
}

void CvPlayer::setBarbarianValue3(int iNewValue)
{
    m_iBarbarianValue3 = iNewValue;
}

void CvPlayer::changeBarbarianValue3(int iChange)
{
    m_iBarbarianValue3 += iChange;
}

int CvPlayer::getFaithIncome() const
{
	return m_iFaithIncome;
}

int CvPlayer::getReligionsSpread() const
{
	return m_iReligionsSpread;
}

void CvPlayer::changeReligionsSpread(int iChange)
{
	m_iReligionsSpread += iChange;
}

int CvPlayer::calculateFaithByBuildings() const
{
	int iTemp=0;
	int iLoop;

	bool bFaithFromImprovements = false;

	for(int i = 0; i < GC.getNumImprovementInfos(); ++i) {
		if(getFaithFromImprovement(i) > 0) {
			bFaithFromImprovements = true;
			break;
		}
	}

	for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop)) {

		int iFaithFromCity = pLoopCity->getCommerceRate(COMMERCE_FAITH);

		if(bFaithFromImprovements) {
			int iFaithFromImprovements = 0;
			for(int i = 0; i < pLoopCity->getNumCityPlots(); ++i) {
				CvPlot* pLoopPlot = pLoopCity->getCityIndexPlot(i);
				if(pLoopPlot != NULL)
					if(pLoopPlot->getImprovementType() != NO_IMPROVEMENT && pLoopPlot->getWorkingCity() == pLoopCity) 
						iFaithFromImprovements += getFaithFromImprovement(pLoopPlot->getImprovementType());
			}

			iFaithFromCity += iFaithFromImprovements / 100;
		}

		iFaithFromCity *= 100 + pLoopCity->getCommerceRateModifier(COMMERCE_FAITH);
		iFaithFromCity /= 100;

		iTemp += iFaithFromCity;
	}

	/*for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iTemp += (pLoopCity->getCommerceRate(COMMERCE_FAITH));
	}*/

	return iTemp;
}

int CvPlayer::calculateFaithLostTemples() const
{
	int iTemp=0;

	ImprovementTypes iLostTemple=(ImprovementTypes)GC.getInfoTypeForString("IMPROVEMENT_LOST_TEMPLE");

	for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

		if(pLoopPlot->getOwnerINLINE()==getID())
		{
			if(pLoopPlot->getImprovementType()==iLostTemple)
            {
	            iTemp += 10;
            }
		}
	}

	return iTemp;
}

int CvPlayer::calculateFaithAncientForests() const
{
	int iTemp=0;

	if(getStateReligion()!=RELIGION_FELLOWSHIP_OF_LEAVES)
	{
		return 0;
	}
	FeatureTypes iAncientForest=(FeatureTypes)GC.getInfoTypeForString("FEATURE_FOREST_ANCIENT");

	for (int iI = 0; iI < GC.getMapINLINE().numPlotsINLINE(); iI++)
	{
		CvPlot* pLoopPlot = GC.getMapINLINE().plotByIndexINLINE(iI);

		if(pLoopPlot->getOwnerINLINE()==getID())
		{
			if(pLoopPlot->getFeatureType()==iAncientForest)
            {
	            iTemp+=1;
            }
		}
	}

	return iTemp;
}

int CvPlayer::getTurnSacredKnowledge() const
{
	int iFaithRate = getFaithIncome() / 2;
	if(iFaithRate < 1)
		return MAX_INT;

	int iMinimum = MAX_INT;
	for(int iI = 0; iI < GC.getNumTechInfos(); iI++)
	{
		CvTechInfo& kTech = GC.getTechInfo((TechTypes)iI);
		if(kTech.isHolyKnowledge() && !isHasTech(iI) && kTech.getReligionType() == getStateReligion()
			&& (kTech.getPrereqOrTechs(0) == NO_TECH || isHasTech(kTech.getPrereqOrTechs(0)) )){
				CvTeam& kTeam = GET_TEAM(getTeam());
				iMinimum = std::min(iMinimum, kTeam.getResearchLeft((TechTypes)iI));				
		}
		//Darksavant
		//Added below to try and fix Divine Judgement not working
		if (_tcscmp(kTech.getType(), "TECH_DIVINE_JUDGEMENT") == 0)  //0 means the strings are equal
		{
			int iJ = 0;
			while ((kTech.getPrereqOrTechs(iJ) != NO_TECH) && (iJ < GC.getNumReligionInfos()))
			{

				CvTechInfo& kPreTech = GC.getTechInfo((TechTypes)kTech.getPrereqOrTechs(iJ));
				if (!isHasTech(iI) && isHasTech(kTech.getPrereqOrTechs(iJ)) && kPreTech.getReligionType() == getStateReligion() && kTech.isHolyKnowledge())
				{
					CvTeam& kTeam = GET_TEAM(getTeam());
					iMinimum = std::min(iMinimum, kTeam.getResearchLeft((TechTypes)iI));
				}
				iJ++;
			}
		}
	}

	return iMinimum / iFaithRate;
}

bool CvPlayer::isFaithIncomeDoubled() const
{
	int iStateRel = getStateReligion();

	if(iStateRel == NO_RELIGION)
	{
		return false;
	}

	if(hasHolyCity((ReligionTypes)iStateRel))
	{
		return true;
	}

	if(getFaithMod() < 300)
		return false;

	if(iStateRel==RELIGION_RUNES_OF_KILMORPH)
	{
		if(getGold()>3000)
		{
			return true;
		}
	}

	if(iStateRel==RELIGION_FELLOWSHIP_OF_LEAVES)
	{
		if(!GET_TEAM(getTeam()).isAtWar((TeamTypes)GC.getANIMAL_TEAM()))
		{
			return true;
		}
	}

	if(iStateRel==RELIGION_THE_EMPYREAN)
	{
		bool bValid=true;
		for(int iI=0;iI<GC.getMAX_CIV_TEAMS();iI++)
		{
			if(iI!=getTeam())
			{
				if(GET_TEAM(getTeam()).isAtWar((TeamTypes)iI))
				{
					bValid=false;
					break;
				}
			}
		}
		if(bValid)
		{
			return true;
		}
	}

	if(iStateRel==RELIGION_THE_ORDER)
	{
		bool bValid=false;
		for(int iI=0;iI<GC.getMAX_CIV_PLAYERS();iI++)
		{
			CvPlayer &kPlayer=GET_PLAYER((PlayerTypes)iI);

			if(kPlayer.getTeam()!=getTeam())
			{
				if(kPlayer.getAlignment()==ALIGNMENT_EVIL)
				{
					if(GET_TEAM(getTeam()).isAtWar(kPlayer.getTeam()))
					{						
						bValid=true;
						break;
					}
				}
			}
		}
		if(bValid)
		{
			return true;
		}
	}

	if(iStateRel==RELIGION_OCTOPUS_OVERLORDS)
	{
		int iAsylum=GC.getInfoTypeForString("BUILDINGCLASS_ASYLUM");

		if(iAsylum!=NO_BUILDINGCLASS && getBuildingClassCount((BuildingClassTypes)iAsylum)>getNumCities()/3)
		{
			return true;
		}
	}

	if(iStateRel==RELIGION_THE_ASHEN_VEIL)
	{
		if(GC.getGameINLINE().getGlobalCounter()>70)
		{
			return true;
		}
	}

	return false;
}

void CvPlayer::updateFaithIncome()
{
	int iTemp = 0;
	iTemp += calculateFaithByBuildings();
	iTemp += calculateFaithLostTemples();
	iTemp += calculateFaithAncientForests();
	iTemp -= getFaithUpkeep();

	if(isFaithIncomeDoubled() && iTemp > 0)
	{
		iTemp += 20;
	}
	m_iFaithIncome = iTemp;
}
void CvPlayer::doFaith()
{
/**
	if (isHuman() && getID() == GC.getGameINLINE().getActivePlayer())
	{
		for(int iI = 0; iI < GC.getNumTechInfos();iI++)
		{
			if(canBuyHolyKnowledge((TechTypes)iI))
			{
				CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_PICK_SACREDKNOWLEDGE);
				if (NULL != pInfo)
				{
					gDLL->getInterfaceIFace()->addPopup(pInfo, getID());
				}
				break;
			}
		}
	}
**/
	if(GC.getCivilizationInfo(getCivilizationType()).isNoFaith())
    {
        changeFaith(-getFaith());
        changeFaithMod(-getFaithMod());
        return;
    }

	changeFaithMod(1);
	if(getFaithMod() > 10000)
	{
		changeFaithMod(-getFaithMod() + 10000);
	}

	updateFaithIncome();
	//half Faith Income is spend on Sacred Knowledge
	int iSacred = getFaithIncome() / 2;
    int iChange = getFaithIncome() - iSacred;

	for(int iI = 0; iI < GC.getNumTechInfos(); iI++)
	{
		CvTechInfo& kTech = GC.getTechInfo((TechTypes)iI);
		if(kTech.isHolyKnowledge() && !isHasTech(iI) && kTech.getReligionType() == getStateReligion()
			&& (kTech.getPrereqOrTechs(0) == NO_TECH || isHasTech(kTech.getPrereqOrTechs(0)) )){
				CvTeam& kTeam = GET_TEAM(getTeam());
				kTeam.changeResearchProgress((TechTypes)iI, iSacred, getID());
		}

		//Darksavant
		//Added below to try and fix Divine Judgement not working
		if (_tcscmp(kTech.getType(), "TECH_DIVINE_JUDGEMENT") == 0)  //0 means the strings are equal
		{
			int iJ = 0;
			while ((kTech.getPrereqOrTechs(iJ) != NO_TECH) && (iJ < GC.getNumReligionInfos()))
			{
				
				CvTechInfo& kPreTech = GC.getTechInfo((TechTypes)kTech.getPrereqOrTechs(iJ));
				if (!isHasTech(iI) && isHasTech(kTech.getPrereqOrTechs(iJ)) && kPreTech.getReligionType() == getStateReligion() && kTech.isHolyKnowledge())
				{
					CvTeam& kTeam = GET_TEAM(getTeam());
					kTeam.changeResearchProgress((TechTypes)iI, iSacred, getID());
				}
				iJ++;
			}
		}
	}

    if(!isHuman())
    {
		iChange *= GC.getHandicapInfo(GET_PLAYER((PlayerTypes)0).getHandicapType()).getAIManaModifier();
        iChange /= 100;
    }
	
	changeFaith(iChange);

	//Darksavant
	//Commented out in lieu of below changes
	/*if(getFaith()<0)
	{
		changeFaith(-getFaith());
	}*/

	//Darksavant
	//Mirroring the method from doMana() that cancels divine enchants and summons if your faith runs out

	if (getFaith()<0 && getFaithIncome() * 10 + getFaith()<0)
	{
		int iFaithNeededToSave = std::max(3, -getFaithIncome());
		//need to remove either Global Enchantment or Summoned Unit
		for (int iI = 0; iI<GC.getNumProjectInfos(); iI++)
		{
			CvProjectInfo& kProject = GC.getProjectInfo((ProjectTypes)iI);
			int iProjectUpkeep = kProject.getFaithUpkeep();
			if (kProject.isGlobalEnchantment() && iProjectUpkeep>0)
			{
				if (GET_TEAM(getTeam()).getProjectCount((ProjectTypes)iI)>0)
				{
					RemoveGlobalEnchantment((ProjectTypes)iI);
					CvWString szBuffer;
					szBuffer.Format(L"Your diety finds your lack of Faith disturbing and has canceled the %s global enchantment.", kProject.getDescription());
					gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DEAL_CANCELLED", MESSAGE_TYPE_MINOR_EVENT, NULL, (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), -1, -1, true, true);
					iFaithNeededToSave -= iProjectUpkeep;
					if (iFaithNeededToSave <= 0)
					{
						break;
					}
				}
			}
		}
		if (iFaithNeededToSave>0)
		{
			int iLoop;
			int iFaithUpkeep;
			for (CvUnit* pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
			{
				iFaithUpkeep = GC.getUnitInfo(pLoopUnit->getUnitType()).getFaithUpkeep();
				if (iFaithUpkeep>0)
				{
					pLoopUnit->scrap();
					CvWString szBuffer;
					szBuffer.Format(L"Your diety finds your lack of Faith disturbing and has recalled your %s summon.", GC.getUnitInfo(pLoopUnit->getUnitType()).getDescription());
					gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_DEAL_CANCELLED", MESSAGE_TYPE_MINOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_HAPPY_PERSON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), -1, -1, true, true);
					iFaithNeededToSave -= iFaithUpkeep;
					if (iFaithNeededToSave <= 0)
					{
						break;
					}
				}
			}
		}
	}
}

void CvPlayer::doGlobalCulture()
{
	changeGlobalCulture(calculateGlobalCultureIncome());
}

int CvPlayer::calculateGlobalCultureIncome() const
{
	int iTemp=0;
	int iLoop;

	for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iTemp+=(pLoopCity->getCommerceRate(COMMERCE_CULTURE));
	}

	return iTemp;
}

void CvPlayer::changeManaSchoolCostModifier(int iManaschool, int iChange)
{
	m_paiManaSchoolCostModifier[iManaschool]+=iChange;
}

int CvPlayer::getManaSchoolCostModifier(int iManaschool) const
{
	return m_paiManaSchoolCostModifier[iManaschool];
}

void CvPlayer::setSpellResearchFocus(int iNewValue)
{
	if(iNewValue!=m_iSpellResearchFocus)
	{
		m_iSpellResearchFocus=iNewValue;
		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(SelectionButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

int CvPlayer::getSpellResearchFocus() const
{
	return m_iSpellResearchFocus;
}

void CvPlayer::setCanEverSpellResearch(int iSpellResearch, bool bNewValue)
{
	m_pabCanEverSpellResearch[iSpellResearch]=bNewValue;
}

bool CvPlayer::isCanEverSpellResearch(int iSpellResearch) const
{
	return m_pabCanEverSpellResearch[iSpellResearch];
}

void CvPlayer::changeGlobalYield(int iYield, int iChange)
{
	if(iChange!=0)
	{
		m_paiGlobalYield[iYield]+=iChange;
		m_paiGlobalYield[iYield]=std::max(m_paiGlobalYield[iYield],0);

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(SelectionButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

int CvPlayer::getGlobalYield(int iYield) const
{
	return m_paiGlobalYield[iYield];
}

void CvPlayer::changeGlobalYieldUpkeep(int iYield, int iChange)
{
	if(iChange!=0)
	{
		m_paiGlobalYieldUpkeep[iYield]+=iChange;

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(SelectionButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

int CvPlayer::getGlobalYieldUpkeep(int iYield) const
{
	return m_paiGlobalYieldUpkeep[iYield];
}

void CvPlayer::changeGlobalYieldNeededForEquipmentBonus(int iYield, int iChange)
{
	if(iChange!=0 && iYield!=NO_YIELD)
	{
		m_paiGlobalYieldNeededForEquipmentBonus[iYield] += iChange;
	}
}

int CvPlayer::getGlobalYieldNeededForEquipmentBonus(int iYield) const
{
	return m_paiGlobalYieldNeededForEquipmentBonus[iYield];
}

int CvPlayer::getCombatBonusFromEquipment(const CvUnit* pUnit) const
{
	// Disable Mechanic
	return 0;
	// Disable Mechanic
	YieldTypes eYield = (YieldTypes)pUnit->getEquipmentYieldType();
	
	if(eYield == NO_YIELD)
		return 0;

	double iMod = getGlobalYield(eYield);
	int iGlobalYieldNeeded = getGlobalYieldNeededForEquipmentBonus(eYield);

	if(iGlobalYieldNeeded == 0)
		return 0;

	iMod /= iGlobalYieldNeeded;

	iMod = pow(iMod, 0.66);

	iMod = std::min(1.0, iMod);

	return (int)(GC.getDefineINT("MAX_EQUIPMENT_BONUS")*iMod);
}

void CvPlayer::changeNumBonusesConsumed(int iBonus, int iChange)
{
	if(iChange!=0)
	{
		m_paiNumBonusesConsumed[iBonus]+=iChange;
	}
}

int CvPlayer::getNumBonusesConsumed(int iBonus) const
{
	return m_paiNumBonusesConsumed[iBonus];
}

void CvPlayer::changeNumBonusesConsumedByProduction(int iBonus, int iChange)
{
	if(iChange!=0)
	{
		m_paiNumBonusesConsumedByProduction[iBonus]+=iChange;
	}
}

int CvPlayer::getNumBonusesConsumedByProduction(int iBonus) const
{
	return m_paiNumBonusesConsumedByProduction[iBonus];
}

void CvPlayer::changeFoundHolyCityCount(int iReligion, int iChange)
{
	if(iChange!=0)
	{
		m_paiFoundHolyCityCount[iReligion]+=iChange;
	}
}

int CvPlayer::getFoundHolyCityCount(int iReligion) const
{
	return m_paiFoundHolyCityCount[iReligion];
}

void CvPlayer::changeManaFromImprovement(int iImprovement, int iChange)
{
	if(iChange != 0) {
		m_paiManaFromImprovement[iImprovement] += iChange;
	}
}

int CvPlayer::getManaFromImprovement(int iImprovement) const
{
	return m_paiManaFromImprovement[iImprovement];
}

void CvPlayer::changeFaithFromImprovement(int iImprovement, int iChange)
{
	if(iChange != 0) {
		m_paiFaithFromImprovement[iImprovement] += iChange;
	}
}

int CvPlayer::getFaithFromImprovement(int iImprovement) const
{
	return m_paiFaithFromImprovement[iImprovement];
}

void CvPlayer::changeIgnoreBuildingGYCostCount(int eBuildingClass, int iChange)
{
	if(iChange != 0) {
		m_paiIgnoreBuildingGYCostCount[eBuildingClass] += iChange;
	}
}

int CvPlayer::getIgnoreBuildingGYCostCount(int eBuildingClass) const
{
	return m_paiIgnoreBuildingGYCostCount[eBuildingClass];
}

bool CvPlayer::isIgnoreGlobalYieldCost(BuildingClassTypes eBuildingClass) const
{
	return m_paiIgnoreBuildingGYCostCount[eBuildingClass] > 0;
}

int CvPlayer::getFoundHolyCityProb(int iReligion) const
{
	int iProb = getFoundHolyCityCount(iReligion);
	int iMod = 100;
	for(int iI = 0; iI < GC.getNumTraitInfos(); iI++)
	{
		if(hasTrait((TraitTypes)iI))
		{
			//disabled for now
			//iMod+=GC.getTraitInfo((TraitTypes)iI).getFoundHolyCityMod();
		}
	}
	iProb*=iMod;
	iProb/=100;

	return iProb;
}
void CvPlayer::doHolyCityCounts()
{
	int iLoop;
	int iCount;
	if(getStateReligion()!=NO_RELIGION)
	{
		iCount=getFaithIncome();

		for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
		{
			if(pLoopCity->isHasReligion(getStateReligion()))
			{
				iCount+=pLoopCity->getPopulation()*3;
			}
		}

		changeFoundHolyCityCount(getStateReligion(),iCount);
	}
}
void CvPlayer::doGlobalYield()
{
	for(int iI=YIELD_COMMERCE+1;iI<GC.getNUM_YIELD_TYPES();iI++)
	{
		changeGlobalYield(iI,calculateGlobalYieldIncome(iI));
		changeGlobalYield(iI,-getGlobalYieldUpkeep(iI));
	}
}

int CvPlayer::calculateGlobalYieldIncome(int iYield) const
{
	if(isAnarchy() || (getDisableProduction() > 0 ))
	{
		return 0;
	}

	int iIncome=0;
	int iLoop;
	for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		iIncome+=pLoopCity->getYieldIncome(iYield);
	}

	return iIncome;
}

bool CvPlayer::canCreateSummon(ProjectTypes eRitual, bool bAISaveUpMana) const
{
	CvProjectInfo &kProject=GC.getProjectInfo(eRitual);

	if(kProject.getSummonUnitType()==NO_UNIT)
	{
		return false;
	}

	if(kProject.getTechPrereq()!=NO_TECH)
	{
		if(!GET_TEAM(getTeam()).isHasTech((TechTypes)kProject.getTechPrereq()))
		{
			return false;
		}
	}

	if(kProject.getPrereqCivilization()!=NO_CIVILIZATION)
	{
		if(kProject.getPrereqCivilization()!=getCivilizationType())
		{
			return false;
		}
	}

	if(!bAISaveUpMana)
	{
		if(getManaIncome()==0)
		{
			return false;
		}

		if(kProject.getFaithCost()>getFaith())
		{
			return false;
		}
	}

	return true;
}

int CvPlayer::doTerraformRitual(ProjectTypes eRitual, bool bCountOnly, CvPlot* pTargetPlot)
{
	CvProjectInfo &kProject=GC.getProjectInfo(eRitual);
	CvPlot* pLoopPlot;
	int iCounter=0;
	int iTarget=kProject.getNumTerrainChanges();

	int iCostMod=100;
	/** maybe give a discount for Targeted Terraforming? (effects only one plot instead of several)
	if(pTargetPlot!=NULL && iTarget!=1)
	{
		iCostMod=150/std::max(1,iTarget);
	}
	**/

	if(iTarget<=0)
	{
		return 0;
	}

	ManaschoolTypes iSpellSchool=NO_MANASCHOOL;
	if(kProject.getTechPrereq()!=NO_TECH)
	{
		iSpellSchool=(ManaschoolTypes)GC.getTechInfo((TechTypes)kProject.getTechPrereq()).getSpellSchool();
	}
	if(iSpellSchool!=NO_MANASCHOOL && GC.getManaschoolInfo(iSpellSchool).getBonusIncreasedTerraformEffect()!=NO_BONUS)
	{
		iTarget*=100+20*getNumAvailableBonuses((BonusTypes)GC.getManaschoolInfo(iSpellSchool).getBonusIncreasedTerraformEffect());
		iTarget/=100;
	}

	if(kProject.isHostileTerraform())
	{
		iTarget+=getIncreaseHostileTerraformTiles();
	}
	else
	{
		iTarget+=getIncreaseTerraformTiles();
	}

	//Bonus from Mana Bonuses
	if(kProject.isTransmutationManaBonus() && kProject.getTechPrereq()!=NO_TECH
		&& GC.getTechInfo((TechTypes)kProject.getTechPrereq()).getPrereqBonus()!=NO_BONUS)
	{
		iTarget+=getNumAvailableBonuses((BonusTypes)GC.getTechInfo((TechTypes)kProject.getTechPrereq()).getPrereqBonus());
	}

	int iValue=0;

	if(pTargetPlot!=NULL)
	{
		pTargetPlot->doTerraformRitual(eRitual, getID(), iValue,false);
		setTerraformPlan(NO_PROJECT);
		gDLL->getEngineIFace()->RebuildPlot(pTargetPlot->getX_INLINE(),pTargetPlot->getY_INLINE(),true,true);
		return iValue;
	}

	if(iCounter<iTarget)
	{
		for(int iI=0;iI<GC.getMapINLINE().numPlotsINLINE();iI++)
		{
			pLoopPlot=GC.getMapINLINE().plotByIndexINLINE(iI);
			if(pLoopPlot->isOwned())
			{
				if((!kProject.isHostileTerraform() && pLoopPlot->getOwnerINLINE()==getID())
					|| (kProject.isHostileTerraform() && GET_TEAM(getTeam()).isAtWar(pLoopPlot->getTeam())))
				{
					if((!pLoopPlot->isWater() && !kProject.isWater())
						||(pLoopPlot->isWater() && kProject.isWater()))
					{
						if(pLoopPlot->isBeingWorked())
						{
							if(pLoopPlot->doTerraformRitual(eRitual,getID(), iValue,bCountOnly))
							{
								iCounter++;

								//Darksavant
								//Previously the Resist Hostile Terraforming ability of some rituals listed a % chance of avoiding hostile terraforming but the code basically just ignored this and halved the terrain effected
								//regardless of how much resistance you had.  I modified it to actually have the combined listed chance of avoiding hostile terraforming effects.
								//Commented out the below in favor of % chance to avoid terraforming effects in the CvPlot->doTerraformRitual
								/* if (kProject.isHostileTerraform() && pLoopPlot->isOwned() && GET_PLAYER(pLoopPlot->getOwnerINLINE()).getResistHostileTerraforming()>0)
								{
									iCounter++;
								} */

								if(iCounter>=iTarget)
								{
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	if(iCounter<iTarget)
	{
		for(int iI=0;iI<GC.getMapINLINE().numPlotsINLINE();iI++)
		{
			pLoopPlot=GC.getMapINLINE().plotByIndexINLINE(iI);
			if(pLoopPlot->isOwned())
			{
				if((!kProject.isHostileTerraform() && pLoopPlot->getOwnerINLINE()==getID())
					|| (kProject.isHostileTerraform() && GET_TEAM(getTeam()).isAtWar(pLoopPlot->getTeam())))
				{
					if((!pLoopPlot->isWater() && !kProject.isWater())
						||(pLoopPlot->isWater() && kProject.isWater()))
					{
						if(kProject.isHostileTerraform() ||
							(pLoopPlot->getWorkingCity()!=NULL && pLoopPlot->getWorkingCity()->getTeam()==getTeam()))
						{
							if(pLoopPlot->doTerraformRitual(eRitual,getID(), iValue,bCountOnly))
							{
								iCounter++;

								/* if(kProject.isHostileTerraform() && pLoopPlot->isOwned() && GET_PLAYER(pLoopPlot->getOwnerINLINE()).getResistHostileTerraforming()>0)
								{
									iCounter++;
								} */

								if(iCounter>=iTarget)
								{
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	if(iCounter<iTarget)
	{

		for(int iI=0;iI<GC.getMapINLINE().numPlotsINLINE();iI++)
		{
			pLoopPlot=GC.getMapINLINE().plotByIndexINLINE(iI);
			if(pLoopPlot->isOwned())
			{
				if((!kProject.isHostileTerraform() && pLoopPlot->getOwnerINLINE()==getID())
					|| (kProject.isHostileTerraform() && GET_TEAM(getTeam()).isAtWar(pLoopPlot->getTeam())))
				{
					if((!pLoopPlot->isWater() && !kProject.isWater())
						||(pLoopPlot->isWater() && kProject.isWater()))

					{
						if(pLoopPlot->doTerraformRitual(eRitual,getID(), iValue,bCountOnly))
						{
							iCounter++;

							/* if(kProject.isHostileTerraform() && pLoopPlot->isOwned() && GET_PLAYER(pLoopPlot->getOwnerINLINE()).getResistHostileTerraforming()>0)
							{
								iCounter++;
							} */

							if(iCounter>=iTarget)
							{
								break;
							}
						}
					}
				}
			}
		}
	}

	if(!bCountOnly)
	{
		changeFaith(-kProject.getFaithCost());
		//gDLL->getEngineIFace()->RebuildAllPlots();

		CvWString szBuffer = gDLL->getText("TXT_KEY_TERRAFORM_RITUAL", GC.getProjectInfo(eRitual).getDescription());
		gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_WELOVEKING", MESSAGE_TYPE_MINOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_HAPPY_PERSON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), -1, -1, true, true);
	}

	return iValue;
}

int CvPlayer::getSpellManaCost(SpellTypes eSpell, CvUnit* pCaster)
{
	CvSpellInfo& kSpell=GC.getSpellInfo(eSpell);
	int iCost=kSpell.getManaCost();
	int iModifier=100;
	TechTypes eTechPrereq=(TechTypes)kSpell.getTechPrereq();

	if(eTechPrereq!=NO_TECH && GC.getTechInfo(eTechPrereq).isSpellResearch())
	{
		if(GC.getTechInfo(eTechPrereq).getSpellSchool()!=NO_MANASCHOOL)
		{
			iModifier+=getManaSchoolCostModifier(GC.getTechInfo(eTechPrereq).getSpellSchool());
		}
		if(GC.getTechInfo(eTechPrereq).getPrereqBonus()!=NO_BONUS)
		{
			if(kSpell.isManaCostReductionBonus())
			{
				iModifier-=10*std::min(3,std::max(0,getNumAvailableBonuses((BonusTypes)GC.getTechInfo(eTechPrereq).getPrereqBonus())-1));
			}
		}
	}

	iCost *= iModifier;
	iCost /= 100;

	iCost *= std::max(50, 100 - GC.getCivilizationInfo(getCivilizationType()).getReducedManaCostInGoldenAge());
	iCost /= 100;

	if (getDisableSpellcasting() > 0)
    {
		iCost *= 2;
    }

	iCost*=100+(GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getConstructPercent()-100)/4;
    iCost/=100;

	if(pCaster!=NULL)
	{
		iModifier=100;
		for(int iI=0;iI<GC.getNumPromotionInfos();iI++)
		{
			if(pCaster->isHasPromotion((PromotionTypes)iI))
			{
				iModifier-=GC.getPromotionInfo((PromotionTypes)iI).getReducedManaCost();
			}
		}
		iCost*=iModifier;
		iCost/=100;
	}

	return iCost;
}

//return a Value between 0 and iMaximumConversionRate
int CvPlayer::getProductionIncreaseByYield(YieldTypes eYield, const CvCity* pCity, BuildingTypes eBuilding, UnitTypes eUnit)
{
	if(eYield<=YIELD_COMMERCE || eYield>GC.getNUM_YIELD_TYPES())
	{
		return 0;
	}

	int iLoop;
	CvCity* pLoopCity;

	int eTotalResource=getGlobalYield(eYield);
	int eTotalUsed=0;
	int iMaximumConversionRate=200;
	int iConversionRatio=200;

	for (pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		if(pCity!=pLoopCity)
		{
			if(pLoopCity->isProductionBuilding() && GC.getBuildingInfo(pLoopCity->getProductionBuilding()).getReducedCostByYield()==eYield)
			{
				//eTotalUsed+=pLoopCity->getBaseYieldRate(YIELD_PRODUCTION);
				eTotalUsed+=std::max(0,pLoopCity->getProductionNeeded(pLoopCity->getProductionBuilding())-pLoopCity->getBuildingProduction(pLoopCity->getProductionBuilding()));
			}

			else if(pLoopCity->isProductionUnit() && GC.getUnitInfo(pLoopCity->getProductionUnit()).getReducedCostByYield()==eYield)
			{
				//eTotalUsed+=pLoopCity->getBaseYieldRate(YIELD_PRODUCTION);
				eTotalUsed+=std::max(0,pLoopCity->getProductionNeeded(pLoopCity->getProductionUnit())-pLoopCity->getUnitProduction(pLoopCity->getProductionUnit()));
			}
		}
	}
	
	if(pCity!=NULL)
	{
		if(eBuilding!=NO_BUILDING && GC.getBuildingInfo(eBuilding).getReducedCostByYield()==eYield)
		{
			//eTotalUsed+=pCity->getBaseYieldRate(YIELD_PRODUCTION);
			eTotalUsed+=std::max(0,pCity->getProductionNeeded(eBuilding)-pCity->getBuildingProduction(eBuilding));
		}
		else if(pCity->isProductionBuilding() && GC.getBuildingInfo(pCity->getProductionBuilding()).getReducedCostByYield()==eYield)
		{
			eTotalUsed+=std::max(0,pCity->getProductionNeeded(pCity->getProductionBuilding())-pCity->getBuildingProduction(pCity->getProductionBuilding()));
		}

		if(eUnit!=NO_UNIT && GC.getUnitInfo(eUnit).getReducedCostByYield()==eYield)
		{
			//eTotalUsed+=pCity->getBaseYieldRate(YIELD_PRODUCTION);
			eTotalUsed+=std::max(0,pCity->getProductionNeeded(eUnit)-pCity->getUnitProduction(eUnit));
		}
		else if(pCity->isProductionUnit() && GC.getUnitInfo(pCity->getProductionUnit()).getReducedCostByYield()==eYield)
		{
			eTotalUsed+=std::max(0,pCity->getProductionNeeded(pCity->getProductionUnit())-pCity->getUnitProduction(pCity->getProductionUnit()));
		}
	}

	eTotalUsed=(eTotalUsed*iConversionRatio)/100;
	eTotalUsed=std::max(1,eTotalUsed);

	return std::min(iMaximumConversionRate,(10*eTotalResource)/eTotalUsed);
}

int CvPlayer::getFlavorValue(int eFlavor) const
{	
	return std::max(GC.getCivilizationInfo(getCivilizationType()).getFlavorValue(eFlavor),GC.getLeaderHeadInfo(getLeaderType()).getFlavorValue(eFlavor));
}

/** Allow to modify the CivilizationType of a Player **/

void CvPlayer::setCivilizationType(CivilizationTypes newCivType)
{
	FAssert(newCivType != NO_CIVILIZATION);

    if (newCivType==NO_CIVILIZATION || getCivilizationType()==newCivType)
        return;

	BuildingClassTypes ePalaceClass=(BuildingClassTypes)GC.getBUILDINGCLASS_PALACE();
	if(getCapitalCity()!=NULL && ePalaceClass!=NO_BUILDINGCLASS)
	{
		getCapitalCity()->setNumRealBuilding((BuildingTypes)GC.getCivilizationInfo(getCivilizationType()).getCivilizationBuildings(ePalaceClass),0);
		getCapitalCity()->setNumRealBuilding((BuildingTypes)GC.getCivilizationInfo(newCivType).getCivilizationBuildings(ePalaceClass),1);
	}

    GC.getInitCore().setCiv(getID(),newCivType);
    GC.getInitCore().setCivDescription(getID(), GC.getCivilizationInfo(newCivType).getDescription());
    GC.getInitCore().setArtStyle(getID(), (ArtStyleTypes)GC.getCivilizationInfo(newCivType).getArtStyleType());

    setPopulationLimit(GC.getCivilizationInfo(newCivType).getPopulationLimit());
}

/** BETTER AI End **/
/*********************************************************************/

//>>>>BUGFfH: Added by Denev 2009/09/29
UnitArtStyleTypes CvPlayer::getUnitArtStyleType() const
{
	int iUnitArtStyle = NO_UNIT_ARTSTYLE;

	int iDefaultRace = GC.getCivilizationInfo(getCivilizationType()).getDefaultRace();
	if (iDefaultRace != NO_PROMOTION)
	{
		iUnitArtStyle = GC.getPromotionInfo((PromotionTypes)iDefaultRace).getUnitArtStyleType();
	}
	if (iUnitArtStyle == NO_UNIT_ARTSTYLE)
	{
		iUnitArtStyle = GC.getCivilizationInfo(getCivilizationType()).getUnitArtStyleType();
	}

	return (UnitArtStyleTypes)iUnitArtStyle;
}
//<<<<BUGFfH: End Add

/*************************************************************************************************/
/**	New Gameoption (Passive XP Training) Sephi                                				    **/
/*************************************************************************************************/
int CvPlayer::getTrainXPCap(int iI)
{
	if (iI < 0 || iI > GC.getNumUnitCombatInfos()) return 0;
    return m_paiTrainXPCap[iI];
}
int CvPlayer::getTrainXPRate(int iI)
{
	if (iI < 0 || iI > GC.getNumUnitCombatInfos()) return 0;
    return m_paiTrainXPRate[iI];
}

void CvPlayer::changeTrainXPCap(int iChange, int iI)
{
	if (iI >= 0 && iI <= GC.getNumUnitCombatInfos())
	{
        m_paiTrainXPCap[iI] += iChange;
    }
}
void CvPlayer::changeTrainXPRate(int iChange, int iI)
{
	if (iI >= 0 && iI <= GC.getNumUnitCombatInfos())
	{
        m_paiTrainXPRate[iI] += iChange;
    }
}

/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	ADDON (Houses of Erebus) Sephi			                                 					**/
/**																								**/
/**						                                            							**/
/*************************************************************************************************/
int CvPlayer::getCorporationSupport(CorporationTypes eCorporation) const
{
	if (m_pabCorporationSupport[eCorporation])
		return m_paiCorporationSupport[eCorporation];

	return 0;
}

int CvPlayer::getCorporationSupportMultiplier(CorporationTypes eCorporation) const
{
	if (m_pabCorporationSupport[eCorporation])
		return m_paiCorporationSupportMod[eCorporation];

	return 0;
}

bool CvPlayer::isHasCorporationSupport(CorporationTypes eCorporation) const
{
	return m_pabCorporationSupport[eCorporation];
}

void CvPlayer::changeCorporationSupport(CorporationTypes eCorporation, int change)
{
	if(isHasCorporationSupport(eCorporation))
    {
        int iBonusValue=change;

        int iMultiplier=getCorporationSupportMultiplier(eCorporation);

        if(!isHuman())
        {
            iMultiplier+=GC.getHandicapInfo(GET_PLAYER((PlayerTypes)0).getHandicapType()).getAIFreeNobleHouseSupportModifier();
        }

        if (change>0)
        {
            iBonusValue*=(100+iMultiplier);
        }
        else
        {
            if (getCorporationSupportMultiplier(eCorporation)<=-100)
            {
                iBonusValue*=200;
            }
            else
            {
                iBonusValue*=10000/(100+iMultiplier);
            }
        }
        iBonusValue/=100;
        m_paiCorporationSupport[eCorporation]=range(m_paiCorporationSupport[eCorporation]+iBonusValue,0,10000);
    }
    else if (GC.getGameINLINE().getSorenRandNum(10000,"offer Support")<change)
    {
        if(GET_TEAM(getTeam()).isHasTech((TechTypes)GC.getCorporationInfo(eCorporation).getTechPrereq()))
        {
            //offer Support
            //allow Python to Handle this
            CyArgsList argsList;
            argsList.add(getID());
            argsList.add(eCorporation);
            long lResult=-1;
            gDLL->getPythonIFace()->callFunction(PYGameModule, "CorporationOffersSupport", argsList.makeFunctionArgs(), &lResult);
        }
    }
}

void CvPlayer::changeCorporationSupportMultiplier(CorporationTypes eCorporation, int change)
{
	if(isHasCorporationSupport(eCorporation))
	{
        m_paiCorporationSupportMod[eCorporation]+=change;
	}
}

void CvPlayer::setHasCorporationSupport(CorporationTypes eCorporation, bool bNewValue)
{
    //BonusTrait Tag
    TraitTypes iBonusTrait=(TraitTypes)GC.getCorporationInfo(eCorporation).getBonusTrait();
    if(iBonusTrait!=NO_TRAIT)
    {
        if (bNewValue)
        {
            setHasTrait(iBonusTrait,true);
        }
        else
        {
            setHasTrait(iBonusTrait,false);
        }
    }

	m_pabCorporationSupport[eCorporation]=bNewValue;
}

/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	ADDON (Adventures) Sephi                                                      				**/
/*************************************************************************************************/
bool CvPlayer::isAdventureEnabled(int iAdventure) const
{
	FAssert(iAdventure >= 0 && iAdventure < GC.getNumAdventureInfos());
	return m_pabAdventureEnabled[iAdventure];
}

bool CvPlayer::isAdventureFinished(int iAdventure) const
{
	FAssert(iAdventure >= 0 && iAdventure < GC.getNumAdventureInfos());
	return m_pabAdventureFinished[iAdventure];
}

int CvPlayer::getAdventureStatus(int iAdventure, int iStep) const
{
	FAssert(iAdventure >= 0 && iAdventure < GC.getNumAdventureInfos());
	FAssert(iStep >= 0 && iStep < GC.getMAX_ADVENTURESTEPS());
	return m_ppaaiAdventureCounter[iAdventure][iStep];
}

//returns the AdventureCounter of iAdventure/iStep Combo
int CvPlayer::getAdventureLimit(int iAdventure, int iStep) const
{
	FAssert(iAdventure >= 0 && iAdventure < GC.getNumAdventureInfos());
	FAssert(iStep >= 0 && iStep < GC.getMAX_ADVENTURESTEPS());

	int iMod=100;

	if(GC.getAdventureStepInfo((AdventureStepTypes)GC.getAdventureInfo((AdventureTypes)iAdventure).getAdventureStep(iStep)).isCivicDuration()
		|| GC.getAdventureStepInfo((AdventureStepTypes)GC.getAdventureInfo((AdventureTypes)iAdventure).getAdventureStep(iStep)).isLeadOvercouncil()
		|| GC.getAdventureStepInfo((AdventureStepTypes)GC.getAdventureInfo((AdventureTypes)iAdventure).getAdventureStep(iStep)).isLeadUndercouncil()
		)
	{
		iMod=GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getGrowthPercent();
	}

	return (GC.getAdventureInfo((AdventureTypes)iAdventure).getAdventureCounter(iStep)*iMod)/100;
}

void CvPlayer::setAdventureEnabled(int iAdventure, bool bNewValue)
{
	FAssert(iAdventure >= 0 && iAdventure < GC.getNumAdventureInfos());
	m_pabAdventureEnabled[iAdventure]=bNewValue;
}
void CvPlayer::setAdventureFinished(int iAdventure, bool bNewValue)
{
	FAssert(iAdventure >= 0 && iAdventure < GC.getNumAdventureInfos());
	m_pabAdventureFinished[iAdventure]=bNewValue;
}
void CvPlayer::changeAdventureCounter(int iAdventure, int iStep, int iChange)
{
	FAssert(iAdventure >= 0 && iAdventure < GC.getNumAdventureInfos());
	FAssert(iStep >= 0 && iStep < GC.getMAX_ADVENTURESTEPS());
	m_ppaaiAdventureCounter[iAdventure][iStep]+=iChange;
	if(iChange>0)
	{
	    VerifyFinishedAdventure(iAdventure);
	}
}

void CvPlayer::VerifyEnabledAdventures()
{
    for (int iI=0;iI<GC.getNumAdventureInfos();iI++)
    {
		CvAdventureInfo& kAdventure=GC.getAdventureInfo((AdventureTypes)iI);
		if(!isAdventureEnabled(iI) && !kAdventure.isDisabled())
        {
            bool bValid=false;
            bool bFirst=false;
            for(int iJ=0;iJ<GC.getNumCivilizationInfos();iJ++)
            {
                if(kAdventure.isPrereqCiv((CivilizationTypes)iJ))
                {
                    bFirst=true;
                    if(iJ==getCivilizationType())
                    {
                        bValid=true;
                        break;
                    }
                }
            }

            //Adventure has Prereqcivs, but Player has not the right Civ
            if(bFirst && !bValid)
                continue;

            bValid=false;
            bFirst=false;

            for(int iJ=0;iJ<GC.getNumLeaderHeadInfos();iJ++)
            {
                if(kAdventure.isPrereqLeader((LeaderHeadTypes)iJ))
                {
                    bFirst=true;
                    if(iJ==getLeaderType())
                    {
                        bValid=true;
                        break;
                    }
                }
            }

            //Adventure has PrereqLeaders, but Player has not the right Leader
            if(bFirst && !bValid)
                continue;

            bool bNoMatch=false;
            for (int iJ=0;iJ<GC.getNumCivilizationInfos();iJ++)
            {
                if(kAdventure.isPrereqCivMet((CivilizationTypes)iJ))
                {
                    bValid=false;
                    for(int iK=0;iK<GC.getMAX_CIV_PLAYERS();iK++)
                    {
                        if(GET_PLAYER((PlayerTypes)iK).isAlive() && GET_PLAYER((PlayerTypes)iK).getTeam()!=getTeam() && GET_PLAYER((PlayerTypes)iK).getCivilizationType()==iJ)
                        {
                            if(GET_TEAM(getTeam()).isHasMet(GET_PLAYER((PlayerTypes)iK).getTeam()))
                            {
                                bValid=true;
                            }
                        }
                    }
                    if(!bValid)
                    {
                        bNoMatch=true;
                        break;
                    }
                }
            }

            //Prereq for Adventure is to met a Civ, which did not happen so far
            if (bNoMatch)
                continue;

            for (int iJ=0;iJ<GC.getNumReligionInfos();iJ++)
            {
                if(kAdventure.isPrereqReligionFounded((ReligionTypes)iJ))
                {
                    if(!GC.getGameINLINE().isReligionFounded((ReligionTypes)iJ))
                    {
                        bNoMatch=true;
                        break;
                    }
                }
            }

            //Prereq for Adventure is that a Religion must have been founded, which did not happen so far
            if (bNoMatch)
                continue;

            if(kAdventure.getPrereqAC()!=-1)
            {
                if(GC.getGameINLINE().getGlobalCounter()<kAdventure.getPrereqAC())
                {
                    continue;
                }
            }

            CvWString szBuffer;
			szBuffer = gDLL->getText("TXT_KEY_ADVENTURE_ADDED", kAdventure.getDescription());
			gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_WELOVEKING", MESSAGE_TYPE_MINOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_HAPPY_PERSON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), -1, -1, true, true);

            setAdventureEnabled(iI,true);
        }
    }
}

bool CvPlayer::VerifyAdventureProgress(int iAdventureStep, CvUnit* pUnit, int iPlayerType, CvCity* pCity)
{
    AdventureStepTypes eStep = (AdventureStepTypes)iAdventureStep;

    if(pUnit!=NULL)
    {
        if(GC.getAdventureStepInfo(eStep).isNoBarBarian() && pUnit->isBarbarian())
        {
            return false;
        }
    }

    if(iPlayerType!=NO_PLAYER)
    {
        if(GC.getAdventureStepInfo(eStep).getCivilizationType()!=NO_CIVILIZATION && GET_PLAYER((PlayerTypes)iPlayerType).getCivilizationType()!=GC.getAdventureStepInfo(eStep).getCivilizationType())
        {
            return false;
        }
    }

    return true;
}

void CvPlayer::VerifyFinishedAdventure(int iAdventure)
{
	if(isAdventureFinished(iAdventure))
	{
		return;
	}

    AdventureTypes eAdv = (AdventureTypes)iAdventure;

    for(int iJ=0;iJ<GC.getMAX_ADVENTURESTEPS();iJ++)
    {
        if(GC.getAdventureInfo((AdventureTypes)eAdv).getAdventureStep(iJ)!=NO_ADVENTURESTEP)
        {
            if(getAdventureStatus(eAdv,iJ)<getAdventureLimit(eAdv,iJ))
            {
                return;
            }
        }
    }

    CvWString szBuffer;
    szBuffer = gDLL->getText("TXT_KEY_ADVENTURE_FINISHED", GC.getAdventureInfo((AdventureTypes)iAdventure).getDescription());
    gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szBuffer, "AS2D_WELOVEKING", MESSAGE_TYPE_MINOR_EVENT, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_HAPPY_PERSON")->getPath(), (ColorTypes)GC.getInfoTypeForString("COLOR_WHITE"), -1, -1, true, true);

    setAdventureFinished(iAdventure,true);

	//allow Python to Handle this
    CyArgsList argsList;
	argsList.add(getID());		// pass in this players ID
	argsList.add(iAdventure);
    long lResult=-1;
    gDLL->getPythonIFace()->callFunction(PYGameModule, "AdventureFinished", argsList.makeFunctionArgs(), &lResult);

}

void CvPlayer::VerifyFinishedAdventures()
{
    for (int iI=0;iI<GC.getNumAdventureInfos();iI++)
    {
		if(!isAdventureFinished((AdventureTypes)iI) && isAdventureEnabled((AdventureTypes)iI))
		{
			VerifyFinishedAdventure(iI);
		}
    }
}

/*************************************************************************************************/
/**	    									END													**/
/*************************************************************************************************/

/*************************************************************************************************/
/**	ADDON (AIGroups) Sephi																		**/
/*************************************************************************************************/
CvAIGroup* CvPlayer::initAIGroup(int iGroupType)
{
	PROFILE_FUNC();

	CvAIGroup* pAIGroup;

	pAIGroup = addAIGroup();

	FAssertMsg(pAIGroup != NULL, "AIGroup is not assigned a valid value");

	pAIGroup->init(pAIGroup->getID(), getID(), iGroupType);

	return pAIGroup;
}

CvAIGroup* CvPlayer::getAIGroup(int iID) const
{
	return(m_AIGroups.getAt(iID));
}


CvAIGroup* CvPlayer::addAIGroup()
{
	return(m_AIGroups.add());
}


void CvPlayer::deleteAIGroup(int iID)
{
	m_AIGroups.removeAt(iID);
}

CvAIGroup* CvPlayer::firstAIGroup(int *pIterIdx, bool bRev) const
{
	return !bRev ? m_AIGroups.beginIter(pIterIdx) : m_AIGroups.endIter(pIterIdx);
}


CvAIGroup* CvPlayer::nextAIGroup(int *pIterIdx, bool bRev) const
{
	return !bRev ? m_AIGroups.nextIter(pIterIdx) : m_AIGroups.prevIter(pIterIdx);
}

/*************************************************************************************************/
/**	    									END													**/
/*************************************************************************************************/

/*************************************************************************************************/
/**	ADDON (Combatauras) Sephi																	**/
/*************************************************************************************************/
CvCombatAura* CvPlayer::initCombatAura(int iUnitID, int iCombatAuraType)
{
	PROFILE_FUNC();

	CvCombatAura* pCombatAura;

	pCombatAura = addCombatAura();

	FAssertMsg(pCombatAura != NULL, "CombatAura is not assigned a valid value");

	pCombatAura->init(pCombatAura->getID(), getID(), iUnitID, iCombatAuraType);

	return pCombatAura;
}

CvCombatAura* CvPlayer::getCombatAura(int iID) const
{
	return(m_combatAuras.getAt(iID));
}


CvCombatAura* CvPlayer::addCombatAura()
{
	return(m_combatAuras.add());
}


void CvPlayer::deleteCombatAura(int iID)
{
	m_combatAuras.removeAt(iID);
}

void CvPlayer::doTurnRemoveCombatAuras()
{
	int iLoop;

	for(CvUnit* pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{
		if(pLoopUnit->getBlessing1()!=NULL)
		{
			if(!pLoopUnit->getBlessing1()->isValid(pLoopUnit))
			{
				pLoopUnit->getBlessing1()->Remove(pLoopUnit);
				pLoopUnit->setBlessing1(NULL);
			}
		}

		if(pLoopUnit->getBlessing2()!=NULL)
		{
			if(!pLoopUnit->getBlessing2()->isValid(pLoopUnit))
			{
				pLoopUnit->getBlessing2()->Remove(pLoopUnit);
				pLoopUnit->setBlessing2(NULL);
			}
		}

		if(pLoopUnit->getCurse1()!=NULL)
		{
			if(!pLoopUnit->getCurse1()->isValid(pLoopUnit))
			{
				pLoopUnit->getCurse1()->Remove(pLoopUnit);
				pLoopUnit->setCurse1(NULL);
			}
		}

		if(pLoopUnit->getCurse2()!=NULL)
		{
			if(!pLoopUnit->getCurse2()->isValid(pLoopUnit))
			{
				pLoopUnit->getCurse2()->Remove(pLoopUnit);
				pLoopUnit->setCurse2(NULL);
			}
		}
	}
}

void CvPlayer::doTurnApplyCombatAuras()
{
	int iLoop;

	for (int iPass=4; iPass>0; iPass--)
	{
		for(CvUnit* pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
		{
			if(pLoopUnit->getCombatAura()!=NULL)
			{				
				CvCombatAura* pAura=pLoopUnit->getCombatAura();
				if(pAura->getTier()==iPass)
				{
					if(pAura->getTargets()==-1 || pAura->getTargets()>pAura->getTargetsApplied())
					{
						int iDX, iDY;
						int iRange=pAura->getRange();
						int iAuraType=GC.getCombatAuraInfo(pAura->getCombatAuraType()).getCombatAuraClassType();

						bool bBlessing =GC.getCombatAuraInfo((CombatAuraTypes)pLoopUnit->getCombatAuraType()).isBlessing();

						for (iDX = -(iRange); iDX <= iRange; iDX++)
						{
							for (iDY = -(iRange); iDY <= iRange; iDY++)
							{
								CvPlot* pLoopPlot= plotXY(pLoopUnit->getX_INLINE(), pLoopUnit->getY_INLINE(), iDX, iDY);

								if (pLoopPlot != NULL)
								{
									if(bBlessing && pLoopPlot->getNumUnits()>0)
									{
										CLLNode<IDInfo>* pUnitNode;
										CvUnit* pLoopPlotUnit;
										pUnitNode = pLoopPlot->headUnitNode();
										while (pUnitNode != NULL)
										{
											pLoopPlotUnit = ::getUnit(pUnitNode->m_data);
											pUnitNode = pLoopPlot->nextUnitNode(pUnitNode);

											if(pAura->getTargets()>pAura->getTargetsApplied())
											{										
												if (pLoopPlotUnit->canApplyCombatAura(pAura))
												{
													if(pLoopPlotUnit->getBlessing1()==NULL)
													{
														pLoopPlotUnit->setBlessing1(pAura);
														pAura->Apply(pLoopPlotUnit);
													}
													else if(pLoopPlotUnit->getBlessing2()==NULL)
													{
														pLoopPlotUnit->setBlessing2(pAura);
														pAura->Apply(pLoopPlotUnit);
													}												
												}
												else if (pLoopPlotUnit->canApplyCombatAura(pAura,true))
												{
													if(pLoopPlotUnit->getBlessing1()!=NULL)
													{
														CvCombatAuraInfo &kAura=GC.getCombatAuraInfo(pAura->getCombatAuraType());
														CvCombatAuraInfo &kAura1=GC.getCombatAuraInfo(pLoopPlotUnit->getBlessing1()->getCombatAuraType());

														if(kAura.getCombatAuraClassType()==kAura1.getCombatAuraClassType() && 
															kAura.getTier()>kAura1.getTier())
														{
															pLoopPlotUnit->getBlessing1()->Remove(pLoopPlotUnit);
															pLoopPlotUnit->setBlessing1(pAura);
															pAura->Apply(pLoopPlotUnit);
														}

														else if(pLoopPlotUnit->getBlessing2()!=NULL)
														{																							
															CvCombatAuraInfo &kAura2=GC.getCombatAuraInfo(pLoopPlotUnit->getBlessing2()->getCombatAuraType());

															if(kAura.getCombatAuraClassType()==kAura2.getCombatAuraClassType() && 
																kAura.getTier()>kAura2.getTier())
															{
																pLoopPlotUnit->getBlessing2()->Remove(pLoopPlotUnit);
																pLoopPlotUnit->setBlessing2(pAura);
																pAura->Apply(pLoopPlotUnit);
															}
														}
													}
												}
											}
										}
									}

									else if(pLoopPlot->isVisibleEnemyUnit(getID()))
									{
										CLLNode<IDInfo>* pUnitNode;
										CvUnit* pLoopPlotUnit;
										pUnitNode = pLoopPlot->headUnitNode();
										while (pUnitNode != NULL)
										{
											pLoopPlotUnit = ::getUnit(pUnitNode->m_data);
											pUnitNode = pLoopPlot->nextUnitNode(pUnitNode);
										
											if (pLoopPlotUnit->canApplyCombatAura(pAura))
											{
												if(pLoopPlotUnit->getCurse1()==NULL)
												{
													pLoopPlotUnit->setCurse1(pAura);
													pAura->Apply(pLoopPlotUnit);
												}
												else if(pLoopPlotUnit->getCurse2()==NULL)
												{
													pLoopPlotUnit->setCurse2(pAura);
													pAura->Apply(pLoopPlotUnit);
												}
											}
											else if (pLoopPlotUnit->canApplyCombatAura(pAura,true))
											{
												if(pLoopPlotUnit->getCurse1()!=NULL)
												{
													CvCombatAuraInfo &kAura=GC.getCombatAuraInfo(pAura->getCombatAuraType());
													CvCombatAuraInfo &kAura1=GC.getCombatAuraInfo(pLoopPlotUnit->getCurse1()->getCombatAuraType());

													if(kAura.getCombatAuraClassType()==kAura1.getCombatAuraClassType() && 
														kAura.getTier()>kAura1.getTier())
													{
														pLoopPlotUnit->getCurse1()->Remove(pLoopPlotUnit);
														pLoopPlotUnit->setCurse1(pAura);
														pAura->Apply(pLoopPlotUnit);
													}

													else if(pLoopPlotUnit->getBlessing2()!=NULL)
													{																							
														CvCombatAuraInfo &kAura2=GC.getCombatAuraInfo(pLoopPlotUnit->getBlessing2()->getCombatAuraType());

														if(kAura.getCombatAuraClassType()==kAura2.getCombatAuraClassType() && 
															kAura.getTier()>kAura2.getTier())
														{
															pLoopPlotUnit->getCurse2()->Remove(pLoopPlotUnit);
															pLoopPlotUnit->setCurse2(pAura);
															pAura->Apply(pLoopPlotUnit);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

/*************************************************************************************************/
/**	END	                                        												**/
/*************************************************************************************************/

bool CvPlayer::AI_isUnitCategory(UnitTypes eUnit, UnitCategoryTypes eUnitCategory) const
{
	CvUnitInfo &kUnit=GC.getUnitInfo(eUnit);

	switch(eUnitCategory)
	{
		case UNITCATEGORY_SIEGE:
			if(kUnit.getDomainType()==DOMAIN_LAND)
			{
				//land siege Units
				if(kUnit.getBombardRange()>0 && kUnit.getBombardRate())
				{
					return true;
				}

				//Units with fireball ability
				for(int iI=0;iI<GC.getNumSpellInfos();iI++)
				{
					if(GC.getSpellInfo((SpellTypes)iI).isBombard())
					{
						if(GC.getSpellInfo((SpellTypes)iI).getUnitPrereq()==eUnit)
						{
							if(GC.getSpellInfo((SpellTypes)iI).getTechPrereq()==NO_TECH || GET_TEAM(getTeam()).isHasTech((TechTypes)GC.getSpellInfo((SpellTypes)iI).getTechPrereq()))
							{
								return true;
							}
						}
					}
				}
			}
			break;
		case UNITCATEGORY_WARWIZARD:
			//Darksavant added Disciples to this category as well to hopefully get them casting
			if(kUnit.getArcaneRange() > 0 && (kUnit.getUnitCombatType() == GC.getUNITCOMBAT_ADEPT() || kUnit.getUnitCombatType() == GC.getInfoTypeForString("UNITCOMBAT_DISCIPLE")))
			{
				return true;
			}
			break;
		case UNITCATEGORY_COMBAT:
			if(kUnit.getCombat()>0 && kUnit.getDomainType()==DOMAIN_LAND)
			{
				return true;
			}
			break;
		case UNITCATEGORY_COMBAT_NAVAL:
			if(kUnit.getCombat()>0 && kUnit.getDomainType()==DOMAIN_SEA)
			{
				return true;
			}
			break;
		case UNITCATEGORY_TRANSPORT_NAVAL:
			if(kUnit.getCargoSpace()>0 && kUnit.getDomainType()==DOMAIN_SEA)
			{
				return true;
			}
			break;
		case UNITCATEGORY_SETTLE:
			if(GC.getUnitInfo(eUnit).isFound())
			{
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}

bool CvPlayer::AI_canUseMoreOfUnitCategory(UnitCategoryTypes eUnitCategory, int iArea)
{
	int iUnitsNeeded=0;
	switch(eUnitCategory)
	{
		case UNITCATEGORY_WARWIZARD:
			iUnitsNeeded+=3;
			iUnitsNeeded-=CountUnitCategoryPlusMaking(eUnitCategory,iArea);
			return (iUnitsNeeded>0);
			break;
		default:
			break;
	}
	return false;
}

int CvPlayer::CountUnitCategoryPlusMaking(UnitCategoryTypes eUnitCategory, int iArea)
{
	int iCount=0;
	int iLoop;

	for (CvUnit* pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
	{
		if(pLoopUnit->getArea()==iArea)
		{
			if(AI_isUnitCategory(pLoopUnit->getUnitType(),eUnitCategory))
			{
				iCount++;
			}
		}
	}

	iCount+=CountUnitCategoryinMaking(eUnitCategory,iArea);

	return iCount;
}

int CvPlayer::CountUnitCategoryinMaking(UnitCategoryTypes eUnitCategory, int iArea)
{
	int iCount=0;
	int iLoop;

	for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		if(pLoopCity->getArea()==iArea)
		{
			UnitTypes eUnit=pLoopCity->getProductionUnit();
			if(eUnit!=NO_UNIT)
			{
				if(AI_isUnitCategory(eUnit,eUnitCategory))
				{
					iCount++;
				}
			}
		}
	}

	return iCount;
}

int CvPlayer::CountBuildingPlusMakinginArea(BuildingTypes eBuilding, int iArea)
{
	if(eBuilding==NO_BUILDING)
	{
		return 0;
	}
	int iCount=0;
	int iLoop;

	for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
	{
		if(pLoopCity->getArea()==iArea)
		{
			if(pLoopCity->getNumRealBuilding(eBuilding)>0 ||
				(pLoopCity->isProductionBuilding() && pLoopCity->getProductionBuilding()==eBuilding))
			{
				iCount++;
			}
		}
	}

	return iCount;
}

void CvPlayer::initCanEverSpellResearch()
{
	/**
	for(int iI=0;iI<GC.getNumTechInfos();iI++)
	{
		CvTechInfo &kResearch=GC.getTechInfo((TechTypes)iI);
		if(kResearch.isSpellResearch())
		{
			if(kResearch.getSpellResearchLevel()==4 || kResearch.getSpellResearchLevel()==6)
			{
				if(GC.getGameINLINE().getSorenRandNum(100,"Unlock Spellresearch")<33)
				{
					setCanEverSpellResearch(iI,false);
				}
			}
		}
	}
	**/
}

//returns true if Player has researched all Spells from one Manaschool
bool CvPlayer::isSpellschoolResearchComplete(ManaschoolTypes eManaschool)
{
	bool bResearchFinished=true;
	for(int iJ=0;iJ<GC.getNumTechInfos();iJ++)
	{
		if(GC.getTechInfo((TechTypes)iJ).isSpellResearch() && GC.getTechInfo((TechTypes)iJ).getSpellSchool()==eManaschool)
		{
			if(isCanEverSpellResearch(iJ) && !GET_TEAM(getTeam()).isHasTech((TechTypes)iJ))
			{
				bResearchFinished=false;
				break;
			}
		}
	}
	
	return bResearchFinished;
}

//returns Bonus Damage for a Spell
int CvPlayer::getBonusSpellDamage(int iSpell)
{
	if(iSpell==NO_SPELL)
	{
		return 0;
	}

	int iBonusDamage=0;
	CvSpellInfo& kSpell=GC.getSpellInfo((SpellTypes)iSpell);

	if(kSpell.getDamageType()!=NO_DAMAGE)
	{
		iBonusDamage+=getSpellDamageModify(kSpell.getDamageType());
	}

	if(kSpell.isManaCostReductionBonus())
	{
		if(kSpell.getTechPrereq()!=NO_TECH)
		{
			if(GC.getTechInfo((TechTypes)kSpell.getTechPrereq()).getPrereqBonus()!=NO_BONUS)
			{
				iBonusDamage+=getNumAvailableBonuses((BonusTypes)GC.getTechInfo((TechTypes)kSpell.getTechPrereq()).getPrereqBonus());
			}
		}
	}

	return iBonusDamage;
}

int CvPlayer::getTerraformPlan()
{
	return m_iTerraformPlan;
}

void CvPlayer::setTerraformPlan(int iProject)
{
	m_iTerraformPlan=iProject;
}

bool CvPlayer::isIgnoreUnHealthFromImprovements() const
{
	return m_iIgnoreUnHealthFromImprovements > 0;
}

void CvPlayer::changeIgnoreUnHealthFromImprovements(int iChange)
{
	m_iIgnoreUnHealthFromImprovements += iChange;
}

int CvPlayer::getGlobalCulture() const
{
	return m_iGlobalCulture;
}

void CvPlayer::changeGlobalCulture(int iChange)
{
	if (iChange != 0)
	{
		m_iGlobalCulture+=iChange;

		if (getID() == GC.getGameINLINE().getActivePlayer())
		{
			gDLL->getInterfaceIFace()->setDirty(MiscButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(SelectionButtons_DIRTY_BIT, true);
			gDLL->getInterfaceIFace()->setDirty(GameData_DIRTY_BIT, true);
		}
	}
}

int CvPlayer::getExtraImprovementPerCity() const
{
	return m_iExtraImprovementPerCity;
}

void CvPlayer::changeExtraImprovementPerCity(int iChange)
{
	m_iExtraImprovementPerCity += iChange;
}

int CvPlayer::getExtraManaNodesPercent() const
{
	return m_iExtraManaNodesPercent;
}

void CvPlayer::changeExtraManaNodesPercent(int iChange)
{
	m_iExtraManaNodesPercent += iChange;
}

int CvPlayer::getReducedHurryCostBuilding() const
{
	return m_iReducedHurryCostBuilding;
}

void CvPlayer::changeReducedHurryCostBuilding(int iChange)
{
	m_iReducedHurryCostBuilding += iChange;
}

int CvPlayer::getExtraSupportUnitLimit() const
{
	return m_iExtraSupportUnitLimit;
}

void CvPlayer::changeExtraSupportUnitLimit(int iChange)
{
	m_iExtraSupportUnitLimit += iChange;
}

int CvPlayer::getExtraBuildingCapturedChance() const
{
	return m_iExtraBuildingCapturedChance;
}

void CvPlayer::changeExtraBuildingCapturedChance(int iChange)
{
	m_iExtraBuildingCapturedChance += iChange;
}

int CvPlayer::getTotalTraitsFromCulture() const
{
	return m_iTotalTraitsFromCulture;
}

void CvPlayer::changeTotalTraitsFromCulture(int iChange)
{
	m_iTotalTraitsFromCulture += iChange;
}

int CvPlayer::getTotalCityInfastructureLevel() const
{
	return m_iTotalCityInfastructureLevel;
}

void CvPlayer::changeTotalCityInfastructureLevel(int iChange)
{
	m_iTotalCityInfastructureLevel += iChange;
}

void CvPlayer::launchPickEquipmentPopup()
{
	CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_PICK_EQUIPMENT);
	if (NULL != pInfo)
	{
		gDLL->getInterfaceIFace()->addPopup(pInfo);
	}
}

void CvPlayer::killGlobalEnchantments()
{
	int iI, iJ;

	//isPlayer Last in the team?
	for(iI=0;iI<GC.getMAX_CIV_PLAYERS();iI++)
	{
		if(iI!=getID() && getTeam()==GET_PLAYER((PlayerTypes)iI).getTeam())
		{
			if(GET_PLAYER((PlayerTypes)iI).isAlive())
			{
				return;
			}
		}
	}

	for(iJ=0;iJ<GC.getMAX_CIV_TEAMS();iJ++)
	{
		TeamTypes eTeam=(TeamTypes)iJ;
		if(GET_TEAM(eTeam).isAlive())
		{
			for(iI=0;iI<GC.getNumProjectInfos();iI++)
			{
				if(GET_TEAM(getTeam()).getProjectCount((ProjectTypes)iI)>0)
				{
					if(GC.getProjectInfo((ProjectTypes)iI).isGlobalEnchantment() && GC.getProjectInfo((ProjectTypes)iI).isHostile())
					{
						RemoveGlobalEnchantment((ProjectTypes)iI,eTeam);
					}
				}

				if(GET_TEAM(eTeam).getProjectCount((ProjectTypes)iI)>0)
				{
					if(GC.getProjectInfo((ProjectTypes)iI).isGlobalEnchantment() && GC.getProjectInfo((ProjectTypes)iI).isHostile())
					{
						GET_PLAYER(GET_TEAM(eTeam).getLeaderID()).RemoveGlobalEnchantment((ProjectTypes)iI,getTeam());
					}
				}
			}
		}
	}
}

//if bTreasure is true, some checks will be ignored
bool CvPlayer::canEquipUnit(const CvUnit* pUnit, PromotionTypes Equipment, bool bTestVisible, bool bTreasure) const
{
	if(Equipment==NO_PROMOTION || pUnit==NULL)
	{
		return false;
	}

	if(pUnit->getDomainType()!=DOMAIN_LAND)
	{
		return false;
	}

	if(!pUnit->canAttack() && !pUnit->canDefend())
	{
		return false;
	}

	if(pUnit->isGarrision())
	{
		return false;
	}

	CvPromotionInfo& kPromotion=GC.getPromotionInfo(Equipment);
	int iTier=GC.getUnitInfo(pUnit->getUnitType()).getTier();
	EquipmentCategoryTypes eEquipmentCategory=(EquipmentCategoryTypes)kPromotion.getEquipmentCategory();

	if(!(kPromotion.isGear() || eEquipmentCategory!=NO_EQUIPMENTCATEGORY))
	{
		return false;
	}

	if(kPromotion.getDurabilityType() == NO_DURABILITY)
	{
		return false;
	}

	if(GC.getDurabilityInfo((DurabilityTypes)kPromotion.getDurabilityType()).getValidByTier() > iTier)
	{
		if(!pUnit->isEquipmentDurabilityValid((DurabilityTypes)kPromotion.getDurabilityType()))
		{
			return false;
		}
	}

	if(pUnit->isHasPromotion(Equipment))
	{
		return false;
	}

	if((kPromotion.getMaxLevel() != 0) && 
		(   kPromotion.getMaxLevel() <= pUnit->getLevel()
		 || pUnit->getExperienceNeeded(kPromotion.getMaxLevel()) <= pUnit->getExperience() ))
		return false;

	if(!bTreasure)
	{		
		if(pUnit->getLevel()<GC.getEquipmentCategoryInfo(eEquipmentCategory).getMinLevel())
		{
			return false;
		}

		if(kPromotion.getTechPrereq()!=NO_TECH && !GET_TEAM(getTeam()).isHasTech((TechTypes)kPromotion.getTechPrereq()))
		{
			return false;
		}

		if(kPromotion.getBonusPrereq()!=NO_BONUS && getNumAvailableBonuses((BonusTypes)kPromotion.getBonusPrereq())<1)
		{
			return false;
		}

		if(kPromotion.getAdventurePrereq()!=NO_ADVENTURE && !isAdventureFinished(kPromotion.getAdventurePrereq()))
		{
			return false;
		}

	}

	if(pUnit->getUnitCombatType()==NO_UNITCOMBAT)
	{
		if(!kPromotion.isAnyUnitCombat()) 
		{
			return false;
		}
	}
	else if(!kPromotion.getUnitCombat(pUnit->getUnitCombatType()))
	{
		return false;
	}

	if(kPromotion.getPrereqPromotion()!=NO_PROMOTION)
	{
		if(!pUnit->isHasPromotion((PromotionTypes)kPromotion.getPrereqPromotion()))
		{
			return false;
		}
	}

    int iNumPrereqs = kPromotion.getNumPrereqCivilizations();
    if (iNumPrereqs > 0)
    {
		bool bValid = false;
        for (int iI = 0; iI < iNumPrereqs; iI++)
		{
            if (getCivilizationType() == (CivilizationTypes)kPromotion.getPrereqCivilization(iI))
			{
                bValid = true;
			}
		}
        if (!bValid)
		{
            return false;
		}
	}

	//Don't allow Equipment that is worse!
	if(kPromotion.getEquipmentCategory()!=NO_EQUIPMENTCATEGORY)
	{
		PromotionTypes eCurrentEquipment=(PromotionTypes)pUnit->getEquipment((EquipmentCategoryTypes)kPromotion.getEquipmentCategory());
		if(eCurrentEquipment!=NO_PROMOTION)
		{
			if(kPromotion.getCombatPercent()>0)
			{
				if(GC.getPromotionInfo(eCurrentEquipment).getCombatPercent()>kPromotion.getCombatPercent())
				{
					return false;
				}
			}

			if(kPromotion.getCombatPercentDefense()>0)
			{
				if(GC.getPromotionInfo(eCurrentEquipment).getCombatPercentDefense()>kPromotion.getCombatPercentDefense())
				{
					return false;
				}
			}
		}
	}

	if(kPromotion.getStateReligionPrereq()!=NO_RELIGION)
	{
		if(kPromotion.getStateReligionPrereq()!=getStateReligion())
		{
			return false;
		}
	}

	if(kPromotion.getCombatPercent() > 0 || kPromotion.getCombatPercentDefense() > 0)
	{
		if(GC.getUnitInfo(pUnit->getUnitType()).isNoEquipment())
		{
			return false;
		}

		//TEMPFIX AI DO NOT WASTE RESOURCES ON TIER1
		if(!isHuman() && GC.getUnitInfo(pUnit->getUnitType()).getTier()==1)
		{
			if(!bTreasure)
			{
				return false;
			}
		}
	}

	if(!bTreasure)
	{
		if(kPromotion.getBonusPrereq() != NO_BONUS && kPromotion.isLimitedByNumberBonuses())
		{
			int iLoop;
			int iCount = 0;
			for (CvUnit* pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
			{
				if(pLoopUnit->isHasPromotion(Equipment))
				{
					iCount++;
				}
			}
			if(iCount >= getNumAvailableBonuses((BonusTypes)kPromotion.getBonusPrereq()))
			{
				return false;
			}
		}
	}

	if(bTreasure)  {
		//can't find Equipment if you already have some
		if(kPromotion.getEquipmentCategory() != NO_EQUIPMENTCATEGORY)  {
			if(pUnit->getEquipment((EquipmentCategoryTypes)kPromotion.getEquipmentCategory())!=NO_PROMOTION)
			{
				return false;
			}
		}

		//Block Enchantments - Quickfix
		if(kPromotion.getManaCost() > 0)  {
			return false;
		}
	}

	if(bTestVisible || bTreasure)  {
		return true;
	}

	if(kPromotion.getGoldCost()>0)
	{
		int iCost = kPromotion.getGoldCost();

		int iMod = (isHuman()) ? 100 : -100 + GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAITrainPercent() * 2;

		iMod -= getReducedEquipmentCost();
		iMod = std::max(iMod, 20);

		iCost *= iMod;
		iCost /= 100;

		if (getGold()<iCost)
		{
			return false;
		}
	}

	if(kPromotion.getManaCost()>0)
	{
		int iCost = kPromotion.getManaCost();

		int iMod = (isHuman()) ? 100 : -100 + GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAITrainPercent() * 2;

		iMod -= getReducedEquipmentCost();
		iMod = std::max(iMod, 20);

		iCost *= iMod;
		iCost /= 100;

		if (getMana()<iCost)
		{
			return false;
		}
	}

	if(kPromotion.getFaithCost()>0)
	{
		int iCost = kPromotion.getFaithCost();

		int iMod = (isHuman()) ? 100 : -100 + GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAITrainPercent() * 2;

		iMod -= getReducedEquipmentCost();
		iMod = std::max(iMod, 20);

		iCost *= iMod;
		iCost /= 100;

		if (getFaith()<iCost)
		{
			return false;
		}
	}

	if(kPromotion.getYieldCostType()!=NO_YIELD && kPromotion.getYieldCost(iTier)>0)
	{
		int iCost=kPromotion.getYieldCost(iTier);

		int iMod = (isHuman()) ? 100 : -100+GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAITrainPercent()*2;

		iMod -=getReducedEquipmentCost();
		iMod =std::max(iMod,20);

		iCost *= iMod;
		iCost /= 100;

		if(Equipment == GC.getInfoTypeForString("PROMOTION_POTION_STRONG"))
			iCost *= (pUnit->getStrengthPotionsUsed() + 1);


		if(getGlobalYield(kPromotion.getYieldCostType())<iCost)
		{
			return false;
		}
	}

	return true;
}

int CvPlayer::CalculateCivCounter(int iStep, int iValue)
{
	CyArgsList argsList;
    argsList.add(getID());
    argsList.add(iStep);
    argsList.add(iValue);
    long lResult=-1;
    gDLL->getPythonIFace()->callFunction(PYGameModule, "CalculateCivCounter", argsList.makeFunctionArgs(), &lResult);

	return lResult;
}

int CvPlayer::getManaChanneledIntoRitual()
{
	ProjectTypes eRitual=(ProjectTypes)getCurrentMagicRitual();
	if(eRitual==NO_PROJECT)
	{
		return 0;
	}

	int iManaAdded = 0;
	
	if(getCurrentMagicRitualManaNeeded()>getMana())
	{
		iManaAdded = std::min(getMana(),getManaIncome()/2);
		if(getMana() + getManaIncome() >= getMaxMana())
		{
			iManaAdded = std::min(getMana(),getManaIncome()+getMana()-getMaxMana());
		}
	}
	else
	{
		iManaAdded = getCurrentMagicRitualManaNeeded();
	}

	return iManaAdded;
}

int CvPlayer::getCrime() const
{
	return m_iCrime;
}

void CvPlayer::changeCrime(int iChange)
{
	m_iCrime += iChange;
}

int CvPlayer::getCaptureCityGoldPercentChange() const
{
	return m_iCaptureCityGoldPercentChange;
}

void CvPlayer::changeCaptureCityGoldPercentChange(int iChange)
{
	m_iCaptureCityGoldPercentChange += iChange;
}


void CvPlayer::AdvanceMagicRitual()
{
	ProjectTypes eRitual=(ProjectTypes)getCurrentMagicRitual();
	if(eRitual==NO_PROJECT)
	{
		return;
	}

	int iManaAdded=getManaChanneledIntoRitual();
	
	changeMana(-iManaAdded);
	changeCurrentMagicRitualManaNeeded(-iManaAdded);
}

int CvPlayer::getMagicRitualTurnsNeeded(ProjectTypes eProject, bool bStarted)
{
	bool bTerraform=false;
	bool bEnchantment=false;
	bool bSummon=false;

	CvProjectInfo &kRitual=GC.getProjectInfo(eProject);

	if(kRitual.isGlobalEnchantment())
		bEnchantment=true;

	if(kRitual.getNumTerrainChanges()>0)
		bTerraform=true;

	if(kRitual.getSummonUnitType()!=NO_UNIT)
		bSummon=true;

	if((bSummon && !canCreateSummon(eProject))
		|| (bTerraform && !canDoTerraformRitual(eProject))
		|| (bEnchantment && !canDoGlobalEnchantment(eProject)))
	{
		return -1;
	}

	if(getManaIncome()==0)
	{
		return -1;
	}

	int iManaNeeded = (bStarted) ? getCurrentMagicRitualManaNeeded() : getSpecificMagicRitualCost(eProject);
	iManaNeeded = iManaNeeded - getMana();
	if(iManaNeeded > 0)
	{
		return iManaNeeded / getManaIncome();
	}

	return 0;
}

void CvPlayer::updateBanditLordsCivic(int iChange)
{
	if(iChange>0)
	{
		return;
	}

	UnitClassTypes eBandit=(UnitClassTypes)GC.getInfoTypeForString("UNITCLASS_BANDIT_LORD");
	CvUnit* pLoopUnit;
	int iLoop;
	if(eBandit!=NO_UNITCLASS)
	{
		for(pLoopUnit = firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = nextUnit(&iLoop))
		{
			if(pLoopUnit->getUnitClassType()==eBandit)
			{
				pLoopUnit->kill(false);
			}
		}
	}
}

CvCity* CvPlayer::getSummonCity()
{
	BuildingTypes eSummonCircle = (BuildingTypes)GC.getInfoTypeForString("BUILDING_SUMMON_CIRCLE");
	if(eSummonCircle==NO_BUILDING || getBuildingClassCount((BuildingClassTypes)GC.getBuildingInfo(eSummonCircle).getBuildingClassType())==0)
	{
		return getCapitalCity();
	}

	int iLoop;
	CvCity* pLoopCity;
	for(pLoopCity = firstCity(&iLoop);pLoopCity!=NULL;pLoopCity = nextCity(&iLoop))
	{
		if(pLoopCity->getNumRealBuilding(eSummonCircle)>0)
		{
			return pLoopCity;
		}
	}

	return getCapitalCity();
}

bool CvPlayer::canBuyHolyKnowledge(TechTypes eTech) const
{
	if(eTech == NO_TECH)
		return false;

	if(!GC.getTechInfo(eTech).isHolyKnowledge())
		return false;

	if(GET_TEAM(getTeam()).isHasTech(eTech))
		return false;

	if(GC.getTechInfo(eTech).getPrereqOrTechs(0) != NO_TECH) {
		if(!GET_TEAM(getTeam()).isHasTech((TechTypes)GC.getTechInfo(eTech).getPrereqOrTechs(0))) {
			return false;
		}
	}

	if(GC.getTechInfo(eTech).getPrereqAndTechs(0) != NO_TECH) {
		if(!GET_TEAM(getTeam()).isHasTech((TechTypes)GC.getTechInfo(eTech).getPrereqAndTechs(0))) {
			return false;
		}
	}

	if(GC.getTechInfo((TechTypes)eTech).getResearchCost() <= 0 || 
		(getFaith() < GET_TEAM(getTeam()).getResearchCost(eTech))) {
		return false;
	}

	if(GC.getTechInfo((TechTypes)eTech).getPrereqReligion() != NO_RELIGION) {
		if(getStateReligion() != GC.getTechInfo((TechTypes)eTech).getPrereqReligion()) {
			return false;
		}
	}

	return true;
}

void CvPlayer::BuyHolyKnowledge(TechTypes eTech)
{
	if(!canBuyHolyKnowledge(eTech))
	{
		return;
	}
	GET_TEAM(getTeam()).setHasTech(eTech, true, getID(), true, false);
	changeFaith(-GET_TEAM(getTeam()).getResearchCost(eTech));
}

bool CvPlayer::canSupportMoreUnits(int iExtra) const
{
	if(isBarbarian())
		return true;
	return (getUnitSupportUsed()+iExtra<getUnitSupportLimitTotal());
}

bool CvPlayer::canIncreaseCityInfrastructureLevel(CvCity* pCity) const
{
	if(pCity==NULL)
	{
		return false;
	}

	if(getGlobalCulture()<calculateCultureNeededForCityInfrastructure())
	{
		return false;
	}

	return true;
}

int CvPlayer::calculateCultureNeededForCityInfrastructure() const
{
	int iIncrease=500*(getTotalCityInfastructureLevel()+1);
	iIncrease*=getTotalCityInfastructureLevel()+10;
	iIncrease/=10;
	return (iIncrease);
}

int CvPlayer::doCivicPreservation(bool bCountOnly) const
{
	TerrainTypes eDesert=(TerrainTypes)GC.getInfoTypeForString("TERRAIN_DESERT");
	TerrainTypes eSnow=(TerrainTypes)GC.getInfoTypeForString("TERRAIN_SNOW");
	TerrainTypes ePlains=(TerrainTypes)GC.getInfoTypeForString("TERRAIN_PLAINS");
	TerrainTypes eTundra=(TerrainTypes)GC.getInfoTypeForString("TERRAIN_TUNDRA");
	FeatureTypes eFlood=(FeatureTypes)GC.getInfoTypeForString("FEATURE_FLOOD_PLAINS");

	int iMapPlots=GC.getMapINLINE().numPlots();
	std::vector<int> PossiblePlots;

	for (int iI=0;iI<iMapPlots;iI++)
	{
		CvPlot* pPlot = GC.getMapINLINE().plotByIndex(iI);
		if(pPlot!=NULL)
		{
			if(pPlot->getOwnerINLINE()==getID())
			{
				if((pPlot->getTerrainType()==eDesert && pPlot->getFeatureType()!=eFlood) || pPlot->getTerrainType()==eSnow)
				{
					if(!(pPlot->isWater() || pPlot->isPeak() || GC.getCivilizationInfo((CivilizationTypes)getCivilizationType()).isNativeTerrains(pPlot->getTerrainType())))
					{
						PossiblePlots.push_back(iI);
					}
				}
			}
		}
	}

	if(bCountOnly || PossiblePlots.size()==0)
	{
		return PossiblePlots.size();
	}
						
	int iRnd = GC.getGameINLINE().getSorenRandNum(PossiblePlots.size(), "Pick Preservation Plot");
	CvPlot* pTerraformPlot=GC.getMapINLINE().plotByIndex(PossiblePlots.at(iRnd));
	if(pTerraformPlot->getTerrainType()==eDesert)
	{
		pTerraformPlot->setTerrainType(ePlains,true,true);
		pTerraformPlot->setFeatureType(NO_FEATURE);
	}
	if(pTerraformPlot->getTerrainType()==eSnow)
	{
		pTerraformPlot->setTerrainType(eTundra,true,true);
	}

	return PossiblePlots.size();
}

void CvPlayer::VerifyGlobalEnchantments()
{
	int iLoop;

	for(int iI=0;iI<GC.getNumProjectInfos();iI++)
	{
		CvProjectInfo& kProject=GC.getProjectInfo((ProjectTypes)iI);
		if(kProject.isGlobalEnchantment())
		{
			//verify if another Enchantment must be cancelled
			if(GC.getProjectInfo((ProjectTypes)iI).getProjectPrereq()!=NO_PROJECT)
			{
				if(GET_TEAM(getTeam()).getProjectCount((ProjectTypes)iI)>0)
				{
					if(GET_TEAM(getTeam()).getProjectCount((ProjectTypes)GC.getProjectInfo((ProjectTypes)iI).getProjectPrereq())<=0)
					{
						RemoveGlobalEnchantment((ProjectTypes)iI);
					}
				}
			}

			//verify if religion prereq is met
			// LS: all players of a team will be checked now
			if(GC.getProjectInfo((ProjectTypes)iI).getPrereqReligion()!=NO_RELIGION)
			{
				bool bTeamValid = false;
				for(int iK = 0; iK < MAX_CIV_PLAYERS; iK++)
				{
					CvPlayer &kPlayer = GET_PLAYER((PlayerTypes)iK);
					if(kPlayer.getTeam() == getTeam())
					{
						if(GC.getProjectInfo((ProjectTypes)iI).getPrereqReligion() == kPlayer.getStateReligion())
						{
							bTeamValid = true;
							break;
						}
					}
				}
				if(GET_TEAM(getTeam()).getProjectCount((ProjectTypes)iI)>0 && !bTeamValid)
				{
					RemoveGlobalEnchantment((ProjectTypes)iI);
				}
			}

			//temporary Solution until Globalenchantments count for every Player
			if(kProject.getBuildingType()!=NO_BUILDING)
			{
				bool bValid=false;
				for(int iK = 0; iK < MAX_CIV_PLAYERS; iK++)
				{
					CvPlayer &kPlayer =GET_PLAYER((PlayerTypes)iK);

					if(kPlayer.isAlive() && GET_TEAM(kPlayer.getTeam()).getProjectCount((ProjectTypes)iI)>0)
					{
						if((kProject.isHostile() && kPlayer.getTeam()!=getTeam() && GET_TEAM(kPlayer.getTeam()).isAtWar(getTeam()))
							||(!kProject.isHostile() && kPlayer.getTeam()==getTeam()))
						{
							bValid=true;
						}
					}
				}
				if(!bValid)
				{
					for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop))
					{
						pLoopCity->setNumRealBuilding((BuildingTypes)kProject.getBuildingType(),0);
					}
				}
			}
		}
	}
}

int CvPlayer::getTraitCost(TraitTypes eTrait)
{
	int iCost = 5000;
	int iNumTraits = getTotalTraitsFromCulture();
	TraitTypes eMinorTrait = (TraitTypes)GC.getInfoTypeForString("TRAIT_MINOR");
	bool bMinor = false;

	if(eMinorTrait != NO_TRAIT && hasTrait(eMinorTrait)) {
		bMinor = true;
		iCost /= 2;
	}
	
	for(int i = 0; i < iNumTraits; ++i) {
		iCost = iCost * ((bMinor) ? 3 : 4);		
	}

	if(isHuman()) {
		iCost *= GC.getHandicapInfo(GC.getGame().getHandicapType()).getResearchPercent();
		iCost /= 100;
	}

	return iCost;
}

AlignmentTypes CvPlayer::getAlignmentAfterConversion(ReligionTypes eReligion) const {
	AlignmentTypes eAlignment = (AlignmentTypes)getAlignment();

	if (getAlignment() < GC.getReligionInfo(eReligion).getAlignmentBest())
	{
		eAlignment = (AlignmentTypes)GC.getReligionInfo(eReligion).getAlignmentBest();
	}
	if (getAlignment() > GC.getReligionInfo(eReligion).getAlignmentWorst())
	{
		eAlignment = (AlignmentTypes)GC.getReligionInfo(eReligion).getAlignmentWorst();
	}

	return eAlignment;
}

int CvPlayer::addFoodToCities(int iFood)
{
	int iLoop, iChange;
	int iFoodTaken = 0;

	for (int i = 0; i < iFood / 10; ++i) {
		for (CvCity* pLoopCity = firstCity(&iLoop); pLoopCity != NULL; pLoopCity = nextCity(&iLoop)) {
			if((pLoopCity->getFood() < pLoopCity->growthThreshold()-10)
				|| pLoopCity->happyLevel() > pLoopCity->unhappyLevel(0)) {
					iChange = std::min(10, iFood);
					pLoopCity->changeFood(iChange);
					iFood -= iChange;
					iFoodTaken += iChange;
			}
		}
		if(iFood <= 0)
			break;
	}

	return iFoodTaken;
}

void CvPlayer::displayText(CvWString szText, int xCoord, int yCoord, LPCTSTR pszSound, LPCSTR pszIcon)
{
	int x,y;
	x = xCoord;
	y = yCoord;
	if(xCoord == -1 || yCoord == -1) {
		if(getCapitalCity() != NULL) {
			x = getCapitalCity()->getX_INLINE();
			y = getCapitalCity()->getY_INLINE();
		}
	}
	gDLL->getInterfaceIFace()->addMessage(getID(), false, GC.getEVENT_MESSAGE_TIME(), szText, pszSound, MESSAGE_TYPE_MINOR_EVENT, pszIcon, NO_COLOR, x, y);	
}