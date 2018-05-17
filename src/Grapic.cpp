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


#include <Grapic.h>


namespace grapic
{

using namespace std;


Grapic Grapic::currentGrapic;


Grapic::Grapic() :
    m_width(-1),
    m_height(-1),
    m_window(NULL),
    m_renderer(NULL),
    m_font(NULL),
    m_fontFile(""),
    m_fontSize(-1),
    m_quit(false),
    m_anim(false),
    m_keyRepeatMode(false)
{
    m_currentColor.r = 0;
    m_currentColor.g = 0;
    m_currentColor.b = 0;
    m_currentColor.a = 255;
    m_backgroundColor.r = 255;
    m_backgroundColor.g = 255;
    m_backgroundColor.b = 255;
    m_backgroundColor.a = 255;
}


void Grapic::init(const char* name, int w, int h)
{
    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();
        assert(0);
        exit(1);
    }

    if (TTF_Init() != 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL_ttf : " << SDL_GetError() << std::endl;
        SDL_Quit();
        assert(0);
        exit(1);
    }

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        SDL_Quit();
        assert(0);
        exit(1);
    }

    //system("cd");
    //setFont( 20, "data/ttf/Raleway-Regular.ttf");
    setFont( 20, "data/ttf/Roboto-Regular.ttf");

    // Creation de la fenetre
    if (m_window) SDL_DestroyWindow(m_window);
    m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN ); //| SDL_WINDOW_RESIZABLE);
    if (m_window == NULL)
    {
        std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
        SDL_Quit();
        assert(0);
        exit(1);
    }

    SDL_SetWindowTitle(m_window, name);

    m_renderer = SDL_CreateRenderer( m_window, -1, SDL_RENDERER_ACCELERATED);

    m_width = w;
    m_height = h;
    m_quit = false;
    m_anim = false;

    SDL_SetRenderDrawBlendMode( m_renderer, SDL_BLENDMODE_BLEND);

    initKeyArray();
    help();
}


const SDL_Surface* Image::surface() const
{
    return m_surface;
}

    bool Image::isInit() const { return m_surface && m_texture; }


Menu::Menu() : m_select(0), m_visible(true) {}

void Menu::change(int i, const std::string& str)
{
    if ((i>=0) && (i<m_txt.size()))
        m_txt[i] = str;
    else
        std::cerr<<"menu_change(...): i is not in the range of the menu"<<std::endl;
}

    Plot::Plot() : m_nb_plot_max(-1) {}

    void Plot::clear()
    {
        for(int i=0; i<m_dat.size(); ++i)
            m_dat[i].clear();
        m_dat.clear();
    }

    void Plot::setSize(const int n)
    {
        clear();
        m_nb_plot_max = n;
    }


int Menu::select() const
{
    return m_select;
}
void Menu::setSelect(int s)
{
    assert(s>=0);
    assert(s<m_txt.size());
    m_select=s;
}
int Menu::caseToPixel(int c, int ymin, int ymax) const
{
    return ymin + (m_txt.size()-c) * ( (ymax-ymin+1)/m_txt.size() );
}

void Menu::add(const std::string& str)
{
    m_txt.push_back(str);
}


bool saveScreenshotPNG(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer)
{
    SDL_Surface* saveSurface = NULL;
    SDL_Surface* infoSurface = NULL;
    infoSurface = SDL_GetWindowSurface(SDLWindow);
    if (infoSurface == NULL)
    {
        std::cerr << "Failed to create info surface from window in saveScreenshotBMP(string), SDL_GetError() - " << SDL_GetError() << "\n";
    }
    else
    {
        unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
        if (pixels == 0)
        {
            std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";
            return false;
        }
        else
        {
            if (SDL_RenderReadPixels(SDLRenderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0)
            {
                std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";
                pixels = NULL;
                return false;
            }
            else
            {
                saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
                if (saveSurface == NULL)
                {
                    std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";
                    return false;
                }
                //SDL_SaveBMP(saveSurface, filepath.c_str());
                IMG_SavePNG(saveSurface, filepath.c_str());
                SDL_FreeSurface(saveSurface);
                saveSurface = NULL;
            }
            delete[] pixels;
        }
        SDL_FreeSurface(infoSurface);
        infoSurface = NULL;
    }
    return true;
}



bool Grapic::manageEvent()
{
    SDL_Event event;
    SDL_PumpEvents();
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            m_quit = true;
            break;
        case SDL_KEYDOWN:
            if (((size_t) event.key.keysym.scancode < m_keyStates.size()) && ((m_keyRepeatMode) || (event.key.repeat==0)))
            {
                //cout<<event.key.keysym.scancode<<endl;
                m_keyStates[event.key.keysym.scancode]++; // = 1;
                //last_key= event.key;    // conserver le dernier evenement
            }

            if ((event.key.keysym.sym == SDLK_ESCAPE) || (event.key.keysym.sym == SDLK_q))
                m_quit = true;
            else if (event.key.keysym.sym == SDLK_F12)
            {
                saveScreenshotPNG( "grapic.png", m_window, m_renderer);
                printf("Save grapic.png\n");
            }
            else if (event.key.keysym.sym == SDLK_h)
            {
                help();
            }
            break;
        case SDL_KEYUP:
            // modifier l'etat du clavier
            if((size_t) event.key.keysym.scancode < m_keyStates.size())
            {
                m_keyStates[event.key.keysym.scancode]= 0;
                //last_key= event.key;    // conserver le dernier evenement
            }
            break;
        default:
            break;
        }
    }
    return m_quit;
}


void Grapic::clearEvent()
{
    SDL_Event events;
    SDL_PumpEvents();
    while (SDL_PollEvent(&events))
    {
    }
    initKeyArray();
}


Grapic& Grapic::singleton(bool secure)
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


void Grapic::help() const
{
    printf("Help:\n");
    printf("   q,ESC: quit\n");
    printf("   F12: save the screen\n");
    printf("   F1: hide/show the menu (if you use one in your program)\n");
    printf("   h: help\n");
}


Image::Image() : m_surface(NULL), m_texture(NULL), m_has_changed(false)
{}


int Grapic::keyHasBeenPressed(unsigned int key)
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


