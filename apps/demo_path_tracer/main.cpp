#include <iostream>
#include <chrono>
#include <math.h>   
#include <stdlib.h> 
#include <stdio.h>  

#include <Grapic.h>
using namespace grapic;

// Smallpt + Grapic = GrapicPT 
// Source code original:
// smallpt: Global Illumination in 99 lines of C++
// A Path Tracer by Kevin Beason, 2008
// cf: https://www.kevinbeason.com/smallpt/ 

// Structure Vec
// Est utilisée pour les positions dans l'espace (x,y,z), les vecteurs (x,y,z) et pour les couleurs (r,g,b)
struct Vec 
{        
    double x, y, z;                  
    Vec(double x_=0, double y_=0, double z_=0){ x=x_; y=y_; z=z_; } 
    Vec operator+(const Vec &b) const { return Vec(x+b.x,y+b.y,z+b.z); } 
    Vec operator-(const Vec &b) const { return Vec(x-b.x,y-b.y,z-b.z); } 
    Vec operator*(double b) const { return Vec(x*b,y*b,z*b); } 
    Vec mult(const Vec &b) const { return Vec(x*b.x,y*b.y,z*b.z); } 
    Vec& norm(){ return *this = *this * (1/sqrt(x*x+y*y+z*z)); }            // normaliser
    double dot(const Vec &b) const { return x*b.x+y*b.y+z*b.z; }            // produit scalaire
    Vec operator%(Vec&b){return Vec(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);}  // produit vectoriel  
}; 

// Rayon (origine, direction)
struct Ray 
{ 
    Vec o, d; 
    Ray(Vec o_, Vec d_) : o(o_), d(d_) {} 
};

// Types de matériaux
enum Refl_t { DIFF, SPEC, REFR };  

// Sphere (rayon, position, emission, couleur, materiau)
struct Sphere 
{ 
    double rad;       // rayon 
    Vec p, e, c;      // position, emission, couleur 
    Refl_t refl;      // materiau (DIFFuse, SPECular, REFRactive) 

    // Sphere(rayon, position, emission, couleur, mteriau)
    Sphere(double rad_, Vec p_, Vec e_, Vec c_, Refl_t refl_): rad(rad_), p(p_), e(e_), c(c_), refl(refl_) {} 

    // Intersection rayon / sphere
    // Retourne la distance sur le rayon si une intersection est trouvée, 0 sinon 
    double intersect(const Ray &r) const 
    { 
        Vec op = p-r.o; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0 
        double t, eps=1e-4, b=op.dot(r.d), det=b*b-op.dot(op)+rad*rad; 
        if (det<0) 
            return 0; 
        else 
            det=sqrt(det); 
        return (t=b-det)>eps ? t : ((t=b+det)>eps ? t : 0); 
    } 
}; 

// Description de la scene, composée uniquement de sphères: Sphere(rayon, position, emission, couleur, mteriau)
Sphere spheres[] = 
{
    Sphere(1e5, Vec( 1e5+1,40.8,81.6), Vec(),Vec(.75,.25,.25),DIFF),    // Mur gauche
    Sphere(1e5, Vec(-1e5+99,40.8,81.6),Vec(),Vec(.25,.25,.75),DIFF),    // Mur droit
    Sphere(1e5, Vec(50,40.8, 1e5),     Vec(),Vec(.75,.75,.75),DIFF),    // Mur du fond
    Sphere(1e5, Vec(50,40.8,-1e5+170), Vec(),Vec(),           DIFF),    // Frnt
    Sphere(1e5, Vec(50, 1e5, 81.6),    Vec(),Vec(.75,.75,.75),DIFF),    // Sol
    Sphere(1e5, Vec(50,-1e5+81.6,81.6),Vec(),Vec(.75,.75,.75),DIFF),    // Plafond
    Sphere(16.5,Vec(27,16.5,47),       Vec(),Vec(1,1,1)*.999, SPEC),    // Sphere miroir
    Sphere(16.5,Vec(73,16.5,78),       Vec(),Vec(1,1,1)*.999, REFR),    // Sphere verre
    Sphere(1.5*5, Vec(50,81.6-16.5,81.6),Vec(4,4,4)*8,  Vec(), DIFF),   // Source 1
    // Sphere(1.5, Vec(75,16.5,59)       ,Vec(4,4,4)*100,  Vec(), DIFF),   // Source 2
};
int numSpheres = sizeof(spheres)/sizeof(Sphere);

