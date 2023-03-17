// g++ main.cpp -o output -L/usr/include/mariadb/mysql -lmariadbclient
#include <iostream>
#include </usr/include/mysql/mysql.h>
#include <cstring>
#include "cardNumberVerify.h"

using namespace std;

struct connection_details
{
    const char *server, *user, *password, *database;
};

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

//Datos de inicio de sesión
struct connection_details mysqlD = {"localhost", "root", "eqsgon2015!","Sitio_Ventas"};

int column_number_returner(string table_name_parameter)
{
    // connect to the mysql database
    con = mysql_connection_setup(mysqlD);
    int *ptrColumns = &columns;

    string queryColumnsNumber = "SELECT count(*) AS NUMBEROFCOLUMNS FROM information_schema.columns WHERE table_name = '";
    string finalQuery = queryColumnsNumber + table_name_parameter;
    //Consulta #1
    numberColumns = mysql_perform_query(con, (finalQuery.append("';")).c_str());
    while ((row = mysql_fetch_row(numberColumns)) != NULL){
        // the below row[] parametes may change depending on the size of the table and your objective
        //save number of columns
        *ptrColumns = *row[0] - '0';

    }
    //cout << "Number of columns: " << *numberRows << endl;
    mysql_free_result(numberColumns);
    

    mysql_close(con);

    return columns;

}

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


int create_matrix_splitted(string table_name, int colum_number, int row_number, string *ptrMatrix)
{
    int j = 0;
    string articulos_lista = obtain_string_tabledb(table_name, column_number_returner(table_name));
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


*/
//Insertar articulo en tabla cuando un usuario quiere vender algo
int insert_product(string nombre, double precio, int numero_existencias)
{
    
    con = mysql_connection_setup(mysqlD);

    string query = "INSERT INTO Articulo(nombre,precio,numeroExistencias) Values(\"" + nombre + "\"," + to_string(precio) + ","+ to_string(numero_existencias) + ");";
    mysql_perform_query(con, query.c_str());
    
    // close database connection
    mysql_close(con);
    return 0;
}


//Verificar si un articulo existe en la base de datos, retorna el número de articulos disponibles del mismo
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


//Realiza update de productos después de una compra menor al stock disponible o realiza delete en caso de una compra que deje 0 productos
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



/*

Operaciones de Cuenta
    -USER_EXISTENCE
    -ACCOUNT_EXISTENCE
    -RETURN_USER_MAIL


*/

//Retorna true o false, según sea el caso de la existencia del usuario
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

//Verifica si coincide el username y el password con uno almacenado en la base de datos
bool verify_account_existence(string username,string password)
{
    bool exist = false;
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

//retorna el mail asociado a un nombre de usuario
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

//retorna 1 si la cuenta se pudo crear y 0 si la cuenta no se pudo crear por la ya existencia de una cuenta con el mismo usuario
int create_new_account(string username, string password, string mail)
{
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
        string query = "INSERT INTO PersonaUsuario(correo,primerApellido,segundoApellido, nombre,telefono,direccion) Values(\"" + mail + "\",\"" + primerApellido + "\", \"" + segundoApellido + "\",\"" + nombre + "\"," + to_string(telefono) + ",\"" + direccion + "\");";
        mysql_perform_query(con, query.c_str());
        mysql_close(con);
         
        create_new_account(username,password,mail);
    }

    return created;
}



/*

Operaciones de Metodo de Pago
    -INSERT_CREDIT_CARD


*/

//Retorna 1 si pudo insertar la tarjeta debido al tener un numero valido, retorna 0 si el numero de tarjeta es invalido y no pudo realizar la insercion de la tarjeta
int insert_credit_card(long credit_card, string password, string date, string mail_asociated)
{
    int insertion_result = 0;
    con = mysql_connection_setup(mysqlD);

    string query = "INSERT INTO MetodoPago(numeroTarjeta,contrasena,fechaVencimiento, correo_metodoPago) Values(" + to_string(credit_card) + ",\"" + password + "\", \"" + date + "\",\"" + mail_asociated + "\");";
    if(isValid(credit_card))
    {
        mysql_perform_query(con, query.c_str());
        insertion_result = 1;
    }
    //close database connection
    mysql_close(con);
    
    return insertion_result;
}



