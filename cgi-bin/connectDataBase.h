// g++ main.cpp -o output -L/usr/include/mariadb/mysql -lmariadbclient
#include </usr/include/mysql/mysql.h>
#include <cstring>
#include "cardNumberVerify.h"
#include "AESEncDec.h"
#include "Hash.h"

using namespace std;

struct connection_details
{
    const char *server, *user, *password, *database;
};


Hash decoder;

MYSQL* mysql_connection_setup(struct connection_details mysql_details){
    MYSQL *connection = mysql_init(NULL); // mysql instance
    
    //connect database
    if(!mysql_real_connect(connection, mysql_details.server, mysql_details.user, mysql_details.password, mysql_details.database, 0, NULL, 0)){
        cout << "Connection Error: " << mysql_error(connection) << endl;
        exit(1); 
    }

    return connection;
}

// mysql_res = mysql result
MYSQL_RES* mysql_perform_query(MYSQL *connection, const char *sql_query){
    //send query to db
    if(mysql_query(connection, sql_query)){
        cout << "MySQL Query Error: " << mysql_error(connection) << endl;
        exit(1);
    }

    return mysql_use_result(connection);
}


//Variables generales
MYSQL *con;	// the connection
MYSQL_RES *numberColumns; //row number
MYSQL_RES *numberRows; //row number
MYSQL_RES *res;	// the results
MYSQL_ROW row;	// the results rows (array)
int columns = 0;
int rows = 0;

//Datos de inicio de sesión a la base de datos
struct connection_details mysqlD = {"localhost", "root", "eqsgon2015!","Sitio_Ventas"};

 /**
 * Funcion para retornar el numero de columnas que tiene una determinada tabla de la base de datos
 * @param table_name_parameter nombre de la tabla de la base de datos
 * @param database_name nombre de la base de datos
 * @return el numero de columnas de una tabla
 */
int column_number_returner(string table_name_parameter, string database_name)
{
    // connect to the mysql database
    con = mysql_connection_setup(mysqlD);
    int *ptrColumns = &columns;
    int columnas;
    
    string query = "SELECT COUNT(*) AS NUMBEROFCOLUMNS FROM INFORMATION_SCHEMA.COLUMNS WHERE table_schema = '" + database_name  + "' AND table_name = \"" + table_name_parameter + "\";";
    //Consulta #1
    numberColumns = mysql_perform_query(con, query.c_str());
    while ((row = mysql_fetch_row(numberColumns)) != NULL){
        // the below row[] parametes may change depending on the size of the table and your objective
        //save number of columns
        columnas = atoi(*row);

    } 
    mysql_free_result(numberColumns);
    mysql_close(con);

    return columnas;
}

 /**
 * Funcion para retornar el numero de filas que tiene una determinada tabla de la base de datos
 * @param table_name_parameter nombre de la tabla de la base de datos
 * @return el numero de filas de una tabla
 */
int row_number_returner(string table_name_parameter)
{
    // connect to the mysql database
    con = mysql_connection_setup(mysqlD);
    int *ptrRows = &rows;
    int row_number;

    string queryColumnsNumber = "SELECT COUNT(*) FROM ";
    string finalQuery = queryColumnsNumber + table_name_parameter;
    //Consulta #1
    numberRows = mysql_perform_query(con, (finalQuery.append(";")).c_str());
    while ((row = mysql_fetch_row(numberRows)) != NULL){
        // the below row[] parametes may change depending on the size of the table and your objective
        //save number of rows
        row_number = atoi(*row);
    }
    //cout << "Number of rows: " << *numberRows << endl;
    mysql_free_result(numberRows);
    

    mysql_close(con);

    return row_number;

}

 /**
 * Funcion para retornar el numero de columnas que tiene una determinada tabla de la base de datos
 * @param table_name_parameter nombre de la tabla de la base de datos
 * @param column_number numero de columnas de la tabla
 * @return retorna una tabla de la base de datos en formato string
 */