// Intersection entre un rayon et la scene (boucle sur toutes les spheres)
// Renvoie la distance t d'intersection la plus proche et l'indice de l'objet intersecté
inline bool intersect(const Ray &r, double &t, int &id)
{ 
    static const double inf = 1e20;
    t = inf;
    double d;
    for(int i=numSpheres; i >= 0; i--) 
    {
        d = spheres[i].intersect(r);
        if( d > 0 && d < t )
        {
            t = d;
            id = i;
        } 
    }
    return t < inf; 
} 

// Construit et evalue un chemin
Vec radiance(Ray r)
{
    double t;       // Distance d'intersection
    int id=0;       // id de l'objet intersecté
    int E=1;        // energie des sources trouvée par hasard (sans echantillonnage explicite des sources)
    int depth = 0;  // longueur du chemin (nombre de rebond sur la scene)
    Vec cl(0,0,0);  // contribution du chemin (couleur ajoutée au pixel) 
    Vec cf(1,1,1);  // energie porté par le chemin

    // Construction iterative des chemins
    while (1)
    {   
        // Si on ne trouve pas d'intersection, on a terminé le chemin
        if(!intersect(r, t, id)) 
            break;

        // Recuperation des infos de l'objet intersecté
        const Sphere &obj = spheres[id];        
        Vec x = r.o + r.d * t; // Position
        Vec n = (x-obj.p).norm(); // Normale
        Vec nl = n.dot(r.d) < 0 ? n : n * -1;
        Vec f = obj.c; // Couleur
        
        // On ajoute les emission
        cl = cl + cf.mult(obj.e * E);

        // On peut reactiver les contribution directes si elles ont été desactivées au rebond diffus precedent.
        // Mais cela ajoute de la variance qui se manifeste comme du bruit dans l'image au niveau des caustiques.
        // On peut choisir de ne pas reactiver les sources directes apres un premier rebond diffus (pas de chemins ES*DS*L)
        // cela biaise le rendu final mais reduit considerablement le bruit (pas de caustiques).
        // if( E == 0 )
            // E = 1;
        
        // Roulette russe, 
        // On continue le chemin avec une probabilité P  
        // On arrete le chemin avec une probabilité 1 - P 
        // Ici on prend P comme etant le max de la reflectance de l'objet (on pourrait prendre un autre critère...)
        double p = f.x>f.y && f.x>f.z ? f.x : f.y>f.z ? f.y : f.z;
        if(++depth>5)
        {
            if (drand48()<p) 
                f=f*(1/p); 
            else
                break;
        }

        // On met a jour l'energie portée par le chemin
        cf = cf.mult(f);
        
        // Reflexion diffuse
        if(obj.refl == DIFF)
        {                  
            // Echantillonnage des sources de lumiere spheriques
            // On tire des directions qui pointent directement sur les sources
            // Cela permet au rendu de converger plus rapidement
            // car les contributions lumineuses sont trouvées plus frequemment qu'en 
            // tirant les rayon aleatoirement dans la scene.
            Vec e;
            double numLights = 0;
            for (int i=0; i<numSpheres; i++)
            {
                const Sphere &s = spheres[i];
                // On passe les sphere qui ne sont pas des sources
                if(s.e.x<=0 && s.e.y<=0 && s.e.z<=0) 
                    continue; 
                // Echantillonnage par importance des sources spheriques
                // Pour les details : http://www.pbr-book.org/3ed-2018/Light_Transport_I_Surface_Reflection/Sampling_Light_Sources.html#x2-SamplingSpheres
                numLights++;
                Vec sw=s.p-x, su=((fabs(sw.x)>.1?Vec(0,1):Vec(1))%sw).norm(), sv=sw%su;
                double cos_a_max = sqrt(1-s.rad*s.rad/(x-s.p).dot(x-s.p));
                double eps1 = drand48(), eps2 = drand48();
                double cos_a = 1-eps1+eps1*cos_a_max;
                double sin_a = sqrt(1-cos_a*cos_a);
                double phi = 2*M_PI*eps2;
                Vec l = su*cos(phi)*sin_a + sv*sin(phi)*sin_a + sw*cos_a;
                l.norm();
                // Test de visiblité entre la source et le point actuel 
                if (intersect(Ray(x,l), t, id) && id==i)
                {  
                    double omega = 2*M_PI*(1-cos_a_max);
                    e = e + cf.mult(s.e*l.dot(nl)*omega)*M_1_PI;  // 1/pi for brdf
                }
            }

            // Ajout de la contribution des sources à la couleur finale
            cl = cl + e * (1.0 / numLights);
            
            // On desactive la contribution des sources trouvées au prochain rebond pour 
            // ne pas accumuler plusieurs fois l'energie des chemins de longueur d+1  
            E = 0; 

            // Une surface diffuse reflete de la lumiere venant de toutes les directions,
            // donc on tire une direction uniforme sur l'hemisphere pour continuer le chemin
            double r1= 2 * M_PI * drand48(), r2 = drand48(), r2s = sqrt(r2);
            Vec w=nl, u=((fabs(w.x)>.1?Vec(0,1):Vec(1))%w).norm(), v=w%u;
            Vec d = (u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2)).norm();
            
            // Prochain rayon
            r = Ray(x,d);
            continue;
        } 
        
        // Reflexion parfaite (miroir)
        Ray reflRay(x, r.d - n * 2 * n.dot(r.d)); 
        if(obj.refl == SPEC)
        {   
            r = reflRay;
            continue;
        }

        // Transmission dielectrique parfaite (refraction)
        // Rayon de dehors vers dedans ? ou l'inverse ?
        bool into = n.dot(nl)>0;       
        // Calcul de l'angle de refraction
        double nc = 1;                      // indice de refraction exterieur (1 => air)
        double nt = 1.5;                    // indice de refraction interieur (1.5 => verre)
        double nnt = into ? nc/nt : nt/nc;  // ratio des indices de refraction (eta)
        double ddn = r.d.dot(nl);           
        double cos2t;                       // cosinus de l'angle de refraction au carré
        // Reflexion total interne: l'angle du rayon refracté est tel que le rayon 
        // resterait bloqué a l'interieur de l'objet indefiniment.
        // On choisit donc de faire une reflexion parfaite
        if((cos2t=1-nnt*nnt*(1-ddn*ddn))<0)
        {    
            r = reflRay;
            continue;
        }
        // Direction refractée       
        Vec tdir = (r.d*nnt-n*((into?1:-1)*(ddn*nnt+sqrt(cos2t)))).norm();
        // Calcul du terme de Fresnel et probabilités de reflexion / transmission selon l'angle incident
        double a=nt-nc, b=nt+nc, R0=a*a/(b*b), c = 1-(into?-ddn:tdir.dot(n));
        double Re=R0+(1-R0)*c*c*c*c*c,Tr=1-Re,P=.25+.5*Re,RP=Re/P,TP=Tr/(1-P);
        // Selection aleatoire (Roulette Russe) entre reflexion et transmission dielectrique
        if( drand48() < P )
        {
            // Reflexion
            cf = cf * RP;
            r = reflRay;
        } 
        else 
        {
            // Transmission
            cf = cf * TP;
            r = Ray(x, tdir);
        }

        continue;
    }
    return cl;
}

