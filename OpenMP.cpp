#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <omp.h>

using namespace std;

struct Transaccion {
    string SKU;
    string NOMBRE;
    int MONTO;
    int DESCUENTO;
    string FECHA;
    string ESTADO;
};

int eliminarComillasYConvertirAEntero(const std::string& str) {
    // Verificar si el string tiene al menos dos caracteres (comillas al inicio y al final)
    if (str.size() < 2) {
        return 0;  // Otra opción es lanzar una excepción
    }
    
    // Verificar que el primer y último caracteres sean comillas
    if (str[0] != '"' || str[str.size() - 1] != '"') {
        return 0;  // Otra opción es lanzar una excepción
    }
    
    // Obtener el substring sin las comillas
    std::string strWithoutQuotes = str.substr(1, str.size() - 2);
    
    // Crear un stringstream y escribir el substring en él
    std::stringstream ss(strWithoutQuotes);
    
    // Leer el entero desde el stringstream
    int number;
    if (!(ss >> number)) {
        return 0;  // Otra opción es lanzar una excepción
    }
    
    // Verificar si hay caracteres adicionales después del número
    char c;
    if (ss >> c) {
        return 0;  // Otra opción es lanzar una excepción
    }
    
    return number;
}

int main() {
    string nombreArchivo = "/srv/utem/supermercado.csv";

    double meses[13] = {0};
    double IPC[13] = {0.0};
    double inflacion[13] = {0.0};
    double infa = 0.0;
    int numTransacciones = 0;

    ifstream archivo(nombreArchivo.c_str());
    
    if (!archivo) {
        cout << "No se pudo abrir el archivo " << nombreArchivo << endl;
        return 1;
    }
    
    string linea;
    getline(archivo, linea); // Saltar la primera línea de encabezados

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        Transaccion transaccion;

        getline(ss, campo, ';');
        transaccion.SKU = campo;


        getline(ss, campo, ';');
        transaccion.NOMBRE = campo;

        getline(ss, campo, ';');
        transaccion.MONTO = eliminarComillasYConvertirAEntero(campo);

        getline(ss, campo, ';');
        transaccion.DESCUENTO = eliminarComillasYConvertirAEntero(campo);
        getline(ss, campo, ';');
        transaccion.FECHA = campo;
        getline(ss, campo, ';');
        transaccion.ESTADO = campo;
        // Solo guardar los datos con estado "AUTHORIZED" o "FINALIZED"
        if (transaccion.ESTADO == "\"AUTHORIZED\"" || transaccion.ESTADO == "\"FINALIZED\"") {
            // Obtener el mes de la fecha (formato: "2022-01-01T16:09:45.628Z")
            string fecha = transaccion.FECHA;
            // Buscar la posición del primer guion '-'
            size_t posGuion = fecha.find('-');
    
            // Obtener el substring desde el primer guion hasta el segundo guion
            std::string mes= fecha.substr(posGuion + 1, 2);

            if(mes == "01"){	
                #pragma omp atomic update
                meses[0] += transaccion.MONTO;
            }
            else if(mes == "02"){
                #pragma omp atomic update
                meses[1] += transaccion.MONTO;
            }
            else if(mes == "03"){
                #pragma omp atomic update
                meses[2] += transaccion.MONTO;
            }
            else if(mes == "04"){
                #pragma omp atomic update
                meses[3] += transaccion.MONTO;
            }
            else if(mes == "05"){
                #pragma omp atomic update
                meses[4] += transaccion.MONTO;
            }
            else if(mes == "06"){
                #pragma omp atomic update
                meses[5] += transaccion.MONTO;
            }
            else if(mes == "07"){
                #pragma omp atomic update
                meses[6] += transaccion.MONTO;
            }
            else if(mes == "08"){
                #pragma omp atomic update
                meses[7] += transaccion.MONTO;
            }
            else if(mes == "09"){
                #pragma omp atomic update
                meses[8] += transaccion.MONTO;
            }
            else if(mes == "10"){
                #pragma omp atomic update
                meses[9] += transaccion.MONTO;
            }
            else if(mes == "11"){
                #pragma omp atomic update
                meses[10] += transaccion.MONTO;
            }
            else if(mes == "12"){
                #pragma omp atomic update
                meses[11] += transaccion.MONTO;
            }
        }
        
    }
    
    archivo.close();
    
    // Calcular IPC y acumular inflación
    for(int i = 0; i < 12; i++){
        IPC[i] = (meses[i] / meses[0]) * 100;
        cout << "IPC Mes[" << i << "]: " << IPC[i] << endl;
    }
    
    for(int i = 1; i < 12; i++){
        inflacion[i] = ((IPC[i] - IPC[0]) / IPC[0]) * 100;
        cout << "Inflacion Mes[" << i << "]: " << fixed << setprecision(4) << inflacion[i] << endl;
        
        infa += inflacion[i];
    }
    
    cout << "Inflacion Acumulada: " << fixed << setprecision(4) << infa << endl;

    return 0;
}
