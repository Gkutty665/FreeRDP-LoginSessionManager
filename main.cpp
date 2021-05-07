#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
//#include <wx/msw/wx.rc>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/icon.h>
#include <fstream>
#include <wx/bitmap.h>
#include <wx/statbmp.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/filefn.h> 
#include <wx/msgdlg.h>
#include <wx/file.h>
#include <vector>
#include <string>

#endif




const long wxID_CBOX = wxNewId();
const long wxID_SIGNIN = wxNewId();
const long wxID_STATUS = wxNewId();
using namespace std;
class SessionApp : public wxApp
{
public:
    virtual bool OnInit();
};
class SessionManagerFrame : public wxFrame
{
public:
    SessionManagerFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnToggleShowPassword(wxCommandEvent& event);
    void OnSignIn(wxCommandEvent& event);
    void OnToggleFullScreen(wxCommandEvent& WXUNUSED(event));
    std::vector<std::string> split(const char *str, const char c);
    void replace(string &str, string old_str, string new_str);
    wxTextCtrl* username;
    wxTextCtrl* password;
    wxCheckBox* check_box;
    wxButton* sign_in;
    wxPanel* panel;
    wxBoxSizer* sizer1;
    wxStaticText* login_status;
    wxStaticText* login_title;
    wxStatusBar* status_bar;
    wxStaticBitmap* backgroundImage;
    wxStaticBitmap* userIconImg;
    wxStaticBitmap* keyIconImg;
    wxFont font;
    wxDECLARE_EVENT_TABLE();
};

enum
{
    // menu items
    exitID = wxID_EXIT,
    aboutID = wxID_ABOUT,
	wxFSRC  //id for fullscreen
};


wxBEGIN_EVENT_TABLE(SessionManagerFrame, wxFrame)

    EVT_MENU(exitID, SessionManagerFrame::OnExit)
    EVT_MENU(aboutID, SessionManagerFrame::OnAbout)
    EVT_CHECKBOX(wxID_CBOX, SessionManagerFrame::OnToggleShowPassword)
    EVT_BUTTON(wxID_SIGNIN, SessionManagerFrame::OnSignIn)
    EVT_MENU(wxFSRC, SessionManagerFrame::OnToggleFullScreen)
    

    wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(SessionApp);

bool SessionApp::OnInit()
{
    wxInitAllImageHandlers();
    SessionManagerFrame *frame = new SessionManagerFrame("RDP Session Manager Login", wxPoint(50, 50), wxSize(720,500));
    frame->Show(true);
    // we can modify this portion to get our custom size of tab or fullscreen for start
//    frame->Maximize(true);
//    frame->ShowFullScreen(true);
   frame->SetSize(wxSize(900,600));
   frame->CenterOnScreen(true);
    SetTopWindow(frame);

    return true;
}

