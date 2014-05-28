// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2011-2012 Litecoin Developers
// Copyright (c) 2013 Dogecoin Developers
// Copyright (c) 2014 Pawncoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    static struct sCheckpoint autoCheckpoint = {0, 0};

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //

    static MapCheckpoints mapCheckpoints =
            boost::assign::map_list_of
            (  0, hashGenesisBlockOfficial )
            (  64, uint256("0xc746c854072911b8cb47d4484d36d62630fff6a13b47fd1bedbe1bd301bf7961"))
            (  256, uint256("0xf2315532dfb3989a18ffadca02a3ad6211d14017b6b5e571754e6aa8cf1acb56"))
            (  32768, uint256("0x63a3a0e4fe7f43da54e750ec483f1f084306889e3a9d220ca442da8f5709b822"))
            (  34582 , uint256("0x04914a051dc440bd79dfd6fd3e6a79f62917947304d66fee51ce1de7486d861c"))
            (  37836 , uint256("0x1b0fb6c5913fa7b07b3fc6bb204dbf90c60fe6740c018e045118b56ce59d35d5"))
	;

    // Marks a new auto checkpoint
    void NewCheckPointBlock(int nHeight, const uint256& hash)
    {
	autoCheckpoint.height = nHeight;
	autoCheckpoint.hash = hash;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (fTestNet) return true; // Testnet has no checkpoints
	if (nHeight == autoCheckpoint.height) 
		return hash == autoCheckpoint.hash;
        MapCheckpoints::const_iterator i = mapCheckpoints.find(nHeight);
        if (i == mapCheckpoints.end()) return true;
        return hash == i->second;
		// return true;
    }

    int GetTotalBlocksEstimate()
    {
        if (fTestNet) return 0;
	
	if (autoCheckpoint.height) {
		return autoCheckpoint.height;
	}
        return mapCheckpoints.rbegin()->first;
		// return 0;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (fTestNet) return NULL;

	if (autoCheckpoint.height) {
		std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(autoCheckpoint.hash);
		if (t != mapBlockIndex.end())
			return t->second;
	}
	
        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, mapCheckpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
				// return NULL;
        }
        return NULL;
    }
}
