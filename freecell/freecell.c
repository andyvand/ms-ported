/****************************************************************************

Freecell.c

June 91, JimH     initial code
Oct  91, JimH     port to Win32


Main source module for Windows Free Cell.
Contains WinMain, initialization routines, and MainWndProc.


Design notes:

Note that although this program uses some of the mapping macros,
this version of the code is 32 bit only!  See Wep2 sources for
16 bit sources.

The current layout of the cards is kept in the array card[MAXCOL][MAXPOS].
In this scheme, column 0 is actually the top row.  In this "column", pos
0 to 3 are the free cells, and 4 to 7 are the home cells.  The other
columns numbered 1 to 8 are the stacked card columns.

See PaintMainWindow() for some details on changing the display for EGA.

A previous version of Free Cell used a timer for multi-card moves.
WM_FAKETIMER messages are now sent manually to accomplish the same thing.

****************************************************************************/

#include "freecell.h"
#include "freecons.h"
#include <shellapi.h>

#ifndef _WIN32_WCE
#include <regstr.h>
#include <htmlhelp.h>   // for HtmlHelp()
#endif

#include <commctrl.h>   // for fusion classes.

#ifdef _DEBUG
TCHAR szDebugBuffer[256];
#endif

extern HANDLE hinstApp;

TCHAR   bigbuf[BIG];            // general purpose LoadString() buffer
TCHAR   bighelpbuf[BIG];        // general purpose char buffer.
BOOL    bCheating;              // hit magic key to win?
BOOL    bDblClick;              // honor double click?
BOOL    bFastMode;              // hidden option, don't do glides?
BOOL    bFlipping;              // currently flipping cards in a column?
BOOL    bGameInProgress;        // true if game is in progress
BOOL    bMessages;              // are "helpful" MessageBoxen shown?
BOOL    bMonochrome;            // 2 colour display?
BOOL    bMoveCol;               // did user request column move (or 1 card)?
BOOL    bSelecting;             // is user selecting game numbers?
BOOL    bWonState;              // TRUE if game won and new game not started
UINT    dxCrd, dyCrd;           // extents of card bitmaps in pixels
CARD    card[MAXCOL][MAXPOS];   // current layout of cards
INT     cFlashes;               // count of main window flashes remaining
UINT    cGames;                 // number of games played in current session
UINT    cLosses;                // number of losses in current session
UINT    cWins;                  // number of wins in current session
UINT    cMoves;                 // number of moves in this game
UINT    dyTops;                 // vert space between cards in columns
CARD    shadow[MAXCOL][MAXPOS]; // shadows card array for multi-moves & cleanup
INT     gamenumber;             // current game number (rand seed)
HBITMAP hBM_Ghost;              // bitmap for ghost (empty) free/home cells
HBITMAP hBM_Bgnd1;              // screen under source location
HBITMAP hBM_Bgnd2;              // screen under destination location
HBITMAP hBM_Fgnd;               // bitmap that moves across screen
HICON   hIconMain;              // the main freecell icon.
HKEY    hkey;                   // registry key
HPEN    hBrightPen;             // 3D highlight colour
HANDLE  hInst;                  // current instance
HWND    hMainWnd;               // hWnd for main window

#ifndef _WIN32_WCE
HFONT   hMenuFont;              // for Cards Left display
#endif

CARD    home[4];                // card on top of home pile for this suit
CARD    homesuit[4];            // suit for each home pile
HBRUSH  hBgndBrush;             // green background brush
UINT_PTR idTimer;               // flash timer id
UINT    moveindex;              // index to end of movelist
MOVE    movelist[MAXMOVELIST];  // compacted list of pending moves for timer
INT     oldgamenumber;          // previous game (repeats don't count in score)
TCHAR   *pszIni;                // .ini filename
TCHAR   smallbuf[SMALL];        // generic small buffer for LoadString()
TCHAR   titlebuf[BIG];          // a buffer used to store the window title.
UINT    wCardCount;             // cards not yet in home cells (0 == win)
UINT    wFromCol;               // col user has selected to transfer from
UINT    wFromPos;               // pos "
UINT    wMouseMode;             // selecting place to transfer FROM or TO
UINT    xOldLoc;                // previous location of cards left text
INT     cUndo;                  // number of cards to undo

/* Registry strings -- do not translate */

#ifndef MIN_MARGIN
#define MIN_MARGIN  ((dxCrd / 8) + 3)
#endif