bool Grapic::hasFinished()
{
    return m_quit;
}

bool Grapic::isInit()
{
    return m_window;
}


const  SDL_Window* Grapic::window() const
{
    return m_window;
}

SDL_Renderer * Grapic::renderer()
{
    return m_renderer;
}

TTF_Font* Grapic::font()
{
    return m_font;
}


void Grapic::initKeyArray()
{
    int keys;
    const unsigned char *state= SDL_GetKeyboardState(&keys);
    m_keyStates.assign(state, state + keys);
}


void Grapic::clear()
{
    // Clear the entire screen to our selected color.
    SDL_SetRenderDrawColor(m_renderer, m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
    SDL_RenderClear(m_renderer);
    SDL_SetRenderDrawColor(m_renderer, m_currentColor.r, m_currentColor.g, m_currentColor.b, m_currentColor.a);
}



bool Grapic::display()
{
    manageEvent();
    SDL_RenderPresent(m_renderer);
    return m_quit;
}


void Grapic::quit()
{
    TTF_CloseFont(m_font);
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    m_font = NULL;
    m_window = NULL;
    m_renderer = NULL;
    SDL_Quit();
}


void Grapic::setKeyRepeatMode(bool kr)
{
    m_keyRepeatMode = kr;
}

void Grapic::color(unsigned char r, unsigned char g, unsigned b, unsigned char a)
{
    m_currentColor.r = r;
    m_currentColor.g = g;
    m_currentColor.b = b;
    m_currentColor.a = a;
    SDL_SetRenderDrawColor(m_renderer, m_currentColor.r, m_currentColor.g, m_currentColor.b, m_currentColor.a);
}


SDL_Color& Grapic::getColor()
{
    return m_currentColor;
}

SDL_Color& Grapic::getBackgroundColor()
{
    return m_backgroundColor;
}


void Grapic::backgroundColor(unsigned char r, unsigned char g, unsigned b, unsigned char a)
{
    m_backgroundColor.r = r;
    m_backgroundColor.g = g;
    m_backgroundColor.b = b;
    m_backgroundColor.a = a;
}


int Grapic::inverseY(int y)
{
    SDL_GetRendererOutputSize(m_renderer, &m_width, &m_height);
    return m_height - y - 1;
}


void Grapic::setFont(int s, const char* ttf)
{
    if ((m_font) && (m_fontSize==s) && (ttf && m_fontFile==std::string(ttf))) return;
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



//==================================================================================================
//==================================================================================================
//==================================================================================================
//============================= GRAPIC FUNCTIONS ===================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================


SDL_Renderer* renderer()
{
    return Grapic::singleton().renderer();
}

void winInit(const char* name, int w, int h)
{
    Grapic::singleton(false).init(name,w,h);
}

void winClear()
{
    Grapic::singleton().clear();
}

bool winHasFinished()
{
    return Grapic::singleton().hasFinished();
}

bool winDisplay()
{
    return Grapic::singleton().display();
}

void winQuit()
{
    Grapic::singleton().quit();
}

void color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
    Grapic::singleton().color( _r, _g, _b, _a );
}


void backgroundColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
    Grapic::singleton().backgroundColor( _r, _g, _b, _a);
}

void pressSpace()
{
    winClearEvent();
    winDisplay();
    print(10,10,"Press space");
    winDisplay();
    printf("Press space\n");

    while(!winHasFinished() && !isKeyPressed(' '))
    {
        Grapic::singleton().manageEvent();
        delay(50);
    }
}

bool winManageEvent()
{
    return Grapic::singleton().manageEvent();
}


void rectangle(int xmin, int ymin, int xmax, int ymax)
{
    Grapic& g = Grapic::singleton();
    SDL_Rect r;
    r.x = xmin;
    r.y = g.inverseY(ymax);
    r.w = xmax - xmin + 1;
    r.h = ymax - ymin + 1;
    SDL_RenderDrawRect(renderer(), &r);
}

void rectangleFill(int xmin, int ymin, int xmax, int ymax)
{
    Grapic& g = Grapic::singleton();
    SDL_Rect r;
    r.x = xmin;
    r.y = g.inverseY(ymax);
    r.w = xmax - xmin + 1;
    r.h = ymax - ymin + 1;
    SDL_RenderFillRect(renderer(), &r);
}

