/* wXChip
   (c) 2016
*/

#if defined(__APPLE__) || defined(__linux__)
#include <dirent.h>
#elif defined(_WIN32)
#include <wXChip/dirent.h>
#endif

#include <sstream>
#include <thread>

#include <XChip/Media/SDLMedia/SdlRender.h>
#include <XChip/Media/SDLMedia/SdlInput.h>
#include <XChip/Media/SDLMedia/SdlSound.h>
#include <XChip/Utility/Log.h>
#include <wXChip/MainWindow.h>
#include <wXChip/SaveList.h>


enum { ID_Chip = 1, ID_LISTBOX = 2, ID_STARTROM = 3, ID_LOADROM = 4, ID_TEXT = 5, ID_EMUSET, ID_TIMER1};

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(ID_Chip,   MainWindow::OnChip)
EVT_MENU(wxID_EXIT,  MainWindow::OnExit)
EVT_MENU(ID_EMUSET,  MainWindow::LoadSettings)
EVT_CLOSE(MainWindow::OnWindowClose)
EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
EVT_MOTION(MainWindow::OnMouseOver)
EVT_BUTTON(ID_STARTROM, MainWindow::OnStartRom)
EVT_BUTTON(ID_LOADROM, MainWindow::OnChip)
EVT_BUTTON(ID_EMUSET, MainWindow::LoadSettings)
EVT_TIMER(ID_TIMER1, MainWindow::OnTimer)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(wXChip);


bool wXChip::OnInit()
{
	using xchip::utility::make_unique;
	
	try {
		std::string fps_val, cpu_freq;
		const std::string file = getDirectory(fps_val, cpu_freq);
		auto frame = make_unique<MainWindow>( "wXChip ", wxPoint(50, 50), wxSize(800, 600) );
		
		if(file != "nolist")
			frame->LoadList(file, fps_val, cpu_freq);
	
		frame->Show( true );
		frame.release();
	} catch(std::exception &e) {
		xchip::utility::LOGerr(e.what());
		return false;
	}
	return true;
}



MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX), _timer(this, ID_TIMER1)
{
	using xchip::utility::make_unique;
	running = false, closing = false;
	auto menuFile = make_unique<wxMenu>();
	menuFile->Append(ID_Chip, "&Load Roms...\tCtrl-L", 
                         "Load Roms");
    
	menuFile->Append(ID_EMUSET, "&Settings\tCtrl-S", "Settings");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	auto menuHelp = make_unique<wxMenu>();
	menuHelp->Append(wxID_ABOUT);

	auto menuBar = make_unique<wxMenuBar>();
	menuBar->Append( menuFile.release(), "&File" );
	menuBar->Append( menuHelp.release(), "&Help" );
	SetMenuBar( menuBar.release() );

	CreateStatusBar();
	SetStatusText( "Welcome to wXChip" );
	
	CreateControls();

	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}

void MainWindow::CreateControls()
{
	using xchip::utility::make_unique;
	wxArrayString strings;
	_panel = make_unique<wxPanel>(this, wxID_ANY);
	_text = make_unique<wxStaticText>(_panel.get(), ID_TEXT, _T("Chip8 Roms"), wxPoint(10,10), wxSize(100,25));
	_listBox = make_unique<wxListBox>(_panel.get(), ID_LISTBOX, wxPoint(10, 35), wxSize(620, 360), strings, wxLB_SINGLE);
	_listBox->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(MainWindow::OnLDown), NULL, this);
	_startRom = make_unique<wxButton>(_panel.get(), ID_STARTROM, _T("Start Rom"), wxPoint(10, 400), wxSize(100,25));
	_settings = make_unique<wxButton>(_panel.get(), ID_LOADROM, _T("Load Roms"), wxPoint(120, 400), wxSize(100,25));
	_emulatorSettings = make_unique<wxButton>(_panel.get(), ID_EMUSET, _T("Settings"), wxPoint(230, 400), wxSize(100,25));
	_settingsWin = make_unique<SettingsWindow>("wXChip - Settings", wxPoint(150, 150), wxSize(430, 220));
}