string obtain_string_tabledb(string table_name_parameter,int column_number)
{
    // connect to the mysql database
    con = mysql_connection_setup(mysqlD);

    string table_name = table_name_parameter;
    //int column_number = column_number_returner(table_name_parameter);
    

    // get the results from executing commands
    //Consulta #2
    string query = "SELECT * FROM ";
    string data = "";
    res = mysql_perform_query(con, (query.append(table_name)).c_str());
    //cout << ("Database Output:\n") << endl;
    while ((row = mysql_fetch_row(res)) != NULL){
        for(int i = 0; i < column_number; i++){
            data += row[i];
            data += " | ";
        }
        data += "$";

        //cout << typeid(row).name() << endl;
        // the below row[] parametes may change depending on the size of the table and your objective
        
    }
    // clean up the database result
    mysql_free_result(res);

    // close database connection
    mysql_close(con);

    return data;
}

 /**
 * Crear una matriz con los datos de una tabla de la base de datos, realiza un  split de los datos, por lo tanto cada casilla representa un campo de una tabla
 * @param table_name nombre de la tabla de la base de datos
 * @param colum_number numero de columnas
 * @param row_number numero de filas
 * @param ptrMatrix puntero a la matriz
 * @return 0 si la operacion de creacion fue exitosa
 */
int create_matrix_splitted(string table_name, int colum_number, int row_number, string *ptrMatrix)
{
    int j = 0;
    string articulos_lista = obtain_string_tabledb(table_name, column_number_returner(table_name,"Sitio_Ventas"));
    //cout << articulos_lista << endl;
    //Volvemos el string un char* (mera conversion)
    char* articulos_lista_iterator = &articulos_lista[0];
    char* ptrSplitter = NULL;
    //Hacemos split de los datos pro primera vez, separando en filas distintas cada vez que encuentra un $
    ptrSplitter = strtok(articulos_lista_iterator, "$");

while(ptrSplitter != NULL)
    {
      //cout << ptrSplitter << endl;

      string line = ptrSplitter;
      string temp = "";
      for(int i=0; i<(int)line.size(); i++){
        // If cur char is not del, then append it to the cur "word", otherwise
          // you have completed the word, print it, and start a new word.
         if(line[i] != '|'){
            temp += line[i];
        }
          else{
            //cout << temp << endl;
            //cout << " J: " << j << endl;
            ptrMatrix[j] = temp;
            temp = "";
            j++;
        }
      }

      ptrSplitter = strtok(NULL, "$");
    }

    return 0;
}


/*
Operaciones de Producto
    -INSERT_PRODUCT
    -VIEW_ARTICLE_EXISTENCES
    -UPDATE_DELETE_PRODUCT
    -RETURN_PRODUCT_PRICE
    -RETURN_ARTICLE_EXIST
*/


 /**
 * Verifica si ya existe un producto con el nombre del producto a insertar
 * @param product_name nombre del producto a verificar su existencia
 * @return retorna true si existe, de lo contrario false
 */
bool article_exist(string product_name){
    bool exist = false;
    con = mysql_connection_setup(mysqlD);

    string query = "SELECT EXISTS(SELECT nombre FROM Articulo WHERE nombre = \"" + product_name + "\");";
    res = mysql_perform_query(con, query.c_str());
    while ((row = mysql_fetch_row(res)) != NULL){
        exist = atoi(*row);
    }

    mysql_close(con);

    return exist;
}

 /**
 * Insertar articulo en tabla cuando un usuario quiere vender algo
 * @param nombre nombre del producto a insertar
 * @param precio precio del producto a insertar
 * @param numero_existencias numero de existencias del producto a insertar
 * @return retorna true si existe, de lo contrario false
 */
