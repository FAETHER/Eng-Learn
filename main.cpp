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
#include <fstream>
#include <Tlhelp32.h>

// WXWINDGETS-------
#include <wx/app.h>
#include <wx/wxprec.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/button.h>
#include <wx/wx.h>
#include <wx/utils.h> 
#include <wx/dirdlg.h> 
#include <wx/filedlg.h>
#include <wx/textdlg.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/font.h>
#include <wx/dc.h>
#include <wx/numdlg.h>
#include <wx/fontdlg.h>  

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

class BasicDrawPane : public wxPanel
{
    
public:
    BasicDrawPane(wxFrame* parent);
    
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    
    void render(wxDC& dc);
      
    DECLARE_EVENT_TABLE()
};

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxPoint &pos, const wxSize &size);
	
	BasicDrawPane *panel;
	wxButton *FileL;
	wxRichTextCtrl *FileOpened;
	wxButton *Swap_mode;
	wxButton *Random_btn;
	bool swaped = false;
	wxRichTextCtrl *Input1;
	wxRichTextCtrl *Input2;
	//Fonts
	wxButton *Show_all_btn;
	wxButton *Font_format_btn;
	wxFont *font;
	int font_size = 24;
	
	wxString filename;
	std::string h_part;
	std::string l_part;	
	std::vector<int> prev_rand;
	
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void LoadFile(wxCommandEvent& event);
	void Swap(wxCommandEvent& event);
	void Show_all_boxes(wxCommandEvent& event);
	void Change_Font_Format(wxCommandEvent& event);
	void OnRandom(wxCommandEvent& event);
	void Dialog(wxCommandEvent& event);
	void Dialog2(wxCommandEvent& event);
	void OnResize(wxPaintEvent& event);
	void Update();
	void Parse_Random_Line();
	
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
	BUTTON_show_all = wxID_HIGHEST + 3
};

enum
{
	BUTTON_font_format = wxID_HIGHEST + 4
};

enum
{
	BUTTON_rand = wxID_HIGHEST + 5
};

enum 
{
	INPUT1 = wxID_HIGHEST + 6
};

