
// eProsimaClock.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "eProsimaClock.h"
#include "eProsimaClockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CeProsimaClockApp

BEGIN_MESSAGE_MAP(CeProsimaClockApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
    ON_BN_CLICKED(IDC_ENDPOINT_KIND, &CeProsimaClockApp::OnBnClickedEndpointType)
    ON_UPDATE_COMMAND_UI_RANGE(IDC_PUBLISHER_NAME, IDC_VALUE, &CeProsimaClockApp::OnUpdateDisplays)
    ON_UPDATE_COMMAND_UI(IDC_ENDPOINT_KIND, &CeProsimaClockApp::OnUpdateEndpointKind)
    ON_UPDATE_COMMAND_UI(IDC_TIMER_STATE, &CeProsimaClockApp::OnUpdateTimerState)
END_MESSAGE_MAP()


// CeProsimaClockApp construction

CeProsimaClockApp::CeProsimaClockApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CeProsimaClockApp object

CeProsimaClockApp theApp;

// CeProsimaClockApp initialization

BOOL CeProsimaClockApp::InitInstance()
{
    using namespace eprosima::fastrtps;

    // fastrtps support, enable use of xml config files
    ParticipantAttributes PParam;
    Domain::getDefaultParticipantAttributes(PParam);
    m_part.reset(Domain::createParticipant(PParam, this), &Domain::removeParticipant);

    if(!m_part)
    {
        AfxMessageBox(_T("Unable to create a fastrtps participant!"), MB_ICONERROR | MB_OK);
        return FALSE;
    }

    // register the type
    if(!Domain::registerType(m_part.get(), &m_type))
    {
        AfxMessageBox(_T("Unable to register the idl type!"), MB_ICONERROR | MB_OK);
        return FALSE;
    }

    // generate a subscriber by default
    createSubscriber();

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CeProsimaClockDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

    // Shutdown fastrpt operation is done before the dialog is closed to prevent
    // callbacks from querying non-existent window (see OnCloseDialog)

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CeProsimaClockApp::OnCloseDialog()
{
    using namespace eprosima::fastrtps;

    // Free fastrtps resources
    m_pub.reset();
    m_sub.reset();
    Domain::unregisterType(m_part.get(), m_type.getName());
    m_part.reset();
    Domain::stopAll();
}

void CeProsimaClockApp::OnUpdateDisplays(CCmdUI *pCmdUI)
{
    using namespace std;

    CTimeSpan ts;
    CWnd * pE = pCmdUI->m_pOther;

    if(pE->GetDlgCtrlID() == IDC_PUBLISHER_NAME)
    {
        ostringstream os;

        if(m_pub)
        {
            // If publishing then use the publisher name
            os << m_pub->getGuid();
        }
        else
        {
            auto it = m_publishers.begin();

            if(it != m_publishers.end())
            {
                // If subscribing then use the first publisher
                os << *it;
            }
            else
            {
                os << "No publishers discovered.";
            }
        }

        pE->SetWindowText(CA2T(os.str().c_str()));
    }
    else
    {
        ts = counter;
        pE->SetWindowText(ts.Format(_T("%H:%M:%S")));
    }

}

void CeProsimaClockApp::OnUpdateEndpointKind(CCmdUI *pCmdUI)
{
    CButton * pB = (CButton*)pCmdUI->m_pOther;

    // Checked means publishing
    CString caption, actual;

    if(m_pub)
    {
        pB->SetCheck(BST_CHECKED);
        caption = _T("Publishing...");
    }
    else
    {
        pB->SetCheck(BST_UNCHECKED);
        caption = _T("Subscribing...");
    }

    pB->GetWindowText(actual);
    if(caption != actual)
        pB->SetWindowText(caption);
}

void CeProsimaClockApp::OnUpdateTimerState(CCmdUI *pCmdUI)
{
    CButton * pB = (CButton*)pCmdUI->m_pOther;

    // if subscribing disable this button
    if(m_sub)
    {
        pB->EnableWindow(FALSE);
        pB->SetWindowText(_T(""));
        return;
    }

    // if publishing check the state to verify if a timer is working
    pB->EnableWindow(TRUE);
    int estado = pB->GetCheck();
    CString caption, actual;

    if(estado == BST_CHECKED)
    {	// Parar
        caption = _T("Stop");
    }
    else
    {	// Arrancar
        caption = _T("Start");
    }

    pB->GetWindowText(actual);
    if(caption != actual)
        pB->SetWindowText(caption);

}

void CeProsimaClockApp::OnBnClickedEndpointType()
{
    // Flip-flop between Subscribing and Publishing
    if(m_pub)
    {
        createSubscriber();
    }
    else
    {
        createPublisher();
    }

    // Update controls and channel commands through this
    GetMainWnd()->UpdateDialogControls(this, TRUE);
}

// Update the publishers set
void CeProsimaClockApp::onPublisherDiscovery(eprosima::fastrtps::Participant* participant, eprosima::fastrtps::rtps::WriterDiscoveryInfo&& info) /*override*/
{
    using STATUS = eprosima::fastrtps::rtps::WriterDiscoveryInfo::DISCOVERY_STATUS;

    switch(info.status)
    {
    case STATUS::DISCOVERED_WRITER:
        m_publishers.insert(info.info.guid());
        break;
    case STATUS::REMOVED_WRITER:
        m_publishers.erase(info.info.guid());
        break;
    }

    // MFC mechanism is serialized through message queues. Avoid taking any UI action
    // from callbacks
    GetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_LISTENER, 0));
}

