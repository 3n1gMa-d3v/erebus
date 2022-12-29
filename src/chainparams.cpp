// SPDX-FileCopyrightText: © 2020 Phantom Developers
// SPDX-FileCopyrightText: © 2016 SpectreCoin Developers
// SPDX-FileCopyrightText: © 2010 Satoshi Nakamoto
// SPDX-FileCopyrightText: © 2009 Bitcoin Developers
//
// SPDX-License-Identifier: MIT

#include "assert.h"

#include "chainparams.h"
#include "main.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

int64_t CChainParams::GetProofOfWorkReward(int nHeight, int64_t nFees) const
{
    // miner's coin base reward
    int64_t nSubsidy = 0;

    if(nHeight == 1)
        nSubsidy = (NetworkID() == CChainParams::TESTNET ? 2000000 : 20000000) * COIN;  // 20Mill Pre-mine on MainNet, 2Mill pre-mine on TestNet

    else if(nHeight <= nLastPOWBlock)
        nSubsidy = 3;

    if (fDebug && GetBoolArg("-printcreation"))
        LogPrintf("GetProofOfWorkReward() : create=%s nSubsidy=%d\n", FormatMoney(nSubsidy).c_str(), nSubsidy);

    return nSubsidy;
};


int64_t CChainParams::GetProofOfStakeReward(const CBlockIndex* pindexPrev, int64_t nCoinAge, int64_t nFees) const
{
    // miner's coin stake reward based on coin age spent (coin-days)
    int64_t nSubsidy;

    if (IsForkV4SupplyIncrease(pindexPrev))
        nSubsidy = (NetworkID() == CChainParams::TESTNET ? 300000 : 3000000) * COIN;
    else if (IsProtocolV3(pindexPrev->nHeight))
        nSubsidy = Params().IsForkV3(pindexPrev->GetBlockTime()) ?
                    nStakeReward :
                    (pindexPrev->nMoneySupply / COIN) * COIN_YEAR_REWARD / (365 * 24 * (60 * 60 / 64));
    else
        nSubsidy = nCoinAge * COIN_YEAR_REWARD * 33 / (365 * 33 + 8);

    if (fDebug && GetBoolArg("-printcreation"))
    {
        if (IsProtocolV3(pindexPrev->nHeight))
            LogPrintf("GetProofOfStakeReward(): create=%s\n", FormatMoney(nSubsidy).c_str());
        else
            LogPrintf("GetProofOfStakeReward(): create=%s nCoinAge=%d\n", FormatMoney(nSubsidy).c_str(), nCoinAge);
    }

    return nSubsidy + nFees;
}

int64_t CChainParams::GetProofOfAnonStakeReward(const CBlockIndex* pindexPrev, int64_t nFees) const
{
    int64_t nSubsidy = nAnonStakeReward;
    if (IsForkV4SupplyIncrease(pindexPrev))
        nSubsidy = (NetworkID() == CChainParams::TESTNET ? 300000 : 3000000) * COIN;

    if (fDebug && GetBoolArg("-printcreation"))
        LogPrintf("GetProofOfAnonStakeReward(): create=%s\n", FormatMoney(nSubsidy).c_str());

    return nSubsidy + nFees;
}

bool CChainParams::IsForkV4SupplyIncrease(const CBlockIndex* pindexPrev) const
{
    return pindexPrev->GetBlockTime() >= nForkV4Time && pindexPrev->pprev->GetBlockTime() < nForkV4Time;
}

//
// Main network
//

// Convert the pnSeeds array into usable address objects.
static void convertSeeds(std::vector<CAddress> &vSeedsOut, const unsigned int *data, unsigned int count, int port)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int k = 0; k < count; ++k)
    {
        struct in_addr ip;
        unsigned int i = data[k], t;

        // -- convert to big endian
        t =   (i & 0x000000ff) << 24u
            | (i & 0x0000ff00) << 8u
            | (i & 0x00ff0000) >> 8u
            | (i & 0xff000000) >> 24u;

        memcpy(&ip, &t, sizeof(ip));

        CAddress addr(CService(ip, port));
        addr.nTime = GetTime()-GetRand(nOneWeek)-nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CBaseChainParams : public CChainParams {
public:
    CBaseChainParams() {
        const char* pszTimestamp = "Fenton Testnet | Stage One";
        CTransaction txNew;
        txNew.nTime = GENESIS_BLOCK_TIME;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 0 << CBigNum(42) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].SetEmpty();
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = GENESIS_BLOCK_TIME;

        vSeeds.push_back(CDNSSeedData("", ""));
        vSeeds.push_back(CDNSSeedData("", ""));
        vSeeds.push_back(CDNSSeedData("", ""));
        vSeeds.push_back(CDNSSeedData("", ""));
      }
    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual const std::vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    std::vector<CAddress> vFixedSeeds;
};

