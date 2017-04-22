///////////////////////////////////////////////////////////////////////////////
//
// Tr069DeleteObject_fsm.h
//
// Autogenerated from Tr069DeleteObject.fsm
//
// See header file for documentation.
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __Tr069DeleteObject_fsm_h_
#define __Tr069DeleteObject_fsm_h_
///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <list>
#include <deque>
#include <system/Trace.h>

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "Tr069DeleteObject_fsm_interface.h"

using namespace tr069;

namespace tr069
{

/*** WARNING: Code AUTO-GENERATED from Tr069DeleteObject.fsm. Do not edit! ***/
class DeleteObjectFsm
{
public:

    DeleteObjectFsm(
        DeleteObjectFsmInterface & interfaceClass,
        bool traceEnabled = false,
        const char * name = "DeleteObjectFsm"
        );
    virtual ~DeleteObjectFsm();

    void InjectDeleteObject(shared_ptr<RpcDeleteObject> event);
    void InjectMibDeleteObjectCnf(shared_ptr<MibDeleteObjectCnf> event);
    std::string ToString() const
    {
        std::string returnedString(mName);

        if(mState)
        {
            returnedString += ".";
            returnedString += mState->mName;
        }

        return returnedString;
    }

    void Rename( const std::string & newName)
    {
        if(mFsmTraceEnabled)
        {
        	TRACE_PRINTF("%s.-: Renamed to %s", mName.c_str(), newName.c_str() );
        }
        mName = newName;
    }

    void SetFsmTraceEnable( bool traceEnable = true ) { mFsmTraceEnabled = traceEnable; }

    void TracePoint( const char * s )
    {
        if(mFsmTraceEnabled)
        {
        	TRACE_PRINTF("%s.%u: %s", mName.c_str(), mEventCounter, s );
        }
    }

private:
    typedef enum EventId_tag
    {
        FSM_EVENT_DeleteObject,
        FSM_EVENT_MibDeleteObjectCnf,
        NUMBER_OF_INJECTABLE_EVENTS
    } EventId;

    typedef enum EventType_tag
    {
        FSM_EVENT_TYPE_shared_ptr_LT_RpcDeleteObject_GT_,
        FSM_EVENT_TYPE_shared_ptr_LT_MibDeleteObjectCnf_GT_,
        NUMBER_OF_EVENT_TYPES
    } EventType;

    class FsmEvent
    {
    public:
        FsmEvent( EventType eventType ) :
            mEventType(eventType) {}
        virtual ~FsmEvent(){}

        EventType mEventType;
    };

    class FsmEvent_shared_ptr_LT_RpcDeleteObject_GT_ : public FsmEvent
    {
    public:
        FsmEvent_shared_ptr_LT_RpcDeleteObject_GT_( shared_ptr<RpcDeleteObject> event ) :
            FsmEvent(FSM_EVENT_TYPE_shared_ptr_LT_RpcDeleteObject_GT_),
            mEvent(event) {}
        ~FsmEvent_shared_ptr_LT_RpcDeleteObject_GT_() {}
    	shared_ptr<RpcDeleteObject> mEvent;
    };

    class FsmEvent_shared_ptr_LT_MibDeleteObjectCnf_GT_ : public FsmEvent
    {
    public:
        FsmEvent_shared_ptr_LT_MibDeleteObjectCnf_GT_( shared_ptr<MibDeleteObjectCnf> event ) :
            FsmEvent(FSM_EVENT_TYPE_shared_ptr_LT_MibDeleteObjectCnf_GT_),
            mEvent(event) {}
        ~FsmEvent_shared_ptr_LT_MibDeleteObjectCnf_GT_() {}
    	shared_ptr<MibDeleteObjectCnf> mEvent;
    };

    void Enqueue( EventId eventId, shared_ptr<FsmEvent> fsmEvent );

    class State;
    typedef std::deque< State * > StateHistory;

    class State
    {
    public:
        State(DeleteObjectFsm & fsm, const char * name ) :
            mFsm(fsm),
            mName(name){}
        virtual ~State(){}

        virtual void Inject( EventId eventId, shared_ptr<FsmEvent> fsmData ) { RSYS_ASSERT_PRINTF(false,"Cannot inject in state %s.", mName.c_str()); }
        virtual bool RunToCompletion() { RSYS_ASSERT_PRINTF(false,"RunToCompletion failure in state %s.", mName.c_str()); return false; };
        virtual bool DeferEvent( EventId ) const { RSYS_ASSERT_PRINTF(false,"Cannot defer in state %s.", mName.c_str()); return false; }

