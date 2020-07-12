/*
 *------------------------------------------------------------------------
 *      Object: psxtxv.h
 *         $Id: psxtxv.h,v 1.8 2009/12/07 20:56:00 molle Exp $
 *        Date: Oct 2005 
 *       Notes: Textvariablen
 *------------------------------------------------------------------------
 *
 * Copyright (c) 1997-2005 Michael Molkenthin. All Rights Reserved.
 *
 * Proprietary & Confidential
 *
 * The entire sourcecode is copyrighted under the German copyright laws. 
 * The owner of the copyright is Michael Molkenthin
 * 
 * Alle einfachen oder ausschliesslichen Nutzungs- und Verwertungsrechte 
 * verbleiben allein bei Michael Molkenthin, Lerchenstr. 98, 22767 Hamburg
 *
 * Eine Modifikation oder Bearbeitung des Quellcodes durch Dritte begruendet 
 * ausdruecklich keinerlei Miturheberschaft.
 *
 * Redistribution of the sourcecode without prior permission of Michael Molkenthin
 * are strictly prohibited.

 * This source code and the algorithms implemented therein constitute
 * confidential information and may comprise trade secrets of M. Molkenthin
 * or its associates, and any use thereof is subject to the terms and
 * conditions of the Non-Disclosure Agreement pursuant to which this
 * source code was originally received.
 * 
 * Modified copies of the source code must retain the above copyright
 * notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef TEXTVAR_H
#define TEXTVAR_H

// -- sprachabhaengige Textkonstanten
//
typedef enum _langdef {
   txxNull = 0,
   txxPoliceFlyer,
   txxPoliceFlyerWithShield,
   txxPoliceLeader,
   txxBountyHunter,
   txxLangSwitched,
   txxKeyHelpScreen1,
   txxKeyHelpScreen2,
   txxKeyHelpScreen3,
   txxKeyHelpScreen4,
   txxKeyHelpScreen5,
   txxKeyHelpScreen6,
   txxKeyHelpScreen7,
   txxKeyHelpScreen8,
   txxKeyHelpScreen9,
   txxKeyHelpScreen10,
   txxKeyHelpScreen11,
   txxKeyHelpScreen12,
   txxKeyHelpScreen13,
   txxKeyHelpScreen14,
   txxKeyHelpScreen15,
   txxKeyHelpScreen16,
   txxKeyHelpScreen17,
   txxKeyHelpScreen18,
   txxKeyHelpScreen19,
   txxKeyHelpScreen20,
   txxKeyHelpScreen21,
   txxKeyHelpScreen22,
   txxKeyHelpScreen23,
   //txxKeyHelpScreen24,
   txxPressF1ForKeymapESCForMenu,
   txxPressESCToContinue,
   txxMissionGoal,
   txxAbsoluteCompleted,
   txxWitchSpaceCountdown,
   txxGameOver,
   txxMenuItemStartGame  ,
   txxMenuItemOptionMenu,
   txxMenuItemExteriorView,
   txxMenuItemCargoBox,
   txxTurboActivated,
   txxProducer,
   txxGameDesign,
   txxCoding,
   txxGrafix,
   txxSound,
   txxDistance,
   txxEconomy,
   txxGoverment,
   txxTechnicalLevel,
   txxPopulation,
   txxTurnover,
   txxAverRad,
   txxBillions,
   txxLaserTemp,
   txxLightYearsUnit,
   txxAnarchy,
   txxFeudal,
   txxMulti_gov,
   txxDictatorship,
   txxCommunist,
   txxConfederacy,
   txxDemocracy,
   txxCorporateState,
   txxRichInd,
   txxAverageInd,
   txxPoorInd,
   txxMainlyInd,
   txxMainlyAgri,
   txxRichAgri,
   txxAverageAgri,
   txxPoorAgri,
   txxFuel,
   txxDeflector,
   txxMenuItemStatus,
   txxDocked,
   txxGreen,
   txxYellow,
   txxRed,
   txxClean,
   txxOffender,
   txxFugitive,
   txxHarmless,
   txxMostlyHarmless,
   txxPoor,
   txxAverage,
   txxAboveAverage,
   txxCompetent,
   txxDangerous,
   txxDeadly,
   txxElite,
   txxPresentSystem,
   txxHyperspaceSystem,
   txxCondition,
   txxChash,
   txxLegalStatus,
   txxRating ,
   txxEquipment,
   txxMissile,
   txxLargeCargoBay,
   txxShieldGenerator,
   txxPulseLaser,
   txxBattleLaser,
   txxGatlingLaserI,
   txxGatlingLaserII,
   txxSuperLaser,
   txxFuelScoops,
   txxEscapeCapsule,
   txxEnergyBomb,
   txxTorpedo,
   txxExtraEnergyUnit,
   txxDockingComputers,
   txxGalacticHyperdrive,
   txxMiningLaser,
   txxMilitaryLaser,
   txxFood,
   txxTextiles,
   txxRadioactives,
   txxRobotSlaves,
   txxBeverages,
   txxCyborgs,
   txxLiquorWines,
   txxLuxuries,
   txxRareSpecies,
   txxNarcotics,
   txxComputers,
   txxMachinery,
   txxAlloys,
   txxFirearms,
   txxFurs,
   txxMinerals,
   txxGold,
   txxPlatinum,
   txxGemStones,
   txxAlienItems,
   txxCommodity,
   txxOffer,
   txxPrice,
   txxShipsHold,
   txxSell,
   txxBuy,
   txxBankAccount,
   txxMenuItemCockpit  ,
   txxMenuItemGalMap,
   txxMenuItemHelp,
   txxLeaveDock,
   txxArtifacts,
   txxOre,
   txxSpareParts,
   txxPepperGameShares,
   txxWasChosenAsWitchspaceDestination,
   txxIsToFarForCurrentFuel,
   txxCredits,
   txxWelcomeToPlanetSystem,
   txxAlreadyInWitchSpaceDestinationSystem,
   txxMenuItemTrade,
   txxShipIsNotDocked,
   txxMaximumCapacityReached,
   txxCautionAsteroids,
   txxNoAsteroids,
   txxTrader,
   txxHunter,
   txxPirate,
   txxWingman,
   txxCombatExperience,
   txxWorldsTechLevelTooLow,
   txxOutOfStock,
   txxTooExpensive,
   txxNotOffered,
   txxSpeed,
   txxNextPage,
   txxHunterHyperSpace,
   txxDrop,
   txxEjectCargoBox,
   txxGun,
   txxAsteroid,
   txxStarBase,
   txxEnemy,
   txxNeutral,
   txxSchild   ,
   txxChooseSaveGameFile,
   txxGameSuccessfullySaved,
   txxGameSuccessfullyLoaded,
   txxReady ,
   txxErrorOccurredPleaseSave,
   txxGoodMorningCommander,
   txxSurrender,
   txxRadiationTooHigh,
   txxNoTargetSet,
   txxMine,
   txxTooNearToStarBase,
   txxWitchSpaceJumpAborted,
   txxCrimeTicketPaid,
   txxCannotPayCrimeTicket,
   txxPoliceScanMustBePassed,
   txxGalaxy,
   txxAddSubtractToChange,
   txxIllegal,
   txxMenuItemLoad,
   txxMenuItemSave,
   txxCantCreateFile,
   txxCantOpenFile,
   txxNoValidFileOrUnknownVersion,
   txx1234UnknownChunk,
   txxReadErrorOccured,
   txxOreLoadedInCargoBay ,
   txxTotal,
   txxThargoid,
   txxThargoiddrone,
   txxWitchSpaceDriveMalfunction,
   txxRewardEarned,
   txxLanguage,
   txxMenuItemSeenPirates,
   txxLastSeenAt,
   txxXJumpsAgo,
   txxReward,
   txxTradersReportSeenPirates,
   txxNeededToMountThis,
   txxOtherGoodsOrEquipment,
   txxAreDependentToThis,
   TXX_LASTENUM
} ENUM_TXX_ID;

#endif
