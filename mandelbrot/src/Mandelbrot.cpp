#include "Mandelbrot.hpp"

#include <cmath>

#include "constants.hpp"

#include "WindowSurface.hpp"
#include "Pair.hpp"

const long double LOG2 = std::log(2);

Mandelbrot::Mandelbrot(SDL::Window& window, Complex origin, double zoom, unsigned maxIteration)
    : m_renderer(window, SDL::RendererAccelerated),
      m_format(nullptr),
      m_zoom(zoom),
      m_center(origin),
      m_width(0),
      m_height(0),
      m_maxIteration (maxIteration)
{
    SDL::Pair<int> windowSize = window.getSize();

    m_width  = windowSize.getLeftValue();
    m_height = windowSize.getRightValue();

    SDL::WindowSurface windowSurface(window);

    m_format = windowSurface.toSDL()->format;
}

Mandelbrot::~Mandelbrot()
{

}


long double Mandelbrot::getZoom() const
{
    return m_zoom;
}

long double Mandelbrot::getEpsilon() const
{
    if (m_zoom == 0)
        return -1;
    return 1/m_zoom;
}

Complex Mandelbrot::getCenter() const
{
    return m_center;
}

double Mandelbrot::getWidth() const
{
    return m_width;
}

double Mandelbrot::getHeight() const
{
    return m_height;
}

long double Mandelbrot::iterate(Complex c) const
{

    Complex z(c);

    unsigned i(0);

    while (std::abs(z) <= 2 && i < m_maxIteration)
    {
        z = z*z + c;
        ++i;
    }

    if (std::abs(z) <= 2)
        return -1;

    return static_cast<long double>((i + 1 - (std::log(std::log(std::abs(z))) / LOG2) ) / m_maxIteration);
}

Mandelbrot& Mandelbrot::zoomIn(long double factor)
{
    if (factor != 0)
        m_zoom *= factor;
    return *this;
}

Mandelbrot& Mandelbrot::moveFrom(Complex c)
{
    m_center += c;
    return *this;
}

Mandelbrot& Mandelbrot::moveTo(Complex c)
{
    m_center = c;
    return *this;
}

Complex Mandelbrot::screenToCartesian(unsigned x, unsigned y) const
{
    Complex screenPosition(x - m_width/2 , m_height/2 - y);
    screenPosition += m_zoom * m_center;
    screenPosition *= this->getEpsilon();

    return screenPosition;
}

Mandelbrot& Mandelbrot::renderPixel(unsigned x, unsigned y, long double color)
{ 

    if (color == -1)
    {
        m_renderer.setDrawColor(SDL::Color::fromRGB(0, 0, 0)).drawPoint(x, y);
    }
    else
    {
        int smooth = static_cast<int>(color*923) % 924;

        SDL::Color rgbColor(COLORS[smooth][0], COLORS[smooth][1], COLORS[smooth][2]);

        m_renderer.setDrawColor(rgbColor).drawPoint(x, y);
    }

    return *this;
}

Mandelbrot& Mandelbrot::renderImage()
{
    m_renderer.clear();

    for (unsigned x=0; x<m_width; ++x)
    { 
        for (unsigned y=0; y<m_height; ++y)
        {
            this->renderPixel(x, y, iterate(screenToCartesian(x, y)));
        }
    }

    m_renderer.present();

    return *this;
}
