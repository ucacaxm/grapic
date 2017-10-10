/****************************************************************************
Copyright (C) 2010-2020 Alexandre Meyer

This file is part of Grapic.

Grapic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Grapic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Grapic.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef GRAPIC_H
#define GRAPIC_H

/// \cond

#include <cassert>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

/// \endcond


namespace grapic
{

//! \todo Finish coding the game "dame"



/// \cond

/**
//==================================================================================================
//==================================================================================================
//==================================================================================================
//================================= GRAPIC CLASS ===================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================
* @brief The Grapic class
*/
class Grapic
{
public:
    Grapic();
    void init(const char* name, int w, int h);
    bool manageEvent();

    inline void clear();
    inline void clearEvent();
    inline bool display();
    inline bool hasFinished();
    inline void quit();
    inline bool isInit();
    inline void color(unsigned char r, unsigned char g, unsigned b, unsigned char a);
    inline SDL_Color& getColor();
    inline SDL_Color& getBackgroundColor();
    inline void backgroundColor(unsigned char r, unsigned char g, unsigned b, unsigned char a);
    inline int inverseY(int y);
    inline void setFont(int s, const char* ttf=NULL);
    inline const SDL_Window* window() const;
    inline SDL_Renderer * renderer();
    inline TTF_Font* font();
    inline int keyHasBeenPressed(unsigned int sc);
    inline void setKeyRepeatMode(bool kr);

    static inline Grapic& singleton(bool secure=true);

protected:
    int m_width;
    int m_height;
    SDL_Window* m_window;
    SDL_Renderer *m_renderer;
    TTF_Font *m_font;
    std::string m_fontFile;
    int m_fontSize;
    bool m_quit;
    bool m_anim;
    SDL_Color m_currentColor;
    SDL_Color m_backgroundColor;
    std::vector<int> m_keyStates;
    bool m_keyRepeatMode;

    inline void initKeyArray();
    inline void help() const;

    static Grapic currentGrapic;
};


class Image
{
public:
    Image() : m_surface(NULL), m_texture(NULL), m_has_changed(false) {}
    Image(const char* filename, bool transparency, unsigned char r, unsigned char g, unsigned b, unsigned char a);
    Image(int w, int h);
    void savePNG(const char* filename) const;
    bool isInit() const { return m_surface && m_texture; }
    unsigned char get(int x, int y, int c);
    void set(int x, int y, unsigned char r, unsigned char g, unsigned b, unsigned char a);
    void printInfo() const;

    const SDL_Surface* surface() const { return m_surface; }

    void draw(int x, int y, int w, int h);
    void draw(int x, int y, int w, int h, float angle, int flip);

protected:
    SDL_Surface* m_surface;
    SDL_Texture* m_texture;
    bool m_has_changed;
};



class Menu
{
public:
    Menu() : m_select(0), m_visible(true) {}
    void change(int i, const std::string& str)
    {
        if ((i>=0) && (i<m_txt.size()))
            m_txt[i] = str;
        else
            std::cerr<<"menu_change(...): i is not in the range of the menu"<<std::endl;
    }

    int select() const { return m_select; }
    void setSelect(int s) { assert(s>=0); assert(s<m_txt.size()); m_select=s; }
    int caseToPixel(int c, int ymin, int ymax) const
    {
        return ymin + (m_txt.size()-c) * ( (ymax-ymin+1)/m_txt.size() );
    }

    void draw(int xmin, int ymin, int xmax, int ymax);

    void add(const std::string& str) { m_txt.push_back(str); }

protected:
    std::vector<std::string> m_txt;
    int m_select;
    bool m_visible;
};



typedef std::vector< std::pair<float,float> > Curve;
class Plot
{
public:
    Plot() : m_nb_plot_max(-1) {}

    void clear()
    {
        for(int i=0; i<m_dat.size(); ++i)
            m_dat[i].clear();
        m_dat.clear();
    }

    void setSize(const int n)
    {
        clear();
        m_nb_plot_max = n;
    }

