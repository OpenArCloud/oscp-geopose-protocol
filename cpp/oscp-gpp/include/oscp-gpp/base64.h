// This file was copied and adapted from https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c

#ifndef _OSCP_BASE64_H_
#define _OSCP_BASE64_H_

#include <vector>
#include <string>
typedef unsigned char BYTE;

namespace oscp {
std::string base64_encode(BYTE const* buf, unsigned int bufLen);
std::vector<BYTE> base64_decode(std::string const&);
} // namespace oscp

#endif // __OSCP_BASE64_H_