int filledEllipseRGBA(SDL_Renderer* m_renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    int ix, iy;
    int h, i, j, k;
    int oh, oi, oj, ok;
    int xmh, xph;
    int xmi, xpi;
    int xmj, xpj;
    int xmk, xpk;

    /*
    * Sanity check radii
    */
    if ((rx < 0) || (ry < 0))
    {
        return (-1);
    }

    /*
    * Special case for rx=0 - draw a vline
    */
    if (rx == 0)
    {
        return SDL_RenderDrawLine(m_renderer, x, y-ry, x, y+ry);
        //return (vlineRGBA(renderer, x, y - ry, y + ry, r, g, b, a));
    }
    /*
    * Special case for ry=0 - draw a hline
    */
    if (ry == 0)
    {
        return SDL_RenderDrawLine(m_renderer, x-rx, y, x+rx, y);
        //		return (hlineRGBA(renderer, x - rx, x + rx, y, r, g, b, a));
    }

    /*
    * Set color
    */
    result = 0;
    result |= SDL_SetRenderDrawBlendMode(m_renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
    result |= SDL_SetRenderDrawColor(m_renderer, r, g, b, a);

    /*
    * Init vars
    */
    oh = oi = oj = ok = 0xFFFF;

    /*
    * Draw
    */
    if (rx > ry)
    {
        ix = 0;
        iy = rx * 64;

        do
        {
            h = (ix + 32) >> 6;
            i = (iy + 32) >> 6;
            j = (h * ry) / rx;
            k = (i * ry) / rx;

            if ((ok != k) && (oj != k))
            {
                xph = x + h;
                xmh = x - h;
                if (k > 0)
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmh, y+k, xph, y+k); //hline(renderer, xmh, xph, y + k);
                    result |= SDL_RenderDrawLine(m_renderer, xmh, y-k, xph, y-k); //hline(renderer, xmh, xph, y - k);
                }
                else
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmh, y, xph, y); // hline(renderer, xmh, xph, y);
                }
                ok = k;
            }
            if ((oj != j) && (ok != j) && (k != j))
            {
                xmi = x - i;
                xpi = x + i;
                if (j > 0)
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmi, y+j, xpi, y+j);  //hline(renderer, xmi, xpi, y + j);
                    result |= SDL_RenderDrawLine(m_renderer, xmi, y-j, xpi, y-j); //hline(renderer, xmi, xpi, y - j);
                }
                else
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmi, y, xpi, y); //hline(renderer, xmi, xpi, y);
                }
                oj = j;
            }

            ix = ix + iy / rx;
            iy = iy - ix / rx;

        }
        while (i > h);
    }
    else
    {
        ix = 0;
        iy = ry * 64;

        do
        {
            h = (ix + 32) >> 6;
            i = (iy + 32) >> 6;
            j = (h * rx) / ry;
            k = (i * rx) / ry;

            if ((oi != i) && (oh != i))
            {
                xmj = x - j;
                xpj = x + j;
                if (i > 0)
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmj, y+i, xpj, y+i); //hline(renderer, xmj, xpj, y + i);
                    result |= SDL_RenderDrawLine(m_renderer, xmj, y-i, xpj, y-i); //hline(renderer, xmj, xpj, y - i);
                }
                else
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmj, y, xpj, y); //hline(renderer, xmj, xpj, y);
                }
                oi = i;
            }
            if ((oh != h) && (oi != h) && (i != h))
            {
                xmk = x - k;
                xpk = x + k;
                if (h > 0)
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmk, y+h, xpk, y+h); //hline(renderer, xmk, xpk, y + h);
                    result |= SDL_RenderDrawLine(m_renderer, xmk, y-h, xpk, y-h); //hline(renderer, xmk, xpk, y - h);
                }
                else
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmk, y, xpk, y); //hline(renderer, xmk, xpk, y);
                }
                oh = h;
            }

            ix = ix + iy / ry;
            iy = iy - ix / ry;

        }
        while (i > h);
    }

    return (result);
}



int pixelRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result = 0;
    result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
    result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
    result |= SDL_RenderDrawPoint(renderer, x, y);
    return result;
}


int pixelRGBAWeight(SDL_Renderer * renderer, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint32 weight)
{
    /*
    * Modify Alpha by weight
    */
    Uint32 ax = a;
    ax = ((ax * weight) >> 8);
    if (ax > 255)
    {
        a = 255;
    }
    else
    {
        a = (Uint8)(ax & 0x000000ff);
    }

    return pixelRGBA(renderer, x, y, r, g, b, a);
}


int aaellipseRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    int i;
    int a2, b2, ds, dt, dxt, t, s, d;
    Sint16 xp, yp, xs, ys, dyt, od, xx, yy, xc2, yc2;
    float cp;
    double sab;
    Uint8 weight, iweight;

    /*
    * Sanity check radii
    */
    if ((rx < 0) || (ry < 0))
    {
        return (-1);
    }

    /*
    * Special case for rx=0 - draw a vline
    */
    if (rx == 0)
    {
        return SDL_RenderDrawLine(renderer, x, y-ry, x, y+ry); //(vlineRGBA(renderer, x, y - ry, y + ry, r, g, b, a));
    }
    /*
    * Special case for ry=0 - draw an hline
    */
    if (ry == 0)
    {
        return SDL_RenderDrawLine(renderer, x-rx, y, x+rx, y); //(hlineRGBA(renderer, x - rx, x + rx, y, r, g, b, a));
    }

    /* Variable setup */
    a2 = rx * rx;
    b2 = ry * ry;

    ds = 2 * a2;
    dt = 2 * b2;

    xc2 = 2 * x;
    yc2 = 2 * y;

    sab = sqrt((double)(a2 + b2));
    od = (Sint16)lrint(sab*0.01) + 1; /* introduce some overdraw */
    dxt = (Sint16)lrint((double)a2 / sab) + od;

    t = 0;
    s = -2 * a2 * ry;
    d = 0;

    xp = x;
    yp = y - ry;

    /* Draw */
    result = 0;
    result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);

    /* "End points" */
    result |= pixelRGBA(renderer, xp, yp, r, g, b, a);
    result |= pixelRGBA(renderer, xc2 - xp, yp, r, g, b, a);
    result |= pixelRGBA(renderer, xp, yc2 - yp, r, g, b, a);
    result |= pixelRGBA(renderer, xc2 - xp, yc2 - yp, r, g, b, a);

    for (i = 1; i <= dxt; i++)
    {
        xp--;
        d += t - b2;

        if (d >= 0)
            ys = yp - 1;
        else if ((d - s - a2) > 0)
        {
            if ((2 * d - s - a2) >= 0)
                ys = yp + 1;
            else
            {
                ys = yp;
                yp++;
                d -= s + a2;
                s += ds;
            }
        }
        else
        {
            yp++;
            ys = yp + 1;
            d -= s + a2;
            s += ds;
        }

        t -= dt;

        /* Calculate alpha */
        if (s != 0)
        {
            cp = (float) abs(d) / (float) abs(s);
            if (cp > 1.0)
            {
                cp = 1.0;
            }
        }
        else
        {
            cp = 1.0;
        }

        /* Calculate weights */
        weight = (Uint8) (cp * 255);
        iweight = 255 - weight;

        /* Upper half */
        xx = xc2 - xp;
        result |= pixelRGBAWeight(renderer, xp, yp, r, g, b, a, iweight);
        result |= pixelRGBAWeight(renderer, xx, yp, r, g, b, a, iweight);

        result |= pixelRGBAWeight(renderer, xp, ys, r, g, b, a, weight);
        result |= pixelRGBAWeight(renderer, xx, ys, r, g, b, a, weight);

        /* Lower half */
        yy = yc2 - yp;
        result |= pixelRGBAWeight(renderer, xp, yy, r, g, b, a, iweight);
        result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, iweight);

        yy = yc2 - ys;
        result |= pixelRGBAWeight(renderer, xp, yy, r, g, b, a, weight);
        result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, weight);
    }

    /* Replaces original approximation code dyt = abs(yp - yc); */
    dyt = (Sint16)lrint((double)b2 / sab ) + od;

    for (i = 1; i <= dyt; i++)
    {
        yp++;
        d -= s + a2;

        if (d <= 0)
            xs = xp + 1;
        else if ((d + t - b2) < 0)
        {
            if ((2 * d + t - b2) <= 0)
                xs = xp - 1;
            else
            {
                xs = xp;
                xp--;
                d += t - b2;
                t -= dt;
            }
        }
        else
        {
            xp--;
            xs = xp - 1;
            d += t - b2;
            t -= dt;
        }

        s += ds;

        /* Calculate alpha */
        if (t != 0)
        {
            cp = (float) abs(d) / (float) abs(t);
            if (cp > 1.0)
            {
                cp = 1.0;
            }
        }
        else
        {
            cp = 1.0;
        }

        /* Calculate weight */
        weight = (Uint8) (cp * 255);
        iweight = 255 - weight;

        /* Left half */
        xx = xc2 - xp;
        yy = yc2 - yp;
        result |= pixelRGBAWeight(renderer, xp, yp, r, g, b, a, iweight);
        result |= pixelRGBAWeight(renderer, xx, yp, r, g, b, a, iweight);

        result |= pixelRGBAWeight(renderer, xp, yy, r, g, b, a, iweight);
        result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, iweight);

        /* Right half */
        xx = xc2 - xs;
        result |= pixelRGBAWeight(renderer, xs, yp, r, g, b, a, weight);
        result |= pixelRGBAWeight(renderer, xx, yp, r, g, b, a, weight);

        result |= pixelRGBAWeight(renderer, xs, yy, r, g, b, a, weight);
        result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, weight);
    }

    return (result);
}



