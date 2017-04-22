///////////////////////////////////////////////////////////////////////////////
//
// NetUtils.h
//
// Networking utility functions.  E.g. to get current host's local IP address.
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __NetUtils_h_
#define __NetUtils_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////


namespace threeway
{

///////////////////////////////////////////////////////////////////////////////
// Forward Declarations
///////////////////////////////////////////////////////////////////////////////

class IPv4Address;
class MacAddress;

///////////////////////////////////////////////////////////////////////////////
// Functions / Classes
///////////////////////////////////////////////////////////////////////////////

class NetUtils
{
public:
    /**
     * Get local IP address.  Works with single interface, not sure what you'll
     * get if multi-homed.
     * 
     * \param localIpAddress Gets filled in with IP address or left unset if
     *                       couldn't get IP address.
     */
    static void GetLocalIpAddress(IPv4Address& localIpAddress);
    
    /**
     * Get MAC address of specified interface.
     * 
     * \param interface  The interface to get the MAC address of e.g. "eth0"
     * \param macAddress Gets filled in with MAC address.
     * \return true if successful, false otherwise.
     */
    static bool GetMacAddress(const char * interface, MacAddress& macAddress);
};

}

#endif
