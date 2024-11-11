// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2019 The PIVX developers
// Copyright (c) 2022-2023 The SUDA developers
// Copyright (c) 2023 The CeilingCatCoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "chainparamsseeds.h"
#include "consensus/merkle.h"
#include "util.h"
#include "utilstrencodings.h"

#include <boost/assign/list_of.hpp>

#include <assert.h>

void SolveAlternateGenesis(CBlock& genesis, uint32_t nTime, uint256& powLimit)
{
    genesis.nTime = nTime;
    genesis.nNonce = 0;

    while (genesis.GetPoWHash() > powLimit) {
        printf("\r%08x", genesis.nNonce);
        ++genesis.nNonce;
    }

    printf("\n%s\n", genesis.ToString().c_str());
}

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
    genesis.hashPrevBlock.SetNull();
    genesis.nVersion = nVersion;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, const char* pszTimestamp)
{
    uint32_t nBits = 0x1e0ffff0;
    int32_t nVersion = 1;
    const CAmount& genesisReward = 0.00000050 * COIN;
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of(0, uint256S("17121853915c8e6036c3b813cae096312b8ffaff3b33b51a770e22c357c6b895"))
    (1500, uint256S("8bdbc733a41c288bc63f57d68cfc56df9cf4e2d519dbfa1062de7bc749bd7154"))
    (8500, uint256S("2c09cd4ca28d8eaeadaf79d54333d1b4882caf9a7f1512ed596806e19e9caaba"))
    (32200, uint256S("b2540e8f17d87207fb917f9d5220fa064e124a66051bb6d941473720df7517f4"))
    (51000, uint256S("2355533f52835b4967dd4960b0acda622726da5e24f9d33cf243758765a2ee5b"))
    (98000, uint256S("129a066f0f5b06da64f94a20d840455c50906f9d2d87338c1df50f969a9d3ec8"))
    (197770, uint256S("3283fed2ce29fda12135b2c47248372f72a40cf547e1c5671b1ce7477d366fd6"))
    (197773, uint256S("668c867c34b2b2f20bd7537874c2cd6531a88e1df7076a2fb5cfc6ad5481be7d"))
    (197775, uint256S("65fb6469df2b9e74384efd1222a22532ed4655af86943a2e6bfc5b49b5b86ec6"))
    (197779, uint256S("5cda91155f948d8a054ff2e792e7ad1ba1ecb8300de20baef95cc88970128604"))
    (197780, uint256S("77f6204053a8e301ff5b10e4979adc1db012f562f6d45f164192e776ee2ed82d"))
    (197781, uint256S("f0275877010fcf7ca9d756756ccae634a28682685bf488cc12312683ca925524")) //split of chain
    ;

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1611351525, // * UNIX timestamp of last checkpoint block
    0,          // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256S("0x001"))
 ;
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1610066000,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256S("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1610068000,
    0,
    100};


class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.powLimit   = ~UINT256_ZERO >> 18;   // ccc starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 43200;       // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 90;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 5000000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.devAddress = "CCCdisabledXXXXXXXXXXXXXXXXXXXXXX";

        consensus.nKAWPOWActivation = 1684857487;

        // spork key
        consensus.strSporkPubKey = "04638c049e470eee6fc99c6398018b183c21c4058e067be44ce88ae1f5d20519e11070a23101625050e4a159f1cff2c6d18218a0076b191e14b9c27df1c199cc6f";

        // height-based activations
        consensus.height_last_PoW 			   = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT_MAX;
        consensus.height_start_BIP65 			   = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT_MAX;
        consensus.height_start_MessSignaturesV2            = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT_MAX;  // height_start_TimeProtoV2
        consensus.height_start_StakeModifierNewSelection   = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT_MAX;
        consensus.height_start_StakeModifierV2             = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT_MAX;
        consensus.height_start_TimeProtoV2                 = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT_MAX;

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_PURPLE_FENIX].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xf8;
        pchMessageStart[1] = 0x34;
        pchMessageStart[2] = 0x1e;
        pchMessageStart[3] = 0x2c;
        nDefaultPort = 7979;

        genesis = CreateGenesisBlock(1680947030, 3703410, "Pilot applauded for his courage after finding a cobra in the cockpit mid-flight");
        SolveAlternateGenesis(genesis, 1731146578, consensus.powLimit);
        consensus.hashGenesisBlock = genesis.GetHash();