void MainWindow::CreateEmulator()
{
	using xchip::SdlRender;
	using xchip::SdlInput;
	using xchip::SdlSound;
	using xchip::utility::make_unique;

	if(_emulator) 
	{
		_emulator->Stop();
		_emulator.reset();
	}

	_emulator = make_unique<EmulatorThread>();
	_emulator->GetEmulator().Initialize(make_unique<xchip::SdlRender>(),
			                            make_unique<SdlInput>(),
			                            make_unique<SdlSound>());
}


void MainWindow::OnLDown(wxMouseEvent& event)
{
	auto m_lbox = static_cast<wxListBox*>(event.GetEventObject());
	int item = m_lbox->HitTest(event.GetPosition());
    
	if ( item != wxNOT_FOUND ) 
	{
		wxString str = m_lbox->GetString(item);
		std::ostringstream stream;
		stream << _filePath << "/" << str.c_str();
		std::string fullname = stream.str();
		std::cout << "Start Rom At Path: " << fullname << "\n";
		//_timer.Stop();
		StartProgram(fullname);
	}
}

void MainWindow::LaunchRom()
{
	int item = _listBox->GetSelection();
	
	if (item != wxNOT_FOUND )
	{
		const wxString str = _listBox->GetString(item);
		std::ostringstream stream;
		stream << _filePath << "/" << str.c_str();
		const std::string fullname = stream.str();
		std::cout << "Start Rom At Path: " << fullname << "\n";
		StartProgram(fullname);
	}
}

void MainWindow::OnStartRom(wxCommandEvent &event)
{
	std::cout << "Starting Rom...\n";
	LaunchRom();
}

void MainWindow::LoadSettings(wxCommandEvent &event)
{
	_settingsWin->Show(true);
}

void MainWindow::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
	wxMessageBox( "wXChip - Chip8 Emulator",
                     "About wXChip", wxOK | wxICON_INFORMATION );
}

void MainWindow::OnMouseOver(wxMouseEvent &event)
{
    
}

void MainWindow::OnSize(wxSizeEvent& event)
{
    
}


void MainWindow::OnWindowClose(wxCloseEvent &event)
{
	_emulator->Stop();
	closing = true;
	Update();
	Destroy();
}

void MainWindow::LoadList(const std::string &text, const std::string &fps, std::string &cpu_freq)
{

	saveDirectory(text, fps, cpu_freq);

	if(text == "nopath") return;
	
	wxArrayString strings;

	_listBox->Clear();

	DIR *dir = opendir(text.c_str());

	if(dir == NULL)
	{
		std::cerr << "Error could not open directory.\n";
		return;
	}
    
	dirent *e;

	while((e = readdir(dir)))
	{
		if(e->d_type == DT_REG)
		{
			wxString w(e->d_name);
			strings.Add(w);
		}
	}
    
	closedir(dir);
	
	
	if(!strings.IsEmpty())
	{
		_listBox->InsertItems(strings, 0);
		_filePath = text;
		_settingsWin->setRomPath(text, fps, cpu_freq);

	}
	else
 	{
		_settingsWin->setRomPath("", fps, cpu_freq);

	}
}

void MainWindow::OnChip(wxCommandEvent& event)
{
	wxDirDialog dlg(NULL, "Choose input directory", "",
                        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dlg.ShowModal() == wxID_CANCEL)
		return;


	wxString value = dlg.GetPath();
	std::string fps = _settingsWin->FPS();
	std::string cpu = _settingsWin->CPUFreq();
	LoadList(std::string(value.c_str()), fps, cpu);
}


void MainWindow::StartProgram(const std::string &rom)
{
	static bool emuCreated = false;
	if(!emuCreated)
	{
		CreateEmulator();
		emuCreated = true;
	}

	_emulator->Stop();
	_emulator->GetEmulator().Reset();
	_emulator->GetEmulator().LoadRom(rom);
	_emulator->Run();
}

void MainWindow::OnTimer(wxTimerEvent &te)
{
	/*
	if(emu->GetExitFlag()) {
		emu->Dispose();
		_timer.Stop();
		return;
	}
	if(closing == true) return;
	emu->UpdateSystems(); // update window events / input events / timers / flags
	emu->HaltForNextFlag(); // sleep until instrFlag or drawFlag is TRUE
	
	if (emu->GetInstrFlag()) // if instrFLag is true, is time to execute one instruction
		emu->ExecuteInstr();
	if (emu->GetDrawFlag()) // if drawFlag is true, is time to the frame
		emu->Draw();
	 */
}
