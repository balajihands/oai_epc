///////////////////////////////////////////////////////////////////////////////
//
// OamHwEntity.h
//
// This class handles sending of messages to l1l2 task.
//
// Copyright radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __OamHwEntity_h_
#define __OamHwEntity_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////
#include <map>
#include <messaging/transport/SingleThreadedApplicationWithMessaging.h>
#include <system/AlarmEvent.h>
#include <system/AlarmEventList.h>

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

namespace threeway
{
///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Functions / Classes
///////////////////////////////////////////////////////////////////////////////

class StuckAlarms : public AlarmEventListVisitor
{
public:
    StuckAlarms();
    virtual ~StuckAlarms();

    void VisitAlarmEvent(const AlarmEvent& alarmEvent);
    void Clear(SingleThreadedApplicationWithMessaging & app);

private:
    std::map<u32,AlarmEvent> m_stuckAlarms;
};


}

#endif