int insert_product(string nombre, double precio, int numero_existencias)
{   
    if(!article_exist(nombre))
    {
        con = mysql_connection_setup(mysqlD);
        string query = "INSERT INTO Articulo(nombre,precio,numeroExistencias) Values(\"" + nombre + "\"," + to_string(precio) + ","+ to_string(numero_existencias) + ");";
        mysql_perform_query(con, query.c_str());
        // close database connection
        mysql_close(con);
    }
    return 0;
}

 /**
 * Verificar si un articulo existe en la base de datos, retorna el número de articulos disponibles del mismo
 * @param nombre nombre del producto a verificar numero de existencias
 * @return retorna el numero de existencias de un producto
 */
int view_article_existences(string product_name)
{
    int article_number = 0;
    
    con = mysql_connection_setup(mysqlD);
    
    string query = "SELECT numeroExistencias FROM Articulo WHERE nombre =\"" + product_name + "\";";
    res = mysql_perform_query(con, query.c_str());
    while ((row = mysql_fetch_row(res)) != NULL){
        article_number = atoi(*row);
    }

    mysql_close(con);
    
    return article_number;
}

 /**
 * Realiza update de productos después de una compra menor al stock disponible o realiza delete en caso de una compra que deje 0 productos
 * @param number_existences_to_buy numero de existencias del producto a comprar
 * @param product_name nombre del producto a actualizar/eliminar
 * @return retorna 0 si la operacion fue exitosa
 */
int update_delete_product(int number_existences_to_buy, string product_name)
{
    int actual_existences = view_article_existences(product_name);
    con = mysql_connection_setup(mysqlD);
    if(actual_existences > number_existences_to_buy)
    {
        int new_article_existences = actual_existences - number_existences_to_buy;
        string query = "UPDATE Articulo SET numeroExistencias =" + to_string(new_article_existences) + " WHERE nombre = \"" + product_name + "\";";
        mysql_perform_query(con, query.c_str());
    }
    else
    {
        if(actual_existences == number_existences_to_buy){
            string query = "DELETE FROM Articulo WHERE nombre = \"" + product_name + "\";";
            mysql_perform_query(con, query.c_str());
        }
    }
    // close database connection
    mysql_close(con);
    
    return 0;
}

 /**
 * Retorna el precio despues de darle el nombre del mismo
 * @param product_name nombre del producto a devolver su precio
 * @return retorna el precio del producto
 */
double return_product_price(string product_name)
{
    double precio = 0.0;
    con = mysql_connection_setup(mysqlD);

    string query = "SELECT precio FROM Articulo WHERE nombre = \"" + product_name + "\";";
    res = mysql_perform_query(con, query.c_str());
    while ((row = mysql_fetch_row(res)) != NULL){
        precio = stod(*row);
    }

    mysql_close(con);

    return precio;
}


/*
Operaciones de Cuenta
    -USER_EXISTENCE
    -VERIFY_ACCOUNT_EXISTENCE
    -RETURN_USER_MAIL
    -CREATE_NEW_COUNT
    -DELETE_ACCOUNT
*/

 /**
 * Retorna true o false, según sea el caso de la existencia del usuario
 * @param username nombre del usuario a verificar si existe o no
 * @return retorna true si existe, de lo contrario false
 */
bool user_existence(string username)
{
    bool exist = false;
    con = mysql_connection_setup(mysqlD);

    string query = "SELECT EXISTS(SELECT * FROM Cuenta WHERE nombreUsuario = \"" + username + "\");";
    //cout << endl << "CONSULTA USUARIO/PASS; " << query << endl;
    res = mysql_perform_query(con, query.c_str());
    while ((row = mysql_fetch_row(res)) != NULL){
        exist = atoi(*row);
    }
    // close database connection
    mysql_close(con);

    return exist;
}


 /**
 * Verifica si el username y el password coincide con uno almacenado en la base de datos
 * @param username nombre del usuario
 * @param password contrasena del usuario
 * @return retorna true si existe, de lo contrario false
 */

