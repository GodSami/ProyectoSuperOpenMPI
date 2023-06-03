#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <omp.h>
#include <libssh/libssh.h>

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
    string servidorIP1 = "10.124.0.3";
    string servidorUsuario1 = "grupoj";
    string servidorContrasena1 = "dHE,6K2.vj";
    string servidorIP2 = "10.124.0.6";
    string servidorUsuario2 = "grupoj";
    string servidorContrasena2 = "dHE,6K2.vj";
    string servidorIP3 = "10.124.0.5";
    string servidorUsuario3 = "grupoj";
    string servidorContrasena3 = "dHE,6K2.vj";

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

      // Conexión y ejecución en el servidor 1
    ssh_session session1 = ssh_new();
    ssh_options_set(session1, SSH_OPTIONS_HOST, servidorIP1.c_str());
    ssh_options_set(session1, SSH_OPTIONS_USER, servidorUsuario1.c_str());

    int rc1 = ssh_connect(session1);
    if (rc1 != SSH_OK) {
        cout << "Error de conexión con el servidor 1: " << ssh_get_error(session1) << endl;
        ssh_free(session1);
        return 1;
    }

    rc1 = ssh_userauth_password(session1, NULL, servidorContrasena1.c_str());
    if (rc1 != SSH_AUTH_SUCCESS) {
        cout << "Error de autenticación con el servidor 1: " << ssh_get_error(session1) << endl;
        ssh_disconnect(session1);
        ssh_free(session1);
        return 1;
    }

    ssh_channel channel1 = ssh_channel_new(session1);
    rc1 = ssh_channel_open_session(channel1);
    if (rc1 != SSH_OK) {
        cout << "Error al abrir el canal de comunicación con el servidor 1: " << ssh_get_error(session1) << endl;
        ssh_disconnect(session1);
        ssh_free(session1);
        return 1;
    }

    string comando1 = "echo " + to_string(infa) + " > /srv/utem/inflacion.txt";
    rc1 = ssh_channel_request_exec(channel1, comando1.c_str());
    if (rc1 != SSH_OK) {
        cout << "Error al ejecutar el comando en el servidor 1: " << ssh_get_error(session1) << endl;
        ssh_channel_close(channel1);
        ssh_channel_free(channel1);
        ssh_disconnect(session1);
        ssh_free(session1);
        return 1;
    }

    ssh_channel_send_eof(channel1);
    ssh_channel_close(channel1);
    ssh_channel_free(channel1);
    ssh_disconnect(session1);
    ssh_free(session1);

    // Conexión y ejecución en el servidor 2
    ssh_session session2 = ssh_new();
    ssh_options_set(session2, SSH_OPTIONS_HOST, servidorIP2.c_str());
    ssh_options_set(session2, SSH_OPTIONS_USER, servidorUsuario2.c_str());

    int rc2 = ssh_connect(session2);
    if (rc2 != SSH_OK) {
        cout << "Error de conexión con el servidor 2: " << ssh_get_error(session2) << endl;
        ssh_free(session2);
        return 1;
    }

    rc2 = ssh_userauth_password(session2, NULL, servidorContrasena2.c_str());
    if (rc2 != SSH_AUTH_SUCCESS) {
        cout << "Error de autenticación con el servidor 2: " << ssh_get_error(session2) << endl;
        ssh_disconnect(session2);
        ssh_free(session2);
        return 1;
    }

    ssh_channel channel2 = ssh_channel_new(session2);
    rc2 = ssh_channel_open_session(channel2);
    if (rc2 != SSH_OK) {
        cout << "Error al abrir el canal de comunicación con el servidor 2: " << ssh_get_error(session2) << endl;
        ssh_disconnect(session2);
        ssh_free(session2);
        return 1;
    }

    string comando2 = "echo " + to_string(infa) + " > /srv/utem/inflacion.txt";
    rc2 = ssh_channel_request_exec(channel2, comando2.c_str());
    if (rc2 != SSH_OK) {
        cout << "Error al ejecutar el comando en el servidor 2: " << ssh_get_error(session2) << endl;
        ssh_channel_close(channel2);
        ssh_channel_free(channel2);
        ssh_disconnect(session2);
        ssh_free(session2);
        return 1;
    }

    ssh_channel_send_eof(channel2);
    ssh_channel_close(channel2);
    ssh_channel_free(channel2);
    ssh_disconnect(session2);
    ssh_free(session2);

    // Conexión y ejecución en el servidor 3
    ssh_session session3 = ssh_new();
    ssh_options_set(session3, SSH_OPTIONS_HOST, servidorIP3.c_str());
    ssh_options_set(session3, SSH_OPTIONS_USER, servidorUsuario3.c_str());

    int rc3 = ssh_connect(session3);
    if (rc3 != SSH_OK) {
        cout << "Error de conexión con el servidor 3: " << ssh_get_error(session3) << endl;
        ssh_free(session3);
        return 1;
    }

    rc3 = ssh_userauth_password(session3, NULL, servidorContrasena3.c_str());
    if (rc3 != SSH_AUTH_SUCCESS) {
        cout << "Error de autenticación con el servidor 3: " << ssh_get_error(session3) << endl;
        ssh_disconnect(session3);
        ssh_free(session3);
        return 1;
    }

    ssh_channel channel3 = ssh_channel_new(session3);
    rc3 = ssh_channel_open_session(channel3);
    if (rc3 != SSH_OK) {
        cout << "Error al abrir el canal de comunicación con el servidor 3: " << ssh_get_error(session3) << endl;
        ssh_disconnect(session3);
        ssh_free(session3);
        return 1;
    }

    string comando3 = "echo " + to_string(infa) + " > /srv/utem/inflacion.txt";
    rc3 = ssh_channel_request_exec(channel3, comando3.c_str());
    if (rc3 != SSH_OK) {
        cout << "Error al ejecutar el comando en el servidor 3: " << ssh_get_error(session3) << endl;
        ssh_channel_close(channel3);
        ssh_channel_free(channel3);
        ssh_disconnect(session3);
        ssh_free(session3);
        return 1;
    }

    ssh_channel_send_eof(channel3);
    ssh_channel_close(channel3);
    ssh_channel_free(channel3);
    ssh_disconnect(session3);
    ssh_free(session3);

    return 0;
}

