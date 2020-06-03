// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "consensus/merkle.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>
#include <limits>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock = 0;
    genesis.nVersion = nVersion;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of the genesis coinbase cannot
 * be spent as it did not originally exist in the database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Start of EPG to provide cost-effective and simple solutions to execute various trade finance instruments.";
    const CScript genesisOutputScript = CScript() << ParseHex("04a930869d88701cbcbf54a5785ee8b5352f72af9c30c7d016128299bcb5177607b620b0c1cc76c486d9b5f711391be964dc6db2da1f7539aaa6c3ee65607e8aa5") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x00000ec95644c720b0d42b9c2206fb75f6e9c14418e5b6b66f3365d189238efb"))
    (1, uint256("0x000004b9c6ea3f605e193587a905bd495d41ee6ac33ecc91a83ecaa724ce3a26"))
    (15100, uint256("0x889081c8803082944ec707bfdbad1b5819d4b8dc918e44e25e406b87714bd41a"))
    (15589, uint256("0x7385f76e0dd7d33a2b94aa9abfca67bb028865196be378bfe72beb1f6331d0cb"))
    (20104, uint256("0xa0ee7c4e71b75bd086c5e7ffdde679bda468ee56c38d41fde1c118c6879f3631"))
    (39000, uint256("0xf36469956d75be27537d3a509c3bdc4a883a605f612a956232c6813402bdfb21"))
    ; 
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1580855918, // * UNIX timestamp of last checkpoint block
    45026,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    100        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of
    (0, uint256("0x001"))
