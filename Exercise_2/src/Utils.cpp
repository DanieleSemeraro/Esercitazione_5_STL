#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

//formula di Gauss calcolo area poligoni
double calculatePolygonArea(const vector<unsigned int>& vertexIndices, const std::vector<Vector2d>& Cell0DCoordinates) {
    double area = 0.0;
    int numVertices = vertexIndices.size();//numero di vertici del poligono
    for (int i = 0; i < numVertices; i++) {
        int j = (i + 1) % numVertices;  // l'ultimo vertice ritorna il primo
        const Vector2d& vi = Cell0DCoordinates[vertexIndices[i]];//accedo alle coordinate dell'i-esimo vertice del poligono infatti ho vettore di 2 double

        const Vector2d& vj = Cell0DCoordinates[vertexIndices[j]];

        area += vi[0] * vj[1] - vj[0] * vi[1];//vedi formula Gauss
    }
    return fabs(area) * 0.5;}  //prendo il valore assoluto
//verifica lunghezza lati non nulli
bool verifyNonZeroEdges(const vector<unsigned int>& vertexIndices, const std::vector<Vector2d>& Cell0DCoordinates) {
    int numVertices = vertexIndices.size();
    for (int i = 0; i < numVertices; i++) {
        int j = (i + 1) % numVertices;
        const Vector2d& vi = Cell0DCoordinates[vertexIndices[i]];
        const Vector2d& vj = Cell0DCoordinates[vertexIndices[j]];
        if ((vi - vj).norm() == 0) {//se la distanza tra le coordinate dei due vertici in norma euclidea è minore della precisione di macchina ritorno errore
            return false;
        }
    }
    return true;
}


namespace PolygonalLibrary {
bool ImportMesh(const string& filepath,//restituisce un booleano
                PolygonalMesh& mesh)
{
    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",//il file Cell0Ds.csv è nella cartella PolygonalMesh
                       mesh))
    {
        return false;//se la funzione ImportCell0Ds mi restituisce false, la negazione si attiva entra nell'if e ritorna false
    }
    else
    {
        cout << "Cell0D marker:" << endl;//altrimenti stampo i marker letti con ImportCell0Ds
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)//auto stabilisce automaticamente il tipo della variabile
        {//mesh.Cell0DMarkers.begin() mi restituisce un iteratore che punta alla prima chiave della mappa
            cout << "key:\t" << it -> first << "\t values:";//stampa la chiave
            for(const unsigned int id : it -> second)//stampa i valori associati perchè second si riferisce ai valori associati alla chiave
                cout << "\t" << id;

            cout << endl;
        }}//itero finchè finisco le chiavi

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",//stessa identica cosa
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }
    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {//visto che se la funzione ImportCell2Ds non ritorna false ho acquisito i dati correttamente allora verifico correttezza poligoni
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            vector<unsigned int> edges = mesh.Cell2DEdges[c];//assegno il vettore che contiene i lati del c-esimo poligono al vettore di unsigned int "edges"
            for(unsigned int r = 0; r < mesh.Cell2DNumEdges[c] ; r++)
            {//itero per ogni lato nel poligono
                const unsigned int origin = mesh.Cell1DVertices[edges[r]][0];//prendo l'id di origine del lato r-esimo dalla mesh.Cell1DVertices perchè gli do l'id del lato
                const unsigned int end = mesh.Cell1DVertices[edges[r]][1];//quindi sto iterando sul vettore edges che ha come valori gli id dei lati

                auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin);
                if(findOrigin == mesh.Cell2DVertices[c].end())//se find non trova il valore origin tra i vertici del poligono memorizzati in mesh.Cell2DVertices[c] allora ritorna
                {                                             //mesh.Cell2DVertices[c].end() quindi verifico poi che siano uguali per dare errore
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end);
                if(findEnd == mesh.Cell2DVertices[c].end())//stessa cosa se ritorna mesh.Cell2DVertices[c].end() vuol dire che sono andato fuori dal vettore come conteggio
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }

            }
        }
    }

    return true;


}
//funzione raccolta dati per i vertici
bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;//creo insieme di stringhe chiamato listLines che saranno le righe del mio file di dati
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();//cancello primo elemento insieme

    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);//alloco almeno tanta memoria per il vector quanto è grande mesh.NumberCell0D
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string id_str, marker_str, coordx_str, coordy_str;
        unsigned int id, marker;
        Vector2d coord;
        getline(converter, id_str, ';');//leggo fino al ;
        getline(converter, marker_str, ';');//
        getline(converter, coordx_str, ';');//
        getline(converter, coordy_str, ';');//
        id = stoi(id_str);//trasformo in intero
        marker = stoi(marker_str);
        coord(0) = stod(coordx_str);//trasformo in double e assegno la coordinata x
        coord(1)= stod(coordy_str);
        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if( marker != 0)
        {
            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)//se l'id di un altro vertice con lo stesso marker è diverso
                (ret.first)->second.push_back(id);//metti in coda anche l'altro id trovato e assegna alla stessa chiave(first) che è il marker in questo caso
        }

    }
    file.close();
    return true;
}
//funzione raccolta dati per i vertici
bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)//stessa cosa dei vertici tranne che non ho coordinate ma due id per ogni edge
    {
        istringstream converter(line);
        string id_str, marker_str, begin_str, end_str;
        unsigned int id, marker;
        Vector2i vertices;
        getline(converter, id_str, ';');
        getline(converter, marker_str, ';');
        getline(converter, begin_str, ';');
        getline(converter, end_str, ';');
        id = stoi(id_str);
        marker = stoi(marker_str);
        vertices(0) = stoi(begin_str);
        vertices(1)= stoi(end_str);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}