void circle(int xc, int yc, int circleR)
{
    Grapic& g = Grapic::singleton();
    Uint32 cc = *((Uint32*)(&g.getColor()));
    Uint8 *c = (Uint8 *)&cc;
    aaellipseRGBA( g.renderer(), xc, g.inverseY(yc), circleR, circleR, c[0], c[1], c[2], c[3]);
}

void circleFill(int xc, int yc, int circleR)
{
    Grapic& g = Grapic::singleton();
    Uint32 cc = *((Uint32*)(&g.getColor()));
    Uint8 *c = (Uint8 *)&cc;
    filledEllipseRGBA(g.renderer(), xc, g.inverseY(yc), circleR, circleR, c[0], c[1], c[2], c[3]);
}



void line(int xmin, int ymin, int xmax, int ymax)
{
    Grapic& g = Grapic::singleton();
    SDL_RenderDrawLine(renderer(), xmin, g.inverseY(ymin), xmax, g.inverseY(ymax));
}

void put_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    Grapic& gr = Grapic::singleton();
    SDL_Color save = gr.getColor();
    color(r,g,b,a);
    SDL_RenderDrawPoint(renderer(), x, gr.inverseY(y));
    color(save.r,save.g,save.b,save.a);
}

void point(int x, int y)
{
    Grapic& g = Grapic::singleton();
    SDL_RenderDrawPoint(renderer(), x, g.inverseY(y));
}

void points(int p[][2], int n)
{
    //Grapic& g = Grapic::singleton();
    SDL_RenderDrawPoints(renderer(), ((const SDL_Point*)(p)), n);
}

void grid(int xmin, int ymin, int xmax, int ymax, int nx, int ny)
{
    int i;
    //Grapic& g = Grapic::singleton();
    float stepx = static_cast<float>(xmax - xmin + 1) / (nx);
    float stepy = static_cast<float>(ymax - ymin + 1) / (ny);
    for (i = 0; i <= nx; ++i)
        line(xmin + stepx*i, ymin, xmin + stepx*i, ymax);
    for (i = 0; i <= ny; ++i)
        line(xmin, ymin + stepy*i, xmax, ymin + stepy*i);
}

int irand(int rmin, int rmax)
{
    return rmin + rand() % (rmax - rmin + 1);
}

float frand(float rmin, float rmax)
{
    float r = static_cast<float>(rand()) / RAND_MAX;
    return rmin + r * (rmax - rmin);
}

float elapsedTime()
{
    //return float(clock()) / CLOCKS_PER_SEC;
    return 0.001f * SDL_GetTicks();
}



int isKeyPressed(int key)
{
    Grapic& g = Grapic::singleton();
    //SDL_PumpEvents();
    //int siz;
    //const Uint8 *state = SDL_GetKeyboardState(&siz);
    //if (state[key]>0) SDL_Delay(10);
    return (g.keyHasBeenPressed(key)); // || (state[key]>0) );
}

void setKeyRepeatMode(bool repeat)
{
    Grapic& g = Grapic::singleton();
    g.setKeyRepeatMode(repeat);
}


bool isMousePressed(int button)
{
    //SDL_PumpEvents();
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button);
}

void mousePos(int& x, int& y)
{
    SDL_PumpEvents();
    SDL_GetMouseState(&x, &y);
    Grapic& g = Grapic::singleton();
    y = g.inverseY(y);
}



void fontSize(int s)
{
    Grapic::singleton().setFont(s);
}

void print(int x, int y, int nb)
{
    char txt[64];
    sprintf(txt,"%d", nb);
    print(x,y,txt);
}

void print(int x, int y, float nb)
{
    char txt[64];
    sprintf(txt,"%.2f", nb);
    print(x,y,txt);
}

void print(int x, int y, const char* txt)
{
    Grapic& g = Grapic::singleton();
    SDL_Surface* textSurface = TTF_RenderText_Solid( g.font(), txt, g.getColor() );
    SDL_Texture* text = SDL_CreateTextureFromSurface( g.renderer(), textSurface);
    int text_width = textSurface->w;
    int text_height = textSurface->h;
    SDL_FreeSurface(textSurface);
    SDL_Rect renderQuad = { x, g.inverseY(y+text_height), text_width, text_height };
    SDL_RenderCopy( g.renderer(), text, NULL, &renderQuad);
    SDL_DestroyTexture(text);
}