;
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1576364812,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1576364812,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.powLimit   = ~uint256(0) >> 20; // EPGC starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~uint256(0) >> 24;
        consensus.posLimitV2 = ~uint256(0) >> 20; // 60/4 = 15 ==> use 2**4 higher limit
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMaxMoneyOut = 40000000 * COIN;
        consensus.nStakeMinAge = 60 * 60;               // 1 hour
        consensus.nStakeMinDepth = 200;
        consensus.nTargetTimespan = 6 * 60;				// 6  minutes
        consensus.nTargetTimespanV2 = 30 * 60;			// 30 minutes
        consensus.nTargetSpacing = 2 * 60;				// 1  minutes
        consensus.nTimeSlotLength = 15;					// 15 seconds

        // height based activations
        consensus.height_last_PoW = 1000;
        consensus.height_start_BIP65 = 0; // 82629b7a9978f5c7ea3f70a12db92633a7d2e436711500db28b97efd48b1e527
        consensus.height_start_StakeModifierV2 = 15000;
        consensus.height_start_TimeProtoV2 = 60000;

         /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x91;
        pchMessageStart[1] = 0xd4;
        pchMessageStart[2] = 0xfa;
        pchMessageStart[3] = 0xe9;
        vAlertPubKey = ParseHex("048b322f15140bb99df0de27653b3761190b7f162c875893c1da3138a5b0d5189bbf4f8ecfe7fe7c5374de522006f295da412059138d73e7ec2e526a51530e1292");
        nDefaultPort = 29442;
        nMaxReorganizationDepth = 100;

        genesis = CreateGenesisBlock(1576364812, 2470982, 0x1e0ffff0, 1, 250 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256("0x00000ec95644c720b0d42b9c2206fb75f6e9c14418e5b6b66f3365d189238efb"));
        assert(genesis.hashMerkleRoot == uint256("0xdfcf7c1de1f1c9a4bf68518daed009bd72735810e239aa8aa5bf937739c4fe71"));

        nEnforceBlockUpgradeMajority = 8100; // 75%
        nRejectBlockOutdatedMajority = 10260; // 95%
        nToCheckBlockUpgradeMajority = 10800; // Approximate expected amount of blocks in 7 days (1440*7.5)
        nMinerThreads = 0;
        nMasternodeCountDrift = 20;
        nMinColdStakingAmount = 1 * COIN;

        nMasternodeCollateral = 200;
        strDevFundAddress = "7Ns4orZTzEqrVPSPE4JhcGHhoEsqSnrYg7";
        nStakeInputMinimal = 75 * COIN;

        /** Height or Time Based Activations **/
        nEpgcBadBlockTime = 1471401614; // Skip nBit validation of Block 259201 per PR #915
        nEpgcBadBlocknBits = 0x1c056dac; // Skip nBit validation of Block 259201 per PR #915
        nModifierUpdateBlock = 0;
        nZerocoinStartHeight = 999999999;
        nZerocoinStartTime = 1873242920; // October 17, 2017 4:30:00 AM
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 999999999; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 999999999; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 999999999; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 999999999; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 999999999; //!> The block that zerocoin v2 becomes active - roughly Tuesday, May 8, 2018 4:00:00 AM GMT
        nBlockDoubleAccumulated = 999999999;
        nEnforceNewSporkKey = 1576364812; //!> Sporks signed after Monday, August 26, 2019 11:00:00 PM GMT must use the new spork key
        nRejectOldSporkKey = 1576364812; //!> Fully reject old spork key after Thursday, September 26, 2019 11:00:00 PM GMT

        // Public coin spend enforcement
        nPublicZCSpends = 0;

        // New P2P messages signatures
        nBlockEnforceNewMessageSignatures = consensus.height_start_TimeProtoV2;

        // Blocks v7
        nBlockLastAccumulatorCheckpoint = 0;
        nBlockV7StartHeight = consensus.height_start_TimeProtoV2;

        // Fake Serial Attack
        nFakeSerialBlockheightEnd = -1;
        nSupplyBeforeFakeSerial = 0;

        // Note that of those with the service bits flag, most only support a subset of possible options

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("173.199.119.55", "173.199.119.55"));     // Primary DNS Seeder 
        vSeeds.push_back(CDNSSeedData("149.28.34.121", "149.28.34.121")); 
        vSeeds.push_back(CDNSSeedData("45.76.63.148", "45.76.63.148"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 15);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 13);
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 33);     // starting with 'E'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 197);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        nBudgetCycleBlocks = 43200; //!< Amount of blocks in a months period of time (using 1 minutes per) = (60*24*30)
        strSporkPubKey = "04410cf61ab8bdf2f3bb39c361312bb60f549135232b00085736fa2905f73e60216c32edce58a1916c2af49a5665d69a875217a39d30bd20855e84eaf587e92e17";
        strSporkPubKeyOld = "04410cf61ab8bdf2f3bb39c361312bb60f549135232b00085736fa2905f73e60216c32edce58a1916c2af49a5665d69a875217a39d30bd20855e84eaf587e92e17";
        strObfuscationPoolDummyAddress = "7GSKHnPBTu6V3S89qwvA3rb95BMYgKCi9X";
        nStartMasternodePayments = 1576364812; //genises

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMaxZerocoinPublicSpendsPerTransaction = 637; // Assume about 220 bytes each input
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 7; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zepg to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
        nProposalEstablishmentTime = 60 * 60 * 24; // Proposals must be at least a day old to make it into a budget
    }

    int GetRequiredMasternodeCollateral(int nTargetHeight) const
    {
        if(nTargetHeight < 10001  ) {
            return 550;
        } else if (nTargetHeight < 45501){
            return 550;
        }
        else
        {
           return 2000;
        }
    }
    
    CAmount StakingMinInput(int nTargetHeight) const
    {
        if(nTargetHeight < 10000) {
            return 0 * COIN;
        }
        else if (nTargetHeight < 39000)
        {
            return 10 * COIN;
        }
        else
        {
            return 1 * COIN;
        }
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }

};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~uint256(0) >> 20; // EPGC starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~uint256(0) >> 24;
        consensus.posLimitV2 = ~uint256(0) >> 20; // 60/4 = 15 ==> use 2**4 higher limit
        consensus.nCoinbaseMaturity = 15;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 6 * 60;				// 6  minutes
        consensus.nTargetTimespanV2 = 30 * 60;			// 30 minutes
        consensus.nTargetSpacing = 2 * 60;				// 1  minutes
        consensus.nTimeSlotLength = 15;					// 15 seconds

        // height based activations
        consensus.height_last_PoW = 200;
        consensus.height_start_BIP65 = 851019;
        consensus.height_start_StakeModifierV2 = 1214000;
        consensus.height_start_TimeProtoV2 = 100; // TimeProtocolV2, Blocks V7 and newMessageSignatures

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0x41;
        pchMessageStart[1] = 0x72;
        pchMessageStart[2] = 0x61;
        pchMessageStart[3] = 0xbb;
        vAlertPubKey = ParseHex("04a2ccfd4ef7e1b4533a25456819eaf66023590e5c7032606e45bae29f86e3868a64a21b2c832790c21265e1297cfdbdb8692c9f8dc0ddab0f6e22a668eaede2ad");
        nDefaultPort = 51474;

        genesis = CreateGenesisBlock(1576364811, 3611038, 0x1e0ffff0, 1, 250 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256("0x000008a90b61e6ef817f4b071a10a6c3afbb2582cbf7d882596a0e19d05b0829"));
        assert(genesis.hashMerkleRoot == uint256("0xdfcf7c1de1f1c9a4bf68518daed009bd72735810e239aa8aa5bf937739c4fe71"));

        nEnforceBlockUpgradeMajority = 4320; // 75%
        nRejectBlockOutdatedMajority = 5472; // 95%
        nToCheckBlockUpgradeMajority = 5760; // 4 days
        nMinerThreads = 0;
        nEpgcBadBlockTime = 1489001494; // Skip nBit validation of Block 259201 per PR #915
        nEpgcBadBlocknBits = 0x1e0a20bd; // Skip nBit validation of Block 201 per PR #915
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nZerocoinStartHeight = 201576;
        nZerocoinStartTime = 1501776000;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 999999999; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 999999999; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 999999999; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 999999999; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 444020; //!> The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1576364812; //!> Sporks signed after Monday, August 26, 2019 11:00:00 PM GMT must use the new spork key
        nRejectOldSporkKey = 1576364812; //!> Reject old spork key after Thursday, September 26, 2019 11:00:00 PM GMT

        // Public coin spend enforcement
        nPublicZCSpends = 100;

        // New P2P messages signatures
        nBlockEnforceNewMessageSignatures = consensus.height_start_TimeProtoV2;

        // Blocks v7
        nBlockLastAccumulatorCheckpoint = nPublicZCSpends - 10;
        nBlockV7StartHeight = consensus.height_start_TimeProtoV2;

        // Fake Serial Attack
        nFakeSerialBlockheightEnd = -1;
        nSupplyBeforeFakeSerial = 0;

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 38); // Testnet epgc addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet epgc script addresses start with '8' or '9'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 73);     // starting with 'W'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet epgc BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet epgc BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet epgc BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        nBudgetCycleBlocks = 144; //!< Ten cycles per day on testnet
        strSporkPubKey = "04E88BB455E2A04E65FCC41D88CD367E9CCE1F5A409BE94D8C2B4B35D223DED9C8E2F4E061349BA3A38839282508066B6DC4DB72DD432AC4067991E6BF20176127";
        strSporkPubKeyOld = "04410cf61ab8bdf2f3bb39c361312bb60f549135232b00085736fa2905f73e60216c32edce58a1916c2af49a5665d69a875217a39";
        strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet

        nProposalEstablishmentTime = 60 * 5; // Proposals must be at least 5 mns old to make it into a test budget
    }

    int GetRequiredMasternodeCollateral(int nTargetHeight) const
    {
        if(nTargetHeight > 10000) {
            return 450;
        }

        return 250;
    }
    
    CAmount StakingMinInput(int nTargetHeight) const
    {
        if(nTargetHeight > 2000) {
            return 30 * COIN;
        }
        
        return 0;
    }
    
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */

