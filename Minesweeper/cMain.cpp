#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Minesweeper - wxWidgets!")
{
	btn = new wxButton * [nFieldWidth * nFieldHeight];
	wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);

	nField = new int[nFieldWidth * nFieldHeight];

	//Better font
	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int x = 0; x < nFieldWidth; x++)
	{
		for (int y = 0; y < nFieldHeight; y++)
		{
			btn[y * nFieldWidth + x] = new wxButton(this, 10000 + (y * nFieldWidth + x));
			btn[y * nFieldWidth + x]->SetFont(font);
			grid->Add(btn[y * nFieldWidth + x], 1, wxEXPAND | wxALL);

			btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
			nField[y * nFieldWidth + x] = 0;
		}
	}

	this->SetSizer(grid);
	grid->Layout();
}

cMain::~cMain()
{
	delete[]btn;
}

void cMain::OnButtonClicked(wxCommandEvent &evt)
{
	//Get coordinate of button in field array
	int x = (evt.GetId() - 10000) % nFieldWidth;
	int y = (evt.GetId() - 10000) / nFieldWidth;

	if (bFirstClick)
	{
		int mines = 30;

		while (mines)
		{
			int rx = rand() % nFieldWidth;
			int ry = rand() % nFieldHeight;
			
			if (nField[ry * nFieldWidth + rx] == 0 && rx != x && ry != y)
			{
				//put a mine there
				nField[ry * nFieldWidth + rx] = -1;
				mines--;
			}
		}

		bFirstClick = false;
	}

	//Disable button, preventing it bing pressed again
	btn[y * nFieldWidth + x]->Enable(false);

	//Check if player hit a mine
	if (nField[y*nFieldWidth + x] == -1)
	{
		wxMessageBox("BOOOOOOOM !!! - Game Over :(");

		//Reset game
		bFirstClick = true;
		for (int x = 0; x < nFieldWidth; x++)
		{
			for (int y = 0; y < nFieldHeight; y++)
			{
				nField[y * nFieldWidth + x] = 0;
				btn[y * nFieldWidth + x]->SetLabel("");
				btn[y * nFieldWidth + x]->Enable(true);

			}
		}
	}
	else
	{
		//Count neighboring mines
		int mine_count = 0;
		for (int minesx = -1; minesx < 2; minesx++)
		{
			for (int minesy = -1; minesy < 2; minesy++)
			{
				if (x + minesx >= 0 && x + minesx < nFieldWidth && y + minesy >= 0 && y + minesy < nFieldHeight)
				{
					if (nField[(y + minesy) * nFieldWidth + (x + minesx)] == -1)
						mine_count++;
				}
			}
		}

		//Update buttons label to show mine count if > 0
		if (mine_count > 0)
		{
			//display mine count
			btn[y * nFieldWidth + x]->SetLabel(std::to_string(mine_count));
		}
	}
	evt.Skip();
}