    void add(float x, float y, int curve_n);
    void draw(int xmin, int ymin, int xmax, int ymax, bool clearOrNot) const;
    void draw( const Curve& cu, int xmin, int ymin, int xmax, int ymax, float fxmin, float fymin, float fxmax, float fymax) const;
    void minMax(float& fxmin, float& fymin, float& fxmax, float& fymax, int& maxsize) const;

protected:
    std::vector< Curve  > m_dat;
    int m_nb_plot_max;
};



inline void Grapic::help() const
{
    printf("Help:\n");
    printf("   q,ESC: quit\n");
    printf("   F12: save the screen\n");
    printf("   F1: hide/show the menu (if you use one in your program)\n");
    printf("   h: help\n");
}


inline int Grapic::keyHasBeenPressed(unsigned int key)
{
    SDL_Scancode code= SDL_GetScancodeFromKey(key);
    assert((size_t) code < m_keyStates.size());

    int res = (int)  m_keyStates[code];
    if (!m_keyRepeatMode)
    {
        m_keyStates[code]= 0;
    }
    return res;
}


inline Grapic& Grapic::singleton(bool secure)
{
    if (secure)
    {
        if (!currentGrapic.isInit())
        {
            std::cout<<"You have to call winInit before any call to Grapic functions !"<<std::endl;
            exit(1);
        }
    }
    return currentGrapic;
}



inline    bool Grapic::hasFinished()
{
    return m_quit;
}

inline    bool Grapic::isInit()
{
    return m_window;
}


inline  const  SDL_Window* Grapic::window() const
{
    return m_window;
}

inline     SDL_Renderer * Grapic::renderer()
{
    return m_renderer;
}

inline TTF_Font* Grapic::font()
{
    return m_font;
}


inline void Grapic::initKeyArray()
{
    int keys;
    const unsigned char *state= SDL_GetKeyboardState(&keys);
    m_keyStates.assign(state, state + keys);
}