int main(int argc, char * argv[])
{   
    srand(time(NULL));
    // window
    int w = 800, h = 800, n = w * h; 
    winInit("Path tracer", w, h);
    winClear();

    // image
    Image image(w, h);
    for(int x = 0; x < w; ++x)
        for(int y = 0; y < h; ++y)
            image_set(image, x, y, 0, 0, 0, 255);

    // Image HDR 
    std::vector<double> image_double(w * h * 4, 0.f);

    // Camera
    Vec from = Vec(50,52,295.6);
    Vec camd = Vec(0,-0.042612,-1).norm();
    Vec to = from + camd * 100;
    Ray cam(Vec(50,52,295.6), Vec(0,-0.042612,-1).norm()); // cam pos, dir 
    Vec cx=Vec(w*.5135/h), cy=(cx%cam.d).norm()*.5135; 

    // Progression
    unsigned int batch = h * 4;
    unsigned int loop = 0;
    auto start = std::chrono::system_clock::now();
    unsigned int perm = lrand48();
    // loop
    while(true)
    {
        // Bouger la camera
        if(isMousePressed(SDL_BUTTON_LEFT))
        {
            // Recuperer la position de la souris et les distances relatives au centre de la fenetre (dx, dy)
            int x, y;
            mousePos(x, y);
            double dx = 2.0 * x / double(h - 1) - 1.0; 
            double dy = 2.0 * y / double(w - 1) - 1.0; 
            
            // Deplacer le point visé par la camera en x et en Y
            Vec newto = Vec(to.x + dx * 20, to.y + dy * 20, to.z);
            Vec newcamd = (newto-from).norm();

            // Reinitialiser les parametres camera
            cam = Ray(from, newcamd);
            Vec cx=Vec(w*.5135/h), cy=(cx%cam.d).norm()*.5135;
            
            // Remettre les images et les parametres du rendu à 0 
            std::fill(image_double.begin(), image_double.end(), 0.);
            for(int i = 0; i < w; i++)
                for(int j = 0; j < h; j++)
                    image_set(image, i, j, 0, 0, 0, 255);
            
            loop = 0;
            start = std::chrono::system_clock::now();
            perm = lrand48();
        }

        // Lancer de rayons
        #pragma omp parallel for
        for(int i = 0; i < batch; ++i)
        {
            // Indice du pixel à raffiner et coordonnées dans l'image
            unsigned int pix = (loop * batch + i)%n;
            int x = pix % w;
            int y = pix / w;

            // Rayon camera a travers le pixel (x, y) plus une perturbation aleatoire
            Vec d = cx*( (x + drand48() - 0.5f)/w - .5) + cy*( (y + drand48() - 0.5f)/h - .5) + cam.d;

            // Lance le rayon et recupere la contribution du chemin echantillonné
            // Camera rays are pushed ^^^^^ forward to start in interior 
            Vec contribution = radiance(Ray(cam.o+d*140,d.norm()));

            // Accumulation des echantillons : moyenne progressive ~ calcule la moyenne
            image_double[4*pix+3] ++;
            image_double[4*pix+0] += (contribution.x - image_double[4*pix+0]) / image_double[4*pix+3];
            image_double[4*pix+1] += (contribution.y - image_double[4*pix+1]) / image_double[4*pix+3];
            image_double[4*pix+2] += (contribution.z - image_double[4*pix+2]) / image_double[4*pix+3];

            // Mise a jour de l'image
            unsigned char r = std::min(255., std::max(0., std::sqrt(image_double[4*pix+0]) * 255.));
            unsigned char g = std::min(255., std::max(0., std::sqrt(image_double[4*pix+1]) * 255.));
            unsigned char b = std::min(255., std::max(0., std::sqrt(image_double[4*pix+2]) * 255.));
            image_set(image, x, y, r, g, b, 255);
        }

        // Affichage de l'image dans la fenetre Grapic
        image_draw(image, 0, 0, w, h);

        // Mise a jour de la fenetre + quitte si besoin
        if( winDisplay() ) break;

        // Mise a jour de la progression et affichage du nombre de rayon / temps de calcul
        ++loop;
        if( (loop*batch)%n == 0 )
        {
            auto now = std::chrono::system_clock::now();
            unsigned int sec = std::chrono::duration_cast<std::chrono::seconds>(now-start).count();
            perm = lrand48();
            printf("rays %u time %u (s)\n",loop*h, sec);
        }
    }
    // Sauvegarde de l'image résultat
    image_savePNG(image, argc > 1 ? argv[1] : "image.png");
    winQuit();
    return 0;
}