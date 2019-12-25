
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef EPGC_INVALID_OUTPOINTS_JSON_H
#define EPGC_INVALID_OUTPOINTS_JSON_H
#include <string>

std::string LoadInvalidOutPoints()
{
    std::string str = "[\n"
            "]";
    return str;
}

#endif //EPGC_INVALID_OUTPOINTS_JSON_H