bool verify_account_existence(string username,string password)
{
    bool exist = false;
    /*
    byte key[16];
    byte iv[16];  
    // Initialize common key and IV with appropriate values
    init_Key(key,iv,sizeof(key));
    password = encrypt(password,key,iv);
    */
    
    password = decoder.SHA512(password);
    //cout << "<span>" << password << "</span>\n";
    con = mysql_connection_setup(mysqlD);

    string query = "SELECT EXISTS(SELECT * FROM Cuenta WHERE nombreUsuario = \"" + username + "\" && contrasena = \"" + password + "\");";
    res = mysql_perform_query(con, query.c_str());
    while ((row = mysql_fetch_row(res)) != NULL){
        exist = atoi(*row);
    }
    // close database connection
    mysql_close(con);

    return exist;
}

 /**
 * Retorna el mail asociado a un nombre de usuario
 * @param username nombre del usuario
 * @return retorna el email del usuario
 */
string return_user_mail(string username)
{
    string mail = "";
    con = mysql_connection_setup(mysqlD);

    string query = " SELECT personaUsuario_correo_cuenta FROM Cuenta WHERE nombreUsuario = \"" + username + "\";";
    res = mysql_perform_query(con, query.c_str());
    while ((row = mysql_fetch_row(res)) != NULL){
        mail = row[0];
    }
    // close database connection
    mysql_close(con);

    return mail;
}

//retorna el mail asociado a un nombre de usuario
string return_user_password(string username)
{
    string password = "";
    con = mysql_connection_setup(mysqlD);

    string query = " SELECT contrasena FROM Cuenta WHERE nombreUsuario = \"" + username + "\";";
    res = mysql_perform_query(con, query.c_str());
    while ((row = mysql_fetch_row(res)) != NULL){
        password = row[0];
    }
    // close database connection
    mysql_close(con);

    return password;
}

//retorna 1 si la cuenta se pudo crear y 0 si la cuenta no se pudo crear por la ya existencia de una cuenta con el mismo usuario
int create_new_account(string username, string password, string mail)
{
    /*
    byte key[16];
    byte iv[16];  
    // Initialize common key and IV with appropriate values
    init_Key(key,iv,sizeof(key));
    password = encrypt(password,key,iv);
    */

    password = decoder.SHA512(password);
    bool can_create = false;
    if(!user_existence(username))
    {
        con = mysql_connection_setup(mysqlD);
        can_create = true;
        string query = "INSERT INTO Cuenta(nombreUsuario, contrasena, personaUsuario_correo_cuenta) Values(\"" + username + "\",\"" + password + "\", \"" + mail + "\");";
        mysql_perform_query(con, query.c_str());
        // close database connection
        mysql_close(con);
    }

    return can_create;
}

//elimina la cuenta de usuario de la tabla Cuenta
int delete_account(string username, string password){
    con = mysql_connection_setup(mysqlD);
    string query = "DELETE FROM Cuenta WHERE nombreUsuario = \"" + username + "\" && contrasena = \"" + password + "\";";
    mysql_perform_query(con, query.c_str());
    // close database connection
    mysql_close(con);
}

/*

Operaciones de PersonaUsuario
    -MAIL_EXIST
    -CREATE_INFORMATION_USER


*/

//Verifica la existencia de un mail, devuelve 1 si existe y 0 si no existe
bool verify_mail_exists(string mail)
{   
    bool exist = false;
    con = mysql_connection_setup(mysqlD);
    string query = "SELECT EXISTS(SELECT * FROM PersonaUsuario WHERE correo =\"" + mail + "\");";
    res = mysql_perform_query(con, query.c_str());
    while ((row = mysql_fetch_row(res)) != NULL){
        exist = atoi(*row);
    }
    mysql_close(con);
    
    return exist;
}