#ifndef REGSTR_PATH_WINDOWSAPPLETS
#define REGSTR_PATH_WINDOWSAPPLETS TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Applets")
#endif

CONST TCHAR pszRegPath[]  = REGSTR_PATH_WINDOWSAPPLETS TEXT("\\FreeCell");
CONST TCHAR pszWon[]      = TEXT("won");
CONST TCHAR pszLost[]     = TEXT("lost");
CONST TCHAR pszWins[]     = TEXT("wins");
CONST TCHAR pszLosses[]   = TEXT("losses");
CONST TCHAR pszStreak[]   = TEXT("streak");
CONST TCHAR pszSType[]    = TEXT("stype");
CONST TCHAR pszMessages[] = TEXT("messages");
CONST TCHAR pszQuick[]    = TEXT("quick");
CONST TCHAR pszDblClick[] = TEXT("dblclick");
CONST TCHAR pszAlreadyPlayed[] = TEXT("AlreadyPlayed");

#ifndef WTSIZE
#define WTSIZE     50              // window text size in characters
#endif

#ifdef REDUCE_C_RUNTIMES
void _setargv() { }     // reduces size of C runtimes
void _setenvp() { }
#endif

#ifdef _WIN32_WCE
#include <windows.h>
#include <commctrl.h>

VOID APIENTRY HandleToolbarCreate(HWND hwnd, HINSTANCE g_hInst)
{
    HWND g_hWndCB = CommandBar_Create(g_hInst, hwnd, 1);			
    CommandBar_InsertMenubar(g_hWndCB, g_hInst, IDM_MENU, 0);
    CommandBar_AddAdornments(g_hWndCB, 0, 0);    
}
#endif

/****************************************************************************

WinMain(HANDLE, HANDLE, LPSTR, int)

****************************************************************************/

MMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow) /* { */
    MSG msg;                            // message
    HANDLE  hAccel;                     // LifeMenu accelerators

	hinstApp = hInstance;

    if (!hPrevInstance)                 // Other instances of app running?
	{
        if (!InitApplication(hInstance))    // Initialize shared things
            return FALSE;                   // Exits if unable to initialize
	}

    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    hAccel = LoadAccelerators(hInstance, TEXT("FreeMenu"));
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(hMainWnd, hAccel, &msg))
        {
            TranslateMessage(&msg);    // Translates virtual key codes
            DispatchMessage(&msg);     // Dispatches message to window
        }
    }
    DEBUGMSG(TEXT("----  Free Cell Terminated ----\n\r"),0);
    return (INT) msg.wParam;             /* Returns the value from PostQuitMessage */
}


/****************************************************************************

InitApplication(HANDLE hInstance)

****************************************************************************/

BOOL InitApplication(HANDLE hInstance)
{
    WNDCLASS    wc;
    HDC         hIC;            // information context
    INITCOMMONCONTROLSEX icc;   // common control registration.

    DEBUGMSG(TEXT("----  Free Cell Initiated  ----\n\r"),0);

    /* Check if monochrome */

#ifndef _WIN32_WCE
    hIC = CreateIC(TEXT("DISPLAY"), NULL, NULL, NULL);
    if (GetDeviceCaps(hIC, NUMCOLORS) == 2)
#else
	hIC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	if (GetDeviceCaps(hIC, NUMCOLORS) == 2)
#endif
	{
        bMonochrome = TRUE;
        /* BrightPen is not so bright in mono. */
        hBrightPen = CreatePen(PS_SOLID, 1, RGB(  0,   0,   0));
        hBgndBrush = CreateSolidBrush(RGB(255, 255, 255));
		DEBUGMSG(TEXT("----  Device Caps Black And White  ----\n\r"),0);
    }
    else
    {
        bMonochrome = FALSE;
        hBrightPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
        hBgndBrush = CreateSolidBrush(RGB(0, 127, 0));      // green background
        DEBUGMSG(TEXT("----  Device Caps %d Colors  ----\n\r"), GetDeviceCaps(hIC, NUMCOLORS));
    }
    DeleteDC(hIC);

    // Create the freecell icon
    hIconMain = LoadIcon(hInstance, MAKEINTRESOURCE(ID_ICON_MAIN));
    DEBUGMSG(TEXT("----  Loaded Icon %d  ----\n\r"), ID_ICON_MAIN);

    // Register the common controls.
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);

