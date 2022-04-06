// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2021-2022 The SafeMineCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"
#include "arith_uint256.h"

using namespace std;

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.nTime = nTime;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 0  << CScriptNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}


static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Kiamat Tahun 2060 - 12/18/2021";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}


/*** M A I N   N E T W O R K ***/

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nMaxReorganizationDepth = 500;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV2 = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nTargetTimespan =  10 * 60;
        consensus.nTargetSpacingV1 = 2 * 60;
        consensus.nTargetSpacing = 2 * 60;
        consensus.BIP34Height = -1;
        consensus.BIP34Hash = uint256();
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.fPoSNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 5;
        consensus.nMinerConfirmationWindow = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        consensus.nProtocolV1RetargetingFixedTime = 1639822305;
        consensus.nProtocolV2Time = 1639822306;
        consensus.nProtocolV3Time = 1639822307;
        consensus.nLastPOWBlock = 2000000;
        consensus.nStakeTimestampMask = 0xf;
        consensus.nCoinbaseMaturity = 20;
        consensus.nStakeMinAge = 12 * 60 * 60;

        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000100001");

        pchMessageStart[0] = 0xb5;
        pchMessageStart[1] = 0x3d;
        pchMessageStart[2] = 0x80;
        pchMessageStart[3] = 0x00;
        nDefaultPort = 25252;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1639822305, 445016, 0x1e0ffff0, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();

        if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) {
		std::cout << std::string("Calculating main genesis block...\n");
            arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
            uint256 hash;
            genesis.nNonce = 0;
            while (UintToArith256(genesis.GetHash()) > hashTarget)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    ++genesis.nTime;
                }
            }
            std::cout << "Genesis block found!\n";
            std::cout << "nonce: " << genesis.nNonce << "\n";
            std::cout << "time: " << genesis.nTime << "\n";
            std::cout << "blockhash: " << genesis.GetHash().ToString().c_str() << "\n";
            std::cout << "merklehash: " << genesis.hashMerkleRoot.ToString().c_str() << "\n";
        }
        assert(consensus.hashGenesisBlock == uint256S("0x00000515601d3a48a250b62584becbc12fa50fb12609dc81b0732e9a05804d72"));
        assert(genesis.hashMerkleRoot == uint256S("0xb1aaa2e7d1bc5b982c83b94787057b2e7394062097e07944d55b51d23549a452"));

        vSeeds.push_back(CDNSSeedData("dnsseed.safeminecoin.org", "dnsseed.safeminecoin.org",true));
		vSeeds.push_back(CDNSSeedData("51.77.48.45", "51.77.48.45",true));   
		vSeeds.push_back(CDNSSeedData("51.195.249.132", "51.195.249.132",true));
        vSeeds.push_back(CDNSSeedData("135.125.225.55", "135.125.225.55",true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,63); // S
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,28); // C
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1,52); // M & N
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        cashaddrPrefix = "SafeMineCoin";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
                    boost::assign::map_list_of
                    (0, uint256S("0x00000515601d3a48a250b62584becbc12fa50fb12609dc81b0732e9a05804d72"))
                    (123600, uint256S("db7b2e65576491e6dd0d44008870d6252d06c5669dc6ea929dce3c0c755fcad6")),					
                    1648805367, // * UNIX timestamp of last checkpoint block
                    0,    // * total number of transactions between genesis and last checkpoint
                                //   (the tx=... number in the SetBestChain debug.log lines)
                    0      // * estimated number of transactions per day after checkpoint
        };
   }
};
static CMainParams mainParams;


/*** T E S T N E T ***/