//Crear nueva cuenta de usuario, retorna 1 si se pudo crear, 0 si no se pudo crear
bool create_information_user(string mail, string primerApellido,string segundoApellido, string nombre, int telefono, string direccion, string username, string password)
{
    bool created = false;
    if(!verify_mail_exists(mail))
    {
        created = true;
        con = mysql_connection_setup(mysqlD);
        string query = "INSERT INTO PersonaUsuario(correo,primerApellido,segundoApellido,nombre,telefono,direccion) Values(\"" + mail + "\",\"" + primerApellido + "\", \"" + segundoApellido + "\",\"" + nombre + "\"," + to_string(telefono) + ",\"" + direccion + "\");";
        mysql_perform_query(con, query.c_str());
        mysql_close(con);
         
        create_new_account(username,password,mail);
    }

    return created;
}

//Elimina tanto la cuenta como la información personal de un usuario en el sistema, es el removedor completo, no usar solo el delete_account, sino usar este
bool delete_information_user(string mail, string username, string password){
    bool succesful_delete = false;
    if(verify_account_existence(username, password))
    {
        delete_account(username, password);
        if(verify_mail_exists(mail)){
            con = mysql_connection_setup(mysqlD);
            string query = "DELETE FROM PersonaUsuario WHERE correo = \"" + mail + "\";";
            mysql_perform_query(con, query.c_str());
            // close database connection
            mysql_close(con);
        }
    }

}


/*

Operaciones de Metodo de Pago
    -INSERT_CREDIT_CARD
    -RETURN_CREDIT_CARD_BY_MAIL
    -VERYFY_CREDIT_CARD_EXISTENCE_REGISTRY

*/

//Verifica la existencia de la tarjeta de credito en la base de datos
bool verify_credit_card_existence_registry(string credit_card_number)
{   
    bool exist = false;

    byte key[16];
    byte iv[16];  
    // Initialize common key and IV with appropriate values
    init_Key(key,iv,sizeof(key));
    credit_card_number = encrypt(credit_card_number,key,iv);

    con = mysql_connection_setup(mysqlD);
    string query = "SELECT EXISTS(SELECT * FROM MetodoPago WHERE numeroTarjeta =\"" + credit_card_number + "\");";
    res = mysql_perform_query(con, query.c_str());
    while ((row = mysql_fetch_row(res)) != NULL){
        exist = atoi(*row);
    }
    mysql_close(con);
    return exist;

}

//Retorna 1 si pudo insertar la tarjeta debido al tener un numero valido, retorna 0 si el numero de tarjeta es invalido y no pudo realizar la insercion de la tarjeta
int insert_credit_card(string credit_card_number, string password, string date, string mail_asociated)
{
    int insertion_result = 0;
    bool exist_credit_card = false;
    exist_credit_card = verify_credit_card_existence_registry(credit_card_number);

    con = mysql_connection_setup(mysqlD);

    if(isValid(stol(credit_card_number)))
    {
        if(!exist_credit_card)
        {
            byte key[16];
            byte iv[16];  
            init_Key(key,iv,sizeof(key));
            credit_card_number = encrypt(credit_card_number,key,iv);
            password = encrypt(password,key,iv);
            string query = "INSERT INTO MetodoPago(numeroTarjeta,contrasena,fechaVencimiento, correo_metodoPago) Values(\"" + credit_card_number + "\",\"" + password + "\", \"" + date + "\",\"" + mail_asociated + "\");";
            mysql_perform_query(con, query.c_str());
            insertion_result = 1;
        }
    }
    //close database connection
    mysql_close(con);
    
    return insertion_result;
}

