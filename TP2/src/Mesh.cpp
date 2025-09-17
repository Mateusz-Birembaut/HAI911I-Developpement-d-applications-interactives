#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <cmath>

void Mesh::loadOFF (const std::string & filename) {
    std::ifstream in (filename.c_str ());
    if (!in)
        exit (EXIT_FAILURE);
    std::string offString;
    unsigned int sizeV, sizeT, tmp;
    in >> offString >> sizeV >> sizeT >> tmp;
    V.resize (sizeV);
    T.resize (sizeT);
    for (unsigned int i = 0; i < sizeV; i++)
        in >> V[i].p;
    int s;
    for (unsigned int i = 0; i < sizeT; i++) {
        in >> s;
        for (unsigned int j = 0; j < 3; j++)
            in >> T[i].v[j];
    }
    in.close ();
    recomputeNormals ();
}

void Mesh::recomputeNormals () {
    for (unsigned int i = 0; i < V.size (); i++)
        V[i].n = Vec3 (0.0, 0.0, 0.0);
    for (unsigned int i = 0; i < T.size (); i++) {
        Vec3 e01 = V[T[i].v[1]].p -  V[T[i].v[0]].p;
        Vec3 e02 = V[T[i].v[2]].p -  V[T[i].v[0]].p;
        Vec3 n = Vec3::cross (e01, e02);
        n.normalize ();
        for (unsigned int j = 0; j < 3; j++)
            V[T[i].v[j]].n += n;
    }
    for (unsigned int i = 0; i < V.size (); i++)
        V[i].n.normalize ();
}

//https://iquilezles.org/articles/distfunctions/
double sdCapsule( Vec3 p, Vec3 a, Vec3 b, double r )
{
  Vec3 pa = p - a;
  Vec3 ba = b - a;
  float h = Vec3::dot(pa,ba)/Vec3::dot(ba,ba);
  h = std::fmax(0.0, std::fmin(1.0, h));
  return  (pa - h*ba).length() - r;
}

void Mesh::compute_skinning_weights( Skeleton & skeleton ) {
    //---------------------------------------------------//
    //---------------------------------------------------//

    // Indications:
    // you should compute weights for each vertex w.r.t. the skeleton bones
    // so each vertex will have B weights (B = number of bones)
    // these weights shoud be stored in vertex.w:
    const double n {20.0};
    // code to change :
    for(auto& vertex : V){
        double wsum {0.0};
        for(size_t i=0; i < skeleton.bones.size(); ++i){
            Articulation & a0 {skeleton.articulations[ skeleton.bones[i].joints[0] ]};
            Articulation & a1 {skeleton.articulations[ skeleton.bones[i].joints[1] ]};
            double dist {sdCapsule(vertex.p, a0.p, a1.p, 0.0)};
            double w { pow(1/dist,n) };
            //if(w > 0){
                vertex.w.push_back(w);
                wsum += w;
            //}
        }

        if(wsum > 0.0){
            for(size_t i=0; i < skeleton.bones.size(); ++i){
                vertex.w[i] /= wsum;
            }
        }
    }
}



void Mesh::draw( int displayedBone ) const {

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glBegin (GL_TRIANGLES);
    for (unsigned int i = 0; i < T.size (); i++)
        for (unsigned int j = 0; j < 3; j++) {
            const MeshVertex & v = V[T[i].v[j]];
            if( displayedBone >= 0 && displayedBone < v.w.size() ) {
                Vec3 rgb = HSVtoRGB( v.w[displayedBone], 0.8,0.8 );
                glColor3f( rgb[0], rgb[1], rgb[2] );
            } else glColor3f( 0.6, 0.6, 0.6 );
            glNormal3f (v.n[0], v.n[1], v.n[2]);
            glVertex3f (v.p[0], v.p[1], v.p[2]);
        }

    glEnd ();
}



void Mesh::drawTransformedMesh( SkeletonTransformation & transfo ) const {


    std::vector< Vec3 > new_positions( V.size() );
    std::vector< Vec3 > new_normals( V.size() );

    //---------------------------------------------------//
    //---------------------------------------------------//
    // code to change :
    for( unsigned int i = 0 ; i < V.size() ; ++i ) {
        Vec3 p = V[i].p;
        Vec3 n = V[i].n;

        Vec3 newPos{0.0f, 0.0f, 0.0f};
        Mat3 transfoMat{};
        // Indications:
        // you should use the skinning weights to blend the transformations of the vertex position by the bones.
        for(int j=0; j < V[i].w.size(); ++j){
            transfoMat += ( V[i].w[j] * transfo.bone_transformations[j].world_space_rotation );
            newPos += V[i].w[j] * (transfo.bone_transformations[j].world_space_rotation * p + transfo.bone_transformations[j].world_space_translation);
        }

        new_positions[ i ] = newPos;
        new_normals[ i ] =  Mat3::inverse(transfoMat).getTranspose() * n ;
        new_normals[ i ].normalize();
    }
    //---------------------------------------------------//
    //---------------------------------------------------//
    //---------------------------------------------------//

    glEnable(GL_LIGHTING);
    glBegin (GL_TRIANGLES);
    for (unsigned int i = 0; i < T.size (); i++)
        for (unsigned int j = 0; j < 3; j++) {
            const MeshVertex & v = V[T[i].v[j]];
            Vec3 p = new_positions[ T[i].v[j] ];
            Vec3 n = new_normals[ T[i].v[j] ];
            glColor3f( 0.6, 0.6, 0.6 );
            glNormal3f (n[0], n[1], n[2]);
            glVertex3f (p[0], p[1], p[2]);
        }
    glEnd ();
}
/*! \brief Convert HSV to RGB color space

  Converts a given set of HSV values `h', `s', `v' into RGB
  coordinates. The output RGB values are in the range [0, 1], and
  the input HSV values are in the ranges h = [0, 360], and s, v =
  [0, 1], respectively.

  \param fH Hue component, used as input, range: [0, 1]
  \param fS Hue component, used as input, range: [0, 1]
  \param fV Hue component, used as input, range: [0, 1]

  \param fR Red component, used as output, range: [0, 1]
  \param fG Green component, used as output, range: [0, 1]
  \param fB Blue component, used as output, range: [0, 1]

*/
Vec3 Mesh::HSVtoRGB( float fH, float fS, float fV) const {

    fH=(1.-fH)*0.65*360.;

    float fR, fG, fB;
    float fC = fV * fS; // Chroma
    float fHPrime = fmod(fH / 60.0, 6);
    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = fV - fC;

    if(0 <= fHPrime && fHPrime < 1) {
        fR = fC;
        fG = fX;
        fB = 0;
    } else if(1 <= fHPrime && fHPrime < 2) {
        fR = fX;
        fG = fC;
        fB = 0;
    } else if(2 <= fHPrime && fHPrime < 3) {
        fR = 0;
        fG = fC;
        fB = fX;
    } else if(3 <= fHPrime && fHPrime < 4) {
        fR = 0;
        fG = fX;
        fB = fC;
    } else if(4 <= fHPrime && fHPrime < 5) {
        fR = fX;
        fG = 0;
        fB = fC;
    } else if(5 <= fHPrime && fHPrime < 6) {
        fR = fC;
        fG = 0;
        fB = fX;
    } else {
        fR = 0;
        fG = 0;
        fB = 0;
    }

    fR += fM;
    fG += fM;
    fB += fM;
    return Vec3(fR,fG,fB);
}
