//------------------------------------------------------------------------------
// Console.h: interface for the Console manipulators.
// https://www.codeproject.com/Articles/9130/Add-Color-to-Console-Projects
// LICENSE: CPOL
//------------------------------------------------------------------------------
#pragma once

namespace JadedHoboConsole
{
    static const WORD bgMask( BACKGROUND_BLUE      | 
                              BACKGROUND_GREEN     | 
                              BACKGROUND_RED       | 
                              BACKGROUND_INTENSITY   );
    static const WORD fgMask( FOREGROUND_BLUE      | 
                              FOREGROUND_GREEN     | 
                              FOREGROUND_RED       | 
                              FOREGROUND_INTENSITY   );
    
    static const WORD fgBlack    ( 0 ); 
    static const WORD fgLoRed    ( FOREGROUND_RED   ); 
    static const WORD fgLoGreen  ( FOREGROUND_GREEN ); 
    static const WORD fgLoBlue   ( FOREGROUND_BLUE  ); 
    static const WORD fgLoCyan   ( fgLoGreen   | fgLoBlue ); 
    static const WORD fgLoMagenta( fgLoRed     | fgLoBlue ); 
    static const WORD fgLoYellow ( fgLoRed     | fgLoGreen ); 
    static const WORD fgLoWhite  ( fgLoRed     | fgLoGreen | fgLoBlue ); 
    static const WORD fgGray     ( fgBlack     | FOREGROUND_INTENSITY ); 
    static const WORD fgHiWhite  ( fgLoWhite   | FOREGROUND_INTENSITY ); 
    static const WORD fgHiBlue   ( fgLoBlue    | FOREGROUND_INTENSITY ); 
    static const WORD fgHiGreen  ( fgLoGreen   | FOREGROUND_INTENSITY ); 
    static const WORD fgHiRed    ( fgLoRed     | FOREGROUND_INTENSITY ); 
    static const WORD fgHiCyan   ( fgLoCyan    | FOREGROUND_INTENSITY ); 
    static const WORD fgHiMagenta( fgLoMagenta | FOREGROUND_INTENSITY ); 
    static const WORD fgHiYellow ( fgLoYellow  | FOREGROUND_INTENSITY );
    static const WORD bgBlack    ( 0 ); 
    static const WORD bgLoRed    ( BACKGROUND_RED   ); 
    static const WORD bgLoGreen  ( BACKGROUND_GREEN ); 
    static const WORD bgLoBlue   ( BACKGROUND_BLUE  ); 
    static const WORD bgLoCyan   ( bgLoGreen   | bgLoBlue ); 
    static const WORD bgLoMagenta( bgLoRed     | bgLoBlue ); 
    static const WORD bgLoYellow ( bgLoRed     | bgLoGreen ); 
    static const WORD bgLoWhite  ( bgLoRed     | bgLoGreen | bgLoBlue ); 
    static const WORD bgGray     ( bgBlack     | BACKGROUND_INTENSITY ); 
    static const WORD bgHiWhite  ( bgLoWhite   | BACKGROUND_INTENSITY ); 
    static const WORD bgHiBlue   ( bgLoBlue    | BACKGROUND_INTENSITY ); 
    static const WORD bgHiGreen  ( bgLoGreen   | BACKGROUND_INTENSITY ); 
    static const WORD bgHiRed    ( bgLoRed     | BACKGROUND_INTENSITY ); 
    static const WORD bgHiCyan   ( bgLoCyan    | BACKGROUND_INTENSITY ); 
    static const WORD bgHiMagenta( bgLoMagenta | BACKGROUND_INTENSITY ); 
    static const WORD bgHiYellow ( bgLoYellow  | BACKGROUND_INTENSITY );
    
    static class con_dev
    {
        private:
        HANDLE                      hCon;
        DWORD                       cCharsWritten; 
        CONSOLE_SCREEN_BUFFER_INFO  csbi; 
        DWORD                       dwConSize;