Uint32 image_get(SDL_Surface *surface, int x, int y)
{
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    y = surface->h-1-y;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;
    switch(nbOctetsParPixel)
    {
    case 1:
        return *p;
    case 2:
        return *(Uint16 *)p;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32 *)p;
    default:
        return 0;
    }
}


unsigned char Image::get(int x, int y, int c) const
{
    assert(surface());
    SDL_Surface* surf = m_surface;
    if ((x<0) || (y<0) || (x>=surf->w) || (y>=surf->h)) return 0;
    SDL_PixelFormat *fmt = surf->format;
    Uint32 pixel;

    SDL_LockSurface(surf);
    pixel = image_get( surf, x, y);
    SDL_UnlockSurface(surf);

    Uint8 red, green, blue, alpha;
    SDL_GetRGBA( pixel, fmt, &red, &green, &blue, &alpha);

    switch(c)
    {
    case 0: //2
        return red;
    case 1:
        return green;
    case 2: // 0
        return blue;
    case 3:
        return alpha;
    default:
        return red;
    }
}


void image_set(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    y = surface->h-1-y;

    //Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;
    //*((Uint32 *)p) = pixel;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;
    switch(nbOctetsParPixel)
    {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(Uint16 *)p = pixel;
        break;
    case 3:
        /*Suivant l'architecture de la machine*/
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

void Image::set(int x, int y, unsigned char r, unsigned char g, unsigned b, unsigned char a)
{
    assert(m_surface);
    SDL_PixelFormat *fmt = m_surface->format;
    Uint32 pixel;
    Uint8 *p = (Uint8*)(&pixel);

    if ((x<0) || (y<0) || (x>=m_surface->w) || (y>=m_surface->h)) return;
    SDL_LockSurface(m_surface);
    //pixel = SDL_MapRGBA(fmt, r,g,b,a);
    //pixel = SDL_MapRGBA(fmt, a,r,g,b);
    p[0] = b;
    p[1] = g;
    p[2] = r;
    p[3] = a;
    image_set(m_surface, x, y, pixel);
    SDL_UnlockSurface(m_surface);

    m_has_changed = true;
}

void image_set(Image& im, int x, int y, unsigned char r, unsigned char g, unsigned b, unsigned char a)
{
    im.set(x,y,r,g,b,a);
}



Image::Image(int w, int h)
{
    Grapic& g = Grapic::singleton();
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if 0 //SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    //rmask = gmask = bmask = amask = 0x00000000;

    m_surface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask); // 0, 0, 0, 0);
    //printf("image surface format=%d %d %d\n", res.surface->format, SDL_PIXELFORMAT_RGBA8888, SDL_PIXELFORMAT_BGRA8888);
    if (m_surface == NULL)
    {
        printf("error: Can not create this surface: %dx%d\n", w, h);
        return ;
    }

    //res.texture = SDL_CreateTexture( g.renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, res.surface->w, res.surface->h);
    m_texture = SDL_CreateTextureFromSurface( g.renderer(), m_surface);
    if (m_texture == NULL)
    {
        printf("error: problem to create the texture\n");
        return ;
    }

    m_has_changed = false;

    SDL_SetTextureBlendMode( m_texture, SDL_BLENDMODE_BLEND);

    return ;
}




Image::Image(const char* filename, bool transparency,  unsigned char r, unsigned char g, unsigned b, unsigned char a)
{
    Grapic& gp = Grapic::singleton();

    //res.surface = SDL_LoadBMP(filename);
    m_surface = IMG_Load(filename);
    if (m_surface == NULL)
    {
        std::string nfn = std::string("../") + filename;
        m_surface = IMG_Load(nfn.c_str());
        if (m_surface == NULL)
        {
            nfn = std::string("../") + nfn;
            m_surface = IMG_Load(nfn.c_str());
        }
    }
    if (m_surface == NULL)
    {
        std::cout<<"error: Can not load "<< filename<<std::endl;
        return ;
    }
    SDL_Surface * surfaceCorrectPixelFormat = SDL_ConvertSurfaceFormat(m_surface,SDL_PIXELFORMAT_ARGB8888,0);
    SDL_FreeSurface(m_surface);
    m_surface = surfaceCorrectPixelFormat;
    if (transparency)
    {
        SDL_Color c;
        c.r=r;
        c.g=g;
        c.b=b;
        c.a=a;
        Uint32 cc = *((Uint32*)(&c));
        SDL_SetColorKey(m_surface, SDL_TRUE, cc);
    }

    //res.texture = SDL_CreateTexture(g.renderer(), SDL_PIXELFORMAT_ARGB8888, 	SDL_TEXTUREACCESS_STREAMING,	res.surface->w, res.surface->h);
    m_texture = SDL_CreateTextureFromSurface( gp.renderer(), m_surface);

    if (m_texture == NULL)
    {
        printf("error: problem to create the texture of %s\n", filename);
        return ;
    }

    //int ok = SDL_UpdateTexture(res.texture, NULL, res.surface->pixels, res.surface->pitch);
    //assert(ok==0);

    printf("Image loaded: %s\t", filename);
    printInfo();

}


void Image::draw(int x, int y, int w, int h)
{
    int ok;
    Grapic& g = Grapic::singleton();
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = (w<0)?surface()->w:w;
    r.h = (h<0)?surface()->h:h;
    r.y = g.inverseY( r.y+r.h);

    if (m_has_changed)
    {
        ok = SDL_UpdateTexture(m_texture, NULL, m_surface->pixels, m_surface->pitch);
        assert(ok == 0);
        m_has_changed = false;
    }

    ok = SDL_RenderCopy(g.renderer(), m_texture, NULL, &r);
    assert(ok == 0);
}


void Image::draw(int x, int y, int w, int h, float angle, int flip)
{
    int ok;
    Grapic& g = Grapic::singleton();
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = (w<0)?m_surface->w:w;
    r.h = (h<0)?m_surface->h:h;
    r.y = g.inverseY( r.y+r.h);

    if (m_has_changed)
    {
        ok = SDL_UpdateTexture(m_texture, NULL, m_surface->pixels, m_surface->pitch);
        assert(ok == 0);
        m_has_changed = false;
    }

    //ok = SDL_RenderCopy(g.renderer(), im.texture, NULL, &r);
    //SDL_Point point = {x,y};
    SDL_RendererFlip fl;
    switch (flip)
    {
    case 0 :
        fl = SDL_FLIP_NONE;
        break;
    case 1 :
        fl = SDL_FLIP_HORIZONTAL;
        break;
    case 2 :
        fl = SDL_FLIP_VERTICAL;
        break;
    };
    ok = SDL_RenderCopyEx( g.renderer(), m_texture, NULL, &r, angle, NULL, fl );
    assert(ok == 0);
}


void Image::savePNG(const char* filename) const
{
    int r = IMG_SavePNG( m_surface, filename);
    if (r!=0) printf("image: error saving %s\n", filename);
}


void Image::printInfo() const
{
    if (!isInit())
    {
        printf("image not initialized\n");
        return;
    }
    printf("image: %dx%d format=%d pitch=%d\n", m_surface->w, m_surface->h, m_surface->format->format, m_surface->pitch);
}


void Menu::draw(int xmin, int ymin, int xmax, int ymax)
{
    if (m_txt.size()==0) return;
    if (ymax<0) ymax = 25*m_txt.size();
    int i;
    const int fontsize = 12;

    if (isKeyPressed( SDLK_F1))
    {
        m_visible = !m_visible;
        cout<<"menu: "<< (m_visible?"on":"off")<<endl;
    }
    if (!m_visible) return;

    if (xmax<0)
    {
        int nc=-1;
        for(i=0; i<m_txt.size(); ++i)
        {
            if (nc < int(m_txt[i].length()) )
            {
                nc=int(m_txt[i].length());
            }
        }
        xmax = xmin + 0.6f*fontsize*nc + 10;
    }

    if (isMousePressed(SDL_BUTTON_LEFT))
    {
        int x, y;
        mousePos(x, y);
        if ((x>xmin) && (x<xmax) && (y>ymin) && (y<ymax))
        {
            m_select = m_txt.size()-1 - (y-ymin) / ((ymax-ymin)/m_txt.size());
        }
    }

    fontSize(fontsize);                                           // Change the default size of the font
    for(i=0; i<m_txt.size(); ++i)
    {
        if (i==m_select) color(255,55,124);
        else color(255,255,255);
        rectangleFill( xmin, caseToPixel(i,ymin,ymax), xmax, caseToPixel(i+1,ymin,ymax) );

        color(0, 0, 0);                                       // Change the default color (the color of the pen)
        rectangle( xmin, caseToPixel(i,ymin,ymax), xmax, caseToPixel(i+1,ymin,ymax) );

        print(xmin+10, caseToPixel(i+1,ymin,ymax)+(((ymax-ymin)/m_txt.size()) - fontsize)/2 -3,   m_txt[i].c_str());
    }
}


struct sort_pred
{
    bool operator() (std::pair<float,float> a, std::pair<float,float> b)
    {
        return ( a.first < b.first );
    }
};

void Plot::add(float x, float y, int curve_n)
{
    if (curve_n<0)
    {
        cerr<<"error==> plot_add: curve number invalid"<<endl;
        return;
    }
    if (curve_n>=m_dat.size()) m_dat.resize(curve_n+1);
    Curve& curve = m_dat[curve_n];
    if ((m_nb_plot_max<0) || (curve.size()<m_nb_plot_max))
    {
        curve.push_back( std::make_pair(x,y) );
        std::sort(curve.begin(), curve.end(), sort_pred() );
    }
    else
    {
        curve.push_back( std::make_pair(x,y) );
        std::sort(curve.begin(), curve.end(), sort_pred() );
        curve.erase( curve.begin() );
    }
}

void Plot::minMax(float& fxmin, float& fymin, float& fxmax, float& fymax, int& maxsize) const
{
    int i,j;
    if (m_dat.size()==0) return;

    fxmin = fxmax = m_dat[0][0].first;
    fymin = fymax = m_dat[0][0].second;
    maxsize = m_dat[0].size();
    for(j=0; j<m_dat.size(); ++j)
    {
        const Curve& cu = m_dat[j];
        if (cu.size()>maxsize) maxsize = cu.size();
        for(i=0; i<cu.size(); ++i)
        {
            if (cu[i].first>fxmax) fxmax = cu[i].first;
            if (cu[i].first<fxmin) fxmin = cu[i].first;
            if (cu[i].second>fymax) fymax = cu[i].second;
            if (cu[i].second<fymin) fymin = cu[i].second;
        }
    }
}

void Plot::draw( const Curve& cu, int xmin, int ymin, int xmax, int ymax, float fxmin, float fymin, float fxmax, float fymax) const
{
    int i;
    float x1, y1, x2, y2;
    if (cu.size()<2) return;
    for(i=0; i<cu.size()-1; ++i)
    {

        x1 = cu[i].first;
        y1 = cu[i].second;
        x2 = cu[i+1].first;
        y2 = cu[i+1].second;

        x1 = xmin + ((x1-fxmin)/(fxmax-fxmin)) * (xmax-xmin);
        x2 = xmin + ((x2-fxmin)/(fxmax-fxmin)) * (xmax-xmin);
        y1 = ymin + ((y1-fymin)/(fymax-fymin)) * (ymax-ymin);
        y2 = ymin + ((y2-fymin)/(fymax-fymin)) * (ymax-ymin);

        line( x1, y1, x2, y2);
    }
}

void Plot::draw(int xmin, int ymin, int xmax, int ymax, bool clearOrNot) const
{
    SDL_Color bg = Grapic::singleton().getBackgroundColor();
    SDL_Color col = Grapic::singleton().getColor();

    float fymin;
    float fymax;
    float fxmin;
    float fxmax;

    if (clearOrNot)
    {
        color( bg.r, bg.g, bg.b);
        rectangleFill( xmin, ymin, xmax, ymax);

        backgroundColor( bg.r, bg.g, bg.b);
        color(col.r, col.g, col.b);

        xmin+=25;
        ymin+=20;
        rectangle(xmin,ymin,xmax,ymax);

        //fxmin = cu[0].first;
        //fxmax = cu[ cu.size()-1 ].first;
        int maxsize;
        minMax( fxmin, fymin, fxmax, fymax, maxsize);
        if (m_nb_plot_max>0)
            fxmax = fxmin + ((fxmax-fxmin) * ((float)(m_nb_plot_max))) / maxsize;

        fontSize(11);
        print( xmin-25, ymin-5, fymin);
        print( xmin-25, ymax-15, fymax);

        print( xmin-5, ymin-20, fxmin);
        print( xmax-30, ymin-20, fxmax);
    }

    Grapic& gr = Grapic::singleton();
    SDL_Color save = gr.getColor();
    const int N = 5;
    static const SDL_Color colcu[] = { {0,255,255, 255}, {0,255,0, 255}, {0,0,255, 255}, {255,255,0, 255}, {255,0,255, 255} };
    int i;
    for(i=0; i<m_dat.size(); ++i)
    {
        color( colcu[i%N].r, colcu[i%N].g, colcu[i%N].b, colcu[i%N].a );
        draw( m_dat[i], xmin, ymin, xmax, ymax, fxmin, fymin, fxmax, fymax);
    }
    color(save.r,save.g,save.b,save.a);
}






void triangle(int x1, int y1,  int x2, int y2, int x3, int y3)
{
    line(x1, y1, x2, y2);
    line(x2, y2, x3, y3);
    line(x3, y3, x1, y1);

}



void regular_polygone(int x, int y, unsigned int apotheme, unsigned int line_number)
{
    //A polygon must have at least 3 vertices
    if(line_number < 3)
    {
        return;
    }

    for(unsigned int i = 0; i < line_number; i++)
    {
        //Simple maths to calculate the position of the points
        line( x + apotheme * cos((2.f * 3.14f *  i     ) / line_number), y + apotheme * sin((2.f * 3.14f *  i     ) / line_number),
              x + apotheme * cos((2.f * 3.14f * (i + 1)) / line_number), y + apotheme * sin((2.f * 3.14f * (i + 1)) / line_number));
    }

}



void triangleFill( int x1, int y1,
                   int x2, int y2,
                   int x3, int y3)
{
#define MAX(a, b) ((a < b) ? b : a)
#define MIN(a, b) ((a < b) ? a : b)

    //Find the maximum Y and the minimum Y in the vertices
    int topY    = MAX( MAX(y1, y2), y3) ;
    int bottomY = MIN( MIN(y1, y2), y3) ;
    //Find the x and the y corresponding to the maximum Y and the minimum Y
    int topX    = (topY    == y1) ? x1 : ( (topY    == y2) ? x2 : x3 ) ;
    int bottomX = (bottomY == y1) ? x1 : ( (bottomY == y2) ? x2 : x3 ) ;

    //Get the last vertex
    int lastX(0), lastY(0);

    if(topX == x1 && topY == y1)
    {
        if(bottomX == x2 && bottomY == y2)
        {
            lastX = x3;
            lastY = y3;
        }
        else
        {
            lastX = x2;
            lastY = y2;
        }
    }
    else if (topX == x2 && topY == y2)
    {
        if(bottomX == x1 && bottomY == y1)
        {
            lastX = x3;
            lastY = y3;
        }
        else
        {
            lastX = x1;
            lastY = y1;
        }
    }
    else if (topX == x3 && topY == y3)
    {
        if(bottomX == x1 && bottomY == y1)
        {
            lastX = x2;
            lastY = y2;
        }
        else
        {
            lastX = x1;
            lastY = y1;
        }
    }
    else
    {
        printf("ERROR : No matching vertex for TOP");
        return;
    }
    //Calculate the linear equations of each segments
    //Prevent from dividing by 0
    float delta = bottomX - topX;
    if(delta == 0)
    {
        delta = 0.0001f;
    }

    const float A = float( bottomY - topY ) / delta;
    const float B = float( bottomX * topY - topX * bottomY ) / delta;

    //Prevent from dividing by 0
    delta = lastX - topX;
    if(delta == 0)
    {
        delta = 0.0001f;
    }

    const float C = float( lastY - topY ) / delta;
    const float D = float( lastX * topY - topX * lastY ) / delta;

    //Prevent from dividing by 0
    delta = lastX - bottomX;
    if(delta == 0)
    {
        delta = 0.0001f;
    }

    const float E = float( lastY - bottomY ) / delta;
    const float F = float( lastX * bottomY - bottomX * lastY ) / delta;

    for( int i = bottomY; i < topY; i++ )
    {
        //Draw lines which are parallel to the x-axis for each pixel on the "main" line delimited by (topX, topY) and (bottomX, bottomY)
        int finalX = ( i >= lastY ) ? ( i - D ) / C : ( i - F ) / E;

        grapic::line(( i - B ) / A, i, finalX, i);
    }
}


void ellipse(int xc, int yc, int h, int v)
{
    Grapic& g = Grapic::singleton();
    Uint32 cc = *((Uint32*)(&g.getColor()));
    Uint8 *c = (Uint8 *)&cc;
    color(c[0], c[1], c[2]);

    aaellipseRGBA(g.renderer(), xc, g.inverseY(yc), h, v, c[0], c[1], c[2], c[3]);
}


void ellipseFill(int xc, int yc, int h, int v)
{
    Grapic& g = Grapic::singleton();
    Uint32 cc = *((Uint32*)(&g.getColor()));
    Uint8 *c = (Uint8 *)&cc;

    filledEllipseRGBA(g.renderer(), xc, g.inverseY(yc), h, v, c[0], c[1], c[2], c[3]);
}


void regular_polygonFill(int x, int y, unsigned int apotheme, unsigned int line_number)
{
    const int MAX_TRIANGLE = 30;

    //A polygon must have at least 3 vertices
    if( line_number < 3)
    {
        return;
    }
    //Draw one triangle is faster than 3
    if( line_number == 3)
    {
        triangleFill(x + apotheme, y,
                     x - apotheme * 0.5, y + apotheme * 0.866,
                     x - apotheme * 0.5, y - apotheme * 0.866);
        return;
    }

    //Above MAX_TRIANGLE vertices the polygon becomes a circle
    if(line_number >= MAX_TRIANGLE)
    {
        circleFill(x, y, apotheme);
        return;
    }

    for(unsigned int i = 0; i < line_number; i++)
    {
        triangleFill(x, y,                                                                                                                  //Point 1 (centre)
                     x + apotheme * cos((2.f * 3.14f *  i     ) / line_number), y + apotheme * sin((2.f * 3.14f *  i     ) / line_number),  //Point 2
                     x + apotheme * cos((2.f * 3.14f * (i + 1)) / line_number), y + apotheme * sin((2.f * 3.14f * (i + 1)) / line_number)); //Point 3
    }
}



bool isInTriangle(float px, float py, float ax, float ay, float bx, float by, float cx, float cy)
{
    //From PolyK.js (http://polyk.ivank.net/)

    float v0x = cx-ax;
    float v0y = cy-ay;
    float v1x = bx-ax;
    float v1y = by-ay;
    float v2x = px-ax;
    float v2y = py-ay;

    float dot00 = v0x*v0x+v0y*v0y;
    float dot01 = v0x*v1x+v0y*v1y;
    float dot02 = v0x*v2x+v0y*v2y;
    float dot11 = v1x*v1x+v1y*v1y;
    float dot12 = v1x*v2x+v1y*v2y;

    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v < 1);
}