// Update the dialog displays

void CeProsimaClockApp::onNewDataMessage(eprosima::fastrtps::Subscriber* sub) /*override*/
{
    eProsimaClock val;
    eprosima::fastrtps::SampleInfo_t si;

    // Take the count value from the sub 
    if(sub->takeNextData((void*)&val, &si))
    {
        auto it = m_publishers.begin();
        if(it != m_publishers.end())
        {
            // only rely information from the first publisher of the map
            if(si.sample_identity.writer_guid() == *it)
            {
                counter = static_cast<__time64_t>(val.time());

                // MFC mechanism is serialized through message queues. Avoid taking any UI action
                // from callbacks
                GetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_LISTENER,0));
            }
        }
    }
}

// Create dialog endpoints

void CeProsimaClockApp::createPublisher()
{
    using namespace eprosima::fastrtps;
    using namespace eprosima::fastrtps::rtps;

    // Use xml config file if exists
    PublisherAttributes Wparam;
    Domain::getDefaultPublisherAttributes(Wparam);

    // Specify the topic
    Wparam.topic.topicKind = NO_KEY;
    Wparam.topic.topicDataType = "eProsimaClock";
    Wparam.topic.topicName = "eProsimaTimer";

    m_sub.reset();
    m_pub.reset(Domain::createPublisher(m_part.get(), Wparam), &Domain::removePublisher);

    // restart counting
    reference = CTime::GetCurrentTime();
}

void CeProsimaClockApp::createSubscriber()
{
    using namespace eprosima::fastrtps;
    using namespace eprosima::fastrtps::rtps;

    // Use xml config file if exists
    SubscriberAttributes Rparam;
    Domain::getDefaultSubscriberAttributes(Rparam);

    // Specify the topic
    Rparam.topic.topicKind = NO_KEY;
    Rparam.topic.topicDataType = "eProsimaClock";
    Rparam.topic.topicName = "eProsimaTimer";

    m_pub.reset();
    m_sub.reset(Domain::createSubscriber(m_part.get(), Rparam, this),&Domain::removeSubscriber);

}

void CeProsimaClockApp::OnTimer(UINT_PTR nIDEvent)
{
    // Only makes sense if publishing
    if(m_pub)
    {
        counter = CTime::GetCurrentTime()- reference;

        eProsimaClock val;
        val.time(static_cast<int64_t>(counter.GetTimeSpan()));

        // publish the time
        m_pub->write((void*)&val);

        // Update the dialog controls, delegate the callbacks in the application object
        // if not handled by the dialog itself
        GetMainWnd()->UpdateDialogControls(this, TRUE);
    }
}