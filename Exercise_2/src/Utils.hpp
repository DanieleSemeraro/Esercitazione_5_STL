#pragma once
#include <iostream>
#include "PolygonalMesh.hpp"
using namespace std;
//definizione delle funzioni che userò per la Mesh poligonale
namespace PolygonalLibrary {
/// Import the triangular mesh and test if the mesh is correct
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportMesh(const string &filepath,//gli passo la stringa costante che contiene il nome della cartella da cui estrarre i file
                PolygonalMesh& mesh);//gli passo la struttura PolygonalMesh definita in PolygonalMesh.hpp

/// Import the Cell0D properties from Cell0Ds.csv file
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh);

/// Import the Cell1D properties from Cell1Ds.csv file
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh);

/// Import the Cell2D properties from Cell2Ds.csv file
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh);

}