//Retorna el numero de tarjeta de credito a partir del mail de un cliente
string return_credit_card_by_mail(string mail)
{
    byte key[16];
    byte iv[16];  
    // Initialize common key and IV with appropriate values
    init_Key(key,iv,sizeof(key));

    string credit_card = "";
    con = mysql_connection_setup(mysqlD);
    string query = "SELECT numeroTarjeta FROM MetodoPago WHERE correo_metodoPago =\"" + mail + "\";";
    res = mysql_perform_query(con, query.c_str());
    while ((row = mysql_fetch_row(res)) != NULL){
        credit_card =  *row;
    }
    mysql_close(con);

    credit_card = decrypt(key,iv,credit_card);

    //cout << "String length: " << credit_card.length() << endl;
    //cout << "Last 4 digits: " << credit_card.substr(credit_card.length()-4,4) << endl;

    if(credit_card.length() == 16)
    {
        credit_card = "************" +  credit_card.substr(credit_card.length()-4,4);
    }
    else{
        credit_card = "***********" +  credit_card.substr(credit_card.length()-4,4);
    }

    return credit_card;
}



/*
    Operaciones CarritoCompras
    -Agregar al carrito, sumar una existencia en caso de que exista, si no existe agregar un producto con ese nombre
    -Existence_Product_Shop_Car
    -Sacar del carrito
    -Listado de todos los prodcutos agregados al carrito, cantidad y precio (ya esta hecho, es el obtain_string_tabledb)
*/


//verifica la existencia de un producto en el carrito de compras asociado a un user del sistema, mediante su correo y nombre de un producto
bool verify_existence_product_shop_car(string product_name, string mail)
{
    bool exist = false;
    con = mysql_connection_setup(mysqlD);
    string query = "SELECT EXISTS(SELECT * FROM CarritoCompras WHERE nombreProducto =\"" + product_name + "\" && correo = \"" + mail + "\");";
    res = mysql_perform_query(con, query.c_str());
    while ((row = mysql_fetch_row(res)) != NULL){
        exist = atoi(*row);
    }
    mysql_close(con);
    return exist;
}

//Obtiene el numero de existencias de un producto que se desean a comprar en el carrito de compras por un cliente
int product_quantity_in_shop_car_by_email(string product_name, string mail)
{
    con = mysql_connection_setup(mysqlD);
    string query = "SELECT cantidad FROM CarritoCompras WHERE nombreProducto =\"" + product_name + "\" && correo = \"" + mail + "\";";
    res = mysql_perform_query(con, query.c_str());
    while ((row = mysql_fetch_row(res)) != NULL){
        return stol(*row);
    }
    mysql_close(con);
}

//Retorna 1 si hizo nuevo agregado, 2 si hizo update, 0 si fallo la operacion de agregar al shop car
int add_to_shop_car(string product_name,int cantidad, string mail)
{
    int state_result = 0;
    
    bool exist = false;
    exist = verify_existence_product_shop_car(product_name,mail);
    
    int existence_shop_car = 0;
    existence_shop_car = product_quantity_in_shop_car_by_email(product_name, mail);

    int stock = view_article_existences(product_name);

    con = mysql_connection_setup(mysqlD);
    string query = "";
    if(!exist && stock >= cantidad && cantidad != 0)
    {
        query = "INSERT INTO CarritoCompras(cantidad,nombreProducto,correo) Values(" + to_string(cantidad) + ",\"" + product_name + "\", \"" + mail + "\");";
        mysql_perform_query(con, query.c_str());
        state_result = 1;
    }
    else
    {
        if(stock >= cantidad && cantidad != 0)
        {
            query = "UPDATE CarritoCompras SET cantidad =" + to_string(cantidad) + " WHERE nombreProducto = \"" + product_name + "\" && correo = \"" + mail + "\";";
            mysql_perform_query(con, query.c_str());
            state_result = 2;
        }
    }
    //close database connection
    mysql_close(con);

    return state_result;
}

//
bool delete_from_shop_car(string product_name, string mail)
{
    bool exist = verify_existence_product_shop_car(product_name,mail);
    bool succesful_delete = false;
    con = mysql_connection_setup(mysqlD);
    if(exist){
        string query = "DELETE FROM CarritoCompras WHERE nombreProducto = \"" + product_name + "\" && correo = \"" + mail + "\";";
        mysql_perform_query(con, query.c_str());
        succesful_delete = true;
    }
    mysql_close(con);
    return succesful_delete;
}