class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nMaxReorganizationDepth = 500;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV2 = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nTargetTimespan =  10 * 60;
        consensus.nTargetSpacingV1 = 2 * 60;
        consensus.nTargetSpacing = 2 * 60;
        consensus.BIP34Height = -1;
        consensus.BIP34Hash = uint256();
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.fPoSNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512;
        consensus.nMinerConfirmationWindow = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        consensus.nProtocolV1RetargetingFixedTime = 1639925421;
        consensus.nProtocolV2Time = 1639925422;
        consensus.nProtocolV3Time = 1639925423;
        consensus.nLastPOWBlock = 0x1e0ffff0;
        consensus.nStakeTimestampMask = 0xf;
        consensus.nCoinbaseMaturity = 10;
        consensus.nStakeMinAge = 12 * 60 * 60;

        pchMessageStart[0] = 0x42;
        pchMessageStart[1] = 0x56;
        pchMessageStart[2] = 0xb1;
        pchMessageStart[3] = 0x41;
        nDefaultPort = 35353;

        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000000100001");

        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1639925421, 2121665, 0x1e0ffff0, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();

        if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) {
		std::cout << std::string("Calculating testnet genesis block...\n");
            arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
            uint256 hash;
            genesis.nNonce = 0;
            while (UintToArith256(genesis.GetHash()) > hashTarget)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    ++genesis.nTime;
                }
            }
            std::cout << "Genesis block found!\n";
            std::cout << "nonce: " << genesis.nNonce << "\n";
            std::cout << "time: " << genesis.nTime << "\n";
            std::cout << "blockhash: " << genesis.GetHash().ToString().c_str() << "\n";
            std::cout << "merklehash: " << genesis.hashMerkleRoot.ToString().c_str() << "\n";
        }
        assert(consensus.hashGenesisBlock == uint256S("0x000004fe4e10bad7e0a800bc952c3c84b8ccbe944228c294f1d6d2398dfcf7f5"));
        assert(genesis.hashMerkleRoot == uint256S("0x287a180add21a87aa2662ed5f9612fa0e3326046c8aa2e3b446702a3ccf698f0"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,66); // T
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,65); // T
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,111); // m & n
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        cashaddrPrefix = "smcntest";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x000004fe4e10bad7e0a800bc952c3c84b8ccbe944228c294f1d6d2398dfcf7f5")),
            1639925421,
            0,
            0
        };
    }
};
static CTestNetParams testNetParams;


/*** R E G R E S S I O N   T E S T ***/

class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nMaxReorganizationDepth = 50;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV2 = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nTargetTimespan =  10 * 60;
        consensus.nTargetSpacingV1 = 64;
        consensus.nTargetSpacing = 2 * 60;
        consensus.BIP34Height = -1;
        consensus.BIP34Hash = uint256();
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108;
        consensus.nMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        consensus.nProtocolV1RetargetingFixedTime = 1639925421;
        consensus.nProtocolV2Time = 1639925422;
        consensus.nProtocolV3Time = 1639925423;
        consensus.nLastPOWBlock = 1000;
        consensus.nStakeTimestampMask = 0xf;
        consensus.nCoinbaseMaturity = 10;
        consensus.nStakeMinAge = 1 * 60 * 60;

        pchMessageStart[0] = 0x3d;
        pchMessageStart[1] = 0x80;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0x06;
        nDefaultPort = 45454;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1639925421, 2121665, 0x1e0ffff0, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();

        if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) {
		std::cout << std::string("Calculating testnet genesis block...\n");
            arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
            uint256 hash;
            genesis.nNonce = 0;
            while (UintToArith256(genesis.GetHash()) > hashTarget)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    ++genesis.nTime;
                }
            }
            std::cout << "Genesis block found!\n";
            std::cout << "nonce: " << genesis.nNonce << "\n";
            std::cout << "time: " << genesis.nTime << "\n";
            std::cout << "blockhash: " << genesis.GetHash().ToString().c_str() << "\n";
            std::cout << "merklehash: " << genesis.hashMerkleRoot.ToString().c_str() << "\n";
        }
        assert(consensus.hashGenesisBlock == uint256S("0x000004fe4e10bad7e0a800bc952c3c84b8ccbe944228c294f1d6d2398dfcf7f5"));
        assert(genesis.hashMerkleRoot == uint256S("0x287a180add21a87aa2662ed5f9612fa0e3326046c8aa2e3b446702a3ccf698f0"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,61); // R
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,60); // R 
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1,126); // s & t
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        cashaddrPrefix = "smcnreg";

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}

