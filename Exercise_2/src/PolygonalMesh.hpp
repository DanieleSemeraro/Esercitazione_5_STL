#pragma once
#include <iostream>
#include "Eigen/Eigen"
using namespace std;
using namespace Eigen;

namespace PolygonalLibrary {

struct PolygonalMesh
{
    unsigned int NumberCell0D = 0; ///< number of Cell0D
    std::vector<unsigned int> Cell0DId = {}; ///< Cell0D id, size 1 x NumberCell0D
    std::vector<Vector2d> Cell0DCoordinates = {}; ///< Cell0D coordinates, size 2 x NumberCell0D (x,y) le coordinate sono double
    std::map<unsigned int, list<unsigned int>> Cell0DMarkers = {}; ///< Cell0D markers, size 1 x NumberCell0D (marker)
//map assegna ad ogni chiave che corrisponde al marker un insieme di id che corrispondono ai vertici con quel marker

    unsigned int NumberCell1D = 0; ///< number of Cell1D cioè lati
    std::vector<unsigned int> Cell1DId = {}; ///< Cell1D id, size 1 x NumberCell1D
    std::vector<Vector2i> Cell1DVertices = {}; ///< Cell1D vertices indices, size 2 x NumberCell1D (fromId,toId) gli id dei vertici sono interi
    std::map<unsigned int, list<unsigned int>> Cell1DMarkers = {}; ///< Cell1D propertoes, size 1 x NumberCell1D (marker)
//stessa cosa per i marker dei lati

    unsigned int NumberCell2D = 0; ///< number of Cell2D
    std::vector<unsigned int> Cell2DId = {}; ///< Cell2D id, size 1 x NumberCell2D
    std::vector<vector<unsigned int>> Cell2DVertices = {}; //Cell2DVertices è un vettore di vettori con indicizzazione Id riferita al poligono che contiene poi gli id dei vertici
    //altra possibilità fornita da Eigen per allocazione memoria dinamica std::vector<VectorXi> Cell2DVertices = {};VectorXi vettore di interi di misura da definire fornito da Eigen
    std::vector<vector<unsigned int>> Cell2DEdges = {}; // stessa cosa dei vertici
    //std::vector<VectorXi> Cell2DEdges = {};
    std::vector<unsigned int> Cell2DNumEdges = {};//vettore che al poligono i=id associa il numero di lati
};

}