//      assert(genesis.hashMerkleRoot == uint256("3df22896ffc77d8d740ac3b9925ba899e21d165d5ac4c2780a44fdecd9f99967"));
//      assert(consensus.hashGenesisBlock == uint256("17121853915c8e6036c3b813cae096312b8ffaff3b33b51a770e22c357c6b895"));

        vSeeds.push_back(CDNSSeedData("206.71.149.163", "65.38.120.99"));
        vSeeds.push_back(CDNSSeedData("192.153.57.72", "168.100.9.124"));


        // Note that of those with the service bits flag, most only support a subset of possible options

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 28);		// CCC addresses start with 'C'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 30);		// CCC script addresses start with 'D'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 87);	// CCC private keys start with 'c'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
        // CCC BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // CCC BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
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

        genesis = CreateGenesisBlock(1680947030, 3703410, "Pilot applauded for his courage after finding a cobra in the cockpit mid-flight");
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(genesis.hashMerkleRoot == uint256("3df22896ffc77d8d740ac3b9925ba899e21d165d5ac4c2780a44fdecd9f99967"));
        assert(consensus.hashGenesisBlock == uint256("17121853915c8e6036c3b813cae096312b8ffaff3b33b51a770e22c357c6b895"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 16;   // ccc starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 43200;       // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 60;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 250000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.devAddress = "RSGQZGPGzxMNQkNB5KPPbEDuEPaJytMY6q";

        consensus.nKAWPOWActivation = 1683634832;

        // spork key
        consensus.strSporkPubKey = "04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f";

        // height based activations
        consensus.height_last_PoW = 1000;
        consensus.height_start_BIP65 = 851019;                  // Block v5: d1ec8838ba8f644e78dd4f8e861d31e75457dfe607b31deade30e806b5f46c1c
        consensus.height_start_MessSignaturesV2 = 1347000;      // height_start_TimeProtoV2
        consensus.height_start_StakeModifierNewSelection = 51197;
        consensus.height_start_StakeModifierV2 = 1214000;       // Block v6: 1822577176173752aea33d1f60607cefe9e0b1c54ebaa77eb40201a385506199
        consensus.height_start_TimeProtoV2 = 1347000;           // Block v7: 30c173ffc09a13f288bf6e828216107037ce5b79536b1cebd750a014f4939882

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_PURPLE_FENIX].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        
        pchMessageStart[0] = 0xf3;
        pchMessageStart[1] = 0x24;
        pchMessageStart[2] = 0x9a;
        pchMessageStart[3] = 0x5f;
        nDefaultPort = 8452;

        vFixedSeeds.clear();
        vSeeds.clear();
        
        // nodes with support for servicebits filtering should be at the top

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 75); // CCC addresses start with 'X'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 4); // CCC script addresses start with '3'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 80); // CCC private keys start with 'Z'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
        // CCC BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
	base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
	// CCC BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
	base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
	// BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
	base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));
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
/*
//! Modify the REGTEST genesis block so the timestamp is valid for a later start.
*/
        genesis = CreateGenesisBlock(1610066000, 0, "Lorem ipsum dolor sit amet, consectetur adipiscing elit");
        consensus.hashGenesisBlock = genesis.GetHash();
	//assert(consensus.hashGenesisBlock == uint256S("0x001"));
        //assert(genesis.hashMerkleRoot == uint256S("0x001"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 14;   // ccc starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 43200;       // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 30;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 99000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.devAddress = "RSGQZGPGzxMNQkNB5KPPbEDuEPaJytMY6q";

        consensus.nKAWPOWActivation = std::numeric_limits<uint32_t>::max();

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        consensus.strSporkPubKey = "046f781df141e6ab82ca37ddb745803e2d4744137dcfdccd4e8f1e308e2001738e450e35a92828e85b99e18d88f4b78f7fe4eaf1072d91a06b82e44f02a4e425b2";

        // height based activations
        consensus.height_last_PoW = 350;
        consensus.height_start_BIP65 = 851019;              // Not defined for regtest. Inherit TestNet value.
        consensus.height_start_MessSignaturesV2 = 1;
        consensus.height_start_StakeModifierNewSelection = 0;
        consensus.height_start_StakeModifierV2 = 251;       // start with modifier V2 on regtest
        consensus.height_start_TimeProtoV2 = 999999999;

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_PURPLE_FENIX].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0xaa;
        pchMessageStart[1] = 0xcb;
        pchMessageStart[2] = 0x7c;
        pchMessageStart[3] = 0xad;
        nDefaultPort = 8552;

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }

    void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
    {
        assert(idx > Consensus::BASE_NETWORK && idx < Consensus::MAX_NETWORK_UPGRADES);
        consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
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

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    regTestParams.UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}