        virtual void Entry()
        {
            RSYS_ASSERT( mFsm.mState == GetSuperstate() );

            UpdateHistoryUponEntry();

            if(mFsm.mFsmTraceEnabled) { TRACE_PRINTF("%s.%u: %s (entry) ", mFsm.mName.c_str(), mFsm.mEventCounter, mName.c_str() ); }

            InterfaceEntryMethod();

            mFsm.mState = this;
        }

        virtual void Exit()
        {
            RSYS_ASSERT( mFsm.mState == this);

            UpdateHistoryUponExit();

            if(mFsm.mFsmTraceEnabled) { TRACE_PRINTF("%s.%u: %s (exit) ", mFsm.mName.c_str(), mFsm.mEventCounter, mName.c_str() ); }

            InterfaceExitMethod();

            mFsm.mState =  GetSuperstate();
        }

        DeleteObjectFsm& mFsm;
        std::string mName;
        virtual State * GetSuperstate() const { return NULL; }
        virtual void  InterfaceEntryMethod() {}
        virtual void  InterfaceExitMethod() {}
        virtual void  UpdateHistoryUponExit() { mFsm.mCurrentStateTransitionExitsList.push_front( mFsm.mState ); }
        virtual void  UpdateHistoryUponEntry() { mFsm.mCurrentStateTransitionExitsList.clear(); }
    };

    class SimpleState : public State
    {
    public:
        SimpleState(DeleteObjectFsm & fsm, const char * name) :
            State(fsm, name){}
        virtual ~SimpleState(){}

        void Inject( EventId eventId, shared_ptr<FsmEvent> fsmData );
        virtual bool RunToCompletion() { return true; }

        virtual bool DeferEvent( EventId ) const { return false; }
        virtual void InjectDeleteObject( shared_ptr<RpcDeleteObject> eventData) { EventConsumed(FSM_EVENT_DeleteObject); }
        virtual void InjectMibDeleteObjectCnf( shared_ptr<MibDeleteObjectCnf> eventData) { EventConsumed(FSM_EVENT_MibDeleteObjectCnf); }

    private:
        void EventConsumed( EventId queuedEventId )
        {
            if(mFsm.mFsmTraceEnabled) { TRACE_PRINTF("%s.%u: %s consumed %s", mFsm.mName.c_str(), mFsm.mEventCounter, mName.c_str(), mEventName[queuedEventId] ); }
        }
    };

    class Idle : public SimpleState
    {
    public:
        Idle(DeleteObjectFsm & fsm) :
            SimpleState(fsm, "Idle") {}
        virtual ~Idle(){}

        void InterfaceEntryMethod( void ) { mFsm.mInterface.IdleEntry();}
        virtual void InjectDeleteObject( shared_ptr<RpcDeleteObject> eventData);
    };

    class DeleteObjectInProgress : public SimpleState
    {
    public:
        DeleteObjectInProgress(DeleteObjectFsm & fsm) :
            SimpleState(fsm, "DeleteObjectInProgress") {}
        virtual ~DeleteObjectInProgress(){}

        bool DeferEvent( EventId ) const ;
        virtual void InjectMibDeleteObjectCnf( shared_ptr<MibDeleteObjectCnf> eventData);
    };

    class Superstate : public State
    {
    public:
        Superstate(DeleteObjectFsm & fsm, const char * name) :
            State(fsm, name)
        { }
        virtual ~Superstate()
        { }
    };

    class SuperstateWithInitialState : public Superstate
    {
    public:
        SuperstateWithInitialState(DeleteObjectFsm & fsm, const char * name) :
            Superstate(fsm, name) {}
        virtual ~SuperstateWithInitialState() {}

        virtual bool RunToCompletion() = 0;
    };

    class QueuedFsmEvent
    {
    public:
        QueuedFsmEvent( EventId eventId, shared_ptr<FsmEvent> fsmEvent) :
            mId(eventId),
            mFsmEvent(fsmEvent){}
        ~QueuedFsmEvent(){}

        EventId mId;
        shared_ptr<FsmEvent> mFsmEvent;
    };

    DeleteObjectFsmInterface & mInterface;
    bool mFsmTraceEnabled;
    std::string mName;
    State * mState;
    Idle mIdle;
    DeleteObjectInProgress mDeleteObjectInProgress;
    std::list< shared_ptr<QueuedFsmEvent> > mfsmEventQueue;
    bool mFsmInjectInProgress;
    unsigned mEventCounter;
    StateHistory mCurrentStateTransitionExitsList;
    static const char * mEventName[NUMBER_OF_INJECTABLE_EVENTS];

};

} // -namespace-

#endif // __Tr069DeleteObject_fsm_interface_h_

