// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2015-2019 The PIVX developers
// Copyright (c) 2022-2023 The SUDA developers
// Copyright (C) 2011-2022 The Litecoin Core developers
// Copyright (c) 2023 The CeilingCatCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/block.h"

#include "chainparams.h"
#include "hash.h"
#include "script/standard.h"
#include "script/sign.h"
#include "tinyformat.h"
#include "utilstrencodings.h"
#include "util.h"

#include "crypto/kawpow.h"
#include "crypto/scrypt_pow.h"

uint32_t fActivationKAWPOW = std::numeric_limits<uint32_t>::max();

void SetKAWPOWActivation(uint32_t nTime)
{
    fActivationKAWPOW = nTime;
}

bool CBlockHeader::IsKAWPOW() const
{
    return nTime >= fActivationKAWPOW;
}

uint256 CBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CBlockHeader::GetPoWHash() const
{
    if (IsKAWPOW()) {
        uint256 mixHash;
        return KAWPOWHash(*this, mixHash);
    }

    uint256 thash;
    scrypt_1024_1_1_256(BEGIN(nVersion), BEGIN(thash));
    return thash;
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=%d, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (unsigned int i = 0; i < vtx.size(); i++)
    {
        s << "  " << vtx[i].ToString() << "\n";
    }
    return s.str();
}

void CBlock::print() const
{
    LogPrintf("%s", ToString());
}