enum 
{
	INPUT2 = wxID_HIGHEST + 7
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

BEGIN_EVENT_TABLE ( BasicDrawPane, wxPanel )
EVT_PAINT(BasicDrawPane::paintEvent)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE ( MainFrame, wxFrame )
EVT_BUTTON ( BUTTON_File, MainFrame::LoadFile )
EVT_BUTTON ( BUTTON_Swap, MainFrame::Swap )
EVT_BUTTON ( BUTTON_show_all, MainFrame::Show_all_boxes )
EVT_BUTTON ( BUTTON_font_format, MainFrame::Change_Font_Format )
EVT_BUTTON ( BUTTON_rand, MainFrame::OnRandom )
EVT_TEXT_ENTER(INPUT1, MainFrame::Dialog)
EVT_TEXT_ENTER(INPUT2, MainFrame::Dialog2)
END_EVENT_TABLE()
	
MainFrame::MainFrame(const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, "YOU HAVE TO LEARN ENGLISH", pos, size)
{
	panel = new BasicDrawPane(this);
	Bind(wxEVT_PAINT, &MainFrame::OnResize, this);
	
/* 	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(panel, 1, wxEXPAND);
    this->SetSizer(sizer);
    this->SetAutoLayout(true);	 */
	
	FileL = new wxButton(panel, BUTTON_File, _T("Load Txt File"), wxPoint(1, 1), wxSize(100,50), 0);
	FileOpened = new wxRichTextCtrl(panel, -1, _T("File: "), wxPoint(1, 50), wxSize(200,50));
	Swap_mode = new wxButton(panel, BUTTON_Swap, _T("Swap Modes"), wxPoint(100, 1), wxSize(100,50), 0); //885, 0
	Show_all_btn = new wxButton(panel, BUTTON_show_all, _T("Show all"), wxPoint(1, 100), wxSize(100,50), 0);
	Font_format_btn = new wxButton(panel, BUTTON_font_format, _T("Font Format"), wxPoint(100, 100), wxSize(100,50), 0);
	Random_btn = new wxButton(panel, BUTTON_rand, _T("Roll Random!"), wxPoint(1, 150), wxSize(200,100), 0);
/* 	Input1 = new wxRichTextCtrl(panel, INPUT1, _T("Default Text"), wxPoint(350, 100), wxSize(300,200));
	Input2 = new wxRichTextCtrl(panel, INPUT2, _T("Default Text"), wxPoint(350, 400), wxSize(300,200)); */
	font = new wxFont(font_size, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	
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

BasicDrawPane::BasicDrawPane(wxFrame* parent) :
wxPanel(parent)
{
}

void BasicDrawPane::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void BasicDrawPane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void BasicDrawPane::render(wxDC&  dc)
{
    dc.DrawText(wxT("The txt file has to be formatted to be in UTF-8 encoding"), 1, 300); 
	dc.DrawText(wxT("Row example: CAT # (any UTF-8 string)"), 1, 320); 
}

void MainFrame::OnResize(wxPaintEvent& event)
{
	Update();
}

void MainFrame::Update()
{
	wxSize sz = GetClientSize();
	
	//if this than other objects will be not respectfully. 
	static bool once = true;
	if(!once)
	{
		Input1->~wxRichTextCtrl();
		Input2->~wxRichTextCtrl();
		delete Input1;
		delete Input2;
		Input1 = nullptr;
		Input2 = nullptr;
	}
	once = false;
	
	std::cout<<"X = "<<sz.x<<std::endl;
	std::cout<<"Y = "<<sz.y<<std::endl;
	
	int new_box_x = sz.x / 3;
	int new_box_y = sz.y / 6;
	
	//Swap_mode = new wxButton(panel, BUTTON_Swap, _T("Swap Modes"), wxPoint(sz.x-100, 0), wxSize(100,50), 0);
	Input1 = new wxRichTextCtrl(panel, INPUT1, _T("Default Text"), wxPoint((sz.x/2)-new_box_x/2, (sz.y/2)+new_box_y), wxSize(new_box_x,new_box_y*2));
	Input2 = new wxRichTextCtrl(panel, INPUT2, _T("Default Text"), wxPoint((sz.x/2)-new_box_x/2, (sz.y/2)-new_box_y*3), wxSize(new_box_x,new_box_y*2));	
/* 	Input1->BeginFontSize(font_size);
	Input2->BeginFontSize(font_size); */
	Input1->SetFont(*font);
	Input2->SetFont(*font);

	//Input1->WriteText(wxT("by Julian Smart "));
	
	
/* 	wxButton FileL(panel, BUTTON_File, _T("Load Txt File"), wxPoint(1, 1), wxSize(100,50), 0);
	wxButton Swap_mode(panel, BUTTON_Swap, _T("Swap Modes"), wxPoint(885, 0), wxSize(100,50), 0);
	wxRichTextCtrl Input1(panel, INPUT1, _T("Default Text"), wxPoint(350, 100), wxSize(300,200));
	wxRichTextCtrl Input2(panel, INPUT2, _T("Default Text"), wxPoint(350, 400), wxSize(300,200)); */		
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

int get_line_count(std::fstream &fs)
{
	fs.unsetf(ios_base::skipws);
	return count(istream_iterator<char>(fs),istream_iterator<char>(),'\n');
}

void MainFrame::Parse_Random_Line()
{
	if(filename.empty())
	{
		return;
	}
	
	std::fstream fs;
	fs.open(filename);
	if(!fs.is_open())
	{
		std::cout<<"Failed to open file!"<<filename<<std::endl;
		debug_pause();
	}
	
	std::string line;
	int rand_index = 1;
	int line_index = 0;
	int line_count = get_line_count(fs);
	fs.close();
	fs.open(filename, std::fstream::in | std::fstream::out);
	
	if(line_count > 1)
	{
		if(prev_rand.size() == line_count)
		{
			std::cout<<"Info: Run out of indexes. Restarting randomizer."<<std::endl;
			prev_rand.clear();
		}
		generate_new_rand:
		rand_index = rand() % line_count + 1;
		for(int i = 0; i<prev_rand.size(); i++)
		{
			if(prev_rand[i] == rand_index)
			{
				goto generate_new_rand;
			}
		}
	}
	
	std::cout<<"Random Index: "<<rand_index<<std::endl;
	prev_rand.push_back(rand_index);
	while (getline(fs,line))
    {
	//	cout << line << '\n';
		line_index++;
	//	std::cout<<line_index<<std::endl;
		size_t pos = line.find("#");
		if(pos != std::string::npos)
		{
			if(rand_index == line_index)
			{
				h_part = line.substr(pos+1);
				l_part = line.substr(0, pos);
				break;
			}
			else{continue;}
		}
		else
		{
			std::cout<<"Failed to find a ` # ` separator for line: ";
			std::cout<<line<<std::endl;
			std::cout<<"Line Number = "<<line_index<<std::endl;
			debug_pause();
		}
    }
	
	if(h_part.empty() || l_part.empty())
	{
		std::cout<<"Failed to find the target line with random index:  "<<rand_index<<std::endl;
		Parse_Random_Line();
	}	
//	std::cout<<wxString::FromUTF8(h_part)<<std::endl;
//	std::cout<<wxString::FromUTF8(l_part)<<std::endl;
	
	Input1->Clear();
	Input2->Clear();
	if(swaped)
	{
		Input1->WriteText(wxString::FromUTF8(h_part));
	}
	else
	{
		Input1->WriteText(wxString::FromUTF8(l_part));	
	}
	
}

void MainFrame::LoadFile(wxCommandEvent& event)
{
	filename = wxFileSelector("Choose a file to open");
    if(!filename.empty())
    {
		std::cout<<"File: "<<filename<<std::endl;
		FileOpened->Clear();
		FileOpened->AppendText(filename);
    }	
	Parse_Random_Line();
}

void MainFrame::Swap(wxCommandEvent& event)
{
	if(!swaped)
	{
		swaped = true;
	}
	else
	{
		swaped = false;
	}
}

void MainFrame::Show_all_boxes(wxCommandEvent& event)
{
	Input1->Clear();
	Input2->Clear();	
	if(swaped)
	{
		Input1->WriteText(wxString::FromUTF8(h_part));
		Input2->WriteText(wxString::FromUTF8(l_part));
	}
	else
	{
		Input1->WriteText(wxString::FromUTF8(l_part));
		Input2->WriteText(wxString::FromUTF8(h_part));		
	}
}

void MainFrame::Change_Font_Format(wxCommandEvent& event)
{
	*font = wxGetFontFromUser(panel, *font, "Select Font");
	Update();
}

void MainFrame::OnRandom(wxCommandEvent& event)
{
	Parse_Random_Line();
}

void MainFrame::Dialog(wxCommandEvent& event)
{
	
}

void MainFrame::Dialog2(wxCommandEvent& event)
{
	
}