#ifdef _WIN32_WCE
	icc.dwICC  = ICC_BAR_CLASSES | ICC_COOL_CLASSES | ICC_LISTVIEW_CLASSES | 
                 ICC_PROGRESS_CLASS | ICC_TAB_CLASSES | ICC_UPDOWN_CLASS;
#else
	icc.dwICC  = ICC_ANIMATE_CLASS | ICC_BAR_CLASSES | ICC_COOL_CLASSES | ICC_HOTKEY_CLASS | ICC_LISTVIEW_CLASSES | 
                 ICC_PAGESCROLLER_CLASS | ICC_PROGRESS_CLASS | ICC_TAB_CLASSES | ICC_UPDOWN_CLASS | ICC_USEREX_CLASSES;
#endif

    InitCommonControlsEx(&icc);

	DEBUGMSG(TEXT("----  Common Controls Initiated  ----\n\r"),0);

    wc.style = CS_DBLCLKS;              // allow double clicks
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = hIconMain;
    wc.hCursor = NULL;
    wc.hbrBackground = hBgndBrush;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = TEXT("FreeWClass");

    return RegisterClass(&wc);
}


/****************************************************************************

InitInstance(HANDLE hInstance, int nCmdShow)

****************************************************************************/

#ifndef WS_OVERLAPPEDWINDOW
#define WS_OVERLAPPEDWINDOW WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
#endif

BOOL InitInstance(HANDLE hInstance, INT nCmdShow)
{
    HWND        hWnd;               // Main window handle.
    UINT        col, pos;
    INT         nWindowHeight;
	INT         nWindowWidth;
    UINT        wAlreadyPlayed;     // have we already updated the registry ?
    UINT        cTLost, cTWon;      // total losses and wins
    UINT        cTLosses, cTWins;   // streaks
    UINT        wStreak;            // current streak amount
    UINT        wSType;             // current streak type
    LONG        lRegResult;                 // used to store return code from registry call
	HMENU       hMenu = NULL;
#ifdef _WIN32_WCE
	HDC         hDC = NULL;
	INT         i = 0;
#endif

    if (!hBrightPen || !hBgndBrush)
        return FALSE;

    /* Initialize some global variables */

    for (col = 0; col < MAXCOL; col++)          // clear the deck
        for (pos = 0; pos < MAXPOS; pos++)
            card[col][pos] = EMPTY;

    hInst = hInstance;
    cWins = 0;
    cLosses = 0;
    cGames = 0;
    cUndo = 0;
    gamenumber = 0;             // so no cards are drawn in main wnd
    oldgamenumber = 0;          // this is first game and will count

#ifndef _WIN32_WCE
	hMenuFont = 0;
#endif

    bWonState = FALSE;
    bGameInProgress = FALSE;
    bCheating = FALSE;
    bFastMode = FALSE;
    bFlipping = FALSE;
    pszIni = TEXT("entpack.ini");
    bDblClick = TRUE;
    bMessages = FALSE;

    /* for VGA or smaller, window will just fit inside screen */

    nWindowWidth = min(WINWIDTH, GetSystemMetrics(SM_CXSCREEN));
    nWindowHeight = min(WINHEIGHT, GetSystemMetrics(SM_CYSCREEN));

    /* Create a main window for this application instance.  */
	hMenu = LoadMenu(hInst, TEXT("FreeMenu"));

    LoadString(hInst, IDS_APPNAME, smallbuf, SMALL);

	hWnd = CreateWindow(
        TEXT("FreeWClass"),             // See RegisterClass() call.
        smallbuf,                       // Text for window title bar.
        WS_OVERLAPPEDWINDOW,            // Window style.
        CW_USEDEFAULT,                  // Default horizontal position.
        CW_USEDEFAULT,                  // Default vertical position.
        nWindowWidth,                   //         width.
        nWindowHeight,                  //         height.
        NULL,                           // Overlapped windows have no parent.
        hMenu,                          // Use the window class menu.
        hInstance,                      // This instance owns this window.
        NULL                            // Pointer not needed.
    );

    /* If window could not be created, return "failure" */

    if (!hWnd)
        return FALSE;
    hMainWnd = hWnd;

    /* Make the window visible; update its client area; and return "success" */

    ShowWindow(hWnd, nCmdShow);     // Show the window
    UpdateWindow(hWnd);             // Sends WM_PAINT message

#ifdef _WIN32_WCE
	HandleToolbarCreate(hWnd, hInstance);
#endif

    // Do the transfer of stats from the .ini file to the
    // registry (for people migrating from NT 4.0 freecell to NT 5.0)

    lRegResult = REGOPEN

    if (ERROR_SUCCESS == lRegResult)
    {
        wAlreadyPlayed = GetInt(pszAlreadyPlayed, 0);

        // If this is the first time we are playing 
        // update the registry with the stats from the .ini file.
        if (!wAlreadyPlayed)
        {
            LoadString(hInst, IDS_APPNAME, bigbuf, BIG);

#ifdef _WIN32_WCE
            cTLost = 0;
            cTWon  = 0;

            cTLosses = 0;
            cTWins   = 0;

            wStreak = 0;
            wSType = 0;
#else
            // Read the stats from the .ini file. (if present)
            // If we can't read the stats, default value is zero.
            cTLost = GetPrivateProfileInt(bigbuf, TEXT("lost"), 0, pszIni);
            cTWon  = GetPrivateProfileInt(bigbuf, TEXT("won"), 0, pszIni);

            cTLosses = GetPrivateProfileInt(bigbuf, TEXT("losses"), 0, pszIni);
            cTWins   = GetPrivateProfileInt(bigbuf, TEXT("wins"), 0, pszIni);

            wStreak = GetPrivateProfileInt(bigbuf, TEXT("streak"), 0, pszIni);
            wSType = GetPrivateProfileInt(bigbuf, TEXT("stype"), 0, pszIni);
#endif

            // Copy the stats from the .ini file to the registry.
            SetInt(pszLost, cTLost);
            SetInt(pszWon, cTWon);
            SetInt(pszLosses, cTLosses);
            SetInt(pszWins, cTWins);
            SetInt(pszStreak, wStreak);
            SetInt(pszSType, wSType);

            // Set the already-played flag to 1.
            SetInt(pszAlreadyPlayed, 1);
        }

        REGCLOSE;
    }

#ifdef _WIN32_WCE
	bGameInProgress = FALSE;
    wFromCol = EMPTY;               // no FROM selected
    wMouseMode = FROM;              // FROM selected next
    moveindex = 0;                  // no queued moves
    for (i = 0; i < 4; i++)         // nothing in home cells
    {
        homesuit[i] = EMPTY;
        home[i] = EMPTY;
    }
    ShuffleDeck(hWnd, GenerateRandomGameNum());
    InvalidateRect(hWnd, NULL, TRUE);
    wCardCount = 52;
    bGameInProgress = TRUE;

    DisplayCardCount(hWnd);
    hDC = GetDC(hWnd);
    DrawKing(hDC, RIGHT, FALSE);
    bWonState = FALSE;
    ReleaseDC(hWnd, hDC);
#endif

    return TRUE;                    // Returns the value from PostQuitMessage
}