        public:
        con_dev() 
        { 
            hCon = GetStdHandle( STD_OUTPUT_HANDLE );
        }
        private:
        void GetInfo()
        {
            GetConsoleScreenBufferInfo( hCon, &csbi );
            dwConSize = csbi.dwSize.X * csbi.dwSize.Y; 
        }
        public:
        void Clear()
        {
            COORD coordScreen = { 0, 0 };
            
            GetInfo(); 
            FillConsoleOutputCharacter( hCon, ' ',
                                        dwConSize, 
                                        coordScreen,
                                        &cCharsWritten ); 
            GetInfo(); 
            FillConsoleOutputAttribute( hCon,
                                        csbi.wAttributes,
                                        dwConSize,
                                        coordScreen,
                                        &cCharsWritten ); 
            SetConsoleCursorPosition( hCon, coordScreen ); 
        }
        void GotoXY(WORD x, WORD y)
        {
            COORD pos = { (SHORT)x, (SHORT)y };
            SetConsoleCursorPosition(hCon, pos);
        }
        void SetColor( WORD wRGBI, WORD Mask )
        {
            GetInfo();
            csbi.wAttributes &= Mask; 
            csbi.wAttributes |= wRGBI; 
            SetConsoleTextAttribute( hCon, csbi.wAttributes );
        }
        inline void SetConsoleSize(WORD cols, WORD rows)
        {
            GetInfo();
            WORD oldc = csbi.dwSize.X;
            WORD oldr = csbi.dwSize.Y;
            COORD newSize = { (SHORT)cols, (SHORT)rows };
            SMALL_RECT rect = { 0, 0, cols - 1, rows - 1 };

            if (oldr <= rows)
            {
                if (oldc <= cols)
                {                           // increasing both dimensions
                BUFWIN:
                    SetConsoleScreenBufferSize(hCon, newSize);
                    SetConsoleWindowInfo(hCon, TRUE, &rect);
                }
                else
                {                           // cols--, rows+
                    SMALL_RECT tmp = { 0, 0, cols - 1, oldr - 1 };
                    SetConsoleWindowInfo(hCon, TRUE, &tmp);
                    goto BUFWIN;
                }
            }
            else
            {
                if (oldc <= cols)
                {                           // cols+, rows--
                    SMALL_RECT tmp = { 0, 0, oldc - 1, rows - 1 };
                    SetConsoleWindowInfo(hCon, TRUE, &tmp);
                    goto BUFWIN;
                }
                else
                {                           // cols--, rows--
                    SetConsoleWindowInfo(hCon, TRUE, &rect);
                    SetConsoleScreenBufferSize(hCon, newSize);
                }
            }
            GetInfo();
        }
    } console;
    
    inline void setsize(WORD x, WORD y)
    {
        console.SetConsoleSize(x, y);
    }
    inline void gotoxy(WORD x, WORD y)
    {
        console.GotoXY(x, y);
    }
    //narrow manipulators
    inline std::ostream& clr( std::ostream& os )
    {
        os.flush();
        console.Clear();
        return os;
    };
    
    inline std::ostream& fg_red( std::ostream& os )
    {
        os.flush();
        console.SetColor( fgHiRed, bgMask );
        
        return os;
    }
    
    inline std::ostream& fg_green( std::ostream& os )
    {
        os.flush();
        console.SetColor( fgHiGreen, bgMask );
        
        return os;
    }
    
    inline std::ostream& fg_blue( std::ostream& os )
    {
        os.flush();
        console.SetColor( fgHiBlue, bgMask );
        
        return os;
    }
    
    inline std::ostream& fg_white( std::ostream& os )
    {
        os.flush();
        console.SetColor( fgHiWhite, bgMask );
        
        return os;
    }
    
    inline std::ostream& fg_cyan( std::ostream& os )
    {
        os.flush();
        console.SetColor( fgHiCyan, bgMask );
        
        return os;
    }
    
    inline std::ostream& fg_magenta( std::ostream& os )
    {
        os.flush();
        console.SetColor( fgHiMagenta, bgMask );
        
        return os;
    }
    
    inline std::ostream& fg_yellow( std::ostream& os )
    {
        os.flush();
        console.SetColor( fgHiYellow, bgMask );
        
        return os;
    }
    
    inline std::ostream& fg_black( std::ostream& os )
    {
        os.flush();
        console.SetColor( fgBlack, bgMask );
        
        return os;
    }
    
    inline std::ostream& fg_gray( std::ostream& os )
    {
        os.flush();
        console.SetColor( fgGray, bgMask );
        
        return os;
    }
    
    inline std::ostream& bg_red( std::ostream& os )
    {
        os.flush();
        console.SetColor( bgHiRed, fgMask );
        
        return os;
    }
    
    inline std::ostream& bg_green( std::ostream& os )
    {
        os.flush();
        console.SetColor( bgHiGreen, fgMask );
        
        return os;
    }
    
