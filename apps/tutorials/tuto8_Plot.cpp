#include <Grapic.h>
using namespace grapic;

const int WIN_DIM_X = 600;
const int WIN_DIM_Y = 500;

int main(int , char** )
{
    winInit("Demo", WIN_DIM_X, WIN_DIM_Y);

    Plot p1, p2;

    // Plot titles
    plot_title(p1, "First graph");
    plot_title(p2, "Second graph");
    // Define where plot are drawn
    plot_area(p1, 20,               20, WIN_DIM_X - 20, WIN_DIM_Y/2 - 20 );
    plot_area(p2, 20, WIN_DIM_Y/2 + 20, WIN_DIM_X - 20, WIN_DIM_Y   - 20);

    // Axis options     
    plot_yaxisTitle(p1, "Time"); // Set yaxis title to "Time"
    plot_xaxisType (p2, "log");  // Set xaxis of p2 to be in log-scaled

    // Curve options
    // Curve names
    plot_curveName(p1, 0, "cos"); // curve 0 of p1 will be labeled "cos"
    plot_curveName(p1, 1, "sin"); // curve 1 of p1 will be labeled "sin"
    plot_curveName(p2, 0, "cos2"); 
    plot_curveName(p2, 1, "sin*0.5");
    
    // Curve types
    plot_curveType(p2, 1, "scatter"); // Only points
    
    // Location
    plot_legendLocation(p1, "botright");     // Bot right
    plot_legendLocation(p2, "centercenter"); // Center of the plot

    // Legend title
    plot_legendTitle(p1, "Curves");
    plot_legendTitle(p2, "Points");

    float x, y, y2;
    x = 0.f;

    bool stop = false;

    while( !stop )
    {
        winClear();

        x += 0.1f;
        y = cos(x);
        y2 = 0.5 * sin(x);

        // Add data to plot
        plot_add(p1, x,  y, 0);
        plot_add(p1, x, y2, 1);
        plot_add(p2, x,  y, 0);
        plot_add(p2, x, y2, 1);

        // Draw plots
        plot_draw(p1);
        plot_draw(p2);

        stop = winDisplay();    
    }
	return 0;
}