/****************************************************************************

MainWndProc(HWND, unsigned, UINT, LONG)

****************************************************************************/

LRESULT APIENTRY MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    INT     i;                      // generic counter
    int     nResp;                  // messagebox response
    UINT    col, pos;
    HDC     hDC;
#ifndef _WIN32_WCE
    POINT   FAR *MMInfo;            // for GetMinMaxInfo
	HMENU   hMenu;
#endif
    static  BOOL bEatNextMouseHit = FALSE;  // is next hit only for activation?

    switch (message) {
        case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
#ifndef _WIN32_WCE
                case IDM_ABOUT:
                    LoadString(hInst, IDS_FULLNAME, bigbuf, BIG);
                    LoadString(hInst, IDS_CREDITS, smallbuf, SMALL);
                    ShellAbout(hWnd, (LPCTSTR)bigbuf, (LPCTSTR)smallbuf, hIconMain);
                               
                    break;
#endif

                case IDM_EXIT:
                    SendMessage(hWnd, WM_CLOSE, 0, 0);
                    break;

                case IDM_NEWGAME:
                    lParam = GenerateRandomGameNum();
                case IDM_SELECT:
                case IDM_RESTART:
                    if (bGameInProgress)
                    {
                        LoadString(hInst, IDS_RESIGN, bigbuf, BIG);
                        LoadString(hInst, IDS_APPNAME, smallbuf, SMALL);
                        MessageBeep(MB_ICONQUESTION);
                        if (IDNO == MessageBox(hWnd, bigbuf, smallbuf,
                                                 MB_YESNO | MB_ICONQUESTION))
                        {
                            break;
                        }
                        UpdateLossCount();
                    }

                    if (wParam == IDM_RESTART)
                    {
                        if (bGameInProgress)
                            lParam = gamenumber;
                        else
                            lParam = oldgamenumber;
                    }
                    else if (wParam == IDM_SELECT)
                        lParam = 0L;

                    if (wParam == IDM_NEWGAME)
                        bSelecting = FALSE;
                    else if (wParam == IDM_SELECT)
                        bSelecting = TRUE;

                    bGameInProgress = FALSE;
                    wFromCol = EMPTY;               // no FROM selected
                    wMouseMode = FROM;              // FROM selected next
                    moveindex = 0;                  // no queued moves
                    for (i = 0; i < 4; i++)         // nothing in home cells
                    {
                        homesuit[i] = EMPTY;
                        home[i] = EMPTY;
                    }
                    ShuffleDeck(hWnd, lParam);
                    if (gamenumber == CANCELGAME)
                        break;

                    InvalidateRect(hWnd, NULL, TRUE);
                    wCardCount = 52;
                    bGameInProgress = TRUE;

#ifndef _WIN32_WCE
                    hMenu = GetMenu(hWnd);
                    EnableMenuItem(hMenu, IDM_RESTART, MF_ENABLED);
#endif

                    DisplayCardCount(hWnd);
                    hDC = GetDC(hWnd);
                    DrawKing(hDC, RIGHT, FALSE);
                    bWonState = FALSE;
                    ReleaseDC(hWnd, hDC);
                    break;

                case IDM_STATS:
#ifndef _WIN32_WCE
                    DialogBox(hInst, TEXT("Stats"), hWnd, StatsDlg);
#else
                    DialogBox(hInst, TEXT("Stats"), NULL, StatsDlg);
#endif
					break;

                case IDM_OPTIONS:
#ifndef _WIN32_WCE
                    DialogBox(hInst, MAKEINTRESOURCE(DLG_OPTIONS), hWnd, OptionsDlg);
#else
					DialogBox(hInst, MAKEINTRESOURCE(DLG_OPTIONS), NULL, OptionsDlg);
#endif
					break;

#ifndef _WIN32_WCE
                case IDM_HELP:
                    HtmlHelp(GetDesktopWindow(), GetHelpFileName(), HH_DISPLAY_TOPIC, 0);
                    break;

                case IDM_HOWTOPLAY:
                    HtmlHelp(GetDesktopWindow(), GetHelpFileName(), HH_DISPLAY_INDEX, 0);
                    break;

                case IDM_HELPONHELP:
                    HtmlHelp(GetDesktopWindow(), TEXT("NTHelp.chm"), HH_DISPLAY_TOPIC, 0);
                    break;
#endif

                case IDM_UNDO:
                    Undo(hWnd);
                    break;

                 /* Hidden options -- these strings need not be translated */

                case IDM_CHEAT:
                    i = MessageBox(hWnd, TEXT("Choose Abort to Win,\n")
                                   TEXT("Retry to Lose,\nor Ignore to Cancel."),
                                   TEXT("User-Friendly User Interface"),
                                   MB_ABORTRETRYIGNORE | MB_ICONQUESTION);
                    if (i == IDABORT)
                        bCheating = CHEAT_WIN;
                    else if (i == IDRETRY)
                        bCheating = CHEAT_LOSE;
                    else
                        bCheating = FALSE;
                    break;

                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;

        case WM_CLOSE:
            if (bGameInProgress)        // did user quit mid-game?
            {
                LoadString(hInst, IDS_APPNAME, smallbuf, SMALL);
                LoadString(hInst, IDS_RESIGN, bigbuf, BIG);
                MessageBeep(MB_ICONQUESTION);
                nResp = MessageBox(hWnd, bigbuf, smallbuf,
                                   MB_YESNO | MB_ICONQUESTION);
                if (nResp == IDNO)
                    break;

                UpdateLossCount();
            }

            WriteOptions();
            return DefWindowProc(hWnd, message, wParam, lParam);

        case WM_CREATE:
            WMCreate(hWnd);
            break;

        case WM_SIZE:
        {
            DrawMenuBar(hWnd);              // fixes overlapping score on menu
            xOldLoc = 30000;                // force cards left to redraw
            DisplayCardCount(hWnd);         // must update if size changes
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }

        case WM_DESTROY:
            if (hBgndBrush)
                DeleteObject(hBgndBrush);
            if (hBrightPen)
                DeleteObject(hBrightPen);
            if (hBM_Fgnd)
                DeleteObject(hBM_Fgnd);
            if (hBM_Bgnd1)
                DeleteObject(hBM_Bgnd1);
            if (hBM_Bgnd2)
                DeleteObject(hBM_Bgnd2);
            if (hBM_Ghost)
                DeleteObject(hBM_Ghost);

#ifndef _WIN32_WCE
            if (hMenuFont)
                DeleteObject(hMenuFont);
#endif

            cdtTerm();
            PostQuitMessage(0);
            break;

        case WM_PAINT:
            PaintMainWindow(hWnd);
            break;

        /***** NOTE: WM_LBUTTONDBLCLK falls through to WM_LBUTTONDOWN ****/

        /* Double clicking works by simulating a move to a free cell.  On
           the off cycle (that is, when wMouseMode == FROM) the double
           click is processed as a single click to cancel the move, and a
           second double click message is posted. */

        case WM_LBUTTONDBLCLK:
            if (moveindex != 0)     // no mouse hit while cards moving
                break;

            if (gamenumber == 0)
                break;

            if (bFlipping)
                break;

             if (bDblClick && wFromCol > TOPROW && wFromCol < MAXCOL)
            {
                if (wMouseMode == TO)
                {
                    Point2Card(LOWORD(lParam), HIWORD(lParam), &col, &pos);
                    if (col == wFromCol)
                        if (ProcessDoubleClick(hWnd))   // if card moved ok
                            break;
                }
                else
                    PostMessage(hWnd, message, wParam, lParam);
            }

        case WM_LBUTTONDOWN:
            if (bEatNextMouseHit)       // is this only window activation?
            {
                bEatNextMouseHit = FALSE;
                break;
            }
            bEatNextMouseHit = FALSE;

            if (bFlipping)          // cards flipping for keyboard players
                break;

            if (moveindex != 0)     // no mouse hit while cards moving
                break;

            if (gamenumber == 0)
                break;

            if (wMouseMode == FROM)
                SetFromLoc(hWnd, LOWORD(lParam), HIWORD(lParam));
            else
                ProcessMoveRequest(hWnd, LOWORD(lParam), HIWORD(lParam));
            break;


        case WM_RBUTTONDOWN:
            SetCapture(hWnd);
            if (bFlipping)
                break;

            if (gamenumber != 0)
                RevealCard(hWnd, LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_RBUTTONUP:
            ReleaseCapture();
            RestoreColumn(hWnd);
            break;

#ifndef _WIN32_WCE
        case WM_MOUSEACTIVATE:                  // app is being activated,
            if (LOWORD(lParam) == HTCLIENT)     // so don't try new cell on
                bEatNextMouseHit = TRUE;        // clicked location
            break;
#endif

        case WM_MOUSEMOVE:
            SetCursorShape(hWnd, LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_MOVE:                           // card count erases when moved
            DisplayCardCount(hWnd);
            return (DefWindowProc(hWnd, message, wParam, lParam));

#ifndef _WIN32_WCE
        case WM_GETMINMAXINFO:
            if (GetSystemMetrics(SM_CXSCREEN) > 640)    // skip if VGA
            {
                MMInfo = (POINT FAR *) lParam;  // see SDK ref
                if (MMInfo[4].x > WINWIDTH)
                    MMInfo[4].x = WINWIDTH;     // set max window width to 640
            }
            else
                return DefWindowProc(hWnd, message, wParam, lParam);

            break;
#endif

        case WM_CHAR:
            if (!bFlipping)
                KeyboardInput(hWnd, (UINT) wParam);
            break;

        case WM_TIMER:                          // flash main window
            if (wParam == FLASH_TIMER)
                Flash(hWnd);
            else
                Flip(hWnd);
            break;       

        default:                                // Passes it on if unproccessed
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


/****************************************************************************

WMCreate

Handles WM_CREATE message in main window.

****************************************************************************/

VOID WMCreate(HWND hWnd)
{
    BOOL    bResult;                // result of cards.dll initialization
    HDC     hDC;
    HDC     hMemDC;
    HBITMAP hOldBitmap;
    HBRUSH  hOldBrush;
    HPEN    hOldPen;

    /* initialize cards.dll */


	bResult = cdtInit(&dxCrd, &dyCrd);

#if defined(_WIN32_WCE) && !defined(LARGE_SCREEN)
	dxCrd /= 2;
	dyCrd /= 2;

	dxCrd *= 3;
	dxCrd /= 4;

	dyCrd *= 3;
	dyCrd /= 4;
#endif

    CalcOffsets(hWnd);

    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hBM_Fgnd  = CreateCompatibleBitmap(hDC, dxCrd, dyCrd);
    hBM_Bgnd1 = CreateCompatibleBitmap(hDC, dxCrd, dyCrd);
    hBM_Bgnd2 = CreateCompatibleBitmap(hDC, dxCrd, dyCrd);
    hBM_Ghost = CreateCompatibleBitmap(hDC, dxCrd, dyCrd);
    if (hBM_Ghost)          // if memory allocation succeeded
    {
        hOldBitmap = SelectObject(hMemDC, hBM_Ghost);
        hOldBrush  = SelectObject(hMemDC, hBgndBrush);
        PatBlt(hMemDC, 0, 0, dxCrd, dyCrd, PATCOPY);

        hOldPen = SelectObject(hMemDC, GetStockObject(BLACK_PEN));
        MoveToEx(hMemDC, 0, dyCrd-2, NULL);
        LineTo(hMemDC, 0, 0);
        LineTo(hMemDC, dxCrd-1, 0);

        SelectObject(hMemDC, hBrightPen);
        MoveToEx(hMemDC, dxCrd-1, 1, NULL);
        LineTo(hMemDC, dxCrd-1, dyCrd-1);
        LineTo(hMemDC, 0, dyCrd-1);

        SelectObject(hMemDC, hOldPen);
        SelectObject(hMemDC, hOldBitmap);
        SelectObject(hMemDC, hOldBrush);
    }
    DeleteDC(hMemDC);
    ReleaseDC(hWnd, hDC);

	DEBUGMSG(TEXT("Load cards: %d\r\n"), bResult);
	DEBUGMSG(TEXT("Foreground: %p\r\n"), hBM_Fgnd);
	DEBUGMSG(TEXT("Background 1: %p\r\n"), hBM_Bgnd1);
	DEBUGMSG(TEXT("Background 2: %p\r\n"), hBM_Bgnd2);

    if (!bResult || !hBM_Fgnd || !hBM_Bgnd1 || !hBM_Bgnd2)
    {
        LoadString(hInst, IDS_MEMORY, bigbuf, BIG);
        LoadString(hInst, IDS_APPNAME, smallbuf, SMALL);
        MessageBeep(MB_ICONHAND);
        MessageBox(hWnd, bigbuf, smallbuf, MB_OK | MB_ICONHAND);
        PostQuitMessage(0);
        return;
    }

    ReadOptions();

#ifndef _WIN32_WCE
    CreateMenuFont();
#endif
}


/****************************************************************************

CreateMenuFont

Makes a copy of the menu font and puts the handle in hMenuFont

****************************************************************************/

#ifndef _WIN32_WCE
VOID CreateMenuFont()
{
    LOGFONT lf;                         // description of menu font
    NONCLIENTMETRICS ncm;

    hMenuFont = 0;
	ncm.cbSize = sizeof(ncm);

    if (!SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0))
        return;

    lf.lfHeight         = (int)ncm.lfMenuFont.lfHeight;
    lf.lfWidth          = (int)ncm.lfMenuFont.lfWidth;
    lf.lfEscapement     = (int)ncm.lfMenuFont.lfEscapement;
    lf.lfOrientation    = (int)ncm.lfMenuFont.lfOrientation;
    lf.lfWeight         = (int)ncm.lfMenuFont.lfWeight;
    lf.lfItalic         = ncm.lfMenuFont.lfItalic;
    lf.lfUnderline      = ncm.lfMenuFont.lfUnderline;
    lf.lfStrikeOut      = ncm.lfMenuFont.lfStrikeOut;
    lf.lfCharSet        = ncm.lfMenuFont.lfCharSet;
    lf.lfOutPrecision   = ncm.lfMenuFont.lfOutPrecision;
    lf.lfClipPrecision  = ncm.lfMenuFont.lfClipPrecision;
    lf.lfQuality        = ncm.lfMenuFont.lfQuality;
    lf.lfPitchAndFamily = ncm.lfMenuFont.lfPitchAndFamily;
    lstrcpyn(lf.lfFaceName, ncm.lfMenuFont.lfFaceName, LF_FACESIZE);

    hMenuFont = CreateFontIndirect(&lf);
}
#endif