void polygonFill(int p[][2], unsigned int number)
{
    //Triangulation Algorithm from PolyK.js (http://polyk.ivank.net/)
    int n = number;
    if (n<3) return;

    std::vector<int> tgs;
    std::vector<int> avl;

    for(int i = 0; i < n; i++)
    {
        avl.push_back(i);
    }

    int i = 0;
    int al = n;

    while(al > 3)
    {

        int i0 = avl[(i + 0)%al];
        int i1 = avl[(i + 1)%al];
        int i2 = avl[(i + 2)%al];

        int ax = p[i0][0];
        int ay = p[i0][1];
        int bx = p[i1][0];
        int by = p[i1][1];
        int cx = p[i2][0];
        int cy = p[i2][1];

        bool earFound = false;
        if((ay-by)*(cx-bx) + (bx-ax)*(cy-by) >= 0)
        {

            earFound = true;

            for(int j = 0; j < al && earFound == true; j++)
            {
                int vi = avl[j];

                if(vi != i0 && vi != i1 &&    vi != i2)
                {
                    if(isInTriangle(p[vi][0], p[ vi][1], ax, ay, bx, by, cx, cy))
                    {
                        earFound = false;
                    }
                }
            }
        }

        if(earFound)
        {
            tgs.push_back(i0);
            tgs.push_back(i1);
            tgs.push_back(i2);

            avl.erase(avl.begin() + ((i + 1) % al));
            al --;
            i = 0;
        }
        else if (i++ > 3 * al)
        {
            break;
        }
    }

    tgs.push_back(avl[0]);
    tgs.push_back(avl[1]);
    tgs.push_back(avl[2]);

    //Draw every triangle from the result of the triangulation
    int x1, y1;
    int x2, y2;
    int x3, y3;

    for(int k = 0; k < tgs.size(); k++)
    {
        if(k % 3 == 0)
        {
            if(k != 0)
            {
                triangleFill(x1, y1, x2, y2, x3, y3);
            }

            x1 = p[tgs[k]][0];
            y1 = p[tgs[k]][1];
        }
        else if(k % 3 == 1)
        {
            x2 = p[tgs[k]][0];
            y2 = p[tgs[k]][1];
        }
        else if(k % 3 == 2)
        {
            x3 = p[tgs[k]][0];
            y3 = p[tgs[k]][1];
        }
    }

    //Draw the last triangle
    triangleFill(x1, y1, x2, y2, x3, y3);

}