class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
 
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~uint256(0) >> 20; // EPGC starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~uint256(0) >> 24;
        consensus.posLimitV2 = ~uint256(0) >> 20; // 60/4 = 15 ==> use 2**4 higher limit
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nStakeMinAge = 0;
        consensus.nStakeMinDepth = 0;
        consensus.nTargetTimespan = 6 * 60;				// 6  minutes
        consensus.nTargetTimespanV2 = 30 * 60;			// 30 minutes
        consensus.nTargetSpacing = 2 * 60;				// 1  minutes
        consensus.nTimeSlotLength = 15;					// 15 seconds

        // height based activations
        consensus.height_last_PoW = 250;
        consensus.height_start_BIP65 = 851019; // Not defined for regtest. Inherit TestNet value.
        consensus.height_start_StakeModifierV2 = consensus.height_last_PoW + 1; // start with modifier V2 on regtest
        consensus.height_start_TimeProtoV2 = 999999999;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nDefaultPort = 51476;

        genesis = CreateGenesisBlock(1454124731, 2635406, 0x1e0ffff0, 1, 250 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256("0x00000a45c57e2886e13a8b862631810067014b5400d2686a96b7f5486723be2b"));
        assert(genesis.hashMerkleRoot == uint256("0xdfcf7c1de1f1c9a4bf68518daed009bd72735810e239aa8aa5bf937739c4fe71"));

        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 0;       //approx Mon, 17 Apr 2017 04:00:00 GMT
        nZerocoinStartHeight = 300;
        nBlockZerocoinV2 = 300;
        nZerocoinStartTime = 1501776000;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 999999999; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 999999999; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 999999999; //Last valid accumulator checkpoint

        // Public coin spend enforcement
        nPublicZCSpends = 350;

        // Blocks v7
        nBlockV7StartHeight = nBlockZerocoinV2;
        nBlockLastAccumulatorCheckpoint = nBlockZerocoinV2+1; // no accumul. checkpoints check on regtest

        // New P2P messages signatures
        nBlockEnforceNewMessageSignatures = 1;

        // Fake Serial Attack
        nFakeSerialBlockheightEnd = -1;

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        strSporkPubKey = "043969b1b0e6f327de37f297a015d37e2235eaaeeb3933deecd8162c075cee0207b13537618bde640879606001a8136091c62ec272dd0133424a178704e6e75bb7";
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
