#include <shlobj.h>
#include <cstdio>
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>
#include <tchar.h>
#include <string>
#include <Shlobj.h>
#include <dbghelp.h>
#include <Tlhelp32.h>

// WXWINDGETS-------
#include <wx/app.h>
#include <wx/wxprec.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/utils.h> 
#include <wx/dirdlg.h> 
#include <wx/filedlg.h>
#include <wx/textdlg.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>

using namespace std;

void debug_pause()
{
	cout<<"Press any key to continue..."<<endl;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
	exit(1);
}

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxPoint &pos, const wxSize &size);
	wxPanel *panel;
	wxButton *FileL;
	wxButton *Swap_mode;
	wxTextCtrl *Input1;
	wxTextCtrl *Input2;
	
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void LoadFile(wxCommandEvent& event);
	void Swap(wxCommandEvent& event);
	void Dialog(wxCommandEvent& event);
	void Dialog2(wxCommandEvent& event);
	void MainFrame::OnPaint(wxPaintEvent& event);
	DECLARE_EVENT_TABLE()
};

enum
{
    ID_Hello = 1
};

enum
{
    BUTTON_File = wxID_HIGHEST + 1 // declares an id which will be used to call our button
};

enum
{
	BUTTON_Swap = wxID_HIGHEST + 2
};

enum 
{
	INPUT1 = wxID_HIGHEST + 3
};

enum 
{
	INPUT2 = wxID_HIGHEST + 4
};

wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MainFrame *frame = new MainFrame(wxPoint(1, 1), wxSize(1000,750));
	frame->SetWindowStyle(frame->GetWindowStyle() | wxFULL_REPAINT_ON_RESIZE);
    frame->Show(true);
	SetTopWindow(frame);
    return true;
}

BEGIN_EVENT_TABLE ( MainFrame, wxFrame )
EVT_BUTTON ( BUTTON_File, MainFrame::LoadFile )
EVT_BUTTON ( BUTTON_Swap, MainFrame::Swap )
EVT_TEXT_ENTER(INPUT1, MainFrame::Dialog)
EVT_TEXT_ENTER(INPUT2, MainFrame::Dialog2)
//EVT_INIT_DIALOG(MainFrame::Dialog)
END_EVENT_TABLE()
	
MainFrame::MainFrame(const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, "YOU HAVE TO LEARN ENGLISH", pos, size)
{
	panel = new wxPanel(this, wxID_ANY);
	Bind(wxEVT_PAINT, &MainFrame::OnPaint, this);
	
	FileL = new wxButton(panel, BUTTON_File, _T("Load Txt File"), wxPoint(1, 1), wxSize(100,50), 0);
	Swap_mode = new wxButton(panel, BUTTON_Swap, _T("Swap Modes"), wxPoint(885, 0), wxSize(100,50), 0);
	Input1 = new wxTextCtrl(panel, INPUT1, _T("Default Text"), wxPoint(350, 100), wxSize(300,200));
	Input2 = new wxTextCtrl(panel, INPUT2, _T("Default Text"), wxPoint(350, 400), wxSize(300,200));
	//wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	//wxStdDialogButtonSizer* buttonSizer = new wxStdDialogButtonSizer;
	//Input = new wxDialog(panel, 3, _T("diag"),  wxDefaultPosition);
	//buttonSizer->AddButton(HelloWorld);
	
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText("Powered by wxWidgets c++!");
    Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

// wxString::FromUTF8() 

void MainFrame::OnPaint(wxPaintEvent& event)
{
	wxSize sz = GetClientSize();
	
	//if this than other objects will be not respectfully. 
	if(Swap_mode != nullptr)
	{
		//FileL->~wxButton();
		Swap_mode->~wxButton();
		Input1->~wxTextCtrl();
		Input2->~wxTextCtrl();
		//delete FileL;
		delete Swap_mode;
		delete Input1;
		delete Input2;
		//FileL = nullptr;
		Swap_mode = nullptr;
		Input1 = nullptr;
		Input2 = nullptr;
	}
	
	std::cout<<"X = "<<sz.x<<std::endl;
	std::cout<<"Y = "<<sz.y<<std::endl;
	
	int new_box_x = sz.x / 3;
	int new_box_y = sz.y / 6;
	
	//FileL = new wxButton(panel, BUTTON_File, _T("Load Txt File"), wxPoint(1, 1), wxSize(100,50), 0);
	Swap_mode = new wxButton(panel, BUTTON_Swap, _T("Swap Modes"), wxPoint(sz.x-100, 0), wxSize(100,50), 0);
	Input1 = new wxTextCtrl(panel, INPUT1, _T("Default Text"), wxPoint((sz.x/2)-new_box_x/2, (sz.y/2)+new_box_y), wxSize(new_box_x,new_box_y*2));
	Input2 = new wxTextCtrl(panel, INPUT2, _T("Default Text"), wxPoint((sz.x/2)-new_box_x/2, (sz.y/2)-new_box_y*3), wxSize(new_box_x,new_box_y*2));	
	
/* 	wxButton FileL(panel, BUTTON_File, _T("Load Txt File"), wxPoint(1, 1), wxSize(100,50), 0);
	wxButton Swap_mode(panel, BUTTON_Swap, _T("Swap Modes"), wxPoint(885, 0), wxSize(100,50), 0);
	wxTextCtrl Input1(panel, INPUT1, _T("Default Text"), wxPoint(350, 100), wxSize(300,200));
	wxTextCtrl Input2(panel, INPUT2, _T("Default Text"), wxPoint(350, 400), wxSize(300,200)); */	
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("LOL Why are you even here?! Go learn words!",
                 "LOL", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}

void MainFrame::LoadFile(wxCommandEvent& event)
{
	wxString filename = wxFileSelector("Choose a file to open");
    if(!filename.empty())
    {
		std::cout<<"File Loaded Into Memory ! \n";
    }	
}

void MainFrame::Swap(wxCommandEvent& event)
{
	
}

void MainFrame::Dialog(wxCommandEvent& event)
{
	
}

void MainFrame::Dialog2(wxCommandEvent& event)
{
	
}