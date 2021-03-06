///////////////////////////////////////////////////////////////////////////////
//
// Tr069XmlNodeString.h
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __Tr069XmlNodeString_h_
#define __Tr069XmlNodeString_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <libxml/parser.h>

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Forward Declarations
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Functions / Classes
///////////////////////////////////////////////////////////////////////////////

class XmlNodeString
{
public:
    XmlNodeString( xmlNodePtr node )
    {
        xmlChar * xmlNodeListString = xmlNodeListGetString( node->doc, node->xmlChildrenNode, 1);

        if(xmlNodeListString)
        {
            m_xmlString = (const char *)xmlNodeListString;
            xmlFree(xmlNodeListString);
        }

    }
	~XmlNodeString() { }

	const std::string & str() const { return m_xmlString;}
private:
    std::string m_xmlString;

};

#endif