bool ImportCell2Ds(const string &filename,PolygonalMesh& mesh)
{
    ifstream file;//creo oggetto ifstream
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;//creo insieme di stringhe chiamato listLines che saranno le righe del mio file di dati
    string line;
    while (getline(file,line))
        listLines.push_back(line);//finchè ci sono righe aggiungile alla listLines

    listLines.pop_front();//cancella la prima riga da listLines

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }
    for (const string& line : listLines)
    {
        istringstream converter(line);//per ogni riga del file che è in listLines assegno i vari dati (Id, numVertices,numEdges)

        unsigned int id, marker, numVertices,numEdges;
        char toDelete;
        converter >>  id >> toDelete >> marker >> toDelete;//altro modo di leggere i dati dal file senza getline

        converter >> numVertices;
        converter >> toDelete;

        /*VectorXi vertices(numVertices);//alternativa se volevo allocare memoria dinamicamente con Eigen
        for(unsigned int i = 0; i < numVertices; i++)
        {
            converter >> vertices[i];
            converter >> toDelete;
        }*/
        vector<unsigned int> vertices(numVertices);//ho letto prima il numero di vertici del poligono,posso dare la dimensione al vertice
        for(unsigned int i = 0; i < numVertices; i++)
        {
            converter >> vertices[i];
            converter >> toDelete;
        }
        converter >> numEdges;
        converter >> toDelete;
        vector<unsigned int> edges(numEdges);
        for(unsigned int i = 0; i < numEdges; i++)
        {
            converter >> edges[i];
            converter >> toDelete;
        }

        mesh.Cell2DId.push_back(id);//aggiungo id dei poligoni
        mesh.Cell2DVertices.push_back(vertices);//aggiungo vettore che contiene gli id dei vertici del poligono
        mesh.Cell2DEdges.push_back(edges);//aggiungo vettore che contiene gli id dei lati
        mesh.Cell2DNumEdges.push_back(numEdges);//aggiungo vettore che contiene numero dei lati dell'i-esimo poligono
        double area = calculatePolygonArea(vertices, mesh.Cell0DCoordinates);
        if (area <= 0) {
            cerr << "Polygon with ID " << id << " has zero or negative area." << endl;
            return false;
        }
            if (!verifyNonZeroEdges(vertices, mesh.Cell0DCoordinates)) {
                cerr << "Polygon with ID " << id<< " has zero-length edge." << endl;
                return false;  // ritorna falso per dare errore a ImportMesh
            }
    }

    file.close();

    return true;
}
}