inline void Grapic::clear()
{
    // Clear the entire screen to our selected color.
    SDL_SetRenderDrawColor(m_renderer, m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
    SDL_RenderClear(m_renderer);
    SDL_SetRenderDrawColor(m_renderer, m_currentColor.r, m_currentColor.g, m_currentColor.b, m_currentColor.a);
}

inline void Grapic::clearEvent()
{
    SDL_Event events;
    SDL_PumpEvents();
    while (SDL_PollEvent(&events))
    {
    }
    initKeyArray();
}


inline bool Grapic::display()
{
    manageEvent();
    SDL_RenderPresent(m_renderer);
    return m_quit;
}


inline void Grapic::quit()
{
    TTF_CloseFont(m_font);
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    m_font = NULL;
    m_window = NULL;
    m_renderer = NULL;
    SDL_Quit();
}


inline void Grapic::setKeyRepeatMode(bool kr)
{
    m_keyRepeatMode = kr;
}

inline void Grapic::color(unsigned char r, unsigned char g, unsigned b, unsigned char a)
{
    m_currentColor.r = r;
    m_currentColor.g = g;
    m_currentColor.b = b;
    m_currentColor.a = a;
    SDL_SetRenderDrawColor(m_renderer, m_currentColor.r, m_currentColor.g, m_currentColor.b, m_currentColor.a);
}


inline SDL_Color& Grapic::getColor()
{
    return m_currentColor;
}

inline SDL_Color& Grapic::getBackgroundColor()
{
    return m_backgroundColor;
}


inline void Grapic::backgroundColor(unsigned char r, unsigned char g, unsigned b, unsigned char a)
{
    m_backgroundColor.r = r;
    m_backgroundColor.g = g;
    m_backgroundColor.b = b;
    m_backgroundColor.a = a;
}


inline int Grapic::inverseY(int y)
{
    SDL_GetRendererOutputSize(m_renderer, &m_width, &m_height);
    return m_height - y - 1;
}


inline void Grapic::setFont(int s, const char* ttf)
{
    if ((m_fontSize==s) && (ttf && m_fontFile==std::string(ttf))) return;
    if (m_font) TTF_CloseFont(m_font);
    m_fontSize = s;
    if (ttf) m_fontFile = ttf;
    m_font = TTF_OpenFont( m_fontFile.c_str(), m_fontSize);
    if (!m_font) m_font = TTF_OpenFont( ("../"+m_fontFile).c_str(), m_fontSize);
    if (!m_font) m_font = TTF_OpenFont( ("../../"+m_fontFile).c_str(), m_fontSize);
    if (!m_font) m_font = TTF_OpenFont( ("../../../"+m_fontFile).c_str(), m_fontSize);
    if (!m_font) m_font = TTF_OpenFont( ("../../../../"+m_fontFile).c_str(), m_fontSize);
    if (!m_font)
    {
        std::cout << "Erreur lors de l'initialisation de la police : " << SDL_GetError() << std::endl;
        SDL_Quit();
        assert(0);
        exit(1);
    }
}



/// \endcond


//==================================================================================================
//==================================================================================================
//==================================================================================================
//============================= GRAPIC FUNCTIONS ===================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================




/** \brief Initialize the window
*/
void winInit(const char* name, int w, int h);

/** \brief Clear the window with the default background color
    ~~~~~~~~~~~~~~~{.c}
	bool stop=false;
	winInit("Tutorials", 500, 500);
	winClear();
	rectangle(100,100, 200, 200);
	winDisplay();
	pressSpace();
	winQuit();
    ~~~~~~~~~~~~~~~
*/
void winClear();

/** \brief return true if the application should be closed (because of 'ESC' or 'q' key pressed for instance)
*/
bool winHasFinished();

/** \brief Clear the event queue of the window
*/
void winClearEvent();

/** \brief Display the window. All drawing is hidden until this function is not called.
    ~~~~~~~~~~~~~~~{.c}
	bool stop=false;
	winInit("Tutorials", 500, 500);
	while( !stop )
	{
		winClear();
		draw();
		stop = winDisplay();
	}
	winQuit();
    ~~~~~~~~~~~~~~~
*/
bool winDisplay();

/** \brief Quit and close all things
*/
void winQuit();

/** \brief Change the default color
*/
void color(unsigned char _r = 255, unsigned char _g = 255, unsigned char _b = 255, unsigned char _a = 255);

/** \brief Change the default background color (the color used to clear the screen)
*/
void backgroundColor(unsigned char _r = 255, unsigned char _g = 255, unsigned char _b = 255, unsigned char _a = 255);

/** \brief Draw a circle from (xmin,ymin) to (xmax,ymax)
*/
void circle(int xc, int yc, int radius);

/** \brief Draw a filled circle from (xmin,ymin) to (xmax,ymax)
*/
void circleFill(int xc, int yc, int radius);

/** \brief Draw a rectangle from (xmin,ymin) to (xmax,ymax)
*/
void rectangle(int xmin, int ymin, int xmax, int ymax);

/** \brief Draw a filled rectangle from (xmin,ymin) to (xmax,ymax)
*/
void rectangleFill(int xmin, int ymin, int xmax, int ymax);

/** \brief Draw a line from (x1,y1) to (x2,y2)
*/
void line(int x1, int y1, int x2, int y2);

/** \brief Draw a pixel on (x,y) with color (r,g,b,a)
*/
void put_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);

/** \brief Draw a point at (x,y)
*/
void point(int x, int y);

/** \brief Draw an array of n points
*/
void points(int p[][2], int n);

/** \brief Draw a grid from (xmin,ymin) to (xmax,ymax) with nx columns and ny rows
*/
void grid(int xmin, int ymin, int xmax, int ymax, int nx, int ny);

/** \brief return a random number (integer) between rmin to rmax included
*/
int irand(int rmin=0, int rmax=100);

/** \brief return a random number (float) between rmin to rmax included
*/
float frand(float rmin = 0.f, float rmax = 1.f);

/** \brief return the time elapsed since the beginning of the process in second
*/
float elapsedTime();

/** \brief return the number of time the key 'key' has been pressed since the last call to this function.
    ~~~~~~~~~~~~~~~{.c}
	if (isKeyPressed(SDLK_LEFT)) { ... }     // test if the left arrow is pressed
	if (isKeyPressed(SDLK_LEFT)) {  ... }    // if the left arrow is pressed then do ...
	if (isKeyPressed(SDLK_a)) {  ... }       // if the key 'a' is pressed then do ...
	if (isKeyPressed('a')) { ... }          // if the key 'a' is pressed then do ...
    ~~~~~~~~~~~~~~~
*/
int isKeyPressed(int key);