    inline std::ostream& bg_blue( std::ostream& os )
    {
        os.flush();
        console.SetColor( bgHiBlue, fgMask );
        
        return os;
    }
    
    inline std::ostream& bg_white( std::ostream& os )
    {
        os.flush();
        console.SetColor( bgHiWhite, fgMask );
        
        return os;
    }
    
    inline std::ostream& bg_cyan( std::ostream& os )
    {
        os.flush();
        console.SetColor( bgHiCyan, fgMask );
        
        return os;
    }
    
    inline std::ostream& bg_magenta( std::ostream& os )
    {
        os.flush();
        console.SetColor( bgHiMagenta, fgMask );
        
        return os;
    }
    
    inline std::ostream& bg_yellow( std::ostream& os )
    {
        os.flush();
        console.SetColor( bgHiYellow, fgMask );
        
        return os;
    }
    
    inline std::ostream& bg_black( std::ostream& os )
    {
        os.flush();
        console.SetColor( bgBlack, fgMask );
        
        return os;
    }
    
    inline std::ostream& bg_gray( std::ostream& os )
    {
        os.flush();
        console.SetColor( bgGray, fgMask );
        
        return os;
    }
    
    //wide manipulators
    inline std::wostream& clr( std::wostream& os )
    {
        os.flush();
        console.Clear();
        return os;
    };
    
    inline std::wostream& fg_red( std::wostream& os )
    {
        os.flush();
        console.SetColor( fgHiRed, bgMask );
        
        return os;
    }
    
    inline std::wostream& fg_green( std::wostream& os )
    {
        os.flush();
        console.SetColor( fgHiGreen, bgMask );
        
        return os;
    }
    
    inline std::wostream& fg_blue( std::wostream& os )
    {
        os.flush();
        console.SetColor( fgHiBlue, bgMask );
        
        return os;
    }
    
    inline std::wostream& fg_white( std::wostream& os )
    {
        os.flush();
        console.SetColor( fgHiWhite, bgMask );
        
        return os;
    }
    
    inline std::wostream& fg_cyan( std::wostream& os )
    {
        os.flush();
        console.SetColor( fgHiCyan, bgMask );
        
        return os;
    }
    
    inline std::wostream& fg_magenta( std::wostream& os )
    {
        os.flush();
        console.SetColor( fgHiMagenta, bgMask );
        
        return os;
    }
    
    inline std::wostream& fg_yellow( std::wostream& os )
    {
        os.flush();
        console.SetColor( fgHiYellow, bgMask );
        
        return os;
    }
    
    inline std::wostream& fg_black( std::wostream& os )
    {
        os.flush();
        console.SetColor( fgBlack, bgMask );
        
        return os;
    }
    
    inline std::wostream& fg_gray( std::wostream& os )
    {
        os.flush();
        console.SetColor( fgGray, bgMask );
        
        return os;
    }
    
    inline std::wostream& bg_red( std::wostream& os )
    {
        os.flush();
        console.SetColor( bgHiRed, fgMask );
        
        return os;
    }
    
    inline std::wostream& bg_green( std::wostream& os )
    {
        os.flush();
        console.SetColor( bgHiGreen, fgMask );
        
        return os;
    }
    
    inline std::wostream& bg_blue( std::wostream& os )
    {
        os.flush();
        console.SetColor( bgHiBlue, fgMask );
        
        return os;
    }
    
    inline std::wostream& bg_white( std::wostream& os )
    {
        os.flush();
        console.SetColor( bgHiWhite, fgMask );
        
        return os;
    }
    
    inline std::wostream& bg_cyan( std::wostream& os )
    {
        os.flush();
        console.SetColor( bgHiCyan, fgMask );
        
        return os;
    }
    
    inline std::wostream& bg_magenta( std::wostream& os )
    {
        os.flush();
        console.SetColor( bgHiMagenta, fgMask );
        
        return os;
    }
    
    inline std::wostream& bg_yellow( std::wostream& os )
    {
        os.flush();
        console.SetColor( bgHiYellow, fgMask );
        
        return os;
    }
    
    inline std::wostream& bg_black( std::wostream& os )
    {
        os.flush();
        console.SetColor( bgBlack, fgMask );
        
        return os;
    }
    
    inline std::wostream& bg_gray( std::wostream& os )
    {
        os.flush();
        console.SetColor( bgGray, fgMask );
        
        return os;
    }
}

