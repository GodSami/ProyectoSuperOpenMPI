#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
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

    double meses[11];
    double IPC[11];
    double inflacion[11];
    double infa;
    
    ifstream archivo(nombreArchivo.c_str());
    
    if (!archivo) {
        cout << "No se pudo abrir el archivo " << nombreArchivo << endl;
    }
    
    string linea;
    getline(archivo, linea); // Saltar la primera línea de encabezados
    int j=0;
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
        stringstream(campo) >> transaccion.DESCUENTO;

        getline(ss, campo, ';');
        transaccion.FECHA = campo;

        getline(ss, campo, ';');
        transaccion.ESTADO = campo;
        
                // Solo guardar los datos con estado "AUTHORIZED" o "FINALIZED"
        if (transaccion.ESTADO == "\"AUTHORIZED\"" || transaccion.ESTADO == "\"FINALIZED\"") {
        j++;
		// Obtener el mes de la fecha (formato: 2022-01-01T16:09:45.628Z)
    	string fecha = transaccion.FECHA;
    	
    	// Buscar la posición del primer guion '-'
    	size_t posGuion = fecha.find('-');
    	
    	// Obtener el substring desde el primer guion hasta el segundo guion
    	string mes = fecha.substr(posGuion + 1, 2);

    		
			
			if(mes== "01"){	
    			meses[0]=meses[0]+transaccion.MONTO;
			}
			if(mes=="02"){
				meses[1]=meses[1]+transaccion.MONTO;
			}
			if(mes=="03"){
				meses[2]=meses[2]+transaccion.MONTO;
			}
			if(mes=="04"){
				meses[3]=meses[3]+transaccion.MONTO;
			}
			if(mes=="05"){
				meses[4]=meses[4]+transaccion.MONTO;
			}
			if(mes=="06"){
				meses[5]=meses[5]+transaccion.MONTO;
			}
			if(mes=="07"){
				meses[6]=meses[6]+transaccion.MONTO;
			}
			if(mes=="08"){
				meses[7]=meses[7]+transaccion.MONTO;
			}
			if(mes=="09"){
				meses[8]=meses[8]+transaccion.MONTO;
			}
			if(mes=="10"){
				meses[9]=meses[9]+transaccion.MONTO;
			}
			if(mes=="11"){
				meses[10]=meses[10]+transaccion.MONTO;
			}
			if(mes=="12"){
				meses[11]=meses[11]+transaccion.MONTO;
			}
			
        }      
}
	
	for(int i=0;i<12;i++){
		
		IPC[i]=(meses[i]/meses[0])*100;
		cout<<"IPC Mes["<<i<<"]: "<<IPC[i]<<endl;
	}
	
	for(int i=1;i<12;i++){
		inflacion[i]= ((IPC[i]-IPC[0])/IPC[0])*100;
		cout<<"Inflacion Mes["<<i<<"]: "<<inflacion[i]<<endl;
		
		infa = infa + inflacion[i];
	}
	
	cout<<"Inflacion Acumulada: "<<infa<<endl;

	
	
	
		
    return 0;
}