/** \brief (de)Activate the repeat mode: when the user presses continuously on the key touch is repeated. It has to be set at the beguinning of the program.
*/
void setKeyRepeatMode(bool repeat);

/** \brief Stop the program during d milliseconds
    ~~~~~~~~~~~~~~~{.c}
	if (isKeyPressed(SDLK_LEFT)) {  ... }    // if the left arrow is pressed then do ...
	if (isKeyPressed(SDLK_a)) {  ... }       // if the key 'a' is pressed then do ...
	if (isKeyPressed('a')) { ... }          // if the key 'a' is pressed then do ...
    ~~~~~~~~~~~~~~~
*/
static inline void delay(int d)
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(d));
    SDL_Delay(d);
}

/** \brief return true if the mouse button 'button' is pressed
    ~~~~~~~~~~~~~~~{.c}
    if (isMousePressed(SDL_BUTTON_LEFT)) { ... }    // test if the left button of the mouse is pressed
    ~~~~~~~~~~~~~~~
*/
bool isMousePressed(int button);

/** \brief After this function (x,y) store the mouse position
    ~~~~~~~~~~~~~~~{.c}
    int x,y;
    mousePos(x,y);                                                                          // test if the left button of the mouse is pressed
    if ((isMousePressed(SDL_BUTTON_LEFT)) && (x>10) && (y>10) && (x<50) && (y<50)) { ... }  // if the user clicks if the square (10,10)(50,50)
    ~~~~~~~~~~~~~~~
*/
void mousePos(int& x, int& y);

/** \brief Manage standard event like key 'ESC', quit, etc.
*/
bool winManageEvent();

/** \brief Change the default size of the font
*/
void fontSize(int s);

/** \brief Print the text txt , up left corner is (x,y)
    ~~~~~~~~~~~~~~~{.c}
    fontSize(24);                               // Change the default font to the size 24
    print( 10, 20, "Bonjour");                  // Print the text "Bonjour" with the left corner (10,20)
    ~~~~~~~~~~~~~~~
*/
void print(int x, int y, const char* txt);

/** \brief Print the integer nb, up left corner is (x,y)
    ~~~~~~~~~~~~~~~{.c}
    print( 10, 20, 128);                      // Print the number "128" with the left corner (10,20)
    ~~~~~~~~~~~~~~~
*/
void print(int x, int y, int nb);

/** \brief Print the float nb, up left corner is (x,y)
    ~~~~~~~~~~~~~~~{.c}
    print( 10, 20, 3.1415);                      // Print the number "3.1415" with the left corner (10,20)
    ~~~~~~~~~~~~~~~
*/
void print(int x, int y, float nb);

/** \brief Stop the program until key 'space'is pressed
*/
void pressSpace();




/** \brief Return an image loaded from the file filename
    ~~~~~~~~~~~~~~~{.c}
    struct Data
    {
        Image im;
    };
    void init(Data& d)
    {
        d.im = image("data/grapic.bmp", true, 255, 255, 255, 255);      // load the image "data/grapic.bmp" with transparency for the color (255,255,255,255)
    }
    void draw(Data& d)
    {
        image_draw(d.im, 0, 255);                                       // Draw the image with the left corner (0,255)
    }
    ~~~~~~~~~~~~~~~
*/
static inline Image image(const char* filename, bool transparency=false, unsigned char r=255, unsigned char g=255, unsigned b=255, unsigned char a=255)
{
    return Image(filename, transparency, r, g, b, a);
}

/** \brief Return an image of width=w and height=h
*/
static inline Image image(int w, int h)
{
    return Image(w,h);
}

/** \brief Save the image into the file: format is PNG
*/
static inline void image_savePNG(const Image& im, const char* filename)
{
    im.savePNG(filename);
}

/** \brief Draw the image at position (x,y) with width=w and height=h (if w<0 or h<0 the original size of the image is used)
*/
static inline void image_draw(Image& im, int x, int y, int w=-1, int h=-1)
{
    im.draw(x,y,w,h);
}