SessionManagerFrame::SessionManagerFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{

    wxString bg_path = wxT("./bg.jpg");
    bool bg_found=false;
    if(wxFileExists(bg_path)){
        wxImage img(bg_path);
        if (img.IsOk()) {
            backgroundImage = new wxStaticBitmap(this, -1, wxBitmap(img), wxDefaultPosition, wxDefaultSize, 0);
            bg_found = true;
        }
    }
    if(! bg_found){
        this->SetBackgroundColour(wxColor(195, 195, 213));
    }

	panel = new wxPanel(this,-1, wxDefaultPosition, wxSize(320,350));
	panel->SetBackgroundColour(wxColor(250, 250, 250));

    wxString icon_path = wxT("./rabbit.png");

    if(wxFileExists(icon_path))
    {
    	wxIcon FrameIcon;
    	FrameIcon.CopyFromBitmap(wxBitmap(wxImage(icon_path)));
    	SetIcon(FrameIcon);
    }

    sizer1 = new wxBoxSizer(wxVERTICAL);

	wxPanel *top_margin = new wxPanel(panel, -1, wxDefaultPosition, wxSize(300,-1));
    wxPanel *bottom_margin = new wxPanel(panel, -1, wxDefaultPosition, wxSize(300,-1));

    sizer1->Add(top_margin, 1);
    sizer1->Add(panel, 0, wxALIGN_CENTER);
	sizer1->Add(bottom_margin, 2);

    this->SetSizerAndFit(sizer1);
    this->SetSize(wxSize(720,500));

    wxString uname = wxT("username");
    wxString pword = wxT("password");
    wxString label = wxT("Sign In");
    wxString show_password = wxT("show password");

    login_title = new wxStaticText(panel, wxID_STATUS, wxString("User LogIn"), wxPoint(10, 10), wxSize(300, 35), wxALIGN_CENTER);
    wxFont LoginTitleFont(18,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("corbel"),wxFONTENCODING_DEFAULT);

    font = wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

    login_title->SetFont(LoginTitleFont);
    login_title->SetBackgroundColour(wxColor(180, 180, 180, 100));
    login_title->SetForegroundColour(wxColor(1,1,1));

    wxString user_path = wxT("./user.png");
    wxString key_path = wxT("./key.png");
    if(wxFileExists(user_path) && wxFileExists(key_path) ){
        wxImage usericon(user_path);
        wxImage keyicon(key_path);

        if (usericon.IsOk() && keyicon.IsOk()) {

            userIconImg =  new wxStaticBitmap(panel, -1, wxBitmap(usericon), wxPoint(20,80), wxSize(30,30), 0);
            keyIconImg = new wxStaticBitmap(panel, -1, wxBitmap(keyicon), wxPoint(20,125), wxSize(30,30), 0);

        }
    }

    username = new wxTextCtrl(panel, -1, uname, wxPoint(55, 80), wxSize(235,30));
    password = new wxTextCtrl(panel, -1, pword, wxPoint(55, 125), wxSize(235,30), wxTE_PASSWORD);
    username->SetFont(font);
    password->SetFont(font);
    check_box = new wxCheckBox(panel, wxID_CBOX, show_password, wxPoint(30, 190), wxSize(250,30));
    check_box->SetFont(font);

    sign_in = new wxButton(panel, wxID_SIGNIN, label, wxPoint(30, 230), wxSize(260,40));
    sign_in->SetFont(font);
    login_status = new wxStaticText(panel, wxID_STATUS, wxEmptyString, wxPoint(5, 320), wxSize(296, 17), wxALIGN_LEFT, _T("wxID_STATUS"));
    login_status->SetFont(font);
    login_status->Show(true);
    
    sign_in->SetBackgroundColour(wxColor(102,155,145));
    Center();
    panel->AcceptsFocus();
    panel->SetWindowStyle(wxBORDER_SUNKEN);


    // menu bar
    wxMenu *fileMenu = new wxMenu;
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(aboutID, _T("&About...\tF1"), _T("Show about dialog"));

    fileMenu->Append(wxFSRC, _T("Toggle Fullscreen\tF11"), _T("Toggle fullscreen display"));
    fileMenu->Append(exitID, _T("E&xit\tAlt-X"), _T("Quit this program"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, _T("&File"));
    menuBar->Append(helpMenu, _T("&Help"));
    SetMenuBar(menuBar);
    menuBar->Show(false);
 

    // status bar
    status_bar = new wxStatusBar(this, -1);
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    status_bar->SetFieldsCount(1, __wxStatusBarWidths_1);
    status_bar->SetStatusStyles(1, __wxStatusBarStyles_1);
    SetStatusBar(status_bar);
    status_bar->SetStatusText(wxT("Welcome to Session Manager"));
    status_bar->SetBackgroundColour(wxColor("white"));

}

void SessionManagerFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}

void SessionManagerFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("Session Manager Login Application",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

void SessionManagerFrame::OnToggleFullScreen(wxCommandEvent& WXUNUSED(event))
{
	ShowFullScreen(!IsFullScreen(),wxFULLSCREEN_NOCAPTION);
}

void SessionManagerFrame::OnToggleShowPassword(wxCommandEvent& event)
{
    Freeze();
    wxString sPass = password->GetValue();
    auto psize = password->GetSize();
    auto pcord = password->GetPosition();
    long lFrom, lTo;
    password->GetSelection(&lFrom, &lTo);
    int iStyle = (check_box->IsChecked() ? 0 : wxTE_PASSWORD);
    auto par = password->GetParent();
    delete password;

    password = new wxTextCtrl(par, wxID_ANY, sPass, pcord, psize, iStyle);
    password->SetFocus();
    password->SetSelection(lFrom, lTo);
    password->SetFont(font);
    Thaw();
}

void SessionManagerFrame::OnSignIn(wxCommandEvent& event)
{

    string username_ = username->GetValue().ToStdString();
    string password_ = password->GetValue().ToStdString();

    int result = -1;
    wxString msg;
    // key for ssh decrypition
    string key = "password";
    bool flag=true;
    wxString config_file = "C:/Program Files/SessionManager/sessionmanager.conf";
    result = system("ping 8.8.8.8");
    if(result==512){
        msg = wxT("Error: no Internet connection");
        login_status->SetLabel(msg);
        login_status->SetForegroundColour(wxColour("red"));
        status_bar->SetStatusText(msg);
        status_bar->SetForegroundColour(wxColour("red"));
        return;
    }
    else if (flag && wxFileExists(config_file)) {
        string outfile = "C:/Program Files/SessionManager/sessionmanager";
        string infile = config_file.ToStdString();
        string decryipt_cmd = "openssl aes-256-cbc -d -salt -pbkdf2 -iter 10000 -k " + key + "-in " + infile + " -out " + outfile;

        int flag = system(decryipt_cmd.c_str());

        if (flag && !wxFileExists(outfile)) {
            // unable to dycirpt file : case 5
            result = 5;
        }
        else{
            wxFile file(wxString(outfile.c_str()));
            wxString raw_commands;
            file.ReadAll(&raw_commands);
            file.Close();
            remove(outfile.c_str());    // removing decrypted file.

            string std_string_raw_commands = raw_commands.ToStdString();
            string validation, startsession;

            vector<string> splited_commands = split(std_string_raw_commands.c_str(),'\n');
            if(splited_commands.size()<2){
                flag = false;
                result = 7;
            }
            else{
                validation = splited_commands[0];
                startsession = splited_commands[1];
            }

            //COMMAND - VALIDATE - CREDENTIALS: �<CONTENTS OF THE COMMAND>�
            //COMMAND - START - SESSION : �<CONENTS OF THE COMMAND>�

            size_t pos = validation.find(':');

            if (pos != string::npos) {
                // take substring from position of colone (;) plus two ( one is for ':' and 2nd is for space ' ').
                validation = validation.substr(pos + 2);
            }
            else {
                // invalid config file...
                result = 7;
                flag = false;
            }
            pos = startsession.find(':');
            if (pos != string::npos) {
                startsession = startsession.substr(pos + 2);
            }
            else {
                // invalid config file...
                result = 7;
                flag = false;
            }
            
            replace(validation,"<password>", password_);
            replace(validation,"<user>", username_);

            //if startsession needed credentials
            replace(startsession, "<password>", password_);
            replace(startsession, "<user>", username_);
            
            result = system(validation.c_str());

            if(flag && result==0){
                // invalid commands
                flag = false;
            }
            result = system(startsession.c_str());
            if(flag && result == 0){
                // unable to make session
                flag = false;
            }
            // we can put some more condition according to result code output
            
        }
        // need modification according to command and there responses 

    }
    else{
        flag = false;
        result = 6;
    }


    // need modification
    switch (result) {
    case 1:
        msg = wxT("Error: username and/or password is incorrect");
        login_status->SetLabel(msg);
        login_status->SetForegroundColour(wxColour("red"));
        status_bar->SetStatusText(msg);
        status_bar->SetForegroundColour(wxColour("red"));
        break;
    case 2:
        msg = wxT("Error: could not communicate with server");
        login_status->SetLabel(msg);
        login_status->SetForegroundColour(wxColour("red"));
        status_bar->SetStatusText(msg);
        status_bar->SetForegroundColour(wxColour("red"));
        break;
    case 3:
        break;
    case 4:
        msg = wxT("validation success");
        login_status->SetLabel(msg);
        login_status->SetForegroundColour(wxColour("green"));
        status_bar->SetStatusText(msg);
        status_bar->SetForegroundColour(wxColour("grey"));
        break;
    case 5:
        msg = wxT("sessionmanager.conf decryption error");
        login_status->SetLabel(msg);
        login_status->SetForegroundColour(wxColour("red"));
        status_bar->SetStatusText(msg);
        status_bar->SetForegroundColour(wxColour("red"));
        break;

    case 6:
        msg = wxT("unable to find sessionmanager.conf file");
        login_status->SetLabel(msg);
        login_status->SetForegroundColour(wxColour("red"));
        status_bar->SetStatusText(msg);
        status_bar->SetForegroundColour(wxColour("red"));
        break;
    case 7:
        msg = wxT("invalid command in sessionmanager.conf file");
        login_status->SetLabel(msg);
        login_status->SetForegroundColour(wxColour("red"));
        status_bar->SetStatusText(msg);
        status_bar->SetForegroundColour(wxColour("red"));
        break;
    }

}

std::vector<std::string> SessionManagerFrame::split(const char *str, const char c)
{
    vector<std::string> result;
    do
    {
        const char *begin = str;

        while(*str != c && *str)
            str++;

        result.push_back(std::string(begin, str));
    } while (0 != *str++);

    return result;
}

void SessionManagerFrame::replace(string &str, string old_str, string new_str){
    size_t pos = str.find(old_str);
    if(pos==string::npos)
        return;
    str.replace(pos, old_str.size(), new_str);
}