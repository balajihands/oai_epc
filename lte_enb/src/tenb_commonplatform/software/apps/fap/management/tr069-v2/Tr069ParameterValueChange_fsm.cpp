///////////////////////////////////////////////////////////////////////////////
//
//Tr069ParameterValueChange_fsm.cpp
//
// Autogenerated from Tr069ParameterValueChange.fsm
//
// See header file for documentation.
//
// Copyright radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <system/Trace.h>

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "Tr069ParameterValueChange_fsm.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Static Data
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

using namespace tr069;

namespace tr069
{

/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
ParameterValueChangeFsm::ParameterValueChangeFsm(
    ParameterValueChangeFsmInterface & interfaceClass,
    bool traceEnabled,
    const char * name 
    ) :
    mInterface(interfaceClass),
    mFsmTraceEnabled(traceEnabled),
    mName(name),
    mState(NULL),
    mNotConnectedToHms(*this ),
    mHmsConnectionEstablished(*this ),
    mIdle(*this ),
    mActiveNotificationPending(*this ),
    mfsmEventQueue(),
    mFsmInjectInProgress(false),
    mEventCounter(0)
{
    Entry();
}

/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
ParameterValueChangeFsm::~ParameterValueChangeFsm()
{
    RSYS_ASSERT_PRINTF(!mFsmInjectInProgress, "ParameterValueChangeFsm deleted while event-inject in progress.");

    mfsmEventQueue.clear();
    mCurrentStateTransitionExitsList.clear();
}

void ParameterValueChangeFsm::Entry()
{
    // Initial State
    mNotConnectedToHms.Entry();

    while( !mState->RunToCompletion() )
    {
        continue;
    }
}

/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
void ParameterValueChangeFsm::InjectHmsConnection(void* event)
{
    shared_ptr<FsmEvent> fsmEvent(new FsmEvent_void_STAR_(event));
    Enqueue( ParameterValueChangeFsm::FSM_EVENT_HmsConnection, fsmEvent);
}
/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
void ParameterValueChangeFsm::InjectHmsDisconnection(void* event)
{
    shared_ptr<FsmEvent> fsmEvent(new FsmEvent_void_STAR_(event));
    Enqueue( ParameterValueChangeFsm::FSM_EVENT_HmsDisconnection, fsmEvent);
}
/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
void ParameterValueChangeFsm::InjectActiveNotify(void* event)
{
    shared_ptr<FsmEvent> fsmEvent(new FsmEvent_void_STAR_(event));
    Enqueue( ParameterValueChangeFsm::FSM_EVENT_ActiveNotify, fsmEvent);
}
/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
void ParameterValueChangeFsm::InjectTimerExpiry(void* event)
{
    shared_ptr<FsmEvent> fsmEvent(new FsmEvent_void_STAR_(event));
    Enqueue( ParameterValueChangeFsm::FSM_EVENT_TimerExpiry, fsmEvent);
}
/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
void ParameterValueChangeFsm::Enqueue( EventId eventId, shared_ptr<FsmEvent> fsmEvent )
{
    RSYS_ASSERT(eventId < NUMBER_OF_INJECTABLE_EVENTS);

    // Reentrancy Protection is NOT intended to make thread-safe
    shared_ptr<QueuedFsmEvent>  qEvent(new QueuedFsmEvent(eventId, fsmEvent));
    mfsmEventQueue.push_back(qEvent);

    if(!mFsmInjectInProgress)
    {
        mFsmInjectInProgress = true;

        list< shared_ptr<QueuedFsmEvent> >::iterator event = mfsmEventQueue.begin();

        while( event != mfsmEventQueue.end() )
        {
            RSYS_ASSERT(*event);
            RSYS_ASSERT(mState);

            EventId queuedEventId = (*event)->mId;

            RSYS_ASSERT(queuedEventId < NUMBER_OF_INJECTABLE_EVENTS);

            if( !mState->DeferEvent( queuedEventId ) )
            {
                ++mEventCounter;

                if(mFsmTraceEnabled) { TRACE_PRINTF("%s.%u: %s <--(%s)-- ", mName.c_str(), mEventCounter, mState->mName.c_str(), mEventName[queuedEventId] ); }

                mState->Inject(queuedEventId, (*event)->mFsmEvent);

                while( !mState->RunToCompletion() )
                {
                    continue;
                }

                mfsmEventQueue.erase(event);
                event = mfsmEventQueue.begin();
            }
            else
            {
                if(mFsmTraceEnabled) { TRACE_PRINTF("%s.-: %s (%s deferred)", mName.c_str(), mState->mName.c_str(), mEventName[queuedEventId] ); };
                event++;
            }
        }

        mFsmInjectInProgress = false;
    }
    else
    {
        if(mFsmTraceEnabled) { TRACE_PRINTF("%s.-: (%s enqueued)", mName.c_str(), mEventName[eventId] ); }
    }
}

/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
const char * ParameterValueChangeFsm::mEventName[NUMBER_OF_INJECTABLE_EVENTS] =
{
    "HmsConnection",
    "HmsDisconnection",
    "ActiveNotify",
    "TimerExpiry",
};

/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
void ParameterValueChangeFsm::SimpleState::Inject(
    EventId eventId,
    shared_ptr<FsmEvent> fsmEvent
    )
{
    RSYS_ASSERT(eventId < NUMBER_OF_INJECTABLE_EVENTS);

    switch(eventId)
    {
        case ParameterValueChangeFsm::FSM_EVENT_HmsConnection:
        {
            RSYS_ASSERT(FSM_EVENT_TYPE_void_STAR_ == fsmEvent->mEventType);
            shared_ptr<FsmEvent_void_STAR_> event = dynamic_pointer_cast<FsmEvent_void_STAR_>(fsmEvent);
            InjectHmsConnection(event->mEvent);
        }
        break;

        case ParameterValueChangeFsm::FSM_EVENT_HmsDisconnection:
        {
            RSYS_ASSERT(FSM_EVENT_TYPE_void_STAR_ == fsmEvent->mEventType);
            shared_ptr<FsmEvent_void_STAR_> event = dynamic_pointer_cast<FsmEvent_void_STAR_>(fsmEvent);
            InjectHmsDisconnection(event->mEvent);
        }
        break;

        case ParameterValueChangeFsm::FSM_EVENT_ActiveNotify:
        {
            RSYS_ASSERT(FSM_EVENT_TYPE_void_STAR_ == fsmEvent->mEventType);
            shared_ptr<FsmEvent_void_STAR_> event = dynamic_pointer_cast<FsmEvent_void_STAR_>(fsmEvent);
            InjectActiveNotify(event->mEvent);
        }
        break;

        case ParameterValueChangeFsm::FSM_EVENT_TimerExpiry:
        {
            RSYS_ASSERT(FSM_EVENT_TYPE_void_STAR_ == fsmEvent->mEventType);
            shared_ptr<FsmEvent_void_STAR_> event = dynamic_pointer_cast<FsmEvent_void_STAR_>(fsmEvent);
            InjectTimerExpiry(event->mEvent);
        }
        break;

        default:
            RSYS_ASSERT(false);
            break;
    }
}

/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
void ParameterValueChangeFsm::NotConnectedToHms::InjectHmsConnection(
    void* event
    )
{
    {
        mFsm.mState->Exit();
        mFsm.mHmsConnectionEstablished.Entry();
    }
}

// Event HmsDisconnection not handled in state NotConnectedToHms

// Event ActiveNotify not handled in state NotConnectedToHms

// Event TimerExpiry not handled in state NotConnectedToHms

// State NotConnectedToHms has no deferred events.

// Event HmsConnection not handled in state Idle

/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
void ParameterValueChangeFsm::Idle::InjectHmsDisconnection(
    void* event
    )
{
    {
        mFsm.mState->Exit();
        mFsm.mState->Exit();
        mFsm.mNotConnectedToHms.Entry();
    }
}

/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
void ParameterValueChangeFsm::Idle::InjectActiveNotify(
    void* event
    )
{
    {
        mFsm.mState->Exit();
        mFsm.TracePoint( "Action NewValueChangeTransactionSession" );

        mFsm.mInterface.NewValueChangeTransactionSession(event);

        mFsm.mActiveNotificationPending.Entry();
    }
}

// Event TimerExpiry not handled in state Idle

// State Idle has no deferred events.

// Event HmsConnection not handled in state ActiveNotificationPending

/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
void ParameterValueChangeFsm::ActiveNotificationPending::InjectHmsDisconnection(
    void* event
    )
{
    {
        mFsm.mState->Exit();
        mFsm.mState->Exit();
        mFsm.mNotConnectedToHms.Entry();
    }
}

// Event ActiveNotify not handled in state ActiveNotificationPending

/*** WARNING: Code AUTO-GENERATED from Tr069ParameterValueChange.fsm. Do not edit! ***/
void ParameterValueChangeFsm::ActiveNotificationPending::InjectTimerExpiry(
    void* event
    )
{
    // Guard condition
    if( mFsm.mInterface.QueryActiveNotifyStillPending(event))
    {
        mFsm.TracePoint( "QueryActiveNotifyStillPending? (true)" );

        mFsm.mState->Exit();
        mFsm.TracePoint( "Action NewValueChangeTransactionSession" );

        mFsm.mInterface.NewValueChangeTransactionSession(event);

        mFsm.mActiveNotificationPending.Entry();
    }
    else
    {
        mFsm.mState->Exit();
        mFsm.mIdle.Entry();
    }
}

// State ActiveNotificationPending has no deferred events.

} // -namespace-