/** \brief Draw the image at position (x,y) with width=w and height=h (if w<0 or h<0 the original size of the image is used); angle indicate the angle of rotation and flip: 0=no flip, 1=horizontal flip, 2=vertical flip
*/
static inline void image_draw(Image& im, int x, int y, int w, int h, float angle, float flip=SDL_FLIP_NONE)
{
    im.draw(x,y,w,h,angle,flip);
}

/** \brief return the color component c of the pixel (x,y) of the image im.
    c must be 0 for the red component, 1 for the green component, 2 for the blue component or 3 for the alpha/opacity component.
*/
static inline unsigned char image_get(Image& im, int x, int y, int c=0)
{
    return im.get(x,y,c);
}

/** \brief Set the pixel (x,y) of the image im with the color c
*/
void image_set(Image& im, int x, int y, unsigned char r, unsigned char g, unsigned b, unsigned char a);

/** \brief return the width of the image
*/
static inline int image_width(const Image& im)
{
    return im.surface()->w;
}

/** \brief return the height of the image
*/
static inline int image_height(const Image& im)
{
    return im.surface()->h;
}

/** \brief return true if the image is initialized
    ~~~~~~~~~~~~~~~{.c}
	d.im = image("data/grapic.bmp", true, 255, 255, 255, 255);
    if (!image_isInit(d.im))                                            // if the image was not loaded, try an other path
        d.im = image("../data/grapic.bmp", true, 255,255,255,255 );
    ~~~~~~~~~~~~~~~
*/
static inline bool image_isInit(const Image& im)
{
    return im.isInit();
}

/** \brief Print the informations of the image im
*/
static inline void image_printInfo(const Image& im)
{
    im.printInfo();
}



/** \brief Add a line to the menu m with the text str
    ~~~~~~~~~~~~~~~{.c}
	Menu m;
	bool stop=false;
	winInit("Tutorials", 500, 500);
	menu_add( m, "Question 1");
	menu_add( m, "Question 2");
	while( !stop )
	{
		winClear();
		menu_draw(m, 5,5, 100, 102);
		switch(menu_select(m))
		{
			case 0 : draw1(); break;
			case 1 : draw2(); break;
		}
		stop = winDisplay();
	}
	winQuit();
    ~~~~~~~~~~~~~~~
*/
static inline void menu_add(Menu& m, const std::string& str)
{
    m.add(str);
}

//! \brief Change the text of a line in the menu
static inline void menu_change(Menu& m, int i, const std::string& str)
{
    m.change(i,str);
}

//! \brief Draw the menu on the screen. See menu_add for an example of usage.
static inline void menu_draw(Menu& m, int xmin=5, int ymin=5, int xmax=-1, int ymax=-1)
{
    m.draw(xmin,ymin,xmax,ymax);
}

//! \brief return the line selected in the menu. See menu_add for an example of usage.
static inline int menu_select(const Menu& m)
{
    return m.select();
}

//! \brief return the line selected in the menu. See menu_add for an example of usage.
static inline void menu_setSelect(Menu& m, int s)
{
    m.setSelect(s);
}

//! \brief return the pixel from a line of the menu
static inline int caseToPixel(const Menu& m, int c, int ymin, int ymax)
{
    return m.caseToPixel(c,ymin,ymax);
}


//! @todo: plot: setColor for each curves
//! @todo: setRangeXMinMax for each curves
//! \brief Clear the data stored
static inline void plot_clear(Plot& p )
{
    p.clear();
}

//! \brief Define the size of the stored value of the funtion (<0 means infinity)
static inline void plot_setSize(Plot& p, const int n)
{
    p.setSize(n);
}

//! \brief Add a point (x,y=f(x)) to the curve number curve_n
static inline void plot_add(Plot& p, float x, float y, int curve_n=0)
{
    p.add(x,y,curve_n);
}

//! \brief Draw the curve in the rectangle (xmin,ymin,xmax,ymax); clear the rectangle if clearOrNot is true
static inline void plot_draw( const Plot& p, int xmin, int ymin, int xmax, int ymax, bool clearOrNot=true)
{
    p.draw(xmin,ymin,xmax,ymax,clearOrNot);
}




