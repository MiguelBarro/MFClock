
// eProsimaClock.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// fastrts dependencies

#include <fastrtps/participant/ParticipantListener.h>
#include <fastrtps/subscriber/SubscriberListener.h>

#include "Type/eProsimaClockTypePubSubTypes.h"

#include <set>  


// CeProsimaClockApp:
// See eProsimaClock.cpp for the implementation of this class
//

class CeProsimaClockApp : 
    public CWinApp,
    public eprosima::fastrtps::ParticipantListener,
    public eprosima::fastrtps::SubscriberListener

{
    // fastrtps 
    std::shared_ptr<eprosima::fastrtps::Participant> m_part;
    std::shared_ptr<eprosima::fastrtps::Publisher> m_pub;
    std::shared_ptr<eprosima::fastrtps::Subscriber> m_sub;
    eProsimaClockPubSubType m_type;

    // counter state
    CTimeSpan counter; // keeps the former state
    CTime reference;

    // List of available publishers
    std::set<eprosima::fastrtps::rtps::GUID_t> m_publishers;

public:
	CeProsimaClockApp();

// Overrides
public:
	virtual BOOL InitInstance();
    void OnTimer(UINT_PTR nIDEvent);
    void OnCloseDialog();

protected:

    void createPublisher();
    void createSubscriber();

// Listener overrides, only interested on the publishers
    void onPublisherDiscovery(eprosima::fastrtps::Participant* participant, eprosima::fastrtps::rtps::WriterDiscoveryInfo&& info) override;
    void onNewDataMessage(eprosima::fastrtps::Subscriber* sub) override;

// Implementation

	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedEndpointType();
    afx_msg void OnUpdateDisplays(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEndpointKind(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTimerState(CCmdUI *pCmdUI);
};