class CMainParams : public CBaseChainParams {
public:
    CMainParams() {
        strNetworkID = "main";

        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0x33;
        pchMessageStart[2] = 0x1d;
        pchMessageStart[3] = 0xa7;

        vAlertPubKey = ParseHex("04dba01336e48d52bc625e4144549e7483eb193d6bc28ebabe44c2ea1d118eadc49dd432f96ecec15ebe0f194f34ac2deabbf70d8f17c215421b39abd96717297d");

        nDefaultPort = 12600;
        nRPCPort = 12601;
        nBIP44ID = 0x800000d5;

        //nLastPOWBlock = 2016; // Running for 1 Week after ICO
        nLastPOWBlock = 17000;
        nFirstPosv2Block = 17001;
        nFirstPosv3Block = 17010;

        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20); // "standard" scrypt target limit for proof of work, results with 0,000244140625 proof-of-work difficulty
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);
        bnProofOfStakeLimitV2 = CBigNum(~uint256(0) >> 48);

        nStakeMinConfirmationsLegacy = 288;
        nStakeMinConfirmations = 450; // block time 96 seconds * 450 = 12 hours

        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 251624;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xfea263d08105f069b10b0fec357b8d5e102dc9d8a8e1443f82860fb7eb47a49e"));
        assert(genesis.hashMerkleRoot == uint256("0x2b9855ce2c8db2d4e88cf4524a48571ea18c4907d59661e0b8de24632fb59d93"));

        base58Prefixes[PUBKEY_ADDRESS]      = list_of(63).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SCRIPT_ADDRESS]      = list_of(136).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SECRET_KEY]          = list_of(179).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[STEALTH_ADDRESS]     = list_of(40).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_PUBLIC_KEY]      = list_of(0x2C)(0x51)(0x3B)(0xD7).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY]      = list_of(0x2C)(0x51)(0xC1)(0x5A).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_KEY_HASH]        = list_of(137).convert_to_container<std::vector<unsigned char> >();         // x
        base58Prefixes[EXT_ACC_HASH]        = list_of(83).convert_to_container<std::vector<unsigned char> >();          // a
        base58Prefixes[EXT_PUBLIC_KEY_BTC]  = list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >(); // xprv
        base58Prefixes[EXT_SECRET_KEY_BTC]  = list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >(); // xpub

        convertSeeds(vFixedSeeds, pnSeed, ARRAYLEN(pnSeed), nDefaultPort);

        nForkV2Time = 1534888800; // MAINNET V2 chain fork (GMT: Tuesday, 21. August 2018 22.00)
        nForkV3Time = 1558123200; // MAINNET V3 chain fork (GMT: Friday, 17. May 2019 20:00:00)
        nForkV4Time = 1569614400; // MAINNET V4 chain fork (GMT: Friday, 27. September 2019 20:00:00)

        devContributionAddress = "SdrdWNtjD7V6BSt3EyQZKCnZDkeE28cZhr";
        supplyIncreaseAddress = "SSGCEMb6xESgmuGXkx7yozGDxhVSXzBP3a";
    }

    virtual Network NetworkID() const { return CChainParams::MAIN; }
};
static CMainParams mainParams;

//
// Testnet
//

class CTestNetParams : public CBaseChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        strDataDir = "testnet";

        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xb1;
        pchMessageStart[1] = 0x3a;
        pchMessageStart[2] = 0x25;
        pchMessageStart[3] = 0xc1;

        vAlertPubKey = ParseHex("04dba01336e48d52bc625e4144549e7483eb193d6bc28ebabe44c2ea1d118eadc49dd432f96ecec15ebe0f194f34ac2deabbf70d8f17c215421b39abd96717297d");

        nDefaultPort = 12700;
        nRPCPort = 12701;
        nBIP44ID = 0x80000001;

        nLastPOWBlock = 20;
        nFirstPosv2Block = 20;
        nFirstPosv3Block = 500;

        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);
        bnProofOfStakeLimitV2 = CBigNum(~uint256(0) >> 46);

        nStakeMinConfirmationsLegacy = 28;
        nStakeMinConfirmations = 30;

        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 20;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x80b1d2841ab066b9041144711f4d33e11ffba078274ca82395086b64b6b9b006"));

        base58Prefixes[PUBKEY_ADDRESS]      = list_of(127).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SCRIPT_ADDRESS]      = list_of(196).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SECRET_KEY]          = list_of(255).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[STEALTH_ADDRESS]     = list_of(40).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_PUBLIC_KEY]      = list_of(0x76)(0xC0)(0xFD)(0xFB).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY]      = list_of(0x76)(0xC1)(0x07)(0x7A).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_KEY_HASH]        = list_of(75).convert_to_container<std::vector<unsigned char> >();          // X
        base58Prefixes[EXT_ACC_HASH]        = list_of(23).convert_to_container<std::vector<unsigned char> >();          // A
        base58Prefixes[EXT_PUBLIC_KEY_BTC]  = list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >(); // tprv
        base58Prefixes[EXT_SECRET_KEY_BTC]  = list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >(); // tpub

        convertSeeds(vFixedSeeds, pnTestnetSeed, ARRAYLEN(pnTestnetSeed), nDefaultPort);

        nForkV2Time = 1532466000; // TESTNET V2 chain fork (GMT: Tuesday, 24. July 2018 21.00)
        nForkV3Time = 1546470000; // TESTNET V3 chain fork (01/02/2019 @ 11:00pm (UTC))
        nForkV4Time = 1567972800; // TESTNET V4 chain fork (Sunday, 8. September 2019 20:00:00)


        devContributionAddress = "tSJoPZoXumJyDmGKYo9Y7SZkJvymESFYkD";
        supplyIncreaseAddress = devContributionAddress;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        strDataDir = "regtest";

        nDefaultPort = 12800;

        nFirstPosv2Block = -1;
        nFirstPosv3Block = -1;

        pchMessageStart[0] = 0x2a;
        pchMessageStart[1] = 0xd3;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0x3c;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1671851644;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 2;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xbe731d5714b9ff20146ab61a3c5c6d95b71ab5dd3a4411464c48e1e8e4faaec6"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

const CChainParams &TestNetParams() {
    return testNetParams;
}

const CChainParams &MainNetParams() {
    return mainParams;
}

void SelectParams(CChainParams::Network network)
{
    switch (network)
    {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    };
};

bool SelectParamsFromCommandLine()
{
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest)
    {
        return false;
    };

    if (fRegTest)
    {
        SelectParams(CChainParams::REGTEST);
    } else
    if (fTestNet)
    {
        SelectParams(CChainParams::TESTNET);
    } else
    {
        SelectParams(CChainParams::MAIN);
    };

    return true;
}