/**
    \brief Draw a triangle from the vertices (x1, y1), (x2, y2) and (x3, y3). (Code provided by Bastien DOIGNIES, many thanks)
 */
void triangle(int x1, int y1,  int x2, int y2, int x3, int y3);

/**
    \brief Draw a regular polygon with line_numbers edges centred on (x, y). (Code provided by Bastien DOIGNIES, many thanks)
 */
void regular_polygone(int x, int y, unsigned int apotheme, unsigned int line_number);

/**
    \brief Draw a filled triangle from the vertices (x1, y1), (x2, y2), (x3, y3)
 */
void triangleFill( int x1, int y1,
                   int x2, int y2,
                   int x3, int y3);


/**
    \brief Draw a filled regular polygon with line_numbers edges centred on (x, y). (Code provided by Bastien DOIGNIES, many thanks)
*/
void regular_polygonFill(int x, int y, unsigned int apotheme, unsigned int line_number);


/**
    \brief Decide if a point (px, py) is inside the triangle (ax, ay), (bx, by), (cx, xy). (Code provided by Bastien DOIGNIES, many thanks)
    \return true if inside, false otherwise
 */
bool isInTriangle(float px, float py, float ax, float ay, float bx, float by, float cx, float cy);


/** \brief  Draw a simple(no edge-crossing) and without holes filled polygon
 * \param   p[]    : array of integer which represent the coordinate of each vertices
 * \param   number : number of element in p[], must be even (point are two integers each)!
 * \warning Draw nothing if there are not enough vertices or if number is not even
 * \warning Undetermined draw if the polygon is not simple
 (Code provided by Bastien DOIGNIES, many thanks)
 */
void polygonFill(int p[][2], unsigned int number);


/** \brief Draw a polygon. (Code provided by Bastien DOIGNIES, many thanks)
 */
void polygon(int p[][2], unsigned int number);





} // namespace

#endif







//==================================================================================================
//==================================================================================================
//==================================================================================================
//============================= DOCUMENTATION MAIN PAGE ============================================
//==================================================================================================
//==================================================================================================
//==================================================================================================