void polygon(int p[][2], unsigned int number)
{
    for(int i = 0; i < number; i++)
    {
        grapic::line(p[i % number][0], p[i % number][1], p[(i + 1) % number][0], p[(i + 1)% number][1]);
    }
}

void winClearEvent()
{
    grapic::Grapic::singleton().clearEvent();
}

void delay(int d)
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(d));
    SDL_Delay(d);
}


Image image(const char* filename, bool transparency, unsigned char r, unsigned char g, unsigned b, unsigned char a)
{
    return Image(filename, transparency, r, g, b, a);
}

Image image(int w, int h)
{
    return Image(w,h);
}
void image_savePNG(const Image& im, const char* filename)
{
    im.savePNG(filename);
}

void image_draw(Image& im, int x, int y, int w, int h)
{
    im.draw(x,y,w,h);
}

void image_draw(Image& im, int x, int y, int w, int h, float angle, float flip)
{
    im.draw(x,y,w,h,angle,flip);
}

unsigned char image_get(const Image& im, int x, int y, int c)
{
    return im.get(x,y,c);
}

int image_width(const Image& im)
{
    return im.surface()->w;
}

int image_height(const Image& im)
{
    return im.surface()->h;
}

bool image_isInit(const Image& im)
{
    return im.isInit();
}

void image_printInfo(const Image& im)
{
    im.printInfo();
}

void menu_add(Menu& m, const std::string& str)
{
    m.add(str);
}

void menu_change(Menu& m, int i, const std::string& str)
{
    m.change(i,str);
}

void menu_draw(Menu& m, int xmin, int ymin, int xmax, int ymax)
{
    m.draw(xmin,ymin,xmax,ymax);
}

int menu_select(const Menu& m)
{
    return m.select();
}

void menu_setSelect(Menu& m, int s)
{
    m.setSelect(s);
}

int caseToPixel(const Menu& m, int c, int ymin, int ymax)
{
    return m.caseToPixel(c,ymin,ymax);
}


void plot_clear(Plot& p )
{
    p.clear();
}


void plot_setSize(Plot& p, const int n)
{
    p.setSize(n);
}


void plot_add(Plot& p, float x, float y, int curve_n)
{
    p.add(x,y,curve_n);
}

void plot_draw( const Plot& p, int xmin, int ymin, int xmax, int ymax, bool clearOrNot)
{
    p.draw(xmin,ymin,xmax,ymax,clearOrNot);
}


} // namespace
