#include <iostream>
#include "connectDataBase.h"
#include "web.h"
#include <cstring>
#include </usr/include/mysql/mysql.h>

using namespace std;

int main(){


    /*
    Pruebas de operaciones o consultas a bases de datos:
        
        update_delete_product(18,"Leche");
        cout << endl << "N.Existence: " << view_article_existences("Huevitos") << endl;
        
        cout << endl << "Correo: " << return_user_mail("estebanq3")<< endl;
    */
   

       
    //cout << endl << "Existence: " << user_existence("sebasgon") << endl;
    //cout << "Account existence w password: " << verify_account_existence("sebasgon", "ruj1awd23") << endl;

    //cout << "Mail existence: " << verify_mail_exists("estebanq52@gmail.com") << endl;
    
    //cout << create_information_user("bananitoq3@gmail.com","Quesada","Quesada","JesÃºs", 83472343,"50 mtrs del parque") << endl;
    //
    //cout << "Precio del queso: " << return_product_price("Queso");
    //cout << "Article exist: " << article_exist("Queso") << endl;
    //insert_product("Leche",1350,55);
    //cout << create_information_user("oscara@sdgmail.com", "Navarro", "Cespedes","Oscar",84354538,"Vive en cartago a la par de un papero", "oscarnavad","tonto1234");
    //delete_information_user("estebanq52@gmail.com", "estebanq3", "pluto123");
    
    //cout << endl << "Credit card validation: " << insert_credit_card(6011303156238992,"145", "2022-12-23", "sebastian.gonzalezva@ucr.ac.cr") << endl;
    //cout << "SHOP CAR result: " << add_to_shop_car("Queso",0,"sebastian.gonzalezva@ucr.ac.cr") << endl;
    //cout << "SHOP CAR result: " << add_to_shop_car("Alegrias",45,"sebastian.gonzalezva@ucr.ac.cr") << endl;
    //cout << "SHOP CAR result: " << add_to_shop_car("Leche",38,"sebastian.gonzalezva@ucr.ac.cr") << endl;
    //cout << endl << "Delete state: " << delete_from_shop_car("Queso", "sebastian.gonzalezva@ucr.ac.cr") << endl;


    //cout << "Product quantity: " << product_quantity_in_shop_car_by_email("Queso","sebastian.gonzalezva@ucr.ac.cr");
    //cout << endl << "Resultado de crear la cuenta: " << create_new_account("Mbappe","realMadrid123","estebanq52@gmail.com") << endl;
    //cout << endl << "Contrasena encriptada: " << return_user_password("Mbappe") << endl;
    cout << endl << "Credit card created: " << insert_credit_card("4929891166311391","145", "2022-12-23", "estebanq52@gmail.com") << endl; 
    cout << endl << "Credit card created: " << insert_credit_card("5409117008830158","153", "2022-12-23", "sebasgv@gmail.com") << endl; 
    cout << endl << "Credit card existence: " << verify_credit_card_existence_registry("4929891166311391") << endl;
    cout << endl << "Email from credit card: " << return_credit_card_by_mail("estebanq52@gmail.com") << endl;

    //Creamos matrices
    /*Creacion de matriz donde voy a guardar cada fila de informacion de articulos y esa fila a su vez 
    se divide en columnas explicando cada detalle del producto*/
    string articulos[row_number_returner("Articulo")][column_number_returner("Articulo","Sitio_Ventas")];   
    string cuentas[row_number_returner("Cuenta")][column_number_returner("Cuenta","Sitio_Ventas")];   
    string metodos_pago[row_number_returner("MetodoPago")][column_number_returner("MetodoPago","Sitio_Ventas")];   
    string movimientos[row_number_returner("Movimiento")][column_number_returner("Movimiento","Sitio_Ventas")];
    string persona_usuario[row_number_returner("PersonaUsuario")][column_number_returner("PersonaUsuario","Sitio_Ventas")];   
    string shop_car[row_number_returner("CarritoCompras")][column_number_returner("CarritoCompras","Sitio_Ventas")];   


    //Rellenamos matrices
    //Puntero que apunta a la matriz de articulos
    string *ptr_articulos_matrix = articulos[0];
    //Usamos el metodo para guardar en la matriz los datos obtenidos de la base de datos
    create_matrix_splitted("Articulo",column_number_returner("Articulo","Sitio_Ventas"), row_number_returner("Articulo"), ptr_articulos_matrix);

    string *ptr_cuenta_matrix = cuentas[0];    
    create_matrix_splitted("Cuenta",column_number_returner("Cuenta","Sitio_Ventas"), row_number_returner("Cuenta"),ptr_cuenta_matrix);

    string *ptr_metodo_pago = metodos_pago[0];    
    create_matrix_splitted("MetodoPago",column_number_returner("MetodoPago","Sitio_Ventas"), row_number_returner("MetodoPago"),ptr_metodo_pago);

    string *ptr_movimiento = movimientos[0];    
    create_matrix_splitted("Movimiento",column_number_returner("Movimiento","Sitio_Ventas"), row_number_returner("Movimiento"),ptr_movimiento);

    string *ptr_persona_usuario = persona_usuario[0];    
    create_matrix_splitted("PersonaUsuario",column_number_returner("PersonaUsuario","Sitio_Ventas"), row_number_returner("PersonaUsuario"),ptr_persona_usuario);

    string *ptr_carrito_compras = shop_car[0];    
    create_matrix_splitted("CarritoCompras",column_number_returner("CarritoCompras","Sitio_Ventas"), row_number_returner("CarritoCompras"),ptr_carrito_compras);

/*
    //Imprimir matriz de articulos
    for(int a = 0; a < row_number_returner("Articulo"); a++)
    {
        for(int b = 0; b < column_number_returner("Articulo","Sitio_Ventas"); b++){
            cout << articulos[a][b] << "|";
        }
        cout << "<br>";
    }   

cout << "<br>";
*/

/*
    cout << "Array" << "<br>";
    for(int i = 0; i< row_number_returner("Articulo")*column_number_returner("Articulo","Sitio_Ventas"); i++){
        cout << "<br>" << articulos_array[i];
    }

    //cout << articulos[0][0];
    //cout << articulos[0][1];  
    //cout << "<br>" << obtain_string_tabledb("PersonaUsuario", column_number_returner("PersonaUsuario","Sitio_Ventas"));
*/  

  return 0;
}