/*! \mainpage GrAPiC

<br>
\image html grapic_samples.jpg
<br>
\section intro Introduction

GrAPiC is a small, easy-to-use framework to add graphical functionalities to your C/C++ code. Its main goal is to provide easy
graphical functions to be used in introduction courses of algorithm and programming in C/C++.
It is based on SDL2 but to use it you need to know only few very simples functions.
The Grapic archive comes with the needed SDL part, you do not have to install SDL. Just unzip and enjoy !
<br>
<br>
<br>

\section download Download
Download and install the last version:\htmlinclude "VERSION.html".
Previous versions may be downloaded from the [download](../download) directory.

<br>
<br>
<br>
\section start How to start ?

\subsection win Run on Windows
  - Install Codeblocks from: [http://www.codeblocks.org/downloads](http://www.codeblocks.org/downloads)\n
    Take the one with MinGW like codeblocks-16.01mingw-setup.exe

  - Download the archive file, see section download.

  - Run with CodeBlocks
     - Open the file Grapic/build/windows/grapic_workspace with Codeblocks
     - Select in the projects manager (on the left) the project you want with the right button "Activate project":
       \image html cb_activatePrj.jpg "Selection of the project in Codeblocks" width=400
     - And run the program !

\subsection linux Run on Linux
  - Install SDL2, SDL_Image for SDL2 and SDL_ttf for SDL2 ; for instance with
    \code
    sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev
    \endcode

  - Download the archive file, see section download.

  - Run with CodeBlocks
     - Open the file Grapic/build/linux/grapic_workspace with Codeblocks.
     - Select in the projects manager (on the left) the project you want with the right button "Activate project":
       \image html cb_activatePrj.jpg "Selection of the project in Codeblocks" width=400
     - And run the program !
  - Run in the Terminal:
     - cd ....grapic
     - make
     - bin/demo


\subsection mac Run on MacOS
  - Download the archive file, see section download.

  - Run in the Terminal:
     - cd grapic
     - make
     - bin/demo

\subsection edit Edit my first program
The kernel source code of grapic is in grapic/src. The source codes of the programs/demos/tutorials are in grapic/apps.
You can edit your first program by editing the file grapic/apps/start/main_start.cpp
\image html cb_editStart.jpg width=10

\subsection create Create a new project
You can create your own project.
 - Copy the directory apps/start into apps/MYPROJECT  (replace MYPROJECT by the name you want)
 - Rename apps/MYPROJECT/main_start.cpp into main_MYPROJECT.cpp
 - Edit the file Grapic/premake4.lua and add at the end
make_project( "MYPROJECT", 	"apps/MYPROJECT/main_MYPROJECT.cpp" )
 - Run the script 'premake.bat' on windows; or 'make premake' on Linux/MacOS




<br>
<br>
<br>
\section tuto Tutorials

<br>
<br>
<br>
\subsection tuto1 Tutorial 1: Simple program
\image html tuto_simple.jpg "The code of the tutorial 1 "simpleDrawing" illustrates the basis of Grapic" width=200px
\include "../../apps/tutorials/tuto1_Simple.cpp"

<br>
<br>
<br>
\subsection tuto2 Tutorial 2: First Loop
\image html tuto_drawing.jpg "The code of the tutorial 2 shows how to write the main loop of a Grapic application" width=200px
\include "../../apps/tutorials/tuto2_FirstLoop.cpp"

<br>
<br>
<br>
\subsection tuto3 Tutorial 3: Drawing
\image html tuto_drawing.jpg "The code of the tutorial 3 shows how to draw rectangles, circles, lines, etc." width=200px
\include "../../apps/tutorials/tuto3_Drawing.cpp"

<br>
<br>
<br>
\subsection tuto4 Tutorial 4: Keyboard
\image html tuto_drawing.jpg "The code of the tutorial 4 shows how to manage the keyboard." width=200px
\include "../../apps/tutorials/tuto4_Keyboard.cpp"

<br>
<br>
<br>
\subsection tuto5 Tutorial 5: Mouse
\image html tuto_mouse.jpg "The code of the tutorial 5 shows how manage the mouse to draw a square" width=200px
\include "../../apps/tutorials/tuto5_Mouse.cpp"

<br>
<br>
<br>
\subsection tuto6 Tutorial 6: Menu
\image html tuto_menu.jpg "The code of the tutorial 6 shows how to create a simple menu." width=200px
\include "../../apps/tutorials/tuto6_Menu.cpp"

<br>
<br>
<br>
\subsection tuto7 Tutorial 7: Animation
\image html tuto_animation.jpg "The code of the tutorial 2 shows how to display a simple animation depending on the elapsed time." width=200px
\include "../../apps/tutorials/tuto7_Animation.cpp"

<br>
<br>
<br>
\subsection tuto8 Tutoria 8: Plot/Graph
\image html tuto_plot.jpg "The plot code illustrates the drawing of a graph entering values of a function" width=200px
\include "../../apps/tutorials/tuto8_Plot.cpp"

<br>
<br>
<br>
\subsection tutoDemo Tutoria 9: Demo
\image html tuto_demo.jpg "The demo code illustrates many functionalities of Grapic" width=200px
\include "../../apps/tutorials/tuto9_demo.cpp"


<br>
<br>
<br>
\section Examples

Grapic comes with several small examples. It can be used as source of inspiration.


\subsection tetris Tetris
\image html tetris.jpg "A example of tetris" width=200px
\include "../../apps/demo_tetris/main_tetris.cpp"


\subsection pacman Pacman
\image html pacman.jpg "Pacman" width=200px
\include "../../apps/demo_pacman/main_pacman.cpp"



<br>
<br>
\section problems Current problems

- int main() ==> int main(int , char **)

- Image im = image("data\papillon.jpg") ==> Image im = image("data//papillon.jpg")   or Image im = image("data\\papillon.jpg")

- Even tutorials do not compile
   - check in directory 'extern' that you have the version for your computer: mingw for windows, macox for mac and nothing for
        linux since you have to install them with the apt-get explained in the Section installtion
   - Try to rerun premake with premake.bat on windows or make premake on linux/macos

*/
